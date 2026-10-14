"""Save crouched source continuity and bounded snapshot-cancel recovery."""
import datetime,hashlib,json,shutil
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
    t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-carry-crouch-v21';backup.mkdir(exist_ok=True)
for n in ['runtime-video-manifest.json','review-evidence-annotations.json']:
    if not(backup/n).exists():shutil.copy2(P/n,backup/n)
manifest=read(P/'runtime-video-manifest.json');sources={};cancels={}
for role in ['corvo','emily']:
    take='native-pickup-crouched-corvo-v21b'if role=='corvo'else'native-pickup-crouched-emily-v21'
    folder=P/take;v=read(folder/'stream-validation.json');g=read(folder/'geometry-analysis.json')
    assert v['passed']and all(g['checks'].values())
    assert hashlib.sha256((folder/'native.mp4').read_bytes()).hexdigest().upper()==v['video_sha256']
    observed=read(folder/'observations.json')
    prep=[r['native']for r in observed if any(a.get('carryResearch',{}).get('phase110')==1 for a in (r.get('native')or{}).get('active',[]))]
    assert prep and all(r['stance']==1 and {a['id']for a in r['active']}=={6,45,53}for r in prep)
    row=dict(video=take+'/native.mp4',character=role,label=f'{role.title()} · Crouched pickup stream continuity',
        alias='native_crouched_pickup_stream_continuity',native_source_only=True,gameplay_bound=False,complete=False,visual_accepted=False,
        requirements=['carry_body_lift_idle_move_drop_throw'],evidence=take+'/stream-validation.json',actual_capture_hz=g['actual_capture_hz'],
        proxy_sha256=v['identity']['dllSha256'],source_role=v['identity']['role'],
        scope='Native first-person crouched pickup reference with two-peer telemetry. Source and receiver stay ready through exact Sneaking6/Carry45/Equip53 preparation, native In and Loop. No restart or remote body-lift claim.',timing='Actual elapsed capture; no speedup')
    manifest=[r for r in manifest if r['video']!=row['video']];manifest.append(row)
    sources[role]=dict(video=row['video'],evidence=row['evidence'],checks=len(v['checks']),transitions=v['transitions'])
    take=f'carry-snapshot-cancel-{role}-v21';folder=P/take;c=read(folder/'snapshot-cancel-validation.json');assert c['passed']
    assert hashlib.sha256((folder/'native.mp4').read_bytes()).hexdigest().upper()==c['video_sha256']
    review=dict(inspected=True,accepted=False,complete=False,method='Actual game temporal sheet at2.1,2.3,2.6,2.9,3.3,4.2 seconds.',
        observations=('Corvo falls and settles without the v20 large upward launch; a small pose discontinuity remains.' if role=='corvo' else 'Emily still launches upward and sideways at 2.6-2.9 seconds. Mechanical cleanup passes but the visual handoff fails.'),
        limitations=['Small pose/contact discontinuities still need visual refinement.','This is an explicit bounded cancellation test, not a complete body pickup action.'])
    write(folder/'visual-review.json',review)
    row=dict(video=take+'/native.mp4',character=role,label=f'{role.title()} · Blend cancellation with native ragdoll handoff',
        alias='native_carry_snapshot_cancel_recovery',carry_snapshot=True,gameplay_bound=False,complete=False,visual_accepted=False,
        requirements=['carry_body_lift_idle_move_drop_throw','ragdoll_handoff'],evidence=take+'/snapshot-cancel-validation.json',
        visual_review=take+'/visual-review.json',actual_capture_hz=c['actual_capture_hz'],
        scope='The pose blend remains bound until native ragdoll activation completes. Corvo improves but Emily still launches upward. Ownership, native physics and health checks pass; visual handoff and complete pickup remain unfinished.',timing='Actual elapsed capture; no speedup')
    manifest=[r for r in manifest if r['video']!=row['video']];manifest.append(row)
    cancels[role]=dict(video=row['video'],evidence=row['evidence'],checks=len(c['checks']),cleanup_delay_ms=c['cleanup_delay_ms'],visual_observations=review['observations'],visual_accepted=False)
candidate=P.parent/'movement-integration/candidate-carry-crouch-v21';identity=read(candidate/'identity.json')
identity.update(runtimeValidated=True,source_cases=sources,cancel_cases=cancels,
    scope='Both-role crouched native pickup semantic continuity and bounded pre-attachment snapshot cancellation. Complete third-person carrier lifting, automatic body authority and movement phase remain unfinished.')
write(candidate/'identity.json',identity)
findings=dict(build=identity['sha256'],source_cases=sources,cancel_cases=cancels,
    baseline_failure='native-pickup-crouched-corvo-v20/stream-validation.json',
    visual_failure='carry-snapshot-cancel-corvo-v20/visual-review.json',
    confirmed=['Exact crouched preparation has stance1 and Sneaking6/Carry45/Equip53 before a scene exists.',
               'Matching stance/base-state recognition keeps both source and receiver streams ready; existing timeout and target identity checks remain.',
               'During early cancellation, preserve the current snapshot weight until PhysicsSystem mode0 has applied, then remove scene and snapshot bindings.',
               'Native ownership cleanup and health checks pass for both cancellation cases; Corvo settles, but Emily still launches upward.'],
    incomplete=['Full-body carrier pickup acting and exact body/hand contact.','Automatic native pickup timing and general target authority.','Crouch contact polish, throw and remaining releases.'],
    complete=False,visual_accepted=False)
write(P/'carry-crouch-findings-v21.json',findings)
a=read(P/'review-evidence-annotations.json');item=a['items']['carry_body_lift_idle_move_drop_throw']
item['crouched_pickup_stream_continuity']=sources;item['snapshot_cancel_recovery']=cancels
item['notes']='Both roles have isolated Dunwall recordings for loaded standing/crouched movement, native low drops, snapshot blending and interrupted ownership cleanup. v21 keeps crouched pickup streams alive. Pre-attachment cancellation improves for Corvo but Emily still launches upward; mechanical checks alone do not establish visual success. Actual carrier lifting action, matching contact, automatic source-timed pickup, throws, other body types and general world ownership remain incomplete. All first-person references and rejected comparisons are labelled separately.'
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',a)
state=read(P.parent/'current-task-state.json');m=state['MovementIntegration'];entry=m['CarryAttachmentExperiment']
entry.update(status='v21 both-role crouched source continuity passes. Cancellation mechanical checks pass but Emily still launches upward; the visual fix is incomplete.',
    latest_candidate='candidate-carry-crouch-v21',latest_completed='carry-snapshot-cancel-emily-v21',crouched_pickup_sources=sources,snapshot_cancel_recovery=cancels,
    findings='work/natural-moveset/carry-crouch-findings-v21.json',next='Fix Emily snapshot cancellation physics impulse, then connect native preparation/lift timing to authored full-body carrier action and victim blending.',complete=False,visual_accepted=False)
m['LatestContinuation'].update(live=identity['sha256'],liveBuild=identity['sha256'],last_validated_source_fix=identity['sha256'],actual_game_videos=len(manifest),next=entry['next'],complete=False)
state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(P.parent/'current-task-state.json',state)
print(json.dumps(dict(videos=len(manifest),sources=sources,cancels=cancels,build=identity['sha256'],complete=False)))
