from pathlib import Path
import datetime,hashlib,json,shutil
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; P=R/'work/natural-moveset'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-possession-crouch-release-v371';assert not A.exists()
sha=lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
read=lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
logs=[W/f'possession-crouch-release-{s}-v371.log' for s in ('build','world-tests','action-tests','bank-tests','smoke','author')]
for p in logs:
 s=p.read_text(encoding='utf-8-sig');assert s and not any(x in s for x in ('error C','error LNK','FAIL:','FAIL ','Traceback')),p
A.mkdir();shutil.copytree(L/'src',A/'src');shutil.copytree(P/'possession-crouch-release-banks-v371',A/'banks')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in [Path(__file__),*logs,W/'Build-PossessionCrouchRelease-v371.ps1',W/'prepare_possession_crouch_release_v371.py',W/'Invoke-NativeLabOperation.ps1',P/'Test-PossessionCrouchTarget-v371.ps1',P/'build_possession_crouch_release_v371.py',P/'possession-crouch-release-bank-manifest-v371.json',P/'possession-crouch-release-source-manifest-v371.json']:shutil.copy2(p,A/p.name)
x=read(W/'candidate-possession-draw-v370/identity.json')
x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),bankVersion=54,clips=319,banksReference='banks',banks=[dict(path=str(p),sha256=sha(p)) for p in sorted((A/'banks').glob('*.bin'))],runtimeVerified=False,scope='Native370 successful human Possession retains crouch1 through release/internal0 and transfer/internal1, then returns to Sneaking6 with independent sword1133/power333 scenes.371 adds exact context/dual-scene support and appends complete-body crouched400ms release, retaining all318 prior clips byte-for-byte. Native target generation/ownership checks unchanged; no NPC gameplay replication. Synthetic tests pass; runtime and visual validation pending.')
x.pop('runtimeVerificationScope',None)
(A/'identity.json').write_text(json.dumps(x,indent=2))
for name in ('dinput8.dll','d2_runtime_probe.exe'):
 shutil.copy2(A/name,L/'bin'/name);assert sha(L/'bin'/name)==sha(A/name)
print(json.dumps(dict(archive=str(A),dll=x['dll'],probe=x['probe'])))
