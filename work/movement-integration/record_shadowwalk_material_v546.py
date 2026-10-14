from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat();rollback=rd(W/'shadowwalk-material-rollback-v546.json')
assert rd(W/'native-operation-owner.json')['state']=='finished' and rollback['checkpointRestored']
assert sha(R/'work/asset-game/dinput8.dll')==rollback['dll']==rd(W/'candidate-shadowwalk-fx-v533/identity.json')['dll']
before='native-shadowwalk-material-v540b';source='native-shadowwalk-material-source-v542';trial='native-shadowwalk-material-v545'
assert rd(N/before/'result.json')['passed'] and rd(N/source/'result.json')['passed']
result=rd(N/trial/'result.json');assert not result['passed'] and result['failure']=='Native material values differ from bounded candidate'
rows=rd(N/trial/'states.json');steady=[r for r in rows if 4000<r['ms']<10500 and r['materialState']['active'] and r['materialState']['variant']==33 and r['peerMaterial']['stable']]
assert len(steady)>=5
for r in steady:
 p={x['id']:x for x in r['peerMaterial']['parameters']};assert p[7]['words'][0]==4 and abs(p[6]['floats'][0]-.45)<.0001
last=rows[-1]['materialState'];assert last['starts']==last['restores']==1 and last['refused']==0 and not last['active']
for whom in ('peer','observer'):
 a=rd(N/trial/f'{whom}-material-before.json');b=rd(N/trial/f'{whom}-material-after.json');assert a['stable'] and b['stable'] and a['parameters']==b['parameters']
audit=dict(at=now,rawResultPassed=False,steadySamples=len(steady),last=last,peerTableRestored=True,observerTableUnchanged=True,
 testFailureExplanation='One row at11624ms reads stale published variant33/fade0.45 followed by native exit fade0.432699. Each read is locally stable but the combined row is asynchronous. The assertion incorrectly treated it as a coherent settled-loop observation. Original failed result is retained; no rerun is warranted to accept a visibly poor candidate.',
 visualDecision='Rejected: bright white/gold teleportation patches, not a shadow form. Restored verified533. Correct native whole-body material binding remains unresolved.',complete=False)
wr(W/'shadowwalk-material-feasibility-v546.json',audit)
reviews={
 before:dict(inspected=True,sheets=['material-observation-sheet.jpg'],observations=['Ordinary human body crawl and recovery visible; column partly occludes entry.','No new material applied in this observation.']),
 source:dict(inspected=True,sheets=['source-sheet.jpg'],observations=['Native low camera, shadow vision and first-person recovery visible.','This is source reference only.']),
 trial:dict(inspected=True,sheets=['material-sheet.jpg'],observations=['Candidate produces bright white/gold patches on the crawling body.','Native material parameters clear and the normal body returns.','Visual candidate rejected; native whole-body shadow material still needed.'])}
manifest=rd(N/'runtime-video-manifest.json')
for take in (before,source,trial):
 review=dict(at=now,**reviews[take],visualAccepted=False,complete=False,method='Actual video temporal sheet viewed; continuous motion quality not certified.')
 wr(N/take/'visual-review.json',review);res=rd(N/take/'result.json')
 for suffix in (('',)if take==source else('','-source')):
  video=N/(take+suffix+'.mp4');assert video.exists() and not any(x['video']==video.name for x in manifest)
  meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
  isSource=bool(suffix)or take==source
  manifest.append(dict(video=video.name,character='emily',label=('Native Shadow Walk source'if isSource else'Shadow Walk material '+('rejected'if take==trial else'baseline'))+' ·'+take.split('-v')[-1],alias='shadow_walk_native_reference'if isSource else'shadow_walk_material_experiment',requirements=['emily_shadow_walk_enter_move_attack_exit'],gameplay_bound=not isSource,complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=take+'/visual-review.json',proxy_sha256=res['dll'],video_sha256=sha(video),actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),checks_passed=res['passed'],scope=res['scope']))
wr(N/'runtime-video-manifest.json',manifest)
wr(W/'candidate-shadowwalk-material-v544/runtime-rejection-v546.json',dict(audit=audit,rollback=rollback,result=result))
wr(W/'candidate-material-probe-v541/runtime-verification-v546.json',dict(sourceResult=rd(N/source/'result.json'),correlation='native-shadowwalk-material-correlation-v543.json',scope='Read-only exact native helper parameter identity and stable value observation; no broad rendering claim.'))
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=now
latest='Verified533 body/native particles retained. Read-only541/542 identifies native effect4 and fade on temporary arm helpers;251/252 material observations stable.544 per-entity partial-fade candidate visibly fails (bright white/gold patches) and is rolled back546. Native setter produced observed values and one exact cleanup with zero refusals, but raw545 test fails on one asynchronous entry/exit sample; failure retained.540 startup freshness refusal remains open;540b passes15 after retry. Complete Shadow Walk/movement/co-op remain unfinished.'
state['ShadowWalkContinuation'].update(latest=latest,receipt='work/movement-integration/shadowwalk-material-feasibility-v546.json',materialCandidateRejected=True,materialComplete=False,next='Continue functional Shadow Walk crouched entry, directions, attack/upgrade/traversal and interruption coverage on verified533. Defer shadow material binding/polish with concrete native renderer findings retained. Keep509 air landing and540 startup freshness issues open; defer rare partner interactions and preserve movement collision.')
wr(R/'work/current-task-state.json',state)
ann=rd(N/'review-evidence-annotations.json');ann['items']['emily_shadow_walk_enter_move_attack_exit'].update(notes=latest,complete=False);wr(N/'review-evidence-annotations.json',ann)
p=D/'36-native-shadow-walk-investigation.md';s=p.read_text(encoding='utf-8')
s+='''

## Material visual rejection545 and rollback546

[545](../../work/natural-moveset/native-shadowwalk-material-v545/result.json) preserves a failed assertion. At11624ms, the slower published diagnostic still reports loop33/fade0.45, while the later external material read already sees exit fade0.432699. These are individually stable observations from different times; the combined test row was incorrectly treated as atomic. [Offline audit546](../../work/movement-integration/shadowwalk-material-feasibility-v546.json) separately verifies settled-loop values, one start/81updates/one removal, zero native refusals, the exact restored peer table and unchanged observer table. It does not rewrite the failed test or establish interruption reliability. Body/particles and checkpoint/protected-file checks pass within the original result.

The actual viewed observer sheet is decisive: partial native effect4 on ordinary third-person materials produces bright white/gold patches. It is visually rejected. The native Shadow Walk helper material recipe must be bound correctly before this can become a shadow appearance; further arbitrary fade tuning is deferred. [Rollback546](../../work/movement-integration/shadowwalk-material-rollback-v546.json) restores the exact verified533 DLL and its source bridge. Experimental implementation544 is archived and disconnected from the live source; read-only inspectors and material tests remain available. Both isolated checkpoints and original protected files restore. The dashboard contains the baseline, native source and rejected candidate videos with incomplete/unaccepted status. Continue functional movement coverage next, keeping the material gap explicit.
'''
p.write_text(s,encoding='utf-8')
print(json.dumps(dict(videos=len(manifest),steadySamples=len(steady),rollback=rollback['candidate'],materialAccepted=False)))
