object ColoursForm: TColoursForm
  Left = 346
  Top = 312
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Colours...'
  ClientHeight = 234
  ClientWidth = 402
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyPress = FormKeyPress
  DesignSize = (
    402
    234)
  PixelsPerInch = 96
  TextHeight = 13
  object btOk: TButton
    Left = 240
    Top = 201
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 0
    OnClick = btOkClick
  end
  object btCancel: TButton
    Left = 320
    Top = 201
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
    OnClick = btCancelClick
  end
  object btDefault: TButton
    Left = 8
    Top = 201
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Set Default'
    TabOrder = 10
    OnClick = btDefaultClick
  end
  object btApply: TButton
    Left = 160
    Top = 201
    Width = 75
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Apply'
    TabOrder = 11
    OnClick = btApplyClick
  end
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 185
    Height = 25
    BevelOuter = bvLowered
    TabOrder = 2
    object Label3: TLabel
      Left = 9
      Top = 6
      Width = 46
      Height = 13
      Caption = 'Backgnd:'
    end
    object pnBg: TPanel
      Left = 61
      Top = 3
      Width = 100
      Height = 19
      BevelOuter = bvLowered
      Color = clWhite
      TabOrder = 0
      OnDblClick = pnBgDblClick
    end
    object btBg: TButton
      Left = 163
      Top = 3
      Width = 19
      Height = 19
      Caption = '...'
      TabOrder = 1
      OnClick = pnBgDblClick
    end
  end
  object Panel3: TPanel
    Left = 8
    Top = 104
    Width = 185
    Height = 25
    BevelOuter = bvLowered
    TabOrder = 5
    object Label1: TLabel
      Left = 22
      Top = 6
      Width = 33
      Height = 13
      Caption = 'Cursor:'
    end
    object pnCr: TPanel
      Tag = 3
      Left = 61
      Top = 3
      Width = 100
      Height = 19
      BevelOuter = bvLowered
      Color = clBlack
      TabOrder = 0
      OnDblClick = pnBgDblClick
    end
    object btCr: TButton
      Tag = 3
      Left = 163
      Top = 3
      Width = 19
      Height = 19
      Caption = '...'
      TabOrder = 1
      OnClick = pnBgDblClick
    end
  end
  object Panel4: TPanel
    Left = 8
    Top = 40
    Width = 185
    Height = 25
    BevelOuter = bvLowered
    TabOrder = 3
    object Label7: TLabel
      Left = 33
      Top = 6
      Width = 22
      Height = 13
      Caption = 'Axis:'
    end
    object pnAx: TPanel
      Tag = 1
      Left = 61
      Top = 3
      Width = 100
      Height = 19
      BevelOuter = bvLowered
      Color = clSilver
      TabOrder = 0
      OnDblClick = pnBgDblClick
    end
    object btAx: TButton
      Tag = 1
      Left = 163
      Top = 3
      Width = 19
      Height = 19
      Caption = '...'
      TabOrder = 1
      OnClick = pnBgDblClick
    end
  end
  object Panel5: TPanel
    Left = 8
    Top = 72
    Width = 185
    Height = 25
    BevelOuter = bvLowered
    TabOrder = 4
    object Label2: TLabel
      Left = 31
      Top = 6
      Width = 24
      Height = 13
      Caption = 'Text:'
    end
    object pnTx: TPanel
      Tag = 2
      Left = 61
      Top = 3
      Width = 100
      Height = 19
      BevelOuter = bvLowered
      Color = clBlack
      TabOrder = 0
      OnDblClick = pnBgDblClick
    end
    object btTx: TButton
      Tag = 2
      Left = 163
      Top = 3
      Width = 19
      Height = 19
      Caption = '...'
      TabOrder = 1
      OnClick = pnBgDblClick
    end
  end
  object Panel6: TPanel
    Left = 208
    Top = 40
    Width = 185
    Height = 25
    BevelOuter = bvLowered
    TabOrder = 7
    object Label5: TLabel
      Left = 14
      Top = 6
      Width = 41
      Height = 13
      Caption = 'Graph 2:'
    end
    object pnG2: TPanel
      Tag = 5
      Left = 61
      Top = 3
      Width = 100
      Height = 19
      BevelOuter = bvLowered
      Color = clGreen
      TabOrder = 0
      OnDblClick = pnBgDblClick
    end
    object btG2: TButton
      Tag = 5
      Left = 163
      Top = 3
      Width = 19
      Height = 19
      Caption = '...'
      TabOrder = 1
      OnClick = pnBgDblClick
    end
  end
  object Panel8: TPanel
    Left = 208
    Top = 72
    Width = 185
    Height = 25
    BevelOuter = bvLowered
    TabOrder = 8
    object Label6: TLabel
      Left = 14
      Top = 6
      Width = 41
      Height = 13
      Caption = 'Graph 3:'
    end
    object pnG3: TPanel
      Tag = 6
      Left = 61
      Top = 3
      Width = 100
      Height = 19
      BevelOuter = bvLowered
      Color = clBlue
      TabOrder = 0
      OnDblClick = pnBgDblClick
    end
    object btG3: TButton
      Tag = 6
      Left = 163
      Top = 3
      Width = 19
      Height = 19
      Caption = '...'
      TabOrder = 1
      OnClick = pnBgDblClick
    end
  end
  object Panel7: TPanel
    Left = 208
    Top = 8
    Width = 185
    Height = 25
    BevelOuter = bvLowered
    TabOrder = 6
    object Label8: TLabel
      Left = 14
      Top = 6
      Width = 41
      Height = 13
      Caption = 'Graph 1:'
    end
    object pnG1: TPanel
      Tag = 4
      Left = 61
      Top = 3
      Width = 100
      Height = 19
      BevelOuter = bvLowered
      Color = clRed
      TabOrder = 0
      OnDblClick = pnBgDblClick
    end
    object btG1: TButton
      Tag = 4
      Left = 163
      Top = 3
      Width = 19
      Height = 19
      Caption = '...'
      TabOrder = 1
      OnClick = pnBgDblClick
    end
  end
  object Panel9: TPanel
    Left = 208
    Top = 104
    Width = 185
    Height = 25
    BevelOuter = bvLowered
    TabOrder = 9
    object Label9: TLabel
      Left = 13
      Top = 6
      Width = 41
      Height = 13
      Caption = 'Graph 4:'
    end
    object pnG4: TPanel
      Tag = 7
      Left = 61
      Top = 3
      Width = 100
      Height = 19
      BevelOuter = bvLowered
      Color = clYellow
      TabOrder = 0
      OnDblClick = pnBgDblClick
    end
    object btG4: TButton
      Tag = 7
      Left = 163
      Top = 3
      Width = 19
      Height = 19
      Caption = '...'
      TabOrder = 1
      OnClick = pnBgDblClick
    end
  end
  object Panel2: TPanel
    Left = 208
    Top = 136
    Width = 185
    Height = 25
    BevelOuter = bvLowered
    TabOrder = 12
    object Label4: TLabel
      Left = 13
      Top = 6
      Width = 41
      Height = 13
      Caption = 'Graph 5:'
    end
    object pnG5: TPanel
      Tag = 8
      Left = 61
      Top = 3
      Width = 100
      Height = 19
      BevelOuter = bvLowered
      Color = clYellow
      TabOrder = 0
      OnDblClick = pnBgDblClick
    end
    object btG5: TButton
      Tag = 8
      Left = 163
      Top = 3
      Width = 19
      Height = 19
      Caption = '...'
      TabOrder = 1
      OnClick = pnBgDblClick
    end
  end
  object Panel11: TPanel
    Left = 208
    Top = 168
    Width = 185
    Height = 25
    BevelOuter = bvLowered
    TabOrder = 13
    object Label10: TLabel
      Left = 13
      Top = 6
      Width = 41
      Height = 13
      Caption = 'Graph 6:'
    end
    object pnG6: TPanel
      Tag = 9
      Left = 61
      Top = 3
      Width = 100
      Height = 19
      BevelOuter = bvLowered
      Color = clYellow
      TabOrder = 0
      OnDblClick = pnBgDblClick
    end
    object btG6: TButton
      Tag = 9
      Left = 163
      Top = 3
      Width = 19
      Height = 19
      Caption = '...'
      TabOrder = 1
      OnClick = pnBgDblClick
    end
  end
  object ColorDialog: TColorDialog
    Ctl3D = True
  end
end
