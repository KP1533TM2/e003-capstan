#include <vcl.h>
#include <fstream> 
#include <iostream.h>
#include <math.h>
#include <dir.h>
#pragma hdrstop

#include "Main.h"
#include "Connect.h"
#include "About.h"
#include "Colours.h"
#include "Settings.h"

#pragma package(smart_init)
#pragma link "FloatEdit"
#pragma link "ThemeMgr"
#pragma link "FloatEdit"
#pragma resource "*.dfm"
TMainForm *MainForm;

//---------------------------------------------------------------------------

__fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner)
{
}

//-------------------------- Создание формы: --------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{ int i;
  //общие параметры:
  Caption = MakeCaption();
  DecimalSeparator = '.';            //десятичный разделитель
  FFileName = "";
  DragAcceptFiles(Handle, 1);        //разрешение drag-n-drop
  //установка DoubleBuffered для компонентов:
  MainForm->DoubleBuffered = 1;
  UnderLogPanel->DoubleBuffered = 1;
  LogPanel->DoubleBuffered = 1;
  CtrlPanel->DoubleBuffered = 1;
  TrsPanel->DoubleBuffered = 1;
  ToolBar->DoubleBuffered = 1;
  TlPanel->DoubleBuffered = 1;
  StatusBar->DoubleBuffered = 1;
  //настройка связи с устройством:
  WakePort = new TWakePort();
  acDisconnectExecute(NULL);
  //инициализация переменных:
  Bm = new Graphics::TBitmap;
  FCursor = 0;
  FCursorX = 0;    //обнуление координаты курсора
  SetDefColours(); //установка цветов по умолчанию
  FModified = 0;
  FMode = 0;
  FRef = 0;
  //чтение конфигурации:
  Lock = 1;
  ReadConfig();
  Lock = 0;
  //текущий режим:
  FGMode = MD_STOP;                   //запись графика остановлена
  Disconnect();
  Clear();
  ClearScale();
  Timer1->Interval = FSample;
  Timer1->Enabled = 1;                //разрешение таймера
}

//----------------------- Запрос закрытия формы: ----------------------------

void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	int Ans = IDNO;
  if(FModified)
   Ans = MessageDlg("Data modified. Save changes?", mtConfirmation,
                    TMsgDlgButtons() << mbYes << mbNo << mbCancel, 0);
  if(Ans == IDYES) acSaveasExecute(NULL);
  if(Ans == IDCANCEL) CanClose = 0;
}

//-------------------------- Закрытие формы: --------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  SaveConfig();
  Disconnect();
  delete Bm;
}

//------------------ Принятие файла по Drag-n-drop: -------------------------

void __fastcall TMainForm::WmDropFiles(TWMDropFiles &Message)
{
	HDROP drop_handle = (HDROP)Message.Drop;
	char fName[MAXPATH];
	DragQueryFile(drop_handle, 0, fName, MAXPATH);
	ReadData(fName);
	DragFinish(drop_handle);
}

//-------------------- Формирование заголовка окна: -------------------------

AnsiString __fastcall TMainForm::MakeCaption(void)
{
  DecimalSeparator = '.';
  AnsiString s = SOFTNAME;
  s = s + " " + FloatToStrF(VERSION, ffFixed, 3, 2);
  return(s);
}

//-------------------- Установка цветов по умолчанию: -----------------------

void __fastcall TMainForm::SetDefColours(void)
{
  Col_Bg = (TColor)16777215; //RGB(255,255,255);
  Col_Ax = (TColor)13948116; //RGB(172,172,172);
  Col_Tx = (TColor)4737096;  //RGB(0,0,0);
  Col_Cr = (TColor)0;        //RGB(0,0,0);
  Col_G1 = (TColor)32768;    //RGB(255,0,0);
  Col_G2 = (TColor)255;      //RGB(0,128,0);
  Col_G3 = (TColor)4227327;  //RGB(0,0,255);
  Col_G4 = (TColor)16711680; //RGB(255,92,92);
  Col_G5 = (TColor)4259584;  //RGB(0,255,64);
  Col_G6 = (TColor)12615935; //RGB(255,128,192);
}

//------------------------- Чтение INI-файла: -------------------------------

void __fastcall TMainForm::ReadConfig(void)
{
  TMemIniFile *ini;
  ini = new TMemIniFile(ChangeFileExt(Application->ExeName,".ini" ));
  Left = ini->ReadInteger("General","Left",Left);
  Top = ini->ReadInteger("General","Top",Top);
  Width = ini->ReadInteger("General","Width",Width);
  Height = ini->ReadInteger("General","Height",Height);
  WindowState = TWindowState(ini->ReadInteger("General", "WindowState", WindowState));
  FDataDir = ini->ReadString("General","DataDir",ExtractFilePath(ParamStr(0)));
  FPortNumber = ini->ReadInteger("General","Port",0);
  cbGr1->Checked = ini->ReadBool("General","ShowG1",1);
  cbGr2->Checked = ini->ReadBool("General","ShowG2",1);
  cbGr3->Checked = ini->ReadBool("General","ShowG3",0);
  cbGr4->Checked = ini->ReadBool("General","ShowG4",0);
  cbGr5->Checked = ini->ReadBool("General","ShowG5",0);
  cbGr6->Checked = ini->ReadBool("General","ShowG6",0);
  FAutoConnect = ini->ReadBool("Settings","AutoConnect",1);
  FAutoLoad = ini->ReadBool("Settings","AutoLoad",1);
  FSample = ini->ReadInteger("Settings","Sample",20);
  FAutoScroll = ini->ReadBool("Settings","AutoScroll",1);
  Col_Bg = (TColor)ini->ReadInteger("Colours","Background",Col_Bg);
  Col_Ax = (TColor)ini->ReadInteger("Colours","Axis",Col_Ax);
  Col_Tx = (TColor)ini->ReadInteger("Colours","Text",Col_Tx);
  Col_Cr = (TColor)ini->ReadInteger("Colours","Cursor",Col_Cr);
  Col_G1 = (TColor)ini->ReadInteger("Colours","Graph1",Col_G1);
  Col_G2 = (TColor)ini->ReadInteger("Colours","Graph2",Col_G2);
  Col_G3 = (TColor)ini->ReadInteger("Colours","Graph3",Col_G3);
  Col_G4 = (TColor)ini->ReadInteger("Colours","Graph4",Col_G4);
  Col_G5 = (TColor)ini->ReadInteger("Colours","Graph5",Col_G5);
  Col_G6 = (TColor)ini->ReadInteger("Colours","Graph6",Col_G6);
  feDia->Value = ini->ReadFloat("Params","Diameter",7.8);
  feTape->Value = ini->ReadFloat("Params","Thickness",35.0);
  feSlip->Value = ini->ReadFloat("Params","Slip",0.1);
  fePoles->Value = ini->ReadInteger("Params","Poles",174);
  delete ini;
}

