$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../..'))
$launch=Get-Content "$root/work/lab-instances/client/launch.json" -Raw|ConvertFrom-Json
$p=Get-Process -Id $launch.pid
if($p.Path -ne "$root/work/asset-game/Dishonored2.exe".Replace('/','\') -or $p.Id -ne 42208){throw 'Stalled process identity differs'}
Copy-Item "$root/work/lab-instances/client/coop-status.json","$root/work/lab-instances/client/launch.json","$root/work/lab-instances/client/engine.log" -Destination $PSScriptRoot
Add-Type -TypeDefinition @'
using System;
using System.Runtime.InteropServices;
public static class LabDiagnosticDump {
 [DllImport("dbghelp.dll",SetLastError=true)] public static extern bool MiniDumpWriteDump(IntPtr process,uint pid,IntPtr file,uint type,IntPtr exception,IntPtr stream,IntPtr callback);
}
'@
$out=[IO.File]::Create((Join-Path $PSScriptRoot 'client-stall-minidump.dmp'))
try { if(![LabDiagnosticDump]::MiniDumpWriteDump($p.Handle,[uint32]$p.Id,$out.SafeFileHandle.DangerousGetHandle(),0,[IntPtr]::Zero,[IntPtr]::Zero,[IntPtr]::Zero)){throw "Minidump failed $([Runtime.InteropServices.Marshal]::GetLastWin32Error())"} } finally {$out.Dispose()}
Get-Item (Join-Path $PSScriptRoot 'client-stall-minidump.dmp')|Select-Object Name,Length
