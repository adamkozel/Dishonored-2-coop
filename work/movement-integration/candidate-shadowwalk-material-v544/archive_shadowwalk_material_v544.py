from pathlib import Path
import json,hashlib,datetime,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-shadowwalk-material-v544';P=W/'candidate-shadowwalk-fx-v533'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert rd(W/'native-operation-owner.json')['state']=='finished'
assert sha(R/'work/asset-game/dinput8.dll')==rd(P/'identity.json')['dll']
assert rd(R/'work/natural-moveset/native-shadowwalk-material-v540b/result.json')['passed']
assert rd(R/'work/natural-moveset/native-shadowwalk-material-source-v542/result.json')['passed']
logs=[W/f'shadowwalk-body-{x}-v544.log' for x in ('build','world-tests','action-tests','bank-tests','smoke')]
for p in logs:
 s=p.read_text(encoding='utf-8-sig');assert s and not any(t in s for t in ('error C','error LNK','FAIL:', 'Traceback')),p
A.mkdir(exist_ok=False);shutil.copytree(L/'src',A/'src');shutil.copytree(P/'banks',A/'banks');shutil.copytree(P/'assets',A/'assets')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=P.name,protocol=56,bankVersion=58,clips=335,cloneWireVersion=4,runtimeVerified=False,
 scope='Isolated material feasibility only. Native876030 applies effect4 and partial fade0.45 to exact remote Emily renderer under accepted Shadow Walk presentation, with15second cap and no attached-entity propagation. Requires no preexisting overrides; exact own values checked and removed on exit. Native FP/shared materials/assets untouched. Shadow form appearance not accepted until actual in-game comparison.533 remains rollback. Prior509 air dropout and540 first-start root freshness retirement remain open.',
 banks=[dict(path=str(p),sha256=sha(p))for p in(A/'banks').glob('*.bin')],assets=rd(P/'identity.json')['assets'])
(A/'identity.json').write_text(json.dumps(identity,indent=2),encoding='utf-8')
for p in [Path(__file__),W/'prepare_shadowwalk_material_v544.py',W/'Build-ShadowWalkMaterial-v544.ps1',*logs]:shutil.copy2(p,A/p.name)
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
s=(W/'Deploy-ShadowWalkFx-v533.ps1').read_text(encoding='utf-8-sig').replace('shadowwalk-fx-v533','shadowwalk-material-v544').replace('ShadowWalkFxDeploy533','ShadowWalkMaterialDeploy544');(W/'Deploy-ShadowWalkMaterial-v544.ps1').write_text(s,encoding='utf-8')
print(json.dumps({k:identity[k]for k in ('dll','probe','runtimeVerified','scope')}))
