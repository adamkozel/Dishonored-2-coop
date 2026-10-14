from pathlib import Path
import json,datetime,hashlib,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
def wr(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
assert rd(W/'native-operation-owner.json')['state']=='finished'
now=datetime.datetime.now().astimezone().isoformat();identity=rd(W/'candidate-doppelganger-crouch-v490/identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==identity['dll']
assert sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==identity['probe']
manifest=rd(N/'runtime-video-manifest.json');cases=[]
for take in ('native-doppelganger-crouch-v489','doppelganger-crouch-v491'):
    result=rd(N/take/'result.json')
    assert 'All14 protected save/settings hashes retained' in result['checks']
    assert any(x.startswith('Both isolated checkpoints restore') for x in result['checks'])
    cases.append(dict(take=take,passed=result['passed'],failure=result['failure'],checks=len(result['checks']),dll=result['dll']))
    for suffix in (('',) if take.startswith('native-') else ('','-source')):
        video=N/(take+suffix+'.mp4')
        if not video.exists() or any(x['video']==video.name for x in manifest):continue
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
        review=N/take/('visual-review'+suffix+'.json')
        scope='Crouched armed Emily native cast and corresponding complete-body candidate. Exact native489 timing/state evidence;491 records remote body and clone/FX. Standing reuse is separately verified485/486. Holstered, airborne, interruptions and shared gameplay remain open. '+('Scoped take passed.' if result['passed'] else 'Failure retained: '+result['failure'])
        if not review.exists():wr(review,dict(inspected=False,scope=scope,complete=False,visualAccepted=False))
        manifest.append(dict(video=video.name,character='emily',label=f'Doppelganger crouch · {"native source" if take.startswith("native-") or suffix else "remote full body"} ·{take.rsplit("v",1)[-1]}',alias='native_doppelganger_in_crouched',requirements=['emily_doppelganger_cast'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=result['dll'],video_sha256=sha(video),actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),checks_passed=result['passed'],scope=scope))
wr(N/'runtime-video-manifest.json',manifest)
result=rd(N/'doppelganger-crouch-v491/result.json');assert result['dll']==identity['dll']
receipt=dict(at=now,live=identity['dll'],probe=identity['probe'],protocol=52,bankVersion=56,clips=325,cloneWireVersion=4,complete=False,
    sourceObservation=rd(N/'native-doppelganger-crouch-v489/crouch-observations.json'),cases=cases,
    authoring=rd(N/'doppelganger-crouch-bank-manifest-v490.json'),
    scope='Native crouched armed Emily phase reader and full-body context candidate. Rig/feet/grip source checks are numerical; visual inspection remains its own evidence. No source first-person, root, animation event, AI or damage changes.')
wr(W/'doppelganger-crouch-verification-v492.json',receipt)
if result['passed']:
    wr(W/'candidate-doppelganger-crouch-v490/runtime-verification-v492.json',dict(at=now,passed=True,dll=identity['dll'],probe=identity['probe'],complete=False,take='work/natural-moveset/doppelganger-crouch-v491/result.json',scope=receipt['scope']))
notes=('481/485/486 verify continuous219-joint clone playback, native summon/disappearance FX and two casts reusing one cosmetic actor. Viewed486 confirms material recovery;488 shows foreground body disappearance and clearing. '
       '489 independently observes crouch6 plus power63 with466/1200/666ms phases.490 appends complete-body crouch clips to bank56/325, retaining all322 prior payloads. '
       +('491 passes its scoped crouched body/clone/FX checks; see the actual recording for visual judgment. ' if result['passed'] else '491 failed: '+result['failure']+'. ')
       +'Holstered/airborne/interruptions, upgrades/multiple clones, late join and shared outcomes remain open. Full movement and playable co-op remain incomplete.')
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=now
state['DoppelgangerContinuation'].update(at=now,live=identity['dll'],probe=identity['probe'],receipt='work/movement-integration/doppelganger-crouch-verification-v492.json',latest=notes,complete=False,next='Inspect crouched caster footage, then observe holstered contexts and complete remaining power/movement recovery. Partner interaction edge cases remain deferred; normal movement collision retained.')
wr(R/'work/current-task-state.json',state)
annotations=rd(N/'review-evidence-annotations.json');item=annotations['items']['emily_doppelganger_cast']
item.update(notes=notes,runtime_binding='standing_and_crouched_armed_scoped' if result['passed'] else 'standing_armed_cast_verified',native_effects_status='scoped_native_summon_and_disappearance_verified',complete=False)
for phase in ('in','loop','out'):
    for suffix in ('','_crouched'):
        alias='native_doppelganger_'+phase+suffix
        if alias not in item['motions']:item['motions'].append(alias)
# The old all-needs flag includes unverified shared outcomes; do not promote it.
item['native_effects_evidence']=['doppelganger-fx-v485/result.json','doppelganger-fx-v486/result.json','doppelganger-fx-v488/visual-review.json']
wr(N/'review-evidence-annotations.json',annotations)
chapter=D/'35-doppelganger-cast-and-clone-lifecycle.md';text=chapter.read_text(encoding='utf-8');marker='## Crouched native cast and full-body context489–492'
if marker in text:text=text[:text.index(marker)].rstrip()
text+='''

## Crouched native cast and full-body context489–492

[Native489](../../work/natural-moveset/native-doppelganger-crouch-v489/result.json) passes6 checks and restores both checkpoints/protected14. Normal B input establishes stance1/motion1 with Sneaking6 (RVA1c84258). Normal LT1800ms produces UsePower63 (RVA1c80ed8) with that same crouch state. The independent state watcher records65 crouched casting samples; raw Midnight captures12 entry,45 loop and21 release observations, all reciprocal Scene_1 bindings with unchanged466/1200/666ms durations. The then-current standing-only reader accepts none. Mana100→80 includes native replenishment and is not a claim that the power costs20. A real native decoy is separately enumerated.

[Blender authoring490](../../work/natural-moveset/build_doppelganger_crouch_v490.py) uses pinned Blender5.2.0 LTS fbe6228777e7 in a background factory-startup process. Existing whole-body407 acting supplies pose changes; first-person data supplies only timing and context. The Posture-100 crouch rest supplies support and sword guard, with changing torso/arm rotations, native-length leg IK, corrective helpers and retained grip/socket offsets. No fixed first-person wrist targets, animation gameplay events or root displacement are authored. Three121-frame bank entries per rig (322–324;120 motion samples plus closure) are appended; all322 old payloads are byte-identical. Corvo is an offline skeleton comparison because this campaign power belongs to Emily. [Manifest](../../work/natural-moveset/doppelganger-crouch-bank-manifest-v490.json) pins both banks and numerical foot/pelvis checks. Those checks do not establish visual quality.

[Candidate490](../../work/movement-integration/candidate-doppelganger-crouch-v490/identity.json) accepts only matching Running0/stance0 or Sneaking6/stance1 plus owned UsePower63, preserving exact scene/arm/equipment ownership, timing and final identity rereads. Only release may persist on the matching ground state after UsePower retires. Air, holstered and unknown states remain refused. Bank56/325 supports full-body stance blending; older banks retain their standing-only gate. Semantic protocol52/336 and clone protocol4/10712 remain unchanged. Ownership tests include mismatched stance/state/class and crouched release; actual-bank tests cover supported pelvis, native phase durations, finite blends and retained socket offsets. All build/protocol/native ownership/proxy/bank checks pass before isolated deployment.

[491 completed result](../../work/natural-moveset/doppelganger-crouch-v491/result.json), [source/remote phase evidence](../../work/natural-moveset/doppelganger-crouch-v491/phase-incarnations.json) and [492 receipt](../../work/movement-integration/doppelganger-crouch-verification-v492.json) define the current runtime scope. The test requires all three native and remote phases, source stance1 and remote stanceMix≥.95, crouched recovery with sword retained, existing219-joint clone and native FX checks, inventory/health preservation and checkpoint/protected14 restoration. Neither a generated candidate nor one successful take completes the whole power or movement phase.
'''
chapter.write_text(text,encoding='utf-8')
print(json.dumps(dict(videos=len(manifest),cases=cases,live=identity['dll'],complete=False)))
