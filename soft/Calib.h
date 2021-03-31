#ifndef CalibrUH
#define CalibrUH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <vcl\Registry.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ActnList.hpp>
#include <Buttons.hpp>
#include <Graphics.hpp>
#include <Dialogs.hpp>

#include "FloatEdit.hpp"

#define TITLETEXT "Калибровка - " //Заголовок окна

#define MAX_INDEX 74   //max. imageindex for pages (to disable controls)

#define mpOk    1      //Ok TabSheet
#define mpInfo  2      //Info TabSheet
#define mpError 3      //Error TabSheet

//---------------------------------------------------------------------------

class TCalibForm : public TForm
{
__published:	// IDE-managed Components
  TTreeView *TreeView;
  TPanel *RightPanel;
  TButton *btnBack;
  TButton *btnNext;
  TButton *btnClose;
  TActionList *Actions;
  TAction *BackAction;
  TAction *NextAction;
  TAction *CloseAction;
  TOpenDialog *OpenDialog;
  TSaveDialog *SaveDialog;
  TPanel *SplitPanel;
  TPanel *TitlePanel;
  TPageControl *PageControl;
  TTabSheet *tabCalib;
  TLabel *lbCalib;
  TTabSheet *tabLoad;
  TLabel *lbLoad4;
  TEdit *edLoadFileName;
  TTabSheet *tabSave;
  TLabel *lbSave1;
  TLabel *lbSave3;
  TEdit *edSaveFileName;
  TTabSheet *tabPerform;
  TLabel *lbPerform2;
  TButton *btnSkip;
  TAction *SkipAction;
  TTabSheet *tabOk;
  TLabel *lbOk;
  TTabSheet *tabInfo;
  TLabel *lbNo;
  TTabSheet *tabError;
  TLabel *lbEr;
  TImage *Image1;
  TImage *Image3;
  TTabSheet *tab42;
  TTabSheet *tab51;
  TTabSheet *tab52;
  TTabSheet *tab61;
  TTabSheet *tab62;
  TTabSheet *tab70;
  TTabSheet *tab71;
  TTabSheet *tab50;
  TTabSheet *tab60;
  TSpeedButton *btnLoadBrowse;
  TSpeedButton *btnSaveBrowse;
  TLabel *Label51b;
  TLabel *Label52b;
  TLabel *Label42a;
  TLabel *Label51a;
  TLabel *Label62b;
  TLabel *labTitle;
  TTabSheet *tab43;
  TLabel *Label43b;
  TTabSheet *tab53;
  TLabel *Label53b;
  TTabSheet *tab54;
  TLabel *Label54a;
  TLabel *Label54b;
  TLabel *lbPerform1;
  TLabel *lbLoad1;
  TGroupBox *EdBox;
  TLabel *lbStep;
  TFloatEdit *edVal;
  TLabel *lbUnit;
  TLabel *Label1;
  TImage *Image2;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label5;
  TLabel *Label6;
  TLabel *Label7;
  TLabel *Label8;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall NextActionExecute(TObject *Sender);
	void __fastcall BackActionExecute(TObject *Sender);
	void __fastcall TreeViewChange(TObject *Sender, TTreeNode *Node);
	void __fastcall TreeViewKeyDown(TObject *Sender, WORD &Key,
        TShiftState Shift);
	void __fastcall btnLoadBrowseClick(TObject *Sender);
  void __fastcall ConvertKfc(const AnsiString FileName);
	void __fastcall btnSaveBrowseClick(TObject *Sender);
  void __fastcall edValEnterPress(TObject *Sender);
  void __fastcall edValChanged(TObject *Sender);
  void __fastcall edLoadFileNameKeyDown(TObject *Sender, WORD &Key,
        TShiftState Shift);
  void __fastcall edSaveFileNameKeyDown(TObject *Sender, WORD &Key,
        TShiftState Shift);
  void __fastcall SkipActionExecute(TObject *Sender);
  void __fastcall TreeViewDblClick(TObject *Sender);
  void __fastcall TreeViewCollapsing(TObject *Sender,
          TTreeNode *Node, bool &AllowCollapse);
  void __fastcall cb71Click(TObject *Sender);
private:	// User declarations
  double C1, C2; //точки, заданные в кодах
  double R1, R2; //точки, полученные по старым калибровочным коэффициентам
  double E1, E2; //введенные точки
  double k;      //масштабирующий множитель для редактируемой величины
  TTabSheet* __fastcall GetTabByImgIndex(int ImageIndex);
  bool __fastcall IsYNew(double Y1, double Y2, double Z1, double Z2);
  void __fastcall MessPage(int n); //выбор закладки с сообщением
  int FPoint; //номер пункта калибровки (= ImageIndex)
  int FStep;  //шаг: 0, 1, ... -1 - done, -2 - done (not executable)
  bool FModified; //калибровочные коэффициенты изменены
  void __fastcall ControlsDE(int p, int s); //разрешение/запрещение controls
  //реализация процесса калибровки:
  void __fastcall DoCalib(int p, int &s);
  //реализация отдельных пунктов калибровки:
  void __fastcall DoCalNoEx(int &s);
  void __fastcall DoCalLoad(int &s);
  void __fastcall DoCalSave(int &s);
  void __fastcall DoCalDo(int &s);

  void __fastcall DoCalDacV(int &s, int range);
  AnsiString OldCalibName;
public:		// User declarations
	__fastcall TCalibForm(TComponent* Owner);
};

//---------------------------------------------------------------------------

extern PACKAGE TCalibForm *CalibForm;

//---------------------------------------------------------------------------

#endif
