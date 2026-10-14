from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionVisibilityNetwork-v340.ps1').read_text()
old=" C host 'coop_movement_fixture 60000';Start-Sleep -Milliseconds 200\n $before="
assert s.count(old)==1
s=s.replace(old,''' C host 'coop_movement_fixture 60000'
 $isolationWait=[Diagnostics.Stopwatch]::StartNew()
 do{
  $isolation=S host '.observer-isolation.json'
  if($isolation.ready-and$isolation.movementFixture-and$isolation.remainingMs-ge45000){break}
  if(!$isolation.active){break}
  Start-Sleep -Milliseconds 30
 }while($isolationWait.ElapsedMilliseconds-lt2500)
 $isolation|ConvertTo-Json -Depth 8|Set-Content "$out/acquisition-isolation.json"
 if(!$isolation.ready-or!$isolation.movementFixture-or$isolation.remainingMs-lt45000){throw "Native AI isolation not ready before power acquisition: $($isolation.result)"}
 $before=''',1)
(P/'Test-PossessionVisibilityNetwork-v341.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace('Test-PossessionVisibilityNetwork-v340.ps1','Test-PossessionVisibilityNetwork-v341.ps1');p.write_text(s,encoding='utf-8')
print('341 adds native AI lease readiness preflight; no weaker mutation guards, binary unchanged338')
