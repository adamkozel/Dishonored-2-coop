from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-DoorOverlap-v748.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
    global s
    assert a in s,a
    s=s.replace(a,b)
rep('native-door-overlap-opposite-host-v748','native-door-simultaneous-sides-v754')
rep('NativeDoorOverlap748','NativeDoorSimultaneous754')
rep("movingTarget=('0x{0:x}'-f(W $o '0xb0'));", "movingTarget=('0x{0:x}'-f(W $o '0xb0'));openSide=[int]((W $o '0xc8')-band 255);")
rep("I $source X 200;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='first player native open';role=$source;attempt=1});$sent=$true", "I $source X 200;I $peer X 200;$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='simultaneous native open';role=$source;attempt=1});$inputs.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;input='simultaneous opposite-side native open';role=$peer;attempt=2});$sent=$true")
start=s.index('    if($sent-and!$second){');end=s.index('    if(!$duplicateOpen',start)
s=s[:start]+s[end:]
start=s.index('   if(!$ds.enabled-or!$dp.enabled-or$ds.nativeCalls');end=s.index('   $catalogAfter=',start)
s=s[:start]+"""   $nativeFinal=@{source=(DoorSample $source);peer=(DoorSample $peer)};$nativeFinal|ConvertTo-Json -Depth 7|Set-Content "$out/native-final-sides.json"
   $competing=@($samples|Where-Object {$_.doorSource.current-eq1-and$_.doorPeer.current-eq1-and(([Convert]::ToUInt64($_.doorSource.state90.Substring(2),16)-shr16)-band255)-ne(([Convert]::ToUInt64($_.doorPeer.state90.Substring(2),16)-shr16)-band255)})
   $competing|Select-Object -First 3 ms,doorSource,doorPeer,syncSource,syncPeer|ConvertTo-Json -Depth 8|Set-Content "$out/competing-native-starts.json"
   if(!$competing.Count){throw 'Both opposite-side native openings were not observed; simultaneous coverage is inconclusive'}
   if(!$ds.enabled-or!$dp.enabled-or$ds.revision-ne$dp.revision-or$ds.applied-ne$ds.revision-or$dp.applied-ne$dp.revision-or$ds.target-ne2-or$dp.target-ne2-or$ds.side-ne$dp.side-or$ds.rejected-ne$samples[0].syncSource.rejected-or$dp.rejected-ne$samples[0].syncPeer.rejected){throw 'Simultaneous authored door order did not converge'}
   if($nativeFinal.source.current-ne2-or$nativeFinal.peer.current-ne2-or$nativeFinal.source.openSide-ne$ds.side-or$nativeFinal.peer.openSide-ne$dp.side){throw 'Authoritative door side differs from actual native open-side geometry'}
"""+s[end:]
rep('Native second-player input interrupts an in-flight received door transition; both remain enabled and converge closed at host revision3, one receiver API call each, one client request, no replay application or measured rejection delta','Both native opposite-side openings observed; one host order converges in state and actual native opening side, with no measured rejection delta')
rep('Overlapping native use of an ordinary door: second player presses X during a received Moving transition. Host orders the newer closed target; native animation handles reversal; both converge without channel retirement. Success is not assumed; inspect result and native-at-overlap.', 'Simultaneous native openings from opposite sides. Both independent native Moving parameters must be observed, then both actual Open sides must match the host order. No assumed same-side convergence.')
(N/'Test-DoorSimultaneous-v754.ps1').write_text(s)
print('Prepared754 opposite-side simultaneous native opening and geometry assertions')
