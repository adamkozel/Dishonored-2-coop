from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
cands=['v1153b','v1153c'];C={k:rd(W/f'candidate-npc-combat-{k}/identity.json') for k in cands}
assert sha(R/'work/asset-game/dinput8.dll')==C['v1153c']['dll']
names={v['dll']:f'candidate-npc-combat-{k}' for k,v in C.items()};names[rd(W/'candidate-npc-combat-v1153/identity.json')['dll']]='candidate-npc-combat-v1153'
plan=[('mission-renewal-v1158',False,'1153: with partner avatar, door/coin/usable sessions and NPC stream armed, the shared restart completed but every renewal stayed waiting_shared_checkpoint: prepareCheckpointPresentation still required the Dunwall map, the avatar step was refused, and renewal was gated on avatar success -> 1153b. Door sync had already retired all channels before the death.'),
 ('mission-renewal-v1159',False,'1153b: pre-death readiness check: all 16 client door channels retired with world_input_or_connection_scope_changed (one transient blip) while the host stayed converged -> 1153c (door channels pause on transient gaps, as usables 1123c/d).'),
 ('mission-renewal-v1160',True,'Pass (1153c), two cycles (host death, then client death): after each shared restart the partner avatar is restored (2/2, none refused), door session renewed with 16 converged doors, coin sync re-armed and settled again before the next death, usable session renewed, NPC stream renewed (9 bound NPCs); door sessions absorbed 48-224 transient pauses without retiring.')]
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take,passed,scope in plan:
    T=N/take;r=rd(T/'result.json');assert bool(r['passed'])==passed,take;assert rd(T/'restoration.json')['passed'],take
    dll=rd(T/'identity.json')['dll'];cases.append(dict(take=take,passed=passed,candidate=names.get(dll,dll),scope=scope,failure=r.get('failure')))
    for f in sorted(T.glob('*.mp4')):
        try:meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True,stderr=subprocess.DEVNULL))['streams'][0]
        except Exception:continue
        a,b=map(float,meta['avg_frame_rate'].split('/'));path=f'{take}/{f.name}';role='host' if '-host' in f.name else 'client';final=take=='mission-renewal-v1160'
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Campaign recovery / world renewal across mission restart / {take} / {f.name}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,
            review=('Host sheet inspected: after both shared restarts the Emily partner avatar stands on the docks in front of Corvo.' if final and role=='host' else ('Client sheet inspected: death/GameOver/loading/return visible; the Corvo partner is not in frame, so client-side partner visibility is unconfirmed (status reports restored).' if final else 'Raw native recording.')),
            candidate=names.get(dll,dll),dll=dll,runtime_test_passed=passed,visual_accepted=final and role=='host'))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'mission-renewal-audit-v1161.json',dict(at=at,candidate='candidate-npc-combat-v1153c',dll=C['v1153c']['dll'],harnesses=['../natural-moveset/Test-MissionRenewal-v1160.ps1'],cases=cases,
  findings=['Confirmed runtime: after a shared mission restart the partner avatar, door session, coin sync, usable session and NPC stream all renew on both peers (both roles dying).','Fixes: partner presentation restore has no map gate (1153b); renewal no longer waits on a refused presentation step (1153b); door channels pause on transient context gaps (1153c).'],
  complete=False,outstanding=['Client-side partner visibility after restart not visually confirmed.','Map transitions between missions, manual save/load selection, disconnect/rejoin.']))
wr(N/'runtime-video-manifest.json',videos)
C['v1153c'].update(runtimeVerified=True,scopedMissionRenewalVerified=True,runtimeEvidence='../mission-renewal-audit-v1161.json');wr(W/'candidate-npc-combat-v1153c/identity.json',C['v1153c'])
C['v1153b'].update(runtimeVerified=False,result='intermediate (door blips); see audit',runtimeEvidence='../mission-renewal-audit-v1161.json');wr(W/'candidate-npc-combat-v1153b/identity.json',C['v1153b'])
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1153c live: stage 6 on missions - paired autosaves, shared death/restart, and renewal of partner avatar, doors, coins, usables and NPC stream after each restart (audits 1152, 1157, 1161).'
s['MovementExecutionPriority']['next']='Stage 6: map transitions between missions (shared mission end/next mission load), manual save/load; stage 4 dynamic NPC relevance; stage 5 levers/multi-state; stage 7 native UI.'
wr(R/'work/current-task-state.json',s)
print('ok',len(videos))
