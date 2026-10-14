"""Index inspected D2 release recordings without claiming finished carrying."""
import datetime, hashlib, json, shutil
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
    t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-carry-release-v13';backup.mkdir(exist_ok=True)
for n in ['runtime-video-manifest.json','review-evidence-annotations.json','review.html','review-catalog.js']:
    if not (backup/n).exists():shutil.copy2(P/n,backup/n)
manifest=read(P/'runtime-video-manifest.json');cases={}
for role in ['corvo','emily']:
    for crouch in [False,True]:
        take=f'carry-victim-native-{role}'+('-crouched'if crouch else'')+'-v13';folder=P/take
        v=read(folder/'validation.json')
        assert v['mechanical_checks_passed'] and v['native_victim_animation'] and v['native_victim_drop_animation']
        assert hashlib.sha256((folder/'native.mp4').read_bytes()).hexdigest().upper()==v['video_sha256']
        stance='crouched'if crouch else'standing'
        review=dict(inspected=True,accepted=False,complete=False,
            method='Actual D2 temporal contact sheets inspected at loaded standing, crouched carrying, release and ragdoll recovery. Recorded playback uses elapsed capture time.',
            improvement='Source low-drop state now drives the carrier release and the victim native drop clip. The scene retires and native ragdoll resumes before the test lease expires.',
            observations=('The crouched carrier stays low through release; torso pitch, supporting-hand contact and recovery need refinement.' if crouch else 'The loaded shoulder releases the body and the carrier recovers toward armed standing; contact and weight transfer still need refinement.'),
            gaps=['Attachment starts after the native source pickup; no continuous floor-to-shoulder lift yet.',
                'Visible attachment pop occurs in some takes; initial root tracking fails in two of this four-case matrix.',
                'Supporting-hand contact, crouched torso posture, sword clearance and recovery are not visually accepted.',
                'Only the exact dead Mayhew body in the isolated Dunwall experiment is supported.',
                'General pickup, throw, other body types, world physics authority and interruption coverage remain unfinished.'])
        write(folder/'visual-review.json',review)
        scope=f'Explicitly armed Mayhew carry experiment with source-driven {stance} low drop. Native victim animation, carrier release and return to ragdoll verified. Pickup continuity, contact, throw and general gameplay integration remain unfinished.'
        row=dict(video=take+'/native.mp4',character=role,label=f'{role.title()} · Native carried body · {stance} drop',
            alias=f'native_carry_{stance}_drop_experiment',carry_attachment=True,carry_release=True,travelling=True,
            gameplay_bound=True,complete=False,visual_accepted=False,automatic_pickup=False,scope=scope,
            requirements=['carry_body_lift_idle_move_drop_throw'],evidence=take+'/validation.json',
            visual_review=take+'/visual-review.json',actual_capture_hz=v['actual_capture_hz'],
            timing='Actual elapsed capture; no speedup',proxy_sha256=v['identity']['dllSha256'],
            bank_sha256=v['identity']['bankSha256'],source_role=v['identity']['role'],observer_role=v['identity']['peer'])
        manifest=[r for r in manifest if r['video']!=row['video']];manifest.append(row)
        cases[role+'_'+stance]=dict(video=row['video'],validation=row['evidence'],visual_review=row['visual_review'],
            checks=len(v['checks']),samples=v['samples'],held_samples=v['held_samples'],post_release_samples=v['post_release_samples'])
annotations=read(P/'review-evidence-annotations.json');item=annotations['items']['carry_body_lift_idle_move_drop_throw']
item['runtime_binding']='partial_carrier_native_victim_and_low_drop_both_roles'
item['release_runtime_evidence']=dict(cases=cases,complete=False,visual_accepted=False,
    source_declarations='carry-release-scenes-v1.json',carrier_banks='carry-release-bank-manifest-v1.json')
item['notes']='Current in-game evidence covers Corvo and Emily carrying the exact Mayhew body while standing, walking forward and crouching, then releasing it through native standing/crouched low-drop clips. The authored carrier follows the source action through Steam, and the victim returns to ragdoll once. This remains an explicitly armed Dunwall experiment. Pickup continuity, shoulder/hand contact, crouched posture, throw, other body types and general world-physics ownership remain unfinished. The earlier source-only, empty-shoulder and timer-release recordings are retained for comparison. Offline lift, high/soft drop and throw studies are authoring candidates, not integrated gameplay.'
html=P/'review.html';s=html.read_text(encoding='utf-8-sig')
old=":r.carry_attachment?'Bounded native corpse attachment experiment. Pickup, release and visual acceptance remain unfinished.'"
new=":r.carry_release?'Source-driven low-drop experiment. Native victim and carrier release are connected; pickup, throw and visual acceptance remain unfinished.'"+old
if new not in s:
    assert s.count(old)==1;s=s.replace(old,new);html.write_text(s,encoding='utf-8')
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',annotations)
write(P/'carry-release-runtime-findings-v13.json',dict(cases=cases,complete=False,visual_accepted=False,
    scope='Grounded stationary low drops after forward standing and crouched carry; dead authored Mayhew only.',
    bank_version=32,build='43A5D79A5E07E056FCBEA7DC17F515F87199CA065BE579B85C7D732E4596B112'))
state=read(P.parent/'current-task-state.json');m=state['MovementIntegration'];a=m['CarryAttachmentExperiment']
a.update(status='v13 four-role/stance cases passed native loop/drop and exact-once cleanup checks; inspected recordings remain visually unaccepted.',
    release_cases=cases,latest_completed='carry-victim-native-corvo-crouched-v13',latest_candidate='candidate-carry-victim-scene-v13',
    next='Test cancellation and stream interruption, then connect a source-timed pickup. Improve shoulder/hand contact and crouched recovery; throw and general corpse authority remain incomplete.',
    complete=False,visual_accepted=False)
m['LatestContinuation']['actual_game_videos']=len(manifest);state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat()
write(P.parent/'current-task-state.json',state)
print(json.dumps(dict(videos=len(manifest),cases=cases,complete=False)))
