"""Two genuine native jumping casts, paired recordings and independent FX checks."""
from pathlib import Path
P=Path(__file__).resolve().parent;s=(P/'Test-WindblastNetwork.ps1').read_text(encoding='utf-8-sig')
def change(a,b):
 global s
 assert s.count(a)==1,(a[:100],s.count(a));s=s.replace(a,b)
change("[switch]$Crouched,",'')
s=s.replace("$ErrorActionPreference='Stop';", "$Crouched=$false\n$ErrorActionPreference='Stop';",1)
change("'windblast-corvo-network-v28-v1'","'windblast-corvo-air-network-v28-v1'")
change('setviewpos -30.7 -151.8 68.1 203 0','setviewpos -28 -150.5 68.1 204 0')
change('$cameraPitch=if($Crouched){21}else{15}','$cameraPitch=0 # Keep the full native jump arc in frame.')
change('I $Source 255 350 Neutral $walk;', 'I $Source 0 220 A $walk;Start-Sleep -Milliseconds 80;$jump=Q $Source \'--player-states\';$jump|ConvertTo-Json -Depth 10|Set-Content -Encoding utf8 "$out/jump-before-cast-$next.json";if(!$jump.motionValid-or$jump.motion-ne2){throw \'Native jump must precede the Windblast input\'};I $Source 255 250 Neutral $walk;')
change("action='Native LT press 350ms'","action='Native jump confirmed, then LT cast 250ms'")
change('$moveStage=0\n while', '$moveStage=0;$lastDetail=-1000;$power=$null;$peerPower=$null;$nativeFx=$null;$remoteFx=$null\n while')
change("$native=Q $Source '--player-states';$power=Q $Source '--player-windblast';$m=S $peer '.movement.json';$stream=S $peer '.shadow-stream.json';$peerPower=Q $peer '--player-darkvision';$sourceFx=S $Source '.windblast-fx.json';$peerFx=S $peer '.windblast-fx.json';$fx=S $peer '.power-fx.json';$nativeFx=Q $Source '--windblast-effects';$remoteFx=Q $peer '--windblast-effects'",
 """$native=Q $Source '--player-states';$m=S $peer '.movement.json';$stream=S $peer '.shadow-stream.json';$sourceFx=S $Source '.windblast-fx.json';$peerFx=S $peer '.windblast-fx.json';$fx=S $peer '.power-fx.json'
  if(!$power-or$watch.ElapsedMilliseconds-$lastDetail-ge350){$power=Q $Source '--player-windblast';$peerPower=Q $peer '--player-darkvision';$nativeFx=Q $Source '--windblast-effects';$remoteFx=Q $peer '--windblast-effects';$lastDetail=$watch.ElapsedMilliseconds}""")
change("$checks.Add('Full-body cast retains original native crouch/standing stance throughout')", """$checks.Add('Native jump and cast retain standing capsule stance')
 $nativeAir=@($rows|Where-Object {$_.native.powerSceneValid-and$_.native.powerScene.variant-eq11-and$_.native.motionValid-and$_.native.motion-in@(2,3)})
 $remoteAir=@($rows|Where-Object {$_.movement.powerVariant-eq11-and$_.movement.powerWeight-gt.5-and$_.movement.airWeight-gt.5})
 $nativeAirEvents=@($nativeAir|ForEach-Object {$_.native.powerScene.handle}|Select-Object -Unique);$remoteAirEvents=@($remoteAir|ForEach-Object {$_.movement.powerEvent}|Select-Object -Unique)
 if($nativeAir.Count-lt2-or$remoteAir.Count-lt2-or$nativeAirEvents.Count-ne2-or$remoteAirEvents.Count-ne2){throw \"Both genuine jumping casts must be seen in native and remote air: $($nativeAir.Count)/$($remoteAir.Count), events $($nativeAirEvents.Count)/$($remoteAirEvents.Count)\"};$checks.Add('Both confirmed native airborne casts retain remote air pose and one original power event through landing')
 if($m.airWeight-gt.02-or$m.airState){throw 'Cast failed to recover native landing'};$checks.Add('Remote landing recovers standing body without losing network ownership')""")
change('character=$character;crouched=', 'character=$character;airborne=$true;crouched=')
change('Corvo Windblast ground stance/equipment/travel body integration.', 'Corvo Windblast ordinary jump/equipment/travel body integration. Native jump precedes the LT input and is independently confirmed. Double-jump remains unverified.')
(P/'Test-WindblastAir.ps1').write_text(s,encoding='utf-8-sig')
print('Created Windblast airborne integration harness')
