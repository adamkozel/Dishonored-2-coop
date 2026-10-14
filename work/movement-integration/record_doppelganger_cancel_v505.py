from pathlib import Path
import json, hashlib, datetime, subprocess
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'; D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
def wr(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
assert rd(W/'native-operation-owner.json')['state']=='finished'
now=datetime.datetime.now().astimezone().isoformat(); identity=rd(W/'candidate-doppelganger-holstered-v495/identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==identity['dll']
takes=['native-doppelganger-cancel-standing-v499','native-doppelganger-cancel-standing-v500','native-doppelganger-cancel-standing-v501',
       'doppelganger-cancel-standing-v502','doppelganger-cancel-standing-v503','doppelganger-cancel-crouched-holstered-v504']
manifest=rd(N/'runtime-video-manifest.json'); cases=[]
for take in takes:
    result=rd(N/take/'result.json')
    assert any(x.startswith('Both isolated checkpoints restore') for x in result['checks'])
    assert any(x.startswith('All14 protected') for x in result['checks'])
    cases.append(dict(take=take,passed=result['passed'],failure=result['failure'],checks=len(result['checks'])))
    for suffix in (('',) if take.startswith('native-') else ('','-source')):
        video=N/(take+suffix+'.mp4')
        if not video.exists() or any(x['video']==video.name for x in manifest):continue
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
        review=N/take/('visual-review'+suffix+'.json')
        scope='Doppelganger aim cancellation research/checks.499 RT fired a sword slash and briefly spawned/killed a clone;500 LeftThumb also committed the cast.501 confirms X while LT is held: no activation, no clone and no mana use.502 failed a PowerShell array comparison;503 fixes the assertion and504 covers crouched holstered recovery. Raw failures retained. '+('Scoped take passed.' if result['passed'] else 'This take failed: '+str(result['failure']))
        if not review.exists():wr(review,dict(inspected=False,complete=False,visualAccepted=False,scope=scope))
        manifest.append(dict(video=video.name,character='emily',label=f'Doppelganger cancellation · {take} · {"source" if take.startswith("native-") or suffix else "remote"}',
            alias='power_doppelganger_cancel',requirements=['emily_doppelganger_cast'],gameplay_bound=False,complete=False,visual_accepted=False,
            evidence=take+'/result.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=result['dll'],video_sha256=sha(video),
            actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),checks_passed=result['passed'],scope=scope))
wr(N/'runtime-video-manifest.json',manifest)
passed=all(rd(N/t/'result.json')['passed'] for t in takes[-2:]) and rd(N/takes[2]/'result.json')['passed']
receipt=dict(at=now,live=identity['dll'],probe=identity['probe'],protocol=53,bankVersion=57,clips=331,cloneWireVersion=4,
             scopedPassed=passed,complete=False,cases=cases,
             sourceProof=rd(N/takes[2]/'no-spawn-proof.json'),
             runtimeProofs=[dict(take=t,proof=rd(N/t/'no-spawn-proof.json'),recovery=rd(N/t/'cancel-recovery.json')) for t in takes[-2:]],
             scope='Native X aim cancellation and remote recovery in the recorded standing-armed and crouched-holstered cases. No new game code or assets. General interruption/campaign support, full power shared outcomes and visual acceptance remain separate.')
wr(W/'doppelganger-cancel-verification-v505.json',receipt)
if passed:wr(W/'candidate-doppelganger-holstered-v495/runtime-cancel-verification-v505.json',dict(at=now,passed=True,dll=identity['dll'],probe=identity['probe'],complete=False,takes=[f'work/natural-moveset/{t}/result.json' for t in takes[-2:]],scope=receipt['scope']))
state=rd(R/'work/current-task-state.json'); state['UpdatedAt']=now
latest=state['DoppelgangerContinuation']['latest']
marker=' Native cancellation505:'
if marker in latest:latest=latest[:latest.index(marker)]
latest+=marker+(' confirmed X-with-LT cancels with no source activation/mana/clone scene and no remote clone/FX;503 standing armed and504 crouched holstered recover full body. ' if passed else ' failures remain; inspect505 receipt. ')
latest+='Wrong-input499/500 and array-assertion502 failures are preserved. Native air, other interruptions, upgrades/multiple clones, late join and shared gameplay remain unfinished.'
state['DoppelgangerContinuation'].update(at=now,receipt='work/movement-integration/doppelganger-cancel-verification-v505.json',latest=latest,
    next='Continue remaining native movement/power recovery and shared-world co-op. Partner-only interaction edge cases stay deferred; ordinary movement collision retained.',complete=False)
wr(R/'work/current-task-state.json',state)
annotations=rd(N/'review-evidence-annotations.json'); annotations['items']['emily_doppelganger_cast'].update(notes=latest,complete=False)
wr(N/'review-evidence-annotations.json',annotations)
chapter=D/'35-doppelganger-cast-and-clone-lifecycle.md'; text=chapter.read_text(encoding='utf-8'); marker='## Native aim cancellation499–505'
if marker in text:text=text[:text.index(marker)].rstrip()
text+='''

## Native aim cancellation499–505

Two incorrect inputs were investigated before reusing the established native X control from Mesmerize122 and Domino233. [499](../../work/natural-moveset/native-doppelganger-cancel-standing-v499/result.json) presses RT during held LT: native Slash39 (RVA1c85d28) overlaps UsePower63, a clone spawn scene214 runs, then death_fx219. Entity snapshots beginning after5s miss this short lifetime. The test eventually fails on a separate malformed `Where-Object -eq63` argument; the raw result is preserved. Its absence-only clone observation is **not** cancellation proof. [500](../../work/natural-moveset/native-doppelganger-cancel-standing-v500/result.json) tries LeftThumb, and the strengthened test correctly rejects the resulting activation/spawn. Both restore checkpoints/protected14. No production code was changed by either test.

Fresh exact-executable Ghidra decompilation of [14061bea0](../../work/movement-integration/native-power-base-14061bea0-cancel-research-v500.txt) confirms an integer increment at power component+0x64 after its optional140746cf0 call, then byte+a5 is set.499 raw snapshots show counter0→1 alongside the spawn/death scenes. [14061bcb0](../../work/movement-integration/native-power-base-14061bcb0-cancel-research-v500.txt) only sets byte+a6. These observations do not by themselves establish a mana cost or complete component layout. A [confirmed bookmark](../../work/movement-integration/doppelganger-cancel-bookmark-v500.json) was saved at14061bea0 in `Dishonored2.exe`; the project save succeeded and the handoff bookmark export now includes it. No native counter writes or calls were introduced.

[501](../../work/natural-moveset/native-doppelganger-cancel-standing-v501/result.json) uses **X while LT is held**,250ms, then both release. It confirms native Cancel74 (RVA1c81de8), brief Undraw81 (RVA1c7df88), and return to Running0 with the sword still equipped. Only entry466ms and loop1200ms occur; no successful-cast out666ms occurs. [No-spawn proof](../../work/natural-moveset/native-doppelganger-cancel-standing-v501/no-spawn-proof.json) records zero native spawn/disappearance/death scenes, unchanged activation counter0 and mana100→100. This distinguishes cancellation from a briefly surviving cast.

[502](../../work/natural-moveset/doppelganger-cancel-standing-v502/result.json) is a retained **harness failure**: PowerShell compared two counter arrays directly with `-ne`, falsely flagging unchanged FX counters[2,2]. No-spawn proof in that take shows no activation or mana use. [503 correction](../../work/movement-integration/fix_cancel_fx_check_v503.py) compares each of the two counters explicitly. [Standing503](../../work/natural-moveset/doppelganger-cancel-standing-v503/result.json), [crouched-holstered504](../../work/natural-moveset/doppelganger-cancel-crouched-holstered-v504/result.json), and [505 receipt](../../work/movement-integration/doppelganger-cancel-verification-v505.json) are the final scoped evidence. They require native/remote entry and aim without a successful release, no source activation/mana/clone scenes, no remote cosmetic clone/FX, uninterrupted body weight and matching stance/equipment through recovery, plus health/inventory/checkpoint/protected14 preservation. Final visual acceptance and broader interruptions remain separate.
'''
chapter.write_text(text,encoding='utf-8')
print(json.dumps(dict(videos=len(manifest),scopedPassed=passed,cases=cases,complete=False)))
