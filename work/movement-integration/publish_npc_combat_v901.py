from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration';A=W/'candidate-npc-combat-v896'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==sha(A/'dinput8.dll')==pin['dll']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
 if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
videos=rd(N/'runtime-video-manifest.json');cases=[]
for name,source,commits in [('npc-combat-reload-v897','host',1),('npc-combat-client-v900','client',4)]:
 T=N/name;assert rd(T/'result.json')['passed'] and rd(T/'restoration.json')['passed'] and rd(T/'result.json')['candidate']==pin['dll']
 death=rd(T/'native-death.json');dup=rd(T/'duplicate-death.json');old=rd(T/'reload-before.json');reload=rd(T/'reload-completed.json');fresh=rd(T/'rearmed-combat.json')
 assert death['host']['combat']['committed']==death['client']['combat']['applied']==commits
 assert death[source]['combat']['nativeAttempt']['declaration'].endswith('/assassination_damage.damage')
 for role in ('host','client'):
  assert death[role]['health']['current']==dup[role]['health']['current']==0
  assert death[role]['combat']['deaths']==dup[role]['combat']['deaths']==1 and death[role]['combat']['preDeathReleased']==2
  for ai in ('decision','reflex'):
   v=rd(T/f'dead-{role}-{ai}.json');assert not v['schedulerActive'] and not v['pauseReferences']
  assert reload[role]['reset']['completed']==old[role]['reset']['completed']+1 and reload[role]['reset']['slotRetryCalls']==0
  assert len(reload[role]['stream']['entries'])==7 and not reload[role]['combat']['enabled']
  assert fresh[role]['enabled'] and fresh[role]['result']=='active_native_melee' and fresh[role]['nonce']>death[role]['combat']['nonce']
  assert not fresh[role]['committed'] and not fresh[role]['applied'] and not fresh[role]['deaths']
  assert not rd(T/f'final-combat-{role}.json')['enabled'] and rd(T/f'final-player-health-{role}.json')['current']==56
 assert dup['client']['combat']['duplicates']>death['client']['combat']['duplicates']
 assert reload['client']['stream']['session']['released']==old['client']['stream']['session']['acquired']
 if source=='client':
  assert dup['host']['combat']['duplicates']>death['host']['combat']['duplicates']
  for i,hp in enumerate((30,20,10,0)):
   pair=rd(T/f'contact-{i}.json');assert pair['host']['health']['current']==pair['client']['health']['current']==hp
 else:
  assert death['host']['combat']['fixturePairedFrameReleases']==1 and not death['host']['combat']['fixtureHoldsFrames']
  handle=death['host']['health']['handle'];handle=int(handle,16) if isinstance(handle,str) else handle
  for role in ('host','client'):
   target=next(x for x in death[role]['stream']['poseDiagnostics'] if x['handle']==handle);assert not target['gaps'] and not target['failures']
  samples=list(T.glob('contact-0-skeleton-*.json'));assert len(samples)>=11
  assert all(rd(f)['joints'][0]['localPose'][12:15]==[0,0,0] for f in samples)
 scope=f'{source} source, {commits} host-ordered native melee outcome(s), matching death/stopped AI, exact pre-death release, duplicate rejection, then shared checkpoint and seven fresh combat bindings. Finite source AI fixture.'
 if source=='host':scope+=' Native assassination releases test frame hold; target origin stays zero with no target pose gap.'
 review='Paired contact sheets inspected: native first-person action, counterpart victim response/death and shared loading/return visible. Observer foreground actors/sword partly occlude action. Attacker/victim choreography and identical corpse placement remain unaccepted.'
 cases.append(dict(take=name,source=source,commits=commits,passed=True,scope=scope,visualReview=review))
 for role in ('host','client'):
  path=f'{name}/{role}.mp4';m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0];a,b=map(float,m['avg_frame_rate'].split('/'))
  videos=[v for v in videos if v['video']!=path];videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Native NPC {source} melee / checkpoint / {role} view',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2),container_fps=m['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review=review,candidate=A.name,dll=pin['dll']))
for name in ('npc-combat-reload-v886','npc-combat-reload-v888','npc-combat-reload-v895','npc-combat-client-v898'):
 assert not rd(N/name/'result.json')['passed'] and rd(N/name/'restoration.json')['passed']
for f in rd(W/'protected-save-hashes-before-v4.json'):assert sha(Path(f['Path']))==f['Hash']
at=datetime.datetime.now().astimezone().isoformat()
limits='Complete combat/movement/co-op remain unfinished. Finite source fixtures; other damage families/AI both-player authority, complete paired finisher/corpse/limb physics unresolved. Unrelated NPC pelvis translations can still exceed3m. Intermittent handshake882 remains unreproduced, and corrected native save-busy retry branch remains unexercised (slotRetryCalls0).'
audit=dict(at=at,candidate=A.name,dll=pin['dll'],scopedCombatRecoveryVerified=True,complete=False,cases=cases,poseGapRegression='npc-combat-audit-v890.json',limitations=limits)
wr(W/'npc-combat-audit-v901.json',audit);wr(N/'runtime-video-manifest.json',videos)
pin.update(runtimeVerified=False,nativeMeleeFixtureVerified=True,scopedCombatRecoveryVerified=True,runtimeEvidence='../npc-combat-audit-v901.json',runtimeScope='Host897 and client900 native melee/death/duplicate/shared-checkpoint cases pass. '+limits);wr(A/'identity.json',pin)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['NpcCombatContinuation'].update(at=at,candidate=A.name,runtimeVerified=False,complete=False,receipt=str((W/'npc-combat-audit-v901.json').relative_to(R)),latest='Host897 and client900 pass native melee/death, duplicates, shared checkpoint and fresh seven-NPC combat negotiation. Pose-gap damage continuity verified890; corrected test frame release eliminates target finisher root offset/gap897. All tested processes restored, experiments disabled. '+limits,next='Advance active-world generic NPC authority and broader native damage families without per-item implementations. Retain packet diagnostics for882; investigate remaining large native joint translations and paired/corpse presentation as needed. Native UI, real separate-account Steam, campaign/progression still required.');s['MovementExecutionPriority']['current']=s['NpcCombatContinuation']['latest'];wr(R/'work/current-task-state.json',s)
b=rd(R/'work/handoff-npc-combat-bookmark-v899.json');assert json.loads(b['receipt']['structuredContent']['result'])['success'] and json.loads(b['saved']['structuredContent']['result'])['success']
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
 p=R/'work'/name;items=rd(p)
 for entry in b['bookmarks']:
  items=[x for x in items if (x.get('address'),x.get('category'))!=(entry['address'],entry['category'])];items.append(dict(entry,type='Note'))
 wr(p,items)
print(json.dumps(dict(audit='npc-combat-audit-v901.json',passed=True,complete=False,videos=len(videos))))
