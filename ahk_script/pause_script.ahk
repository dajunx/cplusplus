;在不同窗口输出内容
SetBatchLines, -1
i = 1

Loop 1000
{
    WinActivate, ahk_pid 6792 ; 直接通过pid激活对话框
    ;WinActivate, ahk_pid 8716 ; 直接通过pid激活对话框
    MouseMove, 280, 520, 0
    Click
    Send %i%{enter}
    i++
    Sleep, 100
}

+p::Pause