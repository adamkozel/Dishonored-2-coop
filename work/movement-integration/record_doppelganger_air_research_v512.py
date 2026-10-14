from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat();manifest=rd(N/'runtime-video-manifest.json')
for take in ('native-doppelganger-air-release-v506','native-doppelganger-air-entry-holstered-v507','doppelganger-air-release-armed-v509'):
    native=take.startswith('native-');result=rd(N/take/'result.json')
    for suffix in (('',) if native else ('','-source')):
        video=N/(take+suffix+'.mp4');review=N/take/('visual-review'+suffix+'.json')
        inspected=native or not suffix;sheet='source-sheet.jpg' if native else 'remote-sheet.jpg'
        notes=(['Native camera rises through jump; left casting hand and white placement preview are followed by a summon burst and separate clone.','Original first-person view returns to the test floor; sword is sheathed throughout507 and retained in506.'] if native else ['Remote caster jumps with changing leg and casting-arm poses, then returns to ground with the sword visible.','This take fails the numeric landing continuity check: one vertical step resets body weight to zero; sampled images do not negate that defect.'])
        wr(review,dict(inspected=inspected,at=now,method='Viewed temporal contact sheet from the actual game video.' if inspected else 'Not visually inspected.',sheet=sheet if inspected else None,observations=notes if inspected else [],complete=False,visualAccepted=False,limitations=['Temporal samples cannot establish continuous-motion polish.','Native first-person reference only.' if native else 'Dark remote actor and recorded landing dropout prevent visual acceptance.']))
        if any(x['video']==video.name for x in manifest):continue
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
        manifest.append(dict(video=video.name,character='emily',label='Doppelganger airborne research · '+take+suffix,alias='power_doppelganger_air',requirements=['emily_doppelganger_cast'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=result['dll'],video_sha256=sha(video),actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),checks_passed=result['passed'],scope='506/507 source-only native airborne evidence.509 is a failed paired landing-continuity test, preserved without waiving its assertion. Shared gameplay and broader animation quality are unfinished.'))
wr(N/'runtime-video-manifest.json',manifest)
identity=rd(W/'candidate-doppelganger-air-v512/identity.json')
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=now
state['DoppelgangerContinuation']['airborneResearch']=dict(at=now,candidate='candidate-doppelganger-air-v512',identity=identity,passed=False,complete=False,native=['native-doppelganger-air-release-v506','native-doppelganger-air-entry-holstered-v507'],failed='doppelganger-air-release-armed-v509',failure='Native source and remote phases work, but landing rejects -0.15257263m over18.9581ms with prior airState3, dropping body weight to0. Candidate512 adds bounded local evidence only; no guard changes.',next='Capture exact root/frame evidence using512, repair demonstrated boundary, rerun armed release and holstered entry. Then continue Shadow Walk. Partner-only edge cases remain deferred.')
wr(R/'work/current-task-state.json',state)
chapter=D/'35-doppelganger-cast-and-clone-lifecycle.md';s=chapter.read_text(encoding='utf-8');marker='## Airborne research506–512 and landing failure'
if marker in s:s=s[:s.index(marker)].rstrip()
s+='''

## Airborne research506–512 and landing failure

Source-only [506](../../work/natural-moveset/native-doppelganger-air-release-v506/result.json) and [507](../../work/natural-moveset/native-doppelganger-air-entry-holstered-v507/result.json) each pass7 checks.506 records armed aim, A+LT180ms, then release while airborne:17 fast airborne samples and2 direct raw release overlaps.507 uses sheathed equipment and A+LT700ms together:18 fast airborne samples and one direct raw entry overlap. Both recover floor Z66.4370575 and restore inventory/health/checkpoints/protected14. Prior ground-only strict reader accepts no air samples. The same exact466/1200/666ms left-arm scenes continue; no first-person animation is replaced.

[Candidate508](../../work/movement-integration/candidate-doppelganger-air-v508/identity.json) extends exact native Jump/Fall ownership, semantic54/336 validation and existing air locomotion under the authored gesture. Bank57/331, clone4/10712 and native effect resources stay unchanged. Ownership, protocol, bank and build tests pass. [Paired509](../../work/natural-moveset/doppelganger-air-release-armed-v509/result.json) **fails** body continuity: despite all native/remote phases and recovery, a -0.15257263m step over18.9581ms is rejected with prior airState3 and motion0. Body weight briefly becomes0. Subsequent clone/FX assertions are not reached; do not claim the whole test passed. Both checkpoints and protected14 restore. The actual remote recording and temporal sheet are retained.

[Diagnostic512](../../work/movement-integration/candidate-doppelganger-air-v512/identity.json) freezes at most32 local movement inputs and16 already received root samples on the first clock failure. It adds no native hooks/reads, changes no packets, and does not relax motion bounds. The purpose is to identify why the existing bounded contact-settlement rule does not cover this take. Candidate495 remains the preserved fully verified ground/cancellation milestone. Files named511 were prepared before this failure and are not completed receipts unless their actual result files exist.
'''
chapter.write_text(s,encoding='utf-8')
print(json.dumps(dict(videos=len(manifest),landingContinuityPassed=False,complete=False)))
