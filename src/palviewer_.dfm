object paletteviewer: Tpaletteviewer
  Left = 892
  Top = 215
  BorderStyle = bsToolWindow
  Caption = 'Palette Viewer'
  ClientHeight = 345
  ClientWidth = 542
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
  object gCurCol: TGroupBox
    Left = 200
    Top = 2
    Width = 257
    Height = 135
    Caption = 'Details'
    TabOrder = 0
    object lCurColR: TLabel
      Left = 168
      Top = 20
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
      Left = 168
      Top = 44
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
      Left = 168
      Top = 68
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
      Left = 168
      Top = 100
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
    object paCurCol: TPaintBox
      Left = 8
      Top = 16
      Width = 105
      Height = 105
    end
    object eCurColR: TStaticText
      Left = 128
      Top = 16
      Width = 33
      Height = 21
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 0
    end
    object eCurColG: TStaticText
      Left = 128
      Top = 40
      Width = 33
      Height = 21
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 1
    end
    object eCurColB: TStaticText
      Left = 128
      Top = 64
      Width = 33
      Height = 21
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 2
    end
    object eCurColIdx: TStaticText
      Left = 128
      Top = 96
      Width = 33
      Height = 21
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 3
    end
  end
  object gTMS: TGroupBox
    Left = 2
    Top = 2
    Width = 193
    Height = 188
    Caption = 'TMS 9918/9928A Palette'
    TabOrder = 1
    object lVVPal4: TLabel
      Left = 8
      Top = 152
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
      Top = 112
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
      Top = 72
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
      Top = 32
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
    object paTCol1: TPaintBox
      Left = 24
      Top = 24
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
    object paTCol2: TPaintBox
      Left = 64
      Top = 24
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
    object paTCol3: TPaintBox
      Left = 104
      Top = 24
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
    object paTCol4: TPaintBox
      Left = 144
      Top = 24
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
    object paTCol5: TPaintBox
      Left = 24
      Top = 64
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
    object paTCol6: TPaintBox
      Left = 64
      Top = 64
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
    object paTCol7: TPaintBox
      Left = 104
      Top = 64
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
    object paTCol8: TPaintBox
      Left = 144
      Top = 64
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
    object paTCol9: TPaintBox
      Left = 24
      Top = 104
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
    object paTCol10: TPaintBox
      Left = 64
      Top = 104
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
    object paTCol11: TPaintBox
      Left = 104
      Top = 104
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
    object paTCol12: TPaintBox
      Left = 144
      Top = 104
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
    object paTCol13: TPaintBox
      Left = 24
      Top = 144
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
    object paTCol14: TPaintBox
      Left = 64
      Top = 144
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
    object paTCol15: TPaintBox
      Left = 104
      Top = 144
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
    object paTCol16: TPaintBox
      Left = 144
      Top = 144
      Width = 32
      Height = 32
      OnMouseMove = pCurCol0MouseMove
    end
  end
  object gF18A: TGroupBox
    Left = 2
    Top = 192
    Width = 535
    Height = 145
    Caption = 'F18A Palette'
    Enabled = False
    TabOrder = 2
    object Label1: TLabel
      Left = 8
      Top = 20
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
      Top = 52
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
      Top = 84
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
      Top = 116
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
    object paFCol1: TPaintBox
      Left = 24
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol2: TPaintBox
      Left = 54
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol3: TPaintBox
      Left = 86
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol4: TPaintBox
      Left = 118
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol5: TPaintBox
      Left = 150
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol6: TPaintBox
      Left = 182
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol7: TPaintBox
      Left = 214
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol8: TPaintBox
      Left = 246
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol9: TPaintBox
      Left = 278
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol10: TPaintBox
      Left = 308
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol11: TPaintBox
      Left = 340
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol12: TPaintBox
      Left = 372
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol13: TPaintBox
      Left = 404
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol14: TPaintBox
      Left = 436
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol15: TPaintBox
      Left = 468
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol16: TPaintBox
      Left = 500
      Top = 16
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol32: TPaintBox
      Left = 500
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol31: TPaintBox
      Left = 468
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol30: TPaintBox
      Left = 436
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol29: TPaintBox
      Left = 404
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol28: TPaintBox
      Left = 372
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol27: TPaintBox
      Left = 340
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol26: TPaintBox
      Left = 308
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol25: TPaintBox
      Left = 278
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol24: TPaintBox
      Left = 246
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol23: TPaintBox
      Left = 214
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol22: TPaintBox
      Left = 182
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol21: TPaintBox
      Left = 150
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol20: TPaintBox
      Left = 118
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol19: TPaintBox
      Left = 86
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol18: TPaintBox
      Left = 54
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol17: TPaintBox
      Left = 24
      Top = 48
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol48: TPaintBox
      Left = 500
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol47: TPaintBox
      Left = 468
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol46: TPaintBox
      Left = 436
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol45: TPaintBox
      Left = 404
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol44: TPaintBox
      Left = 372
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol43: TPaintBox
      Left = 340
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol42: TPaintBox
      Left = 308
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol41: TPaintBox
      Left = 278
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol40: TPaintBox
      Left = 246
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol39: TPaintBox
      Left = 214
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol38: TPaintBox
      Left = 182
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol37: TPaintBox
      Left = 150
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol36: TPaintBox
      Left = 118
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol35: TPaintBox
      Left = 86
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol34: TPaintBox
      Left = 54
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol33: TPaintBox
      Left = 24
      Top = 80
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol64: TPaintBox
      Left = 500
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol63: TPaintBox
      Left = 468
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol62: TPaintBox
      Left = 436
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol61: TPaintBox
      Left = 404
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol60: TPaintBox
      Left = 372
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol59: TPaintBox
      Left = 340
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol58: TPaintBox
      Left = 308
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol57: TPaintBox
      Left = 278
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol56: TPaintBox
      Left = 246
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol55: TPaintBox
      Left = 214
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol54: TPaintBox
      Left = 182
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol53: TPaintBox
      Left = 150
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol52: TPaintBox
      Left = 118
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol51: TPaintBox
      Left = 86
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol50: TPaintBox
      Left = 54
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
    object paFCol49: TPaintBox
      Left = 24
      Top = 112
      Width = 24
      Height = 24
      OnMouseMove = pCurCol0MouseMove
    end
  end
  object MainMenu1: TMainMenu
    Left = 408
    Top = 8
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
