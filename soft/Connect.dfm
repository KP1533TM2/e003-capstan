object ConnectForm: TConnectForm
  Left = 544
  Top = 451
  ActiveControl = DevListView
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Connect...'
  ClientHeight = 232
  ClientWidth = 273
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
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object btCancel: TButton
    Left = 191
    Top = 196
    Width = 75
    Height = 25
    Caption = '&Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object btOk: TButton
    Left = 111
    Top = 196
    Width = 75
    Height = 25
    Caption = '&OK'
    ModalResult = 1
    TabOrder = 0
    OnClick = btOkClick
  end
  object DevListView: TListView
    Left = 8
    Top = 8
    Width = 257
    Height = 177
    Columns = <
      item
        Caption = 'Port'
        Width = 60
      end
      item
        Caption = 'Device name'
        Width = 189
      end>
    ColumnClick = False
    HideSelection = False
    ReadOnly = True
    RowSelect = True
    TabOrder = 2
    ViewStyle = vsReport
    OnDblClick = btOkClick
  end
  object btRefresh: TButton
    Left = 7
    Top = 196
    Width = 75
    Height = 25
    Caption = '&Refresh'
    TabOrder = 3
    OnClick = btRefreshClick
  end
end
