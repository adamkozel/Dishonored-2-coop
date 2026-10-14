from pathlib import Path
import json,hashlib,datetime,subprocess,shutil
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert rd(W/'native-operation-owner.json')['state']=='finished'
A=W/'candidate-trap-network-v575';identity=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==identity['dll'];assert sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==identity['probe']
at=datetime.datetime.now().astimezone().isoformat();tests=[]
for role,kind,take in (('corvo','razor',578),('emily','stun',576)):
 p=N/f'trap-network-{role}-{kind}-v{take}';r=rd(p/'result.json');assert r['passed'] and len(r['checks'])>=15 and r['dll']==identity['dll'];assert (p/'observer-sheet.jpg').is_file();tests.append(dict(role=role,trap=kind,path=str((p/'result.json').relative_to(R)),checks=r['checks']))
limitations=['Visual polish remains unaccepted: fine grip, precise surface reach/contact, weapon clearance and continuous quality.','Native source places one actual trap; remote body/held prop never spawns a trap or runs a native placement/damage event. Shared placed-trap worlds/outcomes remain separate.','Additional stocked reload contexts, ready-out cancellation, slide, airborne, upgraded items and both gadget kinds on each role still need coverage.','Holstering is inferred only with a unique acquired trap when neither is selected; ambiguous inventories fall back instead of guessing.','Complete moveset and playable co-op remain unfinished; prioritize broad existing-pipeline bindings and defer polish.']
limitations.append('Corvo576 failed its ready-in body-weight check: phase3 arrived, but a250ms semantic freshness gap briefly faded overall body weight. Unchanged Corvo578 passed all15 checks. This intermittent continuity issue remains unresolved; no timing guards or assertions were relaxed.')
receipt=dict(at=at,candidate=A.name,dll=identity['dll'],probe=identity['probe'],protocol=62,bankVersion=62,clips=348,tests=tests,runtimeVerified=True,visualAccepted=False,complete=False,limitations=limitations,unresolvedEvidence='work/natural-moveset/trap-network-corvo-razor-v576/result.json')
wr(W/'trap-verification-v577.json',receipt);wr(A/'runtime-verification-v577.json',receipt)
shutil.copy2(R/'outputs/d2-coop-lab/research/trap-prop-presentation-asset-install.json',A/'trap-asset-install.json')
videos=rd(N/'runtime-video-manifest.json')
cases=[('corvo','native-trap-corvo-razor-v568',True),('emily','native-trap-emily-stun-v568',True),('corvo','trap-network-corvo-razor-v570',False),('corvo','trap-network-corvo-razor-v572',False),('emily','trap-network-emily-stun-v572',False),('corvo','trap-network-corvo-razor-v576',False),('emily','trap-network-emily-stun-v576',False),('corvo','trap-network-corvo-razor-v578',False)]
for character,name,native in cases:
 folder=N/name;r=rd(folder/'result.json')
 for view in (('source',) if native else ('source','observer')):
  video=folder/f'{view}.mp4';rel=video.relative_to(N).as_posix();assert not any(v['video']==rel for v in videos)
  inspected=name.endswith(('v572','v576','v578')) and view=='observer'
  review=dict(at=at,inspected=inspected,visualAccepted=False,complete=False,sheets=['observer-sheet.jpg'] if inspected else [],observations=['Viewed actual D2 temporal sheet: complete native remote body presents held gadget/ready reach, returns through crouch and standing/holster. Static sheet cannot certify fine fingers, release/surface contact or continuous quality.'] if inspected else ['Native source or failed integration recording retained; no separate visual acceptance.'])
  rp=folder/f'{view}-visual-review.json';wr(rp,review)
  m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
  scope='Native trap acquisition and placement observation.' if native else (('FAILED570: shifted clock tuple prevented native action bindings. Held-idle prop worked. Fixed in571; this failed recording is not coverage.' if name.endswith('v570') else 'FAILED Emily572: native reload866 was not represented, and the test expected empty333. Other phases worked. Recovery was added in575; this failed take is not full gadget coverage.') if not r['passed'] else r['scope'])
  if name=='trap-network-corvo-razor-v576':scope='FAILED576: ready-in phase3 reached the remote body but overall blend weight dipped after a250ms semantic freshness gap. Unchanged578 passed. Intermittent continuity remains unresolved; this failed take is retained.'
  videos.append(dict(video=rel,character=character,label=character.title()+' · trap '+('native source' if native else ('Steam body and prop · ' if r['passed'] else 'FAILED integration · ')+view),alias='mine_or_razor_place',requirements=['mine_or_razor_place'],gameplay_bound=not native and r['passed'],complete=False,visual_accepted=False,evidence=name+'/result.json',visual_review=name+'/'+rp.name,proxy_sha256=r['dll'],video_sha256=sha(video),actual_capture_hz=int(m['nb_frames'])/float(m['duration']),checks_passed=r['passed'],scope=scope))
wr(N/'runtime-video-manifest.json',videos)
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['TrapContinuation']=dict(at=at,latest='575/576: Corvo spring razor and Emily stun mine each pass at least15 native Steam presentation checks. Shared observed equip500, holster500, ready-in200/loop666, place300 and empty333 or observed reload866 clocks drive existing authored bank62/348; selected idle and static native wrist props included. Stock consumption hides remote held gadget. Both checkpoints and14 protected hashes restored. Failed570 clock-offset interpretation corrected from probe definition; Emily572 observed native reload866 instead of expected empty333, now represented explicitly; both observer sheets viewed, fine polish deferred.',candidate=A.name,receipt='work/movement-integration/trap-verification-v577.json',complete=False,next='Continue existing-pipeline contextual takedown bindings: observe native guard rear choke entry/loop/completion using existing authored guard and existing full-body clips. Remaining trap reload/cancel/context breadth and shared placed-world outcomes remain tracked; do not resume third-party mod research or fine polish.')
state['TrapContinuation']['latest']=state['TrapContinuation']['latest'].replace('575/576:', '575 with Emily576 and Corvo578:')+' '+limitations[-1]
state['TrapWorkInProgress']['running']='Trap tests complete; scoped passing receipts recorded alongside failed576 freshness evidence. Native rear-choke observation is next.'
state['MovementExecutionPriority']['current']=state['TrapContinuation']['latest'];wr(R/'work/current-task-state.json',state)
ann=rd(N/'review-evidence-annotations.json');item=ann['items']['mine_or_razor_place'];item['notes']=state['TrapContinuation']['latest']+' '+' '.join(limitations);item['runtime_binding']='partial_native_steam_trap_body_and_held_prop';item['complete']=False;item['visual_accepted']=False;wr(N/'review-evidence-annotations.json',ann)
p=W/'refresh_grenade_handoff_v58.py';s=p.read_text();mark="m['LatestContinuation']['probeCandidate']=probeArchive.name";assert mark in s
s=s.replace(mark,"trap=state.get('TrapContinuation',{})\nif trap:\n    m['LatestContinuation']['next']=trap.get('next',m['LatestContinuation']['next'])\n    m['LatestContinuation']['in_progress']+=' '+trap.get('latest','')\n"+mark);p.write_text(s)
print(json.dumps(dict(candidate=A.name,checks=sum(len(t['checks']) for t in tests),recordings=len(videos),complete=False)))

