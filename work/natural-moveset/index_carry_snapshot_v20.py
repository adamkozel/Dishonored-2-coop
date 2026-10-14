"""Publish bounded mechanism evidence without claiming a complete pickup."""
import datetime,hashlib,json,shutil
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
    t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-carry-snapshot-v20';backup.mkdir(exist_ok=True)
for name in ['runtime-video-manifest.json','review-evidence-annotations.json','review.html']:
    if not(backup/name).exists():shutil.copy2(P/name,backup/name)
manifest=read(P/'runtime-video-manifest.json');cases={}
for role in ['corvo','emily']:
    take=f'carry-snapshot-{role}-v20';folder=P/take
    v=read(folder/'validation.json');s=read(folder/'snapshot-validation.json');assert v['mechanical_checks_passed']and s['passed']
    assert hashlib.sha256((folder/'native.mp4').read_bytes()).hexdigest().upper()==s['video_sha256']
    review=dict(inspected=True,accepted=False,complete=False,
        method='Actual D2 temporal sheet at 2.0,2.2,2.4,2.6,2.8,3.1,3.4,4.2 seconds.',
        observations='Floor pose blends visibly into the native folded-body loop without the v18 dive below the floor. After attachment the body follows the carrier. First-person and avatar health remain unchanged; native scene, carried flag and ragdoll state restore.',
        gaps=['Carrier is already in its loaded pose; no lifting action or hand-to-body contact during ascent.',
              'Body rises too high before settling into the shoulder offset.',
              'Explicitly armed mechanism only, not automatic source-timed pickup.',
              'General bodies, collision, throws and complete movement remain unfinished.'])
    write(folder/'visual-review.json',review)
    row=dict(video=take+'/native.mp4',character=role,label=f'{role.title()} · Floor-pose blend and attachment',
        alias='native_carry_snapshot_blend',carry_snapshot=True,gameplay_bound=False,complete=False,visual_accepted=False,
        automatic_pickup=False,requirements=['carry_body_lift_idle_move_drop_throw','ragdoll_handoff'],
        evidence=take+'/snapshot-validation.json',visual_review=take+'/visual-review.json',
        actual_capture_hz=s['actual_capture_hz'],proxy_sha256=v['identity']['dllSha256'],bank_sha256=v['identity']['bankSha256'],
        source_role=v['identity']['role'],observer_role=v['identity']['peer'],
        scope='Bounded native recovery-pose blend, followed by reciprocal attachment. Removes the large floor dive and releases cleanly. The carrier does not perform a lift; contact, native pickup timing and automatic body authority remain unfinished.',
        timing='Actual elapsed capture; no speedup')
    manifest=[r for r in manifest if r['video']!=row['video']];manifest.append(row)
    cases[role]=dict(video=row['video'],evidence=row['evidence'],root_follow=v['root_follows_carrier'],
        transition_stamps=s['transition_stamps'],checks=len(v['checks'])+len(s['checks']))
take='carry-snapshot-cancel-corvo-v20';folder=P/take;s=read(folder/'snapshot-cancel-validation.json');assert s['passed']
assert hashlib.sha256((folder/'native.mp4').read_bytes()).hexdigest().upper()==s['video_sha256']
row=dict(video=take+'/native.mp4',character='corvo',label='Corvo · Cancel during floor-pose blend',
    alias='native_carry_snapshot_cancel',carry_snapshot=True,complete=False,visual_accepted=False,gameplay_bound=False,
    requirements=['carry_body_lift_idle_move_drop_throw','ragdoll_handoff'],evidence=take+'/snapshot-cancel-validation.json',
    scope='Cancel while the body is blending and no attachment exists. Ownership restores and no later attachment is created, but the video exposes an upward body launch before settling. Visual failure; handoff order needs repair.',actual_capture_hz=s['actual_capture_hz'],timing='Actual elapsed capture; no speedup')
