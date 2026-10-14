from pathlib import Path
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'
s=(N/'Observe-NativeSwarm-v265.ps1').read_text(encoding='utf-8-sig')
s=s.replace('Swarm','Doppelganger').replace('swarm','doppelganger').replace('Corvo','Emily')
s=s.replace('Q host','Q client').replace('C host','C client').replace('I host','I client').replace('S host','S client').replace('-Role host','-Role client').replace('$pids.host','$pids.client').replace("source='host'","source='client'")
s=s.replace('devouring_doppelganger/devouring_doppelganger.inventoryitem','doppelganger/doppelganger.inventoryitem')
s=s.replace('power_doppelganger_cast','power_decoy_cast')
s=s.replace("'coop_doppelganger_fixture off','coop_movement_fixture off'","'coop_swarm_fixture off','coop_possession_fixture off','coop_doppelganger_fixture off','coop_movement_fixture off'")
s=s.replace(" $dirty=$true\n", " $dirty=$true\n C host 'setviewpos -34.3 -154.8 68.1 90 0'\n")
key="function Q($r,$mode,$arg=0)"
a=s.index(key);b=s.index('\n',a)
s=s[:b]+'''\nfunction OptionalFx($r){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --doppelganger-effects;$code=$LASTEXITCODE;if($code){return [pscustomobject]@{stable=$false;exitCode=$code}};return($raw|ConvertFrom-Json)}'''+s[b:]
s=s.replace(" $before=Q client '--player-doppelganger';", " $baselineEntities=Q client '--entities';$baselineEntities|ConvertTo-Json -Depth 8|Set-Content \"$out/entities-before.json\"\n $before=Q client '--player-doppelganger';")
s=s.replace("component=(Q client '--object' $power.component);entities=", "component=(Q client '--object' $power.component);effects=(OptionalFx client);entities=")
s=s.replace("'(?i)(devouring_doppelganger|rat_doppelganger)'", "'(?i)(doppelganger|decoy_entity|deadlyshadow)'")
old="  $entities=Q client '--entities';if(@($entities.entities|Where-Object {$_.definitionName-match'^models/characters/animals/rat/devouring_doppelganger(_big)?\\.def$'}).Count){throw 'Spawned doppelganger remains after checkpoint'}"
assert old in s
s=s.replace(old,"  $entities=Q client '--entities';$entities|ConvertTo-Json -Depth 8|Set-Content \"$out/entities-restored.json\"\n  $restoredClones=@($entities.entities|Where-Object {$_.definitionName-match'^models/powers/doppelganger/(decoy_entity|deadlyshadow_entity)(|_corvo)\\.def$'-and$_.handle-ne(S client).probeActorHandle});if($restoredClones.Count){throw 'Unexpected native clone remains after checkpoint'}")
assert 'devouring' not in s and 'rat_' not in s
p=N/'Observe-NativeDoppelganger-v406.ps1';assert not p.exists();p.write_text(s,encoding='utf-8')
wrapper='''param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-doppelganger-source-v406')
$ErrorActionPreference='Stop'
$mutex=[Threading.Mutex]::new($false,'Local\\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true}
 if(!$owned){throw 'Native operation still active'}
 [ordered]@{pid=$PID;operation='DoppelgangerObserve';take=$Take;started=(Get-Date).ToString('o');state='running'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json"
 & "$PSScriptRoot/../natural-moveset/Observe-NativeDoppelganger-v406.ps1" -Take $Take
 if(!$?){throw 'Native observation failed'}
}finally{
 if($owned){[ordered]@{pid=$PID;operation='DoppelgangerObserve';take=$Take;ended=(Get-Date).ToString('o');state='finished'}|ConvertTo-Json|Set-Content "$PSScriptRoot/native-operation-owner.json";$mutex.ReleaseMutex()};$mutex.Dispose()
}
'''
(W/'Run-DoppelgangerObserve-v406.ps1').write_text(wrapper,encoding='utf-8')
print('Prepared isolated source cast recording406 and serialized cleanup wrapper')
