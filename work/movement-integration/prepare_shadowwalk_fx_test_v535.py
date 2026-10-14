from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(N/'Test-ShadowWalkTravel-v532.ps1').read_text(encoding='utf-8-sig')
def once(a,b):
 global s
 assert s.count(a)==1,a;s=s.replace(a,b)
once('setviewpos -44.39 -153.77 68.1 180 0','setviewpos -46.3 -157.4 68.1 135 0')
once('player=$player;movement=', "player=$player;shadowFx=(S host '.shadowwalk-fx.json');nativePeerFx=(O host '--shadowwalk-effects');peerPowerFx=(S host '.power-fx.json');movement=")
once(" $checks.Add('Complete crawl cycle plus return sampled across measured travel; actual native peer sword stays holstered')", ''' $checks.Add('Complete crawl cycle plus return sampled across measured travel; actual native peer sword stays holstered')
 $fx=@($rows|ForEach-Object {$_.nativePeerFx.effects});$known=@($fx|Where-Object {$_.name-like'models/effects/gameplay/powers/shadow_walk/ply_arms/*'})
 $names=@($known.name|Sort-Object -Unique);$peerHandle=(S host).probeActorHandle
 foreach($part in '/idle/ply_arm_idle_01.fx','/idle/ply_hand_idle_01.fx','/loco/ply_arm_loco_01.fx','/loco/ply_arm_loco_02.fx','/loco/ply_hand_loco_01.fx','/transformation/ply_arm_ashes_01.fx'){
  if(!@($names|Where-Object {$_.EndsWith($part)}).Count){throw "Remote native effect not observed: $part"}
 }
 if(@($known|Where-Object {$_.owner-ne$peerHandle}).Count){throw 'Remote effect has a foreign owner'}
 if(@($fx|Where-Object {$_.name-match'(vision|swalk_end)'}).Count){throw 'First-person vision/end effect leaked to observer'}
 $lastFx=$rows[-1].shadowFx
 if($lastFx.refused-or!$lastFx.updates-or!$lastFx.started-or$lastFx.variant-or@($lastFx.slots|Where-Object kind -ne 0).Count){throw 'Remote particle follow or retirement not confirmed'}
 if(@($rows[-1].nativePeerFx.effects|Where-Object {$_.name-like'models/effects/gameplay/powers/shadow_walk/ply_arms/*'}).Count){throw 'Native particle records remain after exit'}
 [ordered]@{effectNames=$names;owner=$peerHandle;snapshots=$known.Count;started=$lastFx.started;stopped=$lastFx.stopped;updates=$lastFx.updates;refused=$lastFx.refused;nativeRecordsRetired=$true;screenEffectsExcluded=$true;scope='Native cosmetic particle ownership, posed-joint update and cleanup. Visual quality and shader morphology remain separate.'}|ConvertTo-Json -Depth 8|Set-Content "$out/remote-fx-observations.json"
 $checks.Add('Six native particle resources follow owned peer joints and retire after exit; no first-person screen effect copied')''')
assert not (N/'Test-ShadowWalkFx-v535.ps1').exists()
(N/'Test-ShadowWalkFx-v535.ps1').write_text(s,encoding='utf-8')
s=(W/'Run-ShadowWalkTravel-v532.ps1').read_text(encoding='utf-8-sig').replace('native-shadowwalk-travel-v532','native-shadowwalk-fx-v535').replace('Test-ShadowWalkTravel-v532','Test-ShadowWalkFx-v535')
(W/'Run-ShadowWalkFx-v535.ps1').write_text(s,encoding='utf-8')
print('Prepared native particle follow/cleanup535 with paired recordings')
