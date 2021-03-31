#include <vcl.h>
#include <fstream>
#include <iostream.h>
#pragma hdrstop

#include "math.h"

#include "Calib.h"

#pragma package(smart_init)
#pragma link "FloatEdit"
#pragma resource "*.dfm"

TCalibForm *CalibForm;

//---------------------------------------------------------------------------

__fastcall TCalibForm::TCalibForm(TComponent* Owner)
  :	TForm(Owner)
{ }

//------------------------- Создание формы ----------------------------------

void __fastcall TCalibForm::FormCreate(TObject *Sender)
{
  FModified = 0;
  //Off(); //выключение выхода
  TreeView->Selected = TreeView->Items->Item[3]; //вызов TreeViewChange
  TreeView->Items->Item[3]->Expand(0);
}

//------------------------  Запрос закрытия формы ---------------------------

void __fastcall TCalibForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  if(FModified)
    switch(Application->MessageBox("Сохранить новые коэффициенты?",
           "Калибровка завершена", MB_YESNOCANCEL | MB_ICONQUESTION))
    {
      case IDYES : { FModified = 0; /*SaveCalib*/ break; }
      case IDNO  : { FModified = 0; /*ReadCalib*/ break; }
      default    : CanClose = 0;
    }
  //if(ModalResult == mrCancel)
    //ReadCalib;
}

//-------------------------- Закрытие формы ---------------------------------

void __fastcall TCalibForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  //Off(); //выключение выхода
  Action    = caFree;
  CalibForm = NULL;
}

//---------------------------------------------------------------------------
//------------------- Управление процессом калибровки -----------------------
//---------------------------------------------------------------------------

//------------------------ Кнопка "Вернуться" -------------------------------

void __fastcall TCalibForm::BackActionExecute(TObject *Sender)
{
  if((FStep == -1) || (FStep == 2))
  {
    TTabSheet* tab = GetTabByImgIndex(FPoint);
    if(tab != NULL) PageControl->ActivePage = tab;
    FStep = 0; DoCalib(FPoint, FStep);
  }
  else
  {
    int i = TreeView->Selected->AbsoluteIndex - 1;
    if(i >= 0)
    TreeView->Selected = TreeView->Items->Item[i];
  }
}

//------------------------- Кнопка "Дальше" ---------------------------------

void __fastcall TCalibForm::NextActionExecute(TObject *Sender)
{
//  if(FStep < 0)
  {
    int i = TreeView->Selected->AbsoluteIndex + 1;
    if(i < TreeView->Items->Count)
      TreeView->Selected = TreeView->Items->Item[i];
  }
//  else
//   DoCalib(FPoint, FStep);
}

//------------------------ Кнопка "Пропустить" ------------------------------

void __fastcall TCalibForm::SkipActionExecute(TObject *Sender)
{
  int i = TreeView->Selected->AbsoluteIndex + 1;
  if(TreeView->Selected->HasChildren && TreeView->Selected->ImageIndex != 30)
    i = i + 2;
  if(i < TreeView->Items->Count)
    TreeView->Selected = TreeView->Items->Item[i];
}

//-------------------------- Обработка Edit ---------------------------------

//On Changed:

void __fastcall TCalibForm::edValChanged(TObject *Sender)
{
  if(FStep == 1) E1 = edVal->Value; else E2 = edVal->Value;
}

//On Enter Press:

void __fastcall TCalibForm::edValEnterPress(TObject *Sender)
{
  if(FStep < 0)
  {
    int i = TreeView->Selected->AbsoluteIndex + 1;
    if(i < TreeView->Items->Count)
      TreeView->Selected = TreeView->Items->Item[i];
  }
  else DoCalib(FPoint, FStep);
}

//-------- Переход к другому пункту калибровки с помощью TreeList -----------

void __fastcall TCalibForm::TreeViewChange(TObject *Sender, TTreeNode *Node)
{
/*
  TTabSheet* tab = GetTabByImgIndex(Node->ImageIndex); //Поиск страницы
  if(tab != NULL)
  {
    PageControl->ActivePage = tab;
    labTitle->Caption = PageControl->ActivePage->Caption;
    FPoint = Node->ImageIndex; FStep = 0;
    DoCalib(FPoint, FStep);
  }
*/
}

//-------------- Обработка нажатия Enter в TreeList -------------------------

void __fastcall TCalibForm::TreeViewKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
  if(Key == VK_RETURN)
  {
    int i = TreeView->Selected->AbsoluteIndex + 1;
    if(i < TreeView->Items->Count)
      TreeView->Selected = TreeView->Items->Item[i];
  }
}

//-------------- Обработка нажатия мыши в TreeList --------------------------

void __fastcall TCalibForm::TreeViewDblClick(TObject *Sender)
{
  TTreeNode *Node;
  Node = TreeView->Selected;
  if(Node) TreeViewChange(TreeView, Node);
}

//-------------------- Запрещение сворачивания дерева: ----------------------

