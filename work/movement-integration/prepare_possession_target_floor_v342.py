from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionVisibilityNetwork-v341.ps1').read_text()
s=s.replace("  if($r-eq'client'){C $r 'coop_movement_fixture 60000'}", "  C $r 'coop_movement_fixture 60000'")
start=s.index(" $dirty=$true\n C host 'setviewpos")
end=s.index(" $before=Q host '--player-possession'",start)
s=s[:start]+''' $dirty=$true
 $isolationWait=[Diagnostics.Stopwatch]::StartNew()
 do{$isolation=S host '.observer-isolation.json';if($isolation.ready-and$isolation.movementFixture){break};if(!$isolation.active){break};Start-Sleep -Milliseconds 30}while($isolationWait.ElapsedMilliseconds-lt2500)
 $isolation|ConvertTo-Json -Depth 8|Set-Content "$out/acquisition-isolation.json"
 if(!$isolation.ready-or!$isolation.movementFixture-or$isolation.remainingMs-lt45000){throw "Native AI isolation not ready before source placement: $($isolation.result)"}
 # Target-relative source-only placement after native AI references are held.
 # Never move the NPC. Each proposed floor is independently measured; failed
 # floor probes return immediately to the previously verified balcony floor.
 $candidate=@((Q host '--world-npcs').npcs|Where-Object {$_.spawnerName-eq'ld_spawner_guard_tutorial_2'-and$_.unique-and$_.reciprocal-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'})
 if($candidate.Count-ne1-or[Math]::Abs([double]$candidate[0].position[2]-66.4)-gt.5){throw 'Exact tutorial guard2 not on upper floor'}
 $floorAttempts=[Collections.Generic.List[object]]::new();$floorFound=$false
 foreach($offset in @(@(-3.0,0.0),@(0.0,-3.0),@(3.0,0.0),@(0.0,3.0))){
  $sourceX=[double]$candidate[0].position[0]+$offset[0];$sourceY=[double]$candidate[0].position[1]+$offset[1]
  C host ('setviewpos {0} {1} 68.1 90 0' -f $sourceX.ToString('F5',[Globalization.CultureInfo]::InvariantCulture),$sourceY.ToString('F5',[Globalization.CultureInfo]::InvariantCulture))
  Start-Sleep -Milliseconds 700
  $ground=Q host '--player-states';$health=Q host '--health' 1
  $floorAttempts.Add([pscustomobject]@{sourceX=$sourceX;sourceY=$sourceY;ground=$ground;health=$health.current})
  if($health.current-ne56){throw 'Source placement changed health'}
  if($ground.motionValid-and$ground.motion-eq0-and$ground.stance-eq0-and$ground.defaultMode-and[Math]::Abs([double]$ground.position[2]-66.4)-lt.1){$floorFound=$true;break}
  C host 'setviewpos -48.59 -155.07 68.1 90 0'
 }
 $floorAttempts|ConvertTo-Json -Depth 10|Set-Content "$out/source-floor-attempts.json"
 if(!$floorFound){throw 'No verified nearby native source floor; NPC retained untouched'}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $Pitch -Evidence "$out/source-camera.json" -Quick
 $ground|ConvertTo-Json -Depth 10|Set-Content "$out/source-grounded.json"
''' + s[end:]
s=s.replace('[double]$_.position[0]+48.59','[double]$_.position[0]-$sourceX').replace('[double]$_.position[1]+155.07','[double]$_.position[1]-$sourceY')
s=s.replace('[double]$target.position[1]+155.07','[double]$target.position[1]-$sourceY').replace('[double]$target.position[0]+48.59','[double]$target.position[0]-$sourceX')
s=s.replace(" C host ('setviewpos -48.59 -155.07 68.1 {0} 0' -f $yaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))\n",'')
s=s.replace("placement='Only yaw aims at verified native guard; original player floor retained'", "placement='Native source floor verified after target-relative placement; NPC untouched'")
s=s.replace('$dx=-48.59-[double]$target.position[0];$dy=-155.07-[double]$target.position[1]', '$dx=$sourceX-[double]$target.position[0];$dy=$sourceY-[double]$target.position[1]')
s=s.replace(' $sourceX=-48.59;$sourceY=-155.07\n','')
s=s.replace('[double]$placed.position[2]-66.4370575)-gt.06','[double]$placed.position[2]-66.4)-gt.1')
# Keep the initial observer on the previously verified floor. Reframe after
# exit using the original340 view for direct comparison.
s=s.replace('$observerX=$sourceX+4.2;$observerY=$sourceY-1.8;$observerYaw=[Math]::Atan2(-1.8,4.2)*180/[Math]::PI+180', '$observerX=-44.39;$observerY=-153.27;$observerYaw=[Math]::Atan2($sourceY-$observerY,$sourceX-$observerX)*180/[Math]::PI')
(P/'Test-PossessionVisibilityNetwork-v342.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace('Test-PossessionVisibilityNetwork-v341.ps1','Test-PossessionVisibilityNetwork-v342.ps1');p.write_text(s,encoding='utf-8')
print('342 tests measured player floors around untouched exact guard2 after AI isolation; binary338 unchanged')
