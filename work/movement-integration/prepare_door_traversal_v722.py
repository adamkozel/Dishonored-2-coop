from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
t=(N/'Test-NativeDoorSync-v718.ps1').read_text(encoding='utf-8-sig').replace('v718','v722').replace('NativeDoorSync718','DoorTraversal722').replace('native-door-sync-corvo','native-door-traversal-corvo')
t=t.replace("C $peer 'setviewpos -44 -161 68.1 220 0'", "C $peer 'setviewpos -47.05 -163.0 68.1 270 0'",1).replace("C $source 'setviewpos -47.05 -163.5 68.1 270 0'", "C $source 'setviewpos -44 -161 68.1 220 0'",1)
t=t.replace('-Role $peer -Pitch 10','-Role $peer -Pitch 0')
a=t.index('   $character=');b=t.index("   foreach($r in 'host','client'){C $r 'coop_shadow_stream on'}",a)
t=t[:a]+'''   foreach($bankTarget in 'host','client'){
    $character=if($bankTarget-eq'host'){'emily'}else{'corvo'}
    $entry=Get-Content "$PSScriptRoot/heavy-bank-manifest-v633.json" -Raw|ConvertFrom-Json|Where-Object role -eq $character
    $bank=Join-Path $PSScriptRoot $entry.bank;if((Get-FileHash $bank).Hash-ne$entry.sha256){throw 'Bank hash differs'}
    Copy-Item $bank "$root/work/lab-instances/$bankTarget/coop-status.json.movement.bin" -Force;C $bankTarget 'coop_movement_bank on'
    $bw=[Diagnostics.Stopwatch]::StartNew();do{$m=Get-Content "$root/work/lab-instances/$bankTarget/coop-status.json.movement.json" -Raw|ConvertFrom-Json;if($m.active-and$m.version-eq67){break};Start-Sleep -Milliseconds 40}while($bw.ElapsedMilliseconds-lt5000)
    if(!$m.active-or$m.version-ne67){throw 'Movement bank preload failed'}
   }
'''+t[b:]
a=t.index('   $watch=[Diagnostics.Stopwatch]::StartNew();$samples=');b=t.index('    $apiPath=',a)
t=t[:a]+'''   $watch=[Diagnostics.Stopwatch]::StartNew();$samples=[Collections.Generic.List[object]]::new();$inputs=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$collision=[ordered]@{};$eventIndex=0
   $schedule=@(
    @{at=1000;name='closed_walk';run={$collision.closedBefore=(S $peer).localPosition;& "$lab/Send-D2Input.ps1" -Role $peer -LeftY 32767 -DurationMs 700|Out-Null}},
    @{at=2500;name='closed_stop';run={I $peer;$collision.closedAfter=(S $peer).localPosition}},
    @{at=3500;name='prepare_source_open';run={C $peer 'setviewpos -44 -161 68.1 220 0';C $source 'setviewpos -47.05 -163.5 68.1 270 0'}},
    @{at=4200;name='native_open';run={I $source X 200;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='native open';attempt=1})}},
    @{at=6500;name='clear_source';run={C $source 'setviewpos -44 -161 68.1 220 0'}},
    @{at=7100;name='prepare_open_walk';run={C $peer 'setviewpos -47.05 -163.0 68.1 270 0'}},
    @{at=7900;name='open_walk';run={if((DoorSample $peer).current-ne2){throw 'Peer door is not natively open before traversal'};$collision.openBefore=(S $peer).localPosition;& "$lab/Send-D2Input.ps1" -Role $peer -LeftY 32767 -DurationMs 1100|Out-Null}},
    @{at=9500;name='open_stop';run={I $peer;$collision.openAfter=(S $peer).localPosition}},
    @{at=10600;name='prepare_source_close';run={C $peer 'setviewpos -44 -161 68.1 220 0';C $source 'setviewpos -47.05 -163.5 68.1 270 0'}},
    @{at=11500;name='native_close';run={I $source X 200;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='native close';attempt=2})}},
    @{at=14000;name='clear_source_again';run={C $source 'setviewpos -44 -161 68.1 220 0'}},
    @{at=14600;name='prepare_reclosed_walk';run={C $peer 'setviewpos -47.05 -163.0 68.1 270 0'}},
    @{at=15400;name='reclosed_walk';run={if((DoorSample $peer).current-ne0){throw 'Peer door is not closed before collision retest'};$collision.reclosedBefore=(S $peer).localPosition;& "$lab/Send-D2Input.ps1" -Role $peer -LeftY 32767 -DurationMs 700|Out-Null}},
    @{at=17100;name='reclosed_stop';run={I $peer;$collision.reclosedAfter=(S $peer).localPosition}}
   )
   while($watch.ElapsedMilliseconds-lt19000){
    if($eventIndex-lt$schedule.Count-and$watch.ElapsedMilliseconds-ge$schedule[$eventIndex].at){$event=$schedule[$eventIndex];& $event.run;$events.Add([pscustomobject]@{name=$event.name;ms=$watch.ElapsedMilliseconds});$eventIndex++}
'''+t[b:]
t=t.replace('syncSource=(DS $source);syncPeer=(DS $peer);api=$api;ms=', 'phase=$eventIndex;syncSource=(DS $source);syncPeer=(DS $peer);api=$api;ms=')
t=t.replace('-or$dp.duplicates-lt2','')
t=t.replace("   $states=@();",'''   $collision|ConvertTo-Json -Depth 5|Set-Content "$out/collision-positions.json";$events|ConvertTo-Json -Depth 5|Set-Content "$out/input-events.json"
   if($eventIndex-ne$schedule.Count){throw 'Traversal schedule incomplete'}
   foreach($phase in 'closed','reclosed'){if($collision[$phase+'After'][1]-lt-164.4-or$collision[$phase+'After'][1]-gt$collision[$phase+'Before'][1]-.1){throw "Expected forward movement stopped in front of $phase door"}}
   if($collision.openAfter[1]-ge-165.0-or[math]::Abs($collision.openAfter[0]-$collision.openBefore[0])-gt.35-or[math]::Abs($collision.openAfter[2]-$collision.openBefore[2])-gt.4){throw 'Open-door traversal did not cross the doorway on the same floor'}
   $checks.Add('Native forward input is blocked by the closed and reclosed peer door and crosses the remotely opened doorway on the same floor')
   $states=@();''')
t=t.replace('duplicate packets do not reapply; no rejected packets during measured actions;','no rejected packets during measured actions;')
t=t.replace('Native Steam sync of one ordinary unlinked unlocked Dunwall door.', 'Native Steam door traversal: receiver native forward movement is blocked before/after and crosses while remotely opened.')
t=t.replace("if($Source-eq'client'-and($ds.requests-ne2-or$dp.requests-ne2))", "if($Source-eq'client'-and($ds.requests-ne2-or$dp.requests-ne2))")
t=t.replace("if($inputs){$inputs|ConvertTo-Json|Set-Content \"$out/partial-inputs.json\"}", "if($inputs){$inputs|ConvertTo-Json|Set-Content \"$out/partial-inputs.json\"};if($collision){$collision|ConvertTo-Json -Depth 5|Set-Content \"$out/partial-collision.json\"};if($events){$events|ConvertTo-Json|Set-Content \"$out/partial-events.json\"}")
(N/'Test-DoorTraversal-v722.ps1').write_text(t,encoding='utf-8')
print('Prepared722 native collision/traversal test with isolated source repositioning, actual receiver input and restoration')
