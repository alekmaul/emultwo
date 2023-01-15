object paletteviewer: Tpaletteviewer
  Left = 589
  Top = 209
  BorderStyle = bsToolWindow
  Caption = 'Palette Viewer'
  ClientHeight = 338
  ClientWidth = 469
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lVVPal4: TLabel
    Left = 8
    Top = 148
    Width = 6
    Height = 13
    Caption = '4'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object lVVPal3: TLabel
    Left = 8
    Top = 108
    Width = 6
    Height = 13
    Caption = '3'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object lVVPPal2: TLabel
    Left = 8
    Top = 68
    Width = 6
    Height = 13
    Caption = '2'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object lVVPal1: TLabel
    Left = 8
    Top = 28
    Width = 6
    Height = 13
    Caption = '1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object lVVPBackPal: TLabel
    Left = 24
    Top = 4
    Width = 99
    Height = 13
    Caption = 'Background Palettes'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object lVVPF18APal: TLabel
    Left = 24
    Top = 188
    Width = 66
    Height = 13
    Caption = 'F18A Palettes'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label1: TLabel
    Left = 8
    Top = 212
    Width = 6
    Height = 13
    Caption = '1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 8
    Top = 244
    Width = 6
    Height = 13
    Caption = '2'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 8
    Top = 276
    Width = 6
    Height = 13
    Caption = '3'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 8
    Top = 308
    Width = 6
    Height = 13
    Caption = '4'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object gCurCol: TGroupBox
    Left = 328
    Top = 18
    Width = 137
    Height = 137
    Caption = 'Details'
    TabOrder = 0
    object lCurColR: TLabel
      Left = 48
      Top = 28
      Width = 20
      Height = 13
      Caption = 'Red'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object lCurColG: TLabel
      Left = 48
      Top = 52
      Width = 29
      Height = 13
      Caption = 'Green'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object lCurColB: TLabel
      Left = 48
      Top = 76
      Width = 21
      Height = 13
      Caption = 'Blue'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object lCurColIdx: TLabel
      Left = 48
      Top = 108
      Width = 67
      Height = 13
      Caption = 'Index Address'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object eCurColR: TStaticText
      Left = 8
      Top = 24
      Width = 33
      Height = 21
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 0
    end
    object eCurColG: TStaticText
      Left = 8
      Top = 48
      Width = 33
      Height = 21
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 1
    end
    object eCurColB: TStaticText
      Left = 8
      Top = 72
      Width = 33
      Height = 21
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 2
    end
    object eCurColIdx: TStaticText
      Left = 8
      Top = 104
      Width = 33
      Height = 21
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 3
    end
  end
  object pCurCol: TPanel
    Left = 216
    Top = 24
    Width = 105
    Height = 89
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 1
  end
  object pCurCol15: TPanel
    Tag = 15
    Left = 168
    Top = 144
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 2
    OnMouseMove = pCurCol0MouseMove
  end
  object pCurCol14: TPanel
    Tag = 14
    Left = 120
    Top = 144
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 3
    OnMouseMove = pCurCol0MouseMove
  end
  object pCurCol13: TPanel
    Tag = 13
    Left = 72
    Top = 144
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 4
    OnMouseMove = pCurCol0MouseMove
  end
  object pCurCol12: TPanel
    Tag = 12
    Left = 24
    Top = 144
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 5
    OnMouseMove = pCurCol0MouseMove
  end
  object pCurCol8: TPanel
    Tag = 8
    Left = 24
    Top = 104
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 6
    OnMouseMove = pCurCol0MouseMove
  end
  object pCurCol9: TPanel
    Tag = 9
    Left = 72
    Top = 104
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 7
    OnMouseMove = pCurCol0MouseMove
  end
  object pCurCol10: TPanel
    Tag = 10
    Left = 120
    Top = 104
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 8
    OnMouseMove = pCurCol0MouseMove
  end
  object pCurCol11: TPanel
    Tag = 11
    Left = 168
    Top = 104
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 9
    OnMouseMove = pCurCol0MouseMove
  end
  object pCurCol7: TPanel
    Tag = 7
    Left = 168
    Top = 64
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 10
    OnMouseMove = pCurCol0MouseMove
  end
  object pCurCol6: TPanel
    Tag = 6
    Left = 120
    Top = 64
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 11
    OnMouseMove = pCurCol0MouseMove
  end
  object pCurCol5: TPanel
    Tag = 5
    Left = 72
    Top = 64
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 12
    OnMouseMove = pCurCol0MouseMove
  end
  object pCurCol4: TPanel
    Tag = 4
    Left = 24
    Top = 64
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 13
    OnMouseMove = pCurCol0MouseMove
  end
  object pCurCol0: TPanel
    Left = 24
    Top = 24
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 14
    OnMouseMove = pCurCol0MouseMove
  end
  object pCurCol1: TPanel
    Tag = 1
    Left = 72
    Top = 24
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 15
    OnMouseMove = pCurCol0MouseMove
  end
  object pCurCol2: TPanel
    Tag = 2
    Left = 120
    Top = 24
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 16
    OnMouseMove = pCurCol0MouseMove
  end
  object pCurCol3: TPanel
    Tag = 3
    Left = 168
    Top = 24
    Width = 32
    Height = 28
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 17
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel1: TPanel
    Left = 24
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 18
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel2: TPanel
    Left = 48
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 19
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel3: TPanel
    Left = 72
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 20
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel4: TPanel
    Left = 96
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 21
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel5: TPanel
    Left = 120
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 22
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel6: TPanel
    Left = 144
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 23
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel7: TPanel
    Left = 168
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 24
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel8: TPanel
    Left = 192
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 25
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel9: TPanel
    Left = 216
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 26
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel10: TPanel
    Left = 240
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 27
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel11: TPanel
    Left = 264
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 28
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel12: TPanel
    Left = 288
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 29
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel13: TPanel
    Left = 312
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 30
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel14: TPanel
    Left = 336
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 31
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel15: TPanel
    Left = 360
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 32
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel16: TPanel
    Left = 384
    Top = 208
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 33
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel17: TPanel
    Left = 24
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 34
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel18: TPanel
    Left = 48
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 35
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel19: TPanel
    Left = 72
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 36
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel20: TPanel
    Left = 96
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 37
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel21: TPanel
    Left = 120
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 38
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel22: TPanel
    Left = 144
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 39
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel23: TPanel
    Left = 168
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 40
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel24: TPanel
    Left = 192
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 41
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel25: TPanel
    Left = 216
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 42
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel26: TPanel
    Left = 240
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 43
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel27: TPanel
    Left = 264
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 44
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel28: TPanel
    Left = 288
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 45
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel29: TPanel
    Left = 312
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 46
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel30: TPanel
    Left = 336
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 47
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel31: TPanel
    Left = 360
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 48
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel32: TPanel
    Left = 384
    Top = 240
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 49
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel33: TPanel
    Left = 24
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 50
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel34: TPanel
    Left = 48
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 51
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel35: TPanel
    Left = 72
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 52
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel36: TPanel
    Left = 96
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 53
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel37: TPanel
    Left = 120
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 54
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel38: TPanel
    Left = 144
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 55
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel39: TPanel
    Left = 168
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 56
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel40: TPanel
    Left = 192
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 57
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel41: TPanel
    Left = 216
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 58
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel42: TPanel
    Left = 240
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 59
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel43: TPanel
    Left = 264
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 60
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel44: TPanel
    Left = 288
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 61
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel45: TPanel
    Left = 312
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 62
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel46: TPanel
    Left = 336
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 63
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel47: TPanel
    Left = 360
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 64
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel48: TPanel
    Left = 384
    Top = 272
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 65
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel49: TPanel
    Left = 24
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 66
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel50: TPanel
    Left = 48
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 67
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel51: TPanel
    Left = 72
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 68
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel52: TPanel
    Left = 96
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 69
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel53: TPanel
    Left = 120
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 70
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel54: TPanel
    Left = 144
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 71
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel55: TPanel
    Left = 168
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 72
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel56: TPanel
    Left = 192
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 73
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel57: TPanel
    Left = 216
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 74
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel58: TPanel
    Left = 240
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 75
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel59: TPanel
    Left = 264
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 76
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel60: TPanel
    Left = 288
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 77
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel61: TPanel
    Left = 312
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 78
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel62: TPanel
    Left = 336
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 79
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel63: TPanel
    Left = 360
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 80
    OnMouseMove = pCurCol0MouseMove
  end
  object Panel64: TPanel
    Left = 384
    Top = 304
    Width = 20
    Height = 20
    BevelOuter = bvLowered
    BevelWidth = 2
    TabOrder = 81
    OnMouseMove = pCurCol0MouseMove
  end
  object MainMenu1: TMainMenu
    Left = 272
    object File1: TMenuItem
      Caption = 'File'
      object Exit: TMenuItem
        Caption = 'Exit'
        OnClick = ExitClick
      end
    end
    object View1: TMenuItem
      Caption = 'View'
      object AutoRefresh1: TMenuItem
        Caption = 'Auto Refresh'
        Enabled = False
        OnClick = AutoRefresh1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Refresh1: TMenuItem
        Caption = 'Refresh'
        ShortCut = 16500
        OnClick = Refresh1Click
      end
    end
  end
end
