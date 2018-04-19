#NoEnv

SetBatchLines -1
ListLines Off
Process, Priority, , High

#SingleInstance ignore
#MaxHotkeysPerInterval 1000
#Persistent
#InstallKeybdHook
#NoTrayIcon

v:="4.0.5"

EnvGet,AppDataLocal,LocalAppData

FileCreateDir,%AppDataLocal%\Power Keys
SetWorkingDir,%AppDataLocal%\Power Keys

if !A_IsAdmin
{
try
{
Run *RunAs "%A_ScriptFullPath%" /restart
}
ExitApp
}

ShellRun(prms*)
{
    shellWindows := ComObjCreate("{9BA05972-F6A8-11CF-A442-00A0C90A8F39}")
    
    desktop := shellWindows.Item(ComObj(19, 8)) ; VT_UI4, SCW_DESKTOP                
   
    ; Retrieve top-level browser object.
    if ptlb := ComObjQuery(desktop
        , "{4C96BE40-915C-11CF-99D3-00AA004AE837}"  ; SID_STopLevelBrowser
        , "{000214E2-0000-0000-C000-000000000046}") ; IID_IShellBrowser
    {
        ; IShellBrowser.QueryActiveShellView -> IShellView
        if DllCall(NumGet(NumGet(ptlb+0)+15*A_PtrSize), "ptr", ptlb, "ptr*", psv:=0) = 0
        {
            ; Define IID_IDispatch.
            VarSetCapacity(IID_IDispatch, 16)
            NumPut(0x46000000000000C0, NumPut(0x20400, IID_IDispatch, "int64"), "int64")
           
            ; IShellView.GetItemObject -> IDispatch (object which implements IShellFolderViewDual)
            DllCall(NumGet(NumGet(psv+0)+15*A_PtrSize), "ptr", psv
                , "uint", 0, "ptr", &IID_IDispatch, "ptr*", pdisp:=0)
           
            ; Get Shell object.
            shell := ComObj(9,pdisp,1).Application
           
            ; IShellDispatch2.ShellExecute
            test:=shell.ShellExecute(prms*)
           
            ObjRelease(psv)
        }
        ObjRelease(ptlb)
    }
}

isenabled=1
GameMode=0

if A_Args.Length()=2
{
toRun="%AppDataLocal%\Power Keys\F%1%\%2%"
ShellRun(toRun)
}

if A_Args.Length()=1
DllCall("PowrProf\SetSuspendState", "int", 1, "int", 0, "int", 1)

RegWrite, REG_SZ, HKLM\Software\Microsoft\Windows\CurrentVersion\Run, Power Keys, %A_ScriptFullPath%
RegWrite, REG_SZ, HKLM\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer\Run, Power Keys, %A_ScriptFullPath%
RegWrite, REG_SZ, HKCU\Software\Microsoft\Windows\CurrentVersion\Run, Power Keys, %A_ScriptFullPath%
RegWrite, REG_SZ, HKCU\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer\Run, Power Keys, %A_ScriptFullPath%

Return

;系统控制 诸如（音量/选歌曲/打开计算器等等） -- ljj 2018.04.19
Lwin & =::
Rwin & =::
Send {Volume_Up}
Return

Lwin & -::
Rwin & -::
Send {Volume_Down}
Return

Lwin & , Up::
Rwin & , Up::
Send {Media_Prev}
Return

Lwin & . Up::
Rwin & . Up::
Send {Media_Next}
Return

Lwin & ' Up::
Rwin & ' Up::
Send {Media_Play_Pause}
Return

Lwin & / Up::
Rwin & / Up::
if GameMode=1
MsgBox,0x40040,Power Keys,版本：%v%`n作者：知阳`n游戏模式：已开启
else
MsgBox,0x40040,Power Keys,版本：%v%`n作者：知阳`n游戏模式：未开启
Return

LWin & NumLock Up::
RWin & NumLock Up::
toRun="%A_WinDir%\system32\calc.exe"
ShellRun(toRun)
Return

LWin & 0 Up::
RWin & 0 Up::
toRun="%A_WinDir%\system32\calc.exe"
ShellRun(toRun)
Return

LWin & CapsLock::
RWin & CapsLock::
winset,AlwaysOnTop,, A
Return

;打开回收站
#Delete::
Run ::{645ff040-5081-101b-9f08-00aa002f954e},,UseErrorLevel
Return

#+Delete::
msgbox,0x40131,Power Keys,确定清空回收站？
ifmsgbox,ok
{
FileRecycleEmpty
}
Return

;win + ？ 快捷键
LWin & Enter Up::
RWin & Enter Up::
Send ^#d
Return

LWin & BackSpace Up::
RWin & BackSpace Up::
Send ^#{F4}
Return

LWin & [ Up::
RWin & [ Up::
Send ^#{Left}
Return

LWin & ] Up::
RWin & ] Up::
Send ^#{Right}
Return

LWin & F1::
RWin & F1::
toRun="https://github.com/szzhiyang/PerfectWindows/blob/master/Power-Keys/Power-Keys.md"
ShellRun(toRun)
Return

Lwin & F4::
Rwin & F4::
MsgBox,0x40131,Power Keys,确定退出 Power Keys？
ifMsgBox,ok
{
RegDelete, HKLM\Software\Microsoft\Windows\CurrentVersion\Run, Power Keys
RegDelete, HKLM\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer\Run, Power Keys
RegDelete, HKCU\Software\Microsoft\Windows\CurrentVersion\Run, Power Keys
RegDelete, HKCU\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer\Run, Power Keys
ExitApp
}
Return

LWin & F5::
RWin & F5::
Send {LWin Up}
Send {RWin Up}
Run,"%A_ScriptFullPath%" /restart
Return

LWin & End::
RWin & End::
Send {LWin Up}
Send {RWin Up}
Run,"%A_ScriptFullPath%" /restart /hibernate
Return

#if GameMode=1

LWin::Return
RWin::Return

LWin & F1::
RWin & F1::
Return

#if

#if isenabled

#+PrintScreen::
toRun="%A_WinDir%\system32\snippingtool.exe"
ShellRun(toRun)
Return

Lwin & PgUp::
Rwin & PgUp::
Send #=
Return

Lwin & PgDn::
Rwin & PgDn::
Send #-
Return

#G::
GameMode=1
isenabled=0
;MsgBox,0x40040,Power Keys,游戏模式已开启。
Return

SetStoreCapsLockMode, Off

CapsLock::
GetKeyState,caps,CapsLock,T
if caps=D
setcapslockstate,off
else
setcapslockstate,on
Return

