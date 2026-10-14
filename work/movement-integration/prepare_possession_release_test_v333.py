from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Observe-NativePossessionTarget-v332.ps1').read_text()
s=s.replace("'-t','16'", "'-t','9'").replace("'15000'", "'8000'")
s=s.replace('$w.ElapsedMilliseconds-lt14000', '$w.ElapsedMilliseconds-lt7000').replace('$w.ElapsedMilliseconds-ge7600', '$w.ElapsedMilliseconds-ge4000')
s=s.replace("  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Player or partner health changed'}", "  $health=[ordered]@{player=(Q $r '--health' 1);partner=(Q $r '--health' (S $r).probeActorHandle)}\n  $health|ConvertTo-Json -Depth 8|Set-Content \"$out/health-after-$r.json\"\n  if($health.player.current-ne56-or$health.partner.current-ne40){throw \"Player or partner health changed: $r player=$($health.player.current) partner=$($health.partner.current)\"}")
(P/'Observe-NativePossessionTarget-v333.ps1').write_text(s,encoding='utf-8')
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1'
s=p.read_text().replace('Observe-NativePossessionTarget-v332.ps1','Observe-NativePossessionTarget-v333.ps1');p.write_text(s,encoding='utf-8')
print('Prepared shorter native release observation with explicit health evidence; no relaxed health or inventory assertions')
