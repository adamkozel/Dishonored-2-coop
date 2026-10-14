# Opens the VM's console window (Hyper-V VMConnect) on the host desktop, e.g. for the user's
# Steam login. Basic session: keyboard, mouse and display of the console the game runs in.
. (Join-Path $PSScriptRoot 'D2LabVM.Common.ps1')
Start-Process "$env:windir\System32\vmconnect.exe" -ArgumentList 'localhost', $VmName
