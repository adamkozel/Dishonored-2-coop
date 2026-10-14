from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-DoorReload-v744.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
    global s
    assert a in s,a
    s=s.replace(a,b)
rep('native-door-retry-probe-client-v744','native-door-session-reload-client-v764')
rep('NativeDoorRetryProbe744','NativeDoorSessionReload764')
rep('candidate-checkpoint-retry-v742','candidate-door-session-v763');rep('archived742','archived763')
rep("C $r 'coop_door_sync on'","C $r 'coop_door_sync session'")
start=s.index("   foreach($r in 'host','client'){\n    C $r 'coop_checkpoint_sync retry_probe'")
end=s.index('   $key=',start)
s=s[:start]+s[end:]
line="    if($reset.retryProbeInjected-$resetBefore[$r].retryProbeInjected-ne2-or$reset.continuationDeferred-$resetBefore[$r].continuationDeferred-lt1-or$reset.continuationAccepted-$resetBefore[$r].continuationAccepted-ne1){throw 'Native deferred continuation path was not exercised exactly once to acceptance'}\n"
rep(line,'')
rep("   $final=@{}", """   $renewWatch=[Diagnostics.Stopwatch]::StartNew()
   do{
    $renewH=DAll host;$renewC=DAll client
    $renewed=(CatalogReady $renewH)-and(CatalogReady $renewC)-and$renewH.session.rearms-eq1-and$renewC.session.rearms-eq1
    if(!$renewed){Start-Sleep -Milliseconds 50}
   }while(!$renewed-and$renewWatch.ElapsedMilliseconds-lt6000)
   if(!$renewed){throw 'Fresh door catalogs did not renew automatically'}
   $final=@{}""")
start=s.index("    if($doors.enabled-or@($doors.entries|Where-Object enabled).Count")
end=s.index('    $door=@($entities.entities',start)
s=s[:start]+"""    if(!(CatalogReady $doors)-or!$doors.session.enabled-or$doors.session.reloadPending-or$doors.session.reloads-ne1-or$doors.session.rearms-ne1-or$doors.session.retiredChannels-ne6){throw 'Expected exactly one automatic fresh catalog renewal'}
    if($doors.session.retiredNativeCalls-ne$(if($r-eq$peer){1}else{0})){throw 'Old-world native call count changed during retirement'}
    foreach($old in $catalogBefore[$r].entries){
     $new=@($doors.entries|Where-Object name -eq $old.name)
     if($new.Count-ne1-or$new[0].arm-le$old.arm-or$new[0].nativeCalls-ne0){throw 'New-world binding reused an arm or replayed a native request'}
    }
    if($doors.session.epoch-ne$bridge.localEpoch-or$doors.session.peerEpoch-ne$bridge.remoteEpoch){throw 'Fresh catalog epochs differ from the ready pair'}
    $isolation=Get-Content "$root/work/lab-instances/$r/coop-status.json.observer-isolation.json" -Raw|ConvertFrom-Json
    $isolation|ConvertTo-Json -Depth 6|Set-Content "$out/postload-isolation-$r.json"
    if($isolation.active-or$isolation.heldReferences){throw 'Door renewal unexpectedly retained AI isolation'}
    $npcs=Q $r '--world-npcs';$ramsey=@($npcs.npcs|Where-Object spawnerName -eq 'ld_spawner_ramsey_escape')
    if($ramsey.Count-ne1){throw 'Native Ramsey identity unavailable'}
    $decision=Q $r '--ai-decision' $ramsey[0].handle;$decision|ConvertTo-Json -Depth 7|Set-Content "$out/postload-native-ai-$r.json"
    if(!$decision.started-or!$decision.schedulerActive-or$decision.pauseReferences){throw 'Native AI did not resume after checkpoint load'}
"""+s[end:]
anchor="    if((W $obj '0x48')-ne0){throw 'New checkpoint door inherited the old-world open request'}"
rep(anchor,anchor+"\n    $doorBindings[$r]=[pscustomobject]@{handle=$door[0].handle;component=$cp[0].pointer;owner=[Convert]::ToUInt64($door[0].pointer.Substring(2),16);generation=((W $obj '0x18')-band [uint64]4294967295)}")
anchor="   $final|ConvertTo-Json -Depth 14|Set-Content \"$out/reset-final.json\""
rep(anchor,anchor+"""
   # No second door-enable command: use the automatically rebuilt channels.
   C $peer 'setviewpos -44 -161 68.1 220 0';C $source 'setviewpos -47.05 -163.5 68.1 270 0'
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $source -Pitch 0 -Evidence "$out/postload-source-camera.json" -Quick
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 10 -Evidence "$out/postload-peer-camera.json" -Quick
   I $source X 200;$inputs.Add([pscustomobject]@{input='fresh native open after automatic renewal';role=$source})
   $useWatch=[Diagnostics.Stopwatch]::StartNew();$afterUse=$null
   do{
    $ns=DoorSample $source;$np=DoorSample $peer;$ds=DS $source;$dp=DS $peer
    $afterUse=@{nativeSource=$ns;nativePeer=$np;source=$ds;peer=$dp}
    $used=$ns.current-eq2-and$np.current-eq2-and$ds.revision-eq2-and$dp.revision-eq2-and$ds.applied-eq2-and$dp.applied-eq2
    if(!$used){Start-Sleep -Milliseconds 65}
   }while(!$used-and$useWatch.ElapsedMilliseconds-lt5000)
   $afterUse|ConvertTo-Json -Depth 10|Set-Content "$out/postload-door-use.json"
   if(!$used-or!$ds.enabled-or!$dp.enabled-or$ds.nativeCalls-ne0-or$dp.nativeCalls-ne1){throw 'Fresh native use did not synchronize after automatic catalog renewal'}
""")
rep('Both roles and counterparts restored after both-ready hold; old door channels retired; new-world door remains checkpoint-closed with no replayed native queue call','Both roles and counterparts restored after both-ready hold; six fresh door channels renew without manual setup; native AI resumes; checkpoint-closed state has no replay, then a fresh use synchronizes')
rep('This does not prove automatic door rearming or cross-campaign saves.','Automatic same-map door rearming and a fresh native use are verified, with active native AI after load. Cross-campaign saves and general world authority remain unverified.')
(N/'Test-DoorRearm-v764.ps1').write_text(s)
print('Prepared764 same-map shared reload, automatic new door catalog, active native AI and fresh native use')
