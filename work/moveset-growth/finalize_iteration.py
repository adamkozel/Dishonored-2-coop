"""Record this iteration without promoting rejected turn candidates."""
import json,hashlib,datetime,zipfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parent.parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
def write(p,d):p.write_text(json.dumps(d,indent=2))
now=datetime.datetime.now().astimezone().isoformat()
write(P/'native-speed-1/assessment.json',dict(valid_calibration=False,script_completed=True,
 reason='The input script completed, but subsequent inspection found client follow disabled and a persistent host Slash overlay; short source movements were inconsistent. Collision/action/follow contamination is unresolved. Do not use summary speeds as accepted gait limits.',
 recovery='Protected SLOT81 fixtures reloaded; Mayhew native scene completed without speedup; restored-walk-result.json passes on both post-reload clone incarnations.'))
base=ROOT/'work/baseline-backup/Dishonored2/base';original=Path('C:/Users/adako/Saved Games/Arkane Studios/Dishonored2/base')
checks=[dict(file=str(p.relative_to(base)),matches=sha(p)==sha(original/p.relative_to(base))) for p in base.rglob('*') if p.is_file()]
for role in ('host','client'):
 for name in ('checkpoint.dat','game.details','screenshot.png'):
  a=P/'pre-runtime-saves'/role/'SLOT81'/name
  if not a.exists():a=P/'pre-runtime-saves'/role/'savegame/SLOT81'/name
  if not a.exists():a=P/'pre-runtime-saves'/role/'base/savegame/SLOT81'/name
  b=ROOT/f'work/lab-instances/{role}/base/savegame/SLOT81'/name
  checks.append(dict(file=f'{role}/SLOT81/{name}',matches=sha(a)==sha(b)))
steam=Path('C:/Program Files (x86)/Steam/steamapps/common/Dishonored2')
checks += [dict(file='Steam Dishonored2.exe',matches=sha(steam/'Dishonored2.exe')=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'),
 dict(file='Steam steam_api64.dll',matches=sha(steam/'steam_api64.dll')=='71AF8666392ACAD2080A816B9F1196FE9CCB950E1DB8A549F8312C4D231F55A8'),
 dict(file='No Steam dinput8.dll',matches=not (steam/'dinput8.dll').exists()),
 dict(file='Live staged DLL matches walking candidate',matches=sha(ROOT/'work/asset-game/dinput8.dll')==sha(ROOT/'outputs/d2-coop-lab/bin/dinput8.dll')=='723540B709CCF7ECD999AC76B1A47F281E47B7AB9B6D2193A0D4EE0CF01941CB')]
write(P/'iteration-integrity.json',dict(at=now,checks=checks,passed=all(c['matches'] for c in checks)))
assert all(c['matches'] for c in checks),'Preserved file mismatch'
tests=[read(P/p) for p in ('walk-test-3/result.json','priority-2/result.json','restored-walk-result.json')]
assert all(t['passed'] for t in tests)
live=[]
for role in ('host','client'):
 s=read(ROOT/f'work/lab-instances/{role}/coop-status.json');m=read(ROOT/f'work/lab-instances/{role}/coop-status.json.movement.json')
 assert s['localGameLogicState']==1 and s['remoteFresh'] and s['probeFollowEnabled'] and not m['active']
 live.append(dict(role=role,epoch=s['localEpoch'],follow=s['probeFollowEnabled'],fresh=s['remoteFresh'],bank_active=m['active']))
result=dict(at=now,full_moveset_complete=False,walking_candidate='723540B709CCF7ECD999AC76B1A47F281E47B7AB9B6D2193A0D4EE0CF01941CB',
 scope='Sword-ready idle and eight walking directions per character in isolated flat Dunwall; distance-driven phase and existing native-action priority. Not running, crouch, traversal or a production-complete moveset.',
 direction_checks=len(tests[0]['checks']),priority_checks=len(tests[1]['checks']),post_reload_role_smoke_checks=len(tests[2]['cases']),
 turns='Both rigs retargeted; no turn candidate deployed. Knee/foot timing quality remains inadequate.',native_fullbody_candidates=read(P/'native-fullbody-candidates.json')['count'],live=live,
 videos=['corvo-walking.mp4','emily-walking.mp4'],coverage='COVERAGE.txt',preserved_milestone='milestone-7235',integrity='iteration-integrity.json')
write(P/'result.json',result)
# Archive offline turn research separately from the tested walking milestone.
archive=P/'turn-research-1010.zip'
if not archive.exists():
 files=[p for folder in ('turns','turns-component','turns-chest','turns-timed','turn-previews','turn-component-previews','turn-chest-previews','donors-feedback','donors-component-feedback') for p in (P/folder).rglob('*') if p.is_file()]
 files += [p for p in P.glob('*turn*.py')]+[P/'probe_root_feedback.py',P/'probe_component_feedback.py',P/'generate_locomotion.py',P/'README.txt',P/'result.json']
 manifest=[dict(path=str(p.relative_to(P)),sha256=sha(p),bytes=p.stat().st_size) for p in sorted(set(files))]
 with zipfile.ZipFile(archive,'w',zipfile.ZIP_DEFLATED,compresslevel=6) as z:
  for entry in manifest:z.write(P/entry['path'],entry['path'])
  z.writestr('archive-manifest.json',json.dumps(manifest,indent=2))
 write(P/'turn-research-1010-pin.json',dict(archive=archive.name,sha256=sha(archive),files=len(manifest),bytes=archive.stat().st_size))
print(json.dumps(result,indent=2))
