from pathlib import Path
import json, datetime, hashlib, subprocess
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'; D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
def wr(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
assert rd(W/'native-operation-owner.json')['state']=='finished'
now=datetime.datetime.now().astimezone().isoformat()
identity=rd(W/'candidate-doppelganger-holstered-v495/identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==identity['dll']
assert sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==identity['probe']
takes=('native-doppelganger-holstered-standing-v493','native-doppelganger-holstered-crouched-v494',
       'doppelganger-holstered-standing-v496','doppelganger-holstered-crouched-v497')
manifest=rd(N/'runtime-video-manifest.json'); cases=[]
for take in takes:
    result=rd(N/take/'result.json')
    assert any(x.startswith('All14 protected') for x in result['checks'])
    assert any(x.startswith('Both isolated checkpoints restore') for x in result['checks'])
    cases.append(dict(take=take,passed=result['passed'],failure=result['failure'],checks=len(result['checks']),dll=result['dll']))
    for suffix in (('',) if take.startswith('native-') else ('','-source')):
        video=N/(take+suffix+'.mp4')
        if not video.exists() or any(x['video']==video.name for x in manifest):continue
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
        review=N/take/('visual-review'+suffix+'.json')
        scope='Standing/crouched hands-free Emily native cast and complete-body remote context. 493/494 independently observe native sheathed-sword state and466/1200/666ms phases. 496/497 include source and remote body, clone/FX, standing recovery and checkpoint restoration. Air, cancellation, upgrades/multiple clones and shared gameplay remain open. '+('Scoped checks pass.' if result['passed'] else 'Failed evidence retained: '+str(result['failure']))
        if not review.exists():wr(review,dict(inspected=False,scope=scope,complete=False,visualAccepted=False))
        manifest.append(dict(video=video.name,character='emily',label=f'Doppelganger holstered {result["stance"]} · {"native source" if take.startswith("native-") or suffix else "remote full body"} ·{take.rsplit("v",1)[-1]}',
            alias=f'native_doppelganger_in_{result["stance"]}_empty',requirements=['emily_doppelganger_cast'],gameplay_bound=False,complete=False,visual_accepted=False,
            evidence=take+'/result.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=result['dll'],video_sha256=sha(video),
            actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),checks_passed=result['passed'],scope=scope))
wr(N/'runtime-video-manifest.json',manifest)
passed=all(c['passed'] for c in cases)
receipt=dict(at=now,live=identity['dll'],probe=identity['probe'],protocol=53,bankVersion=57,clips=331,cloneWireVersion=4,complete=False,scopedPassed=passed,cases=cases,
    sources=[dict(take=t,observations=rd(N/t/'holstered-observations.json')) for t in takes[:2]],
    authoring=rd(N/'doppelganger-holstered-bank-manifest-v495.json'),
    scope='Grounded native Emily hands-free Doppelganger body contexts and standing recovery. Existing separate cosmetic clone/FX transport checked. No shared AI/damage outcomes, general campaign coverage or final visual acceptance claimed.')
wr(W/'doppelganger-holstered-verification-v498.json',receipt)
if passed:wr(W/'candidate-doppelganger-holstered-v495/runtime-verification-v498.json',dict(at=now,passed=True,dll=identity['dll'],probe=identity['probe'],complete=False,takes=[f'work/natural-moveset/{t}/result.json' for t in takes[2:]],scope=receipt['scope']))
notes=('Native493/494 show standing/crouched Emily keeps the sword sheathed throughout the466/1200/666ms Doppelganger phases. '
       '495 appends six complete-body hands-free phase clips to bank57/331, retaining all325 previous payloads; semantic53 accepts the observed native equipment context. '
       +('496/497 pass remote body/clone/FX and standing-recovery checks. ' if passed else 'Runtime failures remain in the498 receipt. ')
       +'Standing/crouched armed evidence491 and continuous clone/material-reuse evidence485/486/488 remain preserved. Airborne/cancellation/upgrades/multiple clones, shared outcomes and visual acceptance remain open; whole movement/playable co-op incomplete.')
state=rd(R/'work/current-task-state.json'); state['UpdatedAt']=now
state['DoppelgangerContinuation'].update(at=now,live=identity['dll'],probe=identity['probe'],receipt='work/movement-integration/doppelganger-holstered-verification-v498.json',latest=notes,complete=False,
    next='Observe native cancellation and remaining movement recovery. Partner interaction-only edge cases stay deferred; normal movement collision retained.')
wr(R/'work/current-task-state.json',state)
annotations=rd(N/'review-evidence-annotations.json'); item=annotations['items']['emily_doppelganger_cast']
item.update(notes=notes,runtime_binding='grounded_armed_and_holstered_scoped' if passed else 'standing_and_crouched_armed_scoped',complete=False)
for phase in ('in','loop','out'):
    for stance in ('standing','crouched'):
        alias=f'native_doppelganger_{phase}_{stance}_empty'
        if alias not in item['motions']:item['motions'].append(alias)
wr(N/'review-evidence-annotations.json',annotations)
chapter=D/'35-doppelganger-cast-and-clone-lifecycle.md'
text=chapter.read_text(encoding='utf-8'); marker='## Holstered ground contexts493–498'
if marker in text:text=text[:text.index(marker)].rstrip()
text+='''

## Holstered ground contexts493–498

Native [standing493](../../work/natural-moveset/native-doppelganger-holstered-standing-v493/result.json) and [crouched494](../../work/natural-moveset/native-doppelganger-holstered-crouched-v494/result.json) each pass six source-observation checks and restore both checkpoints and all14 protected hashes. A native X hold, while looking away to avoid contextual interaction, sheathes the sword. Normal LT1800ms then casts. Both takes independently record58 UsePower63 samples; standing uses Running0, crouched uses Sneaking6. Eleven slower equipment snapshots in each cast show the sword still sheathed; none shows a redraw. Raw reciprocal Scene_1 entry/loop/out clocks remain466/1200/666ms. The old armed-only reader accepts zero phases, as expected. Mana100→80 includes replenishment and is not a measured cost.

[Authoring495](../../work/natural-moveset/build_doppelganger_holstered_v495.py) appends six complete-body candidates (325–330) per rig using pinned Blender5.2.0 LTS fbe6228777e7, factory-startup background mode.407 full-body acting is adapted to standing-empty59/crouched-empty76 references. Torso, casting arm and supported legs remain animated; the empty right hand uses the native rest relationship. Native bone translations, corrective handling and foot IK are retained. First-person timing and state are references, never fixed world-space wrist targets. Corvo output is offline skeleton comparison only. All325 previous clip payloads remain byte-identical. [Bank manifest](../../work/natural-moveset/doppelganger-holstered-bank-manifest-v495.json) records hashes, feet and pelvis measurements; numerical checks do not establish visual quality.

[Candidate495](../../work/movement-integration/candidate-doppelganger-holstered-v495/identity.json) removes only the armed-only gate from the strict scene observer. Exact native sword/selected power identity, ground state/class, owned Emily left arm, durations and final identity rereads remain required. Semantic protocol53/336 explicitly accepts observed sword states1/2 for these phases and rejects the old52 contract; clone4/10712 is unchanged. Bank57/331 blends the four complete-body stance/equipment contexts. Older banks retain their armed presentation gate. Ownership, semantic rejection, full-body blend/timing/pelvis, previous armed behavior and actual-bank/proxy build tests pass.

[Standing496](../../work/natural-moveset/doppelganger-holstered-standing-v496/result.json), [crouched497](../../work/natural-moveset/doppelganger-holstered-crouched-v497/result.json) and the [498 receipt](../../work/movement-integration/doppelganger-holstered-verification-v498.json) define the runtime scope. Each paired recording checks all three native/remote phases, sheathed source and avatar swords, stance/armed blends, body clock continuity, separate219-joint clone transport, native FX/material-only summon/disappearance, inventory/health preservation and checkpoint/protected14 restoration. The crouched take explicitly stands after the completed cast. Recordings and saved visual-review notes remain necessary for judging appearance; these checks do not prove general shared clone gameplay or complete movement.
'''
chapter.write_text(text,encoding='utf-8')
print(json.dumps(dict(videos=len(manifest),cases=cases,live=identity['dll'],complete=False)))
