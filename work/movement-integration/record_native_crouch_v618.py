from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();videos=rd(N/'runtime-video-manifest.json');tests=[]
for role in ('corvo','emily'):
 take=f'native-crouch-combat-{role}-v613';f=N/take;r=rd(f/'result.json');assert r['passed'] and len(r['checks'])==4
 video=f/(('host' if role=='corvo' else 'client')+'-crouch-combat.mp4');rel=video.relative_to(N).as_posix();assert not any(v['video']==rel for v in videos)
 scenes=rd(f/'combat-scenes.json');contracts=sorted({(s['asset'],s['subscene'],s['sceneDurationMs'],len(s['bindingsResearch'])) for s in scenes})
 tests.append(dict(role=role,result=str((f/'result.json').relative_to(R)),sha256=sha(f/'result.json'),contracts=contracts))
 m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
 videos.append(dict(video=rel,kind='game',character=role,label=role.title()+' · native crouched combat phases · source613',alias='crouched_attack',requirements=['crouched_attack'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',proxy_sha256=r['dll'],video_sha256=sha(video),actual_capture_hz=int(m['nb_frames'])/float(m['duration']),checks_passed=True,scope=r['scope']+' Source observation only. Targeted slash933 and separate owned hit recovery are exact native evidence; remote body binding remains pending. No separate visual acceptance.'))
wr(W/'native-crouch-observations-v618.json',dict(at=at,tests=tests,remoteVerified=False,complete=False));wr(N/'runtime-video-manifest.json',videos)
pin=rd(W/'candidate-crouch-combat-v616/identity.json')
p=R/'work/current-task-state.json';s=rd(p);s['UpdatedAt']=at;s['CrouchCombatContinuation']=dict(at=at,candidate='candidate-crouch-combat-v616',dll=pin['dll'],probe=pin['probe'],latest='Native613 both roles confirms targeted crouched slash933 followed by owned hit recovery866/666, including native stance transition. Candidate616 passes build/ownership/wire/bank tests and is deployed; runtime617 is in progress. Protocol66, unchanged bank65/353. Verified610/615 is preserved. No shared victim events/outcomes; complete movement and co-op remain unfinished.',runtimeVerified=False,complete=False,next='Finish native617 runtime and visual checks for crouched strike and hit recovery; then continue missing actions and shared-world co-op development.');s['MovementExecutionPriority']['current']=s['CrouchCombatContinuation']['latest'];wr(p,s)
p=W/'refresh_grenade_handoff_v58.py';t=p.read_text();mark="m['LatestContinuation']['probeCandidate']=probeArchive.name";assert mark in t and "crouchCombat=state.get(" not in t
t=t.replace(mark,"crouchCombat=state.get('CrouchCombatContinuation',{})\nif crouchCombat:\n    m['LatestContinuation']['next']=crouchCombat.get('next',m['LatestContinuation']['next'])\n    m['LatestContinuation']['in_progress']+=' '+crouchCombat.get('latest','')\n"+mark);p.write_text(t)
print(json.dumps(dict(recordings=len(videos),sourceTests=2,runtimePending='617')))
