"""Publish inspected supported-settle experiments with their exact build."""
import json,hashlib,shutil,datetime,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
 t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-pickup-settle-v25';backup.mkdir(exist_ok=True)
for name in ['runtime-video-manifest.json','review-evidence-annotations.json','build_review_catalog.py']:
 if not(backup/name).exists():shutil.copy2(P/name,backup/name)
manifest=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');item=ann['items']['carry_body_lift_idle_move_drop_throw']
cases=item.setdefault('supported_pickup_settle',{})
identity_path=P.parent/'movement-integration/candidate-carry-settle-v25/identity.json';build=read(identity_path)
for take in sys.argv[1:]:
 f=P/take;i=read(f/'identity.json');v=read(f/'validation.json');pair=read(f/'paired-validation.json');review=read(f/'visual-review.json')
 assert v['passed']and pair['passed']and review['inspected']and i['dllSha256']==build['sha256']and i['bankVersion']==34
 assert hashlib.sha256((f/'native.mp4').read_bytes()).hexdigest().upper()==v['video_sha256']
 row=dict(video=take+'/native.mp4',character=i['character'],label=i['character'].title()+' · Supported pickup settle · experimental',
  alias='carry_pickup_supported_settle',requirements=['carry_body_lift_idle_move_drop_throw'],gameplay_bound=True,
  complete=False,visual_accepted=False,evidence=take+'/validation.json',paired_evidence=take+'/paired-validation.json',visual_review=take+'/visual-review.json',
  proxy_sha256=i['dllSha256'],bank_sha256=i['bankSha256'],clip_sha256=i['clipSha256'],actual_capture_hz=v['actual_capture_hz'],scope=i['scope'])
 manifest=[r for r in manifest if r['video']!=row['video']];manifest.append(row)
 cases[take]=dict(video=row['video'],evidence=row['evidence'],paired_evidence=row['paired_evidence'],visual_review=row['visual_review'],
  checks=len(v['checks'])+len(pair['checks']),bank_sha256=i['bankSha256'],video_sha256=v['video_sha256'],role=i['character'],complete=False,visual_accepted=False)
item['motions']=list(dict.fromkeys(item['motions']+['carry_pickup_supported_settle']))
item['notes']='Standing native-source pickup and explicitly armed Mayhew victim transfer are recorded for both roles. The contact pilot brings the gather hand nearer the actual body; v25/v34 retains hand support into a one-time 350ms loaded-pose transition. These remain scoped experiments. Right-hand grasp, deep gather, shoulder fit, static holstered sword, crouched pickup, arbitrary floor poses and full throws still need work.'
build.update(runtimeValidated=True,cases=cases,validation_scope='See each recorded role: new supported endpoint and one-time loaded transition, source-clock ownership and late cleanup. Full movement/carry remains incomplete.')
write(identity_path,build);write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',ann)
findings=dict(build=build['sha256'],bank_version=34,cases=cases,
 tests=['carry-settle-v25-bank-tests.log','carry-settle-v25-bank-regression.log','carry-settle-v25-smoke.log','deploy-carry-settle-v25.log'],
 implementation=['Old bank versions keep their existing transitions.','v34 retains left support at the authored hoist endpoint, then blends once to the loaded pose over350ms.','The transition does not restart on native carry-loop wraps.','Root/network position, first-person controls and native victim ownership routines are unchanged.'],
 limitations=['Measured single-fixture Mayhew contact, not general corpse adaptation.','Visual quality remains unaccepted; right hand, shoulder fit and static holstered blade need refinement.','Crouched pickup, arbitrary bodies and complete throws remain incomplete.'],complete=False,visual_accepted=False)
write(P/'pickup-settle-findings-v25.json',findings)
state=read(P.parent/'current-task-state.json');m=state['MovementIntegration'];m['PickupSupportedSettle']=findings
m['LatestContinuation'].update(live=build['sha256'],liveBuild=build['sha256'],actual_game_videos=len(manifest),
 bank='D2MOVE34 / 232 clips; supported pickup settle experiment, with earlier v33 comparison preserved',deploying=None,
 in_progress='Supported pickup transition runtime evidence; complete movement delivery remains unfinished',
 next='Refine hand/body contact and holstered prop appearance; add general/crouched pickup and finish remaining movement/power integrations.',complete=False)
state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(P.parent/'current-task-state.json',state)
print(json.dumps(dict(game_videos=len(manifest),cases=len(cases),live=build['sha256'],complete=False)))
