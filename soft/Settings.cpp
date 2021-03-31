#include <vcl.h>
#pragma hdrstop

#include "Settings.h"
#include "Main.h"

#pragma package(smart_init)
#pragma link "FloatEdit"
#pragma resource "*.dfm"
TSettingsForm *SettingsForm;

//---------------------------------------------------------------------------

__fastcall TSettingsForm::TSettingsForm(TComponent* Owner)
  : TForm(Owner)
{
}

//----------------------------- Form create: --------------------------------

void __fastcall TSettingsForm::FormCreate(TObject *Sender)
{
  cbAutoConnect->Checked = MainForm->FAutoConnect;
  cbAutoLoad->Checked = MainForm->FAutoLoad;
  feSample->Value = MainForm->FSample;
  cbAutoScroll->Checked = MainForm->FAutoScroll;
}

//----------------------------- Form close: ---------------------------------

void __fastcall TSettingsForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  if(ModalResult == mrOk)
  {
    MainForm->FAutoConnect = cbAutoConnect->Checked;
    MainForm->FAutoLoad = cbAutoLoad->Checked;
    MainForm->FSample = feSample->Value;
    MainForm->FAutoScroll = cbAutoScroll->Checked;
  }
  Action = caFree;
  SettingsForm = NULL;
}

//------------------------------- ESC: --------------------------------------

void __fastcall TSettingsForm::FormKeyPress(TObject *Sender, char &Key)
{
  if (Key == VK_ESCAPE)
   ModalResult = mrCancel;
}

//-------------------------------- OK: --------------------------------------

void __fastcall TSettingsForm::btOkClick(TObject *Sender)
{
  ModalResult = mrOk;
}

//------------------------------ Cancel: ------------------------------------

void __fastcall TSettingsForm::btCancelClick(TObject *Sender)
{
  ModalResult = mrCancel;
}

//---------------------------------------------------------------------------

