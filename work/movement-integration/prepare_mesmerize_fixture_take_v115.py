from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
p=P/'Test-MesmerizeLinksInterrupt-v112.ps1';s=p.read_text(encoding='utf-8-sig')
s=s.replace("sourceWorld=(S client '.mesmerize-world-stream.json');effects=$fx", "sourceWorld=(S client '.mesmerize-world-stream.json');hostIsolation=(S host '.observer-isolation.json');clientIsolation=(S client '.observer-isolation.json');effects=$fx")
old=" foreach($r in 'host','client'){if((Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Remote avatar health changed'}}"
new=""" foreach($r in 'host','client'){
  $h=Q $r '--health' (S $r).probeActorHandle;$h|ConvertTo-Json -Depth 6|Set-Content "$out/avatar-health-after-$r.json"
  $local=Q $r '--health' 1;$local|ConvertTo-Json -Depth 6|Set-Content "$out/player-health-after-$r.json"
  if($h.current-ne40-or$local.current-ne56){throw "Native health changed in $r`: avatar=$($h.current), player=$($local.current)"}
 }
 if(@($rows|Where-Object {!$_.hostIsolation.active-or!$_.hostIsolation.ready-or!$_.clientIsolation.active-or!$_.clientIsolation.ready}).Count){throw 'Finite movement-test NPC isolation retired early'}
 $checks.Add('Finite test-only NPC decision/reflex references stay owned across the menu and are explicitly released during checkpoint cleanup; gameplay remains unsynchronized')"""
assert s.count(old)==1;s=s.replace(old,new)
(P/'Test-MesmerizeLinksInterrupt-v115.ps1').write_text(s,encoding='utf-8')
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert s.count('Test-MesmerizeLinksInterrupt-v112.ps1')==1;p.write_text(s.replace('Test-MesmerizeLinksInterrupt-v112.ps1','Test-MesmerizeLinksInterrupt-v115.ps1'),encoding='utf-8')
print('Prepared finite-fixture continuity evidence')
