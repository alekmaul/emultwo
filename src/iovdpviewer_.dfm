object iovdpviewer: Tiovdpviewer
  Left = 949
  Top = 189
  BorderStyle = bsToolWindow
  Caption = 'I/O VDP Viewer'
  ClientHeight = 416
  ClientWidth = 613
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
    Width = 135
    Height = 247
    Caption = 'TMS VDP Registers'
    Enabled = False
    TabOrder = 0
    object lVDPR0: TLabel
      Left = 40
      Top = 18
      Width = 57
      Height = 13
      Caption = '$0 Control 0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 40
      Top = 40
      Width = 57
      Height = 13
      Caption = '$1 Control 1'
    end
    object Label3: TLabel
      Left = 40
      Top = 62
      Width = 80
      Height = 13
      Caption = '$2 Pattern Name'
    end
    object Label4: TLabel
      Left = 40
      Top = 84
      Width = 69
      Height = 13
      Caption = '$3 Color Table'
    end
    object Label5: TLabel
      Left = 40
      Top = 106
      Width = 72
      Height = 13
      Caption = '$4 Pattern Gen'
    end
    object Label6: TLabel
      Left = 40
      Top = 128
      Width = 61
      Height = 13
      Caption = '$5 Sprite Attr'
    end
    object Label7: TLabel
      Left = 40
      Top = 150
      Width = 65
      Height = 13
      Caption = '$6 Sprite Gen'
    end
    object Label8: TLabel
      Left = 40
      Top = 172
      Width = 44
      Height = 13
      Caption = '$7 Colors'
    end
    object lVDPStat: TLabel
      Left = 40
      Top = 202
      Width = 30
      Height = 13
      Caption = 'Status'
    end
    object lVDPlatch: TLabel
      Left = 40
      Top = 224
      Width = 27
      Height = 13
      Caption = 'Latch'
    end
    object Bevel1: TBevel
      Left = 8
      Top = 194
      Width = 121
      Height = 9
      Shape = bsTopLine
    end
    object eVDPlatch: TStaticText
      Left = 8
      Top = 222
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 0
    end
    object eVDPR0: TStaticText
      Left = 8
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
      Left = 8
      Top = 200
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
      Left = 8
      Top = 170
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
      Left = 8
      Top = 38
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
      Left = 8
      Top = 60
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
      Left = 8
      Top = 82
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
      Left = 8
      Top = 104
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
      Left = 8
      Top = 126
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
      Left = 8
      Top = 148
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
    Left = 140
    Top = 2
    Width = 469
    Height = 247
    Caption = 'F18A VDP Registers'
    Enabled = False
    TabOrder = 1
    object lblReg8: TLabel
      Left = 160
      Top = 18
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
      Left = 160
      Top = 40
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
      Left = 160
      Top = 62
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
      Left = 160
      Top = 84
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
      Left = 160
      Top = 106
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
      Left = 392
      Top = 50
      Width = 64
      Height = 13
      Caption = '$19/$1A TL2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label14: TLabel
      Left = 392
      Top = 30
      Width = 65
      Height = 13
      Caption = '$1B/$1C TL1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label1: TLabel
      Left = 160
      Top = 128
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
      Left = 160
      Top = 150
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
      Left = 160
      Top = 172
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
      Left = 218
      Top = 18
      Width = 42
      Height = 13
      Caption = '$20 Bmp'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label19: TLabel
      Left = 218
      Top = 40
      Width = 52
      Height = 13
      Caption = '$21 Bmp X'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label20: TLabel
      Left = 218
      Top = 62
      Width = 52
      Height = 13
      Caption = '$22 Bmp Y'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label21: TLabel
      Left = 218
      Top = 84
      Width = 56
      Height = 13
      Caption = '$23 Bmp W'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label22: TLabel
      Left = 218
      Top = 106
      Width = 53
      Height = 13
      Caption = '$24 Bmp H'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label23: TLabel
      Left = 218
      Top = 128
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
      Left = 304
      Top = 18
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
      Left = 304
      Top = 40
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
      Left = 304
      Top = 62
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
      Left = 304
      Top = 84
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
      Left = 304
      Top = 106
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
      Left = 304
      Top = 128
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
      Left = 304
      Top = 150
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
      Left = 304
      Top = 172
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
    object Label52: TLabel
      Left = 328
      Top = 12
      Width = 72
      Height = 13
      Caption = 'F18A BG Scroll'
    end
    object Label15: TLabel
      Left = 40
      Top = 18
      Width = 57
      Height = 13
      Caption = '$0 Control 0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label16: TLabel
      Left = 40
      Top = 40
      Width = 57
      Height = 13
      Caption = '$1 Control 1'
    end
    object Label42: TLabel
      Left = 40
      Top = 62
      Width = 80
      Height = 13
      Caption = '$2 Pattern Name'
    end
    object Label43: TLabel
      Left = 40
      Top = 84
      Width = 69
      Height = 13
      Caption = '$3 Color Table'
    end
    object Label44: TLabel
      Left = 40
      Top = 106
      Width = 72
      Height = 13
      Caption = '$4 Pattern Gen'
    end
    object Label45: TLabel
      Left = 40
      Top = 128
      Width = 61
      Height = 13
      Caption = '$5 Sprite Attr'
    end
    object Label46: TLabel
      Left = 40
      Top = 150
      Width = 65
      Height = 13
      Caption = '$6 Sprite Gen'
    end
    object Label47: TLabel
      Left = 40
      Top = 172
      Width = 44
      Height = 13
      Caption = '$7 Colors'
    end
    object Label48: TLabel
      Left = 40
      Top = 202
      Width = 30
      Height = 13
      Caption = 'Status'
    end
    object Label49: TLabel
      Left = 40
      Top = 224
      Width = 27
      Height = 13
      Caption = 'Latch'
    end
    object Label51: TLabel
      Left = 160
      Top = 202
      Width = 65
      Height = 13
      Caption = 'ColTab Addr2'
    end
    object Label53: TLabel
      Left = 160
      Top = 224
      Width = 72
      Height = 13
      Caption = 'NamTab Addr2'
    end
    object Bevel2: TBevel
      Left = 8
      Top = 194
      Width = 433
      Height = 9
      Shape = bsTopLine
    end
    object StaticText1: TStaticText
      Tag = 10
      Left = 128
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
      Left = 128
      Top = 38
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
      Left = 128
      Top = 60
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
      Left = 128
      Top = 82
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
      Left = 128
      Top = 104
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
      Left = 328
      Top = 48
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
      Left = 360
      Top = 48
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
      Left = 328
      Top = 28
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
      Left = 360
      Top = 28
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
      Left = 128
      Top = 126
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
      Left = 128
      Top = 148
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
      Left = 128
      Top = 170
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
      Left = 188
      Top = 16
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
      Left = 188
      Top = 38
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
      Left = 188
      Top = 60
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
      Left = 188
      Top = 82
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
      Left = 188
      Top = 104
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
      Left = 188
      Top = 126
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
      Left = 274
      Top = 16
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
      Left = 274
      Top = 38
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
      Left = 274
      Top = 60
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
      Left = 274
      Top = 82
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
      Left = 274
      Top = 104
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
      Left = 274
      Top = 126
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
      Left = 274
      Top = 148
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
      Left = 274
      Top = 170
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Caption = ' '
      Color = clWhite
      ParentColor = False
      TabOrder = 25
    end
    object StaticText27: TStaticText
      Left = 8
      Top = 16
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 26
    end
    object StaticText28: TStaticText
      Tag = 1
      Left = 8
      Top = 38
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 27
    end
    object StaticText29: TStaticText
      Tag = 2
      Left = 8
      Top = 60
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 28
    end
    object StaticText30: TStaticText
      Tag = 3
      Left = 8
      Top = 82
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 29
    end
    object StaticText31: TStaticText
      Tag = 4
      Left = 8
      Top = 104
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 30
    end
    object StaticText32: TStaticText
      Tag = 5
      Left = 8
      Top = 126
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 31
    end
    object StaticText33: TStaticText
      Tag = 6
      Left = 8
      Top = 148
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 32
    end
    object StaticText34: TStaticText
      Tag = 7
      Left = 8
      Top = 170
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 33
    end
    object eVDPStat1: TStaticText
      Left = 8
      Top = 200
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 34
    end
    object eVDPlatch1: TStaticText
      Left = 8
      Top = 222
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 35
    end
    object eF18ABGCol2: TStaticText
      Left = 88
      Top = 200
      Width = 68
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 36
    end
    object eVDPBGmap2: TStaticText
      Left = 88
      Top = 222
      Width = 68
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 37
    end
  end
  object GroupBox2: TGroupBox
    Left = 2
    Top = 250
    Width = 327
    Height = 159
    Caption = 'Global Information'
    TabOrder = 2
    object lVDPMode: TLabel
      Left = 58
      Top = 90
      Width = 69
      Height = 13
      Caption = 'M1 M2 M3 M4'
    end
    object Label32: TLabel
      Left = 58
      Top = 112
      Width = 41
      Height = 13
      Caption = 'FG Color'
    end
    object Label33: TLabel
      Left = 58
      Top = 134
      Width = 42
      Height = 13
      Caption = 'BG Color'
    end
    object Label34: TLabel
      Left = 112
      Top = 18
      Width = 51
      Height = 13
      Caption = 'Disp Mode'
    end
    object Label35: TLabel
      Left = 254
      Top = 62
      Width = 66
      Height = 13
      Caption = 'NamTab Addr'
    end
    object Label36: TLabel
      Left = 112
      Top = 40
      Width = 52
      Height = 13
      Caption = 'Sprites Opt'
    end
    object Label37: TLabel
      Left = 254
      Top = 18
      Width = 59
      Height = 13
      Caption = 'ColTab Addr'
    end
    object Label38: TLabel
      Left = 254
      Top = 40
      Width = 61
      Height = 13
      Caption = 'PatGen Addr'
    end
    object Label39: TLabel
      Left = 254
      Top = 84
      Width = 57
      Height = 13
      Caption = 'SprAttr Addr'
    end
    object Label40: TLabel
      Left = 254
      Top = 106
      Width = 61
      Height = 13
      Caption = 'SprGen Addr'
    end
    object Label41: TLabel
      Left = 112
      Top = 62
      Width = 65
      Height = 13
      Caption = 'Sprites Status'
    end
    object eVDPMode: TStaticText
      Left = 8
      Top = 16
      Width = 100
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 0
    end
    object eVDPOption: TStaticText
      Left = 8
      Top = 38
      Width = 100
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 1
    end
    object eVDPBGmap: TStaticText
      Left = 184
      Top = 60
      Width = 68
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 2
    end
    object eVDPBGcolor: TStaticText
      Left = 184
      Top = 16
      Width = 68
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 3
    end
    object eVDPBGtile: TStaticText
      Left = 184
      Top = 38
      Width = 68
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 4
    end
    object eVDPSPRdata: TStaticText
      Left = 184
      Top = 82
      Width = 68
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 5
    end
    object eVDPSPRtile: TStaticText
      Left = 184
      Top = 104
      Width = 68
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 6
    end
    object eVDPColF: TStaticText
      Left = 8
      Top = 110
      Width = 48
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
      Left = 8
      Top = 132
      Width = 48
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
      Left = 8
      Top = 88
      Width = 48
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 9
    end
    object eVDPStatus: TStaticText
      Left = 8
      Top = 60
      Width = 100
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 10
    end
  end
  object GroupBox1: TGroupBox
    Left = 332
    Top = 250
    Width = 277
    Height = 159
    Caption = 'F18A Gloabl Information'
    TabOrder = 3
    object Label57: TLabel
      Left = 8
      Top = 14
      Width = 42
      Height = 13
      Caption = 'Counters'
    end
    object Start: TLabel
      Left = 76
      Top = 30
      Width = 22
      Height = 13
      Caption = 'Start'
    end
    object Snap: TLabel
      Left = 76
      Top = 50
      Width = 25
      Height = 13
      Caption = 'Snap'
    end
    object Elapsed: TLabel
      Left = 76
      Top = 70
      Width = 38
      Height = 13
      Caption = 'Elapsed'
    end
    object Label58: TLabel
      Left = 124
      Top = 14
      Width = 33
      Height = 13
      Caption = 'Palette'
    end
    object Label59: TLabel
      Left = 158
      Top = 32
      Width = 19
      Height = 13
      Caption = 'TL1'
    end
    object Label60: TLabel
      Left = 158
      Top = 52
      Width = 19
      Height = 13
      Caption = 'TL2'
    end
    object Label61: TLabel
      Left = 158
      Top = 72
      Width = 14
      Height = 13
      Caption = 'SP'
    end
    object Label54: TLabel
      Left = 8
      Top = 96
      Width = 54
      Height = 13
      Caption = 'Color Mode'
    end
    object Label56: TLabel
      Left = 68
      Top = 114
      Width = 22
      Height = 13
      Caption = 'Tiles'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label55: TLabel
      Left = 68
      Top = 136
      Width = 32
      Height = 13
      Caption = 'Sprites'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label50: TLabel
      Left = 158
      Top = 114
      Width = 68
      Height = 13
      Caption = 'Status RegSel'
    end
    object eF18ACntSt: TStaticText
      Left = 8
      Top = 30
      Width = 66
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 0
    end
    object eF18ACntSn: TStaticText
      Left = 8
      Top = 50
      Width = 66
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 1
    end
    object eF18ACntEl: TStaticText
      Left = 8
      Top = 70
      Width = 66
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 2
    end
    object ePaSeTL1: TStaticText
      Left = 124
      Top = 30
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 3
    end
    object ePaSeTL2: TStaticText
      Left = 124
      Top = 50
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 4
    end
    object ePaSeSP: TStaticText
      Left = 124
      Top = 70
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 5
    end
    object eF18ATCMo: TStaticText
      Left = 8
      Top = 112
      Width = 60
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 6
    end
    object eF18ASCMo: TStaticText
      Left = 8
      Top = 132
      Width = 60
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 7
    end
    object chkT1On: TCheckBox
      Left = 188
      Top = 28
      Width = 57
      Height = 17
      Caption = 'TL1 on'
      TabOrder = 8
    end
    object chkT2On: TCheckBox
      Left = 188
      Top = 46
      Width = 57
      Height = 17
      Caption = 'TL2 on'
      TabOrder = 9
    end
    object chkBMOn: TCheckBox
      Left = 188
      Top = 62
      Width = 60
      Height = 17
      Caption = 'BMP on'
      TabOrder = 10
    end
    object eF18ASelNo: TStaticText
      Left = 124
      Top = 112
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 11
    end
    object eF18ALock: TStaticText
      Left = 124
      Top = 132
      Width = 57
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 12
    end
    object chkRow30: TCheckBox
      Left = 188
      Top = 78
      Width = 60
      Height = 17
      Caption = 'Row30'
      TabOrder = 13
    end
  end
  object MainMenu1: TMainMenu
    Left = 104
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