//----------------------- Сохранение INI-файла: -----------------------------

void __fastcall TMainForm::SaveConfig(void)
{
  TMemIniFile *ini;
  ini = new TMemIniFile(ChangeFileExt(Application->ExeName,".ini" ));
  ini->WriteInteger("General", "WindowState", WindowState);
  if (WindowState != wsMaximized)
   { ini->WriteInteger("General","Left",Left);
     ini->WriteInteger("General","Top",Top);
     ini->WriteInteger("General","Width",Width);
     ini->WriteInteger("General","Height",Height);
   }
  ini->WriteString("General","DataDir",FDataDir);
  ini->WriteInteger("General","Port",FPortNumber);
  ini->WriteBool("General","ShowG1",cbGr1->Checked);
  ini->WriteBool("General","ShowG2",cbGr2->Checked);
  ini->WriteBool("General","ShowG3",cbGr3->Checked);
  ini->WriteBool("General","ShowG4",cbGr4->Checked);
  ini->WriteBool("General","ShowG5",cbGr5->Checked);
  ini->WriteBool("General","ShowG6",cbGr6->Checked);
  ini->WriteBool("Settings","AutoConnect",FAutoConnect);
  ini->WriteBool("Settings","AutoRead",FAutoLoad);
  ini->WriteInteger("Settings","Sample",FSample);
  ini->WriteBool("Settings","AutoScroll",FAutoScroll);
  ini->WriteInteger("Colours","Background",Col_Bg);
  ini->WriteInteger("Colours","Axis",Col_Ax);
  ini->WriteInteger("Colours","Text",Col_Tx);
  ini->WriteInteger("Colours","Cursor",Col_Cr);
  ini->WriteInteger("Colours","Graph1",Col_G1);
  ini->WriteInteger("Colours","Graph2",Col_G2);
  ini->WriteInteger("Colours","Graph3",Col_G3);
  ini->WriteInteger("Colours","Graph4",Col_G4);
  ini->WriteInteger("Colours","Graph5",Col_G5);
  ini->WriteInteger("Colours","Graph6",Col_G6);
  ini->WriteFloat("Params","Diameter",feDia->Value);
  ini->WriteFloat("Params","Thickness",feTape->Value);
  ini->WriteFloat("Params","Slip",feSlip->Value);
  ini->WriteInteger("Params","Poles",fePoles->Value);
  try { ini->UpdateFile(); } catch (Exception &E)
   { MessageDlg(E.Message, mtError, TMsgDlgButtons() << mbOK, 0); }
  delete ini;
}

//------------------------ Чтение файла данных: -----------------------------

void __fastcall TMainForm::ReadData(AnsiString Name)
{
  //сохранение пути и имени файла:
  FDataDir = ExtractFilePath(Name);
  FFileName = ExtractFileName(Name);
  bool result = 1;
  TStringList *in = new TStringList();
  TStringList *line = new TStringList();
  TCursor Save_Cursor = Screen->Cursor;
  try
  {
    Screen->Cursor = crHourGlass;
    try
    {
      //чтение файла в StringList:
      in->LoadFromFile(Name);
      //если первая строка файла не содержит текста "Time",
      //то ошибка формата файла:
      if(!in->Strings[0].AnsiPos("Time"))
        throw EInOutError("Bad file format");
      acStopExecute(NULL); //остановка накопления массивов
      Clear(); //очистка массивов
      ClearScale(); //очистка шкал
      //восстановление набора графиков, сохраненных в файле:
      cbGr1->Checked = in->Strings[0].AnsiPos("G1");
      cbGr2->Checked = in->Strings[0].AnsiPos("G2");
      cbGr3->Checked = in->Strings[0].AnsiPos("G3");
      cbGr4->Checked = in->Strings[0].AnsiPos("G4");
      cbGr5->Checked = in->Strings[0].AnsiPos("G5");
      cbGr6->Checked = in->Strings[0].AnsiPos("G6");
      //подготовка к парсингу строк:
      line->Delimiter = '\t';
      for(int i = 1; i < in->Count; i++)
      {
        //построчная загрузка файла во второй StringList
        //в виде DelimitedText:
        line->DelimitedText = in->Strings[i];
        int j = 1; //первая колонка - время, пропускаем
        //FSample берем из первой колонки второй строки:
        if(i == 2) FSample = StrToInt(line->Strings[0]);
        //преобразуем данные колонок в int и сохраняем в массивах:
        Data1[FPointer] = StrToFloat(line->Strings[j++]);
        Data2[FPointer] = StrToFloat(line->Strings[j++]);
        Data3[FPointer] = StrToFloat(line->Strings[j++]);
        Data4[FPointer] = StrToFloat(line->Strings[j++]);
        Data5[FPointer] = StrToFloat(line->Strings[j++]);
        if(line->Count > j)
          Data6[FPointer] = StrToFloat(line->Strings[j]);
        FPointer++;
      }
    }
    catch (Exception &E)
    {
      MessageDlg("Bad file format.", mtError, TMsgDlgButtons() << mbOK, 0);
      result = 0;
    }
  }
  __finally
  {
    Screen->Cursor = Save_Cursor;
    delete in;
    delete line;
    //перерисовка графиков:
    RedrawAll();
  }
  if(result) //если файл был открыт успешно,
  {
    //обновление заголовка приложения:
    Caption = MakeCaption() + " - " + FFileName;
  }
}

//---------------------- Сохранение файла данных: --------------------------

