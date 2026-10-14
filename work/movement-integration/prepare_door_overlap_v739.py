from pathlib import Path
R=Path(__file__).resolve().parents[2]
s=(R/'work/natural-moveset/Test-DoorCatalog-v729.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
    global s
    assert a in s,a
    s=s.replace(a,b)
rep('native-door-catalog-corvo-v729','native-door-overlap-host-first-v739')
rep('NativeDoorCatalog729','NativeDoorOverlap739')
rep("   I $source;I $peer", "   if($initialOpen){throw 'Overlap test requires the closed closet door'}\n   I $source;I $peer")
rep("C $peer 'setviewpos -44 -161 68.1 220 0'", "C $peer 'setviewpos -46.5 -163.5 68.1 270 0'")
rep('-Role $peer -Pitch 10','-Role $peer -Pitch 0')
rep("    if(!$second-and$watch.ElapsedMilliseconds-ge7000){I $source X 200;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input=$(if($initialOpen){'native open'}else{'native close'});attempt=2});$second=$true}", """    if($sent-and!$second){
     $nativeMoving=DoorSample $peer;$receiving=DS $peer
     if($nativeMoving.current-eq1-and$nativeMoving.pending-eq1-and$receiving.inFlight-and$receiving.nativeCalls-eq1){
      [ordered]@{ms=$watch.ElapsedMilliseconds;native=$nativeMoving;receiving=$receiving;first=(DS $source)}|ConvertTo-Json -Depth 7|Set-Content "$out/native-at-overlap.json"
      I $peer X 200;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='second player uses door during received native motion';role=$peer;attempt=2});$second=$true
     }elseif($watch.ElapsedMilliseconds-gt3500){throw 'Received native moving transition was not available for overlapping input'}
    }""")
rep("if(!$duplicateClose-and$watch.ElapsedMilliseconds-ge10000){C $source", "if(!$duplicateClose-and$watch.ElapsedMilliseconds-ge10000){C $peer")
rep("input=$(if($initialOpen){'native close'}else{'native open'});attempt=1", "input='first player native open';role=$source;attempt=1")
rep('$ds.nativeCalls-ne0-or$dp.nativeCalls-ne2', '$ds.nativeCalls-ne1-or$dp.nativeCalls-ne1')
rep('$dp.duplicates-lt2', '$dp.duplicates-lt1-or$ds.duplicates-lt1')
rep("if(($states-join ',')-ne$expectedCycle){throw \"Native API door cycle differs: $($states-join ',')\"}", "if($states[0]-ne0-or$states[-1]-ne0-or1-notin$states){throw 'First door did not finish closed after overlapping native input'}")
rep("if(($peerStates-join ',')-ne$expectedCycle){throw \"Peer native door cycle differs: $($peerStates-join ',')\"}", "if($peerStates[0]-ne0-or$peerStates[-1]-ne0-or1-notin$peerStates){throw 'Second door did not finish closed after overlapping native input'}")
rep("if($Source-eq'client'-and($ds.requests-ne2-or$dp.requests-ne2)){throw 'Expected two client requests ordered by host'}", "if($ds.requests-ne1-or$dp.requests-ne1){throw 'Expected one client request across the overlapping native input'}")
rep('Two ordinary native X interactions replicate both selected door cycles; exactly two remote native calls; duplicate packets do not reapply; no rejected packets during measured actions; both finish host revision3', 'Native second-player input interrupts an in-flight received door transition; both remain enabled and converge closed at host revision3, one receiver API call each, one client request, no replay application or measured rejection delta')
rep('Native Steam authored ordinary-door catalog: six eligible channels, four excluded doors, selected open/close cycle via native X, other five unchanged. Locks/scripts/other maps remain unverified.', 'Overlapping native use of an ordinary door: second player presses X during a received Moving transition. Host orders the newer closed target; native animation handles reversal; both converge without channel retirement. Success is not assumed; inspect result and native-at-overlap.')
(R/'work/natural-moveset/Test-DoorOverlap-v739.ps1').write_text(s,encoding='utf-8')
print('Created overlapping-native-use regression harness739')
