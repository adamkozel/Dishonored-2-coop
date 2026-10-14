from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration';D=R/'documentation/handoff-2026-10-11';take='possession-empty-v322';t=P/take
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
r=read(t/'result.json');assert r['passed'] and (t/'source-sheet.jpg').exists()
obs=read(t/'possession-observations.json');reader=read(t/'reader-summary.json');now=datetime.datetime.now().astimezone().isoformat()
review=dict(inspected=True,at=now,evidence=['source-sheet.jpg'],method='Viewed actual source temporal sheet at0.6/1.9/2.2/2.8/3.4/4.3/6s',notes='Native Corvo left hand enters the aim pose, then lowers after no-target release. Native power icon/cancel hint remain local. No target transfer, third-person body, or remote effect demonstrated.',visual_accepted=False,complete=False)
write(t/'visual-review.json',review)
video=P/(take+'.mp4');meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True));n,d=map(float,meta['streams'][0]['avg_frame_rate'].split('/'))
row=dict(video=video.name,character='corvo',label='Possession native source · empty target · v322',alias='native_possession_in',requirements=['corvo_possession_enter_loop_exit'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',visual_review=f'{take}/visual-review.json',proxy_sha256=r['dll'],actual_capture_hz=n/d,video_sha256=hashlib.sha256(video.read_bytes()).hexdigest().upper(),checks_passed=True,scope=r['scope'])
videos=read(P/'runtime-video-manifest.json');videos=[v for v in videos if v['video']!=video.name];videos.append(row);write(P/'runtime-video-manifest.json',videos)
ann=read(P/'review-evidence-annotations.json');item=ann['items']['corvo_possession_enter_loop_exit'];item.setdefault('native_integration',{})[video.name]=row
item['runtime_binding']='native_source_empty_target_observed_remote_pending';item['notes']='322 source-only empty-target Corvo cast passes6 identity/cleanup checks. Exact native333ms entry and200ms aim loop bind one left arm under Running0+UsePower63. Mana unchanged; no out scene. Strict read-only reader rejects incomplete bindings. Native inventory rollback, both checkpoints and protected14 retained. Remote full-body binding, valid-target transfer, visibility/reappearance, effects and shared target outcomes remain incomplete.';write(P/'review-evidence-annotations.json',ann)
write(W/'candidate-possession-fixture-v322/runtime-verification.json',dict(at=now,result=r,observations=obs,reader=reader,visualReview=review,complete=False))
old=read(R/'work/handoff-project-bookmarks-current.json');merged={(b['address'],b['category']):b for b in old}
for b in read(W/'ghidra-bookmarks-additions-v322.json'):merged[(b['address'],b['category'])]=b
write(R/'work/handoff-project-bookmarks-current.json',sorted(merged.values(),key=lambda b:(b['address'],b['category'])))
p=D/'32-native-possession-investigation.md';s=p.read_text(encoding='utf-8');heading='## Native empty-target evidence, candidates321–322'
if heading not in s:
 s=s.replace('Status on 12 October 2026: static analysis and baseline inventory inspection only. Possession has not been acquired or exercised in the runtime test. Neither remote body binding nor possessed-target control is implemented by this work.','Status on 12 October 2026: native empty-target acquisition/cast observations pass in candidate322. Remote body binding and possessed-target control remain unimplemented. Earlier sections record the static investigation before this test.')
 s+=f'''\n\n{heading}

321 added the strict equipment reader, finite acquisition fixture and read-only probe. The first run stopped before acquisition because the external command sender had not registered the new command. Original and corrected sender scripts are preserved in the321 archive.321b uses identical DLL/probe with the finite command registration fixed. Native acquisition and rollback succeeded, but the raw-scene assertion failed one sample: a newly initialized loop had state3/clock0 and an empty binding list. Of55 active samples,54 had a reciprocal Corvo arm. The failed result is retained; it is not indexed as success.

322 adds a strict read-only scene reader and separate watcher. It accepts only the observed grounded Running0+UsePower63 context, exact selected native Possession equipment, entry333ms or loop200ms, a single reciprocal Corvo left arm, and stable scene/arm/item/state rereads. Unobserved release and transfer remain excluded. Raw and strict samples are separate observations; their counts cannot be subtracted to prove identical sample rejection.

[322 final result](../../work/natural-moveset/possession-empty-v322/result.json) passes6 checks. The strict watcher accepted57 samples; raw watcher observed57 active samples including1 incomplete initialization.422 strict samples were invalid/idle. Mana remained100 before and after empty-target release. Source states were0 and0+63. Native acquisition restored its original quantity/selection, both isolated checkpoints restored, and all14 protected save/settings hashes passed. Full operation exited0 before further native work. The source temporal sheet was viewed and its video indexed; no third-person acceptance follows from this source recording.

DLL `{r['dll']}`, probe `{r['probe']}`. [Candidate and runtime receipt](../../work/movement-integration/candidate-possession-fixture-v322/runtime-verification.json) pin the scope. Ghidra bookmark `141b531c8 / D2Coop/Possession-runtime-v322` is saved. [Transfer helper822a70](../../work/movement-integration/possession-transfer-decompile-v321.txt) was also decompiled; its control, camera and target side effects remain static hypotheses, and no direct invocation was added.

Next: bind only the verified native entry/aim to authored whole-body poses, preserving network root control and local first person. Empty-target recovery must follow actual native scene retirement. Valid-target out/transfer, target authority, body disappearance/reappearance, crouch/slide/swim, upgrades, death/checkpoint interruptions and VFX still need runtime investigation.
''';p.write_text(s,encoding='utf-8')
print(json.dumps(dict(videos=len(videos),bookmarks=len(merged),checks=len(r['checks']))))
