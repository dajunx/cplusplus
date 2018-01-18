; 循环 单击左键

SetBatchLines, -1 ;让脚本无休眠运行（即让脚本以全速运行）
~$MButton::
  While (true)
  {
    if (GetKeyState("RButton")) {
	ExitApp
    } else {
        Click 
		;Sleep, 65
        Sleep, 900
    }
  }
Return
Space:: Pause, Toggle