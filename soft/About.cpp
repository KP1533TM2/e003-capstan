#include <vcl.h>
#pragma hdrstop

#include "About.h"
#include "Main.h"

#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutForm *AboutForm;

//---------------------------------------------------------------------------
__fastcall TAboutForm::TAboutForm(TComponent* Owner)
  : TForm(Owner)
{
}

//---------------------------------------------------------------------------

void __fastcall TAboutForm::FormCreate(TObject *Sender)
{
  DecimalSeparator = '.';
  Label1->Caption = "NSM lab - " +
  Application->Title + " Software, version " + FloatToStrF(VERSION, ffFixed, 3, 2);
}

//---------------------------------------------------------------------------

void __fastcall TAboutForm::Button1Click(TObject *Sender)
{
  Close();
}

//---------------------------------------------------------------------------

void __fastcall TAboutForm::FormKeyPress(TObject *Sender, char &Key)
{
  if (Key == VK_ESCAPE)
   ModalResult = mrOk;
}

//---------------------------------------------------------------------------

void __fastcall TAboutForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action = caFree;
  AboutForm = NULL;
}

//---------------------------------------------------------------------------

