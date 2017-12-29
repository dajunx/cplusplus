; 示例: 简单的图像查看器:

Gui, +Resize
Gui, Add, Button, default, &Load New Image
Gui, Add, Radio, ym+5 x+10 vRadio checked, Load &actual size
Gui, Add, Radio, ym+5 x+10, Load to &fit screen
Gui, Add, Pic, xm vPic
Gui, Show
return

ButtonLoadNewImage:
FileSelectFile, file,,, Select an image:, Images (*.gif; *.jpg; *.bmp; *.png; *.tif; *.ico; *.cur; *.ani; *.exe; *.dll)
if file =
    return
Gui, Submit, NoHide ; 保存单选按钮的值.
if Radio = 1  ; 按实际大小显示图像.
{
    Width = 0
    Height = 0
}
else ; 选择了第二个单选按钮: 按照屏幕的大小显示图像.
{
    Width := A_ScreenWidth - 28  ; 减去的 28 是用来显示边框和内边缘的空间.
    Height = -1  ; "保持高宽比" 应该是最好的.
}
GuiControl,, Pic, *w%width% *h%height% %file%  ; 载入图像.
Gui, Show, xCenter y0 AutoSize, %file%  ; 调整窗口以适合图片尺寸.
return

GuiClose:
ExitApp