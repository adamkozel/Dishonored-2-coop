from pathlib import Path
import json, hashlib, shutil, subprocess, datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11';A=W/'candidate-doppelganger-visual-v427'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat();identity=rd(A/'identity.json');proofs=[]
assert identity['dll']==sha(R/'work/asset-game/dinput8.dll') and identity['probe']==sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')
for take in ('doppelganger-visual-v428','doppelganger-visual-v430'):
    p=N/take;result=rd(p/'result.json');pose=rd(p/'visual-final-skeleton.json');visual=rd(p/'visual-final.json');transport=rd(p/'clone-transport-verification.json')
    assert result['passed'] and len(result['checks'])==13 and result['dll']==identity['dll'] and result['probe']==identity['probe']
    assert pose['stable'] and pose['finalPublicationComplete'] and len(pose['joints'])==219 and pose['cloneExpectedPoseError']==0
    assert pose['cloneRig']=='5921d3450a60d7e2' and pose['cloneRigRaw']=='d5699c809bde0be2'
    assert not visual['visible'] and visual['poseRefused']==0 and visual['maxTransformError']<.001 and transport['allAcceptedDigestsMatch'] and transport['receiverRejected']==[0]
    before=rd(p/'visual-start.json') if (p/'visual-start.json').exists() else {k:0 for k in ('created','shown','hidden','updated','interpolated','applied')}
    delta={k:visual[k]-before[k] for k in ('created','shown','hidden','updated','interpolated','applied')}
    rows=rd(p/'states.json');source={v['sequence']:v for row in rows for v in row['cloneSource']['history']};accepted={v['sequence']:v for row in rows for v in row['clonePeer']['history']}
    first=min(v['event'] for v in accepted.values() if v['present']);current=[v for v in accepted.values() if v['event']>=first]
    assert all(source[v['sequence']]['digest']==v['digest'] for v in current)
    proofs.append(dict(take=take,result=result,transport=transport,currentIncarnationAccepted=len(current),currentLive=sum(v['present'] for v in current),visual=visual,delta=delta,
        finalPose=dict(joints=219,error=0,canonicalRig=pose['cloneRig'],rawRig=pose['cloneRigRaw']),
        maximumSourcePoseReadMs=max(row['cloneSource']['poseReadMaxMs'] for row in rows),sourcePoseRefusals=max(row['cloneSource']['poseReadFailed'] for row in rows),
        visualScope='430 clear camera shows separate clone walking/turning.428 observer angle does not clearly show separate clone. Native source effects are not replicated.'))
scope='427 passes428 and430: native Steam source-owned219-joint clone pose/root/lifetime, one cosmetic clone per cast, zero final-pose error and no pose refusals.430 viewed remote/source recordings show separate clone walking/turning. Native FX, alternate cast contexts/upgrades, multiple clones, late join and shared gameplay remain incomplete.'
receipt=dict(at=now,scope=scope,complete=False,proofs=proofs)
wr(A/'runtime-verification-v431.json',receipt);identity.update(runtimeVerified=True,runtimeVerificationScope=scope);wr(A/'identity.json',identity)
for p in [Path(__file__),W/'prepare_doppelganger_camera_v430.py',W/'Run-DoppelgangerVisual-v428.ps1',W/'Run-DoppelgangerVisual-v430.ps1',N/'Test-DoppelgangerVisual-v428.ps1',N/'Test-DoppelgangerVisual-v430.ps1',R/'outputs/d2-coop-lab/research/doppelganger-visual-v423-asset-install.json']:shutil.copy2(p,A/p.name)
manifest=rd(N/'runtime-video-manifest.json')
for version in (422,424,426,428,430):
    take=f'doppelganger-rig-v{version}' if version==422 else f'doppelganger-visual-v{version}'
    result=rd(N/take/'result.json')
    for suffix in (('',) if version==422 else ('','-source')):
        filename=take+suffix+'.mp4';assert not any(v['video']==filename for v in manifest)
        video=N/filename;meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
        source=version==422 or bool(suffix);sheet=('source' if source else 'remote')+'-sheet.jpg';inspected=(N/take/sheet).exists()
        note=('Native source preview, summon FX and clone locomotion inspected; local first person remains native.' if source and inspected else
              '430 clear rear/side observer sees the separate clone standing, walking and turning with source poses; native VFX still absent.' if version==430 else
              '428 body cast/recovery visible, separate clone not clearly established by this camera;430 resolves camera coverage.' if version==428 else
              'Failed candidate: source rig fingerprint rejected before receiver creation; recording retained, no visual acceptance.')
        review=N/take/('visual-review'+suffix+'.json');wr(review,dict(inspected=inspected,contactSheet=sheet if inspected else None,scope=note,complete=False,visualAccepted=False))
        manifest.append(dict(video=filename,character='emily',label=f'Doppelganger clone pose · {"source" if source else "remote"} ·{version}',alias='native_doppelganger_cast',requirements=['emily_doppelganger_cast'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=result['dll'],actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),video_sha256=sha(video),checks_passed=result['passed'],scope=note+' '+('All13 runtime checks and protected checkpoint restoration pass.' if version in (428,430) else '')))
