from pathlib import Path
R=Path(__file__).resolve().parents[2]
s=(R/'work/natural-moveset/Test-DoorCatalog-v729.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
    global s
    assert a in s,a
    s=s.replace(a,b)
rep('native-door-catalog-corvo-v729','native-door-reload-host-v736')
rep('NativeDoorCatalog729','NativeDoorReload736')
rep("function DS($r)", "function RS($r){Get-Content \"$root/work/lab-instances/$r/coop-status.json.reset.json\" -Raw|ConvertFrom-Json}\nfunction DS($r)")
rep("   I $source;I $peer", "   if($initialOpen){throw 'Reload interruption requires the verified closed closet door'}\n   I $source;I $peer")
rep("   foreach($r in 'host','client'){C $r 'coop_door_sync on'}", """   foreach($r in 'host','client'){
    if((RS $r).enabled){C $r 'coop_checkpoint_sync off'}
    C $r 'coop_checkpoint_sync on';C $r $(if($r-eq$peer){'coop_checkpoint_delay 1500'}else{'coop_checkpoint_delay 0'})
   }
   $cw=[Diagnostics.Stopwatch]::StartNew();do{$rh=RS host;$rc=RS client;if($rh.enabled-and$rc.enabled-and$rh.peerArm-and$rc.peerArm-and!$rh.phase-and!$rc.phase){break};Start-Sleep -Milliseconds 50}while($cw.ElapsedMilliseconds-lt5000)
   if(!$rh.enabled-or!$rc.enabled-or!$rh.peerArm-or!$rc.peerArm-or$rh.phase-or$rc.phase-or$rh.digest-ne$rc.digest){throw 'Shared checkpoint pair did not arm'}
   $resetBefore=@{host=$rh;client=$rc};$bridgeBefore=@{host=(S host);client=(S client)}
   $resetBefore|ConvertTo-Json -Depth 8|Set-Content "$out/reset-before.json"
   foreach($r in 'host','client'){C $r 'coop_door_sync on'}""")
rep("'-t','20'", "'-t','40'")
start=s.index('   $monitor=Start-Process ')
end=s.index("   C $peer 'coop_movement_bank off'",start)
s=s[:start]+"""   $inputs=[Collections.Generic.List[object]]::new();$samples=[Collections.Generic.List[object]]::new()
   I $source X 200;$inputs.Add([pscustomobject]@{input='native open';role=$source})
   $moving=[Diagnostics.Stopwatch]::StartNew();do{
    $ns=DoorSample $source;$np=DoorSample $peer
    if($ns.current-eq1-and$np.current-eq1-and(DS $peer).nativeCalls-eq1){break};Start-Sleep -Milliseconds 15
   }while($moving.ElapsedMilliseconds-lt3500)
   [ordered]@{source=$ns;peer=$np;sourceSync=(DS $source);peerSync=(DS $peer)}|ConvertTo-Json -Depth 8|Set-Content "$out/door-at-interruption.json"
   if($ns.current-ne1-or$np.current-ne1){throw 'Both native doors must be Moving at the interruption'}
   C $source 'RestartFromCheckpoint';$inputs.Add([pscustomobject]@{input='RestartFromCheckpoint';role=$source})
   C $peer 'RestartFromMemoryCheckpoint';$inputs.Add([pscustomobject]@{input='duplicate RestartFromMemoryCheckpoint';role=$peer})
   $watch=[Diagnostics.Stopwatch]::StartNew();$lastPress=@{host=0;client=0};$done=$false
   do{
    $pair=@{};$done=$true
    foreach($r in 'host','client'){
     $reset=RS $r;$bridge=S $r;$doors=DAll $r
     $pair[$r]=[pscustomobject]@{reset=$reset;bridge=$bridge;doors=$doors}
     if($reset.phase-eq7){throw "Shared checkpoint failed: $r $($reset.result)"}
     if($reset.completed-ne$resetBefore[$r].completed+1-or$bridge.localGameLogicState-ne1-or$bridge.localPendingGameLogicState){$done=$false}
     if(!$reset.held-and$watch.ElapsedMilliseconds-gt6000-and$bridge.localGameLogicState-in8,15-and$watch.ElapsedMilliseconds-$lastPress[$r]-gt700){I $r A 100;$lastPress[$r]=$watch.ElapsedMilliseconds}
    }
    $samples.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;host=$pair.host;client=$pair.client})
    if(!$done){Start-Sleep -Milliseconds 70}
   }while(!$done-and$watch.ElapsedMilliseconds-lt30000)
   if(!$done){throw 'Shared reload did not finish on both players'}
   Start-Sleep -Milliseconds 800
   $final=@{}
   foreach($r in 'host','client'){
    $reset=RS $r;$bridge=S $r;$doors=DAll $r;$character=Q $r '--character';$entities=Q $r '--entities'
    $final[$r]=[pscustomobject]@{reset=$reset;bridge=$bridge;doors=$doors;character=$character}
    if($reset.loads-ne$resetBefore[$r].loads+1-or$reset.completed-ne$resetBefore[$r].completed+1-or$reset.aborted-ne$resetBefore[$r].aborted-or$reset.maxDistance-gt0.001){throw 'Shared checkpoint did not complete exactly one guarded load'}
    if($character.character-ne$(if($r-eq'host'){1}else{2})-or$character.queued-or$bridge.localEpoch-eq$bridgeBefore[$r].localEpoch){throw 'New world epoch or preserved player role missing'}
    if($doors.enabled-or@($doors.entries|Where-Object enabled).Count-or$doors.result-ne'all_channels_retired'){throw 'Door channels survived the old world scope'}
    foreach($entry in $doors.entries){if($entry.result-ne'world_input_or_connection_scope_changed'){throw 'Unexpected door retirement reason'}}
    $selected=@($doors.entries|Where-Object name -eq $DoorName)[0]
    if($selected.nativeCalls-ne$(if($r-eq$peer){1}else{0})){throw 'Old-world door was applied again during reload'}
    $door=@($entities.entities|Where-Object name -eq $DoorName)
    if($door.Count-ne1){throw 'New-world authored door missing'}
    $cp=@((Q $r '--components' $door[0].handle).components|Where-Object type -eq '.?AVarkCpntOpenable@@')
    if($cp.Count-ne1-or$cp[0].owner-ne$door[0].pointer){throw 'New-world door component owner mismatch'}
    $obj=Q $r '--object' $cp[0].pointer;$obj|ConvertTo-Json -Depth 7|Set-Content "$out/new-world-openable-$r.json"
    if((W $obj '0x48')-ne0){throw 'New checkpoint door inherited the old-world open request'}
    if(!$reset.avatarReady-or$reset.avatarPending-or!$bridge.probeFollowEnabled){throw 'Counterpart presentation not restored'}
   }
   $h=$final.host.reset;$c=$final.client.reset
   if($h.transaction-ne$c.transaction-or$h.hostReady-ne$c.hostReady-or$h.clientReady-ne$c.clientReady-or$h.releasedAt-lt$c.readyAt-or$c.releasedAt-lt$h.readyAt){throw 'Shared reset readiness/transaction mismatch'}
   if(($h.duplicates-$resetBefore.host.duplicates)+($c.duplicates-$resetBefore.client.duplicates)-lt1){throw 'Duplicate checkpoint request was not observed'}
   $final|ConvertTo-Json -Depth 14|Set-Content "$out/reset-final.json"
   $samples|ConvertTo-Json -Depth 15|Set-Content "$out/reload-observations.json"
   $inputs|ConvertTo-Json -Depth 5|Set-Content "$out/inputs.json"
   $checks.Add('Both native doors were Moving when either player requested one shared reload; duplicate peer retry performed no extra load')
   $checks.Add('Both roles and counterparts restored after both-ready hold; old door channels retired; new-world door remains checkpoint-closed with no replayed native queue call')
   foreach($r in 'host','client'){C $r 'coop_checkpoint_sync off';C $r 'coop_checkpoint_delay 0'}
   $capture.WaitForExit(40000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode){throw 'Native capture failed'};$capture=$null
"""+s[end:]
rep("count=$samples.Count;sensors=@($sensors.type)", "count=$samples.Count;sensors=@($sensors.type)")
rep("$checks.Add('Bounded native Steam ordinary-door synchronization with host desired-state ordering; both viewpoints recorded')", "$checks.Add('Both native views recorded across interrupted door motion, shared loading and checkpoint return')")
rep("try{I $r;C $r 'coop_door_sync off'", "try{I $r;C $r 'coop_checkpoint_sync off';C $r 'coop_checkpoint_delay 0';C $r 'coop_door_sync off'")
rep("Native Steam authored ordinary-door catalog: six eligible channels, four excluded doors, selected open/close cycle via native X, other five unchanged. Locks/scripts/other maps remain unverified.", "Shared fixed-SLOT82 checkpoint interrupts active native door motion; one load per peer, both-ready role restoration, old door scope retirement and no old target applied to new-world door. This does not prove automatic door rearming or cross-campaign saves.")
(R/'work/natural-moveset/Test-DoorReload-v736.ps1').write_text(s,encoding='utf-8')
print('Created native door/shared-checkpoint interruption test736')
