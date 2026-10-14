param([string]$Take='block-window-v1016',[ValidateSet('host','client')][string]$Role='client',[string]$Press='150,100,400')
$PressMs=@($Press.Split(',')|ForEach-Object{[int]$_})
# Read-only native parry-window timing: ordinary RightShoulder presses of the
# given lengths while --watch-block-state samples arkPlayerStateBlock every 5 ms.
# No enemies, no co-op relay, no memory writes.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$probe="$root/work/movement-integration/probe-block-v1016/d2_runtime_probe.exe"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $l=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
 if($p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
 $total=1000+($PressMs|Measure-Object -Sum).Sum+$PressMs.Count*900
 $watch=Start-Process $probe -ArgumentList @($l.pid,'--watch-block-state',$total) -RedirectStandardOutput "$out/watch.json" -WindowStyle Hidden -PassThru
 Start-Sleep -Milliseconds 400;$marks=@();$sw=[Diagnostics.Stopwatch]::StartNew()
 foreach($ms in $PressMs){$marks+=@{pressAtMs=$sw.ElapsedMilliseconds+400;durationMs=$ms};& "$lab/Send-D2Input.ps1" -Role $Role -Button RightShoulder -DurationMs $ms|Out-Null;Start-Sleep -Milliseconds (900)}
 if(!$watch.WaitForExit($total+5000)){throw 'Watch timeout'}
 @{identity=@{probe=(Get-FileHash $probe).Hash;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;role=$Role};presses=$marks}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}finally{if($owned){& "$lab/Send-D2Input.ps1" -Role $Role -DurationMs 100|Out-Null;$mutex.ReleaseMutex()};$mutex.Dispose()}
