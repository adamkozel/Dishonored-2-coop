"""Refresh measured failure-FX state; every runtime claim remains take-specific."""
import datetime, hashlib, json
from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work';P=W/'natural-moveset';D=R/'documentation/handoff-2026-10-11'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat();archive=W/'movement-integration/candidate-network-failure-fx-v48';identity=read(archive/'identity.json')
live=sha(W/'asset-game/dinput8.dll');probe=sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')
assert live==identity['dll'] and probe==identity['probe']
checks=[]
for f in read(W/'movement-integration/protected-save-hashes-before-v4.json'):
 actual=sha(Path(f['Path']));checks.append(dict(path=f['Path'],expected=f['Hash'],actual=actual,passed=actual==f['Hash']))
steam=Path('C:/Program Files (x86)/Steam/steamapps/common/Dishonored2')
for name,expected in [('Dishonored2.exe','B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'),('steam_api64.dll','71AF8666392ACAD2080A816B9F1196FE9CCB950E1DB8A549F8312C4D231F55A8')]:
 for folder in [steam,W/'asset-game']:
  f=folder/name;actual=sha(f);checks.append(dict(path=str(f),expected=expected,actual=actual,passed=actual==expected))
assert all(x['passed'] for x in checks)
integrity=dict(at=now,passed=True,checks=checks,liveProxy=live,candidate=archive.name+' (code) / candidate-outofmana-contexts-v46 (bank)',probe=probe,isolatedExecutable=sha(W/'asset-game/Dishonored2.exe'),originalExecutable=sha(steam/'Dishonored2.exe'))
write(W/'handoff-integrity-verification.json',integrity)
cases={f.parent.name:read(f) for f in P.glob('outofmana-*/result.json')}
state=read(W/'current-task-state.json');m=state['MovementIntegration'];latest=m['LatestContinuation']
latest.update(live=live,liveBuild=live,bank='D2MOVE41 / 251 clips',actual_game_videos=len(read(P/'runtime-video-manifest.json')),next='Validate automatic failure hand FX and continue remaining movement bindings and quality review',in_progress='Native failure hand effects, replay protection and remaining full movement checklist',complete=False)
m['PowerFailureFxV48']=dict(code_archive=str(archive.relative_to(W)),bank_archive='movement-integration/candidate-outofmana-contexts-v46',bank_manifest='natural-moveset/outofmana-context-bank-manifest-v46.json',cases=cases,scope='v46 four stance/equipment contexts pass; v47 manual Corvo/Emily hand FX pass six checks each; v48 automatic Corvo failure effect results are take-specific. Wrist-centred socket adaptation. No complete visual acceptance or shared time authority.',complete=False)
state['UpdatedAt']=now;write(W/'current-task-state.json',state)
start='<!-- BENDTIME-CURRENT -->';end='<!-- /BENDTIME-CURRENT -->';s=(D/'01-current-state.md').read_text(encoding='utf-8')
section=f'''{start}
## Current movement continuation

Updated {now}. Measured live proxy `{live}`; probe `{probe}`. [Current code milestone](../../work/movement-integration/{archive.name}), [bank pins](../../work/natural-moveset/outofmana-context-bank-manifest-v46.json), [18-file integrity report](../../work/handoff-integrity-verification.json).

Corvo Bend Time uses exact native scene clocks over Steam, with scoped ground/air and same-world pause recovery evidence. Four standing/crouched and drawn/holstered failure-response takes pass twelve checks each in v46. Manual native failure hand FX pass six checks each for Corvo and Emily in v47; viewed temporal samples show hand smoke without obvious observer blur. v48 binds Corvo's effect once to the delayed native failure event, with duplicate/late-event rejection. Consult each take for actual runtime outcome. Emily automatic failure binding, exact tattoo alignment, continuous visual quality, contact/clearance and broader movement coverage remain open.

Prior milestones are retained. Whole movement completion remains false. Counts of clips or videos do not establish accepted movements. See [failure/recovery findings](16-power-failure-and-resume.md), [full state](../../work/current-task-state.json) and [dashboard](http://127.0.0.1:8769/review.html).
{end}'''
assert start in s and end in s
(D/'01-current-state.md').write_text(s[:s.index(start)]+section+s[s.index(end)+len(end):],encoding='utf-8')
print(json.dumps(dict(at=now,cases=len(cases),integrity_checks=len(checks),passed=True,live=live)))