manifest=[r for r in manifest if r['video']!=row['video']];manifest.append(row)
cases['cancel']=dict(video=row['video'],evidence=row['evidence'],checks=len(s['checks']),cleanup_delay_ms=s['cleanup_delay_ms'],visual_failure='Body launches upward before settling; mechanical cleanup passes but pose-to-physics handoff is not accepted.')
# Preserve the actual failed comparison and make its failure explicit.
take='carry-snapshot-corvo-v18';v=read(P/take/'validation.json')
row=dict(video=take+'/native.mp4',character='corvo',label='Corvo · Rejected early-attachment blend',
    alias='native_carry_snapshot_rejected',carry_snapshot=True,complete=False,visual_accepted=False,gameplay_bound=False,rejected=True,
    requirements=['carry_body_lift_idle_move_drop_throw'],evidence=take+'/validation.json',actual_capture_hz=v['actual_capture_hz'],
    scope='Rejected comparison: retarget activation during the recovery blend pulls the body roughly 1.3m below the floor. Ownership tests alone passed; the visible failure led to deferred setup in v19/v20.',timing='Actual elapsed capture; no speedup')
manifest=[r for r in manifest if r['video']!=row['video']];manifest.append(row)
candidate=P.parent/'movement-integration/candidate-carry-snapshot-v20';identity=read(candidate/'identity.json')
identity.update(runtimeValidated=True,cases=cases,scope='Both-role native pose blending and deferred attachment; cancellation before activation. Carrier acting, automatic native pickup and overall movement remain incomplete.')
write(candidate/'identity.json',identity)
findings=dict(build=identity['sha256'],cases=cases,
    confirmed=['Native recovery snapshot inputs can blend a dead Mayhew body without invoking AI recovery or changing health.',
               'Separate pose blending and retarget activation removes the v18 floor dive in both roles.',
               'Engine-thread transition timestamps resolve the coarse status/probe sampling ambiguity.',
               'Early cancellation restores physics and bindings without creating an attachment.'],
    hypotheses=['The native retarget solver reads an unblended clip anchor while final globalPose includes recovery blending.'],
    rejected='carry-snapshot-corvo-v18/native.mp4',interim=['carry-snapshot-corvo-v19','carry-snapshot-emily-v19'],
    remaining=['Fix upward body launch on early snapshot cancellation.','Source-timed full-body carrier lift and matching hands/body contact.','Lower peak victim height and improve shoulder settling.','Automatic native pickup and general body identity/authority.','Throws, other releases and complete movement coverage.'],
    complete=False,visual_accepted=False)
write(P/'carry-snapshot-findings-v20.json',findings)
a=read(P/'review-evidence-annotations.json');item=a['items']['carry_body_lift_idle_move_drop_throw']
item['snapshot_runtime_evidence']=findings
note=' Native recovery-pose blending removes the large floor dive in both-role bounded tests. Early cancellation restores ownership but visibly launches the body before it settles. Carrier lifting action and automatic native timing remain unfinished.'
if note not in item['notes']:item['notes']+=note
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',a)
html=P/'review.html';s=html.read_text(encoding='utf-8-sig');old=":r.carry_recovery?'Interruption recovery test:"
new=":r.carry_snapshot?'Native floor-pose blend experiment. Carrier lifting action, native timing and visual acceptance remain unfinished.'"+old
if new not in s:
    assert s.count(old)==1;s=s.replace(old,new);html.write_text(s,encoding='utf-8')
state=read(P.parent/'current-task-state.json');m=state['MovementIntegration'];entry=m['CarryAttachmentExperiment']
entry.update(status='v20 both-role pose blend/deferred attachment passes mechanical checks. Early cancellation restores ownership but launches the body; visual handoff is not accepted. Actual carrier lift and automatic native pickup still incomplete.',
    latest_candidate='candidate-carry-snapshot-v20',latest_completed='carry-snapshot-cancel-corvo-v20',snapshot_experiment=findings,
    findings='work/natural-moveset/carry-snapshot-findings-v20.json',next='Observe crouched native pickup preparation and preserve semantic stream continuity, then connect source-timed carrier acting and victim blend.',complete=False,visual_accepted=False)
m['LatestContinuation'].update(live=identity['sha256'],liveBuild=identity['sha256'],actual_game_videos=len(manifest),next=entry['next'],complete=False)
state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(P.parent/'current-task-state.json',state)
print(json.dumps(dict(videos=len(manifest),cases=cases,build=identity['sha256'],complete=False)))
