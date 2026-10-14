from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-SwarmRatVisualNetwork-v292.ps1').read_text(encoding='utf-8-sig')
s=s.replace(" $dirty=$true\n", " $visualBaseline=S client '.swarm-rat-visual.json';$visualBaseline|ConvertTo-Json -Depth 8|Set-Content \"$out/visual-baseline.json\"\n $dirty=$true\n")
s=s.replace('$visual.created-ne1-or$visual.started-lt1-or$visual.updated-lt20-or$visual.refused','($visual.created-$visualBaseline.created)-ne1-or($visual.started-$visualBaseline.started)-lt1-or($visual.updated-$visualBaseline.updated)-lt20-or($visual.refused-$visualBaseline.refused)-ne0')
s=s.replace('$hidden.hidden-lt1','($hidden.hidden-$visualBaseline.hidden)-lt1')
anchor="  Start-Sleep -Milliseconds 30\n"
assert s.count(anchor)==1
s=s.replace(anchor,'''  if($w.ElapsedMilliseconds-ge5500-and!(Test-Path "$out/visual-early-components.json")){
   $v=S client '.swarm-rat-visual.json'
   if($v.active-and$v.handle){
    $cp=Q client '--components' $v.handle;$cp|ConvertTo-Json -Depth 12|Set-Content "$out/visual-early-components.json"
    Q client '--object' $cp.components[0].owner|ConvertTo-Json -Depth 12|Set-Content "$out/visual-early-entity.json"
    & "$lab/bin/d2_runtime_probe.exe" $pids.client '--rat-skeleton' $v.handle > "$out/visual-early-skeleton-0.json"
    Start-Sleep -Milliseconds 120
    & "$lab/bin/d2_runtime_probe.exe" $pids.client '--rat-skeleton' $v.handle > "$out/visual-early-skeleton-1.json"
   }
  }
'''+anchor)
p=P/'Test-SwarmRatVisualNetwork-v293.ps1';assert not p.exists();p.write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace('Test-SwarmRatVisualNetwork-v292.ps1','Test-SwarmRatVisualNetwork-v293.ps1');p.write_text(s)
print('Corrected cumulative instrumentation assertions and added early read-only render evidence')