void __fastcall TMainForm::SaveData(AnsiString Name)
{
  //сохранение пути и имени файла:
  FDataDir = ExtractFilePath(Name);
  FFileName = ExtractFileName(Name);
  //сохранение данных в файл:
  TStringList *out = new TStringList();
  TCursor Save_Cursor = Screen->Cursor;
  bool not_saved = 0;
  try
  {
    Screen->Cursor = crHourGlass;
    //формирование строки заголовка:
    AnsiString str = "Time";
    if(cbGr1->Checked) str = str + "\t" + "G1";
    if(cbGr2->Checked) str = str + "\t" + "G2";
    if(cbGr3->Checked) str = str + "\t" + "G3";
    if(cbGr4->Checked) str = str + "\t" + "G4";
    if(cbGr5->Checked) str = str + "\t" + "G5";
    if(cbGr6->Checked) str = str + "\t" + "G6";
    out->Add(str);
    //сохранение массивов:
    for(int i = 0; i < FPointer; i++)
    {
      str = IntToStr(i * FSample);
      str = str + "\t" + FloatToStr(Data1[i]);
      str = str + "\t" + FloatToStr(Data2[i]);
      str = str + "\t" + FloatToStr(Data3[i]);
      str = str + "\t" + FloatToStr(Data4[i]);
      str = str + "\t" + FloatToStr(Data5[i]);
      str = str + "\t" + FloatToStr(Data6[i]);
      out->Add(str);
    }
    try
    {
      out->SaveToFile(Name);
    }
    catch (Exception &E)
    {
      MessageDlg("Unable to save file.", mtError, TMsgDlgButtons() << mbOK, 0);
      not_saved = FModified;
    }
  }
  __finally
  {
    Screen->Cursor = Save_Cursor;
    delete out;
  }
  //обновление заголовка приложения:
  Caption = MakeCaption() + " - " + FFileName;
  FModified = not_saved;
}

//---------------------------------------------------------------------------
//------------------------------ Menus: -------------------------------------
//---------------------------------------------------------------------------

//----------------------------- Menu New: -----------------------------------

void __fastcall TMainForm::acNewExecute(TObject *Sender)
{
  Clear();
  RedrawAll();
  FFileName = "";
  Caption = MakeCaption();
  FModified = 0;
}

//----------------------------- Menu Open: ----------------------------------

void __fastcall TMainForm::acOpenExecute(TObject *Sender)
{
  //установка сепаратора, иначе после "засыпания" Windows
  //восстанавливается системный по умолчанию
  DecimalSeparator = '.';
  //заполнение полей диалога:
  OpenDialog->Title = "Open Data File";
  OpenDialog->InitialDir = FDataDir;
  OpenDialog->FileName = "";
  OpenDialog->Filter = "Data file (*.dat)|*.dat|All files (*.*)|*.*";
  OpenDialog->DefaultExt = "dat";
  //вызов диалога "Open...":
  if(!OpenDialog->Execute()) return;
  //перерисовка формы под окном диалога:
  Refresh();
  //чтение файла:
  ReadData(OpenDialog->FileName);
}

//--------------------------- Menu Save As: ---------------------------------

void __fastcall TMainForm::acSaveasExecute(TObject *Sender)
{
  //установка сепаратора, иначе после "засыпания" Windows
  //восстанавливается системный по умолчанию
  DecimalSeparator = '.';
  //заполнение полей диалога:
  SaveDialog->Title = "Save Data File";
  SaveDialog->InitialDir = FDataDir;
  SaveDialog->FileName = FFileName;
  SaveDialog->Filter = "Data file (*.dat)|*.dat|All files (*.*)|*.*";
  SaveDialog->DefaultExt = "dat";
  //вызов диалога "Save As...":
  if(!SaveDialog->Execute()) return;
  //перерисовка формы под окном диалога:
  Refresh();
  //сохранение файла:
  SaveData(SaveDialog->FileName);
}

//---------------------------- Menu Exit: -----------------------------------

void __fastcall TMainForm::acExitExecute(TObject *Sender)
{
  Close();
}

//----------------------------- Connect: ------------------------------------

void __fastcall TMainForm::acConnectExecute(TObject *Sender)
{
  if(!ConnectForm) ConnectForm = new TConnectForm(Application, &FPortNumber);
  if(ConnectForm->ShowModal() == mrOk)
  {
    Timer1->Enabled = 0;
    Disconnect();
    Connect();
    Timer1->Enabled = 1;
  }
}

void __fastcall TMainForm::Connect(void)
{
  try
  {
    WakePort->Open(FPortNumber);
    FPortName = WakePort->PortName;
    FDeviceName = WakePort->GetInfo(0);
    AnsiString name = DEVICE_NAME;
    FConnected = FDeviceName.SubString(1, name.Length()) == name;
    if(!FConnected) throw EInOutError("Unknown device");
    StatusBar->Panels->Items[0]->Text = " ON LINE: " +
      FDeviceName + " (" + FPortName + ") ";
    //автоматическое чтение параметров:
    if(FAutoLoad) acReadExecute(NULL);
  }
  catch(const EInOutError &e)
  {
    Disconnect();
  }
}

//---------------------------- Disconnect: ----------------------------------

void __fastcall TMainForm::acDisconnectExecute(TObject *Sender)
{
  Timer1->Enabled = 0;
  FPortNumber = 0;
  Disconnect();
}

void __fastcall TMainForm::Disconnect(void)
{
  WakePort->Close();
  FConnected = 0;
  FPortName = "";
  FDeviceName = "";
  if(FPortNumber) StatusBar->Panels->Items[0]->Text = " CONNECTING... ";
    else StatusBar->Panels->Items[0]->Text = " OFF LINE ";
}

//---------------------------- Read Params: ---------------------------------

void __fastcall TMainForm::acReadExecute(TObject *Sender)
{
  TCursor Save_Cursor = Screen->Cursor;
  try
  {
    Screen->Cursor = crHourGlass;
    if(FConnected)
    {
      GetParams();
    }
  }
  __finally { Screen->Cursor = Save_Cursor; }
}

//--------------------------- Write Params: ---------------------------------

void __fastcall TMainForm::acWriteExecute(TObject *Sender)
{
  TCursor Save_Cursor = Screen->Cursor;
  try
  {
    Screen->Cursor = crHourGlass;
    if(FConnected)
    {
      SetParams();
    }
  }
  __finally { Screen->Cursor = Save_Cursor; }
}

//---------------------------- Open Params: ---------------------------------

void __fastcall TMainForm::acParOpenExecute(TObject *Sender)
{
  //установка сепаратора, иначе после "засыпания" Windows
  //восстанавливается системный по умолчанию
  DecimalSeparator = '.';
  //заполнение полей диалога:
  OpenDialog->Title = "Open Params File";
  OpenDialog->InitialDir = FDataDir;
  OpenDialog->FileName = "";
  OpenDialog->Filter = "Params file (*.par)|*.par|All files (*.*)|*.*";
  OpenDialog->DefaultExt = "par";
  //вызов диалога "Open Params...":
  if(!OpenDialog->Execute()) return;
  //сохранение пути:
  FDataDir = ExtractFilePath(OpenDialog->FileName);
  //чтение файла параметров:
  TMemIniFile *ini;
  ini = new TMemIniFile(OpenDialog->FileName);
  Lock = 1;
  feP->Value = ini->ReadInteger("DPLL", "Kp", 100);
  feI->Value = ini->ReadInteger("DPLL", "Ki", 40);
  feD->Value = ini->ReadInteger("DPLL", "Kd", 45);
  Lock = 0;
  delete ini;
}

