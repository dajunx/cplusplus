; 示例 #4: 这是个添加图标到其菜单项的可运行脚本.
Menu, FileMenu, Add, Script Icon, MenuHandler0
Menu, FileMenu, Add, Suspend Icon, MenuHandler1
Menu, FileMenu, Add, Pause Icon, MenuHandler2
Menu, FileMenu, Icon, Script Icon, %A_AhkPath%, 2 ;使用文件中的第二个图标组
Menu, FileMenu, Icon, Suspend Icon, %A_AhkPath%, -206 ;使用资源标识符 206 表示的图标
Menu, FileMenu, Icon, Pause Icon, %A_AhkPath%, -207 ;使用资源表示符 207 表示的图标
Menu, MyMenuBar, Add, &File, :FileMenu
Gui, Menu, MyMenuBar
Gui, Add, Button, gExit, Exit This Example
Gui, Add, Button, gExit, show something
Gui, Show
return

MenuHandler0:
MsgBox, "0"
Return

MenuHandler1:
MsgBox, "1"
Return

MenuHandler2:
MsgBox, "2"
Return

Exit:
ExitApp