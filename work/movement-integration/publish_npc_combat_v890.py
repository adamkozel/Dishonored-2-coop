from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration';A=W/'candidate-npc-combat-v889';T=N/'npc-combat-reload-v890'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
pin=rd(A/'identity.json');assert sha(A/'dinput8.dll')==pin['dll']==rd(T/'result.json')['candidate']
assert rd(T/'result.json')['passed'] and rd(T/'restoration.json')['passed']
death=rd(T/'native-death.json');dup=rd(T/'duplicate-death.json');reload=rd(T/'reload-completed.json');fresh=rd(T/'rearmed-combat.json')
for i,hp in enumerate((30,20,10,0)):
 pair=rd(T/f'contact-{i}.json');assert pair['host']['health']['current']==pair['client']['health']['current']==hp
assert death['host']['combat']['committed']==death['client']['combat']['applied']==4
assert dup['client']['combat']['duplicates']>death['client']['combat']['duplicates']
for role in ('host','client'):
 assert death[role]['combat']['deaths']==1 and death[role]['combat']['preDeathReleased']==2
 assert dup[role]['health']['current']==0 and dup[role]['combat']['deaths']==1
 for ai in ('decision','reflex'):
  v=rd(T/f'dead-{role}-{ai}.json');assert not v['schedulerActive'] and not v['pauseReferences']
 assert reload[role]['reset']['completed']==1 and reload[role]['reset']['loads']==1 and reload[role]['reset']['slotRetryCalls']==0
 assert len(reload[role]['stream']['entries'])==7 and not reload[role]['combat']['enabled']
 assert fresh[role]['enabled'] and fresh[role]['result']=='active_native_melee' and fresh[role]['nonce']>death[role]['combat']['nonce']
 assert not fresh[role]['committed'] and not fresh[role]['applied'] and not fresh[role]['deaths']
 assert not rd(T/f'final-combat-{role}.json')['enabled'] and rd(T/f'final-player-health-{role}.json')['current']==56
assert reload['client']['stream']['session']['released']==10
assert any(p['gaps'] for p in death['client']['stream']['poseDiagnostics'])
assert any(p['lastFailure']=='pose_validation' and p['failures']>=13 for p in death['host']['stream']['poseDiagnostics'])
for f in rd(W/'protected-save-hashes-before-v4.json'):assert sha(Path(f['Path']))==f['Hash']
at=datetime.datetime.now().astimezone().isoformat()
scope='Host three ordinary sword contacts plus assassination synchronize40/30/20/10/0 through13 rejected skeletal samples and an actual client pose gap. Native AI stops, exactly two target references release before death, terminal duplicate rejected. Shared checkpoint renews seven NPCs and fresh combat nonce; restoration passes.'
limits='One finite source-fixture regression; full combat and movement remain incomplete. Native finisher presentation has a pose gap; corpse/limb physics and paired choreography unaccepted. Save-busy retry branch unexercised (slotRetryCalls0); intermittent negotiation failure882 remains unreproduced. Other damage families/active-world AI still outstanding.'
audit=dict(at=at,candidate=A.name,dll=pin['dll'],scopedRegressionPassed=True,poseGapDamageContinuityVerified=True,sharedCheckpointPassed=True,complete=False,scope=scope,limitations=limits,take=str(T.relative_to(R)))
wr(W/'npc-combat-audit-v890.json',audit)
pin.update(runtimeVerified=False,nativeMeleeFixtureVerified=True,poseGapDamageContinuityVerified=True,runtimeEvidence='../npc-combat-audit-v890.json',runtimeScope=scope+' '+limits);wr(A/'identity.json',pin)
videos=rd(N/'runtime-video-manifest.json')
for role in ('host','client'):
 path=f'{T.name}/{role}.mp4';m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0];a,b=map(float,m['avg_frame_rate'].split('/'))
 videos=[v for v in videos if v['video']!=path];videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'NPC melee through finisher pose gap / checkpoint / {role}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2),container_fps=m['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review='Paired 4x4 contact sheets inspected: native repeated sword hits, victim falling and shared loading/return visible. Finisher skeletal gap and differing corpse presentation remain; full attacker/victim choreography not accepted.',candidate=A.name,dll=pin['dll']))
wr(N/'runtime-video-manifest.json',videos)
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at;state['NpcCombatContinuation'].update(at=at,candidate=A.name,runtimeVerified=False,complete=False,receipt=str((W/'npc-combat-audit-v890.json').relative_to(R)),latest=scope+' '+limits,next='Inspect rejected finisher transform with diagnostic891, verify client-source regression and improve skeletal transport without coupling damage authority to transient pose freshness.');state['MovementExecutionPriority']['current']=state['NpcCombatContinuation']['latest'];wr(R/'work/current-task-state.json',state)
b=rd(R/'work/handoff-npc-combat-bookmark-v889.json')
assert json.loads(b['receipt']['structuredContent']['result'])['success'] and json.loads(b['saved']['structuredContent']['result'])['success']
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
 p=R/'work'/name;items=rd(p)
 for entry in b['bookmarks']:
  items=[x for x in items if (x.get('address'),x.get('category'))!=(entry['address'],entry['category'])];items.append(dict(entry,type='Note'))
 wr(p,items)
print(json.dumps(dict(audit='npc-combat-audit-v890.json',passed=True,complete=False,videos=len(videos))))
