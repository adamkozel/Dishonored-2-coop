"""Archive carrier-only screening evidence; never marks pickup complete."""
import datetime,hashlib,json,shutil,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):
    t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
backup=P/'catalog-before-short-lift-v6';backup.mkdir(exist_ok=True)
for n in ['runtime-video-manifest.json','review-evidence-annotations.json']:
    if not (backup/n).exists():shutil.copy2(P/n,backup/n)
manifest=read(P/'runtime-video-manifest.json');cases={}
for version in [4,6]:
    clips={r['role']:r for r in read(P/f'body-lift-short-manifest-v{version}.json')}
    for role in ['corvo','emily']:
        take=f'short-lift-{role}-v{version}';f=P/take
        result=read(f/'result.json');ident=read(f/'identity.json');rows=read(f/'observations.json')
        assert result['passed'] and ident['clipSha256']==clips[role]['sha256']
        assert hashlib.sha256((P/clips[role]['binary']).read_bytes()).hexdigest()==ident['clipSha256']
        active=[r for r in rows if r['review']['active'] and 0<r['review']['seconds']<=1.4]
        checks={'harness_passed':result['passed'],'both_views':set(r['view']for r in active)=={'side','front'},
                'no_review_refusals':all(not r['review']['refused']for r in rows),
                'native_holster_during_motion':all(r['equipment']['active'] and r['equipment']['result']=='native_holstered'for r in active),
                'health_preserved':all(ident['healthBefore'][k]['current']==ident['healthAfter'][k]['current']for k in ['source','observer','avatar'])}
        probe=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(f/'native.mp4')]))
        a,b=map(int,probe['streams'][0]['avg_frame_rate'].split('/'));hz=a/b;checks['capture_at_least_24hz']=hz>=24
        validation=dict(passed=all(checks.values()),checks=checks,active_samples=len(active),actual_capture_hz=hz,
            video_sha256=hashlib.sha256((f/'native.mp4').read_bytes()).hexdigest().upper(),clip_sha256=ident['clipSha256'])
        assert validation['passed'],validation
        review=dict(inspected=True,accepted=False,complete=False,method='Actual D2 side/front temporal contact sheets, plus paired authoring preview.',
            observations=('Deep near-kneeling gather rejected as the final performance.' if version==4 else 'Higher knees than v4, but the gather remains a deep forward bend. Rise and shoulder settle play in D2; contact is not established because the actual game capture has no victim partner.'),
            limitations=['Carrier-only review, not native pickup gameplay binding.','Authoring partner is a grey native skeleton stand-in, not the live Mayhew body.','Needs paired contact, native timing, crouched entry and cancellation comparisons.'])
        write(f/'validation.json',validation);write(f/'visual-review.json',review)
        row=dict(video=take+'/native.mp4',character=role,label=f'{role.title()} · Short lift v{version} · carrier-only study',
            alias='carry_body_short_lift_study' if version==6 else 'carry_body_short_lift_study_v4_baseline',
            requirements=['carry_body_lift_idle_move_drop_throw'],gameplay_bound=False,pose_only=True,complete=False,visual_accepted=False,
            evidence=take+'/validation.json',visual_review=take+'/visual-review.json',actual_capture_hz=hz,proxy_sha256=ident['dllSha256'],
            scope=ident['scope'],timing='Actual elapsed capture; no speedup',clip_sha256=ident['clipSha256'])
        manifest=[r for r in manifest if r['video']!=row['video']];manifest.append(row);cases[take]=dict(**validation,visual_review=row['visual_review'])
write(P/'runtime-video-manifest.json',manifest)
a=read(P/'review-evidence-annotations.json');a['items']['carry_body_lift_idle_move_drop_throw']['short_lift_screening']=cases;write(P/'review-evidence-annotations.json',a)
state=read(P.parent/'current-task-state.json');m=state['MovementIntegration'];m['ShortLiftStudy']=dict(version=6,cases=cases,
    manifest='work/natural-moveset/body-lift-short-manifest-v6.json',pose_tests='work/natural-moveset/body-lift-short-v6-pose-test/result.json',
    roundtrip='work/natural-moveset/body-lift-short-v6-roundtrip.json',gameplay_bound=False,complete=False,visual_accepted=False)
m['LatestContinuation']['actual_game_videos']=len(manifest);state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(P.parent/'current-task-state.json',state)
print(json.dumps(dict(actual_game_videos=len(manifest),screened_cases=len(cases),accepted=False)))
