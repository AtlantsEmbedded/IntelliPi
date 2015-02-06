'VBScript Example
Set WshShell = WScript.CreateObject("WScript.Shell")

With WshShell

    ' Save the original window position. If system-positioned, this key will not exist.
    On Error Resume Next
    intWindowPos = .RegRead("HKCU\Console\WindowPosition")
    On Error GoTo 0

    ' Set Y coordinate to something crazy...
    .RegWrite "HKCU\Console\WindowPosition", &H1000, "REG_DWORD"

    ' Start muse player
	WshShell.Run "launch_muse_player.bat"
	WshShell.AppActivate "muse-player"

    ' Wait to record the trial
	WScript.Sleep 6000
	WshShell.SendKeys "^C"

    ' Wait to record to finish properly
	WScript.Sleep 1000
	WshShell.SendKeys "y"
	WshShell.SendKeys "{ENTER}"

    ' Restore window position, if previously set. Otherwise, remove key...
    If Len(intWindowPos) > 0 Then
        .RegWrite "HKCU\Console\WindowPosition", intWindowPos, "REG_DWORD"
    Else
        .RegDelete "HKCU\Console\WindowPosition"
    End If

End With


