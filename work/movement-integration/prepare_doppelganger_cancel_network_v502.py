from pathlib import Path
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'
read=lambda p:p.read_text(encoding='utf-8-sig')
def new(p,s):
    assert not p.exists(),p
    p.write_text(s,encoding='utf-8')
s=read(N/'Test-DoppelgangerHolstered-v496.ps1')
s=s.replace("[string]$Stance='standing')","[string]$Stance='standing',[switch]$Holstered)")
s=s.replace("'-t','29'","'-t','16'")
s=s.replace(' & {\n  & "$PSScriptRoot/Set-ObserverPitch.ps1"',' if($Holstered){\n  & "$PSScriptRoot/Set-ObserverPitch.ps1"')
s=s.replace('$m.presentedDrawn-ne$true','$m.presentedDrawn-ne[bool]$Holstered').replace('$nativeSword.equipped-ne$true','$nativeSword.equipped-ne[bool]$Holstered')
s=s.replace('[Math]::Abs($m.armedMix)-lt.02','[Math]::Abs($m.armedMix-$(if($Holstered){0}else{1}))-lt.02')
s=s.replace('$m.presentedDrawn-eq$true','$m.presentedDrawn-eq[bool]$Holstered').replace('$nativeSword.equipped-eq$true','$nativeSword.equipped-eq[bool]$Holstered')
s=s.replace('holstered=$true','holstered=[bool]$Holstered')
cut=s.index(' $fxMonitor=')
tail=s[s.index(" C client 'coop_doppelganger_fixture off';$w.Restart()",cut):]
s=s[:cut]+'''
 $rawMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-midnight','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/native-midnight.ndjson" -RedirectStandardError "$out/native-midnight.stderr" -PassThru
 $recorders+=$rawMonitor
 $w.Restart();$aimed=$false;$cancelled=$false
 while($w.ElapsedMilliseconds-lt14000){
  if(!$aimed-and$w.ElapsedMilliseconds-ge1100){I client 255 6000;$aimed=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native LT aim; release-to-cast will be cancelled'})}
  if(!$cancelled-and$w.ElapsedMilliseconds-ge2400){
   & "$lab/Send-D2Input.ps1" -Role client -Button X -LeftTrigger 255 -DurationMs 250|Out-Null
   $cancelled=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native X while LT held; both released after250ms'})
  }
  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=(Q client '--player-states');sword=(Q client '--player-sword');component=(Q client '--object' $power.component);movement=$m;stream=$stream;cloneSource=(S client '.doppelganger-world.json');clonePeer=(S host '.doppelganger-world.json');cloneVisual=(S host '.doppelganger-visual.json')})
  if(!$m.active-or!$stream.ready-or!$stream.armed){throw 'Native body/transport retired during cancellation'}
  Start-Sleep -Milliseconds 30
 }
 I client;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;$rawMonitor.WaitForExit(4000)|Out-Null
 if(!$monitor.HasExited-or$monitor.ExitCode-or!$rawMonitor.HasExited-or$rawMonitor.ExitCode){throw 'Native source watchers failed'}
 $samples=@(Get-Content "$out/power-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json});$scenes=@($samples|Where-Object valid)
 $raw=@(Get-Content "$out/native-midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 foreach($v in29,30){
  if(!@($scenes|Where-Object variant -eq $v).Count-or!@($rows|Where-Object {$_.movement.powerVariant-eq$v-and$_.movement.powerWeight-gt.5}).Count){throw "Native/remote aim phase $v absent"}
 }
 if(@($scenes|Where-Object variant -eq31).Count-or@($rows|Where-Object {$_.movement.powerVariant-eq31-and$_.movement.powerWeight-gt.5}).Count){throw 'Cancellation unexpectedly played successful cast release'}
 $scenes|ConvertTo-Json -Depth 12|Set-Content "$out/strict-phases.json"
 $checks.Add('Native owned entry/aim phases drive remote full-body poses; cancellation plays no successful-cast release')
 $cloneScenes=@($raw.instances|Where-Object {$_.state-eq3-and$_.asset-match'^midnight/gameplay/human/common/doppelganger/doppelganger_(spawn|dispawn|death_fx)\\.midnightscene$'})
 $counters=@($rows|ForEach-Object {$word=@($_.component.words|Where-Object offset -eq '0x60');if($word.Count-ne1){throw 'Power activation counter absent'};[Convert]::ToUInt64($word[0].word,16)-shr32})
 $visual=S host '.doppelganger-visual.json'
 [ordered]@{nativeCloneSceneSamples=$cloneScenes.Count;counterBefore=$counters[0];counterAfter=$counters[-1];manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;visualBefore=$visualStart;visualAfter=$visual}|ConvertTo-Json -Depth 12|Set-Content "$out/no-spawn-proof.json"
 if($cloneScenes.Count-or@($counters|Where-Object {$_-ne$counters[0]}).Count-or[Math]::Abs($manaAfter.currentCandidate-$manaBefore.currentCandidate)-gt.05){throw 'Native cast committed during cancellation'}
 if($visual.created-ne$visualStart.created-or$visual.shown-ne$visualStart.shown-or$visual.fx.started-ne$visualStart.fx.started-or$visual.visible-or$visual.fx.scene-or@($rows|Where-Object {$_.clonePeer.present-or$_.clonePeer.leased-or$_.cloneVisual.visible}).Count){throw 'Cancelled aim produced a remote clone or effect'}
 $checks.Add('Native cancellation spends no mana, increments no activation counter, starts no native clone scene and creates no remote clone/FX')
 $active=@($rows|Where-Object {$_.ms-ge900-and$_.ms-le6500})
 $recovered=@($rows|Where-Object {$_.ms-ge5000})
 if(!$aimed-or!$cancelled-or$active.Count-lt15-or$recovered.Count-lt10){throw 'Cancellation coverage too short'}
 if(@($active|Where-Object {$_.movement.weight-lt.95-or$_.movement.clockRejection.count}).Count){throw 'Cancellation lost body continuity'}
 if(@($recovered|Where-Object {$_.movement.powerWeight-gt.02-or$_.movement.weight-lt.95-or$_.player.stance-ne$expectedStance-or[Math]::Abs($_.movement.stanceMix-$expectedStance)-gt.05-or$_.movement.presentedDrawn-eq[bool]$Holstered-or$_.sword.equipped-eq[bool]$Holstered}).Count){throw 'Cancellation left stale animation or incorrect stance/equipment'}
 [ordered]@{stance=$Stance;holstered=[bool]$Holstered;activeSamples=$active.Count;recoverySamples=$recovered.Count;minimumBodyWeight=($active.movement.weight|Measure-Object -Minimum).Minimum;maxRecoveredPowerWeight=($recovered.movement.powerWeight|Measure-Object -Maximum).Maximum}|ConvertTo-Json|Set-Content "$out/cancel-recovery.json"
 $checks.Add('Body remains continuous through native Cancel/Undraw and recovers its original stance/equipment without stale power posing')
 $avatar=Q host '--avatar-sword' (S host).probeActorHandle;$avatar|ConvertTo-Json -Depth 8|Set-Content "$out/avatar-sword-after.json"
 if($avatar.equipped-eq[bool]$Holstered){throw 'Remote native sword selection changed'}
''' +tail
s=s.replace('foreach($v in29,30)','foreach($v in 29,30)').replace('Where-Object variant -eq31','Where-Object variant -eq 31')
s=s.replace('Standing/crouched holstered native Emily Doppelganger phases drive remote full-body bank57 poses through native Steam in isolated AI-held Dunwall. Local first-person remains native; peer animation does not execute gameplay clone or damage events; separate native FX/material-only scenes reproduce source-owned summon/disappearance effects. Clone identity/root/lifetime Steam transport verified separately; shared clone gameplay, alternate contexts and visual acceptance remain pending. Checkpoint and protected14 restoration mandatory.',
            'Native X while LT is held cancels aimed Doppelganger in the specified ground/equipment context. Verify entry/aim body, no cast-release, source activation/mana/clone-scene absence, no remote clone/FX and full-weight stance/equipment recovery. Local first-person remains native. General interruptions and full power/shared gameplay remain separate. Checkpoint and protected14 restoration mandatory.')
new(N/'Test-DoppelgangerCancel-v502.ps1',s)
w=read(W/'Run-DoppelgangerHolstered-v496.ps1').replace('doppelganger-holstered-standing-v496','doppelganger-cancel-standing-v502').replace('Test-DoppelgangerHolstered-v496','Test-DoppelgangerCancel-v502')
w=w.replace("[string]$Stance='standing')","[string]$Stance='standing',[switch]$Holstered)").replace('-Take $Take -Stance $Stance\n','-Take $Take -Stance $Stance -Holstered:$Holstered\n')
new(W/'Run-DoppelgangerCancel-v502.ps1',w)
print('Prepared paired cancellation coverage using the confirmed native X control; no production changes.')
