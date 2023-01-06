object printviewer: Tprintviewer
  Left = 1048
  Top = 292
  BorderStyle = bsToolWindow
  Caption = 'Adam Printer Viewer'
  ClientHeight = 305
  ClientWidth = 422
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object printdump: TMemo
    Left = 2
    Top = 0
    Width = 415
    Height = 297
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    Lines.Strings = (
      '')
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
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
      end
    end
    object Edit1: TMenuItem
      Caption = 'Edit'
      object clearprint: TMenuItem
        Caption = 'Clear Print'
        OnClick = clearprintClick
      end
    end
  end
end
