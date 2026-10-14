from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-DoorOverlap-v751.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
    global s
    assert a in s,a
    s=s.replace(a,b)
rep('native-door-close-handoff-host-v751','native-door-busy-use-host-v752')
rep('NativeDoorCloseHandoff751','NativeDoorBusyUse752')
rep('$sent=$false;$second=$false;', '$sent=$false;$second=$false;$third=$false;')
rep("    if(!$duplicateOpen-and$watch.ElapsedMilliseconds-ge4000)","""    if($second-and!$third-and$watch.ElapsedMilliseconds-ge3500){
     $readySource=DS $source;$readyPeer=DS $peer;$nativeSource=DoorSample $source;$nativePeer=DoorSample $peer
     [ordered]@{source=$readySource;peer=$readyPeer;nativeSource=$nativeSource;nativePeer=$nativePeer}|ConvertTo-Json -Depth 8|Set-Content "$out/after-busy-use.json"
     if(!$readySource.enabled-or!$readyPeer.enabled-or$readySource.revision-ne2-or$readyPeer.revision-ne2-or$readySource.applied-ne2-or$readyPeer.applied-ne2-or$readySource.nativeCalls-ne0-or$readyPeer.nativeCalls-ne1-or$readySource.requests-or$readyPeer.requests-or$nativeSource.current-ne0-or$nativePeer.current-ne0){throw 'Input while closing changed ordered native operation'}
     I $peer X 200;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='second player native reopen after closing completed';role=$peer;attempt=3});$third=$true
    }
    if(!$duplicateOpen-and$watch.ElapsedMilliseconds-ge6000)""")
rep('$readySource.requests-or$readyPeer.requests',"$readySource.requests-ne$(if($source-eq'client'){1}else{0})-or$readyPeer.requests-ne$(if($source-eq'client'){1}else{0})")
rep('$inputs.Count-ne2','$inputs.Count-ne3')
rep('Native second-player input interrupts an in-flight received door transition;', 'Native second-player input while closing leaves the existing operation unchanged; a subsequent input after completion reopens normally;')
rep('Overlapping native use of an ordinary door: second player presses X during a received Moving transition. Host orders the newer open target; native animation handles reversal; both converge without channel retirement. Success is not assumed; inspect result and native-at-overlap.', 'Native input during received closing is not accepted as a reversal; both finish the same ordered close. The other player then uses the completed door and both reopen at revision3. Explicit input/state observations; no assumed busy-input buffering or reversal.')
(N/'Test-DoorBusy-v752.ps1').write_text(s)
print('Prepared752 native busy input followed by accepted stable-state use')
