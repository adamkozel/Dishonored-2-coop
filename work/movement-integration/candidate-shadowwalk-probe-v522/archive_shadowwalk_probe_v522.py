from pathlib import Path
import json,hashlib,shutil,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-shadowwalk-probe-v522'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert rd(W/'native-operation-owner.json')['state']=='finished' and not A.exists()
live=rd(W/'candidate-shadowwalk-inventory-v515/identity.json')['dll'];assert sha(R/'work/asset-game/dinput8.dll')==live
files=[W/'Build-ShadowWalkScene-v522.ps1',W/'prepare_shadowwalk_scene_v522.py',W/'shadowwalk-state-build-v522.log',W/'shadowwalk-state-world-tests-v522.log']
for p in files[-2:]:
    s=p.read_text(encoding='utf-8-sig');assert s and not any(x in s for x in ['error C','error LNK','FAIL:'])
A.mkdir();shutil.copytree(L/'src',A/'src');shutil.copy2(R/'work/build/Release/d2_runtime_probe.exe',A/'d2_runtime_probe.exe')
for p in [Path(__file__),*files]:shutil.copy2(p,A/p.name)
identity=dict(at=datetime.datetime.now().astimezone().isoformat(),liveDll=live,probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Read-only owned Shadow Walk entry32, sustained form33 and exit34 reader. Exact scene/clock and seven reciprocal bindings. Proxy515, protocols and banks unchanged; runtime test522 pending.')
(A/'identity.json').write_text(json.dumps(identity,indent=2),encoding='utf-8')
shutil.copy2(A/'d2_runtime_probe.exe',L/'bin/d2_runtime_probe.exe');print(json.dumps(identity))
