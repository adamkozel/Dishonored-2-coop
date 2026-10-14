"""Persist exact runtime continuity evidence without marking pickup complete."""
import datetime,json,hashlib,shutil
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
    t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-carry-preparation-v17';backup.mkdir(exist_ok=True)
for name in ['runtime-video-manifest.json','review-evidence-annotations.json']:
    if not(backup/name).exists():shutil.copy2(P/name,backup/name)
manifest=read(P/'runtime-video-manifest.json');cases={}
for role in ['corvo','emily']:
    take=f'native-pickup-stream-{role}-v17';folder=P/take
    v=read(folder/'stream-validation.json');g=read(folder/'geometry-analysis.json')
    assert v['passed'] and all(g['checks'].values())
    assert hashlib.sha256((folder/'native.mp4').read_bytes()).hexdigest().upper()==v['video_sha256']
    row=dict(video=take+'/native.mp4',character=role,label=f'{role.title()} · Pickup stream continuity',
        alias='native_pickup_stream_continuity',native_source_only=True,gameplay_bound=False,complete=False,visual_accepted=False,
        requirements=['carry_body_lift_idle_move_drop_throw'],evidence=take+'/stream-validation.json',
        actual_capture_hz=g['actual_capture_hz'],proxy_sha256=v['identity']['dllSha256'],source_role=v['identity']['role'],
        scope='First-person reference plus two-peer telemetry: stream armed before pickup remains ready through preparation, native In and Loop without restarting. No remote body attachment or complete third-person lifting is demonstrated.',
        timing='Actual elapsed capture; no speedup')
    manifest=[r for r in manifest if r['video']!=row['video']];manifest.append(row)
    cases[role]=dict(evidence=row['evidence'],video=row['video'],checks=len(v['checks']),transitions=v['transitions'],actual_capture_hz=g['actual_capture_hz'])
candidate=P.parent/'movement-integration/candidate-carry-preparation-v17'
identity=read(candidate/'identity.json');identity.update(runtimeValidated=True,cases=cases,
    scope='Exact standing native pickup preparation on both roles. Both source and observer streams continuously ready; no handshake restart. Complete-body lift still unfinished.')
write(candidate/'identity.json',identity)
findings=dict(build=identity['sha256'],baseline_failure='native-pickup-stream-corvo-v16/stream-validation.json',cases=cases,
    confirmed='Native phase1 Carry45 owns its target before a scene exists. Running0+Carry45+Equip53 is a valid temporary grounded state. The previous reader rejected it and the peer stopped as stale.',
    fix='Strict read-only preparation reader plus existing unique authored target resolution. Send grounded presentation until a validated native carry scene begins. Existing 250ms timeout stays unchanged.',
    remaining=['No authored remote lifting clip connected during native In.', 'Remote victim pickup and snapshot blending remain separate experiments.', 'Crouched preparation has not been observed and remains refused.'],complete=False)
write(P/'carry-preparation-findings-v17.json',findings)
a=read(P/'review-evidence-annotations.json');a['items']['carry_body_lift_idle_move_drop_throw']['pickup_stream_continuity']=findings
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',a)
state=read(P.parent/'current-task-state.json');m=state['MovementIntegration'];entry=m['CarryAttachmentExperiment']
entry.update(pickup_stream_continuity=cases,pickup_stream_build=identity['sha256'],
    snapshot_experiment='candidate-carry-snapshot-v18: optional native recovery-pose blend; runtime validation pending.',
    next='Verify optional native snapshot blend visually and mechanically; connect full-body native-timed pickup only after feasibility is established.',complete=False,visual_accepted=False)
m['LatestContinuation'].update(actual_game_videos=len(manifest),last_validated_source_fix=identity['sha256'],
    next=entry['next'],complete=False)
state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(P.parent/'current-task-state.json',state)
print(json.dumps(dict(videos=len(manifest),cases=cases,complete=False)))
