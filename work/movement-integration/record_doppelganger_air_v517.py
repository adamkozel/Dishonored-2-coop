from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat()
manifest=rd(N/'runtime-video-manifest.json');cases=[]
takes=['doppelganger-air-landing-trace-v513','doppelganger-air-entry-holstered-v510']
identity=rd(W/'candidate-doppelganger-air-v512/identity.json')
for take in takes:
    result=rd(N/take/'result.json')
    assert result['passed'] and len(result['checks'])==20 and result['dll']==identity['dll']
    assert any(x.startswith('Both isolated checkpoints restore') for x in result['checks'])
    assert any(x.startswith('All14 protected') for x in result['checks'])
    cases.append(dict(take=take,passed=True,checks=20,air=rd(N/take/'air-verification.json'),continuity=rd(N/take/'continuity.json')))
    for suffix in ('','-source'):
        source=bool(suffix);sheet='source-sheet.jpg' if source else 'remote-sheet.jpg'
        assert (N/take/sheet).exists()
        notes=(['Native first-person casting hand, white summon effect, camera height change and clone arrival are visible.','The sword remains drawn in513 and absent in510.'] if source else ['Remote Emily raises her left arm while leaving the floor, lands and resumes standing; leg poses change during the jump.','The source clone arrives separately with refractive native summon effects.','Drawn sword retained in513; no drawn sword visible in510.'])
        review=N/take/('visual-review'+suffix+'.json')
        wr(review,dict(inspected=True,at=now,method='Viewed six labelled frames from actual game video at1.8,2.5,3.2,3.8,4.4,5.2seconds.',sheet=sheet,observations=notes,complete=False,visualAccepted=False,limitations=['Sampled images do not establish continuous-motion polish or fine hand alignment.','These successful runs do not resolve intermittent landing failure509.']))
        video=N/(take+suffix+'.mp4')
        if any(x['video']==video.name for x in manifest):continue
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
        manifest.append(dict(video=video.name,character='emily',label='Doppelganger airborne · '+take+suffix,alias='power_doppelganger_air',requirements=['emily_doppelganger_cast'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=result['dll'],video_sha256=sha(video),actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),checks_passed=True,scope='Candidate512 paired airborne casting, clone and native effect tests pass in these takes. Earlier509 intermittent landing dropout remains unresolved. Shared clone gameplay and final visual acceptance pending.'))
failed=rd(N/'doppelganger-air-release-armed-v509/result.json');assert not failed['passed']
receipt=dict(at=now,recordedCasePasses=True,airborneContinuityComplete=False,complete=False,identity=identity,cases=cases,unresolved=dict(take='doppelganger-air-release-armed-v509',failure=failed['failure']),next='Retain bounded first-failure movement/root trace. Do not broaden motion guards without evidence. Continue Shadow Walk and larger co-op gaps; revisit this intermittent landing failure with a concrete trace.')
wr(W/'doppelganger-air-verification-v517.json',receipt)
wr(W/'candidate-doppelganger-air-v512/runtime-observations-v517.json',receipt)
wr(N/'runtime-video-manifest.json',manifest)
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=now
state['DoppelgangerContinuation']['airborneResearch'].update(receipt='work/movement-integration/doppelganger-air-verification-v517.json',recordedPasses=takes,passed=False,complete=False,next=receipt['next'])
state['DoppelgangerContinuation']['latest']+=' Airborne517: diagnostic512 passes paired513/510,20checks each; earlier509 landing dropout remains unresolved. Actual source/remote recordings reviewed in temporal samples; no final visual acceptance.'
wr(R/'work/current-task-state.json',state)
chapter=D/'35-doppelganger-cast-and-clone-lifecycle.md'
s=chapter.read_text(encoding='utf-8');marker='## Diagnostic reruns510/513, receipt517'
assert marker not in s
s+='''

## Diagnostic reruns510/513, receipt517

On diagnostic512, [armed release513](../../work/natural-moveset/doppelganger-air-landing-trace-v513/result.json) and [holstered entry510](../../work/natural-moveset/doppelganger-air-entry-holstered-v510/result.json) each pass20 checks. These include source/remote phase identity, airborne body/leg blend, continuous body weight, landing recovery,219-joint clone presentation, native summon/disappearance effects, inventories/health and mandatory checkpoint/protected14 restoration. [Receipt517](../../work/movement-integration/doppelganger-air-verification-v517.json) preserves each measured result and the earlier failed509.

**The intermittent landing problem is still open.** Neither rerun triggered the new diagnostic; its frozen arrays are empty. No movement bound was loosened and no repair of the509 boundary has been demonstrated. Candidate495 remains the preserved ground/cancellation milestone. The task now continues to Shadow Walk rather than repeatedly pursuing the same intermittent edge case without new evidence.

All four actual source/remote recordings are indexed and their six-frame contact sheets were inspected. Emily raises her casting arm, leaves the floor with changing leg poses, lands and stands; a separate clone arrives with native refraction effects.513 retains the sword,510 remains holstered. These temporal samples cannot establish final smoothness or fine hand alignment. Gameplay clone authority and broader animation acceptance remain open.
'''
chapter.write_text(s,encoding='utf-8')
readme=D/'README.md';s=readme.read_text(encoding='utf-8')
if '(35-doppelganger-cast-and-clone-lifecycle.md)' not in s:readme.write_text(s.rstrip()+'\n- [Emily Doppelganger casting, clone presentation and native effects](35-doppelganger-cast-and-clone-lifecycle.md)\n',encoding='utf-8')
print(json.dumps(dict(videos=len(manifest),recordedPasses=takes,unresolved509=True,complete=False)))
