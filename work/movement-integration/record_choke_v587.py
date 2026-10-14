from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert rd(W/'native-operation-owner.json')['state']=='finished'
A=W/'candidate-choke-network-v584';pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll'];assert sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==pin['probe']
at=datetime.datetime.now().astimezone().isoformat();tests=[]
for role,num in (('corvo',585),('emily',586)):
 p=N/f'choke-network-{role}-v{num}';r=rd(p/'result.json');assert r['passed'] and len(r['checks'])>=12 and r['dll']==pin['dll'];assert (p/'observer-sheet.jpg').is_file();tests.append(dict(role=role,result=str((p/'result.json').relative_to(R)),checks=r['checks']))
limits=['Actor presentation only: receiver victim remains native/independent; no shared struggle, knockout/death/corpse outcome or native choke gameplay events added.','Fine hand/contact/footwork polish deferred and visuals not accepted.','Native cancel, interruptions, crouched entries, target dragging, front/side/aerial/slide takedowns and wider character contacts remain unverified.','Previous Corvo trap576 intermittent250ms semantic freshness fade remains open; unchanged578 passed.']
receipt=dict(at=at,candidate=A.name,dll=pin['dll'],probe=pin['probe'],protocol=63,bankVersion=63,clips=351,tests=tests,complete=False,visualAccepted=False,runtimeVerified=True,limitations=limits)
wr(W/'choke-verification-v587.json',receipt);wr(A/'runtime-verification-v587.json',receipt)
videos=rd(N/'runtime-video-manifest.json')
cases=[('corvo','native-guard-choke-corvo-v579',True),('emily','native-guard-choke-emily-v580',True),('corvo','choke-network-corvo-v583',False),('corvo','choke-network-corvo-v585',False),('emily','choke-network-emily-v586',False)]
for role,name,native in cases:
 folder=N/name;r=rd(folder/'result.json');source='host' if role=='corvo' else 'client'
 for view in (('source',) if native else ('source','observer')):
  video=folder/(f'{source}-rear-choke.mp4' if view=='source' else 'observer.mp4');rel=video.relative_to(N).as_posix();assert not any(v['video']==rel for v in videos)
  inspected=view=='observer' or name.endswith('v580');sheet='observer-sheet.jpg' if view=='observer' else 'source-sheet.jpg'
  if inspected:assert (folder/sheet).is_file()
  observation='Actual observer temporal sheet viewed: remote actor entry, held struggle, finish and standing recovery visible. Native receiver guard is not synchronized and remains standing; contact and visual quality are not accepted.'
  if name.endswith('v583'):observation='Failed583: observer fell to floor below; remote actor is outside this recording view. Trace verifies initial phase only, then stance12 was refused. This is not visual coverage.'
  if name.endswith('v580'):observation='Native attempt failed: guard walks out of reach and Emily parries. No choke coverage.'
  review=dict(at=at,inspected=inspected,visualAccepted=False,complete=False,sheets=[sheet] if inspected else [],observations=[observation] if inspected else ['Source recording retained alongside independent native reader and result; no visual acceptance.'])
  rp=folder/f'{view}-visual-review.json';wr(rp,review)
  m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
  scope=r['scope'] if r['passed'] else 'FAILED: '+r['failure']+'. '+observation
  videos.append(dict(video=rel,character=role,label=role.title()+' · rear choke · '+('native ' if native else 'Steam actor ')+view+(' · FAILED' if not r['passed'] else ''),alias='nonlethal_takedown_front_back_sides',requirements=['nonlethal_takedown_front_back_sides'],gameplay_bound=not native and r['passed'],complete=False,visual_accepted=False,evidence=name+'/result.json',visual_review=name+'/'+rp.name,proxy_sha256=r['dll'],video_sha256=sha(video),actual_capture_hz=int(m['nb_frames'])/float(m['duration']),checks_passed=r['passed'],scope=scope))
wr(N/'runtime-video-manifest.json',videos)
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
latest='584/585/586: Corvo and Emily each pass12 source-owned rear-choke and native Steam actor-body/root/recovery/restoration checks. Native entry600, struggle1400 and finish1800 drive three existing authored clips (bank63/351, prior348 payloads unchanged). Failed583 missed native physical stance12; corrected only for exact Choke41 and owned scenes. Failed580 walking-target input and583 low observer camera retained. Both actual body sheets viewed, visuals unaccepted. Receiver victim animation/alignment and shared knockout outcomes remain unimplemented.'
state['ChokeContinuation']=dict(at=at,latest=latest,candidate=A.name,receipt='work/movement-integration/choke-verification-v587.json',complete=False,next='Continue broad current-pipeline coverage: native rear-choke cancel/interruption, then remaining interactions/traversal. Shared victim/world outcomes remain required. Defer cosmetic polish and third-party co-op research.')
state['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',state)
ann=rd(N/'review-evidence-annotations.json');item=ann['items']['nonlethal_takedown_front_back_sides'];item['notes']=latest+' '+' '.join(limits);item['runtime_binding']='partial_native_steam_rear_choke_actor';item['complete']=False;item['visual_accepted']=False;wr(N/'review-evidence-annotations.json',ann)
p=W/'refresh_grenade_handoff_v58.py';s=p.read_text();mark="m['LatestContinuation']['probeCandidate']=probeArchive.name";assert mark in s
s=s.replace(mark,"choke=state.get('ChokeContinuation',{})\nif choke:\n    m['LatestContinuation']['next']=choke.get('next',m['LatestContinuation']['next'])\n    m['LatestContinuation']['in_progress']+=' '+choke.get('latest','')\n"+mark);p.write_text(s)
print(json.dumps(dict(candidate=A.name,checks=sum(len(t['checks']) for t in tests),recordings=len(videos),complete=False)))
