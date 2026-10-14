"""Preserve the authored review pass, evidence and original-file integrity."""
import json,hashlib,datetime,zipfile,argparse,re
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parent.parent
parser=argparse.ArgumentParser();parser.add_argument('--name',default='authoring-pass-01');args=parser.parse_args();assert re.fullmatch(r'authoring-pass-[0-9]+',args.name)
out=P/args.name;out.mkdir(exist_ok=True);archive=out/'review-package.zip';assert not archive.exists(),'Preserved pass already exists'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):
 h=hashlib.sha256()
 with p.open('rb')as f:
  for b in iter(lambda:f.read(2**20),b''):h.update(b)
 return h.hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat();catalog=read(P/'review-catalog.json')
base=ROOT/'work/baseline-backup/Dishonored2/base';original=Path('C:/Users/adako/Saved Games/Arkane Studios/Dishonored2/base')
checks=[dict(file=str(p.relative_to(base)),matches=sha(p)==sha(original/p.relative_to(base)))for p in base.rglob('*')if p.is_file()]
for role in ['host','client']:
 for name in ['checkpoint.dat','game.details','screenshot.png']:
  candidates=[ROOT/'work/moveset-growth/pre-runtime-saves'/role/s/name for s in ['SLOT81','savegame/SLOT81','base/savegame/SLOT81']]
  prior=next(p for p in candidates if p.exists());current=ROOT/f'work/lab-instances/{role}/base/savegame/SLOT81'/name
  checks.append(dict(file=role+'/SLOT81/'+name,matches=sha(prior)==sha(current)))
steam=Path('C:/Program Files (x86)/Steam/steamapps/common/Dishonored2')
expected={'Dishonored2.exe':'B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180','steam_api64.dll':'71AF8666392ACAD2080A816B9F1196FE9CCB950E1DB8A549F8312C4D231F55A8'}
checks += [dict(file='Steam/'+name,matches=sha(steam/name)==digest)for name,digest in expected.items()]
checks.append(dict(file='Steam/dinput8.dll remains absent',matches=not(steam/'dinput8.dll').exists()))
checks.append(dict(file='Isolated live DLL',matches=sha(ROOT/'work/asset-game/dinput8.dll')==sha(ROOT/'outputs/d2-coop-lab/bin/dinput8.dll')=='CF60C5EAB9A08573F128F2EF3958F9237BC92FCDC5B551682601DF44C6BDBD46'))
integrity=dict(at=now,passed=all(c['matches']for c in checks),checks=checks)
(P/'authoring-pass-integrity.json').write_text(json.dumps(integrity,indent=2));assert integrity['passed']
info=dict(at=now,complete_moveset=False,functional_requirements=105,requirements_with_candidates=sum(bool(i['motions'])for i in catalog['items']),paired_role_recordings=sum(bool(m['videos'])for m in catalog['motions']),actual_game_recordings=catalog['game_recordings'],native_sources_sampled=1889,first_person_constraints=False,unchanged_live_dll='CF60C5EAB9A08573F128F2EF3958F9237BC92FCDC5B551682601DF44C6BDBD46',remaining=[dict(id=i['id'],family=i['family'])for i in catalog['items']if not i['motions']],limitations=['Authored candidates are not visual acceptance or complete game-state bindings.','Right game-window capture remains blocked by monitor crop error; left capture recovered.','Two 39.73-second native reading clips exceed the current 30-second in-game review duration limit; offline recordings are complete.','Offline native helper/cloth preview approximations differ from actual D2 rendering.','Contact targets, held props, paired victims, character powers, many transitions and contextual variants remain incomplete.'])
(P/'authoring-pass-progress.json').write_text(json.dumps(info,indent=2))
state=read(ROOT/'work/current-task-state.json');state['UpdatedAt']=now;state['LoaderSha256']=info['unchanged_live_dll'];state['NaturalFullBodyMoveset']=dict(**info,review='natural-moveset/review.html',catalog='natural-moveset/review-catalog.json',integrity='natural-moveset/authoring-pass-integrity.json',stable_milestone='natural-moveset/milestone-CF60',tests=['window-v5-roundtrip.json','slide-v4-roundtrip.json','ammo-changes-v1-roundtrip.json','window-choke-v2-roundtrip.json','top-transition-v1-roundtrip.json','water-exit-v3-roundtrip.json','trap-surface-v1-roundtrip.json','carry-release-v3-roundtrip.json','ladder-top-entry-v1-roundtrip.json','body-lift-v2-roundtrip.json','body-throw-v1-roundtrip.json','gaits-corvo-2/result.json','gaits-emily-1/result.json','gait-priority-1/result.json','stance-pose-test-1/result.json','stance-roundtrip.json','mixamo-v5-pose-test/result.json','mixamo-v5-roundtrip.json','review-feedback-export-test.json','guard-v1-roundtrip.json','shadow-v1-roundtrip.json','mechanisms-v4-roundtrip.json','carry-v3-roundtrip.json','object-carry-v1-roundtrip.json','item-transitions-v1-roundtrip.json','ladder-v2-roundtrip.json','water-v1-roundtrip.json','contextual-v1-roundtrip.json','crouched-pairs-v1-roundtrip.json','blink-assault-v1-roundtrip.json'])
(ROOT/'work/current-task-state.json').write_text(json.dumps(state,indent=2))
files=set()
for ext in ['*.py','*.ps1','*.cpp','*.h','*.html','*.js','*.txt','*.json','*.jpg','*.png']:
 files.update(p for p in P.glob(ext)if p.is_file())
