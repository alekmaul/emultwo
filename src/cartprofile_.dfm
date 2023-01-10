object cartprofile: Tcartprofile
  Left = 687
  Top = 148
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'E.mul T.wo Cartridge Info'
  ClientHeight = 408
  ClientWidth = 848
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 4
    Top = 4
    Width = 385
    Height = 397
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    Lines.Strings = (
      'Memo1')
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object grpMemFootPrint: TGroupBox
    Left = 392
    Top = 0
    Width = 452
    Height = 369
    Caption = 'Memory Footprint'
    TabOrder = 1
    object ScrollBanks: TScrollBox
      Left = 2
      Top = 15
      Width = 448
      Height = 352
      VertScrollBar.Increment = 48
      VertScrollBar.Range = 512
      Align = alClient
      AutoScroll = False
      TabOrder = 0
      object ImageBanks: TImage
        Left = 0
        Top = 0
        Width = 427
        Height = 512
        Align = alClient
      end
      object iBank: TImage
        Left = 32
        Top = 61
        Width = 64
        Height = 128
        Visible = False
      end
    end
  end
  object Button1: TButton
    Left = 754
    Top = 375
    Width = 81
    Height = 26
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    TabOrder = 2
    OnClick = Button1Click
  end
  object Refresh: TButton
    Left = 672
    Top = 376
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Refresh'
    TabOrder = 3
    OnClick = RefreshClick
  end
  object EmptyFF: TRadioButton
    Left = 400
    Top = 376
    Width = 89
    Height = 17
    Caption = 'Empty is $FF'
    Checked = True
    TabOrder = 4
    TabStop = True
    OnClick = EmptyFFClick
  end
  object Empty00: TRadioButton
    Left = 488
    Top = 376
    Width = 89
    Height = 17
    Caption = 'Empty is $00'
    TabOrder = 5
    OnClick = Empty00Click
  end
end
