object iomapviewer: Tiomapviewer
  Left = 1180
  Top = 124
  BorderStyle = bsToolWindow
  Caption = 'I/O Map Viewer'
  ClientHeight = 243
  ClientWidth = 617
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
  object gPSG: TGroupBox
    Left = 412
    Top = 0
    Width = 201
    Height = 241
    Caption = 'PSG AY8910'
    TabOrder = 0
    object Label11: TLabel
      Left = 46
      Top = 20
      Width = 27
      Height = 13
      Caption = 'Latch'
    end
    object Label12: TLabel
      Left = 46
      Top = 44
      Width = 51
      Height = 13
      Caption = '0 Freq A.lo'
    end
    object Label13: TLabel
      Left = 46
      Top = 68
      Width = 51
      Height = 13
      Caption = '1 Freq A.hi'
    end
    object Label14: TLabel
      Left = 46
      Top = 92
      Width = 51
      Height = 13
      Caption = '2 Freq B.lo'
    end
    object Label15: TLabel
      Left = 46
      Top = 116
      Width = 51
      Height = 13
      Caption = '3 Freq B.hi'
    end
    object Label16: TLabel
      Left = 46
      Top = 140
      Width = 51
      Height = 13
      Caption = '4 Freq C.lo'
    end
    object Label17: TLabel
      Left = 46
      Top = 164
      Width = 51
      Height = 13
      Caption = '5 Freq C.hi'
    end
    object Label18: TLabel
      Left = 46
      Top = 188
      Width = 54
      Height = 13
      Caption = '6 Freq Dish'
    end
    object Label19: TLabel
      Left = 46
      Top = 212
      Width = 24
      Height = 13
      Caption = '7 Ctrl'
    end
    object Label8: TLabel
      Left = 142
      Top = 44
      Width = 34
      Height = 13
      Caption = '8 Vol A'
    end
    object Label10: TLabel
      Left = 142
      Top = 68
      Width = 34
      Height = 13
      Caption = '9 Vol B'
    end
    object Label20: TLabel
      Left = 142
      Top = 92
      Width = 35
      Height = 13
      Caption = 'A Vol C'
    end
    object Label21: TLabel
      Left = 142
      Top = 116
      Width = 53
      Height = 13
      Caption = 'B Freq H.lo'
    end
    object Label22: TLabel
      Left = 142
      Top = 140
      Width = 53
      Height = 13
      Caption = 'C Freq H.hi'
    end
    object Label23: TLabel
      Left = 142
      Top = 164
      Width = 35
      Height = 13
      Caption = 'D Huell'
    end
    object Label24: TLabel
      Left = 142
      Top = 188
      Width = 36
      Height = 13
      Caption = 'E None'
    end
    object Label25: TLabel
      Left = 142
      Top = 212
      Width = 35
      Height = 13
      Caption = 'F None'
    end
    object ePSGRL: TStaticText
      Left = 8
      Top = 18
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 0
    end
    object ePSGR0: TStaticText
      Left = 8
      Top = 42
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 1
    end
    object ePSGR1: TStaticText
      Left = 8
      Top = 66
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 2
    end
    object ePSGR2: TStaticText
      Left = 8
      Top = 90
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 3
    end
    object ePSGR3: TStaticText
      Left = 8
      Top = 114
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 4
    end
    object ePSGR4: TStaticText
      Left = 8
      Top = 138
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 5
    end
    object ePSGR5: TStaticText
      Left = 8
      Top = 162
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 6
    end
    object ePSGR6: TStaticText
      Left = 8
      Top = 186
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 7
    end
    object ePSGR7: TStaticText
      Left = 8
      Top = 210
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 8
    end
    object ePSGR8: TStaticText
      Left = 104
      Top = 42
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 9
    end
    object ePSGR9: TStaticText
      Left = 104
      Top = 66
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 10
    end
    object ePSGRA: TStaticText
      Left = 104
      Top = 90
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 11
    end
    object ePSGRB: TStaticText
      Left = 104
      Top = 114
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 12
    end
    object ePSGRC: TStaticText
      Left = 104
      Top = 138
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 13
    end
    object ePSGRD: TStaticText
      Left = 104
      Top = 162
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 14
    end
    object ePSGRE: TStaticText
      Left = 104
      Top = 186
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 15
    end
    object ePSGRF: TStaticText
      Left = 104
      Top = 210
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 16
    end
  end
  object GroupBox1: TGroupBox
    Left = 2
    Top = 0
    Width = 407
    Height = 110
    Caption = 'Controls'
    TabOrder = 1
    object Label9: TLabel
      Left = 56
      Top = 20
      Width = 76
      Height = 13
      Caption = 'Joystick Port #1'
    end
    object lJP1: TLabel
      Tag = 42
      Left = 56
      Top = 44
      Width = 76
      Height = 13
      Caption = 'Joystick Port #2'
    end
    object Label1: TLabel
      Left = 56
      Top = 64
      Width = 74
      Height = 13
      Caption = 'Spinner Port #1'
    end
    object Label2: TLabel
      Left = 56
      Top = 86
      Width = 74
      Height = 13
      Caption = 'Spinner Port #2'
    end
    object eJoyP0E: TStaticText
      Left = 144
      Top = 18
      Width = 257
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 0
    end
    object eJoyP1E: TStaticText
      Left = 144
      Top = 40
      Width = 257
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 1
    end
    object eJoyP0: TStaticText
      Left = 8
      Top = 18
      Width = 44
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 2
    end
    object eJoyP1: TStaticText
      Left = 8
      Top = 40
      Width = 44
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 3
    end
    object eSpinP0: TStaticText
      Left = 8
      Top = 62
      Width = 44
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 4
    end
    object eSpinP1: TStaticText
      Left = 8
      Top = 84
      Width = 44
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 5
    end
  end
  object GroupBox3: TGroupBox
    Left = 2
    Top = 110
    Width = 407
    Height = 131
    Caption = 'Memory'
    TabOrder = 2
    object Label3: TLabel
      Left = 48
      Top = 20
      Width = 40
      Height = 13
      Caption = '$20 Port'
    end
    object Label4: TLabel
      Left = 48
      Top = 42
      Width = 40
      Height = 13
      Caption = '$53 Port'
    end
    object Label5: TLabel
      Left = 48
      Top = 64
      Width = 40
      Height = 13
      Caption = '$60 Port'
    end
    object Label6: TLabel
      Left = 48
      Top = 108
      Width = 74
      Height = 13
      Caption = 'MegaCart Bank'
    end
    object Label7: TLabel
      Left = 48
      Top = 86
      Width = 51
      Height = 13
      Caption = 'SGM RAM'
    end
    object eP20: TStaticText
      Left = 8
      Top = 18
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 0
    end
    object eP53: TStaticText
      Left = 8
      Top = 40
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 1
    end
    object eP60: TStaticText
      Left = 8
      Top = 62
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 2
    end
    object eBank: TStaticText
      Left = 8
      Top = 106
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 3
    end
    object eP60exp: TStaticText
      Left = 118
      Top = 62
      Width = 284
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 4
    end
    object eP53exp: TStaticText
      Left = 118
      Top = 40
      Width = 284
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 5
    end
    object eSGMRAM: TStaticText
      Left = 8
      Top = 84
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 6
    end
  end
  object MainMenu1: TMainMenu
    Left = 360
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
