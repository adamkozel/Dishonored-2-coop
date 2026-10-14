$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent
Set-Location $root
function C($r,$s){& outputs/d2-coop-lab/Send-D2Command.ps1 -Role $r -Command $s -WaitConsumed|Out-Null}
function S($r){for($attempt=0;$attempt-lt40;$attempt++){try{return(Get-Content "work/lab-instances/$r/coop-status.json" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{if($attempt-eq39){throw};Start-Sleep -Milliseconds 25}}}
& outputs/d2-coop-lab/Restore-D2Lab.ps1 -Doppelganger|Out-Null
foreach($r in 'host','client'){C $r 'setviewpos -39.4 -161.4 68.1 270 20';& outputs/d2-coop-lab/Send-D2Input.ps1 -Role $r -Button X -DurationMs 400|Out-Null}
$seen=@{host=$false;client=$false};$watch=[Diagnostics.Stopwatch]::StartNew()
do{$ready=$true;foreach($r in 'host','client'){$s=S $r;if($s.localGameLogicState-eq5){$seen[$r]=$true};if(!$seen[$r]-or$s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 150}}while(!$ready-and$watch.ElapsedMilliseconds-lt45000)
if(!$ready){throw 'Both native Mayhew scenes did not complete'}
C host 'setviewpos -33 -153 68.1 90 0';C client 'setviewpos -33 -148 68.1 270 0'
foreach($r in 'host','client'){C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on'}
'Native sword fixture ready'