CapsLock & Tab::AltTab
CapsLock & `::Send !{Esc}
CapsLock & Esc::Send !{F4}
CapsLock & a::Send ^+!{a}
CapsLock & b::Send ^+!{b}
CapsLock & c::Send ^+!{c}
CapsLock & d::Send ^+!{d}
CapsLock & e::Send ^+!{e}
CapsLock & f::Send ^+!{f}
CapsLock & g::Send ^+!{g}
CapsLock & h::Send ^+!{h}
CapsLock & i::Send ^+!{i}
CapsLock & j::Send ^+!{j}
CapsLock & k::Send ^+!{k}
CapsLock & l::Send ^+!{l}
CapsLock & m::Send ^+!{m}
CapsLock & n::Send ^+!{n}
CapsLock & o::Send ^+!{o}
CapsLock & p::Send ^+!{p}
CapsLock & q::Send ^+!{q}
CapsLock & r::Send ^+!{r}
CapsLock & s::Send ^+!{s}
CapsLock & t::Send ^+!{t}
CapsLock & u::Send ^+!{u}
CapsLock & v::Send ^+!{v}
CapsLock & w::Send ^+!{w}
CapsLock & x::Send ^+!{x}
CapsLock & y::Send ^+!{y}
CapsLock & z::Send ^+!{z}
CapsLock & 1::Send ^+!{1}
CapsLock & 2::Send ^+!{2}
CapsLock & 3::Send ^+!{3}
CapsLock & 4::Send ^+!{4}
CapsLock & 5::Send ^+!{5}
CapsLock & 6::Send ^+!{6}
CapsLock & 7::Send ^+!{7}
CapsLock & 8::Send ^+!{8}
CapsLock & 9::Send ^+!{9}
CapsLock & 0::Send ^+!{0}
CapsLock & F1::Send ^+!{F1}
CapsLock & F2::Send ^+!{F2}
CapsLock & F3::Send ^+!{F3}
CapsLock & F4::Send ^+!{F4}
CapsLock & F5::Send ^+!{F5}
CapsLock & F6::Send ^+!{F6}
CapsLock & F7::Send ^+!{F7}
CapsLock & F8::Send ^+!{F8}
CapsLock & F9::Send ^+!{F9}
CapsLock & F10::Send ^+!{F10}
CapsLock & F11::Send ^+!{F11}
CapsLock & F12::Send ^+!{F12}
CapsLock & ,::Send ^+!{,}
CapsLock & .::Send ^+!{.}
CapsLock & /::Send ^+!{/}
CapsLock & '::Send ^+!{'}
CapsLock & [::Send ^+!{[}
CapsLock & ]::Send ^+!{]}
CapsLock & \::Send ^+!{\}
CapsLock & -::Send ^+!{-}
CapsLock & =::Send ^+!{=}
CapsLock & Space::Send ^+!{Space}
CapsLock & Enter::Send ^+!{Enter}
CapsLock & Backspace::Send ^+!{Backspace}
CapsLock & Delete::Send ^+!{Delete}
CapsLock & Insert::Send ^+!{Insert}
CapsLock & Home::Send ^+!{Home}
CapsLock & End::Send ^+!{End}
CapsLock & PgUp::Send ^+!{PgUp}
CapsLock & PgDn::Send ^+!{PgDn}
CapsLock & Up::Send ^+!{Up}
CapsLock & Down::Send ^+!{Down}
CapsLock & Left::Send ^+!{Left}
CapsLock & Right::Send ^+!{Right}
CapsLock & PrintScreen::Send ^+!{PrintScreen}

;模拟系统 hotkey
`::`
` & Shift::`
` & a::Send +!{a}
` & b::Send +!{b}
` & c::Send +!{c}
` & d::Send +!{d}
` & e::Send +!{e}
` & f::Send +!{f}
` & g::Send +!{g}
` & h::Send +!{h}
` & i::Send +!{i}
` & j::Send +!{j}
` & k::Send +!{k}
` & l::Send +!{l}
` & m::Send +!{m}
` & n::Send +!{n}
` & o::Send +!{o}
` & p::Send +!{p}
` & q::Send +!{q}
` & r::Send +!{r}
` & s::Send +!{s}
` & t::Send +!{t}
` & u::Send +!{u}
` & v::Send +!{v}
` & w::Send +!{w}
` & x::Send +!{x}
` & y::Send +!{y}
` & z::Send +!{z}
` & 1::Send +!{1}
` & 2::Send +!{2}
` & 3::Send +!{3}
` & 4::Send +!{4}
` & 5::Send +!{5}
` & 6::Send +!{6}
` & 7::Send +!{7}
` & 8::Send +!{8}
` & 9::Send +!{9}
` & 0::Send +!{0}
` & F1::Send +!{F1}
` & F2::Send +!{F2}
` & F3::Send +!{F3}
` & F4::Send +!{F4}
` & F5::Send +!{F5}
` & F6::Send +!{F6}
` & F7::Send +!{F7}
` & F8::Send +!{F8}
` & F9::Send +!{F9}
` & F10::Send +!{F10}
` & F11::Send +!{F11}
` & F12::Send +!{F12}
` & ,::Send +!{,}
` & .::Send +!{.}
` & /::Send +!{/}
` & '::Send +!{'}
` & [::Send +!{[}
` & ]::Send +!{]}
` & \::Send +!{\}
` & -::Send +!{-}
` & =::Send +!{=}
` & Space::Send +!{Space}
` & Enter::Send +!{Enter}
` & Backspace::Send +!{Backspace}
` & Delete::Send +!{Delete}
` & Insert::Send +!{Insert}
` & Home::Send +!{Home}
` & End::Send +!{End}
` & PgUp::Send +!{PgUp}
` & PgDn::Send +!{PgDn}
` & Up::Send +!{Up}
` & Down::Send +!{Down}
` & Left::Send +!{Left}
` & Right::Send +!{Right}
` & PrintScreen::Send +!{PrintScreen}
` & Tab::ShiftAltTab

Tab::Tab
Tab & Shift::Tab
Tab & a::Send ^!{a}
Tab & b::Send ^!{b}
Tab & c::Send ^!{c}
Tab & d::Send ^!{d}
Tab & e::Send ^!{e}
Tab & f::Send ^!{f}
Tab & g::Send ^!{g}
Tab & h::Send ^!{h}
Tab & i::Send ^!{i}
Tab & j::Send ^!{j}
Tab & k::Send ^!{k}
Tab & l::Send ^!{l}
Tab & m::Send ^!{m}
Tab & n::Send ^!{n}
Tab & o::Send ^!{o}
Tab & p::Send ^!{p}
Tab & q::Send ^!{q}
Tab & r::Send ^!{r}
Tab & s::Send ^!{s}
Tab & t::Send ^!{t}
Tab & u::Send ^!{u}
Tab & v::Send ^!{v}
Tab & w::Send ^!{w}
Tab & x::Send ^!{x}
Tab & y::Send ^!{y}
Tab & z::Send ^!{z}
Tab & 1::Send ^!{1}
Tab & 2::Send ^!{2}
Tab & 3::Send ^!{3}
Tab & 4::Send ^!{4}
Tab & 5::Send ^!{5}
Tab & 6::Send ^!{6}
Tab & 7::Send ^!{7}
Tab & 8::Send ^!{8}
Tab & 9::Send ^!{9}
Tab & 0::Send ^!{0}
Tab & F1::Send ^!{F1}
Tab & F2::Send ^!{F2}
Tab & F3::Send ^!{F3}
Tab & F4::Send ^!{F4}
Tab & F5::Send ^!{F5}
Tab & F6::Send ^!{F6}
Tab & F7::Send ^!{F7}
Tab & F8::Send ^!{F8}
Tab & F9::Send ^!{F9}
Tab & F10::Send ^!{F10}
Tab & F11::Send ^!{F11}
Tab & F12::Send ^!{F12}
Tab & ,::Send ^!{,}
Tab & .::Send ^!{.}
Tab & /::Send ^!{/}
Tab & '::Send ^!{'}
Tab & [::Send ^!{[}
Tab & ]::Send ^!{]}
Tab & \::Send ^!{\}
Tab & -::Send ^!{-}
Tab & =::Send ^!{=}
Tab & Space::Send ^!{Space}
Tab & Enter::Send ^!{Enter}
Tab & Backspace::Send ^!{Backspace}
Tab & Delete::Run,"%A_WinDir%\system32\taskmgr.exe",,UseErrorLevel
Tab & Insert::Send ^!{Insert}
Tab & Home::Send ^!{Home}
Tab & End::Send ^!{End}
Tab & PgUp::Send ^!{PgUp}
Tab & PgDn::Send ^!{PgDn}
Tab & Up::Send ^!{Up}
Tab & Down::Send ^!{Down}
Tab & Left::Send ^!{Left}
Tab & Right::Send ^!{Right}
Tab & PrintScreen::Send ^!{PrintScreen}

Esc::Esc
Esc & Shift::Esc
Esc & a::Send ^+{a}
Esc & b::Send ^+{b}
Esc & c::Send ^+{c}
Esc & d::Send ^+{d}
Esc & e::Send ^+{e}
Esc & f::Send ^+{f}
Esc & g::Send ^+{g}
Esc & h::Send ^+{h}
Esc & i::Send ^+{i}
Esc & j::Send ^+{j}
Esc & k::Send ^+{k}
Esc & l::Send ^+{l}
Esc & m::Send ^+{m}
Esc & n::Send ^+{n}
Esc & o::Send ^+{o}
Esc & p::Send ^+{p}
Esc & q::Send ^+{q}
Esc & r::Send ^+{r}
Esc & s::Send ^+{s}
Esc & t::Send ^+{t}
Esc & u::Send ^+{u}
Esc & v::Send ^+{v}
Esc & w::Send ^+{w}
Esc & x::Send ^+{x}
Esc & y::Send ^+{y}
Esc & z::Send ^+{z}
Esc & 1::Send ^+{1}
Esc & 2::Send ^+{2}
Esc & 3::Send ^+{3}
Esc & 4::Send ^+{4}
Esc & 5::Send ^+{5}
Esc & 6::Send ^+{6}
Esc & 7::Send ^+{7}
Esc & 8::Send ^+{8}
Esc & 9::Send ^+{9}
Esc & 0::Send ^+{0}
Esc & F1::Send ^+{F1}
Esc & F2::Send ^+{F2}
Esc & F3::Send ^+{F3}
Esc & F4::Send ^+{F4}
Esc & F5::Send ^+{F5}
Esc & F6::Send ^+{F6}
Esc & F7::Send ^+{F7}
Esc & F8::Send ^+{F8}
Esc & F9::Send ^+{F9}
Esc & F10::Send ^+{F10}
Esc & F11::Send ^+{F11}
Esc & F12::Send ^+{F12}
Esc & ,::Send ^+{,}
Esc & .::Send ^+{.}
Esc & /::Send ^+{/}
Esc & '::Send ^+{'}
Esc & [::Send ^+{[}
Esc & ]::Send ^+{]}
Esc & \::Send ^+{\}
Esc & -::Send ^+{-}
Esc & =::Send ^+{=}
Esc & Space::Send ^+{Space}
Esc & Enter::Send ^+{Enter}
Esc & Backspace::Send ^+{Backspace}
Esc & Delete::Send ^+{Delete}
Esc & Insert::Send ^+{Insert}
Esc & Home::Send ^+{Home}
Esc & End::Send ^+{End}
Esc & PgUp::Send ^+{PgUp}
Esc & PgDn::Send ^+{PgDn}
Esc & Up::Send ^+{Up}
Esc & Down::Send ^+{Down}
Esc & Left::Send ^+{Left}
Esc & Right::Send ^+{Right}
Esc & PrintScreen::Send ^+{PrintScreen}
Esc & Tab::Send ^+{Tab}

F1::F1
F2::F2
F3::F3
F4::F4
F5::F5
F6::F6
F7::F7
F8::F8
F9::F9
F10::F10
F11::F11
F12::F12

F1 & Shift::F1
F2 & Shift::F2
F3 & Shift::F3
F4 & Shift::F4
F5 & Shift::F5
F6 & Shift::F6
F7 & Shift::F7
F8 & Shift::F8
F9 & Shift::F9
F10 & Shift::F10
F11 & Shift::F11
F12 & Shift::F12

F1 & Space::Send !{F4}
F2 & Space::Send !{F4}
F3 & Space::Send !{F4}
F4 & Space::Send !{F4}
F5 & Space::Send !{F4}
F6 & Space::Send !{F4}
F7 & Space::Send !{F4}
F8 & Space::Send !{F4}
F9 & Space::Send !{F4}
F10 & Space::Send !{F4}
F11 & Space::Send !{F4}
F12 & Space::Send !{F4}

F1 & PrintScreen::^+!F1
F2 & PrintScreen::^+!F2
F3 & PrintScreen::^+!F3
F4 & PrintScreen::^+!F4
F5 & PrintScreen::^+!F5
F6 & PrintScreen::^+!F6
F7 & PrintScreen::^+!F7
F8 & PrintScreen::^+!F8
F9 & PrintScreen::^+!F9
F10 & PrintScreen::^+!F10
F11 & PrintScreen::^+!F11
F12 & PrintScreen::^+!F12

F1 & Enter Up::
FileCreateDir,F1
Run,F1
Return

F1 & a Up::
if FileExist("F1\a.lnk")
Run,"%A_ScriptFullPath%" /restart 1 a.lnk
else if FileExist("F1\a.url")
Run,"%A_ScriptFullPath%" /restart 1 a.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & b Up::
if FileExist("F1\b.lnk")
Run,"%A_ScriptFullPath%" /restart 1 b.lnk
else if FileExist("F1\b.url")
Run,"%A_ScriptFullPath%" /restart 1 b.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & c Up::
if FileExist("F1\c.lnk")
Run,"%A_ScriptFullPath%" /restart 1 c.lnk
else if FileExist("F1\c.url")
Run,"%A_ScriptFullPath%" /restart 1 c.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & d Up::
if FileExist("F1\d.lnk")
Run,"%A_ScriptFullPath%" /restart 1 d.lnk
else if FileExist("F1\d.url")
Run,"%A_ScriptFullPath%" /restart 1 d.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & e Up::
if FileExist("F1\e.lnk")
Run,"%A_ScriptFullPath%" /restart 1 e.lnk
else if FileExist("F1\e.url")
Run,"%A_ScriptFullPath%" /restart 1 e.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & f Up::
if FileExist("F1\f.lnk")
Run,"%A_ScriptFullPath%" /restart 1 f.lnk
else if FileExist("F1\f.url")
Run,"%A_ScriptFullPath%" /restart 1 f.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & g Up::
if FileExist("F1\g.lnk")
Run,"%A_ScriptFullPath%" /restart 1 g.lnk
else if FileExist("F1\g.url")
Run,"%A_ScriptFullPath%" /restart 1 g.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & h Up::
if FileExist("F1\h.lnk")
Run,"%A_ScriptFullPath%" /restart 1 h.lnk
else if FileExist("F1\h.url")
Run,"%A_ScriptFullPath%" /restart 1 h.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & i Up::
if FileExist("F1\i.lnk")
Run,"%A_ScriptFullPath%" /restart 1 i.lnk
else if FileExist("F1\i.url")
Run,"%A_ScriptFullPath%" /restart 1 i.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & j Up::
if FileExist("F1\j.lnk")
Run,"%A_ScriptFullPath%" /restart 1 j.lnk
else if FileExist("F1\j.url")
Run,"%A_ScriptFullPath%" /restart 1 j.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & k Up::
if FileExist("F1\k.lnk")
Run,"%A_ScriptFullPath%" /restart 1 k.lnk
else if FileExist("F1\k.url")
Run,"%A_ScriptFullPath%" /restart 1 k.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & l Up::
if FileExist("F1\l.lnk")
Run,"%A_ScriptFullPath%" /restart 1 l.lnk
else if FileExist("F1\l.url")
Run,"%A_ScriptFullPath%" /restart 1 l.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & m Up::
if FileExist("F1\m.lnk")
Run,"%A_ScriptFullPath%" /restart 1 m.lnk
else if FileExist("F1\m.url")
Run,"%A_ScriptFullPath%" /restart 1 m.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & n Up::
if FileExist("F1\n.lnk")
Run,"%A_ScriptFullPath%" /restart 1 n.lnk
else if FileExist("F1\n.url")
Run,"%A_ScriptFullPath%" /restart 1 n.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & o Up::
if FileExist("F1\o.lnk")
Run,"%A_ScriptFullPath%" /restart 1 o.lnk
else if FileExist("F1\o.url")
Run,"%A_ScriptFullPath%" /restart 1 o.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & p Up::
if FileExist("F1\p.lnk")
Run,"%A_ScriptFullPath%" /restart 1 p.lnk
else if FileExist("F1\p.url")
Run,"%A_ScriptFullPath%" /restart 1 p.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & q Up::
if FileExist("F1\q.lnk")
Run,"%A_ScriptFullPath%" /restart 1 q.lnk
else if FileExist("F1\q.url")
Run,"%A_ScriptFullPath%" /restart 1 q.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & r Up::
if FileExist("F1\r.lnk")
Run,"%A_ScriptFullPath%" /restart 1 r.lnk
else if FileExist("F1\r.url")
Run,"%A_ScriptFullPath%" /restart 1 r.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & s Up::
if FileExist("F1\s.lnk")
Run,"%A_ScriptFullPath%" /restart 1 s.lnk
else if FileExist("F1\s.url")
Run,"%A_ScriptFullPath%" /restart 1 s.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & t Up::
if FileExist("F1\t.lnk")
Run,"%A_ScriptFullPath%" /restart 1 t.lnk
else if FileExist("F1\t.url")
Run,"%A_ScriptFullPath%" /restart 1 t.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & u Up::
if FileExist("F1\u.lnk")
Run,"%A_ScriptFullPath%" /restart 1 u.lnk
else if FileExist("F1\u.url")
Run,"%A_ScriptFullPath%" /restart 1 u.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & v Up::
if FileExist("F1\v.lnk")
Run,"%A_ScriptFullPath%" /restart 1 v.lnk
else if FileExist("F1\v.url")
Run,"%A_ScriptFullPath%" /restart 1 v.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & w Up::
if FileExist("F1\w.lnk")
Run,"%A_ScriptFullPath%" /restart 1 w.lnk
else if FileExist("F1\w.url")
Run,"%A_ScriptFullPath%" /restart 1 w.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & x Up::
if FileExist("F1\x.lnk")
Run,"%A_ScriptFullPath%" /restart 1 x.lnk
else if FileExist("F1\x.url")
Run,"%A_ScriptFullPath%" /restart 1 x.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & y Up::
if FileExist("F1\y.lnk")
Run,"%A_ScriptFullPath%" /restart 1 y.lnk
else if FileExist("F1\y.url")
Run,"%A_ScriptFullPath%" /restart 1 y.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & z Up::
if FileExist("F1\z.lnk")
Run,"%A_ScriptFullPath%" /restart 1 z.lnk
else if FileExist("F1\z.url")
Run,"%A_ScriptFullPath%" /restart 1 z.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & 1 Up::
if FileExist("F1\1.lnk")
Run,"%A_ScriptFullPath%" /restart 1 1.lnk
else if FileExist("F1\1.url")
Run,"%A_ScriptFullPath%" /restart 1 1.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & 2 Up::
if FileExist("F1\2.lnk")
Run,"%A_ScriptFullPath%" /restart 1 2.lnk
else if FileExist("F1\2.url")
Run,"%A_ScriptFullPath%" /restart 1 2.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & 3 Up::
if FileExist("F1\3.lnk")
Run,"%A_ScriptFullPath%" /restart 1 3.lnk
else if FileExist("F1\3.url")
Run,"%A_ScriptFullPath%" /restart 1 3.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & 4 Up::
if FileExist("F1\4.lnk")
Run,"%A_ScriptFullPath%" /restart 1 4.lnk
else if FileExist("F1\4.url")
Run,"%A_ScriptFullPath%" /restart 1 4.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & 5 Up::
if FileExist("F1\5.lnk")
Run,"%A_ScriptFullPath%" /restart 1 5.lnk
else if FileExist("F1\5.url")
Run,"%A_ScriptFullPath%" /restart 1 5.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & 6 Up::
if FileExist("F1\6.lnk")
Run,"%A_ScriptFullPath%" /restart 1 6.lnk
else if FileExist("F1\6.url")
Run,"%A_ScriptFullPath%" /restart 1 6.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & 7 Up::
if FileExist("F1\7.lnk")
Run,"%A_ScriptFullPath%" /restart 1 7.lnk
else if FileExist("F1\7.url")
Run,"%A_ScriptFullPath%" /restart 1 7.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & 8 Up::
if FileExist("F1\8.lnk")
Run,"%A_ScriptFullPath%" /restart 1 8.lnk
else if FileExist("F1\8.url")
Run,"%A_ScriptFullPath%" /restart 1 8.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & 9 Up::
if FileExist("F1\9.lnk")
Run,"%A_ScriptFullPath%" /restart 1 9.lnk
else if FileExist("F1\9.url")
Run,"%A_ScriptFullPath%" /restart 1 9.url
else
{
FileCreateDir,F1
Run,F1
}
Return

F1 & 0 Up::
if FileExist("F1\0.lnk")
Run,"%A_ScriptFullPath%" /restart 1 0.lnk
else if FileExist("F1\0.url")
Run,"%A_ScriptFullPath%" /restart 1 0.url
else
{
FileCreateDir,F1
Run,F1
}
Return


F2 & Enter Up::
FileCreateDir,F2
Run,F2
Return

F2 & a Up::
if FileExist("F2\a.lnk")
Run,"%A_ScriptFullPath%" /restart 2 a.lnk
else if FileExist("F2\a.url")
Run,"%A_ScriptFullPath%" /restart 2 a.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & b Up::
if FileExist("F2\b.lnk")
Run,"%A_ScriptFullPath%" /restart 2 b.lnk
else if FileExist("F2\b.url")
Run,"%A_ScriptFullPath%" /restart 2 b.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & c Up::
if FileExist("F2\c.lnk")
Run,"%A_ScriptFullPath%" /restart 2 c.lnk
else if FileExist("F2\c.url")
Run,"%A_ScriptFullPath%" /restart 2 c.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & d Up::
if FileExist("F2\d.lnk")
Run,"%A_ScriptFullPath%" /restart 2 d.lnk
else if FileExist("F2\d.url")
Run,"%A_ScriptFullPath%" /restart 2 d.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & e Up::
if FileExist("F2\e.lnk")
Run,"%A_ScriptFullPath%" /restart 2 e.lnk
else if FileExist("F2\e.url")
Run,"%A_ScriptFullPath%" /restart 2 e.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & f Up::
if FileExist("F2\f.lnk")
Run,"%A_ScriptFullPath%" /restart 2 f.lnk
else if FileExist("F2\f.url")
Run,"%A_ScriptFullPath%" /restart 2 f.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & g Up::
if FileExist("F2\g.lnk")
Run,"%A_ScriptFullPath%" /restart 2 g.lnk
else if FileExist("F2\g.url")
Run,"%A_ScriptFullPath%" /restart 2 g.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & h Up::
if FileExist("F2\h.lnk")
Run,"%A_ScriptFullPath%" /restart 2 h.lnk
else if FileExist("F2\h.url")
Run,"%A_ScriptFullPath%" /restart 2 h.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & i Up::
if FileExist("F2\i.lnk")
Run,"%A_ScriptFullPath%" /restart 2 i.lnk
else if FileExist("F2\i.url")
Run,"%A_ScriptFullPath%" /restart 2 i.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & j Up::
if FileExist("F2\j.lnk")
Run,"%A_ScriptFullPath%" /restart 2 j.lnk
else if FileExist("F2\j.url")
Run,"%A_ScriptFullPath%" /restart 2 j.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & k Up::
if FileExist("F2\k.lnk")
Run,"%A_ScriptFullPath%" /restart 2 k.lnk
else if FileExist("F2\k.url")
Run,"%A_ScriptFullPath%" /restart 2 k.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & l Up::
if FileExist("F2\l.lnk")
Run,"%A_ScriptFullPath%" /restart 2 l.lnk
else if FileExist("F2\l.url")
Run,"%A_ScriptFullPath%" /restart 2 l.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & m Up::
if FileExist("F2\m.lnk")
Run,"%A_ScriptFullPath%" /restart 2 m.lnk
else if FileExist("F2\m.url")
Run,"%A_ScriptFullPath%" /restart 2 m.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & n Up::
if FileExist("F2\n.lnk")
Run,"%A_ScriptFullPath%" /restart 2 n.lnk
else if FileExist("F2\n.url")
Run,"%A_ScriptFullPath%" /restart 2 n.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & o Up::
if FileExist("F2\o.lnk")
Run,"%A_ScriptFullPath%" /restart 2 o.lnk
else if FileExist("F2\o.url")
Run,"%A_ScriptFullPath%" /restart 2 o.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & p Up::
if FileExist("F2\p.lnk")
Run,"%A_ScriptFullPath%" /restart 2 p.lnk
else if FileExist("F2\p.url")
Run,"%A_ScriptFullPath%" /restart 2 p.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & q Up::
if FileExist("F2\q.lnk")
Run,"%A_ScriptFullPath%" /restart 2 q.lnk
else if FileExist("F2\q.url")
Run,"%A_ScriptFullPath%" /restart 2 q.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & r Up::
if FileExist("F2\r.lnk")
Run,"%A_ScriptFullPath%" /restart 2 r.lnk
else if FileExist("F2\r.url")
Run,"%A_ScriptFullPath%" /restart 2 r.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & s Up::
if FileExist("F2\s.lnk")
Run,"%A_ScriptFullPath%" /restart 2 s.lnk
else if FileExist("F2\s.url")
Run,"%A_ScriptFullPath%" /restart 2 s.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & t Up::
if FileExist("F2\t.lnk")
Run,"%A_ScriptFullPath%" /restart 2 t.lnk
else if FileExist("F2\t.url")
Run,"%A_ScriptFullPath%" /restart 2 t.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & u Up::
if FileExist("F2\u.lnk")
Run,"%A_ScriptFullPath%" /restart 2 u.lnk
else if FileExist("F2\u.url")
Run,"%A_ScriptFullPath%" /restart 2 u.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & v Up::
if FileExist("F2\v.lnk")
Run,"%A_ScriptFullPath%" /restart 2 v.lnk
else if FileExist("F2\v.url")
Run,"%A_ScriptFullPath%" /restart 2 v.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & w Up::
if FileExist("F2\w.lnk")
Run,"%A_ScriptFullPath%" /restart 2 w.lnk
else if FileExist("F2\w.url")
Run,"%A_ScriptFullPath%" /restart 2 w.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & x Up::
if FileExist("F2\x.lnk")
Run,"%A_ScriptFullPath%" /restart 2 x.lnk
else if FileExist("F2\x.url")
Run,"%A_ScriptFullPath%" /restart 2 x.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & y Up::
if FileExist("F2\y.lnk")
Run,"%A_ScriptFullPath%" /restart 2 y.lnk
else if FileExist("F2\y.url")
Run,"%A_ScriptFullPath%" /restart 2 y.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & z Up::
if FileExist("F2\z.lnk")
Run,"%A_ScriptFullPath%" /restart 2 z.lnk
else if FileExist("F2\z.url")
Run,"%A_ScriptFullPath%" /restart 2 z.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & 1 Up::
if FileExist("F2\1.lnk")
Run,"%A_ScriptFullPath%" /restart 2 1.lnk
else if FileExist("F2\1.url")
Run,"%A_ScriptFullPath%" /restart 2 1.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & 2 Up::
if FileExist("F2\2.lnk")
Run,"%A_ScriptFullPath%" /restart 2 2.lnk
else if FileExist("F2\2.url")
Run,"%A_ScriptFullPath%" /restart 2 2.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & 3 Up::
if FileExist("F2\3.lnk")
Run,"%A_ScriptFullPath%" /restart 2 3.lnk
else if FileExist("F2\3.url")
Run,"%A_ScriptFullPath%" /restart 2 3.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & 4 Up::
if FileExist("F2\4.lnk")
Run,"%A_ScriptFullPath%" /restart 2 4.lnk
else if FileExist("F2\4.url")
Run,"%A_ScriptFullPath%" /restart 2 4.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & 5 Up::
if FileExist("F2\5.lnk")
Run,"%A_ScriptFullPath%" /restart 2 5.lnk
else if FileExist("F2\5.url")
Run,"%A_ScriptFullPath%" /restart 2 5.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & 6 Up::
if FileExist("F2\6.lnk")
Run,"%A_ScriptFullPath%" /restart 2 6.lnk
else if FileExist("F2\6.url")
Run,"%A_ScriptFullPath%" /restart 2 6.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & 7 Up::
if FileExist("F2\7.lnk")
Run,"%A_ScriptFullPath%" /restart 2 7.lnk
else if FileExist("F2\7.url")
Run,"%A_ScriptFullPath%" /restart 2 7.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & 8 Up::
if FileExist("F2\8.lnk")
Run,"%A_ScriptFullPath%" /restart 2 8.lnk
else if FileExist("F2\8.url")
Run,"%A_ScriptFullPath%" /restart 2 8.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & 9 Up::
if FileExist("F2\9.lnk")
Run,"%A_ScriptFullPath%" /restart 2 9.lnk
else if FileExist("F2\9.url")
Run,"%A_ScriptFullPath%" /restart 2 9.url
else
{
FileCreateDir,F2
Run,F2
}
Return

F2 & 0 Up::
if FileExist("F2\0.lnk")
Run,"%A_ScriptFullPath%" /restart 2 0.lnk
else if FileExist("F2\0.url")
Run,"%A_ScriptFullPath%" /restart 2 0.url
else
{
FileCreateDir,F2
Run,F2
}
Return


F3 & Enter Up::
FileCreateDir,F3
Run,F3
Return

F3 & a Up::
if FileExist("F3\a.lnk")
Run,"%A_ScriptFullPath%" /restart 3 a.lnk
else if FileExist("F3\a.url")
Run,"%A_ScriptFullPath%" /restart 3 a.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & b Up::
if FileExist("F3\b.lnk")
Run,"%A_ScriptFullPath%" /restart 3 b.lnk
else if FileExist("F3\b.url")
Run,"%A_ScriptFullPath%" /restart 3 b.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & c Up::
if FileExist("F3\c.lnk")
Run,"%A_ScriptFullPath%" /restart 3 c.lnk
else if FileExist("F3\c.url")
Run,"%A_ScriptFullPath%" /restart 3 c.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & d Up::
if FileExist("F3\d.lnk")
Run,"%A_ScriptFullPath%" /restart 3 d.lnk
else if FileExist("F3\d.url")
Run,"%A_ScriptFullPath%" /restart 3 d.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & e Up::
if FileExist("F3\e.lnk")
Run,"%A_ScriptFullPath%" /restart 3 e.lnk
else if FileExist("F3\e.url")
Run,"%A_ScriptFullPath%" /restart 3 e.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & f Up::
if FileExist("F3\f.lnk")
Run,"%A_ScriptFullPath%" /restart 3 f.lnk
else if FileExist("F3\f.url")
Run,"%A_ScriptFullPath%" /restart 3 f.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & g Up::
if FileExist("F3\g.lnk")
Run,"%A_ScriptFullPath%" /restart 3 g.lnk
else if FileExist("F3\g.url")
Run,"%A_ScriptFullPath%" /restart 3 g.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & h Up::
if FileExist("F3\h.lnk")
Run,"%A_ScriptFullPath%" /restart 3 h.lnk
else if FileExist("F3\h.url")
Run,"%A_ScriptFullPath%" /restart 3 h.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & i Up::
if FileExist("F3\i.lnk")
Run,"%A_ScriptFullPath%" /restart 3 i.lnk
else if FileExist("F3\i.url")
Run,"%A_ScriptFullPath%" /restart 3 i.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & j Up::
if FileExist("F3\j.lnk")
Run,"%A_ScriptFullPath%" /restart 3 j.lnk
else if FileExist("F3\j.url")
Run,"%A_ScriptFullPath%" /restart 3 j.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & k Up::
if FileExist("F3\k.lnk")
Run,"%A_ScriptFullPath%" /restart 3 k.lnk
else if FileExist("F3\k.url")
Run,"%A_ScriptFullPath%" /restart 3 k.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & l Up::
if FileExist("F3\l.lnk")
Run,"%A_ScriptFullPath%" /restart 3 l.lnk
else if FileExist("F3\l.url")
Run,"%A_ScriptFullPath%" /restart 3 l.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & m Up::
if FileExist("F3\m.lnk")
Run,"%A_ScriptFullPath%" /restart 3 m.lnk
else if FileExist("F3\m.url")
Run,"%A_ScriptFullPath%" /restart 3 m.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & n Up::
if FileExist("F3\n.lnk")
Run,"%A_ScriptFullPath%" /restart 3 n.lnk
else if FileExist("F3\n.url")
Run,"%A_ScriptFullPath%" /restart 3 n.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & o Up::
if FileExist("F3\o.lnk")
Run,"%A_ScriptFullPath%" /restart 3 o.lnk
else if FileExist("F3\o.url")
Run,"%A_ScriptFullPath%" /restart 3 o.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & p Up::
if FileExist("F3\p.lnk")
Run,"%A_ScriptFullPath%" /restart 3 p.lnk
else if FileExist("F3\p.url")
Run,"%A_ScriptFullPath%" /restart 3 p.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & q Up::
if FileExist("F3\q.lnk")
Run,"%A_ScriptFullPath%" /restart 3 q.lnk
else if FileExist("F3\q.url")
Run,"%A_ScriptFullPath%" /restart 3 q.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & r Up::
if FileExist("F3\r.lnk")
Run,"%A_ScriptFullPath%" /restart 3 r.lnk
else if FileExist("F3\r.url")
Run,"%A_ScriptFullPath%" /restart 3 r.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & s Up::
if FileExist("F3\s.lnk")
Run,"%A_ScriptFullPath%" /restart 3 s.lnk
else if FileExist("F3\s.url")
Run,"%A_ScriptFullPath%" /restart 3 s.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & t Up::
if FileExist("F3\t.lnk")
Run,"%A_ScriptFullPath%" /restart 3 t.lnk
else if FileExist("F3\t.url")
Run,"%A_ScriptFullPath%" /restart 3 t.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & u Up::
if FileExist("F3\u.lnk")
Run,"%A_ScriptFullPath%" /restart 3 u.lnk
else if FileExist("F3\u.url")
Run,"%A_ScriptFullPath%" /restart 3 u.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & v Up::
if FileExist("F3\v.lnk")
Run,"%A_ScriptFullPath%" /restart 3 v.lnk
else if FileExist("F3\v.url")
Run,"%A_ScriptFullPath%" /restart 3 v.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & w Up::
if FileExist("F3\w.lnk")
Run,"%A_ScriptFullPath%" /restart 3 w.lnk
else if FileExist("F3\w.url")
Run,"%A_ScriptFullPath%" /restart 3 w.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & x Up::
if FileExist("F3\x.lnk")
Run,"%A_ScriptFullPath%" /restart 3 x.lnk
else if FileExist("F3\x.url")
Run,"%A_ScriptFullPath%" /restart 3 x.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & y Up::
if FileExist("F3\y.lnk")
Run,"%A_ScriptFullPath%" /restart 3 y.lnk
else if FileExist("F3\y.url")
Run,"%A_ScriptFullPath%" /restart 3 y.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & z Up::
if FileExist("F3\z.lnk")
Run,"%A_ScriptFullPath%" /restart 3 z.lnk
else if FileExist("F3\z.url")
Run,"%A_ScriptFullPath%" /restart 3 z.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & 1 Up::
if FileExist("F3\1.lnk")
Run,"%A_ScriptFullPath%" /restart 3 1.lnk
else if FileExist("F3\1.url")
Run,"%A_ScriptFullPath%" /restart 3 1.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & 2 Up::
if FileExist("F3\2.lnk")
Run,"%A_ScriptFullPath%" /restart 3 2.lnk
else if FileExist("F3\2.url")
Run,"%A_ScriptFullPath%" /restart 3 2.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & 3 Up::
if FileExist("F3\3.lnk")
Run,"%A_ScriptFullPath%" /restart 3 3.lnk
else if FileExist("F3\3.url")
Run,"%A_ScriptFullPath%" /restart 3 3.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & 4 Up::
if FileExist("F3\4.lnk")
Run,"%A_ScriptFullPath%" /restart 3 4.lnk
else if FileExist("F3\4.url")
Run,"%A_ScriptFullPath%" /restart 3 4.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & 5 Up::
if FileExist("F3\5.lnk")
Run,"%A_ScriptFullPath%" /restart 3 5.lnk
else if FileExist("F3\5.url")
Run,"%A_ScriptFullPath%" /restart 3 5.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & 6 Up::
if FileExist("F3\6.lnk")
Run,"%A_ScriptFullPath%" /restart 3 6.lnk
else if FileExist("F3\6.url")
Run,"%A_ScriptFullPath%" /restart 3 6.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & 7 Up::
if FileExist("F3\7.lnk")
Run,"%A_ScriptFullPath%" /restart 3 7.lnk
else if FileExist("F3\7.url")
Run,"%A_ScriptFullPath%" /restart 3 7.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & 8 Up::
if FileExist("F3\8.lnk")
Run,"%A_ScriptFullPath%" /restart 3 8.lnk
else if FileExist("F3\8.url")
Run,"%A_ScriptFullPath%" /restart 3 8.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & 9 Up::
if FileExist("F3\9.lnk")
Run,"%A_ScriptFullPath%" /restart 3 9.lnk
else if FileExist("F3\9.url")
Run,"%A_ScriptFullPath%" /restart 3 9.url
else
{
FileCreateDir,F3
Run,F3
}
Return

F3 & 0 Up::
if FileExist("F3\0.lnk")
Run,"%A_ScriptFullPath%" /restart 3 0.lnk
else if FileExist("F3\0.url")
Run,"%A_ScriptFullPath%" /restart 3 0.url
else
{
FileCreateDir,F3
Run,F3
}
Return


F4 & Enter Up::
FileCreateDir,F4
Run,F4
Return

F4 & a Up::
if FileExist("F4\a.lnk")
Run,"%A_ScriptFullPath%" /restart 4 a.lnk
else if FileExist("F4\a.url")
Run,"%A_ScriptFullPath%" /restart 4 a.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & b Up::
if FileExist("F4\b.lnk")
Run,"%A_ScriptFullPath%" /restart 4 b.lnk
else if FileExist("F4\b.url")
Run,"%A_ScriptFullPath%" /restart 4 b.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & c Up::
if FileExist("F4\c.lnk")
Run,"%A_ScriptFullPath%" /restart 4 c.lnk
else if FileExist("F4\c.url")
Run,"%A_ScriptFullPath%" /restart 4 c.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & d Up::
if FileExist("F4\d.lnk")
Run,"%A_ScriptFullPath%" /restart 4 d.lnk
else if FileExist("F4\d.url")
Run,"%A_ScriptFullPath%" /restart 4 d.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & e Up::
if FileExist("F4\e.lnk")
Run,"%A_ScriptFullPath%" /restart 4 e.lnk
else if FileExist("F4\e.url")
Run,"%A_ScriptFullPath%" /restart 4 e.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & f Up::
if FileExist("F4\f.lnk")
Run,"%A_ScriptFullPath%" /restart 4 f.lnk
else if FileExist("F4\f.url")
Run,"%A_ScriptFullPath%" /restart 4 f.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & g Up::
if FileExist("F4\g.lnk")
Run,"%A_ScriptFullPath%" /restart 4 g.lnk
else if FileExist("F4\g.url")
Run,"%A_ScriptFullPath%" /restart 4 g.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & h Up::
if FileExist("F4\h.lnk")
Run,"%A_ScriptFullPath%" /restart 4 h.lnk
else if FileExist("F4\h.url")
Run,"%A_ScriptFullPath%" /restart 4 h.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & i Up::
if FileExist("F4\i.lnk")
Run,"%A_ScriptFullPath%" /restart 4 i.lnk
else if FileExist("F4\i.url")
Run,"%A_ScriptFullPath%" /restart 4 i.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & j Up::
if FileExist("F4\j.lnk")
Run,"%A_ScriptFullPath%" /restart 4 j.lnk
else if FileExist("F4\j.url")
Run,"%A_ScriptFullPath%" /restart 4 j.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & k Up::
if FileExist("F4\k.lnk")
Run,"%A_ScriptFullPath%" /restart 4 k.lnk
else if FileExist("F4\k.url")
Run,"%A_ScriptFullPath%" /restart 4 k.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & l Up::
if FileExist("F4\l.lnk")
Run,"%A_ScriptFullPath%" /restart 4 l.lnk
else if FileExist("F4\l.url")
Run,"%A_ScriptFullPath%" /restart 4 l.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & m Up::
if FileExist("F4\m.lnk")
Run,"%A_ScriptFullPath%" /restart 4 m.lnk
else if FileExist("F4\m.url")
Run,"%A_ScriptFullPath%" /restart 4 m.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & n Up::
if FileExist("F4\n.lnk")
Run,"%A_ScriptFullPath%" /restart 4 n.lnk
else if FileExist("F4\n.url")
Run,"%A_ScriptFullPath%" /restart 4 n.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & o Up::
if FileExist("F4\o.lnk")
Run,"%A_ScriptFullPath%" /restart 4 o.lnk
else if FileExist("F4\o.url")
Run,"%A_ScriptFullPath%" /restart 4 o.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & p Up::
if FileExist("F4\p.lnk")
Run,"%A_ScriptFullPath%" /restart 4 p.lnk
else if FileExist("F4\p.url")
Run,"%A_ScriptFullPath%" /restart 4 p.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & q Up::
if FileExist("F4\q.lnk")
Run,"%A_ScriptFullPath%" /restart 4 q.lnk
else if FileExist("F4\q.url")
Run,"%A_ScriptFullPath%" /restart 4 q.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & r Up::
if FileExist("F4\r.lnk")
Run,"%A_ScriptFullPath%" /restart 4 r.lnk
else if FileExist("F4\r.url")
Run,"%A_ScriptFullPath%" /restart 4 r.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & s Up::
if FileExist("F4\s.lnk")
Run,"%A_ScriptFullPath%" /restart 4 s.lnk
else if FileExist("F4\s.url")
Run,"%A_ScriptFullPath%" /restart 4 s.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & t Up::
if FileExist("F4\t.lnk")
Run,"%A_ScriptFullPath%" /restart 4 t.lnk
else if FileExist("F4\t.url")
Run,"%A_ScriptFullPath%" /restart 4 t.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & u Up::
if FileExist("F4\u.lnk")
Run,"%A_ScriptFullPath%" /restart 4 u.lnk
else if FileExist("F4\u.url")
Run,"%A_ScriptFullPath%" /restart 4 u.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & v Up::
if FileExist("F4\v.lnk")
Run,"%A_ScriptFullPath%" /restart 4 v.lnk
else if FileExist("F4\v.url")
Run,"%A_ScriptFullPath%" /restart 4 v.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & w Up::
if FileExist("F4\w.lnk")
Run,"%A_ScriptFullPath%" /restart 4 w.lnk
else if FileExist("F4\w.url")
Run,"%A_ScriptFullPath%" /restart 4 w.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & x Up::
if FileExist("F4\x.lnk")
Run,"%A_ScriptFullPath%" /restart 4 x.lnk
else if FileExist("F4\x.url")
Run,"%A_ScriptFullPath%" /restart 4 x.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & y Up::
if FileExist("F4\y.lnk")
Run,"%A_ScriptFullPath%" /restart 4 y.lnk
else if FileExist("F4\y.url")
Run,"%A_ScriptFullPath%" /restart 4 y.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & z Up::
if FileExist("F4\z.lnk")
Run,"%A_ScriptFullPath%" /restart 4 z.lnk
else if FileExist("F4\z.url")
Run,"%A_ScriptFullPath%" /restart 4 z.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & 1 Up::
if FileExist("F4\1.lnk")
Run,"%A_ScriptFullPath%" /restart 4 1.lnk
else if FileExist("F4\1.url")
Run,"%A_ScriptFullPath%" /restart 4 1.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & 2 Up::
if FileExist("F4\2.lnk")
Run,"%A_ScriptFullPath%" /restart 4 2.lnk
else if FileExist("F4\2.url")
Run,"%A_ScriptFullPath%" /restart 4 2.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & 3 Up::
if FileExist("F4\3.lnk")
Run,"%A_ScriptFullPath%" /restart 4 3.lnk
else if FileExist("F4\3.url")
Run,"%A_ScriptFullPath%" /restart 4 3.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & 4 Up::
if FileExist("F4\4.lnk")
Run,"%A_ScriptFullPath%" /restart 4 4.lnk
else if FileExist("F4\4.url")
Run,"%A_ScriptFullPath%" /restart 4 4.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & 5 Up::
if FileExist("F4\5.lnk")
Run,"%A_ScriptFullPath%" /restart 4 5.lnk
else if FileExist("F4\5.url")
Run,"%A_ScriptFullPath%" /restart 4 5.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & 6 Up::
if FileExist("F4\6.lnk")
Run,"%A_ScriptFullPath%" /restart 4 6.lnk
else if FileExist("F4\6.url")
Run,"%A_ScriptFullPath%" /restart 4 6.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & 7 Up::
if FileExist("F4\7.lnk")
Run,"%A_ScriptFullPath%" /restart 4 7.lnk
else if FileExist("F4\7.url")
Run,"%A_ScriptFullPath%" /restart 4 7.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & 8 Up::
if FileExist("F4\8.lnk")
Run,"%A_ScriptFullPath%" /restart 4 8.lnk
else if FileExist("F4\8.url")
Run,"%A_ScriptFullPath%" /restart 4 8.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & 9 Up::
if FileExist("F4\9.lnk")
Run,"%A_ScriptFullPath%" /restart 4 9.lnk
else if FileExist("F4\9.url")
Run,"%A_ScriptFullPath%" /restart 4 9.url
else
{
FileCreateDir,F4
Run,F4
}
Return

F4 & 0 Up::
if FileExist("F4\0.lnk")
Run,"%A_ScriptFullPath%" /restart 4 0.lnk
else if FileExist("F4\0.url")
Run,"%A_ScriptFullPath%" /restart 4 0.url
else
{
FileCreateDir,F4
Run,F4
}
Return


F5 & Enter Up::
FileCreateDir,F5
Run,F5
Return

F5 & a Up::
if FileExist("F5\a.lnk")
Run,"%A_ScriptFullPath%" /restart 5 a.lnk
else if FileExist("F5\a.url")
Run,"%A_ScriptFullPath%" /restart 5 a.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & b Up::
if FileExist("F5\b.lnk")
Run,"%A_ScriptFullPath%" /restart 5 b.lnk
else if FileExist("F5\b.url")
Run,"%A_ScriptFullPath%" /restart 5 b.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & c Up::
if FileExist("F5\c.lnk")
Run,"%A_ScriptFullPath%" /restart 5 c.lnk
else if FileExist("F5\c.url")
Run,"%A_ScriptFullPath%" /restart 5 c.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & d Up::
if FileExist("F5\d.lnk")
Run,"%A_ScriptFullPath%" /restart 5 d.lnk
else if FileExist("F5\d.url")
Run,"%A_ScriptFullPath%" /restart 5 d.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & e Up::
if FileExist("F5\e.lnk")
Run,"%A_ScriptFullPath%" /restart 5 e.lnk
else if FileExist("F5\e.url")
Run,"%A_ScriptFullPath%" /restart 5 e.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & f Up::
if FileExist("F5\f.lnk")
Run,"%A_ScriptFullPath%" /restart 5 f.lnk
else if FileExist("F5\f.url")
Run,"%A_ScriptFullPath%" /restart 5 f.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & g Up::
if FileExist("F5\g.lnk")
Run,"%A_ScriptFullPath%" /restart 5 g.lnk
else if FileExist("F5\g.url")
Run,"%A_ScriptFullPath%" /restart 5 g.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & h Up::
if FileExist("F5\h.lnk")
Run,"%A_ScriptFullPath%" /restart 5 h.lnk
else if FileExist("F5\h.url")
Run,"%A_ScriptFullPath%" /restart 5 h.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & i Up::
if FileExist("F5\i.lnk")
Run,"%A_ScriptFullPath%" /restart 5 i.lnk
else if FileExist("F5\i.url")
Run,"%A_ScriptFullPath%" /restart 5 i.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & j Up::
if FileExist("F5\j.lnk")
Run,"%A_ScriptFullPath%" /restart 5 j.lnk
else if FileExist("F5\j.url")
Run,"%A_ScriptFullPath%" /restart 5 j.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & k Up::
if FileExist("F5\k.lnk")
Run,"%A_ScriptFullPath%" /restart 5 k.lnk
else if FileExist("F5\k.url")
Run,"%A_ScriptFullPath%" /restart 5 k.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & l Up::
if FileExist("F5\l.lnk")
Run,"%A_ScriptFullPath%" /restart 5 l.lnk
else if FileExist("F5\l.url")
Run,"%A_ScriptFullPath%" /restart 5 l.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & m Up::
if FileExist("F5\m.lnk")
Run,"%A_ScriptFullPath%" /restart 5 m.lnk
else if FileExist("F5\m.url")
Run,"%A_ScriptFullPath%" /restart 5 m.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & n Up::
if FileExist("F5\n.lnk")
Run,"%A_ScriptFullPath%" /restart 5 n.lnk
else if FileExist("F5\n.url")
Run,"%A_ScriptFullPath%" /restart 5 n.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & o Up::
if FileExist("F5\o.lnk")
Run,"%A_ScriptFullPath%" /restart 5 o.lnk
else if FileExist("F5\o.url")
Run,"%A_ScriptFullPath%" /restart 5 o.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & p Up::
if FileExist("F5\p.lnk")
Run,"%A_ScriptFullPath%" /restart 5 p.lnk
else if FileExist("F5\p.url")
Run,"%A_ScriptFullPath%" /restart 5 p.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & q Up::
if FileExist("F5\q.lnk")
Run,"%A_ScriptFullPath%" /restart 5 q.lnk
else if FileExist("F5\q.url")
Run,"%A_ScriptFullPath%" /restart 5 q.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & r Up::
if FileExist("F5\r.lnk")
Run,"%A_ScriptFullPath%" /restart 5 r.lnk
else if FileExist("F5\r.url")
Run,"%A_ScriptFullPath%" /restart 5 r.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & s Up::
if FileExist("F5\s.lnk")
Run,"%A_ScriptFullPath%" /restart 5 s.lnk
else if FileExist("F5\s.url")
Run,"%A_ScriptFullPath%" /restart 5 s.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & t Up::
if FileExist("F5\t.lnk")
Run,"%A_ScriptFullPath%" /restart 5 t.lnk
else if FileExist("F5\t.url")
Run,"%A_ScriptFullPath%" /restart 5 t.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & u Up::
if FileExist("F5\u.lnk")
Run,"%A_ScriptFullPath%" /restart 5 u.lnk
else if FileExist("F5\u.url")
Run,"%A_ScriptFullPath%" /restart 5 u.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & v Up::
if FileExist("F5\v.lnk")
Run,"%A_ScriptFullPath%" /restart 5 v.lnk
else if FileExist("F5\v.url")
Run,"%A_ScriptFullPath%" /restart 5 v.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & w Up::
if FileExist("F5\w.lnk")
Run,"%A_ScriptFullPath%" /restart 5 w.lnk
else if FileExist("F5\w.url")
Run,"%A_ScriptFullPath%" /restart 5 w.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & x Up::
if FileExist("F5\x.lnk")
Run,"%A_ScriptFullPath%" /restart 5 x.lnk
else if FileExist("F5\x.url")
Run,"%A_ScriptFullPath%" /restart 5 x.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & y Up::
if FileExist("F5\y.lnk")
Run,"%A_ScriptFullPath%" /restart 5 y.lnk
else if FileExist("F5\y.url")
Run,"%A_ScriptFullPath%" /restart 5 y.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & z Up::
if FileExist("F5\z.lnk")
Run,"%A_ScriptFullPath%" /restart 5 z.lnk
else if FileExist("F5\z.url")
Run,"%A_ScriptFullPath%" /restart 5 z.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & 1 Up::
if FileExist("F5\1.lnk")
Run,"%A_ScriptFullPath%" /restart 5 1.lnk
else if FileExist("F5\1.url")
Run,"%A_ScriptFullPath%" /restart 5 1.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & 2 Up::
if FileExist("F5\2.lnk")
Run,"%A_ScriptFullPath%" /restart 5 2.lnk
else if FileExist("F5\2.url")
Run,"%A_ScriptFullPath%" /restart 5 2.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & 3 Up::
if FileExist("F5\3.lnk")
Run,"%A_ScriptFullPath%" /restart 5 3.lnk
else if FileExist("F5\3.url")
Run,"%A_ScriptFullPath%" /restart 5 3.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & 4 Up::
if FileExist("F5\4.lnk")
Run,"%A_ScriptFullPath%" /restart 5 4.lnk
else if FileExist("F5\4.url")
Run,"%A_ScriptFullPath%" /restart 5 4.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & 5 Up::
if FileExist("F5\5.lnk")
Run,"%A_ScriptFullPath%" /restart 5 5.lnk
else if FileExist("F5\5.url")
Run,"%A_ScriptFullPath%" /restart 5 5.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & 6 Up::
if FileExist("F5\6.lnk")
Run,"%A_ScriptFullPath%" /restart 5 6.lnk
else if FileExist("F5\6.url")
Run,"%A_ScriptFullPath%" /restart 5 6.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & 7 Up::
if FileExist("F5\7.lnk")
Run,"%A_ScriptFullPath%" /restart 5 7.lnk
else if FileExist("F5\7.url")
Run,"%A_ScriptFullPath%" /restart 5 7.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & 8 Up::
if FileExist("F5\8.lnk")
Run,"%A_ScriptFullPath%" /restart 5 8.lnk
else if FileExist("F5\8.url")
Run,"%A_ScriptFullPath%" /restart 5 8.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & 9 Up::
if FileExist("F5\9.lnk")
Run,"%A_ScriptFullPath%" /restart 5 9.lnk
else if FileExist("F5\9.url")
Run,"%A_ScriptFullPath%" /restart 5 9.url
else
{
FileCreateDir,F5
Run,F5
}
Return

F5 & 0 Up::
if FileExist("F5\0.lnk")
Run,"%A_ScriptFullPath%" /restart 5 0.lnk
else if FileExist("F5\0.url")
Run,"%A_ScriptFullPath%" /restart 5 0.url
else
{
FileCreateDir,F5
Run,F5
}
Return


F6 & Enter Up::
FileCreateDir,F6
Run,F6
Return

F6 & a Up::
if FileExist("F6\a.lnk")
Run,"%A_ScriptFullPath%" /restart 6 a.lnk
else if FileExist("F6\a.url")
Run,"%A_ScriptFullPath%" /restart 6 a.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & b Up::
if FileExist("F6\b.lnk")
Run,"%A_ScriptFullPath%" /restart 6 b.lnk
else if FileExist("F6\b.url")
Run,"%A_ScriptFullPath%" /restart 6 b.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & c Up::
if FileExist("F6\c.lnk")
Run,"%A_ScriptFullPath%" /restart 6 c.lnk
else if FileExist("F6\c.url")
Run,"%A_ScriptFullPath%" /restart 6 c.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & d Up::
if FileExist("F6\d.lnk")
Run,"%A_ScriptFullPath%" /restart 6 d.lnk
else if FileExist("F6\d.url")
Run,"%A_ScriptFullPath%" /restart 6 d.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & e Up::
if FileExist("F6\e.lnk")
Run,"%A_ScriptFullPath%" /restart 6 e.lnk
else if FileExist("F6\e.url")
Run,"%A_ScriptFullPath%" /restart 6 e.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & f Up::
if FileExist("F6\f.lnk")
Run,"%A_ScriptFullPath%" /restart 6 f.lnk
else if FileExist("F6\f.url")
Run,"%A_ScriptFullPath%" /restart 6 f.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & g Up::
if FileExist("F6\g.lnk")
Run,"%A_ScriptFullPath%" /restart 6 g.lnk
else if FileExist("F6\g.url")
Run,"%A_ScriptFullPath%" /restart 6 g.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & h Up::
if FileExist("F6\h.lnk")
Run,"%A_ScriptFullPath%" /restart 6 h.lnk
else if FileExist("F6\h.url")
Run,"%A_ScriptFullPath%" /restart 6 h.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & i Up::
if FileExist("F6\i.lnk")
Run,"%A_ScriptFullPath%" /restart 6 i.lnk
else if FileExist("F6\i.url")
Run,"%A_ScriptFullPath%" /restart 6 i.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & j Up::
if FileExist("F6\j.lnk")
Run,"%A_ScriptFullPath%" /restart 6 j.lnk
else if FileExist("F6\j.url")
Run,"%A_ScriptFullPath%" /restart 6 j.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & k Up::
if FileExist("F6\k.lnk")
Run,"%A_ScriptFullPath%" /restart 6 k.lnk
else if FileExist("F6\k.url")
Run,"%A_ScriptFullPath%" /restart 6 k.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & l Up::
if FileExist("F6\l.lnk")
Run,"%A_ScriptFullPath%" /restart 6 l.lnk
else if FileExist("F6\l.url")
Run,"%A_ScriptFullPath%" /restart 6 l.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & m Up::
if FileExist("F6\m.lnk")
Run,"%A_ScriptFullPath%" /restart 6 m.lnk
else if FileExist("F6\m.url")
Run,"%A_ScriptFullPath%" /restart 6 m.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & n Up::
if FileExist("F6\n.lnk")
Run,"%A_ScriptFullPath%" /restart 6 n.lnk
else if FileExist("F6\n.url")
Run,"%A_ScriptFullPath%" /restart 6 n.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & o Up::
if FileExist("F6\o.lnk")
Run,"%A_ScriptFullPath%" /restart 6 o.lnk
else if FileExist("F6\o.url")
Run,"%A_ScriptFullPath%" /restart 6 o.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & p Up::
if FileExist("F6\p.lnk")
Run,"%A_ScriptFullPath%" /restart 6 p.lnk
else if FileExist("F6\p.url")
Run,"%A_ScriptFullPath%" /restart 6 p.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & q Up::
if FileExist("F6\q.lnk")
Run,"%A_ScriptFullPath%" /restart 6 q.lnk
else if FileExist("F6\q.url")
Run,"%A_ScriptFullPath%" /restart 6 q.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & r Up::
if FileExist("F6\r.lnk")
Run,"%A_ScriptFullPath%" /restart 6 r.lnk
else if FileExist("F6\r.url")
Run,"%A_ScriptFullPath%" /restart 6 r.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & s Up::
if FileExist("F6\s.lnk")
Run,"%A_ScriptFullPath%" /restart 6 s.lnk
else if FileExist("F6\s.url")
Run,"%A_ScriptFullPath%" /restart 6 s.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & t Up::
if FileExist("F6\t.lnk")
Run,"%A_ScriptFullPath%" /restart 6 t.lnk
else if FileExist("F6\t.url")
Run,"%A_ScriptFullPath%" /restart 6 t.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & u Up::
if FileExist("F6\u.lnk")
Run,"%A_ScriptFullPath%" /restart 6 u.lnk
else if FileExist("F6\u.url")
Run,"%A_ScriptFullPath%" /restart 6 u.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & v Up::
if FileExist("F6\v.lnk")
Run,"%A_ScriptFullPath%" /restart 6 v.lnk
else if FileExist("F6\v.url")
Run,"%A_ScriptFullPath%" /restart 6 v.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & w Up::
if FileExist("F6\w.lnk")
Run,"%A_ScriptFullPath%" /restart 6 w.lnk
else if FileExist("F6\w.url")
Run,"%A_ScriptFullPath%" /restart 6 w.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & x Up::
if FileExist("F6\x.lnk")
Run,"%A_ScriptFullPath%" /restart 6 x.lnk
else if FileExist("F6\x.url")
Run,"%A_ScriptFullPath%" /restart 6 x.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & y Up::
if FileExist("F6\y.lnk")
Run,"%A_ScriptFullPath%" /restart 6 y.lnk
else if FileExist("F6\y.url")
Run,"%A_ScriptFullPath%" /restart 6 y.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & z Up::
if FileExist("F6\z.lnk")
Run,"%A_ScriptFullPath%" /restart 6 z.lnk
else if FileExist("F6\z.url")
Run,"%A_ScriptFullPath%" /restart 6 z.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & 1 Up::
if FileExist("F6\1.lnk")
Run,"%A_ScriptFullPath%" /restart 6 1.lnk
else if FileExist("F6\1.url")
Run,"%A_ScriptFullPath%" /restart 6 1.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & 2 Up::
if FileExist("F6\2.lnk")
Run,"%A_ScriptFullPath%" /restart 6 2.lnk
else if FileExist("F6\2.url")
Run,"%A_ScriptFullPath%" /restart 6 2.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & 3 Up::
if FileExist("F6\3.lnk")
Run,"%A_ScriptFullPath%" /restart 6 3.lnk
else if FileExist("F6\3.url")
Run,"%A_ScriptFullPath%" /restart 6 3.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & 4 Up::
if FileExist("F6\4.lnk")
Run,"%A_ScriptFullPath%" /restart 6 4.lnk
else if FileExist("F6\4.url")
Run,"%A_ScriptFullPath%" /restart 6 4.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & 5 Up::
if FileExist("F6\5.lnk")
Run,"%A_ScriptFullPath%" /restart 6 5.lnk
else if FileExist("F6\5.url")
Run,"%A_ScriptFullPath%" /restart 6 5.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & 6 Up::
if FileExist("F6\6.lnk")
Run,"%A_ScriptFullPath%" /restart 6 6.lnk
else if FileExist("F6\6.url")
Run,"%A_ScriptFullPath%" /restart 6 6.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & 7 Up::
if FileExist("F6\7.lnk")
Run,"%A_ScriptFullPath%" /restart 6 7.lnk
else if FileExist("F6\7.url")
Run,"%A_ScriptFullPath%" /restart 6 7.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & 8 Up::
if FileExist("F6\8.lnk")
Run,"%A_ScriptFullPath%" /restart 6 8.lnk
else if FileExist("F6\8.url")
Run,"%A_ScriptFullPath%" /restart 6 8.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & 9 Up::
if FileExist("F6\9.lnk")
Run,"%A_ScriptFullPath%" /restart 6 9.lnk
else if FileExist("F6\9.url")
Run,"%A_ScriptFullPath%" /restart 6 9.url
else
{
FileCreateDir,F6
Run,F6
}
Return

F6 & 0 Up::
if FileExist("F6\0.lnk")
Run,"%A_ScriptFullPath%" /restart 6 0.lnk
else if FileExist("F6\0.url")
Run,"%A_ScriptFullPath%" /restart 6 0.url
else
{
FileCreateDir,F6
Run,F6
}
Return


F7 & Enter Up::
FileCreateDir,F7
Run,F7
Return

F7 & a Up::
if FileExist("F7\a.lnk")
Run,"%A_ScriptFullPath%" /restart 7 a.lnk
else if FileExist("F7\a.url")
Run,"%A_ScriptFullPath%" /restart 7 a.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & b Up::
if FileExist("F7\b.lnk")
Run,"%A_ScriptFullPath%" /restart 7 b.lnk
else if FileExist("F7\b.url")
Run,"%A_ScriptFullPath%" /restart 7 b.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & c Up::
if FileExist("F7\c.lnk")
Run,"%A_ScriptFullPath%" /restart 7 c.lnk
else if FileExist("F7\c.url")
Run,"%A_ScriptFullPath%" /restart 7 c.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & d Up::
if FileExist("F7\d.lnk")
Run,"%A_ScriptFullPath%" /restart 7 d.lnk
else if FileExist("F7\d.url")
Run,"%A_ScriptFullPath%" /restart 7 d.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & e Up::
if FileExist("F7\e.lnk")
Run,"%A_ScriptFullPath%" /restart 7 e.lnk
else if FileExist("F7\e.url")
Run,"%A_ScriptFullPath%" /restart 7 e.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & f Up::
if FileExist("F7\f.lnk")
Run,"%A_ScriptFullPath%" /restart 7 f.lnk
else if FileExist("F7\f.url")
Run,"%A_ScriptFullPath%" /restart 7 f.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & g Up::
if FileExist("F7\g.lnk")
Run,"%A_ScriptFullPath%" /restart 7 g.lnk
else if FileExist("F7\g.url")
Run,"%A_ScriptFullPath%" /restart 7 g.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & h Up::
if FileExist("F7\h.lnk")
Run,"%A_ScriptFullPath%" /restart 7 h.lnk
else if FileExist("F7\h.url")
Run,"%A_ScriptFullPath%" /restart 7 h.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & i Up::
if FileExist("F7\i.lnk")
Run,"%A_ScriptFullPath%" /restart 7 i.lnk
else if FileExist("F7\i.url")
Run,"%A_ScriptFullPath%" /restart 7 i.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & j Up::
if FileExist("F7\j.lnk")
Run,"%A_ScriptFullPath%" /restart 7 j.lnk
else if FileExist("F7\j.url")
Run,"%A_ScriptFullPath%" /restart 7 j.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & k Up::
if FileExist("F7\k.lnk")
Run,"%A_ScriptFullPath%" /restart 7 k.lnk
else if FileExist("F7\k.url")
Run,"%A_ScriptFullPath%" /restart 7 k.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & l Up::
if FileExist("F7\l.lnk")
Run,"%A_ScriptFullPath%" /restart 7 l.lnk
else if FileExist("F7\l.url")
Run,"%A_ScriptFullPath%" /restart 7 l.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & m Up::
if FileExist("F7\m.lnk")
Run,"%A_ScriptFullPath%" /restart 7 m.lnk
else if FileExist("F7\m.url")
Run,"%A_ScriptFullPath%" /restart 7 m.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & n Up::
if FileExist("F7\n.lnk")
Run,"%A_ScriptFullPath%" /restart 7 n.lnk
else if FileExist("F7\n.url")
Run,"%A_ScriptFullPath%" /restart 7 n.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & o Up::
if FileExist("F7\o.lnk")
Run,"%A_ScriptFullPath%" /restart 7 o.lnk
else if FileExist("F7\o.url")
Run,"%A_ScriptFullPath%" /restart 7 o.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & p Up::
if FileExist("F7\p.lnk")
Run,"%A_ScriptFullPath%" /restart 7 p.lnk
else if FileExist("F7\p.url")
Run,"%A_ScriptFullPath%" /restart 7 p.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & q Up::
if FileExist("F7\q.lnk")
Run,"%A_ScriptFullPath%" /restart 7 q.lnk
else if FileExist("F7\q.url")
Run,"%A_ScriptFullPath%" /restart 7 q.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & r Up::
if FileExist("F7\r.lnk")
Run,"%A_ScriptFullPath%" /restart 7 r.lnk
else if FileExist("F7\r.url")
Run,"%A_ScriptFullPath%" /restart 7 r.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & s Up::
if FileExist("F7\s.lnk")
Run,"%A_ScriptFullPath%" /restart 7 s.lnk
else if FileExist("F7\s.url")
Run,"%A_ScriptFullPath%" /restart 7 s.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & t Up::
if FileExist("F7\t.lnk")
Run,"%A_ScriptFullPath%" /restart 7 t.lnk
else if FileExist("F7\t.url")
Run,"%A_ScriptFullPath%" /restart 7 t.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & u Up::
if FileExist("F7\u.lnk")
Run,"%A_ScriptFullPath%" /restart 7 u.lnk
else if FileExist("F7\u.url")
Run,"%A_ScriptFullPath%" /restart 7 u.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & v Up::
if FileExist("F7\v.lnk")
Run,"%A_ScriptFullPath%" /restart 7 v.lnk
else if FileExist("F7\v.url")
Run,"%A_ScriptFullPath%" /restart 7 v.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & w Up::
if FileExist("F7\w.lnk")
Run,"%A_ScriptFullPath%" /restart 7 w.lnk
else if FileExist("F7\w.url")
Run,"%A_ScriptFullPath%" /restart 7 w.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & x Up::
if FileExist("F7\x.lnk")
Run,"%A_ScriptFullPath%" /restart 7 x.lnk
else if FileExist("F7\x.url")
Run,"%A_ScriptFullPath%" /restart 7 x.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & y Up::
if FileExist("F7\y.lnk")
Run,"%A_ScriptFullPath%" /restart 7 y.lnk
else if FileExist("F7\y.url")
Run,"%A_ScriptFullPath%" /restart 7 y.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & z Up::
if FileExist("F7\z.lnk")
Run,"%A_ScriptFullPath%" /restart 7 z.lnk
else if FileExist("F7\z.url")
Run,"%A_ScriptFullPath%" /restart 7 z.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & 1 Up::
if FileExist("F7\1.lnk")
Run,"%A_ScriptFullPath%" /restart 7 1.lnk
else if FileExist("F7\1.url")
Run,"%A_ScriptFullPath%" /restart 7 1.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & 2 Up::
if FileExist("F7\2.lnk")
Run,"%A_ScriptFullPath%" /restart 7 2.lnk
else if FileExist("F7\2.url")
Run,"%A_ScriptFullPath%" /restart 7 2.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & 3 Up::
if FileExist("F7\3.lnk")
Run,"%A_ScriptFullPath%" /restart 7 3.lnk
else if FileExist("F7\3.url")
Run,"%A_ScriptFullPath%" /restart 7 3.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & 4 Up::
if FileExist("F7\4.lnk")
Run,"%A_ScriptFullPath%" /restart 7 4.lnk
else if FileExist("F7\4.url")
Run,"%A_ScriptFullPath%" /restart 7 4.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & 5 Up::
if FileExist("F7\5.lnk")
Run,"%A_ScriptFullPath%" /restart 7 5.lnk
else if FileExist("F7\5.url")
Run,"%A_ScriptFullPath%" /restart 7 5.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & 6 Up::
if FileExist("F7\6.lnk")
Run,"%A_ScriptFullPath%" /restart 7 6.lnk
else if FileExist("F7\6.url")
Run,"%A_ScriptFullPath%" /restart 7 6.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & 7 Up::
if FileExist("F7\7.lnk")
Run,"%A_ScriptFullPath%" /restart 7 7.lnk
else if FileExist("F7\7.url")
Run,"%A_ScriptFullPath%" /restart 7 7.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & 8 Up::
if FileExist("F7\8.lnk")
Run,"%A_ScriptFullPath%" /restart 7 8.lnk
else if FileExist("F7\8.url")
Run,"%A_ScriptFullPath%" /restart 7 8.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & 9 Up::
if FileExist("F7\9.lnk")
Run,"%A_ScriptFullPath%" /restart 7 9.lnk
else if FileExist("F7\9.url")
Run,"%A_ScriptFullPath%" /restart 7 9.url
else
{
FileCreateDir,F7
Run,F7
}
Return

F7 & 0 Up::
if FileExist("F7\0.lnk")
Run,"%A_ScriptFullPath%" /restart 7 0.lnk
else if FileExist("F7\0.url")
Run,"%A_ScriptFullPath%" /restart 7 0.url
else
{
FileCreateDir,F7
Run,F7
}
Return


F8 & Enter Up::
FileCreateDir,F8
Run,F8
Return

F8 & a Up::
if FileExist("F8\a.lnk")
Run,"%A_ScriptFullPath%" /restart 8 a.lnk
else if FileExist("F8\a.url")
Run,"%A_ScriptFullPath%" /restart 8 a.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & b Up::
if FileExist("F8\b.lnk")
Run,"%A_ScriptFullPath%" /restart 8 b.lnk
else if FileExist("F8\b.url")
Run,"%A_ScriptFullPath%" /restart 8 b.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & c Up::
if FileExist("F8\c.lnk")
Run,"%A_ScriptFullPath%" /restart 8 c.lnk
else if FileExist("F8\c.url")
Run,"%A_ScriptFullPath%" /restart 8 c.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & d Up::
if FileExist("F8\d.lnk")
Run,"%A_ScriptFullPath%" /restart 8 d.lnk
else if FileExist("F8\d.url")
Run,"%A_ScriptFullPath%" /restart 8 d.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & e Up::
if FileExist("F8\e.lnk")
Run,"%A_ScriptFullPath%" /restart 8 e.lnk
else if FileExist("F8\e.url")
Run,"%A_ScriptFullPath%" /restart 8 e.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & f Up::
if FileExist("F8\f.lnk")
Run,"%A_ScriptFullPath%" /restart 8 f.lnk
else if FileExist("F8\f.url")
Run,"%A_ScriptFullPath%" /restart 8 f.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & g Up::
if FileExist("F8\g.lnk")
Run,"%A_ScriptFullPath%" /restart 8 g.lnk
else if FileExist("F8\g.url")
Run,"%A_ScriptFullPath%" /restart 8 g.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & h Up::
if FileExist("F8\h.lnk")
Run,"%A_ScriptFullPath%" /restart 8 h.lnk
else if FileExist("F8\h.url")
Run,"%A_ScriptFullPath%" /restart 8 h.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & i Up::
if FileExist("F8\i.lnk")
Run,"%A_ScriptFullPath%" /restart 8 i.lnk
else if FileExist("F8\i.url")
Run,"%A_ScriptFullPath%" /restart 8 i.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & j Up::
if FileExist("F8\j.lnk")
Run,"%A_ScriptFullPath%" /restart 8 j.lnk
else if FileExist("F8\j.url")
Run,"%A_ScriptFullPath%" /restart 8 j.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & k Up::
if FileExist("F8\k.lnk")
Run,"%A_ScriptFullPath%" /restart 8 k.lnk
else if FileExist("F8\k.url")
Run,"%A_ScriptFullPath%" /restart 8 k.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & l Up::
if FileExist("F8\l.lnk")
Run,"%A_ScriptFullPath%" /restart 8 l.lnk
else if FileExist("F8\l.url")
Run,"%A_ScriptFullPath%" /restart 8 l.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & m Up::
if FileExist("F8\m.lnk")
Run,"%A_ScriptFullPath%" /restart 8 m.lnk
else if FileExist("F8\m.url")
Run,"%A_ScriptFullPath%" /restart 8 m.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & n Up::
if FileExist("F8\n.lnk")
Run,"%A_ScriptFullPath%" /restart 8 n.lnk
else if FileExist("F8\n.url")
Run,"%A_ScriptFullPath%" /restart 8 n.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & o Up::
if FileExist("F8\o.lnk")
Run,"%A_ScriptFullPath%" /restart 8 o.lnk
else if FileExist("F8\o.url")
Run,"%A_ScriptFullPath%" /restart 8 o.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & p Up::
if FileExist("F8\p.lnk")
Run,"%A_ScriptFullPath%" /restart 8 p.lnk
else if FileExist("F8\p.url")
Run,"%A_ScriptFullPath%" /restart 8 p.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & q Up::
if FileExist("F8\q.lnk")
Run,"%A_ScriptFullPath%" /restart 8 q.lnk
else if FileExist("F8\q.url")
Run,"%A_ScriptFullPath%" /restart 8 q.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & r Up::
if FileExist("F8\r.lnk")
Run,"%A_ScriptFullPath%" /restart 8 r.lnk
else if FileExist("F8\r.url")
Run,"%A_ScriptFullPath%" /restart 8 r.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & s Up::
if FileExist("F8\s.lnk")
Run,"%A_ScriptFullPath%" /restart 8 s.lnk
else if FileExist("F8\s.url")
Run,"%A_ScriptFullPath%" /restart 8 s.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & t Up::
if FileExist("F8\t.lnk")
Run,"%A_ScriptFullPath%" /restart 8 t.lnk
else if FileExist("F8\t.url")
Run,"%A_ScriptFullPath%" /restart 8 t.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & u Up::
if FileExist("F8\u.lnk")
Run,"%A_ScriptFullPath%" /restart 8 u.lnk
else if FileExist("F8\u.url")
Run,"%A_ScriptFullPath%" /restart 8 u.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & v Up::
if FileExist("F8\v.lnk")
Run,"%A_ScriptFullPath%" /restart 8 v.lnk
else if FileExist("F8\v.url")
Run,"%A_ScriptFullPath%" /restart 8 v.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & w Up::
if FileExist("F8\w.lnk")
Run,"%A_ScriptFullPath%" /restart 8 w.lnk
else if FileExist("F8\w.url")
Run,"%A_ScriptFullPath%" /restart 8 w.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & x Up::
if FileExist("F8\x.lnk")
Run,"%A_ScriptFullPath%" /restart 8 x.lnk
else if FileExist("F8\x.url")
Run,"%A_ScriptFullPath%" /restart 8 x.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & y Up::
if FileExist("F8\y.lnk")
Run,"%A_ScriptFullPath%" /restart 8 y.lnk
else if FileExist("F8\y.url")
Run,"%A_ScriptFullPath%" /restart 8 y.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & z Up::
if FileExist("F8\z.lnk")
Run,"%A_ScriptFullPath%" /restart 8 z.lnk
else if FileExist("F8\z.url")
Run,"%A_ScriptFullPath%" /restart 8 z.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & 1 Up::
if FileExist("F8\1.lnk")
Run,"%A_ScriptFullPath%" /restart 8 1.lnk
else if FileExist("F8\1.url")
Run,"%A_ScriptFullPath%" /restart 8 1.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & 2 Up::
if FileExist("F8\2.lnk")
Run,"%A_ScriptFullPath%" /restart 8 2.lnk
else if FileExist("F8\2.url")
Run,"%A_ScriptFullPath%" /restart 8 2.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & 3 Up::
if FileExist("F8\3.lnk")
Run,"%A_ScriptFullPath%" /restart 8 3.lnk
else if FileExist("F8\3.url")
Run,"%A_ScriptFullPath%" /restart 8 3.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & 4 Up::
if FileExist("F8\4.lnk")
Run,"%A_ScriptFullPath%" /restart 8 4.lnk
else if FileExist("F8\4.url")
Run,"%A_ScriptFullPath%" /restart 8 4.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & 5 Up::
if FileExist("F8\5.lnk")
Run,"%A_ScriptFullPath%" /restart 8 5.lnk
else if FileExist("F8\5.url")
Run,"%A_ScriptFullPath%" /restart 8 5.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & 6 Up::
if FileExist("F8\6.lnk")
Run,"%A_ScriptFullPath%" /restart 8 6.lnk
else if FileExist("F8\6.url")
Run,"%A_ScriptFullPath%" /restart 8 6.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & 7 Up::
if FileExist("F8\7.lnk")
Run,"%A_ScriptFullPath%" /restart 8 7.lnk
else if FileExist("F8\7.url")
Run,"%A_ScriptFullPath%" /restart 8 7.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & 8 Up::
if FileExist("F8\8.lnk")
Run,"%A_ScriptFullPath%" /restart 8 8.lnk
else if FileExist("F8\8.url")
Run,"%A_ScriptFullPath%" /restart 8 8.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & 9 Up::
if FileExist("F8\9.lnk")
Run,"%A_ScriptFullPath%" /restart 8 9.lnk
else if FileExist("F8\9.url")
Run,"%A_ScriptFullPath%" /restart 8 9.url
else
{
FileCreateDir,F8
Run,F8
}
Return

F8 & 0 Up::
if FileExist("F8\0.lnk")
Run,"%A_ScriptFullPath%" /restart 8 0.lnk
else if FileExist("F8\0.url")
Run,"%A_ScriptFullPath%" /restart 8 0.url
else
{
FileCreateDir,F8
Run,F8
}
Return


F9 & Enter Up::
FileCreateDir,F9
Run,F9
Return

F9 & a Up::
if FileExist("F9\a.lnk")
Run,"%A_ScriptFullPath%" /restart 9 a.lnk
else if FileExist("F9\a.url")
Run,"%A_ScriptFullPath%" /restart 9 a.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & b Up::
if FileExist("F9\b.lnk")
Run,"%A_ScriptFullPath%" /restart 9 b.lnk
else if FileExist("F9\b.url")
Run,"%A_ScriptFullPath%" /restart 9 b.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & c Up::
if FileExist("F9\c.lnk")
Run,"%A_ScriptFullPath%" /restart 9 c.lnk
else if FileExist("F9\c.url")
Run,"%A_ScriptFullPath%" /restart 9 c.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & d Up::
if FileExist("F9\d.lnk")
Run,"%A_ScriptFullPath%" /restart 9 d.lnk
else if FileExist("F9\d.url")
Run,"%A_ScriptFullPath%" /restart 9 d.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & e Up::
if FileExist("F9\e.lnk")
Run,"%A_ScriptFullPath%" /restart 9 e.lnk
else if FileExist("F9\e.url")
Run,"%A_ScriptFullPath%" /restart 9 e.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & f Up::
if FileExist("F9\f.lnk")
Run,"%A_ScriptFullPath%" /restart 9 f.lnk
else if FileExist("F9\f.url")
Run,"%A_ScriptFullPath%" /restart 9 f.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & g Up::
if FileExist("F9\g.lnk")
Run,"%A_ScriptFullPath%" /restart 9 g.lnk
else if FileExist("F9\g.url")
Run,"%A_ScriptFullPath%" /restart 9 g.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & h Up::
if FileExist("F9\h.lnk")
Run,"%A_ScriptFullPath%" /restart 9 h.lnk
else if FileExist("F9\h.url")
Run,"%A_ScriptFullPath%" /restart 9 h.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & i Up::
if FileExist("F9\i.lnk")
Run,"%A_ScriptFullPath%" /restart 9 i.lnk
else if FileExist("F9\i.url")
Run,"%A_ScriptFullPath%" /restart 9 i.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & j Up::
if FileExist("F9\j.lnk")
Run,"%A_ScriptFullPath%" /restart 9 j.lnk
else if FileExist("F9\j.url")
Run,"%A_ScriptFullPath%" /restart 9 j.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & k Up::
if FileExist("F9\k.lnk")
Run,"%A_ScriptFullPath%" /restart 9 k.lnk
else if FileExist("F9\k.url")
Run,"%A_ScriptFullPath%" /restart 9 k.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & l Up::
if FileExist("F9\l.lnk")
Run,"%A_ScriptFullPath%" /restart 9 l.lnk
else if FileExist("F9\l.url")
Run,"%A_ScriptFullPath%" /restart 9 l.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & m Up::
if FileExist("F9\m.lnk")
Run,"%A_ScriptFullPath%" /restart 9 m.lnk
else if FileExist("F9\m.url")
Run,"%A_ScriptFullPath%" /restart 9 m.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & n Up::
if FileExist("F9\n.lnk")
Run,"%A_ScriptFullPath%" /restart 9 n.lnk
else if FileExist("F9\n.url")
Run,"%A_ScriptFullPath%" /restart 9 n.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & o Up::
if FileExist("F9\o.lnk")
Run,"%A_ScriptFullPath%" /restart 9 o.lnk
else if FileExist("F9\o.url")
Run,"%A_ScriptFullPath%" /restart 9 o.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & p Up::
if FileExist("F9\p.lnk")
Run,"%A_ScriptFullPath%" /restart 9 p.lnk
else if FileExist("F9\p.url")
Run,"%A_ScriptFullPath%" /restart 9 p.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & q Up::
if FileExist("F9\q.lnk")
Run,"%A_ScriptFullPath%" /restart 9 q.lnk
else if FileExist("F9\q.url")
Run,"%A_ScriptFullPath%" /restart 9 q.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & r Up::
if FileExist("F9\r.lnk")
Run,"%A_ScriptFullPath%" /restart 9 r.lnk
else if FileExist("F9\r.url")
Run,"%A_ScriptFullPath%" /restart 9 r.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & s Up::
if FileExist("F9\s.lnk")
Run,"%A_ScriptFullPath%" /restart 9 s.lnk
else if FileExist("F9\s.url")
Run,"%A_ScriptFullPath%" /restart 9 s.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & t Up::
if FileExist("F9\t.lnk")
Run,"%A_ScriptFullPath%" /restart 9 t.lnk
else if FileExist("F9\t.url")
Run,"%A_ScriptFullPath%" /restart 9 t.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & u Up::
if FileExist("F9\u.lnk")
Run,"%A_ScriptFullPath%" /restart 9 u.lnk
else if FileExist("F9\u.url")
Run,"%A_ScriptFullPath%" /restart 9 u.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & v Up::
if FileExist("F9\v.lnk")
Run,"%A_ScriptFullPath%" /restart 9 v.lnk
else if FileExist("F9\v.url")
Run,"%A_ScriptFullPath%" /restart 9 v.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & w Up::
if FileExist("F9\w.lnk")
Run,"%A_ScriptFullPath%" /restart 9 w.lnk
else if FileExist("F9\w.url")
Run,"%A_ScriptFullPath%" /restart 9 w.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & x Up::
if FileExist("F9\x.lnk")
Run,"%A_ScriptFullPath%" /restart 9 x.lnk
else if FileExist("F9\x.url")
Run,"%A_ScriptFullPath%" /restart 9 x.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & y Up::
if FileExist("F9\y.lnk")
Run,"%A_ScriptFullPath%" /restart 9 y.lnk
else if FileExist("F9\y.url")
Run,"%A_ScriptFullPath%" /restart 9 y.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & z Up::
if FileExist("F9\z.lnk")
Run,"%A_ScriptFullPath%" /restart 9 z.lnk
else if FileExist("F9\z.url")
Run,"%A_ScriptFullPath%" /restart 9 z.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & 1 Up::
if FileExist("F9\1.lnk")
Run,"%A_ScriptFullPath%" /restart 9 1.lnk
else if FileExist("F9\1.url")
Run,"%A_ScriptFullPath%" /restart 9 1.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & 2 Up::
if FileExist("F9\2.lnk")
Run,"%A_ScriptFullPath%" /restart 9 2.lnk
else if FileExist("F9\2.url")
Run,"%A_ScriptFullPath%" /restart 9 2.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & 3 Up::
if FileExist("F9\3.lnk")
Run,"%A_ScriptFullPath%" /restart 9 3.lnk
else if FileExist("F9\3.url")
Run,"%A_ScriptFullPath%" /restart 9 3.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & 4 Up::
if FileExist("F9\4.lnk")
Run,"%A_ScriptFullPath%" /restart 9 4.lnk
else if FileExist("F9\4.url")
Run,"%A_ScriptFullPath%" /restart 9 4.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & 5 Up::
if FileExist("F9\5.lnk")
Run,"%A_ScriptFullPath%" /restart 9 5.lnk
else if FileExist("F9\5.url")
Run,"%A_ScriptFullPath%" /restart 9 5.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & 6 Up::
if FileExist("F9\6.lnk")
Run,"%A_ScriptFullPath%" /restart 9 6.lnk
else if FileExist("F9\6.url")
Run,"%A_ScriptFullPath%" /restart 9 6.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & 7 Up::
if FileExist("F9\7.lnk")
Run,"%A_ScriptFullPath%" /restart 9 7.lnk
else if FileExist("F9\7.url")
Run,"%A_ScriptFullPath%" /restart 9 7.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & 8 Up::
if FileExist("F9\8.lnk")
Run,"%A_ScriptFullPath%" /restart 9 8.lnk
else if FileExist("F9\8.url")
Run,"%A_ScriptFullPath%" /restart 9 8.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & 9 Up::
if FileExist("F9\9.lnk")
Run,"%A_ScriptFullPath%" /restart 9 9.lnk
else if FileExist("F9\9.url")
Run,"%A_ScriptFullPath%" /restart 9 9.url
else
{
FileCreateDir,F9
Run,F9
}
Return

F9 & 0 Up::
if FileExist("F9\0.lnk")
Run,"%A_ScriptFullPath%" /restart 9 0.lnk
else if FileExist("F9\0.url")
Run,"%A_ScriptFullPath%" /restart 9 0.url
else
{
FileCreateDir,F9
Run,F9
}
Return


F10 & Enter Up::
FileCreateDir,F10
Run,F10
Return

F10 & a Up::
if FileExist("F10\a.lnk")
Run,"%A_ScriptFullPath%" /restart 10 a.lnk
else if FileExist("F10\a.url")
Run,"%A_ScriptFullPath%" /restart 10 a.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & b Up::
if FileExist("F10\b.lnk")
Run,"%A_ScriptFullPath%" /restart 10 b.lnk
else if FileExist("F10\b.url")
Run,"%A_ScriptFullPath%" /restart 10 b.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & c Up::
if FileExist("F10\c.lnk")
Run,"%A_ScriptFullPath%" /restart 10 c.lnk
else if FileExist("F10\c.url")
Run,"%A_ScriptFullPath%" /restart 10 c.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & d Up::
if FileExist("F10\d.lnk")
Run,"%A_ScriptFullPath%" /restart 10 d.lnk
else if FileExist("F10\d.url")
Run,"%A_ScriptFullPath%" /restart 10 d.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & e Up::
if FileExist("F10\e.lnk")
Run,"%A_ScriptFullPath%" /restart 10 e.lnk
else if FileExist("F10\e.url")
Run,"%A_ScriptFullPath%" /restart 10 e.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & f Up::
if FileExist("F10\f.lnk")
Run,"%A_ScriptFullPath%" /restart 10 f.lnk
else if FileExist("F10\f.url")
Run,"%A_ScriptFullPath%" /restart 10 f.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & g Up::
if FileExist("F10\g.lnk")
Run,"%A_ScriptFullPath%" /restart 10 g.lnk
else if FileExist("F10\g.url")
Run,"%A_ScriptFullPath%" /restart 10 g.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & h Up::
if FileExist("F10\h.lnk")
Run,"%A_ScriptFullPath%" /restart 10 h.lnk
else if FileExist("F10\h.url")
Run,"%A_ScriptFullPath%" /restart 10 h.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & i Up::
if FileExist("F10\i.lnk")
Run,"%A_ScriptFullPath%" /restart 10 i.lnk
else if FileExist("F10\i.url")
Run,"%A_ScriptFullPath%" /restart 10 i.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & j Up::
if FileExist("F10\j.lnk")
Run,"%A_ScriptFullPath%" /restart 10 j.lnk
else if FileExist("F10\j.url")
Run,"%A_ScriptFullPath%" /restart 10 j.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & k Up::
if FileExist("F10\k.lnk")
Run,"%A_ScriptFullPath%" /restart 10 k.lnk
else if FileExist("F10\k.url")
Run,"%A_ScriptFullPath%" /restart 10 k.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & l Up::
if FileExist("F10\l.lnk")
Run,"%A_ScriptFullPath%" /restart 10 l.lnk
else if FileExist("F10\l.url")
Run,"%A_ScriptFullPath%" /restart 10 l.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & m Up::
if FileExist("F10\m.lnk")
Run,"%A_ScriptFullPath%" /restart 10 m.lnk
else if FileExist("F10\m.url")
Run,"%A_ScriptFullPath%" /restart 10 m.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & n Up::
if FileExist("F10\n.lnk")
Run,"%A_ScriptFullPath%" /restart 10 n.lnk
else if FileExist("F10\n.url")
Run,"%A_ScriptFullPath%" /restart 10 n.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & o Up::
if FileExist("F10\o.lnk")
Run,"%A_ScriptFullPath%" /restart 10 o.lnk
else if FileExist("F10\o.url")
Run,"%A_ScriptFullPath%" /restart 10 o.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & p Up::
if FileExist("F10\p.lnk")
Run,"%A_ScriptFullPath%" /restart 10 p.lnk
else if FileExist("F10\p.url")
Run,"%A_ScriptFullPath%" /restart 10 p.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & q Up::
if FileExist("F10\q.lnk")
Run,"%A_ScriptFullPath%" /restart 10 q.lnk
else if FileExist("F10\q.url")
Run,"%A_ScriptFullPath%" /restart 10 q.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & r Up::
if FileExist("F10\r.lnk")
Run,"%A_ScriptFullPath%" /restart 10 r.lnk
else if FileExist("F10\r.url")
Run,"%A_ScriptFullPath%" /restart 10 r.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & s Up::
if FileExist("F10\s.lnk")
Run,"%A_ScriptFullPath%" /restart 10 s.lnk
else if FileExist("F10\s.url")
Run,"%A_ScriptFullPath%" /restart 10 s.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & t Up::
if FileExist("F10\t.lnk")
Run,"%A_ScriptFullPath%" /restart 10 t.lnk
else if FileExist("F10\t.url")
Run,"%A_ScriptFullPath%" /restart 10 t.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & u Up::
if FileExist("F10\u.lnk")
Run,"%A_ScriptFullPath%" /restart 10 u.lnk
else if FileExist("F10\u.url")
Run,"%A_ScriptFullPath%" /restart 10 u.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & v Up::
if FileExist("F10\v.lnk")
Run,"%A_ScriptFullPath%" /restart 10 v.lnk
else if FileExist("F10\v.url")
Run,"%A_ScriptFullPath%" /restart 10 v.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & w Up::
if FileExist("F10\w.lnk")
Run,"%A_ScriptFullPath%" /restart 10 w.lnk
else if FileExist("F10\w.url")
Run,"%A_ScriptFullPath%" /restart 10 w.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & x Up::
if FileExist("F10\x.lnk")
Run,"%A_ScriptFullPath%" /restart 10 x.lnk
else if FileExist("F10\x.url")
Run,"%A_ScriptFullPath%" /restart 10 x.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & y Up::
if FileExist("F10\y.lnk")
Run,"%A_ScriptFullPath%" /restart 10 y.lnk
else if FileExist("F10\y.url")
Run,"%A_ScriptFullPath%" /restart 10 y.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & z Up::
if FileExist("F10\z.lnk")
Run,"%A_ScriptFullPath%" /restart 10 z.lnk
else if FileExist("F10\z.url")
Run,"%A_ScriptFullPath%" /restart 10 z.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & 1 Up::
if FileExist("F10\1.lnk")
Run,"%A_ScriptFullPath%" /restart 10 1.lnk
else if FileExist("F10\1.url")
Run,"%A_ScriptFullPath%" /restart 10 1.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & 2 Up::
if FileExist("F10\2.lnk")
Run,"%A_ScriptFullPath%" /restart 10 2.lnk
else if FileExist("F10\2.url")
Run,"%A_ScriptFullPath%" /restart 10 2.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & 3 Up::
if FileExist("F10\3.lnk")
Run,"%A_ScriptFullPath%" /restart 10 3.lnk
else if FileExist("F10\3.url")
Run,"%A_ScriptFullPath%" /restart 10 3.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & 4 Up::
if FileExist("F10\4.lnk")
Run,"%A_ScriptFullPath%" /restart 10 4.lnk
else if FileExist("F10\4.url")
Run,"%A_ScriptFullPath%" /restart 10 4.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & 5 Up::
if FileExist("F10\5.lnk")
Run,"%A_ScriptFullPath%" /restart 10 5.lnk
else if FileExist("F10\5.url")
Run,"%A_ScriptFullPath%" /restart 10 5.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & 6 Up::
if FileExist("F10\6.lnk")
Run,"%A_ScriptFullPath%" /restart 10 6.lnk
else if FileExist("F10\6.url")
Run,"%A_ScriptFullPath%" /restart 10 6.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & 7 Up::
if FileExist("F10\7.lnk")
Run,"%A_ScriptFullPath%" /restart 10 7.lnk
else if FileExist("F10\7.url")
Run,"%A_ScriptFullPath%" /restart 10 7.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & 8 Up::
if FileExist("F10\8.lnk")
Run,"%A_ScriptFullPath%" /restart 10 8.lnk
else if FileExist("F10\8.url")
Run,"%A_ScriptFullPath%" /restart 10 8.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & 9 Up::
if FileExist("F10\9.lnk")
Run,"%A_ScriptFullPath%" /restart 10 9.lnk
else if FileExist("F10\9.url")
Run,"%A_ScriptFullPath%" /restart 10 9.url
else
{
FileCreateDir,F10
Run,F10
}
Return

F10 & 0 Up::
if FileExist("F10\0.lnk")
Run,"%A_ScriptFullPath%" /restart 10 0.lnk
else if FileExist("F10\0.url")
Run,"%A_ScriptFullPath%" /restart 10 0.url
else
{
FileCreateDir,F10
Run,F10
}
Return


F11 & Enter Up::
FileCreateDir,F11
Run,F11
Return

F11 & a Up::
if FileExist("F11\a.lnk")
Run,"%A_ScriptFullPath%" /restart 11 a.lnk
else if FileExist("F11\a.url")
Run,"%A_ScriptFullPath%" /restart 11 a.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & b Up::
if FileExist("F11\b.lnk")
Run,"%A_ScriptFullPath%" /restart 11 b.lnk
else if FileExist("F11\b.url")
Run,"%A_ScriptFullPath%" /restart 11 b.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & c Up::
if FileExist("F11\c.lnk")
Run,"%A_ScriptFullPath%" /restart 11 c.lnk
else if FileExist("F11\c.url")
Run,"%A_ScriptFullPath%" /restart 11 c.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & d Up::
if FileExist("F11\d.lnk")
Run,"%A_ScriptFullPath%" /restart 11 d.lnk
else if FileExist("F11\d.url")
Run,"%A_ScriptFullPath%" /restart 11 d.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & e Up::
if FileExist("F11\e.lnk")
Run,"%A_ScriptFullPath%" /restart 11 e.lnk
else if FileExist("F11\e.url")
Run,"%A_ScriptFullPath%" /restart 11 e.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & f Up::
if FileExist("F11\f.lnk")
Run,"%A_ScriptFullPath%" /restart 11 f.lnk
else if FileExist("F11\f.url")
Run,"%A_ScriptFullPath%" /restart 11 f.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & g Up::
if FileExist("F11\g.lnk")
Run,"%A_ScriptFullPath%" /restart 11 g.lnk
else if FileExist("F11\g.url")
Run,"%A_ScriptFullPath%" /restart 11 g.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & h Up::
if FileExist("F11\h.lnk")
Run,"%A_ScriptFullPath%" /restart 11 h.lnk
else if FileExist("F11\h.url")
Run,"%A_ScriptFullPath%" /restart 11 h.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & i Up::
if FileExist("F11\i.lnk")
Run,"%A_ScriptFullPath%" /restart 11 i.lnk
else if FileExist("F11\i.url")
Run,"%A_ScriptFullPath%" /restart 11 i.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & j Up::
if FileExist("F11\j.lnk")
Run,"%A_ScriptFullPath%" /restart 11 j.lnk
else if FileExist("F11\j.url")
Run,"%A_ScriptFullPath%" /restart 11 j.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & k Up::
if FileExist("F11\k.lnk")
Run,"%A_ScriptFullPath%" /restart 11 k.lnk
else if FileExist("F11\k.url")
Run,"%A_ScriptFullPath%" /restart 11 k.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & l Up::
if FileExist("F11\l.lnk")
Run,"%A_ScriptFullPath%" /restart 11 l.lnk
else if FileExist("F11\l.url")
Run,"%A_ScriptFullPath%" /restart 11 l.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & m Up::
if FileExist("F11\m.lnk")
Run,"%A_ScriptFullPath%" /restart 11 m.lnk
else if FileExist("F11\m.url")
Run,"%A_ScriptFullPath%" /restart 11 m.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & n Up::
if FileExist("F11\n.lnk")
Run,"%A_ScriptFullPath%" /restart 11 n.lnk
else if FileExist("F11\n.url")
Run,"%A_ScriptFullPath%" /restart 11 n.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & o Up::
if FileExist("F11\o.lnk")
Run,"%A_ScriptFullPath%" /restart 11 o.lnk
else if FileExist("F11\o.url")
Run,"%A_ScriptFullPath%" /restart 11 o.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & p Up::
if FileExist("F11\p.lnk")
Run,"%A_ScriptFullPath%" /restart 11 p.lnk
else if FileExist("F11\p.url")
Run,"%A_ScriptFullPath%" /restart 11 p.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & q Up::
if FileExist("F11\q.lnk")
Run,"%A_ScriptFullPath%" /restart 11 q.lnk
else if FileExist("F11\q.url")
Run,"%A_ScriptFullPath%" /restart 11 q.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & r Up::
if FileExist("F11\r.lnk")
Run,"%A_ScriptFullPath%" /restart 11 r.lnk
else if FileExist("F11\r.url")
Run,"%A_ScriptFullPath%" /restart 11 r.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & s Up::
if FileExist("F11\s.lnk")
Run,"%A_ScriptFullPath%" /restart 11 s.lnk
else if FileExist("F11\s.url")
Run,"%A_ScriptFullPath%" /restart 11 s.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & t Up::
if FileExist("F11\t.lnk")
Run,"%A_ScriptFullPath%" /restart 11 t.lnk
else if FileExist("F11\t.url")
Run,"%A_ScriptFullPath%" /restart 11 t.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & u Up::
if FileExist("F11\u.lnk")
Run,"%A_ScriptFullPath%" /restart 11 u.lnk
else if FileExist("F11\u.url")
Run,"%A_ScriptFullPath%" /restart 11 u.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & v Up::
if FileExist("F11\v.lnk")
Run,"%A_ScriptFullPath%" /restart 11 v.lnk
else if FileExist("F11\v.url")
Run,"%A_ScriptFullPath%" /restart 11 v.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & w Up::
if FileExist("F11\w.lnk")
Run,"%A_ScriptFullPath%" /restart 11 w.lnk
else if FileExist("F11\w.url")
Run,"%A_ScriptFullPath%" /restart 11 w.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & x Up::
if FileExist("F11\x.lnk")
Run,"%A_ScriptFullPath%" /restart 11 x.lnk
else if FileExist("F11\x.url")
Run,"%A_ScriptFullPath%" /restart 11 x.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & y Up::
if FileExist("F11\y.lnk")
Run,"%A_ScriptFullPath%" /restart 11 y.lnk
else if FileExist("F11\y.url")
Run,"%A_ScriptFullPath%" /restart 11 y.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & z Up::
if FileExist("F11\z.lnk")
Run,"%A_ScriptFullPath%" /restart 11 z.lnk
else if FileExist("F11\z.url")
Run,"%A_ScriptFullPath%" /restart 11 z.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & 1 Up::
if FileExist("F11\1.lnk")
Run,"%A_ScriptFullPath%" /restart 11 1.lnk
else if FileExist("F11\1.url")
Run,"%A_ScriptFullPath%" /restart 11 1.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & 2 Up::
if FileExist("F11\2.lnk")
Run,"%A_ScriptFullPath%" /restart 11 2.lnk
else if FileExist("F11\2.url")
Run,"%A_ScriptFullPath%" /restart 11 2.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & 3 Up::
if FileExist("F11\3.lnk")
Run,"%A_ScriptFullPath%" /restart 11 3.lnk
else if FileExist("F11\3.url")
Run,"%A_ScriptFullPath%" /restart 11 3.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & 4 Up::
if FileExist("F11\4.lnk")
Run,"%A_ScriptFullPath%" /restart 11 4.lnk
else if FileExist("F11\4.url")
Run,"%A_ScriptFullPath%" /restart 11 4.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & 5 Up::
if FileExist("F11\5.lnk")
Run,"%A_ScriptFullPath%" /restart 11 5.lnk
else if FileExist("F11\5.url")
Run,"%A_ScriptFullPath%" /restart 11 5.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & 6 Up::
if FileExist("F11\6.lnk")
Run,"%A_ScriptFullPath%" /restart 11 6.lnk
else if FileExist("F11\6.url")
Run,"%A_ScriptFullPath%" /restart 11 6.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & 7 Up::
if FileExist("F11\7.lnk")
Run,"%A_ScriptFullPath%" /restart 11 7.lnk
else if FileExist("F11\7.url")
Run,"%A_ScriptFullPath%" /restart 11 7.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & 8 Up::
if FileExist("F11\8.lnk")
Run,"%A_ScriptFullPath%" /restart 11 8.lnk
else if FileExist("F11\8.url")
Run,"%A_ScriptFullPath%" /restart 11 8.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & 9 Up::
if FileExist("F11\9.lnk")
Run,"%A_ScriptFullPath%" /restart 11 9.lnk
else if FileExist("F11\9.url")
Run,"%A_ScriptFullPath%" /restart 11 9.url
else
{
FileCreateDir,F11
Run,F11
}
Return

F11 & 0 Up::
if FileExist("F11\0.lnk")
Run,"%A_ScriptFullPath%" /restart 11 0.lnk
else if FileExist("F11\0.url")
Run,"%A_ScriptFullPath%" /restart 11 0.url
else
{
FileCreateDir,F11
Run,F11
}
Return


F12 & Enter Up::
FileCreateDir,F12
Run,F12
Return

F12 & a Up::
if FileExist("F12\a.lnk")
Run,"%A_ScriptFullPath%" /restart 12 a.lnk
else if FileExist("F12\a.url")
Run,"%A_ScriptFullPath%" /restart 12 a.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & b Up::
if FileExist("F12\b.lnk")
Run,"%A_ScriptFullPath%" /restart 12 b.lnk
else if FileExist("F12\b.url")
Run,"%A_ScriptFullPath%" /restart 12 b.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & c Up::
if FileExist("F12\c.lnk")
Run,"%A_ScriptFullPath%" /restart 12 c.lnk
else if FileExist("F12\c.url")
Run,"%A_ScriptFullPath%" /restart 12 c.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & d Up::
if FileExist("F12\d.lnk")
Run,"%A_ScriptFullPath%" /restart 12 d.lnk
else if FileExist("F12\d.url")
Run,"%A_ScriptFullPath%" /restart 12 d.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & e Up::
if FileExist("F12\e.lnk")
Run,"%A_ScriptFullPath%" /restart 12 e.lnk
else if FileExist("F12\e.url")
Run,"%A_ScriptFullPath%" /restart 12 e.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & f Up::
if FileExist("F12\f.lnk")
Run,"%A_ScriptFullPath%" /restart 12 f.lnk
else if FileExist("F12\f.url")
Run,"%A_ScriptFullPath%" /restart 12 f.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & g Up::
if FileExist("F12\g.lnk")
Run,"%A_ScriptFullPath%" /restart 12 g.lnk
else if FileExist("F12\g.url")
Run,"%A_ScriptFullPath%" /restart 12 g.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & h Up::
if FileExist("F12\h.lnk")
Run,"%A_ScriptFullPath%" /restart 12 h.lnk
else if FileExist("F12\h.url")
Run,"%A_ScriptFullPath%" /restart 12 h.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & i Up::
if FileExist("F12\i.lnk")
Run,"%A_ScriptFullPath%" /restart 12 i.lnk
else if FileExist("F12\i.url")
Run,"%A_ScriptFullPath%" /restart 12 i.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & j Up::
if FileExist("F12\j.lnk")
Run,"%A_ScriptFullPath%" /restart 12 j.lnk
else if FileExist("F12\j.url")
Run,"%A_ScriptFullPath%" /restart 12 j.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & k Up::
if FileExist("F12\k.lnk")
Run,"%A_ScriptFullPath%" /restart 12 k.lnk
else if FileExist("F12\k.url")
Run,"%A_ScriptFullPath%" /restart 12 k.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & l Up::
if FileExist("F12\l.lnk")
Run,"%A_ScriptFullPath%" /restart 12 l.lnk
else if FileExist("F12\l.url")
Run,"%A_ScriptFullPath%" /restart 12 l.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & m Up::
if FileExist("F12\m.lnk")
Run,"%A_ScriptFullPath%" /restart 12 m.lnk
else if FileExist("F12\m.url")
Run,"%A_ScriptFullPath%" /restart 12 m.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & n Up::
if FileExist("F12\n.lnk")
Run,"%A_ScriptFullPath%" /restart 12 n.lnk
else if FileExist("F12\n.url")
Run,"%A_ScriptFullPath%" /restart 12 n.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & o Up::
if FileExist("F12\o.lnk")
Run,"%A_ScriptFullPath%" /restart 12 o.lnk
else if FileExist("F12\o.url")
Run,"%A_ScriptFullPath%" /restart 12 o.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & p Up::
if FileExist("F12\p.lnk")
Run,"%A_ScriptFullPath%" /restart 12 p.lnk
else if FileExist("F12\p.url")
Run,"%A_ScriptFullPath%" /restart 12 p.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & q Up::
if FileExist("F12\q.lnk")
Run,"%A_ScriptFullPath%" /restart 12 q.lnk
else if FileExist("F12\q.url")
Run,"%A_ScriptFullPath%" /restart 12 q.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & r Up::
if FileExist("F12\r.lnk")
Run,"%A_ScriptFullPath%" /restart 12 r.lnk
else if FileExist("F12\r.url")
Run,"%A_ScriptFullPath%" /restart 12 r.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & s Up::
if FileExist("F12\s.lnk")
Run,"%A_ScriptFullPath%" /restart 12 s.lnk
else if FileExist("F12\s.url")
Run,"%A_ScriptFullPath%" /restart 12 s.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & t Up::
if FileExist("F12\t.lnk")
Run,"%A_ScriptFullPath%" /restart 12 t.lnk
else if FileExist("F12\t.url")
Run,"%A_ScriptFullPath%" /restart 12 t.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & u Up::
if FileExist("F12\u.lnk")
Run,"%A_ScriptFullPath%" /restart 12 u.lnk
else if FileExist("F12\u.url")
Run,"%A_ScriptFullPath%" /restart 12 u.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & v Up::
if FileExist("F12\v.lnk")
Run,"%A_ScriptFullPath%" /restart 12 v.lnk
else if FileExist("F12\v.url")
Run,"%A_ScriptFullPath%" /restart 12 v.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & w Up::
if FileExist("F12\w.lnk")
Run,"%A_ScriptFullPath%" /restart 12 w.lnk
else if FileExist("F12\w.url")
Run,"%A_ScriptFullPath%" /restart 12 w.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & x Up::
if FileExist("F12\x.lnk")
Run,"%A_ScriptFullPath%" /restart 12 x.lnk
else if FileExist("F12\x.url")
Run,"%A_ScriptFullPath%" /restart 12 x.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & y Up::
if FileExist("F12\y.lnk")
Run,"%A_ScriptFullPath%" /restart 12 y.lnk
else if FileExist("F12\y.url")
Run,"%A_ScriptFullPath%" /restart 12 y.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & z Up::
if FileExist("F12\z.lnk")
Run,"%A_ScriptFullPath%" /restart 12 z.lnk
else if FileExist("F12\z.url")
Run,"%A_ScriptFullPath%" /restart 12 z.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & 1 Up::
if FileExist("F12\1.lnk")
Run,"%A_ScriptFullPath%" /restart 12 1.lnk
else if FileExist("F12\1.url")
Run,"%A_ScriptFullPath%" /restart 12 1.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & 2 Up::
if FileExist("F12\2.lnk")
Run,"%A_ScriptFullPath%" /restart 12 2.lnk
else if FileExist("F12\2.url")
Run,"%A_ScriptFullPath%" /restart 12 2.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & 3 Up::
if FileExist("F12\3.lnk")
Run,"%A_ScriptFullPath%" /restart 12 3.lnk
else if FileExist("F12\3.url")
Run,"%A_ScriptFullPath%" /restart 12 3.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & 4 Up::
if FileExist("F12\4.lnk")
Run,"%A_ScriptFullPath%" /restart 12 4.lnk
else if FileExist("F12\4.url")
Run,"%A_ScriptFullPath%" /restart 12 4.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & 5 Up::
if FileExist("F12\5.lnk")
Run,"%A_ScriptFullPath%" /restart 12 5.lnk
else if FileExist("F12\5.url")
Run,"%A_ScriptFullPath%" /restart 12 5.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & 6 Up::
if FileExist("F12\6.lnk")
Run,"%A_ScriptFullPath%" /restart 12 6.lnk
else if FileExist("F12\6.url")
Run,"%A_ScriptFullPath%" /restart 12 6.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & 7 Up::
if FileExist("F12\7.lnk")
Run,"%A_ScriptFullPath%" /restart 12 7.lnk
else if FileExist("F12\7.url")
Run,"%A_ScriptFullPath%" /restart 12 7.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & 8 Up::
if FileExist("F12\8.lnk")
Run,"%A_ScriptFullPath%" /restart 12 8.lnk
else if FileExist("F12\8.url")
Run,"%A_ScriptFullPath%" /restart 12 8.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & 9 Up::
if FileExist("F12\9.lnk")
Run,"%A_ScriptFullPath%" /restart 12 9.lnk
else if FileExist("F12\9.url")
Run,"%A_ScriptFullPath%" /restart 12 9.url
else
{
FileCreateDir,F12
Run,F12
}
Return

F12 & 0 Up::
if FileExist("F12\0.lnk")
Run,"%A_ScriptFullPath%" /restart 12 0.lnk
else if FileExist("F12\0.url")
Run,"%A_ScriptFullPath%" /restart 12 0.url
else
{
FileCreateDir,F12
Run,F12
}
Return
