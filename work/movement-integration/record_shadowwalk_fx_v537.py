from pathlib import Path
import json,datetime,hashlib,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat();take='native-shadowwalk-fx-v535';result=rd(N/take/'result.json');assert result['passed']
assert rd(W/'native-operation-owner.json')['state']=='finished'
fx=rd(N/take/'remote-fx-observations.json');body=rd(N/take/'remote-body-observations.json');travel=rd(N/take/'long-travel-observations.json')
assert fx['started']==fx['stopped']==24 and fx['refused']==0 and fx['nativeRecordsRetired'] and body['clockRefused']==0
scope='Native Emily Shadow Walk complete-body entry/idle/forward/backward/exit and six native arm/hand/ash particle resources, driven by accepted native Steam state and owned published peer joints. First-person screen effects remain local. Body shader/morphology, attack, dedicated directional clips, broader traversal and final quality remain incomplete.'
review=dict(at=now,inspected=True,sheets=['fx-sheet.jpg','source-sheet.jpg'],method='Viewed temporal sheets extracted from actual paired game recordings.',observations=['Remote body lowers, crawls forward and returns backward, then recovers with native sword.','Observer has its normal game view throughout; native low camera/vision/end prompt remain in the source view.','The remote body still looks human. Particle presence/ownership is confirmed separately by native records; the sheet does not establish strong shadow appearance.'],limitations=['Column partly occludes entry/idle; moving phases are visible.','No final contact, morphology, VFX quality or sustained performance acceptance.','Temporal sheets cannot prove continuous motion quality.'],complete=False,visualAccepted=False)
wr(N/take/'visual-review.json',review)
manifest=rd(N/'runtime-video-manifest.json')
for suffix in ('','-source'):
 video=N/(take+suffix+'.mp4');assert not any(x['video']==video.name for x in manifest)
 meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
 manifest.append(dict(video=video.name,character='emily',label='Shadow Walk '+('source' if suffix else 'remote native particles')+' ·535',alias='shadow_walk_native_reference' if suffix else 'shadow_walk_body_fx',requirements=['emily_shadow_walk_enter_move_attack_exit'],gameplay_bound=not bool(suffix),complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=take+'/visual-review.json',proxy_sha256=result['dll'],video_sha256=sha(video),actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),checks_passed=True,scope=scope))
wr(N/'runtime-video-manifest.json',manifest)
receipt=dict(at=now,result=result,scope=scope,scopeCorrection='The raw test inherited its body-only summary from525. Its additional14th check and remote-fx-observations explicitly establish particles; this receipt states the actual scope without rewriting the original.',body=body,travel=travel,particles=fx,visualReview=review,complete=False)
wr(W/'shadowwalk-fx-verification-v537.json',receipt);wr(W/'candidate-shadowwalk-fx-v533/runtime-verification-v537.json',receipt)
bookmarks=rd(R/'work/handoff-project-bookmarks-current.json');new=json.loads(rd(W/'native-shadowwalk-bookmark-v536.json')['lookup']['structuredContent']['result'])['bookmarks']
for x in new:
 bookmarks=[b for b in bookmarks if (b['address'],b['category'])!=(x['address'],x['category'])];bookmarks.append(x)
wr(R/'work/handoff-project-bookmarks-current.json',bookmarks)
latest='533/535 passes14 scoped native Shadow Walk body/FX checks. Entry, idle,4.68m forward/backward travel (1.94 distance cycles) and exit are network-driven; actual peer sword stays holstered and body clock has no rejection. Six native arm/hand/ash resources produce241 owned snapshots,24 starts/24 stops,1870 joint updates, zero refusals; all records retire after exit with no observer vision/end-screen effect. Paired video reviewed provisionally; body remains human and final quality unaccepted. Native FP, checkpoint/inventory/health and14 protected hashes retained. Prior525 failure and526 repair preserved. Complete movement/co-op unfinished.'
nextWork='Trace native material-effect parameter binding and per-body material override safely; then test shadow morphology, lateral/turning/attack/upgrade/traversal and interruption contexts. Keep509 intermittent air landing issue open; defer rare partner interactions, preserve normal movement collision.'
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=now;state['ShadowWalkContinuation'].update(latest=latest,receipt='work/movement-integration/shadowwalk-fx-verification-v537.json',nativeParticlesBound=True,complete=False,next=nextWork);wr(R/'work/current-task-state.json',state)
ann=rd(N/'review-evidence-annotations.json');ann['items']['emily_shadow_walk_enter_move_attack_exit'].update(notes=latest,complete=False);wr(N/'review-evidence-annotations.json',ann)
p=D/'36-native-shadow-walk-investigation.md';s=p.read_text(encoding='utf-8')
s+='''

## Native remote particle verification535–537

[535](../../work/natural-moveset/native-shadowwalk-fx-v535/result.json) passes14 checks on archived533. Its actual source/observer recordings are indexed. The native body covers4.684692m and1.935007 authored distance cycles, with a settled stop phase0.068768 and no body-clock rejection. The native peer sword remains holstered during the form. [Particle observations](../../work/natural-moveset/native-shadowwalk-fx-v535/remote-fx-observations.json) show all six whitelisted resources on the exact peer generation:241 native FX snapshots,24 starts/24 stops,1870 posed-joint updates and zero refusals. All native records retire after exit. No vision/end-screen effect is present on the observer. Inventory/health, both checkpoints and14 protected file hashes restore.

[Receipt537](../../work/movement-integration/shadowwalk-fx-verification-v537.json) distinguishes this from the inherited body-only summary in the raw script result; original evidence is retained. Viewed temporal sheets confirm crawl/recovery and the source's native camera/vision/sword, but remote particle strength and complete shadow appearance have no aesthetic acceptance. The body remains human; the camera partly occludes entry/idle. Sustained performance, continuous hand/foot contact, shader morphology, attacks and wider traversal remain unfinished.533 is a verified scoped body/particle candidate, not completion of Shadow Walk or the whole movement phase.

Further static [xref536](../../work/movement-integration/native-shadowwalk-material-function-xrefs-v536.json) points at1401bda25. Ghidra's interior-address lookup returned no function and its caller helper returned `No callers found for function: null`; neither is interpreted as proof of no caller. Exact-PE [unwind lookup](../../work/movement-integration/native-shadowwalk-material-unwind-v536.json) resolves the enclosing range1401bd910–1401bdb8f. [Actual decompilation](../../work/movement-integration/native-shadowwalk-1401bd910-decompile-v536.txt) shows a material-cache rebuild/getter using the previous1bd420 mapper, unique material resolution and non-null overrides into `*(this+0x1c40)+0x90/0x98`. It remains unsuitable as an assumed setter. [Bookmark536](../../work/movement-integration/native-shadowwalk-bookmark-v536.json) is confirmed and the Ghidra project saved.

The native transition declarations contain `EMTT_MATERIALRENDERPARM` / `arkMidnightKeyPropMaterialRenderParm` tracks for `materialeffects/effect` and `materialeffects/fadevalue`, with attachment propagation and clear-at-end flags. These are the next targeted leads for presentation; no render parameter or shared material declaration has been mutated yet.
'''
p.write_text(s,encoding='utf-8');print(json.dumps(dict(videos=len(manifest),bookmarks=len(bookmarks),checks=len(result['checks']),complete=False)))
