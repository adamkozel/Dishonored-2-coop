from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
t=(N/'Test-NativeDoorApi-v714.ps1').read_text(encoding='utf-8-sig').replace('v714','v717').replace('NativeDoorApi714','NativeDoorSync717').replace('native-door-api-corvo','native-door-sync-corvo').replace('candidate-door-api-v713','candidate-door-sync-v716').replace('archived713','archived716')
t=t.replace("function S($r)","function DS($r){Get-Content \"$root/work/lab-instances/$r/coop-status.json.door-sync.json\" -Raw|ConvertFrom-Json}\nfunction S($r)")
t=t.replace("   $key=\"$source-$($case.name)\";",'''   foreach($r in 'host','client'){C $r 'coop_door_sync on'}
   $dw=[Diagnostics.Stopwatch]::StartNew();do{$dh=DS host;$dc=DS client;if($dh.ready-and$dc.ready-and$dh.revision-eq1-and$dc.revision-eq1-and$dh.applied-eq1-and$dc.applied-eq1){break};Start-Sleep -Milliseconds 40}while($dw.ElapsedMilliseconds-lt6000)
   if(!$dh.ready-or!$dc.ready-or$dh.revision-ne1-or$dc.revision-ne1){throw 'Door sync pair not armed'}
   $key="$source-$($case.name)";''')
t=t.replace("C $source 'coop_door_fixture open';$inputs", "I $source X 200;$inputs").replace("C $source 'coop_door_fixture close';$inputs", "I $source X 200;$inputs")
t=t.replace("C $source 'coop_door_fixture open';$duplicateOpen", "C $source 'coop_door_sync duplicate';$duplicateOpen").replace("C $source 'coop_door_fixture close';$duplicateClose", "C $source 'coop_door_sync duplicate';$duplicateClose")
t=t.replace('api=$api;ms=', 'syncSource=(DS $source);syncPeer=(DS $peer);api=$api;ms=')
start=t.index('   $api=Get-Content',t.index("   if($inputs.Count-ne2"));end=t.index("   $states=@();",start)
t=t[:start]+'''   $ds=DS $source;$dp=DS $peer
   [ordered]@{source=$ds;peer=$dp}|ConvertTo-Json -Depth 6|Set-Content "$out/native-sync-final.json"
   if(!$ds.enabled-or!$dp.enabled-or$ds.nativeCalls-ne0-or$dp.nativeCalls-ne2-or$ds.revision-ne3-or$dp.revision-ne3-or$ds.applied-ne3-or$dp.applied-ne3-or$dp.duplicates-lt2-or$ds.rejected-or$dp.rejected){throw 'Native Steam door ordering, application or duplicate counters differ'}
'''+t[end:]
t=t.replace("   if(@($samples|Where-Object {$_.doorPeer.current-ne0}).Count){throw 'Local API experiment unexpectedly altered peer world'}",'''   $peerStates=@();foreach($x in $samples){$v=$x.doorPeer.current;if(!$peerStates.Count-or$peerStates[-1]-ne$v){$peerStates+=$v}}
   if(($peerStates-join ',')-ne'0,1,2,1,0'){throw "Peer native door cycle differs: $($peerStates-join ',')"}
   if($Source-eq'client'-and($ds.requests-ne2-or$dp.requests-ne2)){throw 'Expected two client requests ordered by host'}''')
t=t.replace('Two explicit native transitions produce0->1->2->1->0; duplicate open/close are no-ops; peer remains unchanged','Two ordinary native X interactions replicate both0->1->2->1->0 door cycles; exactly two remote native calls; duplicate packets do not reapply; both finish host revision3')
t=t.replace('Native Openable API proof only; door state/animation/event machinery runs in engine, no network synchronization yet','Bounded native Steam ordinary-door synchronization with host desired-state ordering; both viewpoints recorded')
t=t.replace("I $r;C $r 'coop_movement_bank off'","I $r;C $r 'coop_door_sync off';C $r 'coop_movement_bank off'")
t=t.replace("scope='Local native Openable74a400 request proof with ordinary native open/close parameters and component resume. Exact identity, unlinked/unlocked, stable state guards. Both game views recorded; no shared door networking yet.'", "scope='Native Steam sync of one ordinary unlinked unlocked Dunwall door. Source uses real native X inputs; receiver uses74a400 queue. Both engine state cycles and host revision/duplicate counters verified; locks/scripts/collision/campaign-wide doors remain unverified.'")
(N/'Test-NativeDoorSync-v717.ps1').write_text(t,encoding='utf-8')
print('Prepared717 ordinary-input Steam door synchronization test')
