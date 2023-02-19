object memory: Tmemory
  Left = 730
  Top = 241
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'memory'
  ClientHeight = 294
  ClientWidth = 521
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnPaint = FormPaint
  PixelsPerInch = 96
  TextHeight = 13
  object panMem: TPanel
    Left = 0
    Top = 0
    Width = 497
    Height = 265
    Alignment = taLeftJustify
    BevelOuter = bvNone
    Ctl3D = True
    FullRepaint = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentCtl3D = False
    ParentFont = False
    PopupMenu = PopupMenu1
    TabOrder = 0
    OnClick = panMemClick
    OnMouseMove = panMemMouseMove
  end
  object sbMemory: TScrollBar
    Left = 504
    Top = 0
    Width = 17
    Height = 266
    Align = alRight
    Enabled = False
    Kind = sbVertical
    LargeChange = 128
    Max = 65535
    PageSize = 0
    SmallChange = 16
    TabOrder = 1
    OnChange = sbMemoryChange
  end
  object pMemory: TPanel
    Left = 0
    Top = 266
    Width = 521
    Height = 28
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 2
    object Panel4: TLabel
      Left = 380
      Top = 5
      Width = 3
      Height = 13
      Caption = ' '
    end
    object Label1: TLabel
      Left = 248
      Top = 6
      Width = 41
      Height = 13
      Caption = 'Address:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object cboMemory: TComboBox
      Left = 8
      Top = 3
      Width = 193
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 0
      OnChange = cboMemoryChange
      Items.Strings = (
        'Z80 Memory'
        'TMS VDP VRAM Memory'
        'RAM Memory'
        'SGM RAM Memory'
        'EEPROM'
        'SRAM')
    end
    object udMemory: TUpDown
      Left = 208
      Top = 3
      Width = 32
      Height = 22
      Min = -32768
      Max = 32767
      Orientation = udHorizontal
      Position = 0
      TabOrder = 1
      TabStop = True
      Wrap = False
      OnChangingEx = udMemoryChangingEx
    end
    object eMemAdr: TEdit
      Left = 294
      Top = 3
      Width = 73
      Height = 21
      TabOrder = 2
      OnKeyPress = eMemAdrKeyPress
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 400
    Top = 184
    object RunStop2: TMenuItem
      Caption = 'Run/Stop'
    end
    object SetAddress1: TMenuItem
      Caption = 'Goto...'
      OnClick = SetAddress1Click
    end
    object Search1: TMenuItem
      Caption = 'Search...'
    end
    object DumpMemory1: TMenuItem
      Caption = 'Dump Memory...'
    end
  end
  object MainMenu1: TMainMenu
    Left = 384
    Top = 80
    object File1: TMenuItem
      Caption = 'File'
    end
    object Memory1: TMenuItem
      Caption = 'Memory'
      object Goto1: TMenuItem
        Caption = 'Goto ...'
        OnClick = SetAddress1Click
      end
      object Search2: TMenuItem
        Caption = 'Find ...'
        OnClick = Search1Click
      end
      object DumpMemory2: TMenuItem
        Caption = 'Dump Memory ...'
        OnClick = DumpMemory1Click
      end
    end
  end
end
