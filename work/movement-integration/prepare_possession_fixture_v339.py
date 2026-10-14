from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionVisibilityNetwork-v338.ps1').read_text()
s=s.replace(",'coop_movement_fixture 60000'){C $r $cmd}", "){C $r $cmd}\n  if($r-eq'client'){C $r 'coop_movement_fixture 60000'}")
anchor=" $before=Q host '--player-possession';"
assert s.count(anchor)==1
s=s.replace(anchor,''' # Let the native tutorial guard complete its approach before taking the
 # finite AI references. Pausing decision/reflex before entering the balcony
 # can leave him at the far authored starting point indefinitely.
 $approach=[Collections.Generic.List[object]]::new();$approachWait=[Diagnostics.Stopwatch]::StartNew();$near=$false
 do{
  $candidate=@((Q host '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.unique-and$_.reciprocal})
  $health=Q host '--health' 1
  $approach.Add([pscustomobject]@{ms=$approachWait.ElapsedMilliseconds;npc=$candidate;health=$health.current})
  if($health.current-ne56){throw 'Native approach changed source health'}
  if($candidate.Count-eq1){$v=$candidate[0];$near=[Math]::Abs([double]$v.position[2]-66.4)-lt.5-and([Math]::Pow([double]$v.position[0]+48.59,2)+[Math]::Pow([double]$v.position[1]+155.07,2)-lt49)}
  if(!$near){Start-Sleep -Milliseconds 150}
 }while(!$near-and$approachWait.ElapsedMilliseconds-lt6500)
 $approach|ConvertTo-Json -Depth 8|Set-Content "$out/native-guard-approach.json"
 if(!$near){throw 'Native tutorial guard did not approach measured floor within bounded wait'}
 C host 'coop_movement_fixture 60000';Start-Sleep -Milliseconds 200
'''+anchor)
s=s.replace('No verified upper-floor tutorial guard within twenty metres','No verified upper-floor tutorial guard within seven metres')
(P/'Test-PossessionVisibilityNetwork-v339.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace('Test-PossessionVisibilityNetwork-v338.ps1','Test-PossessionVisibilityNetwork-v339.ps1');p.write_text(s,encoding='utf-8')
print('339 allows bounded native guard approach before AI isolation, preserves measured floor and health checks')
