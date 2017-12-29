;在不同窗口输出内容
i = 100 ; 输出
condition = 1 ;用作判断if/else 条件
Loop 4
{
    if (condition){
        IfWinExist, 888100 - test ; 窗口标题做判定条件
        {
            WinActivate
            MouseMove, 284, 451
            Click
            Send %i%{enter}
            ;Sleep, 1000
        }
        condition--
    } else {
        IfWinExist, 888104 - 22222222
        {
            WinActivate
            MouseMove, 284, 451
            Click
            Send %i%{enter}
            ;Sleep, 1000
        }
        condition++
    }
    i++
    Sleep, 500
}