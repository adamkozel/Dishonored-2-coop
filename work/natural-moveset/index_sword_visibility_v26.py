"""Index scoped v26 game evidence without promoting the whole carry action."""
import json,hashlib,shutil,datetime,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
 t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-sword-visibility-v26';backup.mkdir(exist_ok=True)
for name in ['runtime-video-manifest.json','review-evidence-annotations.json']:
 if not(backup/name).exists():shutil.copy2(P/name,backup/name)
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');item=ann['items']['carry_body_lift_idle_move_drop_throw']
cases=item.setdefault('native_sword_visibility',{})
buildpath=P.parent/'movement-integration/candidate-sword-visibility-v26/identity.json';build=read(buildpath)
tests=['equipment-visibility-v26/result.json','equipment-visibility-pause-v26/result.json']
for test in tests:
 t=read(P/test);assert t['passed']and t['dll']==build['sha256']
for take in sys.argv[1:]:
 f=P/take;i=read(f/'identity.json');v=read(f/'validation.json');pair=read(f/'paired-validation.json');sword=read(f/'sword-visibility-validation.json');visual=read(f/'visual-review.json')
 assert v['passed']and pair['passed']and sword['passed']and visual['inspected']and i['dllSha256']==build['sha256']
 assert hashlib.sha256((f/'native.mp4').read_bytes()).hexdigest().upper()==v['video_sha256']
 row=dict(video=take+'/native.mp4',character=i['character'],label=i['character'].title()+' · Pickup with holstered sword hidden',
  alias='carry_pickup_supported_settle',requirements=['carry_body_lift_idle_move_drop_throw'],gameplay_bound=True,complete=False,visual_accepted=False,
  evidence=take+'/validation.json',paired_evidence=take+'/paired-validation.json',sword_evidence=take+'/sword-visibility-validation.json',visual_review=take+'/visual-review.json',
  proxy_sha256=i['dllSha256'],bank_sha256=i['bankSha256'],clip_sha256=i['clipSha256'],actual_capture_hz=v['actual_capture_hz'],scope=i['scope'])
 videos=[r for r in videos if r['video']!=row['video']];videos.append(row)
 cases[take]=dict(video=row['video'],evidence=row['evidence'],paired_evidence=row['paired_evidence'],sword_evidence=row['sword_evidence'],visual_review=row['visual_review'],
  checks=len(v['checks'])+len(pair['checks'])+len(sword['checks']),video_sha256=v['video_sha256'],role=i['character'],complete=False,visual_accepted=False)
item['notes']='Standing native-source pickup and explicitly armed Mayhew victim transfer are recorded for both roles. The supported handoff crossfades once into the loaded pose. v26 hides only the clone sword when native equipment confirms holstering and restores its original state on cleanup; both-role draw/holster and pause tests pass. Right-hand grasp, deep gather, shoulder fit, crouched pickup, arbitrary bodies and full throws remain unfinished.'
findings=dict(build=build['sha256'],cases=cases,tests=tests,
 confirmed=['Exact clone inventory child retained; no replacement weapon or inventory mutation for visibility.','Native entity hide/show follows confirmed holster/draw and matching native attachment tags.','Both roles: standing/crouched draw-holster and unarmed directional movement; pause retires ownership and resume restores original sword.'],
 limitations=['Sword folding is not animated; the static clone blade is hidden while holstered.','Full carry and whole movement delivery remain incomplete.'],complete=False)
write(P/'sword-visibility-findings-v26.json',findings)
equipment=ann['items']['sword_draw_holster'];equipment['native_visibility_v26']=findings
note=' v26: the exact clone sword is hidden after confirmed native holstering and shown on draw; both-role standing/crouched and pause-restoration tests pass. Physical folding is not animated.'
equipment['notes']=equipment.get('notes','').split(' v26:')[0]+note
build.update(runtimeValidated=True,tests=tests,cases=cases,validation_scope='Native sword visibility and restoration, scoped paired pickup; not complete carry or moveset acceptance.')
write(buildpath,build);write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',ann)
state=read(P.parent/'current-task-state.json');state['MovementIntegration']['SwordVisibilityV26']=findings
state['MovementIntegration']['LatestContinuation'].update(live=build['sha256'],liveBuild=build['sha256'],actual_game_videos=len(videos),
 in_progress='Continuous native pickup, carry travel, and standing/crouched drop tests; movement phase incomplete',complete=False)
state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(P.parent/'current-task-state.json',state)
print(json.dumps(dict(game_videos=len(videos),cases=len(cases),live=build['sha256'],complete=False)))
