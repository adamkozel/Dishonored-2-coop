from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert rd(W/'native-operation-owner.json')['state']=='finished'
A=W/'candidate-choke-breakout-v592';pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll'];assert sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==pin['probe']
at=datetime.datetime.now().astimezone().isoformat();tests=[];videos=rd(N/'runtime-video-manifest.json')
cases=[('corvo','choke-breakout-corvo-v593'),('emily','choke-breakout-emily-v595'),('corvo','choke-breakout-corvo-v597'),('corvo','choke-finish-corvo-v598'),('emily','choke-finish-emily-v598')]
for role,name in cases:
 F=N/name;r=rd(F/'result.json');assert r['passed'] and len(r['checks'])==12 and r['dll']==pin['dll'];review=rd(F/'observer-visual-review.json');assert review['inspected'] and not review['visualAccepted'];tests.append(dict(role=role,result=str((F/'result.json').relative_to(R)),checks=r['checks'],visualReview=str((F/'observer-visual-review.json').relative_to(R))))
 for view in ('source','observer'):
  video=F/(('host' if role=='corvo' else 'client')+'-rear-choke.mp4' if view=='source' else 'observer.mp4');rel=video.relative_to(N).as_posix();assert not any(v['video']==rel for v in videos)
  if view=='source':wr(F/'source-visual-review.json',dict(at=at,inspected=False,visualAccepted=False,complete=False,observations=['Native source recording retained with independent scene/ownership checks. No separate visual acceptance.']))
  m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
  videos.append(dict(video=rel,character=role,label=role.title()+' · '+('choke breakout' if 'breakout' in name else 'choke finish regression')+' · '+view,alias='struggle_and_escape' if 'breakout' in name else 'nonlethal_takedown_front_back_sides',requirements=['struggle_and_escape','nonlethal_takedown_front_back_sides'] if 'breakout' in name else ['nonlethal_takedown_front_back_sides'],gameplay_bound=True,complete=False,visual_accepted=False,evidence=name+'/result.json',visual_review=name+f'/{view}-visual-review.json',proxy_sha256=pin['dll'],video_sha256=sha(video),actual_capture_hz=int(m['nb_frames'])/float(m['duration']),checks_passed=True,scope=r['scope']+' '+(' '.join(review['observations']) if view=='observer' else '')))
limits=['Actor-body presentation only; receiver guard animation/placement, knockout/death, corpse and world outcomes remain unsynchronized by this change.','Native cancel400 still unverified; combat_lose1500 is a separate branch.','Corvo593 guard occludes most actor; Corvo597 and Emily595 show actual breakout body. Corvo finish598 camera clips geometry and is numerical regression only; unchanged prior clips retain earlier scoped recordings.','Visual/contact/grip polish deferred; no visual acceptance or completed moveset claim.','Earlier trap576 freshness fade and other archived intermittent cases remain unresolved.']
receipt=dict(at=at,candidate=A.name,dll=pin['dll'],probe=pin['probe'],protocol=64,bankVersion=64,clips=352,tests=tests,complete=False,visualAccepted=False,runtimeVerified=True,limitations=limits)
wr(W/'choke-breakout-verification-v601.json',receipt);wr(A/'runtime-verification-v601.json',receipt);wr(N/'runtime-video-manifest.json',videos)
latest='Candidate592: distinct native combat-lose1500 now drives the remote full body through Steam, with unchanged source first-person and independent network root. Corvo593/597 and Emily595 pass12 checks each; Corvo/Emily598 finish regression each passes12. Breakout body observed on both roles; no visual acceptance. All previous351 clip payloads preserved in bank64/352. Receiver victim alignment/animation and shared outcomes remain unimplemented.'
p=R/'work/current-task-state.json';s=rd(p);s['UpdatedAt']=at;s['ChokeContinuation'].update(at=at,latest=latest,candidate=A.name,receipt='work/movement-integration/choke-breakout-verification-v601.json',complete=False,next='Continue native lethal and other missing functional bindings using existing authored pipeline; no cosmetic-polish or third-party co-op detours.');s['MovementExecutionPriority']['current']=latest;s['MovementExecutionPriority']['coverageReconciliation']='work/movement-integration/coverage-reconciliation-v600.json';wr(p,s)
p=N/'review-evidence-annotations.json';a=rd(p)
for id in ('struggle_and_escape','nonlethal_takedown_front_back_sides'):
 item=a['items'][id];item.update(runtime_binding='partial_native_steam_rear_choke_actor_and_breakout',complete=False,visual_accepted=False,visual_validation='not_accepted',notes=latest+' '+' '.join(limits))
wr(p,a);print(json.dumps(dict(candidate=A.name,checks=sum(len(t['checks']) for t in tests),recordings=len(videos),complete=False)))