wr(N/'runtime-video-manifest.json',manifest)
annotations=rd(N/'review-evidence-annotations.json');annotations['items']['emily_doppelganger_cast'].update(complete=False,visual_validation='not_accepted',notes=scope+' Checkpoints, original inventory/health and all14 protected saves/settings restored in both passing tests. Full movement/co-op remains incomplete.');wr(N/'review-evidence-annotations.json',annotations)
state=rd(R/'work/current-task-state.json');state['DoppelgangerContinuation'].update(at=now,live=identity['dll'],probe=identity['probe'],receipt='work/movement-integration/candidate-doppelganger-visual-v427/runtime-verification-v431.json',latest=scope,complete=False,next='Capture exact native Doppelganger summon FX records and reciprocal clone/bone ownership; implement cosmetic source-owned FX with duplicate/stale-event protection, then alternate cast contexts and remaining co-op requirements. Do not expand partner interaction edge cases; retain ordinary movement collision.');state['UpdatedAt']=now;wr(R/'work/current-task-state.json',state)
with (D/'35-doppelganger-cast-and-clone-lifecycle.md').open('a',encoding='utf-8') as f:f.write('''

## Verified clone pose427, runtime428 and clear-camera430

[Verification431](../../work/movement-integration/candidate-doppelganger-visual-v427/runtime-verification-v431.json) records two passing13-check runs. Both independently bracket a completed native phase3 publication and read all219 local transforms: maximum error against the receiver's exact interpolated pose is zero. Both raw fingerprints are `d5699c809bde0be2` and canonical fingerprints `5921d3450a60d7e2`, confirming that signed-zero normalization resolves the mismatch. Every nonzero bind value and hierarchy check remains exact. No source or receiver pose refusals occur.

428 creates/shows/hides one dedicated clone, applies371 native poses and851 interpolated root updates; maximum transform component error is4.2e-7.430 repeats after a full checkpoint and creates/shows/hides exactly one additional clone, applies513 additional native poses and376 interpolated root updates; maximum transform component error is3.6e-7. Pose application can stop when native rendering culls an offscreen model; source root/lifetime sampling continues. These counters are not a claim of flawless frame pacing on all hardware. Source full-pose read and receiver service maxima are captured in the receipt.

The original428 observer angle did not clearly establish separate clone visibility despite the numerical checks. [430 remote recording](../../work/natural-moveset/doppelganger-visual-v430.mp4), its [viewed temporal sheet](../../work/natural-moveset/doppelganger-visual-v430/remote-sheet.jpg), and the [native source recording](../../work/natural-moveset/doppelganger-visual-v430-source.mp4) now show the separate Emily clone standing, walking and turning. The source-only summon burst remains visibly absent on the peer. This proves the cosmetic clone pipeline in the isolated standing armed Emily test; it is not acceptance of the complete Doppelganger power or all animations. The camera change was test-only; no proxy or asset change was required between428 and430.

Retained Steam histories match exact packet digests with zero receiver rejection.430 histories initially include previous-cast clear records; verification431 also reports counts restricted to the new incarnation, avoiding mislabelling all retained records as one cast. Both runs preserve the three casting body phases, standing/sword recovery, original health/inventory, full checkpoint recovery and all14 protected files. The four-component receiver has no health/physics/AI/Decoy component and executes no gameplay action events. Original Steam assets and first-person behavior are unchanged.

Current verified DLL427: `2711A519D286FF23512B6D865577F7D803C7EC37430884DAB1C611121EBD5B17`; probe427: `77EC1B38C8820D5A1C2A0FED33DFA27BED6CBAA08A85109F3C676F49DBF19ECA`. Source, binaries, banks, assets, tests, harnesses and receipt are archived together. The phase3 finding is saved in the persistent Ghidra project at140540a90. Next is exact source-owned native summon/retirement VFX, followed by unsupported contexts/upgrades/multiple clones and shared world outcomes. Ordinary partner movement collision is retained and broad interaction audits remain deferred.
''')
with (D/'02-roadmap.md').open('a',encoding='utf-8') as f:f.write('\n\nDoppelganger update431: separate cosmetic Emily clone now replicates all219 native joints, interpolated root and authoritative lifetime over Steam in two passing tests.430 supplies clear actual-game visual evidence. Native summon/retirement effects, upgrades/alternate contexts/multiple clones and shared outcomes remain. See [chapter35](35-doppelganger-cast-and-clone-lifecycle.md).\n')
print(json.dumps(dict(videos=len(manifest),checks=[len(p['result']['checks']) for p in proofs],currentIncarnationAccepted=[p['currentIncarnationAccepted'] for p in proofs],complete=False)))
