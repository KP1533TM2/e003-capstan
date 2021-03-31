#include <vcl.h>
#pragma hdrstop

#include "Colours.h"
#include "Main.h"

#pragma package(smart_init)
#pragma resource "*.dfm"
TColoursForm *ColoursForm;

//---------------------------------------------------------------------------

__fastcall TColoursForm::TColoursForm(TComponent* Owner)
  : TForm(Owner)
{
}

//---------------------------------------------------------------------------

void __fastcall TColoursForm::FormCreate(TObject *Sender)
{
  pnBg->Color = MainForm->Col_Bg;
  pnAx->Color = MainForm->Col_Ax;
  pnTx->Color = MainForm->Col_Tx;
  pnCr->Color = MainForm->Col_Cr;
  pnG1->Color = MainForm->Col_G1;
  pnG2->Color = MainForm->Col_G2;
  pnG3->Color = MainForm->Col_G3;
  pnG4->Color = MainForm->Col_G4;
  pnG5->Color = MainForm->Col_G5;
  pnG6->Color = MainForm->Col_G6;
  Cor_Bg = pnBg->Color;
  Cor_Ax = pnAx->Color;
  Cor_Tx = pnTx->Color;
  Cor_Cr = pnCr->Color;
  Cor_G1 = pnG1->Color;
  Cor_G2 = pnG2->Color;
  Cor_G3 = pnG3->Color;
  Cor_G4 = pnG4->Color;
  Cor_G5 = pnG5->Color;
  Cor_G6 = pnG6->Color;
  ColorDialog->Options << cdFullOpen;
}

//---------------------------------------------------------------------------

void __fastcall TColoursForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  MainForm->Col_Bg = Cor_Bg;
  MainForm->Col_Ax = Cor_Ax;
  MainForm->Col_Tx = Cor_Tx;
  MainForm->Col_Cr = Cor_Cr;
  MainForm->Col_G1 = Cor_G1;
  MainForm->Col_G2 = Cor_G2;
  MainForm->Col_G3 = Cor_G3;
  MainForm->Col_G4 = Cor_G4;
  MainForm->Col_G5 = Cor_G5;
  MainForm->Col_G6 = Cor_G6;
  MainForm->RedrawAll();
  Action = caFree;
  ColoursForm = NULL;
}

//---------------------------------------------------------------------------

void __fastcall TColoursForm::FormKeyPress(TObject *Sender, char &Key)
{
  if (Key == VK_ESCAPE)
   ModalResult = mrCancel;
}

//---------------------------------------------------------------------------

void __fastcall TColoursForm::btOkClick(TObject *Sender)
{
  Cor_Bg = pnBg->Color;
  Cor_Ax = pnAx->Color;
  Cor_Tx = pnTx->Color;
  Cor_Cr = pnCr->Color;
  Cor_G1 = pnG1->Color;
  Cor_G2 = pnG2->Color;
  Cor_G3 = pnG3->Color;
  Cor_G4 = pnG4->Color;
  Cor_G5 = pnG5->Color;
  Cor_G6 = pnG6->Color;
  Close();
}

//---------------------------------------------------------------------------

void __fastcall TColoursForm::btCancelClick(TObject *Sender)
{
   Close();
}

//---------------------------------------------------------------------------

void __fastcall TColoursForm::btApplyClick(TObject *Sender)
{
  MainForm->Col_Bg = pnBg->Color;
  MainForm->Col_Ax = pnAx->Color;
  MainForm->Col_Tx = pnTx->Color;
  MainForm->Col_Cr = pnCr->Color;
  MainForm->Col_G1 = pnG1->Color;
  MainForm->Col_G2 = pnG2->Color;
  MainForm->Col_G3 = pnG3->Color;
  MainForm->Col_G4 = pnG4->Color;
  MainForm->Col_G5 = pnG5->Color;
  MainForm->Col_G6 = pnG6->Color;
  MainForm->RedrawAll();
}

//---------------------------------------------------------------------------

void __fastcall TColoursForm::btDefaultClick(TObject *Sender)
{
  MainForm->SetDefColours();
  pnBg->Color = MainForm->Col_Bg;
  pnAx->Color = MainForm->Col_Ax;
  pnTx->Color = MainForm->Col_Tx;
  pnCr->Color = MainForm->Col_Cr;
  pnG1->Color = MainForm->Col_G1;
  pnG2->Color = MainForm->Col_G2;
  pnG3->Color = MainForm->Col_G3;
  pnG4->Color = MainForm->Col_G4;
  pnG5->Color = MainForm->Col_G5;
  pnG6->Color = MainForm->Col_G6;
}

//---------------------------------------------------------------------------

void __fastcall TColoursForm::pnBgDblClick(TObject *Sender)
{
  int tag = ((TComponent *) Sender)->Tag;
  TColor color;
  switch(tag)
  {
    case 0: color = pnBg->Color; break;
    case 1: color = pnAx->Color; break;
    case 2: color = pnTx->Color; break;
    case 3: color = pnCr->Color; break;
    case 4: color = pnG1->Color; break;
    case 5: color = pnG2->Color; break;
    case 6: color = pnG3->Color; break;
    case 7: color = pnG4->Color; break;
    case 8: color = pnG5->Color; break;
    case 9: color = pnG6->Color; break;
  }
  ColorDialog->Color = color;
  if(ColorDialog->Execute())
  {
    color = ColorDialog->Color;
    switch(tag)
    {
      case 0: pnBg->Color = color; break;
      case 1: pnAx->Color = color; break;
      case 2: pnTx->Color = color; break;
      case 3: pnCr->Color = color; break;
      case 4: pnG1->Color = color; break;
      case 5: pnG2->Color = color; break;
      case 6: pnG3->Color = color; break;
      case 7: pnG4->Color = color; break;
      case 8: pnG5->Color = color; break;
      case 9: pnG6->Color = color; break;
    }
  }
}

//---------------------------------------------------------------------------


