from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration';A=W/'candidate-npc-combat-v871'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
 if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
cases=[];videos=rd(N/'runtime-video-manifest.json')
for name,source,commits,decl in [('npc-combat-host-v872','host',1,'assassination_damage'),('npc-combat-client-v874','client',4,'assassination_damage'),('npc-combat-fatality-v875','host',1,'generic_fatality')]:
 t=N/name;assert rd(t/'result.json')['passed'] and rd(t/'restoration.json')['passed']
 after=rd(t/'native-death.json');duplicate=rd(t/'duplicate-death.json');settled=rd(t/'settled-death.json')
 assert after[source]['combat']['nativeAttempt']['declaration']==f'components/characters/player/base/damage/{decl}.damage'
 assert after['host']['combat']['committed']==after['client']['combat']['applied']==commits
 for role in ('host','client'):
  for pair in (after,duplicate,settled):
   assert pair[role]['health']['current']==0 and pair[role]['combat']['deaths']==1
   assert pair[role]['combat']['preDeathReleased']==2 and pair[role]['player']['current']==56
  assert after[role]['combat']['nativeOutcome']['stage']=='native_death_complete'
  assert duplicate[role]['combat']['committed']==after[role]['combat']['committed'] and duplicate[role]['combat']['applied']==after[role]['combat']['applied']
  for ai in ('decision','reflex'):
   a=rd(t/f'dead-{role}-{ai}.json');assert not a['started'] and not a['schedulerActive'] and not a['pauseReferences']
  final=rd(t/f'final-combat-{role}.json');assert not final['enabled'] and not final['fixture']['active'] and not final['fixture']['lost']
  assert final['fixture']['acquired']==final['fixture']['released']
  stream=rd(t/f'final-stream-{role}.json');assert not stream['enabled'] and not stream['session']['lost']
 assert duplicate['client']['combat']['duplicates']>after['client']['combat']['duplicates']
 if source=='client':
  assert duplicate['host']['combat']['duplicates']>after['host']['combat']['duplicates']
  for i,hp in enumerate((30,20,10,0)):
   contact=rd(t/f'contact-{i}.json');assert contact['host']['health']['current']==contact['client']['health']['current']==hp
 scope=f'{source}-initiated native melee: {commits} host-ordered outcome(s), matching death and stopped native AI in both games; two references released per actor before death, terminal duplicates rejected. Finite source AI/root fixture; full paired choreography, shared ragdoll/limbs, other damage families and campaign acceptance remain incomplete.'
 if 'fatality' in name:scope+=' Finite assassination exclusion selects generic fatality for this test only.'
 review='Contact sheets inspected: native first-person action and counterpart victim response/death visible. Observer sword partly obscures the upper view; exact attacker/victim alignment and matching corpse physics are not accepted.'
 if 'fatality' in name:review='Source finishing hit and fallen victim visible. Receiver camera misses the victim: receiver death is validated through native health and stopped AI, not a visual matching-body claim.'
 cases.append(dict(take=name,source=source,commits=commits,declaration=decl,passed=True,scope=scope,visualReview=review))
 for role in ('host','client'):
  path=f'{name}/{role}.mp4';metadata=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,avg_frame_rate,duration','-of','json',str(N/path)],text=True))['streams'][0]
  rate=metadata['avg_frame_rate'];num,den=map(float,rate.split('/'))
  videos=[v for v in videos if v['video']!=path]
  videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Native NPC melee / death · {source} source · {role} view',travelling=False,gameplay_bound=True,actual_capture_hz=round(num/den,2),container_fps=rate,timing='Native wall-time recording, no speedup',scope=scope,review=review,candidate=A.name,dll=pin['dll']))
assert not rd(N/'npc-combat-reload-v876/result.json')['passed']
assert not rd(N/'npc-combat-reload-v876/restoration.json')['passed']
assert rd(N/'npc-combat-reload-v876/recovery/result.json')['passed']
for f in rd(W/'protected-save-hashes-before-v4.json'):assert sha(Path(f['Path']))==f['Hash']
at=datetime.datetime.now().astimezone().isoformat()
limitation='Shared checkpoint after death failed in876: host remained expectBoundary with no loading entry, client timed out. Ordinary cleanup could not disable the host reset; exact isolated process restart restored baseline. Failed restoration and separate successful recovery are both preserved. Further trace/reproduction pending. Complete combat, movement and playable co-op remain unfinished.'
audit=dict(at=at,scopedNativeMeleePassed=True,complete=False,candidate=A.name,dll=pin['dll'],cases=cases,checkpointAfterDeathPassed=False,checkpointFailure=limitation,unchanged=['movement protocol69','both357-clip bank67 payloads','original Steam executable and DLL','protected save/config hashes'])
wr(W/'npc-combat-audit-v877.json',audit);wr(N/'runtime-video-manifest.json',videos)
pin.update(runtimeVerified=False,nativeMeleeFixtureVerified=True,runtimeEvidence='../npc-combat-audit-v877.json',runtimeScope='Three recorded isolated native melee/death cases passed; full adapter acceptance remains incomplete because checkpoint-after-death876 failed. '+limitation);wr(A/'identity.json',pin)
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['NpcCombatContinuation']=dict(at=at,candidate=A.name,runtimeVerified=False,nativeMeleeFixtureVerified=True,complete=False,receipt='work/movement-integration/npc-combat-audit-v877.json',details='documentation/handoff-2026-10-11/73-npc-combat-development.md',latest='Host assassination872, client three ordinary hits plus assassination874, and host generic fatality875 pass native health/death/AI/duplicate checks. All use finite isolated source fixtures. Six videos added with explicit visual limits. '+limitation,next='Resolve post-death shared checkpoint loading with native trace; then eliminate source fixtures for active both-player AI combat, remaining damage families and shared corpse/world lifecycle.',previousAcceptedNpcLifecycle='candidate-npc-stream-v847')
state['MovementExecutionPriority']['current']=state['NpcCombatContinuation']['latest'];wr(R/'work/current-task-state.json',state)
print(json.dumps(dict(cases=len(cases),videos=len(videos),audit='npc-combat-audit-v877.json',complete=False)))
