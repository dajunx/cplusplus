~$MButton::
  While (true)
  {
    if (GetKeyState("RButton")) {
	Return
    } else {
        Click
		sleep, 500
    }
  }
Return
Space:: Pause, Toggle