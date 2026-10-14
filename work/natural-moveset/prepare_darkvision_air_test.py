"""Create the bounded airborne integration harness from the ground harness."""
from pathlib import Path
p=Path(__file__).parent
s=(p/'Test-DarkVisionContexts.ps1').read_text(encoding='utf-8-sig')
def replace(a,b):
    global s
    assert s.count(a)==1,(a[:80],s.count(a))
    s=s.replace(a,b)
replace("[switch]$Crouched,[switch]$Holstered,[switch]$Moving)","[switch]$Holstered,[switch]$Moving)\n$Crouched=$false")
replace("'darkvision-corvo-crouched-v26-v1'","'darkvision-corvo-air-v26-v1'")
replace("setviewpos -30.7 -151.8 68.1 203 0","setviewpos -28 -150.5 68.1 204 0")
replace("$cameraPitch=if($Crouched){21}else{15}","$cameraPitch=0 # Wider framing retains the full body through the 1.5m jump apex.")
replace("I $Source 255 350 Neutral $walk;","I $Source 255 250 A $walk;")
replace("action='Native LT press 350ms'","action='Native simultaneous A jump and LT cast, 250ms'")
replace("$activated=$false;$deactivated=$false;$moveStage=0","$activated=$false;$deactivated=$false;$moveStage=0;$lastPowerRead=-1000;$power=$null;$peerPower=$null")
replace("$native=Q $Source '--player-states';$power=Q $Source '--player-darkvision';$m=S $peer '.movement.json';$stream=S $peer '.shadow-stream.json';$peerPower=Q $peer '--player-darkvision'", "$native=Q $Source '--player-states';$m=S $peer '.movement.json';$stream=S $peer '.shadow-stream.json'\n  if(!$power-or$watch.ElapsedMilliseconds-$lastPowerRead-ge500){$power=Q $Source '--player-darkvision';$peerPower=Q $peer '--player-darkvision';$lastPowerRead=$watch.ElapsedMilliseconds}")
replace("$checks.Add('Full-body cast retains original native crouch/standing stance throughout')","$checks.Add('Native physics remains standing while jumping and casting')\n $nativeAir=@($rows|Where-Object {$_.native.powerSceneValid-and$_.native.powerScene.variant-eq10-and$_.native.motionValid-and$_.native.motion-in@(2,3)})\n $remoteAir=@($rows|Where-Object {$_.movement.powerVariant-eq10-and$_.movement.powerWeight-gt.5-and$_.movement.airWeight-gt.5})\n if($nativeAir.Count-lt2-or$remoteAir.Count-lt2){throw \"No confirmed simultaneous native and remote air cast: $($nativeAir.Count)/$($remoteAir.Count)\"};$checks.Add('Confirmed native Jump/Fall casts preserve remote airborne body animation with the power gesture')\n $airEvents=@($remoteAir|ForEach-Object {$_.movement.powerEvent}|Select-Object -Unique);if($airEvents.Count-ne2){throw 'Both cast events must overlap the remote airborne pose'};$checks.Add('Both jumping casts keep one native power event through landing; no cast restart')\n if($m.airWeight-gt.02-or$m.airState){throw 'Airborne cast did not recover to standing'};$checks.Add('Native landing recovers remote standing body without dropping network ownership')")
replace("crouched=[bool]$Crouched;holstered=","airborne=$true;crouched=[bool]$Crouched;holstered=")
replace("Scoped native Dark Vision on/off casts in the explicitly recorded stance, sword and travel context.","Scoped native Dark Vision on/off casts during two ordinary jumps in the explicitly recorded sword and travel context.")
replace("Airborne/other contexts and broader lifecycle remain unverified.","Double-jump, other contexts and broader lifecycle remain unverified.")
(p/'Test-DarkVisionAir.ps1').write_text(s,encoding='utf-8-sig')
print('Created isolated airborne integration harness')
