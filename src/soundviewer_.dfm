object soundviewer: Tsoundviewer
  Left = 572
  Top = 232
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsToolWindow
  Caption = 'Sound Logger'
  ClientHeight = 397
  ClientWidth = 779
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
  object Label9: TLabel
    Left = 560
    Top = 0
    Width = 106
    Height = 13
    Caption = '1FF1 Sound Play Calls'
  end
  object Label10: TLabel
    Left = 560
    Top = 230
    Width = 101
    Height = 13
    Caption = 'Sound Table Content'
  end
  object mSNPlay: TMemo
    Left = 560
    Top = 16
    Width = 217
    Height = 209
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
    OnKeyDown = mSN76489KeyDown
  end
  object GroupBox1: TGroupBox
    Left = 328
    Top = 0
    Width = 225
    Height = 161
    Caption = 'SN76489 Address'
    TabOrder = 1
    object Label8: TLabel
      Left = 34
      Top = 140
      Width = 181
      Height = 13
      Caption = '$702A Noise Control Register Shadow'
    end
    object Label7: TLabel
      Left = 51
      Top = 116
      Width = 123
      Height = 13
      Caption = '$7028 Tone3 Gen Pointer'
    end
    object Label3: TLabel
      Left = 51
      Top = 20
      Width = 127
      Height = 13
      Caption = '$7020 Music Table Pointer'
    end
    object Label4: TLabel
      Left = 51
      Top = 44
      Width = 119
      Height = 13
      Caption = '$7022 Noise Gen Pointer'
    end
    object Label5: TLabel
      Left = 51
      Top = 68
      Width = 123
      Height = 13
      Caption = '$7024 Tone1 Gen Pointer'
    end
    object Label6: TLabel
      Left = 51
      Top = 92
      Width = 123
      Height = 13
      Caption = '$7026 Tone2 Gen Pointer'
    end
    object eNCRS: TStaticText
      Left = 8
      Top = 136
      Width = 25
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 0
    end
    object eT3GP: TStaticText
      Left = 9
      Top = 112
      Width = 40
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 1
    end
    object eT2GP: TStaticText
      Left = 9
      Top = 88
      Width = 40
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 2
    end
    object eT1GP: TStaticText
      Left = 9
      Top = 64
      Width = 40
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 3
    end
    object eNGP: TStaticText
      Left = 9
      Top = 40
      Width = 40
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 4
    end
    object eMPA: TStaticText
      Left = 9
      Top = 16
      Width = 40
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 5
    end
  end
  object gSN76: TGroupBox
    Left = 328
    Top = 168
    Width = 225
    Height = 225
    Caption = 'SN76489 Chip'
    TabOrder = 2
    object lTone0: TLabel
      Left = 184
      Top = 140
      Width = 31
      Height = 13
      Caption = 'Tone1'
    end
    object lsnLatch: TLabel
      Left = 40
      Top = 19
      Width = 27
      Height = 13
      Caption = 'Latch'
    end
    object Label36: TLabel
      Left = 40
      Top = 43
      Width = 14
      Height = 13
      Caption = 'R0'
    end
    object Label37: TLabel
      Left = 40
      Top = 67
      Width = 14
      Height = 13
      Caption = 'R1'
    end
    object Label38: TLabel
      Left = 40
      Top = 91
      Width = 14
      Height = 13
      Caption = 'R2'
    end
    object Label39: TLabel
      Left = 40
      Top = 115
      Width = 14
      Height = 13
      Caption = 'R3'
    end
    object Label40: TLabel
      Left = 104
      Top = 43
      Width = 14
      Height = 13
      Caption = 'R4'
    end
    object Label41: TLabel
      Left = 104
      Top = 67
      Width = 14
      Height = 13
      Caption = 'R5'
    end
    object Label42: TLabel
      Left = 104
      Top = 91
      Width = 14
      Height = 13
      Caption = 'R6'
    end
    object Label43: TLabel
      Left = 104
      Top = 115
      Width = 14
      Height = 13
      Caption = 'R7'
    end
    object lTone1: TLabel
      Left = 184
      Top = 160
      Width = 31
      Height = 13
      Caption = 'Tone2'
    end
    object lTone2: TLabel
      Left = 184
      Top = 180
      Width = 31
      Height = 13
      Caption = 'Tone3'
    end
    object lNoise: TLabel
      Left = 184
      Top = 200
      Width = 27
      Height = 13
      Caption = 'Noise'
    end
    object Label29: TLabel
      Left = 120
      Top = 19
      Width = 56
      Height = 13
      Caption = 'Noise Rand'
    end
    object pbTone0: TProgressBar
      Left = 8
      Top = 136
      Width = 170
      Height = 17
      Min = 0
      Max = 90
      TabOrder = 0
    end
    object pbTone1: TProgressBar
      Left = 8
      Top = 156
      Width = 170
      Height = 17
      Min = 0
      Max = 90
      TabOrder = 1
    end
    object pbTone2: TProgressBar
      Left = 8
      Top = 176
      Width = 170
      Height = 17
      Min = 0
      Max = 90
      TabOrder = 2
    end
    object pbNoise: TProgressBar
      Left = 8
      Top = 196
      Width = 170
      Height = 17
      Min = 0
      Max = 90
      TabOrder = 3
    end
    object tsnRLatch: TStaticText
      Left = 8
      Top = 16
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 4
    end
    object tsnR0: TStaticText
      Left = 8
      Top = 40
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 5
    end
    object tsnR1: TStaticText
      Left = 8
      Top = 64
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 6
    end
    object tsnR2: TStaticText
      Left = 8
      Top = 88
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 7
    end
    object tsnR3: TStaticText
      Left = 8
      Top = 112
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 8
    end
    object tsnRRand: TStaticText
      Left = 72
      Top = 16
      Width = 40
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 9
    end
    object tsnR4: TStaticText
      Left = 72
      Top = 40
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 10
    end
    object tsnR5: TStaticText
      Left = 72
      Top = 64
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 11
    end
    object tsnR7: TStaticText
      Left = 72
      Top = 112
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 12
    end
    object tsnR6: TStaticText
      Left = 72
      Top = 88
      Width = 28
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 13
    end
  end
  object mSTContent: TMemo
    Left = 560
    Top = 248
    Width = 217
    Height = 145
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 3
    OnKeyDown = mSN76489KeyDown
  end
  object Button1: TButton
    Left = 672
    Top = 228
    Width = 89
    Height = 17
    Caption = 'Find Content'
    TabOrder = 4
    OnClick = Button1Click
  end
  object GroupBox2: TGroupBox
    Left = 2
    Top = 0
    Width = 319
    Height = 393
    Caption = 'SN76489 Values'
    TabOrder = 5
    object mSN76489: TMemo
      Left = 8
      Top = 16
      Width = 305
      Height = 369
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      ScrollBars = ssVertical
      TabOrder = 0
      OnKeyDown = mSN76489KeyDown
    end
  end
  object MainMenu1: TMainMenu
    Left = 288
    object File1: TMenuItem
      Caption = 'File'
      object Copytoclipboard1: TMenuItem
        Caption = 'Copy values to clipboard'
        OnClick = Copytoclipboard1Click
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Exit: TMenuItem
        Caption = 'Exit'
        OnClick = ExitClick
      end
    end
    object Edit1: TMenuItem
      Caption = 'Edit'
      object clearsn: TMenuItem
        Caption = 'Clear SN'
        OnClick = clearsnClick
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object FlushSN1: TMenuItem
        Caption = 'Flush SN'
        OnClick = FlushSN1Click
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
