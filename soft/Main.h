#ifndef PhCnt_MainH
#define PhCnt_MainH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>

#include <vcl\Registry.hpp>
#include <Buttons.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <Dialogs.hpp>
#include <Grids.hpp>

#include "ThemeMgr.hpp"
#include "WakePort.h"
#include "FloatEdit.hpp"

#define SOFTNAME "Motor"
#define VERSION 1.00

#define LOGN   18001 //максимальное количество точек графика
#define ROUND(x) ((x > 0)? (x + 0.5) : (x - 0.5))
#define GRAPHS 6

//---------------------------------------------------------------------------

class TMainForm : public TForm
{
__published:	// IDE-managed Components
  TMainMenu *MainMenu;
  TMenuItem *mnFile;
  TMenuItem *mnNew;
  TMenuItem *mnOpen;
  TMenuItem *mnSaveAs;
  TMenuItem *mnSep1;
  TMenuItem *mnExit;
  TMenuItem *mnDevice;
  TMenuItem *mnStart;
  TMenuItem *mnPause;
  TMenuItem *mnStop;
  TMenuItem *mnView;
  TMenuItem *mnCursor;
  TMenuItem *mnSep3;
  TMenuItem *mnColours;
  TMenuItem *mnHelp;
  TMenuItem *mnAbout;
	TActionList *ActionList1;
	TAction *acNew;
  TAction *acOpen;
	TAction *acSaveas;
  TAction *acExit;
	TAction *acStart;
  TAction *acPause;
	TAction *acStop;
	TAction *acCursor;
  TAction *acColours;
  TAction *acAbout;
  TToolBar *ToolBar;
	TToolButton *tbNew;
  TToolButton *tbOpen;
	TToolButton *tbSaveAs;
  TToolButton *tbSep1;
	TToolButton *tbStart;
  TToolButton *tbPause;
	TToolButton *tbStop;
  TToolButton *tbSep2;
	TToolButton *tbCursor;
  TToolButton *tbSep4;
  TToolButton *tbColours;
	TImageList *ImageList1;
  TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TTimer *Timer1;
  TStatusBar *StatusBar;
  TAction *acSettings;
  TToolButton *ToolButton2;
  TToolButton *ToolButton3;
  TMenuItem *N1;
  TMenuItem *Settings1;
  TPanel *CtrlPanel;
  TPanel *UnderLogPanel;
  TScrollBar *LogScrollBar;
  TPanel *TlPanel;
  TAction *acConnect;
  TAction *acDisconnect;
  TMenuItem *Connect1;
  TMenuItem *Disconnect1;
  TMenuItem *N2;
  TToolButton *ToolButton1;
  TToolButton *ToolButton4;
  TToolButton *ToolButton5;
  TPanel *TrsPanel;
  TLabel *lbTraces;
  TCheckBox *cbGr1;
  TLabel *lbGr1;
  TCheckBox *cbGr2;
  TLabel *lbGr2;
  TCheckBox *cbGr3;
  TLabel *lbGr3;
  TCheckBox *cbGr4;
  TLabel *lbGr4;
  TCheckBox *cbGr5;
  TLabel *lbGr5;
  TAction *acRead;
  TAction *acWrite;
  TToolButton *ToolButton6;
  TToolButton *ToolButton7;
  TMenuItem *ReadParams1;
  TMenuItem *WriteParams1;
  TMenuItem *N3;
  TToolButton *ToolButton8;
  TThemeManager *ThemeManager1;
  TPanel *LogPanel;
  TPaintBox *LogPaintBox;
  TAction *acCutL;
  TAction *acCutR;
  TMenuItem *CutLeft1;
  TMenuItem *CutRight1;
  TMenuItem *N6;
  TAction *acFreeze;
  TToolButton *ToolButton9;
  TMenuItem *Freeze1;
  TGroupBox *GroupBox1;
  TGroupBox *GroupBox2;
  TFloatEdit *feP;
  TLabel *Label1;
  TFloatEdit *feI;
  TGroupBox *GroupBox3;
  TLabel *Label7;
  TFloatEdit *feD;
  TLabel *Label2;
  TScrollBar *sbScale;
  TScrollBar *sbShift;
  TComboBox *cbGrs;
  TLabel *Label9;
  TFloatEdit *feScale;
  TLabel *Label10;
  TFloatEdit *feShift;
  TSpeedButton *sbPlayF;
  TSpeedButton *sbPlayR;
  TSpeedButton *sbStop;
  TShape *shRev;
  TShape *shFwd;
  TButton *btSave;
  TButton *btRes;
  TBitBtn *BitBtn1;
  TBitBtn *BitBtn2;
  TLabel *Label37;
  TLabel *Label38;
  TFloatEdit *feFreq;
  TAction *acTest;
  TMenuItem *N4;
  TMenuItem *TestData1;
  TLabel *Label21;
  TLabel *Label22;
  TButton *btDefault;
  TCheckBox *cbGr6;
  TLabel *lbGr6;
  TAction *acParOpen;
  TAction *acParSave;
  TMenuItem *OpenPrams1;
  TMenuItem *SaveParams1;
  TMenuItem *N5;
  TLabel *Label5;
  TLabel *Label6;
  TFloatEdit *feDia;
  TLabel *Label8;
  TLabel *Label11;
  TFloatEdit *feTape;
  TLabel *Label12;
  TLabel *Label14;
  TFloatEdit *feEDia;
  TLabel *Label15;
  TLabel *Label16;
  TFloatEdit *feSlip;
  TLabel *Label18;
  TFloatEdit *fePoles;
  TCheckBox *cbDirect;
  TLabel *Label17;
  TLabel *Label19;
  TFloatEdit *feSpeed;
  TCheckBox *cbLock;
  TUpDown *udFreq;
  TGroupBox *GroupBox5;
  TComboBox *cbSpeed;
  TComboBox *cbDir;
  TGroupBox *GroupBox4;
  TFloatEdit *fePwm;
  TLabel *Label26;
  TCheckBox *cbManual;
  TFloatEdit *feCurSpd;
  TFloatEdit *fePhase;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label13;
  TCheckBox *cbSep;
  TShape *shStop;
  TLabel *Label20;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall acNewExecute(TObject *Sender);
  void __fastcall acOpenExecute(TObject *Sender);
	void __fastcall acSaveasExecute(TObject *Sender);
  void __fastcall acExitExecute(TObject *Sender);
	void __fastcall acStartExecute(TObject *Sender);
  void __fastcall acPauseExecute(TObject *Sender);
	void __fastcall acStopExecute(TObject *Sender);
	void __fastcall acCursorExecute(TObject *Sender);
  void __fastcall acColoursExecute(TObject *Sender);
  void __fastcall acAboutExecute(TObject *Sender);
  void __fastcall acSettingsExecute(TObject *Sender);
  void __fastcall acConnectExecute(TObject *Sender);
  void __fastcall acDisconnectExecute(TObject *Sender);
  void __fastcall acReadExecute(TObject *Sender);
  void __fastcall acWriteExecute(TObject *Sender);
  void __fastcall acCutLExecute(TObject *Sender);
  void __fastcall acCutRExecute(TObject *Sender);
  void __fastcall acFreezeExecute(TObject *Sender);
  void __fastcall LogPaintBoxMouseDown(TObject *Sender,
  TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall LogPaintBoxMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
  void __fastcall LogPaintBoxPaint(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
  void __fastcall LogScrollBarChange(TObject *Sender);
  void __fastcall cbGr1Click(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall fePChanged(TObject *Sender);
  void __fastcall feScaleChanged(TObject *Sender);
  void __fastcall feShiftChanged(TObject *Sender);
  void __fastcall cbGrsChange(TObject *Sender);
  void __fastcall sbScaleChange(TObject *Sender);
  void __fastcall sbShiftChange(TObject *Sender);
  void __fastcall btResClick(TObject *Sender);
  void __fastcall sbStopClick(TObject *Sender);
  void __fastcall btSaveClick(TObject *Sender);
  void __fastcall acTestExecute(TObject *Sender);
  void __fastcall btDefaultClick(TObject *Sender);
  void __fastcall acParOpenExecute(TObject *Sender);
  void __fastcall acParSaveExecute(TObject *Sender);
  void __fastcall cbSpeedChange(TObject *Sender);
  void __fastcall cbDirectClick(TObject *Sender);
  void __fastcall feSpeedChanged(TObject *Sender);
  void __fastcall feFreqChanged(TObject *Sender);
  void __fastcall cbManualClick(TObject *Sender);
  void __fastcall fePwmChanged(TObject *Sender);
  void __fastcall udFreqChangingEx(TObject *Sender, bool &AllowChange,
          short NewValue, TUpDownDirection Direction);
  void __fastcall cbDirChange(TObject *Sender);
  void __fastcall cbSepClick(TObject *Sender);
  void __fastcall lbGr1Click(TObject *Sender);
private:	// User declarations
  bool Lock;                  //флаг запрещения обработчиков контролов
  int FPortNumber;            //номер порта
  AnsiString FPortName;       //имя порта
  AnsiString FDeviceName;     //имя устройства
  AnsiString FDataDir;        //путь для сохранения файлов данных
  AnsiString FFileName;       //имя файла данных
  bool FModified;             //признак изменения данных
  double Data1[LOGN];         //массив входных данных 1
  double Data2[LOGN];         //массив входных данных 2
  double Data3[LOGN];         //массив входных данных 3
  double Data4[LOGN];         //массив входных данных 4
  double Data5[LOGN];         //массив входных данных 5
  double Data6[LOGN];         //массив входных данных 6
  double Scale[GRAPHS];
  double BaseShift[GRAPHS];
  double Shift[GRAPHS];
  int FPointer;               //указатель данных
  int FGMode;                 //режим (stop, pause, start)
  enum { MD_STOP, MD_PAUSE, MD_START }; //коды режима графика
  bool FCursor;               //режим курсора (on/off)
  bool FCurOn;                //признак прорисовки курсора
  int FCursorX;               //текущая координата курсора X
  Graphics::TBitmap *Bm;
	void __fastcall WmDropFiles(TWMDropFiles& Message);
	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WmDropFiles)
	END_MESSAGE_MAP(TForm);
  AnsiString __fastcall MakeCaption(void);    //формирование заголовка окна
  void __fastcall Connect(void);              //подключение устройства
  void __fastcall Disconnect(void);           //отключение устройства
	void __fastcall ReadConfig(void);           //чтение конфигурации
	void __fastcall SaveConfig(void);           //сохранение конфигурации
  void __fastcall Clear(void);                //очистка данных
  void __fastcall ClearScale(void);           //очистка шкал
  void __fastcall ReadData(AnsiString Name);  //чтение данных
  void __fastcall SaveData(AnsiString Name);  //сохранение данных
  void __fastcall DrawLogGrid(TCanvas *cv);   //прорисовка сетки
  void __fastcall DrawSegment(TCanvas *cv, TColor cl, double *data, //рисование 
                double range, double shift, int x0, int x, int dy); //сегмента
  void __fastcall DrawLog(TCanvas *cv, int x1, int x2);   //прорисовка графиков
	void __fastcall DrawLogCur(TCanvas *cv, bool e, int x); //прорисовка курсора

  void __fastcall SetLeds(int s);
  void __fastcall SetParams(void);
  void __fastcall GetParams(void);
  void __fastcall GetState(void);
  double __fastcall Code2Percent(int c);
  int __fastcall Percent2Code(double p);
  double __fastcall CodeToPhase(int p);
  int FMode;
  int FRef;
  state_t State;
  params_t Params;

public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
  TWakePort *WakePort;        //объект порта
  bool FConnected;            //признак соединения
  TColor Col_Bg, Col_Ax, Col_Tx, Col_Cr,
   Col_G1, Col_G2, Col_G3, Col_G4, Col_G5, Col_G6; //цвета
  bool FAutoConnect;          //флаг автоматического подключения
  bool FAutoLoad;             //флаг автоматической загрузки параметров
  int FSample;                //период опроса
  bool FAutoScroll;           //признак автоматического скроллирования
  void __fastcall SetDefColours(void); //установка цветов по умолчанию
  void __fastcall RedrawAll(void);     //перерисовка всего
};

//---------------------------------------------------------------------------

extern PACKAGE TMainForm *MainForm;

//---------------------------------------------------------------------------

#endif
