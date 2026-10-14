param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='movement-resume-v1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$handles=@{};$checks=[Collections.Generic.List[string]]::new();$rows=[Collections.Generic.List[object]]::new();$failure=$null;$paused=$null
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100,$y=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftY $y|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Probe $r $mode"}
function Snapshot($label){foreach($r in 'host','client'){$rows.Add([pscustomobject]@{label=$label;role=$r;bridge=(S $r);bank=(S $r '.movement.json');resume=(S $r '.movement-resume.json');stream=(S $r '.shadow-stream.json');equipment=(S $r '.shadow-equipment.json')})}}
function AwaitLive($resumed=$false,$old=@{}){
 $watch=[Diagnostics.Stopwatch]::StartNew()
 do{
  $ok=$true
  foreach($r in 'host','client'){$m=S $r '.movement.json';$s=S $r '.shadow-stream.json';$a=S $r '.movement-resume.json';if(!$m.active-or$m.version-ne20-or$m.weight-lt.9-or!$s.ready){$ok=$false};if($resumed-and($a.phase-ne1-or$a.resumed-le$old[$r].resumed)){$ok=$false}}
  if($ok){return};Start-Sleep -Milliseconds 80
 }while($watch.ElapsedMilliseconds-lt14000)
 Snapshot 'readiness-failure';throw 'Movement readiness deadline'
}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
  $pids[$r]=$l.pid;$handles[$r]=(S $r).probeActorHandle;I $r
  foreach($cmd in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_probe_follow exact','coop_probe_locomotion on','coop_movement_feet authored','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}
  if(!(Q $r '--player-sword').equipped-or(Q $r '--health' 1).current-ne56){throw 'Clean armed fixture required'}
  if((S $r).localPlayerStance-eq1){I $r B 180;Start-Sleep -Milliseconds 800}
  $role=if($r-eq'host'){'emily'}else{'corvo'};Copy-Item "$PSScriptRoot/high-mantle-banks-v1/$role-high-mantle-bank.bin" "$root/work/lab-instances/$r/coop-status.json.movement.bin"
  C $r 'setviewpos -34 -153 68.1 0 0'
 }
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 650
 foreach($r in 'host','client'){C $r 'coop_movement_bank on'};AwaitLive
 foreach($r in 'host','client'){C $r 'coop_movement_resume on';if(!(S $r '.movement-resume.json').enabled){Start-Sleep -Milliseconds 350;if(!(S $r '.movement-resume.json').enabled){throw 'Resume opt-in refused'}}}
 Snapshot 'initial'
 foreach($src in 'host','client'){
  $dst=if($src-eq'host'){'client'}else{'host'}
  foreach($crouched in $false,$true){
   foreach($r in 'host','client'){C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'}
   if($crouched){I $src B 180;Start-Sleep -Milliseconds 1000;I $src X 1500;Start-Sleep -Milliseconds 3000}
   $old=@{};foreach($r in 'host','client'){$old[$r]=S $r '.movement-resume.json'}
   $label="$src-crouched-$crouched";Snapshot "$label-before"
   I $src Start 180;$paused=$src;Start-Sleep -Milliseconds 1000;Snapshot "$label-paused"
   foreach($r in 'host','client'){if((S $r '.movement.json').active-or(S $r '.shadow-stream.json').armed-or(S $r '.movement-resume.json').phase-ne2){throw 'Native menu did not suspend both presentations'}}
   $checks.Add("${label}: native pause suspends both banks and action streams")
   I $src A 180;$paused=$null;AwaitLive $true $old;Start-Sleep -Milliseconds 600;Snapshot "$label-resumed"
   foreach($r in 'host','client'){
    $a=S $r '.movement-resume.json';$s=S $r '.shadow-stream.json';$m=S $r '.movement.json'
    if($a.checksum-ne$old[$r].checksum-or$s.token-eq$a.priorToken-or$s.peerToken-eq$a.priorPeerToken-or!$m.nativeFeet-or!$m.currentFeet){throw 'Resume lost bank/config identity or reused stream tokens'}
    if((S $r).probeActorHandle-ne$handles[$r]){throw 'Actor changed during same-world resume'}
   }
   $checks.Add("${label}: same actors, pinned banks and authored-feet settings restored with fresh Steam tokens")
   $sword=Q $dst '--avatar-sword' $handles[$dst]
   if([bool]$sword.equipped-eq$crouched-or[bool](Q $src '--player-sword').equipped-eq$crouched){throw 'Native equipment state not recovered'}
   if((S $src).localPlayerStance-ne[int]$crouched-or($crouched-and(S $dst '.movement.json').stanceMix-lt.9)){throw 'Stance did not survive pause'}
   $checks.Add("${label}: native stance and drawn/holstered sword preserved")
   I $src Neutral 900 12000;Start-Sleep -Milliseconds 450
   if((S $dst '.movement.json').speed-lt.2-or!(S $dst '.movement.json').active){throw 'Resumed movement did not advance'}
   I $src;Start-Sleep -Milliseconds 1000
   $checks.Add("${label}: ordinary controller movement resumes remote gait")
   if($crouched){I $src X 650;Start-Sleep -Milliseconds 1600;I $src B 180;Start-Sleep -Milliseconds 1000}
   C $src 'setviewpos -34 -153 68.1 0 0';Start-Sleep -Milliseconds 900
  }
 }
 foreach($r in 'host','client'){C $r 'coop_movement_resume off'}
 I host Start 180;$paused='host';Start-Sleep -Milliseconds 800;I host A 180;$paused=$null;Start-Sleep -Milliseconds 1800;Snapshot 'explicit-off-after-resume'
 foreach($r in 'host','client'){if((S $r '.movement.json').active-or(S $r '.movement-resume.json').enabled){throw 'Explicit off resurrected movement'};if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}}
 $checks.Add('Explicit off prevents automatic rearm; player and clone health preserved')
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
 $checks.Add('Original game files and protected saves unchanged')
}catch{$failure=$_.Exception.Message}finally{
 if($paused){try{I $paused A 180;Start-Sleep -Milliseconds 800}catch{}}
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_resume off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if(!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 1500};if((S $r).localPlayerStance-eq1){I $r B 180;Start-Sleep -Milliseconds 800};C $r 'coop_probe_follow exact';C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 12|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;scope='Same-world native menu suspension/resume, both roles, standing/drawn and crouched/holstered. Fresh Steam action tokens, same validated animation-bank content, actor identity, gait recovery and explicit-off behavior. Death/load/map/session replacement are separate.'}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
if($failure){throw $failure};"MOVEMENT_RESUME_VALIDATED $($checks.Count) checks"
