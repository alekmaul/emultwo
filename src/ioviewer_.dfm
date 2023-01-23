object iomapviewer: Tiomapviewer
  Left = 910
  Top = 267
  BorderStyle = bsToolWindow
  Caption = 'I/O Map Viewer'
  ClientHeight = 360
  ClientWidth = 638
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
    Left = 424
    Top = 0
    Width = 209
    Height = 345
    Caption = 'PSG AY8910'
    TabOrder = 0
    object Label11: TLabel
      Left = 40
      Top = 92
      Width = 27
      Height = 13
      Caption = 'Latch'
    end
    object Label12: TLabel
      Left = 40
      Top = 113
      Width = 14
      Height = 13
      Caption = 'R0'
    end
    object Label13: TLabel
      Left = 40
      Top = 134
      Width = 14
      Height = 13
      Caption = 'R1'
    end
    object Label14: TLabel
      Left = 40
      Top = 153
      Width = 14
      Height = 13
      Caption = 'R2'
    end
    object Label15: TLabel
      Left = 40
      Top = 174
      Width = 14
      Height = 13
      Caption = 'R3'
    end
    object Label16: TLabel
      Left = 40
      Top = 194
      Width = 14
      Height = 13
      Caption = 'R4'
    end
    object Label17: TLabel
      Left = 40
      Top = 212
      Width = 14
      Height = 13
      Caption = 'R5'
    end
    object Label18: TLabel
      Left = 40
      Top = 234
      Width = 14
      Height = 13
      Caption = 'R6'
    end
    object Label19: TLabel
      Left = 40
      Top = 254
      Width = 14
      Height = 13
      Caption = 'R7'
    end
    object Label20: TLabel
      Left = 96
      Top = 113
      Width = 14
      Height = 13
      Caption = 'R8'
    end
    object Label21: TLabel
      Left = 96
      Top = 134
      Width = 14
      Height = 13
      Caption = 'R9'
    end
    object Label22: TLabel
      Left = 96
      Top = 153
      Width = 20
      Height = 13
      Caption = 'R10'
    end
    object Label23: TLabel
      Left = 96
      Top = 174
      Width = 20
      Height = 13
      Caption = 'R11'
    end
    object Label24: TLabel
      Left = 96
      Top = 194
      Width = 20
      Height = 13
      Caption = 'R12'
    end
    object Label25: TLabel
      Left = 96
      Top = 212
      Width = 20
      Height = 13
      Caption = 'R13'
    end
    object Label26: TLabel
      Left = 96
      Top = 234
      Width = 20
      Height = 13
      Caption = 'R14'
    end
    object Label27: TLabel
      Left = 96
      Top = 254
      Width = 20
      Height = 13
      Caption = 'R15'
    end
    object ePSGR0: TEdit
      Left = 8
      Top = 88
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 0
      Text = 'ePSGR0'
    end
    object ePSGR1: TEdit
      Left = 8
      Top = 109
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 1
      Text = 'ePSGR1'
    end
    object ePSGR2: TEdit
      Left = 8
      Top = 130
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 2
      Text = 'ePSGR2'
    end
    object ePSGR3: TEdit
      Left = 8
      Top = 149
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 3
      Text = 'ePSGR3'
    end
    object ePSGR4: TEdit
      Left = 8
      Top = 170
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 4
      Text = 'ePSGR4'
    end
    object ePSGR5: TEdit
      Left = 8
      Top = 190
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 5
      Text = 'ePSGR5'
    end
    object ePSGR6: TEdit
      Left = 8
      Top = 208
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 6
      Text = 'ePSGR6'
    end
    object ePSGR7: TEdit
      Left = 8
      Top = 230
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 7
      Text = 'ePSGR7'
    end
    object ePSGR8: TEdit
      Left = 8
      Top = 250
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 8
      Text = 'ePSGR8'
    end
    object ePSGR9: TEdit
      Left = 64
      Top = 109
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 9
      Text = 'ePSGR9'
    end
    object ePSGRA: TEdit
      Left = 64
      Top = 130
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 10
      Text = 'ePSGRA'
    end
    object ePSGRB: TEdit
      Left = 64
      Top = 149
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 11
      Text = 'ePSGRB'
    end
    object ePSGRC: TEdit
      Left = 64
      Top = 170
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 12
      Text = 'ePSGRC'
    end
    object ePSGRD: TEdit
      Left = 64
      Top = 190
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 13
      Text = 'ePSGRD'
    end
    object ePSGRE: TEdit
      Left = 64
      Top = 208
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 14
      Text = 'ePSGRE'
    end
    object ePSGRF: TEdit
      Left = 64
      Top = 230
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 15
      Text = 'ePSGRF'
    end
    object ePSGRG: TEdit
      Left = 64
      Top = 250
      Width = 28
      Height = 21
      ReadOnly = True
      TabOrder = 16
      Text = 'ePSGRG'
    end
  end
  object GroupBox1: TGroupBox
    Left = 2
    Top = 0
    Width = 417
    Height = 73
    Caption = 'Joystick'
    TabOrder = 1
    object Label9: TLabel
      Left = 8
      Top = 20
      Width = 35
      Height = 13
      Caption = 'Port #1'
    end
    object lJP1: TLabel
      Left = 8
      Top = 44
      Width = 35
      Height = 13
      Caption = 'Port #2'
    end
    object eJoyP0E: TStaticText
      Left = 128
      Top = 16
      Width = 281
      Height = 21
      AutoSize = False
      BorderStyle = sbsSunken
      TabOrder = 0
    end
    object eJoyP1E: TStaticText
      Left = 128
      Top = 40
      Width = 281
      Height = 21
      AutoSize = False
      BorderStyle = sbsSunken
      TabOrder = 1
    end
    object eJoyP0: TStaticText
      Left = 48
      Top = 18
      Width = 49
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      TabOrder = 2
    end
    object eJoyP1: TStaticText
      Left = 48
      Top = 42
      Width = 49
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      TabOrder = 3
    end
  end
  object GroupBox2: TGroupBox
    Left = 2
    Top = 80
    Width = 311
    Height = 73
    Caption = 'Spinner'
    TabOrder = 2
    object Label1: TLabel
      Left = 8
      Top = 20
      Width = 35
      Height = 13
      Caption = 'Port #1'
    end
    object Label2: TLabel
      Left = 8
      Top = 44
      Width = 35
      Height = 13
      Caption = 'Port #2'
    end
    object eSpinP0: TStaticText
      Left = 48
      Top = 18
      Width = 49
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      TabOrder = 0
    end
    object eSpinP1: TStaticText
      Left = 48
      Top = 42
      Width = 49
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      TabOrder = 1
    end
  end
  object GroupBox3: TGroupBox
    Left = 2
    Top = 160
    Width = 415
    Height = 161
    Caption = 'Memory'
    TabOrder = 3
    object Label3: TLabel
      Left = 48
      Top = 20
      Width = 40
      Height = 13
      Caption = '$20 Port'
    end
    object Label4: TLabel
      Left = 48
      Top = 44
      Width = 40
      Height = 13
      Caption = '$53 Port'
    end
    object Label5: TLabel
      Left = 48
      Top = 68
      Width = 40
      Height = 13
      Caption = '$60 Port'
    end
    object Label6: TLabel
      Left = 48
      Top = 140
      Width = 74
      Height = 13
      Caption = 'MegaCart Bank'
    end
    object Label7: TLabel
      Left = 48
      Top = 92
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
      Top = 42
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
      Top = 66
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
      Top = 138
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 3
    end
    object eP60exp: TStaticText
      Left = 126
      Top = 64
      Width = 281
      Height = 21
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 4
    end
    object eP53exp: TStaticText
      Left = 126
      Top = 40
      Width = 281
      Height = 21
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 5
    end
    object eP20exp: TStaticText
      Left = 126
      Top = 16
      Width = 281
      Height = 21
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 6
    end
    object eSGMRAM: TStaticText
      Left = 8
      Top = 90
      Width = 32
      Height = 18
      AutoSize = False
      BorderStyle = sbsSunken
      Color = clWhite
      ParentColor = False
      TabOrder = 7
    end
  end
  object MainMenu1: TMainMenu
    Top = 320
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