//--------------------------- Save Params: ----------------------------------

void __fastcall TMainForm::acParSaveExecute(TObject *Sender)
{
  //установка сепаратора, иначе после "засыпания" Windows
  //восстанавливается системный по умолчанию
  DecimalSeparator = '.';
  //заполнение полей диалога:
  SaveDialog->Title = "Save Params File";
  SaveDialog->InitialDir = FDataDir;
  SaveDialog->FileName = "Params1";
  SaveDialog->Filter = "Params file (*.par)|*.par|All files (*.*)|*.*";
  SaveDialog->DefaultExt = "par";
  //вызов диалога "Save Params...":
  if(!SaveDialog->Execute()) return;
  //сохранение пути:
  FDataDir = ExtractFilePath(SaveDialog->FileName);
  //сохранение файла параметров:
  TMemIniFile *ini;
  ini = new TMemIniFile(SaveDialog->FileName);
  ini->WriteInteger("DPLL", "Kp", feP->Value);
  ini->WriteInteger("DPLL", "Ki", feI->Value);
  ini->WriteInteger("DPLL", "Kd", feD->Value);
  try { ini->UpdateFile(); } catch (Exception &E)
   { MessageDlg(E.Message, mtError, TMsgDlgButtons() << mbOK, 0); }
  delete ini;
}

//---------------------------- Menu Start: ----------------------------------

void __fastcall TMainForm::acStartExecute(TObject *Sender)
{
  if(!acStart->Checked)
  {
    acStart->Checked = 1;
    acPause->Checked = 0;
    acStop->Checked = 0;
    if(FGMode == MD_STOP) acNewExecute(NULL);
    FGMode = MD_START;
    FModified = 1;
  }
  else
  {
    acStart->Checked = 0;
    acPause->Checked = 1;
    acStop->Checked = 0;
    FGMode = MD_PAUSE;
  }
}

//---------------------------- Menu Pause: ----------------------------------

void __fastcall TMainForm::acPauseExecute(TObject *Sender)
{
  if(acStop->Checked) return;
  if(!acPause->Checked)
  {
    acPause->Checked = 1;
    acStart->Checked = 0;
    acStop->Checked = 0;
    FGMode = MD_PAUSE;
  }
  else
  {
    acPause->Checked = 0;
    acStart->Checked = 1;
    acStop->Checked = 0;
    FGMode = MD_START;
  }
}

//----------------------------- Menu Stop: ----------------------------------

void __fastcall TMainForm::acStopExecute(TObject *Sender)
{
  acStop->Checked = 0;
  acStart->Checked = 0;
  acPause->Checked = 0;
  FGMode = MD_STOP;
}

//----------------------------- Menu Freeze: --------------------------------

void __fastcall TMainForm::acFreezeExecute(TObject *Sender)
{
  acFreeze->Checked = !acFreeze->Checked;
  if(!acFreeze->Checked) RedrawAll();
}

//---------------------------- Menu Test Data: ------------------------------

void __fastcall TMainForm::acTestExecute(TObject *Sender)
{
  double arg = 0;
  for(int i = 0; i < LOGN; i++)
  {
    double s = 1 + sin(arg);
    Data1[i] = 800 + 80 * s;
    Data2[i] = 600 + 70 * s;
    Data3[i] = 100 * s;
    Data4[i] = 200 * s;
    Data5[i] = 300 * s;
    Data6[i] = 400 * s;
    arg += 0.1;
  }
  FPointer = LOGN - 1;
  LogScrollBar->Position = 0;
  DrawLog(LogPaintBox->Canvas, 0, FPointer);
  RedrawAll();
}

//---------------------------- Menu Cursor: ---------------------------------

void __fastcall TMainForm::acCursorExecute(TObject *Sender)
{
  acCursor->Checked = !acCursor->Checked;
  FCursor = acCursor->Checked;
  DrawLogCur(LogPaintBox->Canvas, FCursor, FCursorX);
}

//----------------------------- Cut Left: -----------------------------------

void __fastcall TMainForm::acCutLExecute(TObject *Sender)
{
  if(FCursor)
  {
    int Xo = LogScrollBar->Position + FCursorX;
    if(Xo < FPointer)
    {
      for(int i = 0; i < FPointer - Xo; i++)
      {
        Data1[i] = Data1[i + Xo]; Data1[i + Xo] = 0;
        Data2[i] = Data2[i + Xo]; Data2[i + Xo] = 0;
        Data3[i] = Data3[i + Xo]; Data3[i + Xo] = 0;
        Data4[i] = Data4[i + Xo]; Data4[i + Xo] = 0;
        Data5[i] = Data5[i + Xo]; Data5[i + Xo] = 0;
        Data6[i] = Data6[i + Xo]; Data6[i + Xo] = 0;
      }
      FPointer = FPointer - Xo;
      FCursorX = 0;
      RedrawAll();
      LogScrollBar->Position = 0;
      FModified = 1;
    }
  }
}

//---------------------------- Cut Right: -----------------------------------

void __fastcall TMainForm::acCutRExecute(TObject *Sender)
{
  if(FCursor)
  {
    int Xo = LogScrollBar->Position + FCursorX;
    if(Xo < FPointer)
    {
      for(int i = Xo + 1; i < FPointer; i++)
      {
        Data1[i] = 0;
        Data2[i] = 0;
        Data3[i] = 0;
        Data4[i] = 0;
        Data5[i] = 0;
        Data6[i] = 0;
      }
      FPointer = Xo + 1;
      RedrawAll();
      FModified = 1;
    }
  }
}

//---------------------------- Menu Settings: -------------------------------

void __fastcall TMainForm::acSettingsExecute(TObject *Sender)
{
  if(!SettingsForm) SettingsForm = new TSettingsForm(Application);
  SettingsForm->ShowModal();
  Timer1->Interval = FSample;
  //автоматическая загрузка параметров:
  if(FAutoLoad) acReadExecute(NULL);
  RedrawAll();
}

//---------------------------- Menu Colours: --------------------------------

void __fastcall TMainForm::acColoursExecute(TObject *Sender)
{
  if(!ColoursForm) ColoursForm = new TColoursForm(Application);
  ColoursForm->ShowModal();
}

//---------------------------- Menu About: ----------------------------------

