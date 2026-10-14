from pathlib import Path
import json,datetime,hashlib,subprocess,collections,struct
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat();assert rd(W/'native-operation-owner.json')['state']=='finished'
cases=[];manifest=rd(N/'runtime-video-manifest.json')
for take in ['native-shadowwalk-standing-v516b','native-shadowwalk-state-v520']:
    result=rd(N/take/'result.json');assert result['passed']
    rows=rd(N/take/'states.json')
    effects=[f for r in rows for f in r.get('effects',{}).get('effects',[])]
    effectNames=collections.Counter(f['name'] for f in effects)
    # Keep the original520 file: its 'records' lookup accidentally summarized
    # no effects, though the full independent FX snapshots are intact.
    observations=rd(N/take/'shadowwalk-observations.json')
    strict=rd(N/take/'strict-state-observations.json') if (N/take/'strict-state-observations.json').exists() else None
    if strict:
        corrected=dict(strict,effectNames=sorted(effectNames),effectOwners=sorted(set(f['owner'] for f in effects)),summaryCorrection='Original520 summary read effects.records; actual probe data is effects.effects. Full snapshots are unchanged. Corrected offline521, without rerunning native inputs.')
        wr(N/take/'strict-state-observations-corrected-v521.json',corrected)
    positions=[r['player']['position'] for r in rows if r['player']['stance']==2]
    span=[max(p[i] for p in positions)-min(p[i] for p in positions) for i in range(3)]
    cases.append(dict(take=take,passed=True,checks=len(result['checks']),observations=observations,strict=corrected if strict else None,formPositionSpan=span,fxSnapshots=dict(effectNames),fxOwnerDefinitions=sorted(set(f['ownerDefinition'] for f in effects))))
    review=N/take/'visual-review.json'
    wr(review,dict(inspected=True,at=now,sheets=['exit-sheet.jpg']+(['source-sheet.jpg'] if take.endswith('516b') else []),method='Viewed temporal sheets extracted from the actual game recording.',observations=['Native first-person view drops to the floor with a dark, blurred Shadow Walk screen effect.','During exit the camera rises; the native arms and crouched sword grip recover, and the scene returns to its normal color.'],limitations=['Source reference only; does not show remote third-person form.','Temporal frames do not establish continuous animation quality.'],complete=False,visualAccepted=False))
    video=N/(take+'.mp4')
    assert not any(x['video']==video.name for x in manifest)
    meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
    manifest.append(dict(video=video.name,character='emily',label='Native Shadow Walk source · '+take,alias='shadow_walk_native_reference',requirements=['emily_shadow_walk_enter_move_attack_exit'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=result['dll'],video_sha256=sha(video),actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),checks_passed=True,scope='Native source reference: acquisition, activation, bounded movement, natural exit, inventory/checkpoint/protected-save restoration. Remote Shadow Walk body, morphology, VFX and shared attacks not implemented.'))
