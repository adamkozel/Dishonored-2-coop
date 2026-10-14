from pathlib import Path
import json,datetime,hashlib,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
def wr(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
assert rd(W/'native-operation-owner.json')['state']=='finished'
now=datetime.datetime.now().astimezone().isoformat()
manifest=rd(N/'runtime-video-manifest.json');results=[]
for number in (463,465,469,470,473):
    take=f'doppelganger-fx-v{number}';resultPath=N/take/'result.json'
    if not resultPath.exists():continue
    result=rd(resultPath)
    assert 'All14 protected save/settings hashes retained' in result['checks']
    assert any(x.startswith('Both isolated checkpoints restore') for x in result['checks'])
    results.append(dict(take=take,passed=result['passed'],failure=result['failure'],checks=len(result['checks']),dll=result['dll']))
    for suffix in ('','-source'):
        video=N/(take+suffix+'.mp4')
        if any(x['video']==video.name for x in manifest):continue
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
        sheet=N/take/'remote-disappearance-sheet.jpg'
        reviewed=number==470 and not suffix
        scope=('Native finite cosmetic clone effects over Steam.469 verifies both native scene clocks, bindings and five helpers, body pose and cleanup. '
               '470 independently records running effects and a brief pre-retirement hide; its disappearance footage shows white particles but the column obscures the body. '
               'Same-actor reuse and final visual acceptance remain open. ')
        scope+=('This scoped take passed.' if result['passed'] else 'Failed take retained: '+result['failure'])
        review=N/take/('visual-review'+suffix+'.json')
        wr(review,dict(inspected=reviewed,contactSheet=sheet.relative_to(N).as_posix() if reviewed else None,scope=scope,complete=False,visualAccepted=False))
        manifest.append(dict(video=video.name,character='emily',label=f'Doppelganger cosmetic effects · {"source" if suffix else "remote"} ·{number}',alias='native_doppelganger_cast',requirements=['emily_doppelganger_cast'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=result['dll'],actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),video_sha256=sha(video),checks_passed=result['passed'],scope=scope))
wr(N/'runtime-video-manifest.json',manifest)
baseline=rd(W/'candidate-doppelganger-fx-v468/identity.json');good=rd(N/'doppelganger-fx-v469/result.json')
assert good['passed'] and good['dll']==baseline['dll']
effects=rd(N/'doppelganger-fx-v469/remote-effects-verification.json')
verified=dict(at=now,passed=True,dll=baseline['dll'],probe=baseline['probe'],scope='One standing armed Emily cast in isolated Dunwall: source and peer body/root/FX clocks and helpers, health/inventory and checkpoint/protected14 recovery. Native cosmetic scene force-update option prevents observed LOD pause. Not complete clone gameplay, movement acceptance, reuse or a release approval.',take='work/natural-moveset/doppelganger-fx-v469/result.json',effects=[{k:x[k] for k in ('variant','observations','minimumClock','maximumClock')} for x in effects],complete=False)
wr(W/'candidate-doppelganger-fx-v468/runtime-verification-v474.json',verified)
live=rd(W/'candidate-doppelganger-fx-v472/identity.json');assert sha(R/'work/asset-game/dinput8.dll')==live['dll']
for f in rd(W/'protected-save-hashes-before-v4.json'):assert sha(Path(f['Path']))==f['Hash']
receipt=dict(at=now,complete=False,live=live['dll'],probe=live['probe'],takes=results,verifiedBaseline=verified,securityBlock=dict(candidate=466,test467Ran=False,detail='Windows reported virus or potentially unwanted software when reading the diagnostic DLL during deployment. No antivirus settings changed; blocked build was removed from active use and preserved452 restored. First restoration encountered blocked destination hash, so known452 replaced only stopped physical lab DLL before relaunch. No claim of a false positive. Source hardware-watch changes and temporary pause hooks removed before468.'),sameActorReuseVerified=False,visibilityInvestigation='470 extra hide/reappear around18.475s, source/receiver fresh in adjacent samples;472 records gate masks at the exact hide instead of inferring cause.',nativeCollisionRetained=True,partnerEdgeCasesDeferred=True)
wr(W/'doppelganger-fx-investigation-v474.json',receipt)
state=rd(R/'work/current-task-state.json');state['DoppelgangerContinuation'].update(at=now,live=live['dll'],probe=live['probe'],receipt='work/movement-integration/doppelganger-fx-investigation-v474.json',latest='468 native force-update option repairs cosmetic disappearance LOD pause;469 passes17 checks and both effects advance.470 effects run but one transient visibility gate retires/re-shows the model.472 adds bounded exact hide diagnostics. No current hardware watch or pause detour. Same-actor material recovery still pending;471 never ran.',complete=False,next='Use473 hideHistory to identify the transient visibility gate, then validate two summons on the same model and improve the clear disappearance camera. Preserve verified468/469 and427 body baseline. Continue remaining power contexts and movement, then full shared world/campaign/UI/Steam requirements. Partner edge cases deferred; native movement collision retained.')
state['UpdatedAt']=now;wr(R/'work/current-task-state.json',state)
books=rd(R/'work/handoff-project-bookmarks-current.json')
note=dict(address='1407d9fa0',category='D2Coop confirmed static',comment='Midnight update invokes7cf580 then directly7cfa40 on instance+2b1 LOD suspension; parallel7da0b0 does likewise, bypassing manager pause. Creation option byte4 sets force-update+2b2 through7ce370.468 uses this only for finite cosmetic clone FX/material scenes;469 confirms both phase clocks advance without state5. Saved Ghidra program and native-midnight-1407d9fa0-v468.txt. Same-actor reuse/complete gameplay pending.')
books=[b for b in books if not(b['address']==note['address'] and b['category']==note['category'])]+[note];wr(R/'work/handoff-project-bookmarks-current.json',books)
chapter=D/'35-doppelganger-cast-and-clone-lifecycle.md';text=chapter.read_text(encoding='utf-8');marker='## Native LOD timing repair and visibility investigation462–474'
assert marker not in text
chapter.write_text(text+'''

## Native LOD timing repair and visibility investigation462–474

[463](../../work/natural-moveset/doppelganger-fx-v463/result.json) still saw disappearance paused in state5, while manager7d71f0 and deferred pause7d2d60 traces were empty. An asynchronous hardware watch in464/465 armed too late: [trace](../../work/natural-moveset/doppelganger-fx-v465/midnight-phase-trace.json) captured only later native stop state10 at nextRVA7d1a74. It restored all debug registers; the failed result remains unchanged.466 attempted explicit engine-thread arming before start, but Windows security blocked that DLL during deployment. No467 test ran. Antivirus settings were not changed. The blocked active diagnostic was replaced with hash-verified preserved452 in the stopped physical lab, and both fixtures/protected files were restored. The diagnostic source remains archived; **do not install466**. This report does not assert a false positive.

All temporary hardware-watch additions and pause detours were removed before468. The only468 production difference from452 is the native scene-creation option byte4 set to1 for our own finite FX/material-only scenes. Scene identity, target ownership, network position, all219 joints and source lifetime remain guarded as before. Original Steam assets/settings/saves are unchanged.

### Confirmed native cause and bounded fix

[Per-instance update7d9fa0](../../work/movement-integration/native-midnight-1407d9fa0-v468.txt) and [parallel worker7da0b0](../../work/movement-integration/native-midnight-1407da0b0-v468.txt) call7cf580, then directly invoke7cfa40 when instance+2b1 indicates LOD suspension. That direct path bypasses the manager trace. [7cf580](../../work/movement-integration/native-midnight-1407cf580-v462.txt) skips this suspension when instance+2b2 is1;7ce370 sets that flag from creation option byte4. The recorded paused457 object also has+2b1=1. No scene-resume loop or arbitrary scene-state write is needed. Read-only PE displacement scans and additional Ghidra analysis are preserved in468 scripts/results; a bookmark at1407d9fa0 was saved in the persistent project. Earlier incomplete Ghidra xref output alone did not reveal these calls.

[469 passes17 checks](../../work/natural-moveset/doppelganger-fx-v469/result.json), including both native effect scenes with exact source IDs, reciprocal model bindings, only two cosmetic track types and five independently matched FX helpers each. Spawn has29 running observations at79–988ms; disappearance has14 at51–426ms. Neither is observed paused. One model follows source body/root, hides on source retirement, and health/inventory, both checkpoints and protected14 restore. [Scoped verified468 receipt](../../work/movement-integration/candidate-doppelganger-fx-v468/runtime-verification-v474.json) pins the successful DLL/probe. This is not complete power or release acceptance.

[470 failed](../../work/natural-moveset/doppelganger-fx-v470/result.json) because its body hid/reappeared once around18.475s before the expected retirement. Adjacent snapshots still show source and receiver present, ready and leased; they do not identify the instantaneous failing gate. Both effects nevertheless have independently observed state3 clocks, with28 summon and15 disappearance observations. [Viewed disappearance sheet](../../work/natural-moveset/doppelganger-fx-v470/remote-disappearance-sheet.jpg) shows the white burst and its clearing, but a pillar obscures the model: do not call this final visual acceptance. The second same-world take471 never ran.470 restored both checkpoints and protected14.

472 preserves468 behavior and adds a bounded16-entry hide history. Main mask bits:1 stream identity/currentness,2 lease expiry,4 source absence,8 movement bank,16 local gameplay readiness,32 remote readiness,64 observer readiness,128 fixture. Pair-mask bits:1 follow,2 engine-frame ownership,4 world pairing,8 exact-follow,16 avatar AI hold,32 avatar identity,64 locomotion,128 stream expiry. Signed receive age and lease remaining are sampled on the actual hide. This avoids guessing from a later healthy status. [Current receipt](../../work/movement-integration/doppelganger-fx-investigation-v474.json) records473 when available and the exact live hash.

Remaining: transient visibility cause, same-model second summon/material reset, unobstructed body/fade footage, interrupted/reload/session cleanup during effects, other stance/equipment contexts, upgraded/multiple clones and shared gameplay outcomes. Normal partner movement collision remains allowed; exhaustive partner interaction edge cases remain deferred per user direction. No skill was invoked for this C++/PowerShell/Ghidra analysis.
''',encoding='utf-8')
print(json.dumps(dict(videos=len(manifest),results=results,live=live['dll'],complete=False)))
