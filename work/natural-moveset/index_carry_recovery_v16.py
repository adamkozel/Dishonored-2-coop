"""Index scoped recovery/source evidence, with unfinished movement explicit."""
import datetime,hashlib,json,shutil
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
    t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-carry-recovery-v16';backup.mkdir(exist_ok=True)
for n in ['runtime-video-manifest.json','review-evidence-annotations.json','review.html']:
    if not (backup/n).exists():shutil.copy2(P/n,backup/n)
manifest=read(P/'runtime-video-manifest.json');cases={};sources={}
for role in ['corvo','emily']:
    take=f'carry-cleanup-stream-{role}-v16';folder=P/take;v=read(folder/'validation.json')
    assert v['mechanical_checks_passed']
    assert hashlib.sha256((folder/'native.mp4').read_bytes()).hexdigest().upper()==v['video_sha256']
    damage=read(folder/'damage-observations.json')
    assert damage['carryPresentationDamageBlocked']==1 and not damage['events']
    review=dict(inspected=True,accepted=False,complete=False,
        method='Actual D2 temporal sheets at 5.6, 7.1, 10.7 and 15.8 seconds.',
        observations='The body drops on stream cancellation and remains on the floor after the fresh two-peer handshake. The avatar resumes movement without replaying the retired attachment.',
        gaps=['Initial attachment still pops; full-interval root-follow test fails.',
              'This is explicit interruption recovery, not a complete carry interaction.',
              'Continuous pickup, contact quality, throw and general world ownership remain unfinished.'])
    write(folder/'visual-review.json',review)
    row=dict(video=take+'/native.mp4',character=role,label=f'{role.title()} · Carry interruption recovery',
        alias='native_carry_interruption_recovery',carry_recovery=True,gameplay_bound=True,complete=False,visual_accepted=False,
        automatic_pickup=False,requirements=['carry_body_lift_idle_move_drop_throw'],evidence=take+'/validation.json',
        visual_review=take+'/visual-review.json',actual_capture_hz=v['actual_capture_hz'],proxy_sha256=v['identity']['dllSha256'],
        bank_sha256=v['identity']['bankSha256'],source_role=v['identity']['role'],observer_role=v['identity']['peer'],
        scope='Source action stream stops; exact leased body releases once. Two-sided restart accepts fresh state without replay. One incidental body impact is suppressed and avatar health stays 40. Initial placement, pickup and visual quality remain unfinished.',timing='Actual elapsed capture; no speedup')
    manifest=[r for r in manifest if r['video']!=row['video']];manifest.append(row)
    cases[role]=dict(video=row['video'],evidence=row['evidence'],checks=len(v['checks']),samples=v['samples'],
        avatar_health_after=read(folder/'avatar-health-after.json')['current'],impact_blocks=1,root_follow_passed=v['root_follows_carrier'])
    take=f'native-pickup-geometry-{role}-v1';folder=P/take;g=read(folder/'geometry-analysis.json')
    assert all(g['checks'].values())
    assert hashlib.sha256((folder/'native.mp4').read_bytes()).hexdigest().upper()==g['video_sha256']
    row=dict(video=take+'/native.mp4',character=role,label=f'{role.title()} · Native pickup timing reference',
        alias='native_pickup_geometry_reference',native_source_only=True,gameplay_bound=False,complete=False,visual_accepted=False,
        requirements=['carry_body_lift_idle_move_drop_throw'],evidence=take+'/geometry-analysis.json',
        actual_capture_hz=g['actual_capture_hz'],proxy_sha256=g['identity']['dllSha256'],
        source_role=g['identity']['role'],scope='First-person native source reference only. Read-only traces identify preparation, the one-second lift and the loop. The native body jumps into a raised pose; a continuous third-person lift is still required.',timing='Actual elapsed capture; no speedup')
    manifest=[r for r in manifest if r['video']!=row['video']];manifest.append(row)
    sources[role]=dict(video=row['video'],evidence=row['evidence'],checks=g['checks'])
regression={}
for name in ['friendly-fire-native','friendly-fire-strikes','native-sword-damage-verification']:
    path=P/'carry-impact-v16-regression'/f'{name}.json';v=read(path);assert v['Passed'];regression[name]=str(path.relative_to(P))
candidate=P.parent/'movement-integration/candidate-carry-impact-v16';identity=read(candidate/'identity.json')
identity.update(runtimeValidated=True,scope='Both-role scoped carry interruption/impact tests plus native friendly-fire, real sword contacts and ordinary enemy sword damage. Complete movement remains false.',cases=cases,regression=regression)
write(candidate/'identity.json',identity)
findings=dict(build=identity['sha256'],cases=cases,pickup_sources=sources,regressions=regression,
    fixes=['Fresh action handshake allows the same verified living clone after nonlethal damage without writing health.',
           'Exact leased replicated body impacts are suppressed during presentation and for a two-second release tail; native reclaims and stale identities terminate exclusion.'],
    failures_retained=['v14b: health 39 prevented fresh handshake.',
        'v15: handshake recovered but native impact from the released body still caused one point of avatar damage. Updated health validation correctly marks this case failed.'],
    complete=False,visual_accepted=False,
    next='Connect a source-timed, visually continuous floor pickup. Native snapshot recovery blend is a research candidate; not yet tested. Improve loaded torso/contact, initial attachment continuity and throw.')
write(P/'carry-recovery-findings-v16.json',findings)
a=read(P/'review-evidence-annotations.json');item=a['items']['carry_body_lift_idle_move_drop_throw']
item['recovery_runtime_evidence']=dict(cases=cases,regressions=regression,complete=False)
item['pickup_timing_references']=sources
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',a)
html=P/'review.html';s=html.read_text(encoding='utf-8-sig')
old=":r.carry_release?'Source-driven low-drop experiment."
new=":r.carry_recovery?'Interruption recovery test: exact-once release and fresh stream restart. Complete carrying remains unfinished.'"+old
if new not in s:
    assert s.count(old)==1;s=s.replace(old,new);html.write_text(s,encoding='utf-8')
state=read(P.parent/'current-task-state.json');m=state['MovementIntegration'];entry=m['CarryAttachmentExperiment']
entry.update(status='v16 both-role interruption recovery and body-impact suppression pass. Native standing/crouched low drops and contact comparison recorded. Continuous pickup, initial pop, loaded posture and throw incomplete.',
    latest_candidate='candidate-carry-impact-v16',latest_completed='carry-cleanup-stream-emily-v16',recovery_cases=cases,pickup_timing=sources,
    findings='work/natural-moveset/carry-recovery-findings-v16.json',next=findings['next'],complete=False,visual_accepted=False)
m['LatestContinuation'].update(live=identity['sha256'],liveBuild=identity['sha256'],bank='D2MOVE32 / 231 clips; carry release v1 plus separate crouched-contact v2 comparison; leases disabled between tests',
    actual_game_videos=len(manifest),next=findings['next'],complete=False)
state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(P.parent/'current-task-state.json',state)
print(json.dumps(dict(videos=len(manifest),recovery_cases=cases,pickup_references=2,regressions=regression,build=identity['sha256'],complete=False)))
