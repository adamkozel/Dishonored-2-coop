from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(N/'Test-ShadowWalkBody-v525.ps1').read_text(encoding='utf-8-sig')
for a,b in [("'-t','14'","'-t','18'"),("'13000'","'17000'"),('while($w.ElapsedMilliseconds-lt12000)','while($w.ElapsedMilliseconds-lt16000)'),('-DurationMs 550','-DurationMs 2600'),('Forward550ms','Forward2600ms'),('Backward550ms','Backward2600ms'),('$w.ElapsedMilliseconds-ge5500','$w.ElapsedMilliseconds-ge7300'),('$w.ElapsedMilliseconds-ge7000','$w.ElapsedMilliseconds-ge11200'),('$_.ms-gt10000','$_.ms-gt14200')]:
 assert a in s,a;s=s.replace(a,b)
needle=" $checks.Add('Native Steam entry/form/exit drive remote full-body poses, holstered equipment, distance crawl and recovery without clock rejection')"
assert s.count(needle)==1
s=s.replace(needle,needle+'''
 $distance=0.;$travelCycles=0.
 for($i=1;$i-lt$crawl.Count;$i++){
  $a=$crawl[$i-1];$b=$crawl[$i]
  $dx=[double]$b.player.position[0]-$a.player.position[0];$dy=[double]$b.player.position[1]-$a.player.position[1];$distance+=[Math]::Sqrt($dx*$dx+$dy*$dy)
  $delta=[double]$b.movement.shadowWalkPhase-$a.movement.shadowWalkPhase;if($delta-gt.5){$delta-=1};if($delta-lt-.5){$delta+=1};$travelCycles+=[Math]::Abs($delta)
 }
 $idle=@($crawl|Where-Object {$_.ms-gt6700-and$_.ms-lt7200})
 if($travelCycles-lt1.7-or$distance-lt4){throw 'Long take did not cover the planned complete crawl cycle and return'}
 if(@($crawl|Where-Object {$_.peerSword.stable-ne$false-and($_.peerSword.equipped-or$_.peerSword.attachedTag-ne'holster_wpn')}).Count){throw 'Native remote sword is not holstered during crawl'}
 [ordered]@{sourceDistanceMeters=$distance;absoluteTravelCycles=$travelCycles;stopPhases=@($idle.movement.shadowWalkPhase|Sort-Object -Unique);forwardMillis=2600;backwardMillis=2600;scope='Measured native source travel and accepted distance-driven body cadence. Does not establish final foot/hand contact quality.'}|ConvertTo-Json -Depth 6|Set-Content "$out/long-travel-observations.json"
 $checks.Add('Complete crawl cycle plus return sampled across measured travel; actual native peer sword stays holstered')
''')
assert not (N/'Test-ShadowWalkTravel-v529.ps1').exists()
(N/'Test-ShadowWalkTravel-v529.ps1').write_text(s,encoding='utf-8')
s=(W/'Run-ShadowWalkBody-v525.ps1').read_text(encoding='utf-8-sig').replace('native-shadowwalk-body-v525','native-shadowwalk-travel-v529').replace('Test-ShadowWalkBody-v525','Test-ShadowWalkTravel-v529')
(W/'Run-ShadowWalkTravel-v529.ps1').write_text(s,encoding='utf-8')
print('Prepared longer forward/stop/backward cycle test529')