void __fastcall TMainForm::acAboutExecute(TObject *Sender)
{
  if(!AboutForm) AboutForm = new TAboutForm(Application);
  AboutForm->ShowModal();
}

//-------------------------------- PWM: -------------------------------------
//--------------------------- Вывод графиков: -------------------------------
//---------------------------------------------------------------------------

#define SCALE  100 //шкала по оси Y
#define X_STEP  50 //шаг сетки по оси X
#define Y_DIV   20 //количество делений по оси Y

#define GAPL    32 //зазор слева от сетки
#define GAPR     4 //зазор справа от сетки
#define GAPT    22 //зазор сверху от сетки
#define GAPB    22 //зазор снизу от сетки

//---------------------------- Вывод сетки: ---------------------------------

void __fastcall TMainForm::DrawLogGrid(TCanvas *cv)
{
  //задание цветов:
  TrsPanel->Color = Col_Bg;
  lbTraces->Font->Color = Col_Tx;
  lbGr1->Font->Color = Col_G1;
  lbGr2->Font->Color = Col_G2;
  lbGr3->Font->Color = Col_G3;
  lbGr4->Font->Color = Col_G4;
  lbGr5->Font->Color = Col_G5;
  lbGr6->Font->Color = Col_G6;
  //вычисление диапазонов:
  int DeltaX = LogPaintBox->Width - GAPL - GAPR;
  int DeltaY = LogPaintBox->Height - GAPB - GAPT;
  LogScrollBar->Max = LOGN - DeltaX;
  int Xo = LogScrollBar->Position;
  //очистка:
  cv->Pen->Mode = pmCopy;
  cv->Brush->Color = Col_Bg;
  cv->FillRect(Rect(0, 0, LogPaintBox->Width, LogPaintBox->Height));
  //рисование вертикальных линий сетки:
  cv->Pen->Color = Col_Ax;
  cv->Font->Color = Col_Tx;
  int i = - Xo % X_STEP; if(i < 0) i += X_STEP;
  cv->MoveTo(GAPL, GAPT);
  cv->LineTo(GAPL, GAPT + DeltaY);
  cv->MoveTo(GAPL + DeltaX - 1, GAPT);
  cv->LineTo(GAPL + DeltaX - 1, GAPT + DeltaY);
  AnsiString txt;
  while(i < DeltaX)
  {
    cv->MoveTo(GAPL + i, GAPT);
    cv->LineTo(GAPL + i, GAPT + DeltaY + 3);
    txt = IntToStr((Xo + i) * FSample / 1000);
    int x = GAPL + i - cv->TextWidth(txt) / 2;
    cv->TextOutA(x, GAPT + DeltaY + 5, txt);
    i += X_STEP;
  }
  //cv->TextOutA(GAPL + DeltaX + 14, GAPT + DeltaY + 5, "s");
  //рисование горизонтальных линий сетки:
  cv->Font->Color = Col_Tx;
  txt = "0";
  int h = cv->TextHeight(txt) / 2;
  for(int i = 0; i <= Y_DIV; i++)
  {
    int y = GAPT + (i * (double)DeltaY / Y_DIV + 0.5);
    cv->MoveTo(GAPL - 3, y);
    cv->LineTo(GAPL + DeltaX, y);
    //вывод текста по Y:
    y = y - h;
    txt = IntToStr((Y_DIV - i) * SCALE / Y_DIV);
    int x = GAPL - cv->TextWidth(txt) - 7;
    cv->TextOutA(x, y, txt);
  }
  FCurOn = 0;
  DrawLogCur(cv, FCursor, FCursorX);
}

//--------------- Рисование графика в указанном диапазоне: ------------------

#define SCALE1 65535.0 //Speed
#define SCALE2 65535.0 //Ref. Speed
#define SCALE3 65535.0 //Phase Error (TODO: shift)
#define SCALE4 65535.0 //PWM
#define SCALE5 65535.0
#define SCALE6 65535.0

#define RNG1 (SCALE / SCALE1)
#define RNG2 (SCALE / SCALE2)
#define RNG3 (SCALE / SCALE3)
#define RNG4 (SCALE / SCALE4)
#define RNG5 (SCALE / SCALE5)
#define RNG6 (SCALE / SCALE6)

#define SHR1 (SCALE1 / (SCALE * 10))
#define SHR2 (SCALE2 / (SCALE * 10))
#define SHR3 (SCALE3 / (SCALE * 10))
#define SHR4 (SCALE4 / (SCALE * 10))
#define SHR5 (SCALE5 / (SCALE * 10))
#define SHR6 (SCALE6 / (SCALE * 10))

void __fastcall TMainForm::DrawLog(TCanvas *cv, int x1, int x2)
{
  //вычисление диапазонов:
  int DeltaX = LogPaintBox->Width - GAPL - GAPR;
  int DeltaY = LogPaintBox->Height - GAPB - GAPT;
  int Xo = LogScrollBar->Position;
  x1 = x1 - Xo; x2 = x2 - Xo;
  if(x1 < 0) x1 = 0;
  if(x2 > DeltaX - 1) x2 = DeltaX - 1;
  //выключение курсора:
  DrawLogCur(cv, 0, FCursorX);
  //рисование графиков:
  cv->Pen->Mode = pmCopy;
  for(int i = x1; i <= x2; i++)
  {
    if(cbGr1->Checked)
      DrawSegment(cv, Col_G1, Data1, Scale[0] * RNG1, Shift[0] * SHR1, Xo, i, DeltaY);
    if(cbGr2->Checked)
      DrawSegment(cv, Col_G2, Data2, Scale[1] * RNG2, Shift[1] * SHR2, Xo, i, DeltaY);
    if(cbGr3->Checked)
      DrawSegment(cv, Col_G3, Data3, Scale[2] * RNG3, Shift[2] * SHR3, Xo, i, DeltaY);
    if(cbGr4->Checked)
      DrawSegment(cv, Col_G4, Data4, Scale[3] * RNG4, Shift[3] * SHR4, Xo, i, DeltaY);
    if(cbGr5->Checked)
      DrawSegment(cv, Col_G5, Data5, Scale[4] * RNG5, Shift[4] * SHR5, Xo, i, DeltaY);
    if(cbGr6->Checked)
      DrawSegment(cv, Col_G6, Data6, Scale[5] * RNG6, Shift[5] * SHR6, Xo, i, DeltaY);
  }
  //включение курсора:
  DrawLogCur(cv, FCursor, FCursorX);
}

//--------------------- Рисование сегмента графика: -------------------------

