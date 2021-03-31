#ifndef ColoursH
#define ColoursH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <Buttons.hpp>

//---------------------------------------------------------------------------

class TColoursForm : public TForm
{
__published:	// IDE-managed Components
  TButton *btOk;
  TButton *btCancel;
  TButton *btDefault;
  TButton *btApply;
  TColorDialog *ColorDialog;
  TPanel *Panel1;
  TLabel *Label3;
  TPanel *pnBg;
  TPanel *Panel3;
  TLabel *Label1;
  TPanel *pnCr;
  TPanel *Panel4;
  TLabel *Label7;
  TPanel *pnAx;
  TPanel *Panel5;
  TLabel *Label2;
  TPanel *pnTx;
  TPanel *Panel6;
  TLabel *Label5;
  TPanel *pnG2;
  TPanel *Panel8;
  TLabel *Label6;
  TPanel *pnG3;
  TPanel *Panel7;
  TLabel *Label8;
  TPanel *pnG1;
  TPanel *Panel9;
  TLabel *Label9;
  TPanel *pnG4;
  TButton *btBg;
  TButton *btAx;
  TButton *btTx;
  TButton *btCr;
  TButton *btG1;
  TButton *btG2;
  TButton *btG3;
  TButton *btG4;
  TPanel *Panel2;
  TLabel *Label4;
  TPanel *pnG5;
  TButton *btG5;
  TPanel *Panel11;
  TLabel *Label10;
  TPanel *pnG6;
  TButton *btG6;
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall btOkClick(TObject *Sender);
  void __fastcall btCancelClick(TObject *Sender);
  void __fastcall btApplyClick(TObject *Sender);
  void __fastcall btDefaultClick(TObject *Sender);
  void __fastcall pnBgDblClick(TObject *Sender);
private:	// User declarations
  TColor Cor_Bg, Cor_Ax, Cor_Tx, Cor_Cr,
   Cor_G1, Cor_G2, Cor_G3, Cor_G4, Cor_G5, Cor_G6;
public:		// User declarations
  __fastcall TColoursForm(TComponent* Owner);
};

//---------------------------------------------------------------------------

extern PACKAGE TColoursForm *ColoursForm;

//---------------------------------------------------------------------------

#endif
