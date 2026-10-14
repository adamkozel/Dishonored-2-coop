"""Index reviewed cancellation evidence without claiming complete pickup."""
import datetime,hashlib,json,shutil
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
    t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-carry-pose-v22';backup.mkdir(exist_ok=True)
for n in ['runtime-video-manifest.json','review-evidence-annotations.json']:
    if not (backup/n).exists():shutil.copy2(P/n,backup/n)
manifest=read(P/'runtime-video-manifest.json');cases={}
for take,role in [('carry-snapshot-cancel-emily-v22','emily'),('carry-snapshot-cancel-corvo-v22','corvo'),('carry-snapshot-cancel-emily-v22b','emily'),('carry-snapshot-stream-emily-v22','emily')]:
    f=P/take;c=read(f/'snapshot-cancel-validation.json');ident=read(f/'identity.json')
    assert c['passed'] and hashlib.sha256((f/'native.mp4').read_bytes()).hexdigest().upper()==c['video_sha256']
    stream='stream-' in take
    review=dict(inspected=True,accepted=False,complete=False,method='Actual game temporal sheet: 2.1, 2.3, 2.6, 2.9, 3.3, 4.2 seconds.',
        observations=('Source-stream loss occurs later in the blend than direct cancellation, so the body is already raised farther. It returns to ragdoll and settles; restarting both action streams does not recreate the retired attachment.' if stream else 'Body drops from its current blended pose and settles, without the large upward/sideways launch visible in the v21 Emily baseline. Repeated Emily case confirms this scoped result.'),
        limitations=['The body is raised by an experimental pose blend while the carrier is already in a loaded pose; this is not a complete lifting performance.','Contacts, exact floor settling, arbitrary cancellation times and other body types are not accepted.'])
    write(f/'visual-review.json',review)
    row=dict(video=take+'/native.mp4',character=role,label=role.title()+(' · Pickup blend stream loss and recovery' if stream else ' · Pickup blend cancellation with synchronized physics'),
        alias='native_carry_snapshot_pose_handoff',carry_snapshot=True,gameplay_bound=False,complete=False,visual_accepted=False,
        requirements=['carry_body_lift_idle_move_drop_throw','ragdoll_handoff'],evidence=take+'/snapshot-cancel-validation.json',visual_review=take+'/visual-review.json',actual_capture_hz=c['actual_capture_hz'],proxy_sha256=ident['dllSha256'],
        scope='Bounded Mayhew experiment: native current-pose rigid-body synchronization and zero velocity before ragdoll request. Snapshot, scene, carry flag and attachment ownership retire. This does not establish complete body pickup or general world authority.',timing='Actual elapsed game capture; no speedup')
    manifest=[r for r in manifest if r['video']!=row['video']];manifest.append(row)
    cases[take]=dict(role=role,video=row['video'],evidence=row['evidence'],checks=len(c['checks']),cleanup_delay_ms=c['cleanup_delay_ms'],video_sha256=c['video_sha256'],actual_capture_hz=c['actual_capture_hz'],visual_observations=review['observations'])
candidate=P.parent/'movement-integration/candidate-carry-pose-v22';identity=read(candidate/'identity.json')
identity.update(runtimeValidated=True,cases=cases,scope='Scoped pre-attachment snapshot cancellation; both roles, Emily repeat, and Emily stream loss/rearm. Complete lifting and overall movement remain unfinished.')
write(candidate/'identity.json',identity)
findings=dict(build=identity['sha256'],cases=cases,confirmed=[
    '734d90 copies current native joint transforms into rigid bodies and clears linear/angular velocity.',
    'Calling this exact native routine before requesting ragdoll, with the pose blend still bound, removes the observed v21 Emily launch in two cancellation recordings.',
    'Corvo also settles correctly in the corresponding case; mechanical cleanup/health/Steam transport checks pass.',
    'Emily action-stream loss restores physics and scene ownership; fresh two-sided handshake does not replay the retired attachment.'
],hypotheses=['Residual animated rigid-body velocity and/or unsynchronized body transforms contributed to the previous impulse. This experiment changes both; the individual cause is not isolated.'],remaining=[
    'Full-body carrier gather/hoist acting, synchronized with the actual native pickup clock.',
    'Matching hand contact and victim path without floating, head overlap or foot sliding.',
    'Automatic native pickup, arbitrary corpse poses/types and authoritative world identity.',
    'Throws, remaining releases, other movement bindings, powers and native VFX.'
],complete=False,visual_accepted=False)
write(P/'carry-pose-findings-v22.json',findings)
a=read(P/'review-evidence-annotations.json');item=a['items']['carry_body_lift_idle_move_drop_throw'];item['snapshot_pose_handoff']=cases
item['notes']='v22 synchronizes native body pose and clears velocities before pre-attachment ragdoll handoff. Direct cancellation passes both roles and an Emily repeat; Emily action-stream loss and rearm also clean up without attachment replay. v21 crouched source continuity remains the current reader fix. These are scoped Mayhew experiments: full-body carrier lifting, matching hands/body contact, automatic pickup, arbitrary body authority, throws and remaining releases are incomplete.'
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',a)
state=read(P.parent/'current-task-state.json');m=state['MovementIntegration'];entry=m['CarryAttachmentExperiment']
entry.update(status='v22 scoped pose-to-ragdoll handoff verified in both cancellation roles, Emily repeat and Emily stream loss/rearm. Full pickup acting remains incomplete.',latest_candidate='candidate-carry-pose-v22',latest_completed='carry-snapshot-stream-emily-v22',snapshot_pose_handoff=cases,findings='work/natural-moveset/carry-pose-findings-v22.json',next='Author and visually screen a short complete-body lift, then bind it to native source timing and the victim pose transition.',complete=False,visual_accepted=False)
m['LatestContinuation'].update(live=identity['sha256'],liveBuild=identity['sha256'],actual_game_videos=len(manifest),next=entry['next'],complete=False)
state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(P.parent/'current-task-state.json',state)
print(json.dumps(dict(build=identity['sha256'],videos=len(manifest),cases=len(cases),complete=False)))
