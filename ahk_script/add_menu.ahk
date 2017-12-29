; 示例 #1: 这是个可运行脚本, 它添加一个新菜单项到托盘图标菜单的底部.

#Persistent  ; 让脚本持续运行, 直到用户退出.
Menu, tray, add  ; 创建分隔线.
Menu, tray, add, Item1, MenuHandler  ; 创建新菜单项.
return

MenuHandler:
MsgBox You selected %A_ThisMenuItem% from menu %A_ThisMenu%.
return