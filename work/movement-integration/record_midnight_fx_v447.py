from pathlib import Path
import json,hashlib,shutil,datetime,subprocess,collections
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11';A=W/'candidate-midnight-fx-probe-v445'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat();result=rd(N/'midnight-fx-v446/result.json');identity=rd(A/'identity.json')
assert result['passed'] and len(result['checks'])==16
assert result['dll']==identity['dll']==sha(R/'work/asset-game/dinput8.dll')
assert result['probe']==identity['probe']==sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')
coverage=rd(N/'midnight-fx-v446/strict-scene-coverage.json');assert {x['variant']for x in coverage}=={1,2}
assert all(x['samples']>=3 for x in coverage)
rows=[json.loads(s)for s in(N/'midnight-fx-v446/native-effects.ndjson').read_text(encoding='utf-8-sig').splitlines()if s.strip()]
strict=[r for r in rows if r['worldStable'] and r['cloneScene']['valid']]
summary=dict(samples=len(rows),stableSceneSamples=len(strict),phaseCoverage=coverage,maxSceneReadMs=max(r['cloneScene']['readMs']for r in rows),unknownLines=dict(collections.Counter(str(r['cloneScene']['failureLine'])for r in rows if r['worldPresent'] and not r['cloneScene']['valid'])))
scope='Source-only strict clone summon/disappearance scene reader and helper ownership verified in446, plus15 existing body/Steam/health/cleanup checks. Exact source clocks are1000ms spawn and466ms native dispawn. DLL427 remains unchanged: no remote summon/disappearance FX playback yet. Four native-derived cosmetic resources441 are staged, not installed.'
wr(A/'runtime-verification-v447.json',dict(at=now,result=result,sourceScenes=summary,complete=False,scope=scope))
identity.update(runtimeVerified=True,runtimeVerificationScope=scope);wr(A/'identity.json',identity)
old=W/'candidate-midnight-fx-probe-v438';prior=rd(old/'identity.json');prior.update(runtimeVerified=True,runtimeVerificationScope='440 passes15 checks: reciprocal scene/group/FX-track/key/helper observation matches independent FX map. DLL427 unchanged.');wr(old/'identity.json',prior);wr(old/'runtime-verification-v440.json',rd(N/'midnight-fx-v440/result.json'))
for p in [Path(__file__),W/'Run-MidnightFx-v446.ps1',N/'Test-MidnightFx-v446.ps1',W/'midnight-fx-reader-tests-v442.log',W/'ghidra-bookmarks-additions-v441.json',W/'midnight-group-rtti-v438.json',N/'build_doppelganger_fx_assets_v441.py',N/'doppelganger-fx-assets-v441.json',*W.glob('native-doppelganger-fx-*-decompile-v438.txt')]:shutil.copy2(p,A/p.name)
books=rd(R/'work/handoff-project-bookmarks-current.json');notes=rd(W/'ghidra-bookmarks-additions-v441.json');books=[b for b in books if not any(b['address']==n['address']and b['category']==n['category']for n in notes)]+notes;wr(R/'work/handoff-project-bookmarks-current.json',books)
manifest=rd(N/'runtime-video-manifest.json')
for take in ('doppelganger-midnight-v437','midnight-fx-v439','midnight-fx-v440','midnight-fx-v443','midnight-fx-v446'):
 res=rd(N/take/'result.json')
 for suffix in ('','-source'):
  name=take+suffix+'.mp4';assert not any(x['video']==name for x in manifest),name
  video=N/name;meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
  note=scope if res['passed']else 'Retained failed research harness: '+res['failure']+'. Native body path unchanged; checkpoint/protected14 restoration passed. See chapter35 for correction.'
  review=N/take/('visual-review'+suffix+'.json');wr(review,dict(inspected=False,contactSheet=None,scope='Research capture; body comparison remains the viewed430. '+note,complete=False,visualAccepted=False))
  manifest.append(dict(video=name,character='emily',label=f'Doppelganger native effect scene · {"source"if suffix else"remote"} ·{take.rsplit("v",1)[1]}',alias='native_doppelganger_cast',requirements=['emily_doppelganger_cast'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=res['dll'],actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),video_sha256=sha(video),checks_passed=res['passed'],scope=note))
