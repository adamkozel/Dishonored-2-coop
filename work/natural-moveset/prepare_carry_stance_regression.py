"""Record final published contact after the loaded stance transition."""
from pathlib import Path
P=Path(__file__).resolve().parent
t=(P/'Record-PickupAndDrop-v28.ps1').read_text()
t=t.replace('$nextReleasePose=6200','$nextReleasePose=6200;$stanceContactSaved=$false')
old="  if($null-eq$cancelMs-and$clock.ElapsedMilliseconds-ge6500)"
new="""  if(!$stanceContactSaved-and$clock.ElapsedMilliseconds-ge6050){
   $cb=$clock.ElapsedMilliseconds;$pair=ContactPair $peer ([string](J $peer).probeActorHandle) $target
   [ordered]@{beginMs=$cb;endMs=$clock.ElapsedMilliseconds;movement=(J $peer '.movement.json');native=(Q $Role '--player-states');pair=$pair}|ConvertTo-Json -Depth 18|Set-Content "$out/loaded-stance-contact.json"
   $stanceContactSaved=$true
  }
"""+old
assert old in t;t=t.replace(old,new)
(P/'Record-CarryStanceRegression-v28.ps1').write_text(t)
print('FINAL_PUBLISHED_STANCE_REGRESSION_READY')