void __fastcall TMainForm::DrawSegment(TCanvas *cv, TColor cl, double *data,
                             double range, double shift, int x0, int x, int dy)
{
  cv->Pen->Color = cl;
  //double k = double(dy) / range;
  double k = double(dy) / 100.0 * range;
  int Y2 = int(k * (data[x0 + x] + shift) + 0.5);
  int Y1 = int(k * (data[x0 + x - ((x)? 1 : 0)] + shift) + 0.5);
  if(Y1 > dy) Y1 = dy;
  if(Y2 > dy) Y2 = dy;
  if(Y1 < 0) Y1 = 0;
  if(Y2 < 0) Y2 = 0;
  if(Y2 < Y1) Y2 = Y2 - 1; else Y2 = Y2 + 1;
  cv->MoveTo(GAPL + x, GAPT + dy - Y1);
  cv->LineTo(GAPL + x, GAPT + dy - Y2);
}

//------------------------- Рисование курсора: ------------------------------

void __fastcall TMainForm::DrawLogCur(TCanvas *cv, bool e, int x)
{
  //вычисление диапазонов:
  int DeltaX = LogPaintBox->Width - GAPL - GAPR;
  int DeltaY = LogPaintBox->Height - GAPB - GAPT;
  int Xo = LogScrollBar->Position;
  if (x < 0) x = 0;
  if (x > DeltaX - 1) x = DeltaX - 1;
  cv->Pen->Mode = pmNotXor;
  if(FCurOn)
  {
    //стирание курсора:
    cv->Pen->Color = (Col_Cr ^ ~Col_Bg) & 0xFFFFFF;
    cv->MoveTo(GAPL + FCursorX, GAPT + DeltaY);
    cv->LineTo(GAPL + FCursorX, GAPT - 1);
  }
  if(e)
  {
    int j = x + Xo;
    //рисование курсора:
    cv->Pen->Color = (Col_Cr ^ ~Col_Bg) & 0xFFFFFF;
    cv->MoveTo(GAPL + x, GAPT + DeltaY);
    cv->LineTo(GAPL + x, GAPT - 1);
    //вывод координат Y:
    DecimalSeparator = '.';
    if(cbGr1->Checked)
      StatusBar->Panels->Items[1]->Text =
       "Speed = " + FloatToStrF(Data1[j] / 10,  ffFixed, 5, 1) + " Hz";
      else StatusBar->Panels->Items[1]->Text = "";
    if(cbGr2->Checked)
      StatusBar->Panels->Items[2]->Text =
       "Ref = " + FloatToStrF(Data2[j] / 10,  ffFixed, 5, 1) + " Hz";
      else StatusBar->Panels->Items[2]->Text = "";
    if(cbGr3->Checked)
      StatusBar->Panels->Items[3]->Text =
       "Phase = " + FloatToStrF(CodeToPhase(Data3[j]), ffFixed, 5, 2) + "°";
      else StatusBar->Panels->Items[3]->Text = "";
    if(cbGr4->Checked)
      StatusBar->Panels->Items[4]->Text =
       "PWM = " + FloatToStrF(Data4[j],  ffFixed, 5, 0);
      else StatusBar->Panels->Items[4]->Text = "";
    if(cbGr5->Checked)
      StatusBar->Panels->Items[5]->Text =
       "Aux1 = " + FloatToStrF(Data5[j],  ffFixed, 5, 0);
      else StatusBar->Panels->Items[5]->Text = "";
    if(cbGr6->Checked)
      StatusBar->Panels->Items[6]->Text =
       "Aux2 = " + FloatToStrF(Data6[j],  ffFixed, 5, 0);
      else StatusBar->Panels->Items[6]->Text = "";
    //вывод координаты X:
    StatusBar->Panels->Items[7]->Text = " t = " +
     FloatToStrF(FSample * (j) / 1000.0, ffFixed, 5, 1) + " s ";
  }
  else
  {
    //очистка координат, если курсор выключен:
    StatusBar->Panels->Items[1]->Text = "";
    StatusBar->Panels->Items[2]->Text = "";
    StatusBar->Panels->Items[3]->Text = "";
    StatusBar->Panels->Items[4]->Text = "";
    StatusBar->Panels->Items[5]->Text = "";
    StatusBar->Panels->Items[6]->Text = "";
    StatusBar->Panels->Items[7]->Text = "";
  }
  FCurOn = e;
  FCursorX = x;
}

//-------------------- Включение/отключение графиков: -----------------------

void __fastcall TMainForm::cbGr1Click(TObject *Sender)
{
  RedrawAll();
}

//--------------------------- Скроллирование: -------------------------------

void __fastcall TMainForm::LogScrollBarChange(TObject *Sender)
{
  RedrawAll();
}

//------------------------ Перерисовка графика: -----------------------------

void __fastcall TMainForm::LogPaintBoxPaint(TObject *Sender)
{
  RedrawAll();
}

//---------------------------- Cursor On: -----------------------------------

void __fastcall TMainForm::LogPaintBoxMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(Shift.Contains(ssLeft))
  {
    acCursor->Checked = 1;
    FCursor = 1;
    DrawLogCur(LogPaintBox->Canvas, FCursor, X - GAPL);
  }
}

//--------------------------- Cursor Move: ----------------------------------

void __fastcall TMainForm::LogPaintBoxMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  if (Shift.Contains(ssLeft))
  {
    //автоматическое скроллирование:
    if(FAutoScroll)
    {
      if(X > LogPaintBox->Width - GAPR)
        LogScrollBar->Position += X - LogPaintBox->Width + GAPR;
      if(X < GAPL)
        LogScrollBar->Position -= GAPL - X;
    }
    DrawLogCur(LogPaintBox->Canvas, FCursor, X - GAPL);
  }
}

//-------------------------- Очистка данных: --------------------------------

void __fastcall TMainForm::Clear(void)
{
  for(int i = 0; i < LOGN; i++)
  {
    Data1[i] = 0;
    Data2[i] = 0;
    Data3[i] = 0;
    Data4[i] = 0;
    Data5[i] = 0;
    Data6[i] = 0;
  }
  LogScrollBar->Position = 0;
  FPointer = 0;
}

//-------------------------- Очистка шкал: ----------------------------------

void __fastcall TMainForm::ClearScale(void)
{
  for(int i = 0; i < GRAPHS; i++)
  {
    Scale[i] = 1;
    BaseShift[i] = 0;
    Shift[i] = 0;
  }
  int index = cbGrs->ItemIndex;
  feScale->Value = Scale[index];
  feShift->Value = Shift[index] / 10.0;
  sbShift->Position = 0;
  sbScale->Position = 990;
}

//------------------------ Перерисовка всего: -------------------------------

