from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-DoorOverlap-v739.ps1').read_text(encoding='utf-8-sig')
s=s.replace('native-door-overlap-host-first-v739','native-door-overlap-host-first-v747').replace('NativeDoorOverlap739','NativeDoorOverlap747')
s=s.replace('candidate-door-catalog-v728','candidate-checkpoint-retry-v742').replace('Expected archived728 candidate','Expected archived742 candidate')
old="  foreach($r in 'host','client'){try{I $r;C $r 'coop_door_sync off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_movement_fixture off'}catch{}}"
assert old in s
s=s.replace(old,"""  foreach($r in 'host','client'){
   try{I $r}catch{}
   Copy-Item "$root/work/lab-instances/$r/coop-status.json.door-sync.json" "$out/final-$r-door-sync.json"
  }""")
s=s.replace('  if($dirty){try{',old.replace('try{I $r;','try{')+'\n  if($dirty){try{')
s=s.replace('$peerCapture.WaitForExit(35000)','$peerCapture.WaitForExit(45000)')
(N/'Test-DoorOverlap-v747.ps1').write_text(s)
print('Prepared overlap747 pinned742; retain AI isolation through capture cleanup')
