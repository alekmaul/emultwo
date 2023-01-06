object hardware: Thardware
  Left = 715
  Top = 180
  Width = 340
  Height = 200
  Caption = 'Hardware'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object OK: TButton
    Left = 237
    Top = 129
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    TabOrder = 0
    OnClick = OKClick
  end
  object RadioButton1: TRadioButton
    Left = 8
    Top = 16
    Width = 145
    Height = 25
    Caption = 'Coleco'
    Checked = True
    TabOrder = 1
    TabStop = True
    OnClick = RadioButton1Click
  end
  object RadioButton2: TRadioButton
    Left = 8
    Top = 48
    Width = 145
    Height = 25
    Caption = 'Adam'
    TabOrder = 2
    OnClick = RadioButton2Click
  end
  object chkNTSC: TCheckBox
    Left = 216
    Top = 16
    Width = 97
    Height = 17
    Caption = 'NTSC'
    Checked = True
    State = cbChecked
    TabOrder = 3
    OnClick = chkNTSCClick
  end
  object chkSGM: TCheckBox
    Left = 216
    Top = 40
    Width = 97
    Height = 17
    Caption = 'SGM'
    TabOrder = 4
  end
end
