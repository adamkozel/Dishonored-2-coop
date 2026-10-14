from pathlib import Path
import json,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C={k:rd(W/f'candidate-npc-combat-{k}/identity.json') for k in ('v1166','v1166b')}
assert sha(R/'work/asset-game/dinput8.dll')==C['v1166b']['dll']
plan=[('mission-npc-relevance-v1167',False,'1166: spawn set (9) bound on the host and followed by the client on Hello; after a ~205 m move the host dropped the 9 and added 11, the client bound them, then the NPC session ended with session_pose_stale (a >1 s pose gap during the long move) -> 1166b.'),
      ('mission-npc-relevance-v1168',True,'Pass (1166b): spawn 9 -> far interior 11 (9 dropped, 11 added) -> back at spawn 9 (11 dropped, 9 re-added); host and client bound/ready sets identical at every stage within about 1 s; no transport stall occurred in this take.')]
cases=[]
for take,passed,scope in plan:
    T=N/take;r=rd(T/'result.json');assert bool(r['passed'])==passed and rd(T/'restoration.json')['passed'],take
    cases.append(dict(take=take,passed=passed,scope=scope,failure=r.get('failure'),stages=r.get('cases')))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'mission-npc-relevance-audit-v1169.json',dict(at=at,candidate='candidate-npc-combat-v1166b',dll=C['v1166b']['dll'],harnesses=['../natural-moveset/Test-MissionNpcRelevance-v1168.ps1'],cases=cases,
  findings=['Confirmed runtime: host-owned dynamic NPC relevance - every 2 s the host binds eligible NPCs within 55 m of either player (max 24) and retires channels beyond 75 m of both (Stop to the client); the client binds on host Hello; sets stay identical across a ~205 m move and back.','Session stall tolerance (1166b: 1-10 s without partner poses suspends NPC/door/usable sessions like a menu) is implemented but was not exercised in 1168 (no stall occurred): hypothesis-level until a stall is observed.'],
  complete=False,outstanding=['Exercise the stall-suspend path deliberately.','Combat while channels rebind (a bound NPC engaged in combat leaving 75 m).','Visual review of replicas after rebinding (this harness recorded no video).']))
C['v1166b'].update(runtimeVerified=True,scopedNpcRelevanceVerified=True,runtimeEvidence='../mission-npc-relevance-audit-v1169.json');wr(W/'candidate-npc-combat-v1166b/identity.json',C['v1166b'])
C['v1166'].update(runtimeVerified=False,result='superseded by 1166b (pose stall ended the session); see audit',runtimeEvidence='../mission-npc-relevance-audit-v1169.json');wr(W/'candidate-npc-combat-v1166/identity.json',C['v1166'])
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1166b live: stage 4 dynamic NPC relevance on missions (host-owned set follows both players, audit 1169) on top of stage 6 mission checkpoints/renewal (1153c) and stage 5 shared usables.'
s['MovementExecutionPriority']['next']='Stage 4: sustained multi-enemy combat with rebinding; stage 5/6: Kiscule progression mirroring and mission transitions (chapter 89); stage 7 native UI.'
wr(R/'work/current-task-state.json',s);print('ok')
