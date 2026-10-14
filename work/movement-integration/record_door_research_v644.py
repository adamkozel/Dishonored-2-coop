"""Record bounded ordinary-door evidence; never label unsynchronized doors complete."""
from pathlib import Path
import json,hashlib,datetime,subprocess,collections
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'; D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat(); pin=rd(W/'candidate-heavy-v635/identity.json')
receipt=W/'native-door-research-v644.json'; assert not receipt.exists()
tests=[]
for role,source,version in [('corvo','host',641),('emily','client',642)]:
 take=f'native-unlocked-door-{role}-v{version}';f=N/take;r=rd(f/'result.json')
 assert r['passed'] and r['dll']==pin['dll'] and len(r['checks'])==4
 rows=rd(f/f'{source}-unlocked-door-tap-observations.json');cycle=[]
 for row in rows:
  state=row['doorSource']['current']
  if not cycle or cycle[-1]!=state:cycle.append(state)
 assert cycle==[0,1,2,1,0],cycle
 assert all(x['doorPeer']['current']==0 and x['doorPeer']['locked']==0 and x['doorSource']['locked']==0 for x in rows)
 ids=sorted({x['id'] for row in rows for x in row['player']['active']});assert ids==[0,75,81]
 tests.append(dict(character=role,take=take,result=str((f/'result.json').relative_to(R)),sha256=sha(f/'result.json'),samples=len(rows),sourceDoorCycle=cycle,peerDoorStates=[0],nativePlayerStates=ids,worldSynchronized=False))
limits=['Only one ordinary unlocked Dunwall door, two short-X inputs per character. Source closed/open/closed is proven; receiver stays closed. No shared door implementation.','Corvo638 locked office attempt reports Royal Quarters Key needed. No successful key use, linked door, break/destruction or quest-script outcome coverage.','No dedicated first-person reach or player Midnight scene was observed in these sampled ordinary taps; retain normal locomotion and native equipment overlays here. Both roles briefly include Undraw81 near the second tap as well as UseInteraction75. Absence of a reach in these samples is not universal absence of interaction animation.','Both source and observer sheets were inspected. Source opening/closing is visible. Observer avatar is visible initially but becomes obscured/absent later while its own door stays closed. Cause not established; continuous visual presentation not accepted.','640 retained failure: PowerShell signed 0xffffffff mask included flags beside generation;641 uses explicit unsigned DWORD mask. Both inventories/health and14 protected hashes restored even on640.','No production DLL/bank changed. Native Openable routines are traced, not directly invoked. Propagating door outcomes requires native state/event/physics ownership, not transform-only playback.']
wr(receipt,dict(at=at,dll=pin['dll'],candidate='candidate-heavy-v635',tests=tests,limitations=limits,complete=False,visualAccepted=False,worldSynchronized=False,retainedFailure='work/natural-moveset/native-unlocked-door-corvo-v640/result.json'))
videos=rd(N/'runtime-video-manifest.json')
for take,role,source,kind in [('native-door-corvo-v638','corvo','host','locked attempt'),('native-unlocked-door-corvo-v640','corvo','host','retained harness failure'),('native-unlocked-door-corvo-v641','corvo','host','open-close source'),('native-unlocked-door-emily-v642','emily','client','open-close source')]:
 f=N/take;r=rd(f/'result.json')
 for view in ('source','observer'):
  name=f'{source}-'+('door-tap' if 'v638' in take else 'unlocked-door-tap')+'.mp4' if view=='source'else'observer.mp4'
  video=f/name;rel=video.relative_to(N).as_posix();assert video.exists() and not any(x['video']==rel for x in videos)
  inspected=(f/f'{view}-sheet.jpg').exists() and not (view=='observer' and ('v638'in take or 'v640'in take))
  review=f/f'{view}-visual-review-v644.json'
  wr(review,dict(at=at,inspected=inspected,visualAccepted=False,complete=False,sheets=[f'{view}-sheet.jpg']if inspected else[],observations=[kind,'Native door outcome is not shared. Observer avatar continuity not accepted; recorded mismatch retained.']))
  m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
  videos.append(dict(video=rel,kind='game',character=role,label=f'{role.title()} · ordinary door · {kind} · {view}',alias='door_open_close_locked',requirements=['door_open_close_locked'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=take+'/'+review.name,proxy_sha256=r['dll'],video_sha256=sha(video),actual_capture_hz=int(m['nb_frames'])/float(m['duration']),checks_passed=r['passed'],scope=r['scope']+' Native world door is not synchronized.'))
wr(N/'runtime-video-manifest.json',videos)
a=rd(N/'review-evidence-annotations.json');i=a['items']['door_open_close_locked'];i['runtime_binding']='native_source_open_close_both_roles_world_pending';i['complete']=False;i['notes']='644: ordinary Dunwall short-X taps produce source Openable0->1->2->1->0 for both roles, brief native UseInteraction75 without a separate observed player reach scene. Keep the existing body locomotion for this case; do not force the candidate door gestures. Other native interactions may differ. Peer door remains closed; world sync, key use, linked/scripted/broken variants and observer continuity are unfinished.';i.setdefault('evidence',[]).extend(t['take']+'/result.json'for t in tests);wr(N/'review-evidence-annotations.json',a)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['DoorContinuation']=dict(at=at,receipt=str(receipt.relative_to(R)),nativeSourceVerified=True,worldSynchronized=False,complete=False,next='Keep normal body locomotion for ordinary taps; shared native Openable outcomes remain a world task. Resume broader movement batch coverage; no forced bespoke reach and no fine visual polish.');wr(R/'work/current-task-state.json',s)
# Merge by address/category so older project annotations remain available.
p=R/'work/handoff-project-bookmarks-current.json';b=rd(p);new=rd(R/'work/handoff-project-bookmarks-v644.json');indexed={(x['address'],x['category']):x for x in b};indexed.update({(x['address'],x['category']):x for x in new});wr(p,sorted(indexed.values(),key=lambda x:(x['address'],x['category'])))
p=D/'README.md';text=p.read_text();link='[Native ordinary door observation and state trace](47-native-door-interactions.md)'
if link not in text:text+='\n48. '+link+'\n'
p.write_text(text)
print(json.dumps(dict(nativeSourceCases=len(tests),cycles=[t['sourceDoorCycle']for t in tests],sharedDoors=False,recordings=len(videos),bookmarks=len(indexed))))