void __fastcall TMainForm::RedrawAll(void)
{
  Bm->Height = LogPaintBox->Height;
  Bm->Width = LogPaintBox->Width;
  DrawLogGrid(Bm->Canvas);
  if(FPointer) DrawLog(Bm->Canvas, 0, FPointer - 1);
  LogPaintBox->Canvas->Draw(0, 0, Bm);
  //int index = cbGrs->ItemIndex;
  sbShift->Max = 1000;//LogPaintBox->Height * 2;
  sbShift->Min = -1000; //-LogPaintBox->Height * 2;
}

//-------------------------- Изменение шкалы: -------------------------------

void __fastcall TMainForm::feScaleChanged(TObject *Sender)
{
  if(!Lock)
  {
    Lock = 1;
    int index = cbGrs->ItemIndex;
    Scale[index] = feScale->Value;
    sbScale->Position = sbScale->Max - Scale[index] * 10;
    RedrawAll();
    Lock = 0;
  }
}

void __fastcall TMainForm::sbScaleChange(TObject *Sender)
{
  if(!Lock)
  {
    Lock = 1;
    int index = cbGrs->ItemIndex;
    Scale[index] = (sbScale->Max - sbScale->Position) / 10.0;
    feScale->Value = Scale[index];
    RedrawAll();
    Lock = 0;
  }
}

//------------------------- Изменение смещения: -----------------------------

void __fastcall TMainForm::feShiftChanged(TObject *Sender)
{
  if(!Lock)
  {
    Lock = 1;
    int index = cbGrs->ItemIndex;
    BaseShift[index] = feShift->Value * 10.0;
    Shift[index] = BaseShift[index];
    sbShift->Position = 0;
    RedrawAll();
    Lock = 0;
  }
}

void __fastcall TMainForm::sbShiftChange(TObject *Sender)
{
  if(!Lock)
  {
    Lock = 1;
    int index = cbGrs->ItemIndex;
    Shift[index] = BaseShift[index] - sbShift->Position;
    feShift->Value = Shift[index] / 10.0;
    RedrawAll();
    Lock = 0;
  }
}

//----------------------- Сброс шкалы и смещения: ---------------------------

void __fastcall TMainForm::btResClick(TObject *Sender)
{
  Lock = 1;
  int index = cbGrs->ItemIndex;
  Scale[index] = 1;
  feScale->Value = 1;
  sbScale->Position = sbScale->Max - 10;
  BaseShift[index] = 0;
  Shift[index] = 0;
  feShift->Value = 0;
  sbShift->Position = 0;
  RedrawAll();
  DrawLog(LogPaintBox->Canvas, 0, FPointer);
  Lock = 0;
}

//--------------------------- Выбор графика: --------------------------------

void __fastcall TMainForm::cbGrsChange(TObject *Sender)
{
  Lock = 1;
  int index = cbGrs->ItemIndex;
  feScale->Value = Scale[index];
  feShift->Value = Shift[index] / 10.0;
  sbShift->Position = BaseShift[index] - Shift[index];
  sbScale->Position = sbScale->Max - Scale[index] * 10;
  Lock = 0;
}

void __fastcall TMainForm::lbGr1Click(TObject *Sender)
{
  cbGrs->ItemIndex = ((TSpeedButton*)Sender)->Tag;
  cbGrsChange(NULL);
}

//---------------------------------------------------------------------------
//----------------------- Вспомогательные методы: ---------------------------
//---------------------------------------------------------------------------

//--------------------- Преобразование кода в проценты: ---------------------

double __fastcall TMainForm::Code2Percent(int c)
{
  return((double)c * 100.0 / UINT16_MAX);
}

//-------------------- Преобразование процентов в код: ----------------------

int __fastcall TMainForm::Percent2Code(double p)
{
  return(int(p * UINT16_MAX / 100.0 + 0.5));
}

//------------------ Преобразование кода фазы в градусы: --------------------

double __fastcall TMainForm::CodeToPhase(int p)
{
  double deg_per_unit = 360.0 / UINT16_MAX;
  return((p - 32768) * deg_per_unit);
}

//------------------------ Управление светодиодами: -------------------------

void __fastcall TMainForm::SetLeds(int s)
{
  TColor ColOff = clGray;
  if(s & S_RUN)
  {
    shStop->Brush->Color = ColOff;
    TColor ColOn = (s & S_LOCK)? clLime : clRed;
    if(s & S_REV)
    {
      shFwd->Brush->Color = ColOff;
      shRev->Brush->Color = ColOn;
    }
    else
    {
      shFwd->Brush->Color = ColOn;
      shRev->Brush->Color = ColOff;
    }
  }
  else
  {
    shFwd->Brush->Color = ColOff;
    shRev->Brush->Color = ColOff;
    shStop->Brush->Color = clYellow;
  }
}

//-------------------------- Чтение состояния: ------------------------------

void __fastcall TMainForm::GetState(void)
{
  WakePort->GetState(State);
  Lock = 1;
  FMode = State.s;
  SetLeds(FMode);
  if(!cbDir->DroppedDown) cbDir->ItemIndex = (FMode & S_REV)? 1 : 0;
  if(!cbSpeed->DroppedDown)
  {
    if(FMode & S_PRS1)
     cbSpeed->ItemIndex = (FMode & S_PRS0)?  3 : 2;
       else cbSpeed->ItemIndex = (FMode & S_PRS0)?  1 : 0;
  }
  cbSep->Checked = FMode & S_SEP;
  cbLock->Checked = FMode & S_LOCK;
  if(!cbManual->Checked) fePwm->Value = Code2Percent(State.p);
  feCurSpd->Value = State.v / 10.0;
  fePhase->Value = CodeToPhase(State.e);
  Lock = 0;
  if(FRef != State.r)
  {
    FRef = State.r;
    feFreq->Value = FRef / 10.0;
  }
}

//------------------------ Передача параметров: -----------------------------

void __fastcall TMainForm::SetParams(void)
{
  params_t p;
  p.en = !cbManual->Checked;
  p.kp = feP->Value;
  p.ki = feI->Value;
  p.kd = feD->Value;
  WakePort->SetParams(p);
}

//------------------------- Чтение параметров: ------------------------------

void __fastcall TMainForm::GetParams(void)
{
  params_t p;
  WakePort->GetParams(p);
  Lock = 1;
  cbManual->Checked = !p.en;
  feP->Value = p.kp;
  feI->Value = p.ki;
  feD->Value = p.kd;
  Lock = 0;
}

//---------------------------------------------------------------------------
//---------------------------- Управление: ----------------------------------
//---------------------------------------------------------------------------