void __fastcall TCalibForm::TreeViewCollapsing(TObject *Sender,
      TTreeNode *Node, bool &AllowCollapse)
{
  if(Node->AbsoluteIndex == 0) AllowCollapse = 0;
}

//---------------- Вывод страницы TabSHeet с сообщением ---------------------

void __fastcall TCalibForm::MessPage(int n)
{
  TTabSheet* tab = GetTabByImgIndex(n); //Поиск страницы n
  if(tab != NULL)
  {
    PageControl->ActivePage = tab;
    if(n == mpError) MessageBeep(MB_ICONEXCLAMATION);
  }
  if(n == mpOk) FModified = 1;
  //Caption = TITLETEXT + ExtractFileName(Equip->CalibName) +
  //                                            (FModified? " - изменен":"");
}

//--------- Возвращает страницу с указанным значением ImageIndex ------------

//Если такой страницы не найдено, возвращает NULL

TTabSheet* __fastcall TCalibForm::GetTabByImgIndex(int ImageIndex)
{
  for(int i = 0; i < PageControl->PageCount; i++)
  {
    TTabSheet* page = PageControl->Pages[i];
    if (page->ImageIndex == ImageIndex)
    return(page);
  }
  return NULL;
}

//------- Проверка измеренных при калибровке значений на обновление: --------

bool __fastcall TCalibForm::IsYNew(double Y1, double Y2, double Z1, double Z2)
{
  //if(Equip->DevType == NONE) return(0);
  //return(!((fabs(Y1 - Z1) < PRECISION) && (fabs(Y2 - Z2) < PRECISION)));
  return(0);
}

//---------------------------------------------------------------------------
//---------------- Выполнение пунктов и шагов калибровки --------------------
//---------------------------------------------------------------------------

void __fastcall TCalibForm::DoCalib(int p, int &s)
{
  ControlsDE(p, s); //разрешение/запрещение Controls
  switch (p)
  {
  //Общие пункты:
  case 0:   DoCalNoEx(s);    break;
  case 10:  DoCalLoad(s);    break; //Загрузить
  case 20:  DoCalSave(s);    break; //Сохранить
  case 30:  DoCalNoEx(s);    break; //Выполнить
  //Voltage DAC:
  case 40:  DoCalNoEx(s);    break;
  case 41:  DoCalDacV(s, 0); break; //point 1
  case 42:  DoCalDacV(s, 1); break; //point 2
  //Current DAC:
  case 50:  DoCalNoEx(s);    break;
  case 51:  DoCalDacV(s, 0); break; //point 1
  case 52:  DoCalDacV(s, 1); break; //point 2
  //Voltage ADC:
  case 60:  DoCalNoEx(s);    break;
  case 61:  DoCalDacV(s, 0); break; //point 1
  case 62:  DoCalDacV(s, 1); break; //point 2
  //Current ADC:
  case 70:  DoCalNoEx(s);    break;
  case 71:  DoCalDacV(s, 0); break; //point 1
  case 72:  DoCalDacV(s, 1); break; //point 2
  }
}

//------------------- Разрешение/запрещение Controls ------------------------

void __fastcall TCalibForm::ControlsDE(int p, int s)
{
  btnBack->Enabled = p > 0;
  btnSkip->Enabled = (p < MAX_INDEX);
  btnNext->Enabled = (p < MAX_INDEX) || (s < 1);
  if(s == 0)
  {
    EdBox->Visible = 0;
    edVal->ReadOnly = 0;
    edVal->ValueVisible = 1;
  }
}

//---------------------------------------------------------------------------
//---------------- Обработка всех неисполняемых пунктов ---------------------
//---------------------------------------------------------------------------

void __fastcall TCalibForm::DoCalNoEx(int &s)
{
  //Equip->Off(); //выключение всех устройств
  if(s == 0) s = -2;
}

//---------------------------------------------------------------------------
//-------------- Выполнение пункта "Загрузка калибровки" --------------------
//---------------------------------------------------------------------------

void __fastcall TCalibForm::DoCalLoad(int &s)
{
  if(s == 0)
  {
    s = -2;
    //edLoadFileName->Text = Equip->CalibName;
    edLoadFileName->SelStart = edLoadFileName->Text.Length();
  }
}

//----------------- Обработка нажатия кнопки "Open": ------------------------

void __fastcall TCalibForm::btnLoadBrowseClick(TObject *Sender)
{
  //OpenDialog->InitialDir = ExtractFilePath(ParamStr(0)) + CALIB_DIR;
  OpenDialog->FileName = ExtractFileName(edLoadFileName->Text);
  OpenDialog->Filter =
    "Файлы калибровки (*.cal)|*.cal|Файлы калибровки АПК-2 (*.kfc)|*.kfc";
  OpenDialog->DefaultExt = "cal";
  if(OpenDialog->Execute())
  {
    edLoadFileName->Text = OpenDialog->FileName;
    edLoadFileName->SelStart = edLoadFileName->Text.Length();
    if(ExtractFileExt(OpenDialog->FileName) == ".kfc")
      ConvertKfc(edLoadFileName->Text);
        //else Equip->ReadCalibF(edLoadFileName->Text);
    FModified = 0;
    //Caption = TITLETEXT + ExtractFileName(Equip->CalibName);
  }
}

