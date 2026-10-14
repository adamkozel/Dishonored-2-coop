from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Test-WindblastContexts.ps1').read_text(encoding='utf-8-sig')
s=s.replace("$Take='windblast-corvo-context-v28-v1'","$Take='windblast-corvo-network-v28-v1'")
s=s.replace("$checks.Add('Native Steam stream", " $fxBefore=S $peer '.power-fx.json';$sourceFxBefore=S $Source '.windblast-fx.json';$peerFxBefore=S $peer '.windblast-fx.json';$nativeEffects=@{};$remoteEffects=@{};$effectMatches=0;$effectIds=@{}\n $checks.Add('Native Steam stream")
s=s.replace("$peerPower=Q $peer '--player-darkvision'", "$peerPower=Q $peer '--player-darkvision';$sourceFx=S $Source '.windblast-fx.json';$peerFx=S $peer '.windblast-fx.json';$fx=S $peer '.power-fx.json';$nativeFx=Q $Source '--windblast-effects';$remoteFx=Q $peer '--windblast-effects'\n  foreach($e in $nativeFx.effects){if($e.name-eq'models/effects/gameplay/powers/windblast/blast/wblast_blast_01.fx'-and$e.parent-eq1){$nativeEffects[[string]$e.id]=$e}}\n  foreach($e in $remoteFx.effects){if($e.name-eq'models/effects/d2coop/windblast_remote.fx'){if($e.owner-ne$handles[$peer]){throw 'Remote native FX owner changed'};$remoteEffects[[string]$e.id]=$e;$effectIds[[string]$e.id]=$true}else{throw 'Observer created an unrequested gameplay Windblast effect'}}\n  foreach($e in $fx.effects){if($e.kind-ne8-or$e.owner-ne$handles[$peer]-or$e.record-eq'0x0'){throw 'Wrong effect kind or owner'}}")
s=s.replace('peer=$peerPower})','peer=$peerPower;sourceFx=$sourceFx;peerFx=$peerFx;fx=$fx;nativeFx=$nativeFx;remoteFx=$remoteFx})')
needle=" I $Source;C $Source 'coop_windblast_fixture off'"
assert needle in s
s=s.replace(needle,"""
 if(($sourceFx.sent-$sourceFxBefore.sent)-ne2-or($peerFx.applied-$peerFxBefore.applied)-ne2-or($fx.started-$fxBefore.started)-ne2-or$fx.active-or$effectIds.Count-ne2){throw "Expected exactly two remote cosmetic effects: $($sourceFx|ConvertTo-Json -Compress) / $($peerFx|ConvertTo-Json -Compress)"}
 $checks.Add('Exactly two uniquely identified native source effects each create one finite remote world effect through Steam')
 foreach($e in $remoteEffects.Values){$matched=$false;foreach($n in $nativeEffects.Values){$d=0;$a=0;for($k=0;$k-lt3;$k++){$d+=[Math]::Pow(($e.origin[$k]-$n.origin[$k]),2)};for($k=0;$k-lt9;$k++){$a=[Math]::Max($a,[Math]::Abs($e.axis[$k]-$n.axis[$k]))};if($d-lt.000001-and$a-lt.00001){$matched=$true;break}};if(!$matched){throw 'Remote FX does not preserve an independently read native origin/axis'};$effectMatches++}
 if($effectMatches-ne2){throw 'Insufficient independent native FX matches'};$checks.Add('Both clone-owned remote effects preserve the exact independently read native source origin and orientation')
 $replayBefore=S $peer '.windblast-fx.json';C $Source 'coop_power_fx windblast_duplicate';Start-Sleep -Milliseconds 400;$replayAfter=S $peer '.windblast-fx.json'
 if($replayAfter.received-ne($replayBefore.received+1)-or$replayAfter.rejected-ne($replayBefore.rejected+1)-or$replayAfter.applied-ne$replayBefore.applied){throw 'Duplicate FX packet was not rejected'}
 [ordered]@{before=$replayBefore;after=$replayAfter}|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/replay.json";$checks.Add('Replayed reliable packet is received and rejected without a second effect')
 I $Source;C $Source 'coop_windblast_fixture off'""")
s=s.replace('recorded=($recorders.Count-eq2);scope=', 'recorded=($recorders.Count-eq2);matchedNativeEffects=$effectMatches;remoteEffectIds=@($effectIds.Keys);scope=')
s=s.replace('Native source effects and physics are recorded; remote world VFX, impacts, other contexts and broader lifecycle are not yet implemented or certified.', 'Observed basic native cast FX drives one separate world-space presentation effect on the remote clone, preserving native position/axis and body timing. Original private screen distortion is excluded. Replay, finite cleanup and health are checked. Impacts, world physics outcomes, upgrades and broader lifecycle are not certified; visual quality requires separate review.')
(P/'Test-WindblastNetwork.ps1').write_text(s,encoding='utf-8')
