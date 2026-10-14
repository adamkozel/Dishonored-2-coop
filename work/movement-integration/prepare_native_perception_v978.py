from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';p=N/'Test-NpcNativePerception-v978.ps1';assert not p.exists()
s=(N/'Test-NpcNativePerception-v976.ps1').read_text(encoding='utf-8-sig').replace("'npc-native-perception-v976'","'npc-native-perception-v978'")
a=" $dirty=$true\n";assert s.count(a)==1
s=s.replace(a,a+''' # Positive local-player control also lets the native room encounter leave
 # its initial scripted behavior. Do not infer blindness from an idle scene.
 foreach($r in 'host','client'){
  C $r 'setviewpos -42.9563 -122 62.7 90 0';$localWatch=[Diagnostics.Stopwatch]::StartNew()
  do{$control=Q $r '--ai-targets' $handles[$r];if($control.combat.resolved-and$control.combat.handle-eq1){break};Start-Sleep -Milliseconds 80}while($localWatch.ElapsedMilliseconds-lt4000)
  Save "$out/local-positive-control-$r.json" $control
  C $r 'setviewpos -33 -153 68.1 90 0'
  if(!$control.combat.resolved-or$control.combat.handle-ne1){throw "Native local positive control did not detect player $r"}
 }
''',1)
p.write_text(s,encoding='utf-8');print(p)
