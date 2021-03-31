#ifndef SettingsH
#define SettingsH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "FloatEdit.hpp"
#include <ComCtrls.hpp>

#define PRESETS 10
#define CALIBS  13

//---------------------------------------------------------------------------

class TSettingsForm : public TForm
{
__published:	// IDE-managed Components
  TButton *btCancel;
  TButton *btOk;
  TPageControl *PageControl1;
  TTabSheet *TabSheet4;
  TGroupBox *GroupBox2;
  TCheckBox *cbAutoConnect;
  TCheckBox *cbAutoLoad;
  TGroupBox *GroupBox1;
  TLabel *Label1;
  TCheckBox *cbAutoScroll;
  TFloatEdit *feSample;
  TLabel *Label2;
  TLabel *Label18;
  TLabel *Label19;
  TLabel *Label21;
  TLabel *Label35;
  TLabel *Label36;
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall btOkClick(TObject *Sender);
  void __fastcall btCancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TSettingsForm(TComponent* Owner);
};

//---------------------------------------------------------------------------

extern PACKAGE TSettingsForm *SettingsForm;

//---------------------------------------------------------------------------

#endif
