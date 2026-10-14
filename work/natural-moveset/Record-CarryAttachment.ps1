[CmdletBinding()]
param([ValidateSet('host','client')][string]$Role='host',[Parameter(Mandatory)][string]$Take,[switch]$VictimScene,[switch]$NaturalRelease,[switch]$NativeDrop,[switch]$ReleaseBank,[switch]$ContactBank,[switch]$CrouchedDrop,[switch]$SnapshotBlend,[ValidateSet('side','front')][string]$View='side',[ValidateSet('none','cancel','stream')][string]$Interruption='none')
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
$peer=if($Role-eq'host'){'client'}else{'host'};$character=if($Role-eq'host'){'corvo'}else{'emily'}
$out=Join-Path $PSScriptRoot $Take;if(Test-Path -LiteralPath $out){throw 'Take already exists'}
New-Item -ItemType Directory -Path $out|Out-Null
$launch=@{};foreach($r in 'host','client'){$launch[$r]=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $launch[$r].pid).Path-ne"$root/work/asset-game/Dishonored2.exe".Replace('/','\')){throw 'Wrong process'} }
function C($r,$s){& "$lab/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function Q($r,$flag,$value=''){if($value){$v=& "$lab/bin/d2_runtime_probe.exe" $launch[$r].pid $flag $value}else{$v=& "$lab/bin/d2_runtime_probe.exe" $launch[$r].pid $flag};if($LASTEXITCODE){return $null};$v|ConvertFrom-Json}
function J($r,$suffix=''){try{Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json}catch{$null}}
function I([string]$b='Neutral',[int]$y=0,[int]$ms=150){& "$lab/Send-D2Input.ps1" -Role $Role -Button $b -LeftY $y -DurationMs $ms|Out-Null}
function Word($o,$off){@($o.words|Where-Object offset -eq $off)[0].word}
function SceneResearch($snapshot){
 $scene=@($snapshot.instances|Where-Object asset -eq 'midnight/d2coop/carry_victim_loop.midnightscene')
 if($scene.Count-ne1){return}
 $instance=Q $peer '--object' $scene[0].pointer
 $binding=Q $peer '--object' (Word $instance '0x10')
 $group=Q $peer '--object' (Word $binding '0x0')
 $objects=@{instance=$instance;binding=$binding;group=$group;declaration=(Q $peer '--object' (Word $instance '0x0'));scene=(Q $peer '--object' (Word $instance '0x8'));groupInfo=(Q $peer '--object' (Word $group '0xb8'));tracks=(Q $peer '--object' (Word $group '0x20'))}
 foreach($name in $objects.Keys){$objects[$name]|ConvertTo-Json -Depth 12|Set-Content "$out/scene-$name-held.json"}
}
function CorpsePhysics($phase,$handle){
 $parts=Q $peer '--components' ([string]$handle)
 $p=@($parts.components|Where-Object type -eq '.?AVarkCpntPhysicsSystemEvolved@@')
 if($p.Count-ne1){throw 'Unique corpse physics component missing'}
 (Q $peer '--object' $p[0].pointer)|ConvertTo-Json -Depth 12|Set-Content "$out/corpse-physics-$phase.json"
 (Q $peer '--object' $p[0].owner)|ConvertTo-Json -Depth 12|Set-Content "$out/corpse-object-$phase.json"
}
$capture=$null;$failure=$null;$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new()
try{
 foreach($r in 'host','client'){foreach($c in 'coop_carry_attachment off','coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_movement_fixture off','coop_world_observe on','coop_probe_follow exact','coop_probe_locomotion on','coop_movement_feet authored'){C $r $c}}
 $bankFile=if($ReleaseBank){"$PSScriptRoot/carry-release-banks-v1/$character-carry-release-bank.bin"}else{"$PSScriptRoot/carry-banks-v1/$character-carry-bank.bin"}
 if($ContactBank){$bankFile="$PSScriptRoot/carry-contact-banks-v2/$character-carry-contact-bank.bin"}
 $bankVersion=if($ReleaseBank-or$ContactBank){32}else{31}
 Copy-Item -LiteralPath $bankFile -Destination "$root/work/lab-instances/$peer/coop-status.json.movement.bin" -Force
 C $Role 'setviewpos -38.95 -161.5 68.1 270 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 50 -Evidence "$out/pickup-camera.json"|Out-Null
 $healthBefore=Q $Role '--health' '1';I X 0 900;Start-Sleep -Milliseconds 1900
 $pickup=Q $Role '--player-states';$pickup|ConvertTo-Json -Depth 12|Set-Content "$out/pickup.json"
 if(!$pickup.carrySceneValid-or$pickup.carryScene.variant-ne2){throw 'Native pickup did not settle into owned carry loop'}
 $carryState=@($pickup.active|Where-Object id -eq 45)[0]
 if($carryState.pointer){(Q $Role '--object' $carryState.pointer)|ConvertTo-Json -Depth 8|Set-Content "$out/native-carry-state-object.json"}
 if($SnapshotBlend){C $peer 'setviewpos -35 -162 68.1 180 0'}else{C $Role 'setviewpos -33 -153 68.1 90 0';C $peer 'setviewpos -29 -151.5 68.1 180 0'}
 if($View-eq'front'){C $peer 'setviewpos -31 -147.5 68.1 250 0'}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 0 -Evidence "$out/source-camera.json"|Out-Null
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 12 -Evidence "$out/observer-camera.json"|Out-Null
 foreach($r in 'host','client'){C $r 'coop_movement_fixture 60000';C $r 'coop_shadow_stream on'}
 Start-Sleep -Milliseconds 500;C $peer 'coop_movement_bank on';Start-Sleep -Milliseconds 1400
 $bank=J $peer '.movement.json';$stream=J $peer '.shadow-stream.json'
 $bank|ConvertTo-Json -Depth 8|Set-Content "$out/bank-before.json";$stream|ConvertTo-Json -Depth 8|Set-Content "$out/stream-before.json"
 if(!$bank.active-or$bank.version-ne$bankVersion-or$bank.carryWeight-lt.9-or!$stream.ready-or$stream.remoteState-ne14-or!$stream.carryLocalHandle){throw 'Carrier bank or mapped Steam carry stream not ready'}
 (Q $peer '--world-npcs')|ConvertTo-Json -Depth 12|Set-Content "$out/observer-world-before.json"
 (Q $peer '--midnight')|ConvertTo-Json -Depth 12|Set-Content "$out/midnight-before.json"
 (Q $peer '--health' ([string]$stream.carryLocalHandle))|ConvertTo-Json -Depth 8|Set-Content "$out/corpse-health-before.json"
 CorpsePhysics 'before' $stream.carryLocalHandle
 $avatarHandle=[string](J $peer).probeActorHandle
 (Q $peer '--health' $avatarHandle)|ConvertTo-Json -Depth 8|Set-Content "$out/avatar-health-before.json"
 (Q $peer '--avatar-sword' $avatarHandle)|ConvertTo-Json -Depth 8|Set-Content "$out/avatar-sword-before.json"
 C $peer 'coop_damage_observe off';C $peer 'coop_damage_observe 60000'
 (J $peer '.damage.json')|ConvertTo-Json -Depth 12|Set-Content "$out/damage-observations-before.json"
 [ordered]@{character=$character;role=$Role;peer=$peer;dllSha256=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;exeSha256=(Get-FileHash $launch[$Role].executable).Hash;
 bankSha256=(Get-FileHash "$root/work/lab-instances/$peer/coop-status.json.movement.bin").Hash;victimSceneSourceSha256=(Get-FileHash "$lab/assets/carry_victim_loop.midnightscene").Hash;installedAssetIndexSha256=(Get-FileHash "$root/work/asset-game/base/game1voidmod.index").Hash;healthBefore=$healthBefore;victimSceneRequested=[bool]$VictimScene;naturalRelease=[bool]$NaturalRelease;nativeDropExpected=[bool]$NativeDrop;crouchedDrop=[bool]$CrouchedDrop;view=$View;scope='Explicitly armed native attachment experiment on the authored Mayhew corpse. Maximum lifetime is 8 seconds, or 16 seconds with NaturalRelease. Requested victim-only animation is recorded separately; shoulder pose and ragdoll outcome require visual inspection.';corpseMutations=$true;snapshotBlendRequested=[bool]$SnapshotBlend}|ConvertTo-Json -Depth 8|Set-Content "$out/identity.json"
 $window=(Get-Process -Id $launch[$peer].pid).MainWindowHandle.ToInt64();if(!$window){throw 'Missing observer window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','22','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/native.mp4")
 $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $schedule=@(@{ms=2000;command='coop_carry_attachment 8000'},@{ms=3000;button='Neutral';y=17000;duration=1300},@{ms=6500;button='B';y=0;duration=150},@{ms=9000;button='Neutral';y=17000;duration=1300},@{ms=12000;button='B';y=0;duration=150},@{ms=15000;button='X';y=0;duration=500})
 if($NaturalRelease){$schedule[0].command='coop_carry_attachment 16000'}
 if($SnapshotBlend){
  if($NaturalRelease-or$NativeDrop){throw 'Snapshot proof is a separate bounded case'}
  $schedule[0].command='coop_carry_attachment snapshot8000'
  # Hold still for the complete 0.8s blend and 0.35s contact settle.
  $schedule[1].ms=3700
 }
 if($CrouchedDrop){$schedule[4].button='Neutral'}
 if($Interruption-ne'none'){
  if($NativeDrop){throw 'Interruption cases do not promise native drop playback'}
  if($Interruption-eq'cancel'){$schedule[2]=@{ms=6500;command='coop_carry_attachment off'}}
  else{
   $schedule[2]=@{ms=6500;command='coop_shadow_stream off';targetRole=$Role}
   # Native stream end deliberately disarms both peers. A new stream requires
   # the existing two-sided handshake, not a one-sided replay of the old lease.
   $schedule[3]=@{ms=9500;command='coop_shadow_stream on';targetRole='both'}
  }
 }
 if($SnapshotBlend-and$Interruption-ne'none'){
  # Cancel during the unattached 800ms snapshot blend, not after attachment.
  $schedule[2].ms=2500
  $schedule=@($schedule|Sort-Object ms)
 }
 [ordered]@{interruption=$Interruption;resumeScope='both';scope='Cancel or stop only the carry action stream; the Steam connection itself remains open. A fresh two-sided action handshake must not replay a retired attachment.'}|ConvertTo-Json|Set-Content "$out/interruption.json"
 $watch=[Diagnostics.Stopwatch]::StartNew();$next=0;$skeletonSaved=$false;$dropSaved=$false;$targetHandle=[string]$stream.carryLocalHandle
 while($watch.ElapsedMilliseconds-lt20500){
  if($capture.HasExited){throw 'Capture exited early'}
  if($next-lt$schedule.Count-and$watch.ElapsedMilliseconds-ge$schedule[$next].ms){
   $e=$schedule[$next]
   if($e.command){
    if($e.targetRole-eq'both'){
     foreach($r in $Role,$peer){
      $h=[string](J $r).probeActorHandle
      [ordered]@{health=(Q $r '--health' $h);sword=(Q $r '--avatar-sword' $h);equipment=(J $r '.shadow-equipment.json');stream=(J $r '.shadow-stream.json')}|ConvertTo-Json -Depth 12|Set-Content "$out/rearm-before-$r.json"
     }
     C $Role $e.command;C $peer $e.command
    }else{$dest=if($e.targetRole){$e.targetRole}else{$peer};C $dest $e.command}
   }else{I $e.button $e.y $e.duration}
   $events.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input=$e});$next++
  }
  if(!$skeletonSaved-and$watch.ElapsedMilliseconds-gt5000){
   $skeleton=$null;for($attempt=0;$attempt-lt5-and!$skeleton;$attempt++){$skeleton=Q $peer '--skeleton' $targetHandle}
   $skeleton|ConvertTo-Json -Depth 12|Set-Content "$out/corpse-skeleton-held.json"
   $anim=Q $peer '--animation' $targetHandle
   $anim|ConvertTo-Json -Depth 15|Set-Content "$out/corpse-animation-held.json"
   (Q $peer '--object' $anim.animation)|ConvertTo-Json -Depth 12|Set-Content "$out/corpse-animation-object-held.json"
   $midnightOffset=($anim.declaredInputs|Where-Object name -eq 'midnightData').offset
   $midnightInput=Q $peer '--object' ('0x{0:x}'-f([Convert]::ToUInt64($anim.data.Substring(2),16)+[uint64]$midnightOffset));$midnightInput|ConvertTo-Json -Depth 12|Set-Content "$out/corpse-midnight-input-held.json"
   if(([Convert]::ToUInt64((Word $midnightInput '0x8').Substring(2),16)-band0xffffff)-eq1){(Q $peer '--object' (Word $midnightInput '0x10'))|ConvertTo-Json -Depth 12|Set-Content "$out/corpse-native-clip-held.json"}
   CorpsePhysics 'held' $targetHandle
   $midnight=Q $peer '--midnight';$midnight|ConvertTo-Json -Depth 12|Set-Content "$out/midnight-held.json";SceneResearch $midnight;$skeletonSaved=$true
  }
  if($NativeDrop-and!$dropSaved-and(J $peer '.carry-attachment.json').dropStage-eq2){
   (Q $peer '--midnight')|ConvertTo-Json -Depth 12|Set-Content "$out/midnight-drop.json"
   CorpsePhysics 'drop' $targetHandle
   $a=Q $peer '--animation' $targetHandle
   $a|ConvertTo-Json -Depth 12|Set-Content "$out/corpse-animation-drop.json"
   $offset=($a.declaredInputs|Where-Object name -eq 'midnightData').offset
   $input=Q $peer '--object' ('0x{0:x}'-f([Convert]::ToUInt64($a.data.Substring(2),16)+[uint64]$offset))
   $input|ConvertTo-Json -Depth 12|Set-Content "$out/corpse-midnight-input-drop.json"
   if(([Convert]::ToUInt64((Word $input '0x8').Substring(2),16)-band0xffffff)-eq1){(Q $peer '--object' (Word $input '0x10'))|ConvertTo-Json -Depth 12|Set-Content "$out/corpse-native-clip-drop.json"}
   $dropSaved=$true
  }
  $states=Q $Role '--player-states';$rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;source=$states;sourceRoot=(J $Role);observerRoot=(J $peer);stream=(J $peer '.shadow-stream.json');movement=(J $peer '.movement.json');attachment=(J $peer '.carry-attachment.json');retarget=(Q $peer '--retargeting' ([string](J $peer).probeActorHandle));corpsePose=(Q $peer '--npc-pose' $targetHandle)})
  Start-Sleep -Milliseconds 55
 }
 (Q $Role '--health' '1')|ConvertTo-Json -Depth 8|Set-Content "$out/health-after.json"
 (Q $peer '--world-npcs')|ConvertTo-Json -Depth 12|Set-Content "$out/observer-world-after.json"
 (Q $peer '--midnight')|ConvertTo-Json -Depth 12|Set-Content "$out/midnight-after.json"
 (Q $peer '--health' $targetHandle)|ConvertTo-Json -Depth 8|Set-Content "$out/corpse-health-after.json"
 CorpsePhysics 'after' $targetHandle
 (Q $peer '--health' $avatarHandle)|ConvertTo-Json -Depth 8|Set-Content "$out/avatar-health-after.json"
 (Q $peer '--avatar-sword' $avatarHandle)|ConvertTo-Json -Depth 8|Set-Content "$out/avatar-sword-after.json"
 (J $peer '.damage.json')|ConvertTo-Json -Depth 12|Set-Content "$out/damage-observations.json"
}catch{$failure=$_.Exception.Message;throw}finally{
 I
 $rows|ConvertTo-Json -Depth 20|Set-Content "$out/observations.json";$events|ConvertTo-Json -Depth 8|Set-Content "$out/events.json"
 foreach($r in 'host','client'){try{C $r 'coop_carry_attachment off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off';C $r 'coop_damage_observe off'}catch{}}
 if($capture-and!$capture.HasExited){$capture.WaitForExit(12000)|Out-Null}
 [ordered]@{failure=$failure;samples=$rows.Count;events=$events;corpseMutations=$true}|ConvertTo-Json -Depth 8|Set-Content "$out/result.json"
}
