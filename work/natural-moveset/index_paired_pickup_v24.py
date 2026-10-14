"""Index inspected native pickup studies, without claiming complete carry."""
import datetime,json,hashlib,shutil,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
 t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-paired-pickup-v24';backup.mkdir(exist_ok=True)
for n in ['runtime-video-manifest.json','review-evidence-annotations.json','build_review_catalog.py']:
 if not(backup/n).exists():shutil.copy2(P/n,backup/n)
manifest=read(P/'runtime-video-manifest.json');a=read(P/'review-evidence-annotations.json')
item=a['items']['carry_body_lift_idle_move_drop_throw'];cases=item.setdefault('source_timed_paired_pickup',{})
for take in sys.argv[1:]:
 f=P/take;i=read(f/'identity.json');carrier=read(f/'validation.json');cancel=(f/'cancel-validation.json').exists()
 name='cancel-validation.json'if cancel else'paired-validation.json';v=read(f/name);review=read(f/'visual-review.json')
 assert carrier['passed']and v['passed']and review['inspected']
 assert hashlib.sha256((f/'native.mp4').read_bytes()).hexdigest().upper()==v['video_sha256']
 contact='contact'in i.get('bankManifest','')
 label='Cancel during native pickup'if cancel else'Measured contact pickup pilot'if contact else'Source-timed paired pickup study'
 row=dict(video=take+'/native.mp4',character=i['character'],label=i['character'].title()+' · '+label,
  alias='carry_pickup_contact_pilot'if contact else'carry_body_short_lift_study',requirements=['carry_body_lift_idle_move_drop_throw'],
  gameplay_bound=True,complete=False,visual_accepted=False,evidence=take+'/'+name,visual_review=take+'/visual-review.json',
  actual_capture_hz=v['actual_capture_hz'],proxy_sha256=i['dllSha256'],bank_sha256=i['bankSha256'],clip_sha256=i['clipSha256'],
  scope=i['scope'],timing='Native source clock through Steam, bounded victim experiment; real elapsed video')
 manifest=[r for r in manifest if r['video']!=row['video']];manifest.append(row)
 cases[take]=dict(video=row['video'],evidence=row['evidence'],visual_review=row['visual_review'],
  character=i['character'],checks=len(carrier['checks'])+len(v['checks']),video_sha256=v['video_sha256'],
  bank_sha256=i['bankSha256'],contact_pilot=contact,cancel_before_attachment=cancel,complete=False,visual_accepted=False)
item['motions']=list(dict.fromkeys(item['motions']+['carry_body_short_lift_study','carry_pickup_contact_pilot']))
item['runtime_binding']='bounded_native_source_paired_pickup_both_roles'
item['notes']='Native preparation and hoist now drive carrier and explicitly armed Mayhew victim on one source timeline. Both-role baseline captures pass ownership, native attachment and late cancellation checks. A measured-contact pilot improves Corvo gather reach, but hand fit, shoulder handoff, arbitrary corpse poses, crouched pickup, throws and visual acceptance remain incomplete. Recordings are experiments, not a complete carry implementation.'
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',a)
identity_path=P.parent/'movement-integration/candidate-carry-paired-v24/identity.json'
identity=read(identity_path);identity.update(runtimeValidated=True,cases=cases,validation_scope='Native-input standing pickup with explicitly armed Mayhew victim. Both-role baseline timing/ownership tests; recorded cleanup cases scoped individually. Not arbitrary-body support or visual acceptance.')
write(identity_path,identity)
findings=dict(build=identity['sha256'],cases=cases,confirmed=[
 'Native source preparation holds the captured floor pose; native In time drives the blend to the victim carry loop.',
 'Reciprocal attachment is created only after restoring the snapshot input, followed by bounded anchor calibration.',
 'Both-role baseline takes retain a connected native Steam session and preserve measured player/avatar/corpse health.',
 'Corvo measured-contact v3 changes whole-body reach while preserving native proportions and authored feet; the gathered hand is visibly nearer the actual body.',
 'Read-only compact pair probe samples distinct native skeletons with ownership and generation checks.'],
 limitations=['Explicit isolated Mayhew experiment, not automatic general corpse authority.',
 'No accepted complete pickup. Right-hand grasp, mid-lift contact and shoulder attachment transition remain.',
 'Native-cache reads are sequential and may observe different evaluation phases; status files publish at differing intervals. Contact distances are diagnostic, not exact rendered surface contact.',
 'Current authored contact path is fitted to measured fixture geometry. Crouched lifting, varied floor poses and complete throws are not covered.',
 'The holstered native sword remains visibly extended in the captures.'],complete=False,visual_accepted=False)
write(P/'paired-pickup-findings-v24.json',findings)
state=read(P.parent/'current-task-state.json');m=state['MovementIntegration'];m['NativePairedPickup']=findings
m['LatestContinuation'].update(live=identity['sha256'],liveBuild=identity['sha256'],actual_game_videos=len(manifest),
 bank='D2MOVE33 / 232 clips; base v1 and separate measured-contact v4 comparison; leases disabled between cases',
 in_progress='Paired pickup contact, shoulder handoff and interruption evidence; full movement delivery incomplete',
 next='Resolve paired pickup handoff and general contact limits, then continue remaining native movement/power bindings.',complete=False)
state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(P.parent/'current-task-state.json',state)
print(json.dumps(dict(game_videos=len(manifest),paired_cases=len(cases),complete=False)))
