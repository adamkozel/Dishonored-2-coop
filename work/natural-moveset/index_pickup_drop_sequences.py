"""Record continuous native action evidence, including explicit failed comparisons."""
import json,hashlib,shutil,datetime,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
 t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-pickup-drop-sequences';backup.mkdir(exist_ok=True)
for name in ['runtime-video-manifest.json','review-evidence-annotations.json']:
 if not(backup/name).exists():shutil.copy2(P/name,backup/name)
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');item=ann['items']['carry_body_lift_idle_move_drop_throw']
cases=item.setdefault('continuous_pickup_drop',{})
for take in sys.argv[1:]:
 f=P/take;i=read(f/'identity.json');v=read(f/'validation.json');drop=read(f/'pickup-drop-validation.json');visual=read(f/'visual-review.json')
 assert i['nativeDrop']and v['passed']and visual['inspected']
 assert hashlib.sha256((f/'native.mp4').read_bytes()).hexdigest().upper()==drop['video_sha256']
 orientation=read(f/'orientation-validation.json')if (f/'orientation-validation.json').exists()else None
 contact=read(f/'load-contact-validation.json')if (f/'load-contact-validation.json').exists()else None
 passed=drop['passed']and (orientation is None or orientation['passed'])and (contact is None or contact['passed'])
 label=i['character'].title()+' · '+('Crouched pickup'if i['crouched']else'Pickup')+', walk, '+('crouched'if i['crouchedDrop']else'standing')+' drop'
 if not drop['passed']:label+=' · REJECTED physics comparison'
 elif orientation is not None and not orientation['passed']:label+=' · REJECTED rotation comparison'
 elif contact is not None and not contact['passed']:label+=' · REJECTED load/contact comparison'
 row=dict(video=take+'/native.mp4',character=i['character'],label=label,alias='crouched_carry_pickup'if i['crouched']else'carry_pickup_supported_settle',
  requirements=['carry_body_lift_idle_move_drop_throw'],gameplay_bound=True,complete=False,visual_accepted=False,
  evidence=take+'/pickup-drop-validation.json',visual_review=take+'/visual-review.json',proxy_sha256=i['dllSha256'],bank_sha256=i['bankSha256'],actual_capture_hz=drop['actual_capture_hz'],
  scope=drop['scope'],checks_passed=passed,orientation_evidence=take+'/orientation-validation.json'if orientation else None,
  contact_evidence=take+'/load-contact-validation.json'if contact else None)
 videos=[r for r in videos if r['video']!=row['video']];videos.append(row)
 cases[take]=dict(**row,checks=len(v['checks'])+len(drop['checks'])+(len(orientation['checks'])if orientation else 0)+(len(contact['checks'])if contact else 0),video_sha256=drop['video_sha256'],
  final_corpse_distance_from_source_m=drop['final_corpse_distance_from_source_m'])
item['notes']=item['notes'].split(' Continuous sequence:')[0]+' Continuous sequence: native pickup, brief walk and low drop are now tested together. The v26 Corvo comparison is rejected because the corpse flies away; its recording and failed physical-outcome check are preserved. See the per-build v27 comparisons for the native physics handoff correction. Final hand contact, crouched pickup, arbitrary bodies and throws remain unfinished.'
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',ann)
state=read(P.parent/'current-task-state.json');m=state['MovementIntegration'];m['ContinuousPickupDrop']=cases;m['LatestContinuation']['actual_game_videos']=len(videos)
state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(P.parent/'current-task-state.json',state)
print(json.dumps(dict(game_videos=len(videos),cases=len(cases),passed=sum(c['checks_passed']for c in cases.values()),complete=False)))
