[CmdletBinding()]
param([ValidateSet('host','client')][string]$Role='host',[Parameter(Mandatory)][string]$Take,[switch]$ObserveStream,[switch]$Crouched)
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
$out=Join-Path $PSScriptRoot $Take;if(Test-Path -LiteralPath $out){throw 'Take exists'}
New-Item -ItemType Directory -Path $out|Out-Null
$launch=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json
$p=Get-Process -Id $launch.pid
if(!$launch.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Not isolated clone'}
function C($s){& "$lab/Send-D2Command.ps1" -Role $Role -Command $s -WaitConsumed|Out-Null}
function Q($flag,$value=''){
 if($value){$raw=& "$lab/bin/d2_runtime_probe.exe" $launch.pid $flag $value 2>$null}else{$raw=& "$lab/bin/d2_runtime_probe.exe" $launch.pid $flag 2>$null}
 if($LASTEXITCODE){return $null};try{$raw|ConvertFrom-Json}catch{$null}
}
function I($button='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $Role -Button $button -DurationMs $ms|Out-Null}
foreach($s in 'coop_carry_attachment off','coop_movement_bank off','coop_shadow_stream off','coop_movement_fixture off'){C $s}
C 'setviewpos -38.95 -161.5 68.1 270 0'
if($Crouched){
 $stance=$null;$attempts=[Collections.Generic.List[object]]::new()
 for($attempt=0;$attempt-lt3;$attempt++){
  $stance=Q '--player-states';$attempts.Add($stance)
  if($stance.defaultMode-and$stance.stance-eq1){break}
  # A dropped virtual-controller edge must not cause a blind toggle. Retry
  # only after a fresh read confirms the source is still standing.
  if($stance.defaultMode-and$stance.stance-eq0){I Neutral 120;Start-Sleep -Milliseconds 180;I B 300}
  Start-Sleep -Milliseconds 500
 }
 $stance=Q '--player-states';$attempts.Add($stance)
 $attempts|ConvertTo-Json -Depth 12|Set-Content "$out/crouch-entry-attempts.json"
 if($stance.stance-ne1-or!$stance.defaultMode){throw 'Native crouched source not ready'}
 $stance|ConvertTo-Json -Depth 12|Set-Content "$out/crouched-before.json"
}
& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 50 -Evidence "$out/camera.json"|Out-Null
$world=Q '--world-npcs';$world|ConvertTo-Json -Depth 12|Set-Content "$out/world-before.json"
$targets=@($world.npcs|Where-Object {$_.spawnerName-eq'ld_spawner_alexi_escape'-and$_.definition-eq'models/characters/def/story/alexi_mayhew.def'})
if($targets.Count-ne1-or!$targets[0].reciprocal-or!$targets[0].unique){throw 'Unique authored corpse missing'}
$target=[string]$targets[0].handle;$h=Q '--health' $target;$before=Q '--player-states'
if(!$h-or$h.current-ne0-or$before.carrySceneValid){throw 'Expected unheld native corpse'}
$parts=Q '--components' $target;$physics=@($parts.components|Where-Object type -eq '.?AVarkCpntPhysicsSystemEvolved@@')
if($physics.Count-ne1){throw 'Native physics component missing'}
$healthBefore=Q '--health' '1'
if($ObserveStream){
 $peer=if($Role-eq'host'){'client'}else{'host'}
 foreach($r in $Role,$peer){foreach($s in 'coop_world_observe on','coop_probe_follow exact','coop_probe_locomotion on','coop_movement_fixture 60000','coop_shadow_stream on'){& "$lab/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}}
 Start-Sleep -Milliseconds 500
}
[ordered]@{role=$Role;target=$target;observeStream=[bool]$ObserveStream;crouched=[bool]$Crouched;exeSha256=(Get-FileHash $p.Path).Hash;dllSha256=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probeSha256=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;harnessSha256=(Get-FileHash $PSCommandPath).Hash;healthBefore=$healthBefore;scope='Native first-person pickup reference. Normal X input; read-only corpse skeleton, physics, scene and retarget observations. Optional pre-armed two-peer semantic stream; no remote corpse mutations and no third-person lift acceptance.'}|ConvertTo-Json -Depth 8|Set-Content "$out/identity.json"
$window=$p.MainWindowHandle.ToInt64()
$args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','8','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/native.mp4")
$capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
$rows=[Collections.Generic.List[object]]::new();$watch=[Diagnostics.Stopwatch]::StartNew();$inputMs=$null;$failure=$null
try{
 while($watch.ElapsedMilliseconds-lt6500){
  if($capture.HasExited){throw 'Capture ended early'}
  if($null-eq$inputMs-and$watch.ElapsedMilliseconds-ge700){I X 900;$inputMs=$watch.ElapsedMilliseconds}
  $start=$watch.ElapsedMilliseconds;$states=Q '--player-states';$skeleton=Q '--skeleton' $target
  $retarget=Q '--retargeting' '1';$object=Q '--object' $physics[0].pointer
  $stream=$null;$sourceStream=$null
  if($ObserveStream){try{$stream=Get-Content "$root/work/lab-instances/$peer/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json}catch{}
   try{$sourceStream=Get-Content "$root/work/lab-instances/$Role/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json}catch{}}
  $rows.Add([pscustomobject]@{beginMs=$start;endMs=$watch.ElapsedMilliseconds;native=$states;skeleton=$skeleton;retarget=$retarget;physics=$object;observerStream=$stream;sourceStream=$sourceStream})
 }
 $after=Q '--player-states';$after|ConvertTo-Json -Depth 12|Set-Content "$out/after.json"
 if(!$after.carrySceneValid-or$after.carryScene.variant-ne2){throw 'Pickup did not finish in native carry loop'}
 (Q '--midnight')|ConvertTo-Json -Depth 15|Set-Content "$out/midnight-after.json"
 (Q '--health' '1')|ConvertTo-Json -Depth 8|Set-Content "$out/health-after.json"
}catch{$failure=$_.Exception.Message;throw}finally{
 I
 if($ObserveStream){foreach($r in $Role,$peer){foreach($s in 'coop_shadow_stream off','coop_movement_fixture off'){& "$lab/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}}}
 $rows|ConvertTo-Json -Depth 22|Set-Content "$out/observations.json"
 if(!$capture.HasExited){$capture.WaitForExit(10000)|Out-Null}
 [ordered]@{failure=$failure;samples=$rows.Count;inputMs=$inputMs;scope='Reads are sequential, with sample begin/end times. Individual stable skeleton flags do not make whole rows atomic.'}|ConvertTo-Json|Set-Content "$out/result.json"
}
