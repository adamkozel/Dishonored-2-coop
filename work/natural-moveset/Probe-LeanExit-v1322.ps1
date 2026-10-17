param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='lean-exit-probe-v1322a',[string]$Role='host')
# 1322 (#11): which native controller input leaves the lean (player state 18)? Take 1319a's exit (LeftY -22000 for
# 350 ms) stepped back but kept the lean. Each trial enters a side lean (Y + full side stick, 1300 ms), waits, then
# tries one exit input and reads the native active states. The source is put back on the lab spot after every trial.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function In($a){& "$lab/Send-D2Input.ps1" -Role $Role @a|Out-Null}
$l=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json
function States{$raw=& "$lab/bin/d2_runtime_probe.exe" $l.pid '--player-states';if($LASTEXITCODE){throw 'Native read failed'};$p=$raw|ConvertFrom-Json;[ordered]@{active=@($p.active|ForEach-Object {$_.id});stance=$p.stance;position=$p.position}}
$exits=[ordered]@{
 'release-wait'=@{Button='Neutral';DurationMs=100}
 'y-tap'=@{Button='Y';DurationMs=150}
 'back-350'=@{LeftY=-22000;DurationMs=350}
 'back-1200'=@{LeftY=-22000;DurationMs=1200}
 'forward-800'=@{LeftY=22000;DurationMs=800}
 'turn-600'=@{RightX=22000;DurationMs=600}
 'crouch-tap'=@{Button='B';DurationMs=180}
 'opposite-side'=@{LeftX=32767;DurationMs=800}}
$rows=[Collections.Generic.List[object]]::new()
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($name in $exits.Keys){
  C $Role 'setviewpos -35 -153 68.1 0 0';Start-Sleep -Milliseconds 700
  In @{Button='Y';LeftX=-32767;DurationMs=1300};Start-Sleep -Milliseconds 1700;$lean=States
  $a=$exits[$name];In $a;Start-Sleep -Milliseconds ([int]$a.DurationMs+900);$after=States
  if($name-eq'crouch-tap'-and$after.stance-eq1){In @{Button='B';DurationMs=180};Start-Sleep -Milliseconds 700}
  if((States).active-contains18){C $Role 'setviewpos -33 -153 68.1 90 0';Start-Sleep -Milliseconds 650}  # the known exit (a turned re-placement)
  $rows.Add([ordered]@{exit=$name;input=$a;leaning=($lean.active-contains18);leanStates=$lean.active;afterStates=$after.active;exited=!($after.active-contains18);moved=[Math]::Round([Math]::Sqrt(([double]$after.position[0]-[double]$lean.position[0])*([double]$after.position[0]-[double]$lean.position[0])+([double]$after.position[1]-[double]$lean.position[1])*([double]$after.position[1]-[double]$lean.position[1])),2)})}
}finally{
 try{C $Role 'setviewpos -35 -153 68.1 0 0';Start-Sleep -Milliseconds 500;if((States).active-contains18){C $Role 'setviewpos -33 -153 68.1 90 0'}}catch{}
 $rows|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
 if($owned){$mutex.ReleaseMutex()}}
$rows|ForEach-Object {"$($_.exit): leaning $($_.leaning) -> exited $($_.exited) moved $($_.moved) states $($_.afterStates -join ',')"}
