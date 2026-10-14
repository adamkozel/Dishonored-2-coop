from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert rd(W/'native-operation-owner.json')['state']=='finished'
A=W/'candidate-lethal-v604';pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll'] and sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==pin['probe']
at=datetime.datetime.now().astimezone().isoformat();videos=rd(N/'runtime-video-manifest.json');tests=[]
for role in ('corvo','emily'):
 for native in (True,False):
  name=f'native-lethal-{role}-v596' if native else f'lethal-network-{role}-v605';F=N/name;r=rd(F/'result.json');assert r['passed'] and len(r['checks'])==(4 if native else 10)
  if not native:
   assert r['dll']==pin['dll'];tests.append(dict(role=role,result=str((F/'result.json').relative_to(R)),checks=r['checks']))
  for view in (('source',) if native else ('source','observer')):
   video=F/(('host' if role=='corvo' else 'client')+'-rear-lethal.mp4' if view=='source' else 'observer.mp4');rel=video.relative_to(N).as_posix();assert not any(v['video']==rel for v in videos)
   inspected=(not native and view=='observer') or (native and role=='corvo');sheet='observer-sheet.jpg' if view=='observer' else 'source-sheet.jpg'
   if inspected:assert (F/sheet).is_file()
   note='Actual observer temporal sheet inspected: native third-person actor donor plays the rear sword action, whole-body movement and standing recovery. Sword remains visible. Receiver guard stays standing and independent; victim alignment/animation, blood and shared death/corpse outcome are unfinished. Visual quality is not accepted.' if view=='observer' else ('Actual Corvo source sheet inspected: ordinary native rear sword takedown plays in first person. This is source evidence, not remote victim synchronization.' if inspected else 'Native source recording retained with independent scene evidence; no separate visual acceptance.')
   rp=F/f'{view}-visual-review.json';wr(rp,dict(at=at,inspected=inspected,bodyVisuallyObserved=view=='observer',visualAccepted=False,complete=False,sheets=[sheet] if inspected else [],observations=[note]))
   m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
   videos.append(dict(video=rel,character=role,label=role.title()+' · rear assassination · '+('native ' if native else 'Steam actor ')+view,alias='lethal_takedown_front_back_sides',requirements=['lethal_takedown_front_back_sides'],gameplay_bound=not native,complete=False,visual_accepted=False,evidence=name+'/result.json',visual_review=name+'/'+rp.name,proxy_sha256=r['dll'],video_sha256=sha(video),actual_capture_hz=int(m['nb_frames'])/float(m['duration']),checks_passed=True,scope=r['scope']+' '+note))
limits=['Exact fast-back ordinary-guard actor presentation only; front/side/air/slide, different victim sizes, interruptions and campaign coverage remain incomplete.','Receiver target health remained40 before/after each605 test, so cosmetic body playback did not duplicate damage. Shared victim death/corpse/animation and world outcomes are still separate unfinished work.','Both native sword equipped and started were independently observed during state13; source first-person and inputs unchanged.','Actual observer body sheets viewed for both roles; contact/grip/footwork polish deferred, no visual acceptance.','Existing352 clip payloads unchanged; earlier592 breakout/finish receipts retain their own build attribution.']
receipt=dict(at=at,candidate=A.name,dll=pin['dll'],probe=pin['probe'],protocol=65,bankVersion=65,clips=353,runtimeVerified=True,tests=tests,complete=False,visualAccepted=False,limitations=limits)
wr(W/'lethal-verification-v606.json',receipt);wr(A/'runtime-verification-v606.json',receipt);wr(N/'runtime-video-manifest.json',videos)
latest='604/605: Corvo and Emily each pass10 native/Steam fast-back lethal actor checks. Exact state13/standing0 and owned Npc1333 scene drive existing native third-person donor; native sword equipped/started confirmed, root and idle recovery preserved, receiver victim health40 unchanged. Body sequence observed for both roles. Shared victim alignment/animation/death/corpse and world outcomes remain unimplemented; visual polish deferred.'
p=R/'work/current-task-state.json';s=rd(p);s['UpdatedAt']=at;s['LethalContinuation']=dict(at=at,latest=latest,candidate=A.name,receipt='work/movement-integration/lethal-verification-v606.json',complete=False,next='Observe frontal native assassination607, then extend proven missing action contexts using the established pipeline. No third-party co-op research or cosmetic detours.');s['MovementExecutionPriority']['current']=latest;wr(p,s)
p=N/'review-evidence-annotations.json';a=rd(p);a['items']['lethal_takedown_front_back_sides'].update(runtime_binding='partial_native_steam_fast_back_lethal_actor',complete=False,visual_accepted=False,visual_validation='not_accepted',notes=latest+' '+' '.join(limits));wr(p,a)
p=W/'refresh_grenade_handoff_v58.py';s=p.read_text();mark="m['LatestContinuation']['probeCandidate']=probeArchive.name";assert mark in s and "lethal=state.get('LethalContinuation'" not in s
s=s.replace(mark,"lethal=state.get('LethalContinuation',{})\nif lethal:\n    m['LatestContinuation']['next']=lethal.get('next',m['LatestContinuation']['next'])\n    m['LatestContinuation']['in_progress']+=' '+lethal.get('latest','')\n"+mark);p.write_text(s)
print(json.dumps(dict(candidate=A.name,recordings=len(videos),checks=20,complete=False)))
