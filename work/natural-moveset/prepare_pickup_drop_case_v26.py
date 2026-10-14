"""Build a separate end-to-end native-input pickup/walk/drop capture."""
from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Record-NativeCarrierPickup.ps1').read_text(encoding='utf-8-sig')
s=s.replace("[switch]$Crouched,[switch]$Paired", "[switch]$CrouchedDrop,[switch]$Paired")
s=s.replace("if($Paired-and$Crouched){throw 'Paired standing study only'}", "if(!$Paired){throw 'Full paired case required'}")
begin=s.index(' if($Crouched){');end=s.index(' & "$PSScriptRoot/Set-ObserverPitch.ps1"',begin)
s=s[:begin]+s[end:]
s=s.replace('$heldSaved=$false', '$heldSaved=$false;$moveSent=$false;$crouchSent=$false')
s=s.replace('$heldSaved=$true', '& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 0 -Evidence "$out/drop-camera.json"|Out-Null; $heldSaved=$true')
s=s.replace("  if($Paired-and$null-eq$cancelMs-and$clock.ElapsedMilliseconds-gt6500){C $peer 'coop_carry_attachment off';$cancelMs=$clock.ElapsedMilliseconds}", '''  if($heldSaved-and!$moveSent-and$clock.ElapsedMilliseconds-ge4700){
   & "$lab/Send-D2Input.ps1" -Role $Role -LeftY 12000 -DurationMs 600|Out-Null;$moveSent=$true
  }
  if($CrouchedDrop-and!$crouchSent-and$clock.ElapsedMilliseconds-ge5550){I B 150;$crouchSent=$true}
  if($null-eq$cancelMs-and$clock.ElapsedMilliseconds-ge6500){I X 500;$cancelMs=$clock.ElapsedMilliseconds}''')
s=s.replace("if(!$after.carrySceneValid-or$after.carryScene.variant-ne2){throw 'Native pickup did not finish'}", "if($after.carrySceneValid-or@($after.active|Where-Object {$_.id-in @(45,46)}).Count){throw 'Native drop did not finish'}")
s=s.replace("$checks.Add('Source native pickup completes in Carry45 loop')", "$after|ConvertTo-Json -Depth 12|Set-Content \"$out/source-after-drop.json\"; $checks.Add('Source native pickup, carry walk and drop completed')")
s=s.replace('Test cancels attachment after holding;', 'Source walks and requests native drop after holding;')
s=s.replace('crouched=[bool]$Crouched;', 'crouched=$false;nativeDrop=$true;crouchedDrop=[bool]$CrouchedDrop;')
(P/'Record-PickupAndDrop-v26.ps1').write_text(s)