//------------------- Преобразование KFC в CAL: -----------------------------

void __fastcall TCalibForm::ConvertKfc(const AnsiString FileName)
{
//
}

//-------------------- Обработка нажатия ENTER: -----------------------------

void __fastcall TCalibForm::edLoadFileNameKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
  if(Key == VK_RETURN) btnLoadBrowse->OnClick(NULL);
}

//------------------ Использовать встроенные приборы: ----------------------

void __fastcall TCalibForm::cb71Click(TObject *Sender)
{
  EdBox->Visible = !dynamic_cast<TCheckBox *>(Sender)->Checked;
}

//---------------------------------------------------------------------------
//--------------- Выполнение пункта "Сохранение калибровки" -----------------
//---------------------------------------------------------------------------

void __fastcall TCalibForm::DoCalSave(int &s)
{
  if(s == 0)
  {
    s = -2;
    //edSaveFileName->Text = Equip->CalibName;
    edSaveFileName->SelStart = edSaveFileName->Text.Length();
  }
}

//------------------ Обработка нажатия кнопки "Save": -----------------------

void __fastcall TCalibForm::btnSaveBrowseClick(TObject *Sender)
{
  //SaveDialog->InitialDir = ExtractFilePath(ParamStr(0)) + CALIB_DIR;
  SaveDialog->FileName = ExtractFileName(edSaveFileName->Text);
  SaveDialog->Filter = "Файлы калибровки (*.cal)|*.cal";
  SaveDialog->DefaultExt = "cal";
  if(SaveDialog->Execute())
  {
    edSaveFileName->Text = SaveDialog->FileName;
    edSaveFileName->SelStart = edSaveFileName->Text.Length();
    //Equip->SaveCalibF(edSaveFileName->Text);
    FModified = 0; //Caption = TITLETEXT + ExtractFileName(Equip->CalibName);
  }
}

//---------------------- Обработка нажатия ENTER: ---------------------------

void __fastcall TCalibForm::edSaveFileNameKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
  if(Key == VK_RETURN) btnSaveBrowse->OnClick(NULL);
}

//---------------------------------------------------------------------------
//---------------------- Выполнение пункта "DAC V" --------------------------
//---------------------------------------------------------------------------

void __fastcall TCalibForm::DoCalDacV(int &s, int range)
{
  k = 1000.0;                           //перевод точек в мВ
  lbUnit->Caption = "V";                //единицы измерения - мВ
  //шаг 1:
  if(range == 0)
  {
    s++;
    lbStep->Caption = "Point 1:";
    EdBox->Visible = 1;                 //показываем поле ввода
    edVal->ValueVisible = 0; Repaint(); //убираем значение
    Screen->Cursor = crHourGlass;
    { Screen->Cursor = crDefault; }
    E1 = R1 = 0; //Dcv->Value * k;    //старое значение точки 1
    edVal->Value = E1;                  //выводим старое значение
    edVal->ValueVisible = 1;            //делаем значение видимым
    edVal->SetFocus();
    return;
  }
  //шаг 2:
  if(range == 1)
  {
    s++;
    lbStep->Caption = "Point 2:";
    EdBox->Visible = 1;                 //показываем поле ввода
    EdBox->SetFocus();
    edVal->ValueVisible = 0; Repaint(); //убираем значение
    Screen->Cursor = crHourGlass;
    { Screen->Cursor = crDefault; }
    C1 = 0; //Dcv->Code;              //новый код для точки 1
    E2 = R2 = 0; //Dcv->Value * k;    //старое значение точки 2
    edVal->Value = E2;                  //выводим старое значение
    edVal->ValueVisible = 1;            //делаем значение видимым
    edVal->SetFocus();
    return;
  }
  //вычисление результата калибровки:
  if(s == 2)
  {
    s = -1;
    EdBox->Visible = 0;                 //убираем поле ввода
    if(btnNext->Enabled) btnNext->SetFocus();
    //Screen->Cursor = crHourGlass;
    //try { DcvStart();            //запускаем измерение точки 2
    //} __finally { Screen->Cursor = crDefault; }
    C2 = 0; //Dcv->Code;              //новый код для точки 2
    if(!IsYNew(E1, E2, R1, R2))         //проверяем обновление точек
    {
      MessPage(mpInfo);                 //если точки не поменялись - сообщение
    }
    else
    {
      //if(Dcv->Points2Coeff(E1 / k, E2 / k, C1, C2))
        MessPage(mpOk);                 //сообщение об успешном завершении
          //else MessPage(mpError);       //сообщение об ошибке
    }
    //Off();                       //выключение оборудования
    return;
  }
}

//---------------------------------------------------------------------------


