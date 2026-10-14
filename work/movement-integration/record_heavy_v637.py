from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb')as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();A=W/'candidate-heavy-v635';pin=rd(A/'identity.json');receipt=W/'crouched-heavy-verification-v637.json';assert not receipt.exists()
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll'];tests=[]
for role in ('corvo','emily'):
 f=N/f'crouched-heavy-{role}-v636';r=rd(f/'result.json');assert r['passed']and len(r['checks'])==10 and r['dll']==pin['dll']
 assert(f/'observer-sheet.jpg').is_file()
 tests.append(dict(role=role,result=str((f/'result.json').relative_to(R)),sha256=sha(f/'result.json'),checks=r['checks'],coverage=rd(f/'context-coverage.json')))
limits=['Stationary crouched charge/hold/release actor presentation only. Moving/cancel/interruption contexts and shared victim outcomes are not covered.','Both636 tests selected Scene_1/833. Scene_2/1000 has native source629, asset and unit-test evidence; corrected remote playback of that alternative was not exercised.','Both native release subscenes use the same existing adapted body donor. Direction-specific acting, static-hold breathing, contact and fine stance-transition polish are deferred.','Both observer sheets were inspected: full body windup/release/recovery visible. Coarse pose transitions remain; no final visual acceptance or complete movement claim.','Corvo634 failed release validation due to incorrect role-specific timing inference. Exact asset635 and corrected636 tests establish scene/duration pairs independent of role. Original failed receipt retained, with restoration verified.']
data=dict(at=at,candidate=A.name,dll=pin['dll'],probe=pin['probe'],protocol=69,bankVersion=67,clips=357,runtimeVerified=True,scope='Recorded stationary crouched heavy actor presentation on both roles',complete=False,visualAccepted=False,tests=tests,limitations=limits,retainedFailure='work/natural-moveset/crouched-heavy-corvo-v634/result.json')
wr(receipt,data);wr(A/'runtime-verification-v637.json',data)
videos=rd(N/'runtime-video-manifest.json')
for version,roles in [(634,['corvo']),(636,['corvo','emily'])]:
 for role in roles:
  take=f'crouched-heavy-{role}-v{version}';f=N/take;r=rd(f/'result.json')
  for view in ('source','observer'):
   video=f/(('host'if role=='corvo'else'client')+'-crouched-heavy.mp4'if view=='source'else'observer.mp4');rel=video.relative_to(N).as_posix();assert video.exists()and not any(v['video']==rel for v in videos)
   inspected=view=='observer';review=f/f'{view}-visual-review-v637.json'
   note='Actual body charge/hold and recovery visible; release missing on failed634.'if version==634 else'Full body windup, hold, release and crouched recovery visible; coarse transitions and detailed contact/grip deferred.'
   wr(review,dict(at=at,inspected=inspected,visualAccepted=False,complete=False,sheets=['observer-sheet.jpg']if inspected else[],observations=[note if inspected else'Native first-person source recording; no separate visual acceptance.']))
   m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
   videos.append(dict(video=rel,kind='game',character=role,label=f'{role.title()} · crouched heavy · {version} {view}'+(' · retained failed release'if version==634 else''),alias='heavy_attack_variants',requirements=['heavy_attack_variants','crouched_attack'],gameplay_bound=r['passed'],complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=take+'/'+review.name,proxy_sha256=r['dll'],video_sha256=sha(video),actual_capture_hz=int(m['nb_frames'])/float(m['duration']),checks_passed=r['passed'],scope=r['scope']+' '+note))
wr(N/'runtime-video-manifest.json',videos)
latest='635/636/637: both roles PASS10 stationary crouched heavy charge/hold/release through native Steam, full-body adapted clips354/355/356, root independence and native stance recovery. Protocol69,bank67/357; prior354 unchanged. Both inventories/health14protected hashes restored. Exact asset635 corrects role-specific release inference: Scene_1/833 and Scene_2/1000 are attack variants. Coarse pose transitions/contact polish deferred. Full movement and shared victim/world outcomes remain unfinished.'
p=R/'work/current-task-state.json';s=rd(p);s['UpdatedAt']=at;s['HeavyCombatContinuation']=dict(at=at,latest=latest,candidate=A.name,receipt=str(receipt.relative_to(R)),runtimeVerified=True,complete=False,restorationVerified=True,next='Continue broad missing movement/gameplay integration through the current pipeline. Reuse existing body clips, native state readers and tested fixtures; batch compatible cases before one restoration where safe. Defer fine poses and random kick retries; no third-party co-op mod research. Prioritize remaining functional coverage and progression toward shared-world playable co-op.')
s['MovementExecutionPriority']['current']=latest;wr(p,s)
p=N/'review-evidence-annotations.json';a=rd(p)
for key in ('heavy_attack_variants','crouched_attack'):
 item=a['items'][key];item['runtime_binding']='partial_native_steam_standing_and_crouched_combat';item['notes']+=' '+latest;item['complete']=False
 for t in tests:
  name=str(Path(t['result']).relative_to('work/natural-moveset')).replace('\\','/')
  if name not in item.setdefault('evidence',[]):item['evidence'].append(name)
wr(p,a)
p=D/'46-native-crouched-heavy-attack.md';p.write_text(p.read_text()+'\n\n## Corrected runtime verification637\n\n[Receipt637](../../work/movement-integration/crouched-heavy-verification-v637.json) records both-role636 PASS10 on candidate635. Ordinary held input drives native charge entry/hold and owned release18 through Steam, preserving network root, sword and stance recovery. Both inventories, player/partner health and14 protected files are restored. Source/observer recordings are indexed in the dashboard, and both observer sheets were inspected. Prior354 bank payloads remain unchanged.\n\n'+ '\n\n'.join(limits)+'\n')
print(json.dumps(dict(recordings=len(videos),checks=20,candidate=A.name,stationaryCrouchedHeavyVerified=True,completeMovement=False)))
