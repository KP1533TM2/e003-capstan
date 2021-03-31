object SettingsForm: TSettingsForm
  Left = 413
  Top = 283
  ActiveControl = btOk
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Settings...'
  ClientHeight = 274
  ClientWidth = 368
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
    368
    274)
  PixelsPerInch = 96
  TextHeight = 13
  object Label18: TLabel
    Left = 107
    Top = 133
    Width = 99
    Height = 13
    Caption = #1055#1077#1088#1077#1084#1086#1090#1082#1072' '#1074#1087#1077#1088#1077#1076':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label19: TLabel
    Left = 261
    Top = 133
    Width = 8
    Height = 13
    Caption = '%'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label21: TLabel
    Left = 333
    Top = 133
    Width = 8
    Height = 13
    Caption = '%'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label35: TLabel
    Left = 11
    Top = 29
    Width = 217
    Height = 13
    Caption = #1042#1077#1084#1103' '#1080#1085#1090#1077#1075'. '#1089#1080#1075#1085#1072#1083#1072' '#1086#1087#1090#1080#1095#1077#1089#1082#1086#1075#1086' '#1076#1072#1090#1095#1080#1082#1072':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label36: TLabel
    Left = 285
    Top = 29
    Width = 14
    Height = 13
    Caption = #1084#1089
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object btCancel: TButton
    Left = 281
    Top = 234
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
    OnClick = btCancelClick
  end
  object btOk: TButton
    Left = 197
    Top = 234
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 0
    OnClick = btOkClick
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 368
    Height = 216
    ActivePage = TabSheet4
    Align = alTop
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabIndex = 0
    TabOrder = 2
    object TabSheet4: TTabSheet
      Caption = 'General'
      object GroupBox2: TGroupBox
        Left = 8
        Top = 8
        Width = 345
        Height = 81
        Caption = ' Startup '
        TabOrder = 0
        object cbAutoConnect: TCheckBox
          Left = 16
          Top = 24
          Width = 97
          Height = 17
          Caption = 'Auto connect'
          TabOrder = 0
        end
        object cbAutoLoad: TCheckBox
          Left = 16
          Top = 48
          Width = 121
          Height = 17
          Caption = 'Auto load parameters'
          TabOrder = 1
        end
      end
      object GroupBox1: TGroupBox
        Left = 8
        Top = 96
        Width = 345
        Height = 81
        Caption = ' Graph '
        TabOrder = 1
        object Label1: TLabel
          Left = 15
          Top = 24
          Width = 59
          Height = 13
          Caption = 'Sample rate:'
          Transparent = True
        end
        object Label2: TLabel
          Left = 135
          Top = 24
          Width = 13
          Height = 13
          Caption = 'ms'
          Transparent = True
        end
        object cbAutoScroll: TCheckBox
          Left = 16
          Top = 48
          Width = 97
          Height = 17
          Caption = 'Auto scroll'
          TabOrder = 0
        end
        object feSample: TFloatEdit
          Left = 80
          Top = 21
          Width = 49
          Height = 19
          Format = ffFixed
          Digits = 0
          Precision = 5
          MinMaxCheck = True
          MaxValue = 1000
          ValueVisible = True
          AutoSize = False
          TabOrder = 1
        end
      end
    end
  end
end
