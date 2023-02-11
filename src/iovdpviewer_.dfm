object iovdpviewer: Tiovdpviewer
  Left = 468
  Top = 171
  Width = 670
  Height = 349
  Caption = 'I/O VDP Viewer'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object gVDP: TGroupBox
    Left = 2
    Top = 2
    Width = 159
    Height = 284
    Caption = 'TMS VDP Registers'
    TabOrder = 0
    object lVDPR0: TLabel
      Left = 8
      Top = 20
      Width = 76
      Height = 13
      Caption = '$0 CONTROL 0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 8
      Top = 44
      Width = 76
      Height = 13
      Caption = '$1 CONTROL 1'
    end
    object Label3: TLabel
      Left = 8
      Top = 68
      Width = 100
      Height = 13
      Caption = '$2 PATTERN NAME'
    end
    object Label4: TLabel
      Left = 8
      Top = 92
      Width = 89
      Height = 13
      Caption = '$3 COLOR TABLE'
    end
    object Label5: TLabel
      Left = 8
      Top = 116
      Width = 92
      Height = 13
      Caption = '$4 PATTERN GEN'
    end
    object Label6: TLabel
      Left = 8
      Top = 140
      Width = 86
      Height = 13
      Caption = '$5 SPRITE ATTR'
    end
    object Label7: TLabel
      Left = 8
      Top = 164
      Width = 80
      Height = 13
      Caption = '$6 SPRITE GEN'
    end
    object Label8: TLabel
      Left = 8
      Top = 188
      Width = 59
      Height = 13
      Caption = '$7 COLORS'
    end
    object lVDPStat: TLabel
      Left = 8
      Top = 220
      Width = 43
      Height = 13
      Caption = 'STATUS'
    end
    object lVDPlatch: TLabel
      Left = 8
      Top = 244
      Width = 35
      Height = 13
      Caption = 'LATCH'
    end
    object eVDPlatch: TStaticText
      Left = 112
      Top = 240
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 0
    end
    object eVDPR0: TStaticText
      Left = 112
      Top = 16
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 1
    end
    object eVDPStat: TStaticText
      Left = 112
      Top = 216
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 2
    end
    object eVDPR7: TStaticText
      Tag = 7
      Left = 112
      Top = 184
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 3
    end
    object eVDPR1: TStaticText
      Tag = 1
      Left = 112
      Top = 40
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 4
    end
    object eVDPR2: TStaticText
      Tag = 2
      Left = 112
      Top = 64
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 5
    end
    object eVDPR3: TStaticText
      Tag = 3
      Left = 112
      Top = 88
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 6
    end
    object eVDPR4: TStaticText
      Tag = 4
      Left = 112
      Top = 112
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 7
    end
    object eVDPR5: TStaticText
      Tag = 5
      Left = 112
      Top = 136
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 8
    end
    object eVDPR6: TStaticText
      Tag = 6
      Left = 112
      Top = 160
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 9
    end
  end
  object gF18A: TGroupBox
    Left = 168
    Top = 2
    Width = 209
    Height = 284
    Caption = 'F18A VDP Registers'
    Enabled = False
    TabOrder = 1
    object lblReg8: TLabel
      Left = 8
      Top = 20
      Width = 19
      Height = 13
      Caption = '$0A'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object lblReg9: TLabel
      Left = 8
      Top = 44
      Width = 19
      Height = 13
      Caption = '$0B'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label10: TLabel
      Left = 8
      Top = 68
      Width = 18
      Height = 13
      Caption = '$0F'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label11: TLabel
      Left = 8
      Top = 92
      Width = 18
      Height = 13
      Caption = '$13'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label12: TLabel
      Left = 8
      Top = 116
      Width = 18
      Height = 13
      Caption = '$18'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label13: TLabel
      Left = 8
      Top = 140
      Width = 18
      Height = 13
      Caption = '$19'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label14: TLabel
      Left = 8
      Top = 164
      Width = 19
      Height = 13
      Caption = '$1A'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label15: TLabel
      Left = 8
      Top = 188
      Width = 19
      Height = 13
      Caption = '$1B'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label16: TLabel
      Left = 8
      Top = 212
      Width = 19
      Height = 13
      Caption = '$1C'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label1: TLabel
      Left = 8
      Top = 236
      Width = 20
      Height = 13
      Caption = '$1D'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label9: TLabel
      Left = 8
      Top = 260
      Width = 19
      Height = 13
      Caption = '$1E'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label17: TLabel
      Left = 72
      Top = 20
      Width = 18
      Height = 13
      Caption = '$1F'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label18: TLabel
      Left = 72
      Top = 44
      Width = 18
      Height = 13
      Caption = '$20'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label19: TLabel
      Left = 72
      Top = 68
      Width = 18
      Height = 13
      Caption = '$21'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label20: TLabel
      Left = 72
      Top = 92
      Width = 18
      Height = 13
      Caption = '$22'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label21: TLabel
      Left = 72
      Top = 116
      Width = 18
      Height = 13
      Caption = '$23'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label22: TLabel
      Left = 72
      Top = 140
      Width = 18
      Height = 13
      Caption = '$24'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label23: TLabel
      Left = 72
      Top = 164
      Width = 18
      Height = 13
      Caption = '$2F'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label24: TLabel
      Left = 72
      Top = 188
      Width = 18
      Height = 13
      Caption = '$31'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label25: TLabel
      Left = 72
      Top = 212
      Width = 18
      Height = 13
      Caption = '$32'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label26: TLabel
      Left = 72
      Top = 236
      Width = 18
      Height = 13
      Caption = '$33'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label27: TLabel
      Left = 72
      Top = 260
      Width = 18
      Height = 13
      Caption = '$36'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label28: TLabel
      Left = 136
      Top = 20
      Width = 18
      Height = 13
      Caption = '$37'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label29: TLabel
      Left = 136
      Top = 44
      Width = 18
      Height = 13
      Caption = '$38'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label30: TLabel
      Left = 136
      Top = 68
      Width = 18
      Height = 13
      Caption = '$39'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label31: TLabel
      Left = 136
      Top = 92
      Width = 19
      Height = 13
      Caption = '$3A'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object StaticText1: TStaticText
      Tag = 10
      Left = 32
      Top = 16
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 0
    end
    object StaticText2: TStaticText
      Tag = 11
      Left = 32
      Top = 40
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 1
    end
    object StaticText3: TStaticText
      Tag = 15
      Left = 32
      Top = 64
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 2
    end
    object StaticText4: TStaticText
      Tag = 18
      Left = 32
      Top = 88
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 3
    end
    object StaticText5: TStaticText
      Tag = 24
      Left = 32
      Top = 112
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 4
    end
    object StaticText6: TStaticText
      Tag = 25
      Left = 32
      Top = 136
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 5
    end
    object StaticText7: TStaticText
      Tag = 26
      Left = 32
      Top = 160
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 6
    end
    object StaticText8: TStaticText
      Tag = 27
      Left = 32
      Top = 184
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 7
    end
    object StaticText9: TStaticText
      Tag = 28
      Left = 32
      Top = 208
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 8
    end
    object StaticText10: TStaticText
      Tag = 29
      Left = 32
      Top = 232
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 9
    end
    object StaticText11: TStaticText
      Tag = 30
      Left = 32
      Top = 256
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 10
    end
    object StaticText12: TStaticText
      Tag = 31
      Left = 96
      Top = 16
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 11
    end
    object StaticText13: TStaticText
      Tag = 32
      Left = 96
      Top = 40
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 12
    end
    object StaticText14: TStaticText
      Tag = 33
      Left = 96
      Top = 64
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 13
    end
    object StaticText15: TStaticText
      Tag = 34
      Left = 96
      Top = 88
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 14
    end
    object StaticText16: TStaticText
      Tag = 35
      Left = 96
      Top = 112
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 15
    end
    object StaticText17: TStaticText
      Tag = 36
      Left = 96
      Top = 136
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 16
    end
    object StaticText18: TStaticText
      Tag = 47
      Left = 96
      Top = 160
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 17
    end
    object StaticText19: TStaticText
      Tag = 49
      Left = 96
      Top = 184
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 18
    end
    object StaticText20: TStaticText
      Tag = 50
      Left = 96
      Top = 208
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 19
    end
    object StaticText21: TStaticText
      Tag = 51
      Left = 96
      Top = 232
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 20
    end
    object StaticText22: TStaticText
      Tag = 54
      Left = 96
      Top = 256
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 21
    end
    object StaticText23: TStaticText
      Tag = 55
      Left = 160
      Top = 16
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 22
    end
    object StaticText24: TStaticText
      Tag = 56
      Left = 160
      Top = 40
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 23
    end
    object StaticText25: TStaticText
      Tag = 57
      Left = 160
      Top = 64
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 24
    end
    object StaticText26: TStaticText
      Tag = 58
      Left = 160
      Top = 88
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 25
    end
  end
  object GroupBox2: TGroupBox
    Left = 384
    Top = 2
    Width = 265
    Height = 284
    Caption = 'Information'
    TabOrder = 2
    object lVDPMode: TLabel
      Left = 8
      Top = 228
      Width = 69
      Height = 13
      Caption = 'M1 M2 M3 M4'
    end
    object Label32: TLabel
      Left = 8
      Top = 196
      Width = 41
      Height = 13
      Caption = 'FG Color'
    end
    object Label33: TLabel
      Left = 144
      Top = 196
      Width = 42
      Height = 13
      Caption = 'BG Color'
    end
    object Label34: TLabel
      Left = 8
      Top = 20
      Width = 51
      Height = 13
      Caption = 'Disp Mode'
    end
    object Label35: TLabel
      Left = 8
      Top = 68
      Width = 90
      Height = 13
      Caption = 'Pattern Name Addr'
    end
    object Label36: TLabel
      Left = 8
      Top = 44
      Width = 52
      Height = 13
      Caption = 'Sprites Opt'
    end
    object Label37: TLabel
      Left = 8
      Top = 92
      Width = 71
      Height = 13
      Caption = 'Color Tab Addr'
    end
    object Label38: TLabel
      Left = 8
      Top = 116
      Width = 76
      Height = 13
      Caption = 'Pattern Gen Adr'
    end
    object Label39: TLabel
      Left = 8
      Top = 140
      Width = 63
      Height = 13
      Caption = 'Sprte Attr Adr'
    end
    object Label40: TLabel
      Left = 8
      Top = 164
      Width = 75
      Height = 13
      Caption = 'Sprite Gen Addr'
    end
    object Label41: TLabel
      Left = 8
      Top = 252
      Width = 30
      Height = 13
      Caption = 'Status'
    end
    object eVDPMode: TStaticText
      Left = 104
      Top = 16
      Width = 124
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = 'eVDPMode'
      Color = clWhite
      ParentColor = False
      TabOrder = 0
    end
    object eVDPOption: TStaticText
      Left = 104
      Top = 40
      Width = 124
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = 'eVDPOption'
      Color = clWhite
      ParentColor = False
      TabOrder = 1
    end
    object eVDPBGmap: TStaticText
      Left = 104
      Top = 64
      Width = 124
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = 'eVDPBGmap'
      Color = clWhite
      ParentColor = False
      TabOrder = 2
    end
    object eVDPBGcolor: TStaticText
      Left = 104
      Top = 88
      Width = 124
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = 'eVDPBGcolor'
      Color = clWhite
      ParentColor = False
      TabOrder = 3
    end
    object eVDPBGtile: TStaticText
      Left = 104
      Top = 112
      Width = 124
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = 'eVDPBGtile'
      Color = clWhite
      ParentColor = False
      TabOrder = 4
    end
    object eVDPSPRdata: TStaticText
      Left = 104
      Top = 136
      Width = 124
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = 'eVDPSPRdata'
      Color = clWhite
      ParentColor = False
      TabOrder = 5
    end
    object eVDPSPRtile: TStaticText
      Left = 104
      Top = 160
      Width = 124
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = 'eVDPSPRtile'
      Color = clWhite
      ParentColor = False
      TabOrder = 6
    end
    object eVDPColF: TStaticText
      Left = 56
      Top = 192
      Width = 60
      Height = 18
      Alignment = taCenter
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = 'BitsOn'
      Color = clCaptionText
      ParentColor = False
      TabOrder = 7
    end
    object eVDPColB: TStaticText
      Left = 193
      Top = 192
      Width = 60
      Height = 18
      Alignment = taCenter
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = 'BitsOff/BG'
      Color = clCaptionText
      ParentColor = False
      TabOrder = 8
    end
    object eVDPValMod: TStaticText
      Left = 80
      Top = 224
      Width = 105
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = 'eVDPValMod'
      Color = clWhite
      ParentColor = False
      TabOrder = 9
    end
    object eVDPStatus: TStaticText
      Left = 80
      Top = 248
      Width = 124
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = 'eVDPStatus'
      Color = clWhite
      ParentColor = False
      TabOrder = 10
    end
  end
  object MainMenu1: TMainMenu
    Left = 144
    Top = 65528
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
        OnAdvancedDrawItem = AutoRefresh1AdvancedDrawItem
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Refresh1: TMenuItem
        Caption = 'Refresh'
        ShortCut = 16500
      end
    end
  end
end
