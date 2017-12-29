; 示例: 要求输入姓名的简单输入框:

Gui, Add, Text,, First name:
Gui, Add, Text,, Last name:
Gui, Add, Edit, vFirstName ym  ; ym 选项开始一个新的控件列.
Gui, Add, Edit, vLastName
Gui, Add, Button, default, OK  ; ButtonOK(如果存在)会在此按钮被按下时运行.
Gui, Show,, Simple Input Example
return  ; 自动运行段结束. 在用户进行操作前脚本会一直保持空闲状态.

GuiClose:
ButtonOK:
Gui, Submit  ; 保存用户的输入到每个控件的关联变量中.
MsgBox You entered "%FirstName% %LastName%".
ExitApp