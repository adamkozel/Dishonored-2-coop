from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
cands=['v1102','v1104','v1106','v1108']
C={k:rd(W/f'candidate-npc-combat-{k}/identity.json') for k in cands}
assert sha(R/'work/asset-game/dinput8.dll')==C['v1108']['dll']
names={v['dll']:f'candidate-npc-combat-{k}' for k,v in C.items()}
names[rd(W/'candidate-npc-combat-v1097/identity.json')['dll']]='candidate-npc-combat-v1097'
plan=[('mission-combat-v1101','1097: host faced the guard (RT strikes, RB block); guard health 30 -> 10 on the host only; guard channel retired native_lifecycle_changed; client NPC combat all_bound_actors_retired; replica diverged.'),
 ('mission-combat-v1103','1102 diagnostic: lifecycleDiff 0x40000200 - decision AI behaviour tree swapped into combat (0x200), then the guard died (not living).'),
 ('mission-combat-v1105','1104 (tree swap tolerated on ongoing channels): channel survived until death; host combat refused the first hit as hit_damage_parameters - native player_kick.damage (inflictor = player, not the sword) was an unbound family.'),
 ('mission-combat-v1107','1106 (Kick family): still refused - kick hit has a zero normal, rejected by the unit-normal melee geometry rule; channel survived (diff 0).'),
 ('mission-combat-v1109','1108 (kick geometry rule): host committed 2 outcomes (kick + sword), client applied 2; guard health 30 -> 10 on BOTH peers; NPC combat active on both; host native block (RB) took chip damage only.')]
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take,scope in plan:
    T=N/take;r=rd(T/'result.json');assert rd(T/'restoration.json')['passed'],take
    ident=rd(T/'identity.json');dll=ident['dll']
    combat={q:{k:v for k,v in rd(T/f'after-npc-combat-{q}.json').items() if k in ('result','committed','applied','deaths','rejected')} for q in ('host','client') if (T/f'after-npc-combat-{q}.json').exists()}
    cases.append(dict(take=take,candidate=names.get(dll,dll),scope=scope,combat=combat,timeline=rd(T/'timeline.json') if (T/'timeline.json').exists() else None))
    for f in sorted(T.glob('*.mp4')):
        path=f'{take}/{f.name}';role='host' if '-host' in f.name else 'client'
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Stage 4 / mission combat outcomes / {take} / {f.name}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review='Raw native recording.',candidate=names.get(dll,dll),dll=dll,runtime_test_passed=take=='mission-combat-v1109',requirements=(['kick_or_shove_if_present'] if take=='mission-combat-v1109' else [])))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'mission-outcome-audit-v1110.json',dict(at=at,candidate='candidate-npc-combat-v1108',dll=C['v1108']['dll'],npcCombatWire=7,harnesses=['../natural-moveset/Test-MissionCombat-v1109.ps1'],cases=cases,
  findings=['Confirmed runtime: entering combat swaps the native decision behaviour tree; ongoing NPC channels must not treat that as a new incarnation (1104; acquisition stays strict).','Confirmed runtime: the native player kick is player_kick.damage (amount10, type0x80, reactions1/1) with the kicking player as inflictor and a zero hit normal (region1/regions2).','Confirmed runtime (stage 4 acceptance slice): on a mission map the host player kick and sword hit on a host-authoritative guard each commit once and apply once on the client; guard health converges (30 -> 10 on both peers).'],
  complete=False,outstanding=['Client-sourced hits (client player striking a bound NPC) on missions.','Guard death/body replication on missions (1105 kill retired the channel before a death outcome).','Guards engaging the client player (partner perception) and enemy hits on the client relayed as player damage.','Shared death/checkpoint on missions; tutorial-modal tolerance.']))
wr(N/'runtime-video-manifest.json',videos)
p=N/'review-evidence-annotations.json';an=rd(p)
it=an['items']['kick_or_shove_if_present']
it['runtime_binding']='partial_native_steam_player_kick_outcome_host_source_mission_map'
it['notes']=(it.get('notes') or '')+' 1109 (candidate1108, Edge of the World): the native player kick (player_kick.damage, zero-normal hit) on a host-authoritative guard is captured once on the host and applied once on the client; guard health converges. Kick body presentation on the remote player and client-sourced kicks remain open.'
it.setdefault('evidence',[]).append('../movement-integration/mission-outcome-audit-v1110.json');wr(p,an)
C['v1108'].update(runtimeVerified=True,scopedMissionCombatOutcomeVerified=True,runtimeEvidence='../mission-outcome-audit-v1110.json');wr(W/'candidate-npc-combat-v1108/identity.json',C['v1108'])
for k in ('v1102','v1104','v1106'):
    C[k].update(runtimeVerified=False,result='intermediate step in the 1102-1108 mission combat outcome series; see audit',runtimeEvidence='../mission-outcome-audit-v1110.json');wr(W/f'candidate-npc-combat-{k}/identity.json',C[k])
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1108 live: stage 4 on missions - host player kick/sword outcomes on a host-authoritative guard commit once and apply once on the client (health converges); NPC authority survives combat (tree swap) with pose bandwidth budget.'
s['MovementExecutionPriority']['next']='Stage 4: client-sourced hits, NPC death/body outcome before channel retirement, guards engaging both players (partner perception + relayed player damage), shared death/checkpoint on missions.'
wr(R/'work/current-task-state.json',s)
print('ok',len(videos))
