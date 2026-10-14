from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Observe-NativeDoor-v641.ps1').read_text().replace('native-unlocked-door-corvo-v641','native-door-api-corvo-v708').replace('NativeDoor641','NativeDoorApi708').replace('candidate-heavy-v635','candidate-door-api-v707').replace('archived635','archived707')
s=s.replace('I $source X 250;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input=\'X250\';attempt=1});$sent=$true',"C $source 'coop_door_fixture open';$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='native open';attempt=1});$sent=$true")
s=s.replace("I $source X 250;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='X250';attempt=2});$second=$true", "C $source 'coop_door_fixture close';$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='native close';attempt=2});$second=$true")
s=s.replace('$sent=$false;$second=$false;$inputs=', '$sent=$false;$second=$false;$duplicateOpen=$false;$duplicateClose=$false;$inputs=')
s=s.replace('    $samples.Add([pscustomobject]@{ms=', '''    if(!$duplicateOpen-and$watch.ElapsedMilliseconds-ge4000){C $source 'coop_door_fixture open';$duplicateOpen=$true}
    if(!$duplicateClose-and$watch.ElapsedMilliseconds-ge10000){C $source 'coop_door_fixture close';$duplicateClose=$true}
    $apiPath="$root/work/lab-instances/$source/coop-status.json.door-fixture.json";$api=if(Test-Path $apiPath){Get-Content $apiPath -Raw|ConvertFrom-Json}else{$null}
    $samples.Add([pscustomobject]@{api=$api;ms=''',1)
s=s.replace("$checks.Add('Two ordinary short-X requests recorded at the adjacent unlocked door; scene/state/door results require inspection')",'''$api=Get-Content "$root/work/lab-instances/$source/coop-status.json.door-fixture.json" -Raw|ConvertFrom-Json
   $api|ConvertTo-Json -Depth 5|Set-Content "$out/native-api-final.json"
   if($api.calls-ne2-or$api.noops-ne2-or$api.refused){throw 'Expected two native transitions and two idempotent no-ops'}
   $states=@();foreach($x in $samples){$v=$x.doorSource.current;if(!$states.Count-or$states[-1]-ne$v){$states+=$v}}
   if(($states-join ',')-ne'0,1,2,1,0'){throw "Native API door cycle differs: $($states-join ',')"}
   if(@($samples|Where-Object {$_.doorPeer.current-ne0}).Count){throw 'Local API experiment unexpectedly altered peer world'}
   $checks.Add('Two explicit native transitions produce0->1->2->1->0; duplicate open/close are no-ops; peer remains unchanged')''')
s=s.replace("$checks.Add('Native source observation only; no forced door state, native scene, inventory grant or new binding claim.')","$checks.Add('Native Openable API proof only; door state/animation/event machinery runs in engine, no network synchronization yet')")
s=s.replace("scope='Source-only native unlocked door short-X observation with independently read source/peer Openable states. Both actual views, scene/state trace and before/after door entities retained. Successful script execution does not prove an open/close action or new remote binding. Restoration is established only by checks/failure.'","scope='Local native Openable74d7a0 request proof: open0 and close3 with native direction. Exact identity, unlinked/unlocked, stable state guards. Both game views recorded; no shared door networking yet.'")
(N/'Test-NativeDoorApi-v708.ps1').write_text(s)
print('Native API proof harness708 prepared')
