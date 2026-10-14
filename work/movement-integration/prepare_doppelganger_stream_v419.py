from pathlib import Path
import json,hashlib,shutil,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';L=R/'outputs/d2-coop-lab';A=W/'candidate-doppelganger-stream-v418'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert rd(N/'doppelganger-world-ownership-v417/result.json')['passed'];assert not A.exists()
logs=[W/f'doppelganger-stream-{k}-v418.log' for k in ('build','world-tests','action-tests','bank-tests','smoke')]
for p in logs:
    t=p.read_text(encoding='utf-8-sig');assert t and not any(s in t for s in ('error C','error LNK','FAIL:','Traceback')),p
A.mkdir();shutil.copytree(L/'src',A/'src');shutil.copytree(W/'candidate-doppelganger-binding-v407/banks',A/'banks')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in [Path(__file__),*logs,W/'Build-DoppelgangerStream-v418.ps1',W/'wire_doppelganger_stream_v418.py']:shutil.copy2(p,A/p.name)
x=rd(W/'candidate-doppelganger-binding-v407/identity.json')
x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,doppelgangerWorldProtocol=1,doppelgangerWorldBytes=176,banks=[dict(path=str(p),sha256=sha(p)) for p in (A/'banks').glob('*.bin')],scope='Candidate418 adds cached source clone identity/lifetime/root transport over native Steam, with strict epoch/incarnation ordering and buffered reception. No peer clone, pose, effects or AI spawned. Existing cast bank55 and semantic protocol52 unchanged. Late join with unequipped active clone is pending.',previousVerified=str(W/'candidate-doppelganger-binding-v407'));x.pop('runtimeVerificationScope',None)
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
s=(N/'Test-DoppelgangerNetwork-v411.ps1').read_text(encoding='utf-8-sig')
s=s.replace("'-t','16'","'-t','34'").replace('$w.ElapsedMilliseconds-lt14000','$w.ElapsedMilliseconds-lt32000')
anchor='player=(Q client \'--player-states\');movement=$m;stream=$stream}'
assert s.count(anchor)==1
s=s.replace(anchor,"player=(Q client '--player-states');movement=$m;stream=$stream;cloneSource=(S client '.doppelganger-world.json');clonePeer=(S host '.doppelganger-world.json')}")
anchor=" $checks.Add('Native source clone observed during its lifetime; body playback creates no peer clones or duplicate power actions')"
assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''
 $sent=@($rows|ForEach-Object {$_.cloneSource.history}|Sort-Object sequence -Unique)
 $accepted=@($rows|ForEach-Object {$_.clonePeer.history}|Sort-Object sequence -Unique)
 $sentBySeq=@{};foreach($record in $sent){$sentBySeq[[string]$record.sequence]=$record}
 $live=@($accepted|Where-Object present -eq 1);$cleared=@($accepted|Where-Object present -eq 0)
 if($live.Count-lt20-or!$cleared.Count){throw 'Clone transport lacks20 live frames or authoritative clear'}
 foreach($record in $accepted){$source=$sentBySeq[[string]$record.sequence];if(!$source-or$record.digest-ne$source.digest){throw 'Accepted clone payload has no exact sender digest'}}
 $nativeHandles=@($worldSamples.source.handle|ForEach-Object {[Convert]::ToUInt32($_,16)}|Sort-Object -Unique)
 if(@($live.handle|Sort-Object -Unique).Count-ne1-or[uint32]$live[0].handle-notin$nativeHandles){throw 'Transport clone differs from separately enumerated native clone'}
 if($cleared[-1].event-le$live[-1].event-or$rows[-1].clonePeer.present-or$rows[-1].clonePeer.leased){throw 'Clone retirement did not clear receiver lease'}
 [ordered]@{sent=$sent.Count;accepted=$accepted.Count;live=$live.Count;clear=$cleared.Count;nativeHandles=$nativeHandles;generations=@($live.generation|Sort-Object -Unique);allAcceptedDigestsMatch=$true;sourceReadMaxMs=($rows.cloneSource.readMaxMs|Measure-Object -Maximum).Maximum;receiverRejected=@($rows.clonePeer.rejected|Sort-Object -Unique);scope='Native Steam clone identity, position and lifetime transport only. Receiver creates no model, effect, AI or damage; source handles remain opaque.'}|ConvertTo-Json -Depth 10|Set-Content "$out/clone-transport-verification.json"
 $checks.Add('Native clone identity/root/lifetime traverses Steam with exact accepted packet digests and authoritative receiver retirement')
''')
s=s.replace('Remote world/clone behavior, alternate contexts and visual acceptance remain pending.','Clone identity/root/lifetime Steam transport verified separately; remote model, VFX/gameplay, alternate contexts and visual acceptance remain pending.')
(N/'Test-DoppelgangerStream-v419.ps1').write_text(s,encoding='utf-8')
t=(W/'Run-DoppelgangerNetwork-v411.ps1').read_text(encoding='utf-8-sig').replace('doppelganger-network-v411','doppelganger-stream-v419').replace('Test-DoppelgangerNetwork-v411','Test-DoppelgangerStream-v419')
(W/'Run-DoppelgangerStream-v419.ps1').write_text(t,encoding='utf-8')
print(json.dumps(dict(dll=x['dll'],probe=x['probe'],candidate=str(A))))
