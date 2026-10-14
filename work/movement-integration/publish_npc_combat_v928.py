"""Audit measured results; do not infer untested trigger activation from fixture outcomes."""
from pathlib import Path
import datetime,hashlib,json,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';A=W/'candidate-npc-combat-v926'
def rd(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']==sha(A/'dinput8.dll')
for p in (R/'outputs/d2-coop-lab/src').iterdir():
 if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
cases=[];videos=rd(N/'runtime-video-manifest.json')
matrix=[('npc-combat-client-v906',905,True),('npc-combat-client-v908',907,True),('npc-combat-host-v909',907,False),('npc-combat-world-v912',911,False),('npc-combat-world-v916',915,False),('npc-combat-world-v919',918,True),('npc-combat-hazard-v920',918,True),('npc-combat-trigger-v921',918,True),('npc-combat-fixture-v923',922,False),('npc-combat-fixture-v927',926,None),('npc-combat-client-v929',926,None)]
for name,build,expected in matrix:
 T=N/name;cp=rd(W/f'candidate-npc-combat-v{build}/identity.json');result=rd(T/'result.json');restoration=rd(T/'restoration.json')
 assert result['candidate']==cp['dll'] and restoration['passed'],name
 if expected is not None:assert result['passed']==expected,name
 source=result['source'];world=rd(T/'world-outcomes.json') if (T/'world-outcomes.json').exists() else None
 case=dict(take=name,candidate=f'candidate-npc-combat-v{build}',dll=cp['dll'],passed=result['passed'],failure=result['failure'],source=source,restored=True,worldOutcomes=world)
 if result['passed']:
  death=rd(T/'native-death.json');beforeDup=rd(T/'before-duplicate.json') if (T/'before-duplicate.json').exists() else death
  dup=rd(T/('duplicate-latest.json' if (T/'duplicate-latest.json').exists() else 'duplicate-death.json'))
  old=rd(T/'reload-before.json');reloaded=rd(T/'reload-completed.json');fresh=rd(T/'rearmed-combat.json')
  commits=death['host']['combat']['committed'];assert commits>0 and commits==death['client']['combat']['applied'],name
  for role in ('host','client'):
   c=death[role]['combat'];assert c['enabled'] and c['deaths']==1 and c['nativeLethalSubtractions']==1 and c['preDeathReleased']==2,name
   assert death[role]['health']['current']==dup[role]['health']['current']==0 and dup[role]['combat']['deaths']==1
   assert dup[role]['combat']['committed']==beforeDup[role]['combat']['committed'] and dup[role]['combat']['applied']==beforeDup[role]['combat']['applied']
   assert not any(x['pending'] for x in c['entries'])
   for ai in ('decision','reflex'):
    x=rd(T/f'dead-{role}-{ai}.json');assert not x['schedulerActive'] and not x['pauseReferences']
   assert reloaded[role]['reset']['completed']==old[role]['reset']['completed']+1 and reloaded[role]['bridge']['localGameLogicState']==1
   assert not reloaded[role]['combat']['enabled'] and fresh[role]['enabled'] and fresh[role]['result']=='active_native_melee'
   assert fresh[role]['nonce']>c['nonce'] and not fresh[role]['committed'] and not fresh[role]['applied'] and not fresh[role]['deaths']
   assert rd(T/f'final-player-health-{role}.json')['current']==56 and not rd(T/f'final-combat-{role}.json')['enabled']
  assert dup['client']['combat']['duplicates']>beforeDup['client']['combat']['duplicates']
  if source=='client':assert dup['host']['combat']['duplicates']>beforeDup['host']['combat']['duplicates']
  if world:
   assert world['host']==world['client']
   for h in rd(T/'world-health.json'):assert h['host']['current']==h['client']['current'] and h['host']['maximum']==h['client']['maximum']
  case['commitsAtPrimaryDeath']=commits
  case['scope']='Finite NPC fixture: native target damage/death, exactly-once outcomes, checkpoint/rearm and protected restoration pass. Complete active-world combat, paired choreography and corpse physics are not established.'
 else:case['scope']='Failed investigation retained; no acceptance. '+str(result['failure'])
 if name in ('npc-combat-world-v919','npc-combat-hazard-v920','npc-combat-trigger-v921'):
  assert world and world['host']==world['client']==0
  case['scope']+=' Authored map-trigger and corpse outcome paths were not exercised.'
 if name in ('npc-combat-fixture-v923','npc-combat-fixture-v927'):case['scope']+=' Explicit reconstruction of an observed native fire callback; not natural map-trigger activation.'
 if name=='npc-combat-fixture-v927':
  assert result['passed'] and world['host']==world['client']==3 and world['replicaSuppressed']>=1
  settled=rd(T/'settled-death.json')
  assert settled['host']['combat']['nativeAttempt']['declaration']=='components/base/damage/corpse_damage.damage'
  assert settled['host']['combat']['lastOutcome']==dict(before=50,after=49,maximum=50,inflicted=1)
  assert settled['host']['combat']['nativeSubtractions']==settled['client']['combat']['nativeSubtractions']==3
  assert rd(T/'verified.json')['inputAttempts']==0
  case['scope']+=' Three native world outcomes verified: zero-damage ignition, terminal burning continuation and corpse contact reducing another NPC50 to49; client-local continuation suppressed; no sword input.'
 reviews={}
 for review in ('visual-review-v924.json','visual-review-v910.json'):
  if (T/review).exists():reviews=rd(T/review);break
 case['visualReview']=reviews;cases.append(case)
 for role in ('host','client'):
  path=f'{name}/{role}.mp4'
  if not (N/path).exists():continue
  meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0]
  a,b=map(float,meta['avg_frame_rate'].split('/'));videos=[v for v in videos if v['video']!=path]
  videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'NPC native damage / {name} / {role} / '+('scoped pass' if result['passed'] else 'failed investigation'),travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2),container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=case['scope'],review=reviews.get(role,'Recording retained; no new visual acceptance in this audit.'),candidate=case['candidate'],dll=cp['dll'],runtime_test_passed=result['passed']))
