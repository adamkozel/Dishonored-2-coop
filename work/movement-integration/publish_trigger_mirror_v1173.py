from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C=rd(W/'candidate-npc-combat-v1171/identity.json');assert sha(R/'work/asset-game/dinput8.dll')==C['dll']
cases=[]
for take,passed,scope in (('mission-trigger-components-v1170',True,'Read-only: every arkTrigger volume has exactly one native component, arkCpntTrigger (vtable 0x1c5bae8; interfaces 0x1c5bbc8 at +0x20, 0x1c5ba98 at +0x40).'),
                          ('mission-trigger-mirror-v1172',True,'Pass: with coop_trigger_mirror on, the host walked into three script trigger volumes (and crossed others on the way) while the client stayed at the docks spawn ~80 m away; all 9 host-local script-trigger enter/exit events replayed natively in the client world for Emily\'s player (none missing/rejected); audio volumes were not mirrored; the Wall of Light tutorial trigger raised the native tutorial modal (logic 12) on both peers; the client HUD then showed the native Objective updated banner and HOSTILE TERRITORY indicator.')):
    T=N/take;r=rd(T/'result.json');assert r['passed']==passed and rd(T/'restoration.json')['passed'],take
    cases.append(dict(take=take,passed=passed,scope=scope,cases=r.get('cases')))
videos=rd(N/'runtime-video-manifest.json')
T=N/'mission-trigger-mirror-v1172'
for f in sorted(T.glob('*.mp4')):
    meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
    path=f'{T.name}/{f.name}';role='host' if '-host' in f.name else 'client'
    videos=[x for x in videos if x['video']!=path]
    videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Mission progression / mirrored trigger volumes / {T.name} / {f.name}',travelling=True,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=cases[1]['scope'],
        review=('Client sheet inspected: Emily stays at the docks; native Play Your Way #2 tutorial modal appears, then Objective updated banner and HOSTILE TERRITORY indicator.' if role=='client' else 'Host sheet inspected: host moved into the Wall of Light area; HOSTILE TERRITORY, objective update and tutorials visible.'),
        candidate='candidate-npc-combat-v1171',dll=C['dll'],runtime_test_passed=True,visual_accepted=True))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'mission-trigger-mirror-audit-v1173.json',dict(at=at,candidate='candidate-npc-combat-v1171',dll=C['dll'],harnesses=['../natural-moveset/Test-TriggerMirror-v1172.ps1','../natural-moveset/Probe-TriggerComponents-v1170.ps1'],cases=cases,
  native=dict(triggerHandler='0x140779e00 arkCpntTrigger enter/exit (component, entity, enter): posts the trigger script event (FUN_1408848a0 with onEnter/onExit), once-trigger early-out (component+0xd0 and entity+0xd4&4), messages 0xd2/0xd3',registration='0x140779980 registers arkCpntTrigger::OnTriggerEvent with the physics trigger system',component='arkCpntTrigger vtable 0x1c5bae8; arkTrigger entity vtable 0x1c5eae8'),
  findings=['Confirmed runtime: replaying the native arkCpntTrigger enter/exit handler for the partner\'s own player advances the partner\'s mission script (tutorial modal, objective update, hostile territory) without the partner entering the volume.','Mirroring is limited to script_trigger* volumes (not hideout/noswim/audio volumes, which describe the local player\'s own position).'],
  complete=False,outstanding=['Bidirectional run (client-originated triggers) and a full mission walk.','Triggers whose scripts depend on the entering player\'s position (e.g. spawn-on-player) may need per-trigger review.','Mission end / level transition (arkKisculeNodeLevelTransition) via the same mirror and a both-ready barrier.']))
wr(N/'runtime-video-manifest.json',videos)
C.update(runtimeVerified=True,scopedTriggerMirrorVerified=True,runtimeEvidence='../mission-trigger-mirror-audit-v1173.json');wr(W/'candidate-npc-combat-v1171/identity.json',C)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1171 live: stage 5 shared mission progression - script trigger volumes entered by one player replay natively in the partner world (audit 1173); plus dynamic NPC relevance, mission checkpoints/renewal, shared usables.'
s['MovementExecutionPriority']['next']='Client-originated trigger mirror run; mission end/level transition for both peers; then stage 7 native UI.'
wr(R/'work/current-task-state.json',s);print('ok',len(videos))
