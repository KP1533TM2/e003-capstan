#include <vcl.h>
#pragma hdrstop

#include "Connect.h"
#include "Main.h"

#pragma package(smart_init)
#pragma link "FloatEdit"
#pragma resource "*.dfm"

TConnectForm *ConnectForm;

//---------------------------------------------------------------------------

__fastcall TConnectForm::TConnectForm(TComponent* Owner, int *PortNumber)
  : TForm(Owner)
{
  FPortNumber = PortNumber;
}

//---------------------------------------------------------------------------

void __fastcall TConnectForm::FormShow(TObject *Sender)
{
  ScanPorts();
}

//---------------------------------------------------------------------------

void __fastcall TConnectForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action = caFree;
  ConnectForm = NULL;
}

//---------------------------------------------------------------------------

void __fastcall TConnectForm::ScanPorts(void)
{
  TCursor Save_Cursor = Screen->Cursor;
  TStringList *APortBox = new TStringList;
  DevListView->Clear();
  try
  {
    Screen->Cursor = crHourGlass;
    if(*FPortNumber) MainForm->WakePort->Close();
    MainForm->WakePort->GetPortList(APortBox);
    for (int i = 0; i < APortBox->Count; i++)
      try
      {
        TListItem *pListItem = DevListView->Items->Add();
        pListItem->Caption = APortBox->Strings[i];
        pListItem->SubItems->Add("None");
        MainForm->WakePort->Open(APortBox->Strings[i]);
        pListItem->SubItems->Strings[0] = MainForm->WakePort->GetInfo(0);
        if(DevListView->ItemIndex < 0)
        {
          pListItem->Selected = 1;
          pListItem->Focused = 1;
        }
        MainForm->WakePort->Close();
      }
      catch(const EInOutError &e) { MainForm->WakePort->Close(); }
  }
  __finally { Screen->Cursor = Save_Cursor; delete APortBox; }
  DevListView->Refresh();
  DevListView->SetFocus();
}

//---------------------------------------------------------------------------

void __fastcall TConnectForm::FormKeyPress(TObject *Sender, char &Key)
{
  if (Key == VK_ESCAPE) ModalResult = mrCancel;
  if (Key == VK_RETURN) btOkClick(Sender);
}

//---------------------------------------------------------------------------

void __fastcall TConnectForm::btRefreshClick(TObject *Sender)
{
  ScanPorts();
}

//---------------------------------------------------------------------------

void __fastcall TConnectForm::btOkClick(TObject *Sender)
{
  int i = DevListView->ItemIndex;
  if(i >= 0 && i < DevListView->Items->Count)
  {
    AnsiString pPortName = DevListView->Items->Item[i]->Caption;
    *FPortNumber = MainForm->WakePort->NameToNumber(pPortName);
  }
  else *FPortNumber = 0;
  ModalResult = mrOk;
}

//---------------------------------------------------------------------------


