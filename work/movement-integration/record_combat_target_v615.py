from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();A=W/'candidate-combat-target-v610';pin=rd(A/'identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll'] and sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==pin['probe']
receipt=W/'combat-target-verification-v615.json';assert not receipt.exists()
tests=[]
for role,take in [('corvo','targeted-combat-corvo-v611'),('emily','targeted-combat-emily-v614')]:
 r=rd(N/take/'result.json');assert r['passed'] and len(r['checks'])==10 and r['dll']==pin['dll']
 tests.append(dict(role=role,result=str((N/take/'result.json').relative_to(R)),resultSha256=sha(N/take/'result.json'),checks=r['checks']))
limitations=['Ordinary standing right/rightb target-bearing sword actor playback only. Native local first-person remains unchanged; no victim events or damage replay.','Corvo611 passed numerical checks but foreground guard occludes the body. Emily614 observer sheet inspected and shows the body attack and recovery. Fine contact/grip polish deferred; no visual acceptance.','Emily611 retained failure: crouched native throwhit/kick800 instead of expected sword hit. Emily612 retained failure: one independently validated target scene, below the harness minimum of two; remote playback observed but no passing receipt.','Corvo614 retained fixture failure: source could not stand near geometry and observer capture timed out during early cleanup. Both checkpoints/inventories/health and14 protected hashes restored.','Bank65/353 and protocol65 unchanged. Targeted crouch/left/other contexts, shared enemy reaction/health/death and full campaign coverage remain unfinished.']
data=dict(at=at,candidate=A.name,dll=pin['dll'],probe=pin['probe'],protocol=65,bankVersion=65,clips=353,runtimeVerified=True,tests=tests,complete=False,visualAccepted=False,limitations=limitations)
wr(receipt,data);wr(A/'runtime-verification-v615.json',data)
videos=rd(N/'runtime-video-manifest.json')
cases=[('native-front-corvo-v607','corvo','front-lethal',['source']),('native-front-emily-v607','emily','front-lethal',['source']),('targeted-combat-corvo-v611','corvo','targeted-slash',['source','observer']),('targeted-combat-emily-v611','emily','targeted-slash',['source']),('targeted-combat-emily-v612','emily','targeted-slash',['source','observer']),('targeted-combat-emily-v614','emily','targeted-slash',['source','observer'])]
for take,role,key,views in cases:
 f=N/take;r=rd(f/'result.json');native=take.startswith('native-front')
 for view in views:
  video=f/(('host' if role=='corvo' else 'client')+'-'+key+'.mp4' if view=='source' else 'observer.mp4');rel=video.relative_to(N).as_posix()
  assert video.exists() and not any(v['video']==rel for v in videos)
  inspected=(take,view) in [('native-front-corvo-v607','source'),('targeted-combat-corvo-v611','observer'),('targeted-combat-emily-v611','source'),('targeted-combat-emily-v612','observer'),('targeted-combat-emily-v614','observer')]
  sheet=f'{view}-sheet.jpg';assert not inspected or (f/sheet).exists()
  body=inspected and view=='observer' and role=='emily'
  note=('Actual observer sheet shows Emily body attack and standing recovery. Victim reactions/outcomes remain independent; fine polish deferred.' if body else 'Foreground guard obscures Corvo body; numerical pass does not establish complete visual coverage.' if view=='observer' else 'Native source evidence retained. No separate remote visual acceptance.')
  if not r['passed']:note+=' Retained failed assertion: '+r['failure']+'.'
  review=f/f'{view}-visual-review.json';wr(review,dict(at=at,inspected=inspected,bodyVisuallyObserved=body,visualAccepted=False,complete=False,sheets=[sheet] if inspected else [],observations=[note]))
  m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
  videos.append(dict(video=rel,kind='game',character=role,label=role.title()+' · targeted sword · '+take.rsplit('-',1)[-1]+' '+view+(' · retained failed assertion' if not r['passed'] else ''),alias='light_attack_variants',requirements=['light_attack_variants'],gameplay_bound=not native and r['passed'],complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=take+'/'+review.name,proxy_sha256=r['dll'],video_sha256=sha(video),actual_capture_hz=int(m['nb_frames'])/float(m['duration']),checks_passed=r['passed'],scope=r['scope']+' '+note))
wr(N/'runtime-video-manifest.json',videos)
latest='610/615: native target-bearing sword scenes now drive existing remote combat clips. Corvo611 and Emily614 each pass10 source/Steam/recovery/restoration checks; receiver target health unchanged. Existing protocol65 and bank65/353 reused. Emily body attack/recovery visually observed; Corvo611 camera is occluded. Shared NPC reactions/outcomes and other combat contexts remain unfinished; cosmetic polish deferred.'
p=R/'work/current-task-state.json';s=rd(p);s['UpdatedAt']=at;s['CombatTargetContinuation']=dict(at=at,latest=latest,candidate=A.name,receipt='work/movement-integration/combat-target-verification-v615.json',complete=False,next='Observe native crouched and close-range combat613, then bind proven missing actions through the existing library. Continue toward shared world authority and complete playable co-op. No new tool pipeline or third-party co-op research.');s['MovementExecutionPriority']['current']=latest;wr(p,s)
p=N/'review-evidence-annotations.json';a=rd(p);item=a['items']['light_attack_variants'];item.update(runtime_binding='partial_native_steam_target_bearing_standing_slash',complete=False,visual_accepted=False,visual_validation='not_accepted',notes=item.get('notes','')+' '+latest+' '+' '.join(limitations));wr(p,a)
print(json.dumps(dict(candidate=A.name,recordings=len(videos),checks=20,complete=False)))