wr(N/'runtime-video-manifest.json',manifest)
state=rd(R/'work/current-task-state.json');state['DoppelgangerContinuation'].update(at=now,live=identity['dll'],probe=identity['probe'],receipt='work/movement-integration/candidate-midnight-fx-probe-v445/runtime-verification-v447.json',bodyReceipt='work/movement-integration/candidate-doppelganger-visual-v427/runtime-verification-v431.json',latest=scope,complete=False,next='Integrate confirmed native source scene identity/variant/time into the existing clone pose channel (currentlyv2/10696); receiver must deduplicate by world/clone/scene incarnation, never pooled pointers. Reuse proven Mesmerize Midnight start/stop API with exact cosmetic clone identity and441 FX/material-only scenes, initialized from source time. Install441 only into stopped physical asset-game using preserved backups and original Steam hash verification. Test actual source+remote summon/despawn visuals, native219 final pose, no AI/damage, two casts/reuse, teardown/reload and protected14. Native assets441 are prepared/staged only; no peer FX yet. Partner edge cases deferred; ordinary collision retained.');state['UpdatedAt']=now;wr(R/'work/current-task-state.json',state)
with(D/'35-doppelganger-cast-and-clone-lifecycle.md').open('a',encoding='utf-8')as f:f.write('''

## Confirmed clone effect scene linkage437–447

[437](../../work/natural-moveset/doppelganger-midnight-v437/result.json) passed14 checks with the unchanged427 DLL and434 probe. It captured the exact native1000ms clone summon scene with one reciprocal actor binding, matching the independently enumerated source clone. [Raw instance](../../work/natural-moveset/doppelganger-midnight-v437/clone-spawn-instance-object.json) exposed its `arkMidnightGroupInst` entry.

[438 group RTTI](../../work/movement-integration/midnight-group-rtti-v438.json) and saved exact-image decompilations establish the layout below; [440](../../work/natural-moveset/midnight-fx-v440/result.json) then passed15 runtime checks against it. All five helper handles match independent native FX-map owners and declarations, with exact socket names. Scene, group and track pointers are pooled:439 reused the same addresses for spawn scene221 and disappearance scene223. A pointer is therefore not a scene incarnation key.

| Object | Confirmed field or relationship |
|---|---|
| Scene instance | +10 array, +18 count; entries32bytes: group+0, actor+8, name+10, ID+18. Scene handle+158, current clock+160, state+27c. |
| Group instance | RTTI vtable1c6ca98; +20 pointer array/+28 count of tracks; +c8 scene instance; +d0 bound actor. |
| FX track | RTTI1c7f5a8; +10 reciprocal group; +18/+20 active key array24bytes; +c0/+c8 FX helper records12bytes. |
| Helper record | Packed key ID, helper entity handle, stop mode1. Record order differs between summon and disappearance. |
| Native key | +8 packed ID; +28 property. Property RTTI1afdfd0, +8 FX declaration, +10 socket name. |

The five packed key IDs67428353–67428357 resolve respectively to `decoy_root`, `sl_l_arm`, `sl_r_arm`, `decoy_spawn_l_hand`, `decoy_spawn_r_hand`. Both scenes use five distinct `idEntity` helpers. Root and body effect declarations are native; helper parent handles alone do not encode clone ownership. The disappearance filename is literally `doppelganger_dispawn.midnightscene`, duration466ms; do not correct that spelling when selecting the native source.

Saved decompilations: [scene binding constructor7ce7e0](../../work/movement-integration/native-doppelganger-fx-1407ce7e0-decompile-v438.txt), [group constructor7db090](../../work/movement-integration/native-doppelganger-fx-1407db090-decompile-v438.txt), [actor binding7dc130](../../work/movement-integration/native-doppelganger-fx-1407dc130-decompile-v438.txt), [group setup7dc1d0](../../work/movement-integration/native-doppelganger-fx-1407dc1d0-decompile-v438.txt), [group destructor7db430](../../work/movement-integration/native-doppelganger-fx-1407db430-decompile-v438.txt), [track serializer7e0690](../../work/movement-integration/native-doppelganger-fx-1407e0690-decompile-v438.txt). The group finding is saved at1407db090 in the persistent Ghidra project and the local bookmark index.

Two failed research takes are preserved.439 incorrectly counted nine records because PowerShell added four null values for tracks without a `records` property; the actual FX track had five valid helpers.440 filters absent members and passes.443's18-second observers missed the disappearance: the clone remained present to video21.691s and cleared by22.371s. No native reader failure was present in that interval.445 extends only the bounded external observers to32seconds maximum;446 uses30seconds and verifies both phases.

[Strict reusable reader](../../outputs/d2-coop-lab/src/native_doppelganger_fx_scene.h) validates the exact source clone, declaration, scene duration/clock, reciprocal group and track, five key/socket/effect/helper relationships and stable identity/list membership. Successful stable absence is distinct from failed reads. Synthetic tests cover pooled-pointer reuse, foreign binding, duplicate/foreign helper, wrong socket, invalid clocks and concurrent scene-ID changes. [442 test log](../../work/movement-integration/midnight-fx-reader-tests-v442.log) passes. This reader is currently used by the external probe, not yet by the game DLL.

[446](../../work/natural-moveset/midnight-fx-v446/result.json) passes16 checks. [Scene coverage](../../work/natural-moveset/midnight-fx-v446/strict-scene-coverage.json) records sample counts, source time ranges and measured read cost. Both scene IDs match the independent Midnight observer; all five helpers match the same-snapshot FX map. The live build remains DLL427, bank55/322, semantic protocol52/336 and clone channelv2/10696. [447 receipt](../../work/movement-integration/candidate-midnight-fx-probe-v445/runtime-verification-v447.json) pins the current445 probe, strict reader evidence and unchanged DLL. Both checkpoints, health/inventory and14 protected files are restored.

## Prepared cosmetic effect assets441, not installed or played

[Generator](../../work/natural-moveset/build_doppelganger_fx_assets_v441.py), [four-resource provenance](../../work/natural-moveset/doppelganger-fx-assets-v441.json) and [staging receipt](../../outputs/d2-coop-lab/research/doppelganger-fx-v441-asset-plan.json) reproduce two native-derived FX/material scenes and two root FX declarations. Scene durations, five sockets, particle references/timings and material fade curves are retained. Animation, face and scene sound tracks are excluded; the ordinary root cast/despawn sound remains in its FX. Rumble and the `bsp_ai_properties_02` game-defined sound entry are removed. The current experiment lists only the exact219-joint Emily model and disables feet IK. Body FX continue referencing the original native declarations.

The package was staged with the pinned Void.Utils tooling; no game file was replaced and original Steam indices match. The generator initially had a Python conditional whitespace syntax error, corrected before any execution or asset write. Native parse/playback and visual equivalence are still unverified. Native material keys retain `clearAtEnd=false`; reuse, interrupted cleanup and the next summon must explicitly be tested before promotion. Do not claim the four resources are installed merely because they appear in a manifest.

Next implementation: transport confirmed source scene ID/variant/clock alongside clone identity and pose; use the existing native Midnight start/stop ABI to bind only the cosmetic clone and initialize elapsed time. Deduplicate by session/world/clone/scene incarnation and preserve network-owned root/all219 joints. Never replay the original gameplay/animation scene. Then install only into the stopped physical lab clone, test source versus remote particles/fade/timing, two casts, teardown and checkpoint reload, and update the review dashboard. Peer summon/disappearance effects and shared clone gameplay remain incomplete.
''')
print(json.dumps(dict(videos=len(manifest),coverage=coverage,probe=identity['probe'],complete=False)))
