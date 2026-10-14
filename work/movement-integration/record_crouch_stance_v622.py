from pathlib import Path
import json, hashlib, datetime, subprocess
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();A=W/'candidate-crouch-stance-v620';pin=rd(A/'identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll'] and sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==pin['probe']
receipt=W/'crouch-stance-verification-v622.json';assert not receipt.exists()
tests=[]
for role in ('corvo','emily'):
 f=N/f'crouch-combat-{role}-v621';r=rd(f/'result.json');c=rd(f/'context-coverage.json')
 assert r['passed'] and len(r['checks'])==10 and r['dll']==pin['dll'] and c['ownedHitRecoverySamples']==0
 tests.append(dict(role=role,result=str((f/'result.json').relative_to(R)),sha256=sha(f/'result.json'),checks=r['checks'],coverage=c))
limitations=['Target-bearing crouched strike and native stance recovery verified on both roles; no optional separate hit-recovery15/16 scene occurred in621. Those branches remain runtime-unverified despite native613 source observations and passing synthetic tests.','Both621 observer contact sheets inspected: body crouches, strikes and recovers standing. Guard occlusion limits sword/contact inspection. Fine pose/grip/timing polish deferred, no visual acceptance.','Receiver enemy health unchanged: actor playback does not duplicate damage. Enemy reactions/health/death remain independent; this is not shared-victim gameplay.','Failed617 stream-setup and619 optional-branch assertions retained, with checkpoint/health/inventory and14 protected hashes restored.','Complete movement and playable co-op remain unfinished. No new pipeline or third-party co-op research.']
data=dict(at=at,candidate=A.name,dll=pin['dll'],probe=pin['probe'],protocol=66,bankVersion=65,clips=353,runtimeVerified=True,optionalHitRecoveryVerified=False,tests=tests,complete=False,visualAccepted=False,limitations=limitations)
wr(receipt,data);wr(A/'runtime-verification-v622.json',data)
videos=rd(N/'runtime-video-manifest.json')
for role,version in [('corvo',621),('emily',621),('corvo',619)]:
 take=f'crouch-combat-{role}-v{version}';f=N/take;r=rd(f/'result.json')
 for view in ('source','observer'):
  video=f/(('host' if role=='corvo' else 'client')+'-crouched-slash.mp4' if view=='source' else 'observer.mp4');rel=video.relative_to(N).as_posix()
  assert video.exists() and not any(v['video']==rel for v in videos)
  inspected=view=='observer' or version==619;sheet=f'{view}-sheet.jpg';assert not inspected or (f/sheet).exists()
  note=('Observer sheet shows a whole-body crouched strike and return to standing. Guard occludes detailed sword contact.' if view=='observer' else 'Native source recording. No separate visual acceptance.')
  if not r['passed']:note+=' Retained failure: '+r['failure']+'.'
  review=f/f'{view}-visual-review-v622.json';wr(review,dict(at=at,inspected=inspected,bodyVisuallyObserved=view=='observer',visualAccepted=False,complete=False,sheets=[sheet] if inspected else [],observations=[note]))
  m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
  videos.append(dict(video=rel,kind='game',character=role,label=f'{role.title()} · crouched targeted strike · {version} {view}'+(' · retained failed assertion' if not r['passed'] else ''),alias='crouched_attack',requirements=['crouched_attack'],gameplay_bound=r['passed'],complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=take+'/'+review.name,proxy_sha256=r['dll'],video_sha256=sha(video),actual_capture_hz=int(m['nb_frames'])/float(m['duration']),checks_passed=r['passed'],scope=r['scope']+' '+note))
wr(N/'runtime-video-manifest.json',videos)
latest='620/622: Corvo621 and Emily621 each pass10 targeted crouched strike, native Steam actor playback, root, native stance recovery and restoration checks. Both observer sheets show body attack/recovery; fine polish deferred. Optional hit-recovery15/16 was absent in621 and remains runtime-unverified. Protocol66, unchanged bank65/353. No duplicate receiver victim damage; shared enemy outcomes and complete movement remain unfinished.'
p=R/'work/current-task-state.json';s=rd(p);s['UpdatedAt']=at;s['CrouchCombatContinuation']=dict(at=at,candidate=A.name,dll=pin['dll'],probe=pin['probe'],receipt=str(receipt.relative_to(R)),latest=latest,runtimeVerified=True,optionalHitRecoveryVerified=False,complete=False,next='Bind the observed native throwhit/kick800 using the existing full-body donor and pipeline; retain exact scene/ownership guards. Continue missing movement and shared-world co-op. No third-party co-op research or cosmetic detours.');s['MovementExecutionPriority']['current']=latest;wr(p,s)
p=N/'review-evidence-annotations.json';a=rd(p);item=a['items']['crouched_attack'];item.update(runtime_binding='partial_native_steam_target_bearing_crouched_slash',complete=False,visual_accepted=False,visual_validation='not_accepted',notes=item.get('notes','')+' '+latest+' '+' '.join(limitations));wr(p,a)
p=R/'documentation/handoff-2026-10-11/44-crouched-combat-hit-recovery.md';t=p.read_text();t=t.replace('Build and runtime verification are pending.','At the initial616 preparation, build and runtime verification were pending; later results are recorded below.');t+='\n## Verified620/622 scope\n\n[Receipt622](../../work/movement-integration/crouch-stance-verification-v622.json) records Corvo621 and Emily621, each PASS10. The native target-bearing crouched strike drives the existing complete-body clip through Steam, retains independent root movement, returns to native stance locomotion and leaves the receiver enemy health unchanged. Both checkpoints, inventories, player/partner health and all14 protected save/settings hashes restore. The two observer sheets show crouched strikes and return to standing; foreground guards limit contact/grip assessment. Fine polish is deferred, with no visual acceptance.\n\nNeither621 take selects separate recovery15/16. That optional branch is explicitly **not runtime verified**, despite source613 observations and synthetic tests. Corvo621 records one validated same-scene standing sample; Emily621 stands after the owned scene ends. Failed617/619 receipts stay intact. Six source/observer videos are registered by [record622](../../work/movement-integration/record_crouch_stance_v622.py), including retained619 failure.\n';p.write_text(t)
print(json.dumps(dict(candidate=A.name,recordings=len(videos),checks=20,optionalHitRecoveryVerified=False,complete=False)))
