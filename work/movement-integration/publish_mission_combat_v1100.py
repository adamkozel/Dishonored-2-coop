from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C=rd(W/'candidate-npc-combat-v1097/identity.json');C91=rd(W/'candidate-npc-combat-v1091/identity.json');assert sha(R/'work/asset-game/dinput8.dll')==C['dll']
names={C['dll']:'candidate-npc-combat-v1097',C91['dll']:'candidate-npc-combat-v1091'}
plan=[('mission-combat-v1094','Wall of Light guards: native tutorial modal (logic12, Play Your Way #2) opened on placement; NPC stream refused and the health read failed.'),
 ('mission-combat-v1095','Windmill guard: placement diagnostics captured the tutorial modal on both roles.'),
 ('mission-combat-v1096','Modal closed with B: NPC stream 12 channels and NPC combat (active_native_melee) on both; the guard fought the host natively (70 -> 32.5); every client replica went client_pose_stale - host send queue ~970 ms (pose bandwidth 20 Hz x 12 NPCs).'),
 ('mission-combat-v1098','1097 pose budget (interval 241 ms, queue 0): client guard replica tracks the host guard through the fight; the guard killed the host (70 -> 0) - no shared death on missions yet; NPC attack unavailable (partner follow/stream not re-armed after the modal).'),
 ('mission-combat-v1099','Partner presentation re-armed after the modal; NPC attack active_native_sword on both; host blocking takes chip damage (70 -> 64.75 -> 59.5); replica tracks the host guard; host strikes did not change guard health (host combat hit_damage_parameters); client combat all_bound_actors_retired; guard engaged only the host.')]
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take,scope in plan:
    T=N/take;r=rd(T/'result.json');assert rd(T/'restoration.json')['passed'],take
    ident=rd(T/'identity.json');dll=ident['dll']
    tl=rd(T/'timeline.json') if (T/'timeline.json').exists() else None
    cases.append(dict(take=take,candidate=names.get(dll,dll),harnessPassed=r['passed'],failure=r.get('failure'),scope=scope,timeline=tl))
    for f in sorted(T.glob('*.mp4')):
        path=f'{take}/{f.name}';role='host' if '-host' in f.name else 'client'
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Stage 4 / mission combat / {take} / {f.name}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review='Raw native recording; discovery.',candidate=names.get(dll,dll),dll=dll,runtime_test_passed=r['passed']))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'mission-combat-audit-v1100.json',dict(at=at,candidate='candidate-npc-combat-v1097',dll=C['dll'],harnesses=['../natural-moveset/Test-MissionCombat-v1099.ps1'],cases=cases,discovery=True,
  findings=['Confirmed runtime: mission areas raise native tutorial modals (logic12) that invalidate follow and retire presentation; harnesses close them with B and re-arm (gameplay needs automatic re-arm or modal tolerance).','Confirmed runtime: NPC body pose bandwidth must be budgeted - 20 Hz x 12 NPCs saturated the 512 KB/s Steam send rate; 1097 budget (~160 KB/s, 241 ms interval) keeps queue at 0 and replicas exact during combat.','Confirmed runtime: on a mission map a guard natively engages and damages the host, block reduces damage, and the client replica of the guard follows host authority.'],
  complete=False,outstanding=['Host player strikes on the bound guard did not change its health (blocked or rejected) - investigate npc_combat outcome path on missions.','Client NPC combat retires all bound actors once the guard enters combat.','Guards perceive/engage only the host; partner (clone) perception on missions.','Shared death/checkpoint on missions (host died in 1098).','Modal tolerance/automatic re-arm for tutorial modals.']))
wr(N/'runtime-video-manifest.json',videos)
C.update(runtimeVerified=True,scopedMissionNpcBudgetVerified=True,runtimeEvidence='../mission-combat-audit-v1100.json');wr(W/'candidate-npc-combat-v1097/identity.json',C)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1097 live: stage 4 on missions - NPC authority with pose bandwidth budget holds through native guard combat; guards engage the host; enemy-attack replication active; equipment/item wheel, pickups, doors on missions.'
s['MovementExecutionPriority']['next']='Stage 4: player strike outcomes on bound NPCs (mission), client combat retirement, partner perception (guards engage both players), shared death/checkpoint on missions, tutorial-modal tolerance.'
wr(R/'work/current-task-state.json',s)
print('ok',len(videos))
