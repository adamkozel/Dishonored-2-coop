from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
cands=['v1147','v1147b']
C={k:rd(W/f'candidate-npc-combat-{k}/identity.json') for k in cands}
assert sha(R/'work/asset-game/dinput8.dll')==C['v1147b']['dll']
names={v['dll']:f'candidate-npc-combat-{k}' for k,v in C.items()}
names[rd(W/'candidate-npc-combat-v1123e/identity.json')['dll']]='candidate-npc-combat-v1123e'
plan=[
 ('mission-death-v1146',True,'Discovery (1123e, checkpoint sync off): the host dies by a native fall on Edge of the World; native GameOver "Restart From Last Checkpoint" issues RestartFromCheckpoint (kind 0), which caches the last autosave slot (61, the mission-start autosave), then RestartFromMemoryCheckpoint (kind 1) accepted for that slot.'),
 ('mission-checkpoint-v1148',False,'1147: arm refused to pair: the client last autosave was still its Dunwall autosave (mission-start autosave not yet written) and the autosave write generation differed by timing -> 1147b.'),
 ('mission-checkpoint-v1149',False,'1147b: armed after re-arm; shared death reached; restart aborted (host timeout, client peer_aborted) while the client sat at the load prompt; the harness pressed A on a shared timer and lost its timeline on failure. Cleanup could not disable the client session (load boundary pending); manual recovery: A on the client, sessions off, B out of pause, Recover-InterruptedTake (attempt 1 failed on the pause menu, kept as recovery-attempt1.json; attempt 2 passed).'),
 ('mission-checkpoint-v1150',False,'Diagnostic rerun: shared death -> both native GameOver -> host restart -> both loading -> both held at the barrier -> released -> gameplay in about 8 s; each peer loaded its own slot 61 at its exact spawn; usable session renewed. Marked failed only by a cumulative count check that included 1149\'s abort.'),
 ('mission-checkpoint-v1151',True,'Pass, two cycles: host death + host restart request, then client death + client restart request (the other peer presses as a duplicate). Each cycle: both native GameOver screens, exactly one load and one completion per peer, no abort, new world epochs, correct characters (1/2), exact mission-start spawn, usable session renewed after the shared checkpoint.')]
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take,passed,scope in plan:
    T=N/take;r=rd(T/'result.json');assert bool(r['passed'])==passed,take
    rest=rd(T/'restoration.json');rec=T/'recovery.json'
    assert rest['passed'] or (rec.exists() and rd(rec)['passed']),take
    dll=rd(T/'identity.json')['dll']
    cases.append(dict(take=take,passed=passed,candidate=names.get(dll,dll),scope=scope,failure=r.get('failure'),cases=r.get('cases'),restoration=rest['passed'],recovery=rd(rec)['passed'] if rec.exists() else None))
    for f in sorted(T.glob('*.mp4')):
        try:meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True,stderr=subprocess.DEVNULL))['streams'][0]
        except Exception:continue  # 1149 recordings were cut unfinalised by cleanup
        a,b=map(float,meta['avg_frame_rate'].split('/'));path=f'{take}/{f.name}';role='host' if '-host' in f.name else 'client';final=take=='mission-checkpoint-v1151'
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Campaign recovery / shared mission death and checkpoint / {take} / {f.name}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,
            review=('Sheet inspected: both cycles show the fall or shared native GameOver for this character, the native Edge of the World loading screen and the return to the docks spawn.' if final else 'Raw native recording.'),
            candidate=names.get(dll,dll),dll=dll,runtime_test_passed=passed,visual_accepted=final))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'mission-checkpoint-audit-v1152.json',dict(at=at,candidate='candidate-npc-combat-v1147b',dll=C['v1147b']['dll'],harnesses=['../natural-moveset/Probe-MissionDeath-v1146.ps1','../natural-moveset/Test-MissionCheckpoint-v1151.ps1'],cases=cases,
  native=dict(commands={'RestartFromCheckpoint':'0x65da50 (Restart from the last save)','RestartFromMemoryCheckpoint':'0x65db00','LoadGame':'0x658d00','SaveGame':'0x65f520 (quick save)'},requestSave='0x14065cc50(manager=game+0x2e75d8, slotType, slotNumber, isMissionStart, autoRetry, securityDelay); slotType 1 quick, 2 manual/mission start, 3 autosave',lastAutosaveSlot='game+0x2e75d8+0x1c',autosaveNode='arkKisculeNodeActionAutosave (vtable 0x141b7c9e8), mission-script driven',slots='autosaves rotate SLOT61/SLOT62 per instance in the isolated fs_savepath; game.details holds mapName/chapter/mission/character/slotType'),
  findings=['Confirmed runtime (1146): on a mission, native GameOver restart = RestartFromCheckpoint -> last autosave slot (cached) -> memory continuation; each instance owns its own per-character autosaves.',
   'Confirmed runtime (1151): mission checkpoint mode (1147b) turns either player\'s native death into a shared death (both native GameOver screens), deduplicates restart presses, and restarts each peer exactly once from its own mission autosave behind the existing both-ready barrier; usable sessions renew after the shared checkpoint.',
   'Identity: autosave mapName/chapter/mission of the current map; arming waits for each peer\'s mission-start autosave.'],
  complete=False,outstanding=['Mid-mission autosaves are per-player script events: a peer that crossed an autosave trigger alone holds a later checkpoint than the other (identity still matches by map). Pairing them needs the native RequestSave (slotType 3) issued on the peer when the host autosaves (hypothesis, not yet built).','Partner avatar (probe follow) restoration after a mission restart was not armed in these takes.','Door/coin/NPC stream renewal across a mission restart not yet exercised (usable renewal verified).','Map transitions between missions, manual save/load selection, disconnect/rejoin.']))
wr(N/'runtime-video-manifest.json',videos)
C['v1147b'].update(runtimeVerified=True,scopedMissionCheckpointVerified=True,runtimeEvidence='../mission-checkpoint-audit-v1152.json');wr(W/'candidate-npc-combat-v1147b/identity.json',C['v1147b'])
C['v1147'].update(runtimeVerified=False,result='superseded by 1147b (arm timing); see audit',runtimeEvidence='../mission-checkpoint-audit-v1152.json');wr(W/'candidate-npc-combat-v1147/identity.json',C['v1147'])
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1147b live: stage 6 on missions - shared native death and one shared restart from each peer own mission autosave (both roles as source, audit 1152); stage 5 shared usables (1123e); stage 4 mission combat/corpses (1113d).'
s['MovementExecutionPriority']['next']='Stage 6: paired mid-mission autosaves (native RequestSave slotType 3 on the peer), partner avatar and door/coin/NPC renewal across mission restarts; then map transitions; stage 4 dynamic NPC relevance.'
wr(R/'work/current-task-state.json',s)
print('ok',len(videos))
