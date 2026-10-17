param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='course-probe-v1321a',[string]$Role='host',[int]$SprintMs=3000,[string]$StartList='',[switch]$Run)
# 1321 (#11): finds an open straight run near the Dunwall lab spot for running and sprinting jumps. For each candidate
# start (x, y, yaw; -StartList 'x,y,yaw;...' overrides the list) the source is placed, sprints (or runs with -Run) straight for SprintMs and its travel is read back; the source is put
# back on the lab spot at the end. Movement only: no product controller, no inventory or health change.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
$l=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json
function Pos{$raw=& "$lab/bin/d2_runtime_probe.exe" $l.pid '--player-states';if($LASTEXITCODE){throw 'Native read failed'};($raw|ConvertFrom-Json).position}
$starts=if($StartList){@($StartList-split';'|ForEach-Object {,@($_-split','|ForEach-Object {[double]$_})})}else{@(@(-29.6,-153,180),@(-35,-153,270),@(-35,-153,90),@(-35,-153,180),@(-37.5,-179,0),@(-37.5,-179,90),@(-37.5,-179,270),@(-45.3,-179,180),@(-45.3,-179,90),@(-45.3,-179,270))}
$rows=[Collections.Generic.List[object]]::new()
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Another native operation remains active'}
 if(Test-Path -LiteralPath $out){throw 'Unique evidence take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($s in $starts){$z=if($s[1]-lt-170){if($s[0]-lt-40){65.1}else{68.1}}else{68.1}
  C $Role ('setviewpos {0} {1} {2} {3} 0' -f $s[0],$s[1],$z,$s[2]);Start-Sleep -Milliseconds 700;$a=Pos
  & "$lab/Send-D2Input.ps1" -Role $Role -Button $(if($Run){'Neutral'}else{'LeftThumb'}) -DurationMs $SprintMs -LeftY 32767|Out-Null;Start-Sleep -Milliseconds ($SprintMs+900);$b=Pos
  $d=[Math]::Sqrt(($b[0]-$a[0])*($b[0]-$a[0])+($b[1]-$a[1])*($b[1]-$a[1]))
  $rows.Add([ordered]@{start=$s;from=$a;to=$b;travel=[Math]::Round($d,2);climb=[Math]::Round($b[2]-$a[2],2)})}
}finally{
 try{C $Role 'setviewpos -33 -153 68.1 90 0'}catch{}
 $rows|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
 if($owned){$mutex.ReleaseMutex()}}
$rows|ForEach-Object {"$($_.start -join ',') travel $($_.travel) climb $($_.climb)"}
