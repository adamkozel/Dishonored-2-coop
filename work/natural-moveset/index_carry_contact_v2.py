"""Retain a visually inspected comparison without marking the interaction done."""
import datetime, hashlib, json, shutil
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
    t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-carry-contact-v2';backup.mkdir(exist_ok=True)
for n in ['runtime-video-manifest.json','review-evidence-annotations.json']:
    if not (backup/n).exists():shutil.copy2(P/n,backup/n)
manifest=read(P/'runtime-video-manifest.json');cases={}
for role in ['corvo','emily']:
    take=f'carry-contact-{role}-crouched-v2';folder=P/take;v=read(folder/'validation.json')
    assert v['mechanical_checks_passed'] and v['native_victim_drop_animation']
    assert hashlib.sha256((folder/'native.mp4').read_bytes()).hexdigest().upper()==v['video_sha256']
    review=dict(inspected=True,accepted=False,complete=False,
        method='Actual D2 contact sheets at 4.8, 8.4, 14.6, 15.2, 15.55 and 16.1 seconds; same front view and inputs as the v13 comparison.',
        observations='The sword arm no longer inherits the standing arm rotation on the deeply bent crouch torso. The supporting arm stays nearer the body. Native low drop and return to ragdoll are visible.',
        gaps=['Torso and gaze still lean too far down for comfortable loaded crouching.',
              'Hand contact and sword clearance need a closer multi-angle inspection.',
              'Floor-to-shoulder pickup is not connected; attachment starts after pickup.',
              'Turning, throw, other corpse types and general world ownership remain unfinished.'])
    write(folder/'visual-review.json',review)
    row=dict(video=take+'/native.mp4',character=role,label=f'{role.title()} · Crouched carry contact comparison',
        alias='native_crouched_carry_contact_comparison',carry_attachment=True,carry_release=True,travelling=True,
        gameplay_bound=True,complete=False,visual_accepted=False,automatic_pickup=False,
        scope='Separate arm/contact correction on the same native Mayhew carry and crouched low-drop experiment. Visually inspected comparison, not accepted final movement.',
        requirements=['carry_body_lift_idle_move_drop_throw'],evidence=take+'/validation.json',
        visual_review=take+'/visual-review.json',actual_capture_hz=v['actual_capture_hz'],
        timing='Actual elapsed capture; no speedup',proxy_sha256=v['identity']['dllSha256'],
        bank_sha256=v['identity']['bankSha256'],source_role=v['identity']['role'],observer_role=v['identity']['peer'])
    manifest=[r for r in manifest if r['video']!=row['video']];manifest.append(row)
    cases[role]=dict(video=row['video'],validation=row['evidence'],visual_review=row['visual_review'],
        checks=len(v['checks']),samples=v['samples'],bank_sha256=row['bank_sha256'])
a=read(P/'review-evidence-annotations.json');item=a['items']['carry_body_lift_idle_move_drop_throw']
item['contact_runtime_evidence']=dict(cases=cases,complete=False,visual_accepted=False,manifest='carry-contact-bank-manifest-v2.json')
item['notes']='Current game recordings cover both characters carrying the exact Mayhew body while standing, walking forward and crouching, then using native standing/crouched low drops. A separate crouched contact comparison corrects the sword arm and supporting arm; torso posture and contact remain unaccepted. This is an explicitly armed Dunwall experiment. Continuous pickup, throw, other corpse types and general world ownership are unfinished. Earlier source-only, empty-shoulder and timer-release recordings remain available for comparison.'
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',a)
write(P/'carry-contact-runtime-findings-v2.json',dict(cases=cases,complete=False,visual_accepted=False))
state=read(P.parent/'current-task-state.json');m=state['MovementIntegration']
m['CarryAttachmentExperiment']['contact_cases']=cases
m['LatestContinuation']['actual_game_videos']=len(manifest)
state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(P.parent/'current-task-state.json',state)
print(json.dumps(dict(videos=len(manifest),contact_comparisons=len(cases),complete=False)))
