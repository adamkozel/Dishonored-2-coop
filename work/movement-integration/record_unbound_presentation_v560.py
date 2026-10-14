from pathlib import Path
import collections,datetime,hashlib,json,subprocess
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration';D=R/'documentation/handoff-2026-10-11'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert read(W/'native-operation-owner.json')['state']=='finished'
candidate=W/'candidate-unbound-presentation-v558';identity=read(candidate/'identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==identity['dll']
assert sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==identity['probe']
take=N/'unbound-presentation-v559c';result=read(take/'result.json');rows=read(take/'states.json')
assert result['passed'] and len(result['checks'])==16 and result['dll']==identity['dll']
at=datetime.datetime.now().astimezone().isoformat()
fallback=[x for x in rows if x['stream']['remoteState']==18 and x['movement']['weight']>.9]
groups=collections.Counter(x['source']+'/'+x['trial'] for x in fallback)
for role in ['host','client']:
 for stance in [0,1]:assert groups[role+'/holster-'+str(stance)]>0
receipt=dict(at=at,candidate=candidate.name,dll=identity['dll'],probe=identity['probe'],protocol=59,bankVersion=59,clips=336,
 test='work/natural-moveset/unbound-presentation-v559c/result.json',checks=result['checks'],fallbackRows=dict(groups),
 scope='Both-role standing/crouching missing-equipment-binding fault injection, explicit Steam state18, body continuity, four holstered directions and normal authored draw recovery. Native first-person and protected saves preserved; checkpoints restored.',
 limitations=['Fallback is locomotion continuity, not a completed action animation.','Only independently recognized native default ground motion is eligible; unknown physical states still need binding.','Continuous motion/contact quality and final visual acceptance remain open.','509 airborne landing and540 initial freshness issues remain separate.'],runtimeVerified=True,visualAccepted=False,complete=False)
write(candidate/'runtime-verification-v560.json',receipt);write(W/'unbound-presentation-verification-v560.json',receipt)
videos=read(N/'runtime-video-manifest.json')
for name,role,sheet in [('unbound-presentation-v559c','host','corvo-sheet.jpg'),('unbound-presentation-v559c','client','emily-sheet.jpg'),('unbound-presentation-v559b','host',None)]:
 folder=N/name;r=read(folder/'result.json');video=N/(name+'-'+role+'.mp4');assert video.is_file()
 assert not any(v['video']==video.name for v in videos),video
 character='corvo' if role=='host' else 'emily'
 review=dict(at=at,inspected=bool(sheet),sheets=[sheet] if sheet else [],visualAccepted=False,complete=False,
  observations=['Viewed actual temporal sheet: complete body remains visible through standing/crouching equipment transitions and directional movement; existing locomotion and draw poses recover. Missing holster gesture is deliberately replaced by locomotion. Fine hand/contact/cadence quality deferred.'] if sheet else ['Failed assertion take retained and indexed; its video has not been visually inspected.'],
  method='Viewed temporal sheet; continuous animation quality not certified.' if sheet else 'Unreviewed')
 if sheet:assert (folder/sheet).is_file()
 reviewPath=folder/(character+'-visual-review.json');write(reviewPath,review)
 meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
 videos.append(dict(video=video.name,character=character,label=character.title()+' · Missing-binding movement continuity'+(' · corrected test' if r['passed'] else ' · failed assertion retained'),alias='unbound_ground_continuity',requirements=['sword_draw_holster','walk_8_directions','crouch_8_directions'],gameplay_bound=True,complete=False,visual_accepted=False,evidence=name+'/result.json',visual_review=name+'/'+reviewPath.name,proxy_sha256=identity['dll'],video_sha256=sha(video),actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),checks_passed=r['passed'],scope=r['scope']))
write(N/'runtime-video-manifest.json',videos)
state=read(R/'work/current-task-state.json');state['UpdatedAt']=at
state['UnboundPresentationContinuation']=dict(at=at,latest='558/559c: explicit unbound ground presentation passes16 both-role standing/crouching fault/recovery checks, retains body weight and native equipment, and restores checkpoints/protected saves. Existing bank unchanged. Protocol59. Fallback is not animation completion; visuals remain unaccepted.',receipt='work/movement-integration/unbound-presentation-verification-v560.json',candidate=candidate.name,complete=False,next='Continue existing pipeline with broad functional binding of authored remaining item/interaction/traversal motions. Defer cosmetic polish and third-party co-op research. Keep native Steam/UI and lifecycle guards. Do not treat fallback as completed animation.')
state['MovementExecutionPriority']['current']=state['UnboundPresentationContinuation']['latest']
write(R/'work/current-task-state.json',state)
p=D/'38-unbound-movement-continuity.md';s=p.read_text(encoding='utf-8')
s+='\n## Completed runtime result559c\n\n[559c](../../work/natural-moveset/unbound-presentation-v559c/result.json) passes all16 checks on candidate558. Both-role standing/crouching missing-binding cases preserve full body weight and Steam readiness; normal authored draws resume and four holstered directions remain functional. Both checkpoints and all14 protected save/settings hashes restore. Actual Corvo/Emily temporal sheets were viewed and their two recordings are indexed in the dashboard. The failed559b recording is also retained with a failed/unreviewed label. Continuous visual quality is not certified. [Consolidated receipt560](../../work/movement-integration/unbound-presentation-verification-v560.json).\n'
s+='\nNext: continue broad bindings with existing authored clips. Cosmetic polish remains deferred. No new pipeline, asset generation or third-party code was introduced by this change.\n'
p.write_text(s,encoding='utf-8')
print(json.dumps(dict(candidate=candidate.name,checks=16,recordings=len(videos),fallbackRows=dict(groups),complete=False)))
