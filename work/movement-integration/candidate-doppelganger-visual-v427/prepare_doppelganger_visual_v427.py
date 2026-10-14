from pathlib import Path
import json, hashlib, shutil, datetime
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'; L=R/'outputs/d2-coop-lab'
A=W/'candidate-doppelganger-visual-v427'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
logs=[W/f'doppelganger-visual-{k}-v427.log' for k in ('build','world-tests','action-tests','bank-tests','smoke')]
for p in logs:
    t=p.read_text(encoding='utf-8-sig'); assert t and not any(s in t for s in ('error C','error LNK','FAIL:','Traceback')),p
A.mkdir();shutil.copytree(L/'src',A/'src')
for directory in ('banks','assets'):shutil.copytree(W/'candidate-doppelganger-visual-v423'/directory,A/directory)
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
x=rd(W/'candidate-doppelganger-visual-v425/identity.json')
x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,
    scope='Candidate427: canonicalize signed-zero bind fingerprint, retain every nonzero bit, hierarchy and model check. Probe exposes canonical and raw rig fingerprints. Same dedicated219-joint source-owned cosmetic clone pipeline423. Runtime pending.',
    banks=[dict(path=str(p),sha256=sha(p)) for p in (A/'banks').glob('*.bin')])
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
for p in [Path(__file__),W/'Build-DoppelgangerVisual-v427.ps1',*logs]:shutil.copy2(p,A/p.name)
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
s=(N/'Test-DoppelgangerVisual-v424.ps1').read_text(encoding='utf-8-sig').replace('WaitForExit(18000)','WaitForExit(45000)')
s=s.replace("    $visual=S host '.doppelganger-visual.json';", "    S client '.doppelganger-world.json'|ConvertTo-Json -Depth 12|Set-Content \"$out/source-before-rig.json\"\n    $visual=S host '.doppelganger-visual.json';")
s=s.replace("    if(!$pose.stable-or!$pose.finalPublicationComplete", "    if($pose.cloneRig-ne'5921d3450a60d7e2'-or!$pose.stable-or!$pose.finalPublicationComplete")
(N/'Test-DoppelgangerVisual-v428.ps1').write_text(s,encoding='utf-8')
s=(W/'Run-DoppelgangerVisual-v424.ps1').read_text(encoding='utf-8-sig').replace('v424','v428')
(W/'Run-DoppelgangerVisual-v428.ps1').write_text(s,encoding='utf-8')
s=(W/'Deploy-DoppelgangerVisual-v425.ps1').read_text(encoding='utf-8-sig').replace('Deploy425','Deploy427')
(W/'Deploy-DoppelgangerVisual-v427.ps1').write_text(s,encoding='utf-8')
print(json.dumps(dict(dll=x['dll'],probe=x['probe'],candidate=str(A))))
