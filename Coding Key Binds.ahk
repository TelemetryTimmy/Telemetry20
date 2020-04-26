#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

F13::
Send, /**{Enter}
return 
F14::
Send, //{!}{<}
return

F15::
Send, /*  */{Left}{Left}{Left}
return

F16::F16

F17::F17