//--------------------------- Motor Control: --------------------------------

//Кнопки управления:

void __fastcall TMainForm::sbStopClick(TObject *Sender)
{
  int m = ((TSpeedButton*)Sender)->Tag;
  switch(m)
  {
  case 0: FMode = FMode & ~S_RUN; break;
  case 1: FMode = FMode & ~S_REV | S_RUN; break;
  case 2: FMode = FMode | S_REV | S_RUN; break;
  }
  WakePort->SetMode(FMode);
}

//------------------------------- Preset: -----------------------------------

//Выбор направления:

void __fastcall TMainForm::cbDirChange(TObject *Sender)
{
  if(!Lock)
  {
    if(cbDir->ItemIndex) FMode = FMode | S_REV;
      else FMode = FMode & ~S_REV;
    WakePort->SetMode(FMode);
  }
}

//Выбор номинальной скорости:

void __fastcall TMainForm::cbSpeedChange(TObject *Sender)
{
  if(!Lock)
  {
    FMode = FMode & ~(S_PRS0 | S_PRS1);
    switch(cbSpeed->ItemIndex)
    {
    case 1: FMode = FMode | S_PRS0; break;
    case 2: FMode = FMode | S_PRS1; break;
    case 3: FMode = FMode | S_PRS0 | S_PRS1; break;
    };
    WakePort->SetMode(FMode);
    if(FConnected) GetState();
  }
}

//Режим регулировки скорости separate/joint:

void __fastcall TMainForm::cbSepClick(TObject *Sender)
{
  if(!Lock)
  {
    if(cbSep->Checked) FMode = FMode | S_SEP;
      else FMode = FMode & ~(S_SEP);
    WakePort->SetMode(FMode);
  }
}

//-------------------------------- Speed: -----------------------------------

//Изменение параметров:

void __fastcall TMainForm::feSpeedChanged(TObject *Sender)
{
  if(!Lock)
  {
    Lock = 1;
    feEDia->Value = feDia->Value + feTape->Value / 1000.0;
    double L = feEDia->Value * M_PI;
    double Fcap = feSpeed->Value * 10.0 / L;
    Fcap = Fcap * (1 + feSlip->Value / 100.0);
    feFreq->Value = Fcap * fePoles->Value;
    WakePort->SetSpeed(feFreq->Value * 10.0 + 0.5);
    Lock = 0;
  }
}

//Изменение частоты:

void __fastcall TMainForm::feFreqChanged(TObject *Sender)
{
  if(!Lock)
  {
    Lock = 1;
    feEDia->Value = feDia->Value + feTape->Value / 1000.0;
    double L = feEDia->Value * M_PI;
    double Fcap = feFreq->Value / fePoles->Value;
    feSpeed->Value = Fcap * L / 10.0;
    feSpeed->Value = feSpeed->Value / (1 + feSlip->Value / 100.0);
    WakePort->SetSpeed(feFreq->Value * 10.0 + 0.5);
    Lock = 0;
  }
}

void __fastcall TMainForm::udFreqChangingEx(TObject *Sender,
      bool &AllowChange, short NewValue, TUpDownDirection Direction)
{
  AllowChange = 0;
  if(Direction == updUp) feFreq->Value += 0.1;
    else if(Direction == updDown) feFreq->Value -= 0.1;
}

//Включение прямого ввода частоты:

void __fastcall TMainForm::cbDirectClick(TObject *Sender)
{
  feSpeed->Enabled = !cbDirect->Checked;
  feDia->Enabled = !cbDirect->Checked;
  feTape->Enabled = !cbDirect->Checked;
  feSlip->Enabled = !cbDirect->Checked;
  fePoles->Enabled = !cbDirect->Checked;
  feFreq->Enabled = cbDirect->Checked;
  udFreq->Enabled = cbDirect->Checked;
}

//------------------------------- Loop: -------------------------------------

//Коэффициенты PID:

void __fastcall TMainForm::fePChanged(TObject *Sender)
{
  if(!Lock)
  {
    SetParams();
  }
}

//Разрешение DPLL:

void __fastcall TMainForm::cbManualClick(TObject *Sender)
{
  if(!Lock)
  {
    fePwm->Enabled = cbManual->Checked;
    feP->Enabled = !cbManual->Checked;
    feI->Enabled = !cbManual->Checked;
    feD->Enabled = !cbManual->Checked;
    SetParams();
  }
}


//------------------------------ Motor: -------------------------------------

//Управление PWM:

void __fastcall TMainForm::fePwmChanged(TObject *Sender)
{
  if(!Lock)
  {
    WakePort->SetPWM(Percent2Code(fePwm->Value));
  }
}

//---------------------- Save Params to EEPROM: -----------------------------

void __fastcall TMainForm::btSaveClick(TObject *Sender)
{
  acWriteExecute(NULL);
  WakePort->EESave();
}

//----------------------- Load Default Values: -------------------------------

void __fastcall TMainForm::btDefaultClick(TObject *Sender)
{
  int Ans = MessageDlg("Are you sure to load the default values?",
              mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0);
  if(Ans == IDYES)
  {
    WakePort->Default();
    acReadExecute(NULL);
    acWriteExecute(NULL);
  }
}

//---------------------------------------------------------------------------
//------------------------------ Timer: -------------------------------------
//---------------------------------------------------------------------------

void __fastcall TMainForm::Timer1Timer(TObject *Sender)
{
  if(FConnected) //если устройство подключено
  {
    try
    {
      GetState();
      //отрисовка графиков:
      if(FGMode == MD_START)
      {
        if(FPointer < LOGN)
        {
          //добавление точки:
          Data1[FPointer] = State.v;
          Data2[FPointer] = State.r;
          Data3[FPointer] = State.e;
          Data4[FPointer] = State.p;
          Data5[FPointer] = 0;
          Data6[FPointer] = 0;
          //рисование точки:
          if(!acFreeze->Checked)
            DrawLog(LogPaintBox->Canvas, FPointer, FPointer);
          FPointer++;
          //автоматическое скроллирование:
          if(FAutoScroll && !acFreeze->Checked &&
               FPointer - LogScrollBar->Position >
                 LogPaintBox->Width - GAPL - GAPR)
            LogScrollBar->Position += 100;
        }
        else
        {
          //если конец массивов, стоп:
          acStopExecute(NULL);
        }
      }
    }
    catch(const EInOutError &e) { Disconnect(); }
  }
  else
  {
    //автоматическое подключение:
    if(FAutoConnect && FPortNumber) Connect();
  }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

