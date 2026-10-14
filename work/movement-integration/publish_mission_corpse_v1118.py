from pathlib import Path
import json,hashlib,datetime,subprocess,math
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
cands=['v1113','v1113b','v1113c','v1113d']
C={k:rd(W/f'candidate-npc-combat-{k}/identity.json') for k in cands}
assert sha(R/'work/asset-game/dinput8.dll')==C['v1113d']['dll']
names={v['dll']:f'candidate-npc-combat-{k}' for k,v in C.items()}
plan=[('mission-combat-v1114','1113: NPC combat pre-death release stopped the stream channel (combat_native_death) before corpse mode; corpses 0.90 m apart.'),
 ('mission-combat-v1115','1113b (channel kept as corpse): client re-acquired AI on the next host hello -> ai_acquire_failed; corpses 1.08 m apart.'),
 ('mission-combat-v1116','1113c (no re-acquisition for corpses): corpses matched exactly (0.0 m) during the 8 s corpse stream; releasing the client ragdoll moved it 0.86 m then 0.27 m.'),
 ('mission-combat-v1117','1113d (corpse stays host-streamed for the stream lifetime): client-sourced native fatality, one death on both peers, corpses identical (0.0 m) for the whole take; client video: kill, collapse, natural slumped corpse against the railing.')]
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take,scope in plan:
    T=N/take;assert rd(T/'restoration.json')['passed'],take
    ident=rd(T/'identity.json');dll=ident['dll'];tl=rd(T/'timeline.json')
    dev=[round(math.dist(r['guard_host']['position'],r['guard_client']['position']),3) for r in tl]
    cases.append(dict(take=take,candidate=names.get(dll,dll),scope=scope,corpseDeviationM=dev))
    for f in sorted(T.glob('*.mp4')):
        path=f'{take}/{f.name}';role='host' if '-host' in f.name else 'client';final=take=='mission-combat-v1117'
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Stage 4 / corpse authority / {take} / {f.name}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review=('Client sheet inspected: kill, collapse, natural slumped corpse.' if final and role=='client' else 'Raw native recording.'),candidate=names.get(dll,dll),dll=dll,runtime_test_passed=final,visual_accepted=final and role=='client',requirements=(['death_variants','ragdoll_handoff'] if final and role=='client' else [])))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'mission-corpse-audit-v1118.json',dict(at=at,candidate='candidate-npc-combat-v1113d',dll=C['v1113d']['dll'],harnesses=['../natural-moveset/Test-MissionCombat-v1117.ps1'],cases=cases,
  findings=['Confirmed runtime: after a shared native death each world settles its own ragdoll (0.9-1.4 m apart without authority).','1113d: the bound NPC channel becomes a corpse stream (pose health 0 allowed; pose health is never applied - death only comes from the native combat outcome); the client holds its corpse animated (native physics 736ef0) and applies host corpse poses; releasing to ragdoll mid-session (734d90/734a10) moves it, so release happens only at channel stop.','Confirmed runtime: client-sourced native fatality on a mission guard yields one death and an identical corpse (0.0 m) on both peers.'],
  complete=False,outstanding=['Corpse streams persist for the stream lifetime (bandwidth bounded by the 1097 budget); dynamic relevance/rebinding should retire distant corpses.','Physics interaction with client corpses (bumping/explosions) while held animated; body carry of a streamed corpse.','Guards engaging the client player (partner perception) and relayed player damage; shared death/checkpoint on missions.']))
wr(N/'runtime-video-manifest.json',videos)
p=N/'review-evidence-annotations.json';an=rd(p)
for key,binding,note in (('death_variants','partial_native_steam_npc_death_and_corpse_both_peers_mission_map',' 1117 (candidate1113d): a client-sourced native fatality on a host-authoritative mission guard yields one death on both peers and an identical corpse (0.0 m), natural slumped pose on the client. Player-character death variants remain separate.'),
                         ('ragdoll_handoff','partial_native_steam_npc_corpse_authority_mission_map',' 1113d/1117: after a shared NPC death the host streams the corpse while the client holds its ragdoll animated (native physics mode switch) at the host pose; ragdoll release happens at channel stop. Player-character ragdoll and carry handoff remain open.')):
    if key in an['items']:
        it=an['items'][key];it['runtime_binding']=binding;it['notes']=(it.get('notes') or '')+note;it.setdefault('evidence',[]).append('../movement-integration/mission-corpse-audit-v1118.json')
wr(p,an)
C['v1113d'].update(runtimeVerified=True,scopedMissionCorpseAuthorityVerified=True,runtimeEvidence='../mission-corpse-audit-v1118.json');wr(W/'candidate-npc-combat-v1113d/identity.json',C['v1113d'])
for k in ('v1113','v1113b','v1113c'):
    C[k].update(runtimeVerified=False,result='intermediate step in the 1113 corpse authority series; see audit',runtimeEvidence='../mission-corpse-audit-v1118.json');wr(W/f'candidate-npc-combat-{k}/identity.json',C[k])
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1113d live: stage 4 on missions - host/client player hits on a host-authoritative guard commit once and apply once; shared deaths; corpses identical on both peers via corpse authority; NPC authority survives combat with pose budget.'
s['MovementExecutionPriority']['next']='Stage 4: guards engaging both players (partner perception on missions + relayed player damage), dynamic NPC relevance, shared death/checkpoint on missions (stage 6 dependency).'
wr(R/'work/current-task-state.json',s)
print('ok',len(videos))