def dependencies(value):
 if isinstance(value,dict):
  for k,v in value.items():
   if k in ['binary','model','skeleton','sample_file'] and isinstance(v,str):
    candidate=(P/v).resolve()
    if candidate.is_relative_to(P) and candidate.is_file():
     files.add(candidate)
     if k=='binary' and candidate.with_suffix('.json').exists():files.add(candidate.with_suffix('.json'))
   elif isinstance(v,(dict,list)):dependencies(v)
 elif isinstance(value,list):
  for v in value:
   if isinstance(v,(dict,list)):dependencies(v)
for motion in catalog['motions']:
 for r in motion['roles'].values():
  binary=P/r['binary'];files.add(binary);files.add(binary.with_suffix('.json'))
  dependencies(read(binary.with_suffix('.json')))
 for v in motion['videos']:files.add(P/v['path']);files.add((P/v['path']).with_suffix('.json'))
for r in catalog['game_recordings']:files.add(P/r['video'])
for name in ['window-v5-pose-test-1','slide-v4-pose-test-1','ammo-changes-v1-pose-test-1','window-choke-v2-pose-test-1','window-choke-v2-pose-test-2','top-transition-v1-pose-test-1','water-exit-v2-pose-test-1','water-exit-v3-pose-test-1','trap-surface-v1-pose-test-1','carry-release-v3-pose-test-1','ladder-top-entry-v1-pose-test-1','body-lift-v2-pose-test-1','body-throw-v1-pose-test-1','stance-pose-test-1','mixamo-v5-pose-test','gaits-corvo-2','gaits-emily-1','gait-priority-1','powers-pose-test-1','crouch-actions-pose-test-1','crouch-actions-pose-test-2','crouch-actions-v2-pose-test-1','armed-sprint-v3-pose-test-1','items-v2-pose-test-1','jumps-v3-pose-test-1','guard-v1-pose-test-1','shadow-v1-pose-test-1','mechanisms-v4-pose-test-1','carry-v3-pose-test-1','object-carry-v1-pose-test-1','item-transitions-v1-pose-test-1','ladder-v2-pose-test-1','water-v1-pose-test-1','contextual-v1-pose-test-1','crouched-pairs-v1-pose-test-1','blink-assault-v1-pose-test-1','trap-wall-emily-video-1','blink-assault-emily-video-1']:
 files.update(p for p in (P/name).glob('*.json')if p.is_file())
for name in ['item-research','transition-research','fp-window-body-samples','mixamo-heavy-pickup','item-models','item-mechanisms','mixamo-jump','pair-research','item-contact-v2','contact-fixtures','mixamo-low-crawl','fp-carry-samples','carry-research','mixamo-aerial-attack','ladder-research']:
 files.update(p for p in (P/name).rglob('*')if p.is_file())
for name in ['boot-seam-v1','boot-seam-native-test-1']:
 files.update(p for p in (P/name).rglob('*')if p.is_file() and p.suffix in ['.py','.ps1','.json','.txt','.png','.jpg','.log','.dmp'])
manifest=[]
with zipfile.ZipFile(archive,'x',compression=zipfile.ZIP_DEFLATED,compresslevel=2)as z:
 for file in sorted(files):
  if not file.exists():continue
  relative=str(file.relative_to(P)).replace('\\','/');manifest.append(dict(path=relative,sha256=sha(file),size=file.stat().st_size));z.write(file,relative)
(out/'manifest.json').write_text(json.dumps(dict(at=now,archive=archive.name,archive_sha256=sha(archive),files=manifest),indent=2))
print(json.dumps(dict(integrity=True,files=len(manifest),bytes=archive.stat().st_size,recorded=info['paired_role_recordings'],covered_by_candidates=info['requirements_with_candidates'],complete=False)))
