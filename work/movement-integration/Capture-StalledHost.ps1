$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$launch=Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json
$process=Get-Process -Id $launch.pid
if($process.Id -ne 27964 -or $process.Path -ne (Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Stalled host identity changed'}
foreach($name in 'launch.json','coop-status.json','engine.log') {Copy-Item -LiteralPath "$root/work/lab-instances/host/$name" -Destination "$PSScriptRoot/host-stall-$name"}
Add-Type -TypeDefinition @'
using System;
using System.Runtime.InteropServices;
public static class MovementDiagnosticDump {
 [DllImport("dbghelp.dll",SetLastError=true)] public static extern bool MiniDumpWriteDump(IntPtr process,uint pid,IntPtr file,uint type,IntPtr exception,IntPtr stream,IntPtr callback);
}
'@
$out=[IO.File]::Create((Join-Path $PSScriptRoot 'host-stall-minidump.dmp'))
try {if(![MovementDiagnosticDump]::MiniDumpWriteDump($process.Handle,[uint32]$process.Id,$out.SafeFileHandle.DangerousGetHandle(),0,[IntPtr]::Zero,[IntPtr]::Zero,[IntPtr]::Zero)){throw "Dump failed $([Runtime.InteropServices.Marshal]::GetLastWin32Error())"}}finally{$out.Dispose()}
Get-Item (Join-Path $PSScriptRoot 'host-stall-minidump.dmp')|Select-Object Name,Length
