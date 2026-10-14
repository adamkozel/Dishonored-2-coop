"""Publish the bounded two-role experiment, retaining failed visual acceptance."""
import datetime,hashlib,json,shutil
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
    t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-carry-attachment-v8';backup.mkdir(exist_ok=True)
for n in ['runtime-video-manifest.json','review-evidence-annotations.json','review.html','review-catalog.js']:
    if not (backup/n).exists():shutil.copy2(P/n,backup/n)
manifest=read(P/'runtime-video-manifest.json');cases={}
for role in ['corvo','emily']:
    take=f'carry-victim-native-{role}-v8';folder=P/take
    v=read(folder/'validation.json');assert v['mechanical_checks_passed'] and v['native_victim_animation']
    assert hashlib.sha256((folder/'native.mp4').read_bytes()).hexdigest().upper()==v['video_sha256']
    review=dict(inspected=True,accepted=False,complete=False,
        method='Actual D2 six-frame contact sheet and full-resolution frames at 4.8s and 8.4s.',
        improvement='Native folded victim animation follows the authored carrier through standing, forward walking and crouching; continuous orientation compensation improves crouched shoulder contact.',
        gaps=['Visible initial attachment pop; full-interval root-follow check fails.',
              'Only an explicitly armed eight-second Mayhew experiment, not automatic pickup.',
              'Hand contact, victim clearance and turning require further comparison.',
              'The body falls when the fixture expires, before the source player drops it.',
              'Drop, throw, other victims and lifecycle coverage are not complete.'])
    write(folder/'visual-review.json',review)
    scope='Bounded Mayhew attachment experiment. Native victim animation and ragdoll return verified. Initial attachment pops; hand/contact polish, actual pickup/drop/throw and general lifecycle remain unfinished. Body release at roughly 10 seconds is the test timeout, not the source player drop.'
    row=dict(video=take+'/native.mp4',character=role,label=role.title()+' · Native carried body · shoulder alignment experiment',
        alias='native_carry_attachment_experiment',carry_attachment=True,travelling=True,gameplay_bound=True,
        complete=False,visual_accepted=False,automatic_pickup=False,scope=scope,
        requirements=['carry_body_lift_idle_move_drop_throw'],evidence=take+'/validation.json',
        visual_review=take+'/visual-review.json',actual_capture_hz=v['actual_capture_hz'],
        timing='Actual elapsed capture; no speedup',proxy_sha256=v['identity']['dllSha256'],
        bank_sha256=v['identity']['bankSha256'],source_role=v['identity']['role'],observer_role=v['identity']['peer'])
    manifest=[r for r in manifest if r['video']!=row['video']];manifest.append(row)
    cases[role]=dict(video=row['video'],validation=row['evidence'],visual_review=row['visual_review'],
                     checks=len(v['checks']),samples=v['samples'],held_samples=v['held_samples'])
annotations=read(P/'review-evidence-annotations.json');item=annotations['items']['carry_body_lift_idle_move_drop_throw']
item['runtime_binding']='partial_carrier_and_bounded_native_victim_both_roles'
item['attachment_runtime_evidence']=dict(cases=cases,complete=False,visual_accepted=False)
note='An additional eight-second experiment now attaches the matching Mayhew corpse, plays her native carry clip and restores ragdoll for both remote character roles. Standing/crouched contact is improved, but the initial pop and timer release are not finished pickup/drop gameplay.'
if note not in item['notes']:item['notes']+=' '+note
html=P/'review.html';s=html.read_text(encoding='utf-8-sig')
old=":r.carrier_only?'Native Steam drives the carrier body. Corpse attachment and physics are still pending.'"
new=":r.carry_attachment?'Bounded native corpse attachment experiment. Pickup, release and visual acceptance remain unfinished.'"+old
if new not in s:
    assert s.count(old)==1;s=s.replace(old,new);html.write_text(s,encoding='utf-8')
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',annotations)
state=read(P.parent/'current-task-state.json');m=state['MovementIntegration'];a=m['CarryAttachmentExperiment']
a.update(status='v8 native victim clip, animated physics and shoulder compensation recorded for both roles; initial pop, hand contact and release incomplete.',
    cases=cases,latest_completed='carry-victim-native-emily-v8',latest_candidate='candidate-carry-victim-scene-v10',
    next='Diagnose extended-lease ownership refusal; test source-driven native release. Derived standing/crouched victim drop scenes exist but are not installed or invoked yet.',complete=False,visual_accepted=False)
m['LatestContinuation']['actual_game_videos']=len(manifest)
state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat()
write(P.parent/'current-task-state.json',state)
print(json.dumps(dict(videos=len(manifest),cases=cases,complete=False)))