T=N/'npc-ranged-observe-v903';assert rd(T/'result.json')['passed'] and rd(T/'restoration.json')['passed']
obs=rd(T/'damage.json');assert len(obs['events'])==3 and [(e['before'],e['after']) for e in obs['events']]==[(40,24.8662033),(24.8662033,0),(50,49)]
assert rd(T/'isolation-after.json')['lost']==2
path=f'{T.name}/host.mp4';meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
videos=[v for v in videos if v['video']!=path];videos.append(dict(video=path,role='host',character='corvo',label='Native pistol compound damage observation903',travelling=False,gameplay_bound=False,actual_capture_hz=round(a/b,2),container_fps=meta['avg_frame_rate'],timing='Native wall-time recording',scope='Local observation only: spread, bullet and secondary corpse contact. No replicated ranged acceptance. Old observation fixture lost two release confirmations on death; independent checkpoint restoration passed.',review='Source contact sheet inspected: pistol shot, victim death, nearby NPC and restoration loading visible.',candidate='candidate-npc-combat-v896',dll=rd(T/'result.json')['candidate']))
for x in rd(W/'protected-save-hashes-before-v4.json'):assert sha(Path(x['Path']))==x['Hash']
at=datetime.datetime.now().astimezone().isoformat()
limits='Full combat/co-op and movement acceptance remain unfinished. Actual compound ranged requests, projectile ownership, protected regions, active AI targeting both players, paired victim choreography and shared corpse/limb physics remain incomplete. Natural map-trigger activation is unverified; reconstructed ignition/native burning death plus one secondary corpse contact pass927. Intermittent handshake882 and positive native save-busy retry coverage remain open.'
audit=dict(at=at,candidate=A.name,dll=pin['dll'],combatProtocol=4,combatPacketBytes=256,complete=False,cases=cases,observation='npc-ranged-observe-v903',compoundQueue='Bounded32 FIFO policy and melee integration pass; real multi-contact native ranged burst pending.',limitations=limits)
wr(W/'npc-combat-audit-v928.json',audit);wr(N/'runtime-video-manifest.json',videos)
latest=cases[-1];fixture=cases[-2];pin.update(runtimeVerified=False,scopedNativeWorldDamageFixtureVerified=True,scopedNativeMeleeRecoveryVerified=latest['passed'],runtimeEvidence='../npc-combat-audit-v928.json',runtimeScope=fixture['scope']+' '+latest['scope']+' '+limits,combatProtocol=4,combatPacketBytes=256);wr(A/'identity.json',pin)
for build in (905,907,918):
 p=W/f'candidate-npc-combat-v{build}/identity.json';v=rd(p);v.update(runtimeVerified=False,scopedNativeMeleeRecoveryVerified=True,runtimeEvidence='../npc-combat-audit-v928.json',runtimeScope='See per-take evidence; scoped native melee, death, duplicates and checkpoint recovery. '+limits);wr(p,v)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['NpcCombatContinuation'].update(at=at,candidate=A.name,runtimeVerified=False,complete=False,receipt='work/movement-integration/npc-combat-audit-v928.json',latest='Native final-subtraction retirement905, pending-request FIFO907, host corpse descriptor911, client world authority915, generic authored trigger damage918 and explicit native event fixture922 and shared contact geometry926 implemented. Host919/920/921 and earlier client906/908 regressions pass. Latest927 fixture: '+fixture['scope']+' Latest929 client regression: '+latest['scope']+' '+limits,next='Continue from audit928 and handoff74. Distinguish actual trigger activation from reconstructed event fixture. Validate client melee on latest build; close native world event/source/zero-result gaps and extend a shared contact/spread descriptor plus callback-time projectile ownership. Preserve scoped native results and proceed to active AI and broad co-op without polish gates.')
s['MovementExecutionPriority']['current']=s['NpcCombatContinuation']['latest'];wr(R/'work/current-task-state.json',s)
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
 p=R/'work'/name;items=rd(p)
 for version in (907,918,930):
  b=rd(R/f'work/handoff-npc-combat-bookmark-v{version}.json')
  assert json.loads(b['receipt']['structuredContent']['result'])['success'] and json.loads(b['saved']['structuredContent']['result'])['success']
  for entry in b['bookmarks']:
   items=[x for x in items if (x.get('address'),x.get('category'))!=(entry['address'],entry['category'])];items.append(dict(entry,type='Note'))
 wr(p,items)
print(json.dumps(dict(audit='npc-combat-audit-v928.json',cases=len(cases),recordings=len(videos),complete=False)))
