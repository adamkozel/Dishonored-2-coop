from pathlib import Path
import json,hashlib,datetime,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';L=R/'outputs/d2-coop-lab';A=W/'candidate-doppelganger-fx-v448'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
logs=[W/f'doppelganger-visual-{x}-v448.log'for x in ('build','world-tests','action-tests','bank-tests','smoke')]+[W/'doppelganger-fx-probe-build-v448.log']
for p in logs:
 t=p.read_text(encoding='utf-8-sig');assert t and not any(x in t for x in ('error C','error LNK','FAIL:','Traceback')),p
assert sha(R/'work/asset-game/dinput8.dll')==rd(W/'candidate-doppelganger-visual-v427/identity.json')['dll']
A.mkdir();shutil.copytree(L/'src',A/'src');shutil.copytree(W/'candidate-doppelganger-visual-v427/banks',A/'banks');shutil.copytree(W/'candidate-doppelganger-visual-v427/assets',A/'assets')
for resource in rd(N/'doppelganger-fx-assets-v441.json'):shutil.copy2(resource['generated'],A/'assets')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
x=dict(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,previous='candidate-doppelganger-visual-v427',scope='448 source-owned clone summon/disappearance native cosmetic scenes; clone wirev3/10712 carries strict scene identity/time alongside all219 native joints. DLL/asset runtime verification pending. First-person/gameplay untouched.',banks=[dict(path=str(p),sha256=sha(p))for p in(A/'banks').glob('*.bin')],assets=rd(N/'doppelganger-fx-assets-v441.json'))
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
for p in [Path(__file__),W/'implement_doppelganger_fx_v448.py',W/'Build-DoppelgangerVisual-v448.ps1',N/'build_doppelganger_fx_assets_v441.py',N/'doppelganger-fx-assets-v441.json',*logs]:shutil.copy2(p,A/p.name)
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
s=(W/'Install-DoppelgangerVisual-v423.ps1').read_text(encoding='utf-8-sig').replace('DoppelgangerVisualInstall','DoppelgangerFxInstall448').replace('Build-D2DoppelgangerVisualAssets','Build-D2DoppelgangerFxAssets').replace('doppelganger-visual-install-v423','doppelganger-fx-install-v449').replace('Cosmetic clone assets423','Cosmetic effect assets441')
(W/'Install-DoppelgangerFx-v449.ps1').write_text(s,encoding='utf-8')
print(json.dumps(x|{'assets':len(x['assets'])}))
