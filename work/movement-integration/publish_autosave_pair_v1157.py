from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C=rd(W/'candidate-npc-combat-v1153/identity.json');assert sha(R/'work/asset-game/dinput8.dll')==C['dll']
plan=[('mission-autosave-pair-v1154',False,'coop_autosave_probe was outside the lab command allowlist (Send-D2Command); restoration hit the known native inventory read flake, repaired by Restore-LabNoteInventory (note-restoration-repair passed).'),
 ('mission-autosave-pair-v1155',False,'Pairing worked (host native autosave mirrored by the client in 1.9 s, both settled at one save); the post-restart check crashed on a harness bug ($p and $P are the same PowerShell variable).'),
 ('mission-autosave-pair-v1156',True,'Pass, both origins: one peer makes a native autosave mid-mission (docks apartment), the other mirrors it natively in 1.6-1.9 s; both walk away; the other peer dies; one shared restart returns BOTH players to their own paired autosave position (<= 0.09 m) 57-60 m from the mission start; exactly one load/completion each, correct characters; pair counters reset and resettle after each restart.')]
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take,passed,scope in plan:
    T=N/take;r=rd(T/'result.json');assert bool(r['passed'])==passed,take
    rest=rd(T/'restoration.json');rep=T/'note-restoration-repair/result.json';assert rest['passed'] or (rep.exists() and rd(rep)['passed']),take
    cases.append(dict(take=take,passed=passed,scope=scope,failure=r.get('failure'),cases=r.get('cases')))
    for f in sorted(T.glob('*.mp4')):
        try:meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True,stderr=subprocess.DEVNULL))['streams'][0]
        except Exception:continue  # cut unfinalised by an aborted take
        a,b=map(float,meta['avg_frame_rate'].split('/'))
        path=f'{take}/{f.name}';role='host' if '-host' in f.name else 'client';final=take=='mission-autosave-pair-v1156'
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Campaign recovery / paired mission autosaves / {take} / {f.name}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,
            review=('Sheet inspected: apartment autosave spot, moved to the docks, death/shared GameOver, native loading, back in the apartment at the paired autosave.' if final else 'Raw native recording.'),
            candidate='candidate-npc-combat-v1153',dll=C['dll'],runtime_test_passed=passed,visual_accepted=final))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'mission-autosave-pair-audit-v1157.json',dict(at=at,candidate='candidate-npc-combat-v1153',dll=C['dll'],harnesses=['../natural-moveset/Test-MissionAutosavePair-v1156.ps1'],cases=cases,
  native=dict(requestSave='0x14065cc50(manager game+0x2e75d8, slotType 3, slotNumber -1 -> rotating autosave slot via 0x140656b80, isMissionStart false, autoRetry true, securityDelay 0); queues one of three asynchronous save requests; gates: map running, savegame_enabled, autosave enabled byte 0x142317d88'),
  findings=['Confirmed runtime: a native RequestSave(slotType 3) issued on the engine thread writes a genuine per-character autosave (rotating SLOT61/62) within about 1.6-1.9 s.','Confirmed runtime: paired autosaves keep both peers\' mission checkpoints at the same moment; a shared restart returns each player to its own paired mid-mission autosave.','The one-peer origin is the coop_autosave_probe developer fixture standing in for a script autosave trigger; a real script trigger (arkKisculeNodeActionAutosave) is observed by the same reader but was not exercised in this take.'],
  complete=False,outstanding=['Exercise a real mission-script autosave trigger crossed by one player.','Partner avatar restoration and door/coin/NPC renewal across mission restarts.','Map transitions, manual save/load, disconnect/rejoin.']))
wr(N/'runtime-video-manifest.json',videos)
C.update(runtimeVerified=True,scopedMissionAutosavePairVerified=True,runtimeEvidence='../mission-autosave-pair-audit-v1157.json');wr(W/'candidate-npc-combat-v1153/identity.json',C)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1153 live: stage 6 on missions - paired native autosaves (one peer saves, the other mirrors with native RequestSave) and shared death/restart returning both players to their own paired mid-mission checkpoint (audits 1152, 1157); stage 5 shared usables; stage 4 mission combat/corpses.'
s['MovementExecutionPriority']['next']='Stage 6: real script autosave trigger, partner avatar + door/coin/NPC renewal across mission restarts, then map transitions; stage 4 dynamic NPC relevance; stage 7 native UI.'
wr(R/'work/current-task-state.json',s)
print('ok',len(videos))
