from pathlib import Path
import json,hashlib,datetime,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-shadowwalk-fx-v533'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
previous=W/'candidate-shadowwalk-body-v526';probe=previous
assert rd(W/'native-operation-owner.json')['state']=='finished'
assert sha(R/'work/asset-game/dinput8.dll')==rd(previous/'identity.json')['dll']
assert sha(L/'bin/d2_runtime_probe.exe')==rd(probe/'identity.json')['probe']
assert rd(R/'work/natural-moveset/native-shadowwalk-travel-v532/result.json')['passed']
logs=[W/f'shadowwalk-body-{x}-v533.log' for x in ('build','world-tests','action-tests','bank-tests','smoke')]
for p in logs:
    s=p.read_text(encoding='utf-8-sig');assert s and not any(t in s for t in ('error C','error LNK','FAIL ', 'Traceback')),p
A.mkdir(exist_ok=False);shutil.copytree(L/'src',A/'src')
shutil.copytree(R/'work/natural-moveset/shadowwalk-banks-v523',A/'banks')
shutil.copytree(previous/'assets',A/'assets')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
identity=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),previous=previous.name,previousProbe=probe.name,protocol=56,bankVersion=58,clips=335,cloneWireVersion=4,runtimeVerified=False,
    scope='533 adds native arm/hand idle/movement/ashes FX placed on the current published peer skeleton with350ms renewable lease and native record identity. No screen effects, shader changes, damage or power calls. Candidate runtime/visual acceptance pending. Body527/529/532 pass. 525 failed on unrecognized native333ms fast sword holster before entry;526 adds observed Emily equipment7 with exact scene/owner/clock and actual native attachment state. Timeout unchanged. Source522 exact seven-binding entry, native sustained form and requested exit drive remote Emily complete-body presentation. Native first person/root/gameplay unchanged. Crawl cadence follows accepted replicated distance. Initial forward clip also reversed for backward; directions, shadow material/native particles, attacks and visual quality unfinished. All331 previous payloads retained. Previous509 intermittent airborne landing dropout remains unresolved.',
    banks=[dict(path=str(p),sha256=sha(p)) for p in (A/'banks').glob('*.bin')],assets=rd(previous/'identity.json')['assets'])
(A/'identity.json').write_text(json.dumps(identity,indent=2),encoding='utf-8')
for p in [Path(__file__),W/'prepare_shadowwalk_body_v524.py',W/'prepare_shadowwalk_holster_v526.py',W/'Build-ShadowWalkFx-v533.ps1',W/'prepare_shadowwalk_fx_v533.py',R/'work/natural-moveset/build_shadowwalk_bank_v523.py',R/'work/natural-moveset/shadowwalk-bank-manifest-v523.json',*logs]:shutil.copy2(p,A/p.name)
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
s=(W/'Deploy-ShadowWalkInventory-v515.ps1').read_text(encoding='utf-8-sig').replace('shadowwalk-inventory-v515','shadowwalk-fx-v533').replace('ShadowWalkInventoryDeploy515','ShadowWalkFxDeploy533')
(W/'Deploy-ShadowWalkFx-v533.ps1').write_text(s,encoding='utf-8')
print(json.dumps({k:identity[k] for k in ('dll','probe','protocol','bankVersion','runtimeVerified')}))
