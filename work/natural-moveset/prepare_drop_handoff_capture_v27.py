"""Keep v26 recordings reproducible; add a bounded release pose trace."""
from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Record-PickupAndDrop-v26.ps1').read_text(encoding='utf-8-sig')
s=s.replace('$nextSettleTrace=0','$nextSettleTrace=0;$releasePoses=[Collections.Generic.List[object]]::new();$nextReleasePose=6200')
s=s.replace("  Start-Sleep -Milliseconds 25", '''  if($clock.ElapsedMilliseconds-ge$nextReleasePose-and$clock.ElapsedMilliseconds-lt9300){
   $b=$clock.ElapsedMilliseconds;$p=Q $peer '--npc-pose' $target
   $releasePoses.Add([pscustomobject]@{beginMs=$b;endMs=$clock.ElapsedMilliseconds;pose=$p;attachment=(J $peer '.carry-attachment.json')})
   $nextReleasePose=$clock.ElapsedMilliseconds+100
  }
  Start-Sleep -Milliseconds 25''')
s=s.replace(' $rows|ConvertTo-Json -Depth 18|Set-Content "$out/observations.json"', ' $releasePoses|ConvertTo-Json -Depth 12|Set-Content "$out/release-poses.json"\n $rows|ConvertTo-Json -Depth 18|Set-Content "$out/observations.json"')
(P/'Record-PickupAndDrop-v27.ps1').write_text(s)
