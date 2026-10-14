from pathlib import Path
import json,datetime,hashlib,subprocess
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration';take='possession-release-network-v335';T=P/take
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
r=read(T/'result.json');e=read(T/'release-network-evidence.json');assert r['passed'] and len(r['checks'])==12 and e['presented']==6
review=dict(inspected=True,at=datetime.datetime.now().astimezone().isoformat(),method='Viewed both temporal contact sheets at .8,2.1,2.6,3,3.2,3.4,4.3,6.7,8 seconds',evidence=['peer-sheet.jpg','source-sheet.jpg'],complete=False,visual_accepted=False,notes='Peer shows Corvo fullbody raised left arm and native sword holster during successful release. Source shows target highlight/travel/host vision and native exit. Peer avatar moves out of view toward source host position; this does NOT establish correct disappearance or remote occupancy. Tutorial popup and dark body limit hand-detail judgment. Full native host visibility/return, replicated VFX and target gameplay remain incomplete. Captures have no audio.')
write(T/'visual-review.json',review)
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');item=ann['items']['corvo_possession_enter_loop_exit']
for suffix,label in [('', 'remote full-body successful release'),('-source','native human host cast and exit')]:
 v=P/(take+suffix+'.mp4');info=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(v)],text=True));n,d=map(float,info['streams'][0]['avg_frame_rate'].split('/'))
 row=dict(video=v.name,character='corvo',label='Possession · '+label+' · v335',alias='native_possession_out',requirements=['corvo_possession_enter_loop_exit'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',visual_review=f'{take}/visual-review.json',proxy_sha256=r['dll'],actual_capture_hz=n/d,video_sha256=hashlib.sha256(v.read_bytes()).hexdigest().upper(),checks_passed=True,scope=review['notes'])
 videos=[x for x in videos if x['video']!=v.name];videos.append(row);item.setdefault('native_integration',{})[v.name]=row
item['notes']='335 passes12 checks for native400ms successful Corvo Possession release over native Steam protocol49 and bank52/316. One exact scene233/event10 sent and accepted, six sampled fullbody release poses with holstered sword. All315 earlier clip payloads retained. Native target identity, acquired power through occupation, native exit, exact office-key delta accounting, full checkpoint inventory/health restoration and protected14 pass. Peer disappears from camera through root movement; intentional occupancy visibility is not implemented or proven. Both videos viewed. All movement and co-op remain incomplete.'
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',ann)
write(W/'candidate-possession-release-body-v334/runtime-verification.json',dict(result=r,network=e,visualReview=review,complete=False))
f=R/'documentation/handoff-2026-10-11/32-native-possession-investigation.md';s=f.read_text();s+='''

## Remote successful-release playback,335

[335 passes12 checks](../../work/natural-moveset/possession-release-network-v335/result.json) on archived DLL334, protocol49 and bank52/316. [Native scene233/event10 is sent and accepted once](../../work/natural-moveset/possession-release-network-v335/release-network-evidence.json), with six sampled full-body release poses and a holstered sword. All315 previous clip payloads are preserved. Source target ownership, native acquired-power lifetime, normal-input exit, exact office-key delta accounting and complete post-checkpoint inventories/health pass; protected14 remain unchanged. Final native process exit0 consumed.

[Both videos were viewed](../../work/natural-moveset/possession-release-network-v335/visual-review.json). The remote body raises its left arm and holsters its sword. It subsequently follows the source root toward the host and moves outside this camera view; **this is not proof of correct body disappearance**. Unsupported occupation still retires the semantic stream. Host visibility/return, VFX and shared control remain unfinished. The native first-person cast, tunnel and host vision remain local.

334 failed because the originally upper-floor tutorial guard1 descended before casting. Its source recording shows empty aim, and no native release was observed.335 limits the fixture to the observed upper-floor tutorial guard2. Decision/reflex isolation does not guarantee an already scripted NPC path is frozen. This limitation is recorded rather than treating all failed casts as animation bugs.

Ghidra [visibility methods](../../work/movement-integration/possession-visibility-decompile-v334.txt) confirm8784b0/87c590 toggle entity+D0 bit0 and call87e040; these APIs already support exact sword child visibility. [Possession state effects](../../work/movement-integration/possession-state-effects-decompile-v335.txt) show separate camera/effect flag transitions and five indexed native FX slots. Static call relationships do not authorize applying player-local camera effects to a peer. Body visibility will require its own exact clone ownership lease and runtime verification.
''';f.write_text(s,encoding='utf-8')
print(json.dumps(dict(videos=len(videos),checks=len(r['checks']),remoteReleaseSamples=e['presented'])))
