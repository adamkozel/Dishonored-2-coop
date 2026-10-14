from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C=rd(W/'candidate-npc-combat-v1113d/identity.json');assert sha(R/'work/asset-game/dinput8.dll')==C['dll']
plan=[('mission-combat-v1119',False,'Avatar perception armed on both roles (retained ~1000 continuous clone transforms each); the client native block+attack from behind an unaware guard swung and missed, so the guard was never provoked.'),
 ('mission-combat-v1120',True,'Host alerts the guard (one frontal hit, 30 -> 20 replicated) and withdraws 25 m; the client faces the alerted guard holding native block. Host AI targets show the guard combat target = the client avatar (clone) with the host player also in attention; NPC attack captured 1 enemy hit on the host, acked; consumed on the client; client blocked natively (health 70 unchanged).'),
 ('mission-combat-v1121',True,'Same without block: the guard keeps the client avatar as combat target for all 8 samples; the relayed enemy hit damages the actual client player 70 -> 55.')]
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take,passed,scope in plan:
    T=N/take;assert rd(T/'restoration.json')['passed'],take
    attack={q:{k:v for k,v in rd(T/f'after-npc-attack-{q}.json').items() if k in ('result','event','consumed','acked','captured','replicaBlocked')} for q in ('host','client')}
    targets=[(x['targets'].get('combat') or {}).get('handle') for x in rd(T/'guard-targets.json')] if (T/'guard-targets.json').exists() else None
    cases.append(dict(take=take,passed=passed,scope=scope,attack=attack,guardCombatTargets=targets,timeline=rd(T/'timeline.json')))
    for f in sorted(T.glob('*.mp4')):
        path=f'{take}/{f.name}';role='host' if '-host' in f.name else 'client'
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Stage 4 / guards engage both players / {take} / {f.name}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review='Raw native recording.',candidate='candidate-npc-combat-v1113d',dll=C['dll'],runtime_test_passed=passed))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'mission-engagement-audit-v1122.json',dict(at=at,candidate='candidate-npc-combat-v1113d',dll=C['dll'],harnesses=['../natural-moveset/Test-MissionCombat-v1121.ps1'],cases=cases,
  findings=['Confirmed runtime (stage 4, mission map): with avatar perception (chapter 77, map-agnostic) an alerted host-authoritative guard selects the remote player avatar as its native combat target while the host player stays in its attention list.','Confirmed runtime: the guard hit on the avatar is captured on the host, relayed by NPC attack and consumed once on the client: blocked natively (no damage) or applied as genuine client player damage (70 -> 55).','Together with audits 1110/1118: on Edge of the World enemies perceive and fight both players, each player hit/kill commits once, deaths and corpses are shared.'],
  complete=False,outstanding=['Sustained multi-guard fights, both players attacked at once, ranged/power damage on missions.','Player death and shared checkpoint on missions (stage 6 dependency).','Dynamic NPC relevance and corpse retirement; tutorial-modal tolerance; performance under combat.']))
wr(N/'runtime-video-manifest.json',videos)
C.update(scopedMissionEngagementVerified=True,engagementEvidence='../mission-engagement-audit-v1122.json');wr(W/'candidate-npc-combat-v1113d/identity.json',C)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1113d live: stage 4 on missions - enemies perceive and fight both players (guard targets the client avatar; relayed hits blocked natively or damaging the client); player hits/kills commit once; shared deaths and corpses.'
s['MovementExecutionPriority']['next']='Stage 4/6: player death and shared checkpoint recovery on missions; sustained multi-enemy combat; dynamic NPC relevance; then stage 5 interactions breadth.'
wr(R/'work/current-task-state.json',s)
print('ok',len(videos))
