from pathlib import Path
import json,hashlib,shutil,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-shadowwalk-probe-v519'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert rd(W/'native-operation-owner.json')['state']=='finished'
assert not A.exists()
live=rd(W/'candidate-shadowwalk-inventory-v515/identity.json')['dll']
assert sha(R/'work/asset-game/dinput8.dll')==live
for f in ['shadowwalk-state-build-v519.log','shadowwalk-state-world-tests-v519.log']:
    t=(W/f).read_text(encoding='utf-8-sig');assert t and not any(x in t for x in ['error C','error LNK','FAIL:'])
A.mkdir();shutil.copytree(L/'src',A/'src')
shutil.copy2(R/'work/build/Release/d2_runtime_probe.exe',A/'d2_runtime_probe.exe')
for p in [Path(__file__),W/'Build-ShadowWalkState-v519.ps1',W/'prepare_shadowwalk_state_v519.py',W/'prepare_shadowwalk_watch_v520.py',W/'shadowwalk-state-build-v519.log',W/'shadowwalk-state-world-tests-v519.log']:
    shutil.copy2(p,A/p.name)
identity=dict(at=datetime.datetime.now().astimezone().isoformat(),liveDll=live,probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Read-only explicit Shadow Walk state/mode/phase and filtered FX observation. No change to live515 proxy, packets, banks or assets. Native runtime verification pending520.')
(A/'identity.json').write_text(json.dumps(identity,indent=2),encoding='utf-8')
shutil.copy2(A/'d2_runtime_probe.exe',L/'bin/d2_runtime_probe.exe')
print(json.dumps(identity))
