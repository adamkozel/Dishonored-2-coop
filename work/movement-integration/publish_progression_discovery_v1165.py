from pathlib import Path
import json,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C=rd(W/'candidate-npc-combat-v1163/identity.json');assert sha(R/'work/asset-game/dinput8.dll')==C['dll']
cases=[]
for take,scope in (('mission-entities-v1162','Read-only Edge of the World entity census (11155 entities per role, identical counts): 127 arkTrigger volumes (vtable 0x1c5eae8), 205 area volumes, 371 usables, 472 pickups, 141 NPCs.'),
                   ('mission-activations-v1164','1163 activation observer: zero idEntity::Activate calls during mission load and while the host stood inside three script trigger volumes; a console "trigger" on a Dunwall light registered exactly one (observer confirmed working; lab restored).')):
    T=N/take;r=rd(T/'result.json');assert r['passed'] and rd(T/'restoration.json')['passed'];cases.append(dict(take=take,passed=True,scope=scope,cases=r.get('cases')))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'mission-progression-discovery-v1165.json',dict(at=at,candidate='candidate-npc-combat-v1163',dll=C['dll'],cases=cases,
  native=dict(consoleCommands='no mission-end/next-mission console command (113 commands listed; campaign/map/restartmap only)',scriptNodes=['arkKisculeNodeLevelTransition','arkKisculeNodeActionGameflowMission','arkKisculeNodeActionGameflowObjective','arkKisculeNodeTriggerNotification (vtable 0x141c68488)','arkKisculeNodeActionAutosave (vtable 0x141b7c9e8)'],entityActivate='0x140875290 idEntity::Activate(entity, activator) -> vtable+0x128 when entity+0xd0 & 8; arkTrigger vtable+0x128 is a no-op (0x1400998b0)',triggerPath='trigger volumes feed the Kiscule script graph via their touch/physics path (arkHavokUserData::triggerEnterCount_t) and arkKisculeNodeTriggerNotification, not idEntity::Activate'),
  findings=['Confirmed runtime: mission script progression (trigger volumes, objectives, level transition) does not pass through idEntity::Activate; mirroring progression requires the Kiscule trigger-notification/objective level.','Confirmed static: mission transitions are Kiscule nodes (LevelTransition, GameflowMission); there is no console command for them.'],
  complete=False,outstanding=['Locate the arkTrigger touch -> Kiscule notification dispatch and the objective-state store; design host-authoritative objective/trigger mirroring.','Mission end and next-mission load for both peers (stage 6) depend on the same script level.']))
C.update(runtimeVerified=True,result='read-only activation observer verified (console trigger counted); mission triggers do not use it',runtimeEvidence='../mission-progression-discovery-v1165.json');wr(W/'candidate-npc-combat-v1163/identity.json',C)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1163 live (1153c + read-only activation observer): stage 6 mission checkpoints/renewal done on Edge of the World; progression discovery recorded (Kiscule level).'
s['MovementExecutionPriority']['next']='Stage 4: dynamic NPC relevance (rebind NPC authority as players move); then Kiscule trigger/objective mirroring for stages 5/6 (progression, mission transitions); stage 7 native UI.'
wr(R/'work/current-task-state.json',s);print('ok')
