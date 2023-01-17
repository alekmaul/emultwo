object dumpmem: Tdumpmem
  Left = 1028
  Top = 165
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'Dump Memory'
  ClientHeight = 352
  ClientWidth = 600
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 12
    Width = 36
    Height = 13
    Caption = 'Length:'
  end
  object OK: TButton
    Left = 504
    Top = 320
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    TabOrder = 0
    OnClick = OKClick
  end
  object EditVal: TEdit
    Left = 48
    Top = 8
    Width = 121
    Height = 21
    CharCase = ecUpperCase
    MaxLength = 17
    TabOrder = 1
    Text = '0'
    OnChange = EditValChange
    OnKeyPress = EditValKeyPress
  end
  object memdump: TMemo
    Left = 8
    Top = 40
    Width = 585
    Height = 273
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    Lines.Strings = (
      'memdump')
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 2
  end
  object Save: TButton
    Left = 408
    Top = 320
    Width = 83
    Height = 25
    Caption = 'Copy Clipboard'
    TabOrder = 3
    OnClick = SaveClick
  end
end
