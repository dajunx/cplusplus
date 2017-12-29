;在不同窗口输出内容
SetBatchLines, -1
i = 200 ; 输出
condition = 1 ;用作判断if/else 条件
Loop 2
{
    if (condition){
        WinActivate, ahk_pid 9960 ; 直接通过pid激活对话框
        MouseMove, 284, 451, 0
        Click
        Send %i%{enter}
        condition--
    } else {
        WinActivate, ahk_pid 14060
        MouseMove, 284, 451, 0
        Click
        Send %i%{enter}
        condition++
    }
    i++
    ;Sleep, 500
}