from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';L=R/'outputs/d2-coop-lab';A=W/'candidate-doppelganger-world-probe-v416'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
base=W/'candidate-doppelganger-binding-v407';x=json.loads((base/'identity.json').read_text(encoding='utf-8-sig'))
assert x['runtimeVerified'] and sha(R/'work/asset-game/dinput8.dll')==x['dll']
assert not A.exists(); A.mkdir()
for name in ('doppelganger-world-build-v416.log','doppelganger-world-tests-v416.log'):
    t=(W/name).read_text(encoding='utf-8-sig');assert t and 'FAIL:' not in t and 'error C' not in t;shutil.copy2(W/name,A/name)
shutil.copytree(L/'src',A/'src');shutil.copytree(base/'banks',A/'banks')
shutil.copy2(base/'dinput8.dll',A/'dinput8.dll');shutil.copy2(R/'work/build/Release/d2_runtime_probe.exe',A/'d2_runtime_probe.exe')
x.update(created=datetime.datetime.now().astimezone().isoformat(),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Read-only probe416: cached owner revalidation and explicit registry retirement. DLL407 remains unchanged. Failed reads remain unknown. Native clone lifetime cause and remote presentation not implied.',previousVerified=str(base),banks=[dict(path=str(p),sha256=sha(p)) for p in (A/'banks').glob('*.bin')]);x.pop('runtimeVerificationScope',None)
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
for name in ('Build-DoppelgangerWorldProbe-v416.ps1','implement_doppelganger_world_v416.py','doppelganger-world-tests-v416-first-test-failure.log'):shutil.copy2(W/name,A/name)
shutil.copy2(A/'d2_runtime_probe.exe',L/'bin/d2_runtime_probe.exe')
s=(N/'Observe-DoppelgangerWorld-v415.ps1').read_text(encoding='utf-8-sig')
s=s.replace("'-t','16'","'-t','34'")
s=s.replace(" C client 'coop_doppelganger_fixture off';$w.Restart()", " [ordered]@{tick=[Environment]::TickCount64;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000}|ConvertTo-Json|Set-Content \"$out/fixture-off-time.json\"\n C client 'coop_doppelganger_fixture off';$w.Restart()")
old=" if(!$emptyAfter.Count){throw 'Native clone absence after fixture retirement not observed'}"
assert old in s
s=s.replace(old," if(!$emptyAfter.Count){throw 'No authoritative clone absence observed; invalid reads are not absence'}")
s=s.replace('states=@($live.stateCandidate|Sort-Object -Unique);','rawA0=@($live.rawA0|Sort-Object -Unique);retiredSamples=@($emptyAfter|Where-Object retired).Count;cachedPresentAverageMs=(@($live|Where-Object cached)|Measure-Object readMs -Average).Average;')
s=s.replace('Raw decoy state has no inferred meaning; expiry versus fixture retirement is not conflated.','Registry retirement proves the tracked incarnation is absent. Read failure remains unknown. Raw component+a0 has no inferred meaning; retirement cause is not inferred.')
s=s.replace('then report absence after fixture cleanup','then report authoritative registry absence; no cause of retirement inferred')
anchor="  $checks.Add('Both isolated checkpoints restore original health/inventory and retire spawned Doppelganger entities')"
assert s.count(anchor)==1
s=s.replace(anchor,"  $worldRestored=Q client '--doppelganger-world';$worldRestored|ConvertTo-Json -Depth 8|Set-Content \"$out/world-restored.json\"\n  if(!$worldRestored.valid-or$worldRestored.present){throw 'World observer does not confirm checkpoint absence'}\n"+anchor)
s=s.replace('AI held in the isolated Dunwall fixture','Existing guards held in the isolated Dunwall fixture; newly summoned clone retains native AI')
s=s.replace('Raw scene clocks/state/equipment','Raw scene clocks/equipment')
(N/'Observe-DoppelgangerWorld-v417.ps1').write_text(s,encoding='utf-8')
s=(W/'Run-DoppelgangerWorld-v415.ps1').read_text(encoding='utf-8-sig').replace('v415','v417')
(W/'Run-DoppelgangerWorld-v417.ps1').write_text(s,encoding='utf-8')
print(json.dumps(dict(probe=x['probe'],dll=x['dll'],archive=str(A),restart=False)))