wr(N/'runtime-video-manifest.json',manifest)
receipt=dict(at=now,sourcePassed=True,complete=False,cases=cases,dll=sha(R/'work/asset-game/dinput8.dll'),probe=sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe'),nativeFirstPersonChanged=False,partnerEdgeCasesDeferred=True,next='Bind exact native mode and owned entry/exit scenes to remote full-body presentation; calibrate low-body motion against replicated displacement, then native cosmetic ash/limb effects. Keep owner camera/postprocessing local. Attacks and shared gameplay remain separate authority work.')
wr(W/'shadowwalk-source-verification-v521.json',receipt)
wr(W/'candidate-shadowwalk-inventory-v515/source-runtime-verification-v521.json',receipt)
wr(W/'candidate-shadowwalk-probe-v519/source-runtime-verification-v521.json',receipt)
bookmarks=rd(R/'work/handoff-project-bookmarks-current.json');actual=rd(W/'native-shadowwalk-bookmark-v518.json')
assert actual['success']
for b in actual['bookmarks']:
    bookmarks=[x for x in bookmarks if (x['address'],x['category'])!=(b['address'],b['category'])];bookmarks.append(b)
wr(R/'work/handoff-project-bookmarks-current.json',sorted(bookmarks,key=lambda x:(x['address'],x['category'])))
latest='Shadow Walk source516b passes8checks; explicit phase/mode reader519 passes native520 with9checks. Exact alternate mode state0/class1c7ed28, stance2 and power phases2/3/4/5/0 observed. Entry1000ms/exit1133ms own player body, both arms and four fake-arm helpers. Stable form lasts about20s in this unupgraded fixture and exits to crouch. Native arm/hand ash, idle/loco and owner screen effects enumerated; remote form/VFX not bound yet. Live proxy515 preserved; read-only probe519 advances separately. Protected14 and original Steam image/DLL remain preserved.'
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=now
state['ShadowWalkContinuation']=dict(at=now,latest=latest,receipt='work/movement-integration/shadowwalk-source-verification-v521.json',complete=False,next=receipt['next'],sourcePassed=True,remoteBound=False)
wr(R/'work/current-task-state.json',state)
annotations=rd(N/'review-evidence-annotations.json');annotations['items'].setdefault('emily_shadow_walk_enter_move_attack_exit',{}).update(notes=latest,complete=False);wr(N/'review-evidence-annotations.json',annotations)
p=D/'36-native-shadow-walk-investigation.md';s=p.read_text(encoding='utf-8')
s+='''

## Native source and reader verification515–521

[515](../../work/movement-integration/candidate-shadowwalk-inventory-v515/identity.json) adds the exact Emily inventory/component identity and an opt-in native inventory fixture with20/40second limits, AI-held isolated Dunwall gating, mutual power exclusion and quantity/selection restoration. It uses native inventory API calls and normal controller input. The existing515 DLL remains live; [519](../../work/movement-integration/candidate-shadowwalk-probe-v519/identity.json) advances only the read-only probe. The original shared command wrapper omitted the new command; failed516 records that refusal before acquisition. Its finite command was added and the prior wrapper archived. No approval or security policy was bypassed.

[516b](../../work/natural-moveset/native-shadowwalk-standing-v516b/result.json) passes8 checks. LT220ms activates the native power; conditional forward/backward550ms inputs run only after observing exact ShadowWalk class. The actual form moves on the measured floor and exits naturally to crouch. Mana starts100 and finishes80 after replenishment; this is not an exact40-cost measurement. Entry scene is1000ms, exit1133ms; native hide-left-arm3666ms and equip333ms are separate scenes. Entry and exit each bind player handle1, native left/right arms and four temporary Emily/Corvo arm models. All bindings are reciprocal. Original inventory/health, both isolated checkpoints and protected14 are restored.

[Correlation518](../../work/movement-integration/native-shadowwalk-correlation-v518.json) distinguishes a critical native mode change: slot0 becomes class`1c7ed28` only in an alternate state-machine mode. Physics stance becomes2; default Running0 cannot identify the form. Component+`0xc0` points back to that exact state machine. Power+`0xe4` correlates with preparation2, entry3, sustained form4, exit5 and recovery0. Phase1 was not observed. The alternate mode can remain briefly during exit at stance1/phase0 or5. The sustained form is about20seconds in this fixture. These are observed correlations, not a complete enum or upgrade claim.

[Reader519](../../outputs/d2-coop-lab/src/native_player_shadowwalk_state.h) requires selected owned Emily equipment, reciprocal state-machine ownership, exact class/mode/stance/phase combinations and consistent final identity reads. Ownership tests reject foreign owners, wrong slot0 class, default-mode impersonation, unsupported phase and concurrent mode/phase retirement. A single-test PowerShell array-flattening error was corrected in the build wrapper before its successful test run. [Native520](../../work/natural-moveset/native-shadowwalk-state-v520/result.json) passes9 checks:863 watcher samples,857 accepted,639 sustained-form samples, all five observed phases. Six rejected snapshots are not repaired or treated as usable state.

The [source receipt521](../../work/movement-integration/shadowwalk-source-verification-v521.json) preserves both results and independent FX records. Original520's brief FX summary used the wrong property name; its full snapshots were correct. [Corrected offline summary](../../work/natural-moveset/native-shadowwalk-state-v520/strict-state-observations-corrected-v521.json) reads `effects.effects` and retains the original erroneous summary for audit. It finds arm/hand idle, arm/hand locomotion, arm ashes, Shadow Walk vision and end-screen effects. The persistent effects include owner handle1; transient arm tracks also have native temporary FX owners. This does not yet establish every socket association or authorize copying owner screen effects to another player.

Actual first-person recordings and temporal sheets are indexed in the dashboard. They show the low dark form and native camera/arms/sword recovery. They are source references, not remote visual acceptance. Remote full-body form, shadow material, native cosmetic particles, attack contact, directional motion, sprint upgrades and rat-hole/wall transitions remain unfinished.

## Further confirmed static FX findings518

Actual Ghidra decompilation [1408207e0](../../work/movement-integration/native-shadowwalk-1408207e0-decompile-v518.txt) starts/stops member+`0x40`/`0x50` effects, compares velocity magnitude against declaration+`0x118` and responds to a separate native flag. [140820730](../../work/movement-integration/native-shadowwalk-140820730-decompile-v518.txt) cleans up IDs+`0x2c`/`0x30`/`0x34` and both members. [140820a20](../../work/movement-integration/native-shadowwalk-140820a20-decompile-v518.txt) updates effect transforms. These FX fields are separate from the power phase enum. The [bookmark518](../../work/movement-integration/native-shadowwalk-bookmark-v518.json) was confirmed by an actual lookup and the persistent Ghidra project saved successfully.

The [Void export518 manifest](../../outputs/d2-coop-lab/research/asset-exports/shadowwalk-native-v518/manifest.json) retains source hashes and index-integrity checks. Native transition helpers inherit first-person arms and substitute Shadow Walk materials; no complete third-person shadow creature has been verified from these resources. Use the native effects as references while adapting the authored full-body motion; keep replicated root position authoritative and avoid native damage events in presentation.
'''
p.write_text(s,encoding='utf-8')
print(json.dumps(dict(videos=len(manifest),bookmarks=len(bookmarks),sourcePassed=True,remoteBound=False)))
