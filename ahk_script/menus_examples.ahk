; 示例 #3: 这是个可运行脚本, 它演示了一些菜单命令.

#Persistent
#SingleInstance
menu, tray, add ; 分隔符
menu, tray, add, TestToggle&Check
menu, tray, add, TestToggleEnable
menu, tray, add, TestDefault
menu, tray, add, TestStandard
menu, tray, add, TestDelete
menu, tray, add, TestDeleteAll
menu, tray, add, TestRename
menu, tray, add, Test
return

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

TestToggle&Check:
menu, tray, ToggleCheck, TestToggle&Check
menu, tray, Enable, TestToggleEnable ; 同时启用了下一行的测试, 因为它不能撤销自己的禁用状态.
menu, tray, add, TestDelete ; 类似于上面.
return

TestToggleEnable:
menu, tray, ToggleEnable, TestToggleEnable
return

TestDefault:
if default = TestDefault
{
    menu, tray, NoDefault
    default =
}
else
{
    menu, tray, Default, TestDefault
    default = TestDefault
}
return

TestStandard:
if standard <> n
{
    menu, tray, NoStandard
    standard = n
}
else
{
    menu, tray, Standard
    standard = y
}
return

TestDelete:
menu, tray, delete, TestDelete
return

TestDeleteAll:
menu, tray, DeleteAll
return

TestRename:
if NewName <> renamed
{
    OldName = TestRename
    NewName = renamed
}
else
{
    OldName = renamed
    NewName = TestRename
}
menu, tray, rename, %OldName%, %NewName%
return

Test:
MsgBox, You selected "%A_ThisMenuItem%" in menu "%A_ThisMenu%".
return