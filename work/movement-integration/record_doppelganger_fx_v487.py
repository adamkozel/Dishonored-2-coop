"""Index actual takes and preserve the measured native marker-validation repair."""
from pathlib import Path
import json, datetime, hashlib, subprocess

R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
def wr(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
assert rd(W/'native-operation-owner.json')['state']=='finished'
now=datetime.datetime.now().astimezone().isoformat()
identity=rd(W/'candidate-doppelganger-fx-v481/identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==identity['dll']
assert sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==identity['probe']
for f in rd(W/'protected-save-hashes-before-v4.json'):assert sha(Path(f['Path']))==f['Hash']
manifest=rd(N/'runtime-video-manifest.json');takes=[]
for number in (476,478,480,482,483,485,486,488):
    take=f'doppelganger-fx-v{number}';resultPath=N/take/'result.json'
    if not resultPath.exists():continue
    result=rd(resultPath)
    assert 'All14 protected save/settings hashes retained' in result['checks']
    checkpoint=any(x.startswith('Both isolated checkpoints restore') for x in result['checks'])
    assert result.get('retainedWorld') or checkpoint or (not result['events'] and not result['passed'])
    takes.append(dict(take=take,passed=result['passed'],failure=result['failure'],checks=len(result['checks']),dll=result['dll'],retainedWorld=result.get('retainedWorld'),checkpointInTake=checkpoint))
    for suffix in ('','-source'):
        video=N/(take+suffix+'.mp4')
        if not video.exists():continue # Setup failures have no recording; retain their result.
        if any(x['video']==video.name for x in manifest):continue
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
        scope=('Native Emily Doppelganger cosmetic body/root and finite native FX over Steam. '
               'Exact native leaf-marker bounds and same-model reuse investigation. Shared clone gameplay, alternate contexts and full visual acceptance remain open. ')
        scope+=('This scoped take passed.' if result['passed'] else 'Failed take retained: '+result['failure'])
        review=N/take/('visual-review'+suffix+'.json')
        if not review.exists():wr(review,dict(inspected=False,scope=scope,complete=False,visualAccepted=False))
        manifest.append(dict(video=video.name,character='emily',label=f'Doppelganger pose and effects · {"source" if suffix else "remote"} ·{number}',alias='native_doppelganger_cast',requirements=['emily_doppelganger_cast'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=result['dll'],actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),video_sha256=sha(video),checks_passed=result['passed'],scope=scope))
wr(N/'runtime-video-manifest.json',manifest)
a=rd(N/'doppelganger-fx-v485/result.json');b=rd(N/'doppelganger-fx-v486/result.json')
verified=a['passed'] and b['passed'] and a['retainedWorld'] and b['expectedVisualReuse']
receipt=dict(at=now,complete=False,live=identity['dll'],probe=identity['probe'],protocol=52,bankVersion=55,clips=322,cloneWireVersion=4,cloneWireBytes=10712,
    takes=takes,sameActorReuseVerified=verified,partnerEdgeCasesDeferred=True,nativeMovementCollisionRetained=True,
    actualRefusedMatrices='work/natural-moveset/doppelganger-fx-v480/refused-pose-analysis.json',
    correction='Native rigid leaf markers1/10/17 exceed3m local offset. Only these known219-rig leaves permit10m; other joints remain3m. Finite/rotation/rig/ownership guards unchanged. Sender, receiver and interpolated pose use one validator. Successful native rediscovery also returns to33ms sampling after475; that cadence repair alone did not eliminate rejected poses.')
if verified:
    assert a['dll']==b['dll']==identity['dll']
    assert any(x.startswith('Both isolated checkpoints restore') for x in b['checks'])
    wa=rd(N/'doppelganger-fx-v485/world-retained.json');wb=rd(N/'doppelganger-fx-v486/world-start.json');assert wa==wb
    va=rd(N/'doppelganger-fx-v485/visual-final.json');vb=rd(N/'doppelganger-fx-v486/visual-final.json')
    assert va['handle']==vb['handle'] and va['created']==vb['created']
    receipt['actorHandle']=va['handle'];receipt['worldEpochs']=wa
    receipt['transportHealth']=[rd(N/f'doppelganger-fx-v{v}/transport-health.json') for v in (485,486)]
    scoped=dict(at=now,passed=True,dll=identity['dll'],probe=identity['probe'],complete=False,
        scope='Two standing armed native Emily casts in the same isolated Dunwall world: exact219 body pose, continuous visible lifetime, same cosmetic actor reused, finite native FX clocks/helpers, no native pose or send failures, followed by checkpoint/protected14 restoration. This does not establish shared gameplay, all contexts, complete movement or release acceptance.',
        receipt='work/movement-integration/doppelganger-fx-investigation-v487.json')
    wr(W/'candidate-doppelganger-fx-v481/runtime-verification-v487.json',scoped)
wr(W/'doppelganger-fx-investigation-v487.json',receipt)
state=rd(R/'work/current-task-state.json');state['DoppelgangerContinuation'].update(at=now,live=identity['dll'],probe=identity['probe'],receipt='work/movement-integration/doppelganger-fx-investigation-v487.json',
    latest=('481 repairs native clone leaf-marker rejection;485/486 verify two consecutive casts on the same cosmetic actor with continuous visibility, body/root and native FX clocks/helpers; both checkpoints and protected14 restore.' if verified else '481 narrows native leaf-marker validation using exact480 failing matrices. Two-cast results are recorded in487; unresolved failures must be read before acceptance.'),
    complete=False,next='Same-actor material recovery486 and foreground body/disappearance488 have viewed sheets. Continue remaining cast contexts and interruption recovery, then movement/shared-world work. Keep partner edge cases deferred and normal movement collision enabled.')
state['UpdatedAt']=now;wr(R/'work/current-task-state.json',state)
chapter=D/'35-doppelganger-cast-and-clone-lifecycle.md';text=chapter.read_text(encoding='utf-8');marker='## Native leaf-marker bounds and same-model reuse475–487'
if marker in text:text=text[:text.index(marker)].rstrip()
text+='''

## Native leaf-marker bounds and same-model reuse475–487

The exact473 hide record identified a300ms clone lease expiry while ordinary peer traffic was15ms old.475 corrected a separate sampling defect: successful discovery still retained the500ms idle deadline instead of immediately returning to33ms. **That fix alone was insufficient**.476 still failed;478 passed but retained12 new validation failures. No477 reuse run occurred. The scoped failures and successful control are retained, rather than rewritten as passes.

475 diagnostics separated prepared native snapshots, packet validation refusal and native Steam send failure. All failing trials had zero send failures and stable, owned219-joint native reads. The old per-joint validator imposed a3m local translation bound on every joint.478 independently read a valid anchor after a refusal; it identified the name/hierarchy but was not the failing matrix.479 therefore captured the exact rejected12-float matrix and sequence inside the sender. [480 analysis](../../work/natural-moveset/doppelganger-fx-v480/refused-pose-analysis.json), produced by [this script](../../work/movement-integration/analyze_refused_doppelganger_v480.py), contains five unique observed rejected sequences: anchor offsets3.429–3.685m and hand attachment offsets4.179/3.245m. All are finite proper rigid rotations, determinants approximately1. Native indices1/10/17 are anchor_jnt, left_hand_attachment_jnt and right_hand_attachment_jnt, each a leaf in the exact fingerprinted Emily rig. No claim about their mesh skin weights is needed or established.

[481 preserved source and pins](../../work/movement-integration/candidate-doppelganger-fx-v481/identity.json) introduce one joint validator shared by source validation, receiver validation and interpolated presentation. Only the three evidenced leaves permit10m per-axis offsets; ordinary joints remain3m.10m is a bounded engineering allowance above observed offsets, not a claim about a native theoretical maximum. Finite values, orthonormality, positive determinant, exact219-rig fingerprint, ownership and root-distance checks remain. All219 transforms are preserved, including markers. Clone protocol changes from3 to4 (still10712 bytes) so peers cannot silently disagree about the contract; semantic player protocol52 and bank55/322 remain unchanged. Regression tests exercise actual native left/right matrices, every joint index, interpolation, old-wire refusal, nonfinite/reflected/scaled transforms and excessive marker translations. Build, native ownership/action tests, proxy smoke and both actual banks pass.

[482](../../work/natural-moveset/doppelganger-fx-v482/result.json) passes17 checks with549 prepared/sent packets and no rejected poses, native read failures or send failures. Exactly one show/hide occurred; viewed summon frames show the native white burst, body emergence and clearing.482 deliberately retained the world for483. [483](../../work/natural-moveset/doppelganger-fx-v483/result.json) failed its clean-health setup check before acquiring the power or recording video. The wrapper restored both checkpoints (its completed output reports the native sword fixture ready); the failed take itself has no checkpoint receipt. The cause of the changed health was not captured. Inspection found that the first take released NPC isolation before handing off, allowing ordinary enemy AI to run during second setup.485 retains the same finite pause across the immediate handoff, requires at least5s remaining and records exact health/isolation values. This is a harness correction; DLL481 is unchanged.

[485](../../work/natural-moveset/doppelganger-fx-v485/result.json) and [486](../../work/natural-moveset/doppelganger-fx-v486/result.json) define the repeated-cast result. The second must retain the exact cosmetic actor handle, create no second model and finally restore both checkpoints. Each take adds zero-delta assertions for source packet rejection, native pose-read failure and Steam send failure. Exact packet digests,219 final joints, source retirement, scene clocks/reciprocal bindings/five native FX helpers, health/inventory and protected14 remain checked. [487 consolidated receipt](../../work/movement-integration/doppelganger-fx-investigation-v487.json) records success or failure, hashes and current scope. A passing receipt does not substitute for visual inspection.

The handoff refresher previously used historic37/42/260 defaults when an archive omitted optional metadata. It now reads the semantic version from that exact archive's source and the bank version/count from both archived binary headers, asserting agreement. This fixes documentation pins; it does not change gameplay.

[486 viewed second-summon sheet](../../work/natural-moveset/doppelganger-fx-v486/remote-summon-sheet.jpg) confirms body/material recovery on the same actor after its first disappearance. [488](../../work/natural-moveset/doppelganger-fx-v488/result.json) passes18 checks from the fixed lower-floor observer:551 prepared/sent poses, zero validation/read/send failures, summon29 observations84–976ms, disappearance14 observations59–459ms. [Viewed foreground disappearance](../../work/natural-moveset/doppelganger-fx-v488/remote-disappearance-sheet.jpg) shows the running body, native white rings/column, body disappearance and complete clearing by22.4s. The body is dark but not obscured by the pillar. These are scoped real-game appearance checks, not universal aesthetic acceptance.

Remaining: alternate stance/equipment contexts, interruptions/load/session cleanup, upgraded/multiple clones and shared gameplay outcomes. Basic partner exclusions remain retained; exhaustive partner interaction edge cases stay deferred, and movement collision stays allowed. No new skill or external tool installation was needed.
'''
chapter.write_text(text,encoding='utf-8')
print(json.dumps(dict(videos=len(manifest),takes=takes,verifiedReuse=verified,live=identity['dll'],complete=False)))
