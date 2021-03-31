#ifndef ConnectH
#define ConnectH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "FloatEdit.hpp"

//---------------------------------------------------------------------------

class TConnectForm : public TForm
{
__published:	// IDE-managed Components
  TButton *btCancel;
  TButton *btOk;
  TListView *DevListView;
  TButton *btRefresh;
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall btOkClick(TObject *Sender);
  void __fastcall btRefreshClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
private:	// User declarations
  int *FPortNumber;
  void __fastcall ScanPorts(void);
public:		// User declarations
  __fastcall TConnectForm(TComponent* Owner, int *PortNumber);
};

//---------------------------------------------------------------------------

extern PACKAGE TConnectForm *ConnectForm;

//---------------------------------------------------------------------------

#endif
