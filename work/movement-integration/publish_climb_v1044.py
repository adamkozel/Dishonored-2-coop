from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C42=rd(W/'candidate-npc-combat-v1042/identity.json');C39=rd(W/'candidate-npc-combat-v1039/identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==C42['dll']
cases=[]
for take,passed,dll,scope in [
 ('native-climb-eotw-v1041',True,C39['dll'],'Native chain climbing discovery: ordinary interact at script_chain_01_1_8/2xlonger_2 enters alternate-mode state29 arkPlayerStateClimb (1c78498); jump released into harbor water (swimming). State watch failed (probe duration 16000 > 15000 limit); snapshots retained.'),
 ('native-climb-eotw-v1041b',True,C39['dll'],'Chain1: X grab -> state29; up/down move z 15.74 -> 16.04 -> 14.24 inside state29; B does not release. Chain2 placement landed on a slope (Slipping).'),
 ('climb-coop-eotw-v1043',True,C42['dll'],None)]:
    T=N/take;r=rd(T/'result.json');assert r['passed']==passed and rd(T/'restoration.json')['passed']
    if scope is None:
        cl=rd(T/'climb-results.json')['client'];scope=f'Candidate1042: host native chain climbing (state23, protocol72) presented on the client remote Corvo with bank70: {cl["climbing"]}/{cl["samples"]} climbing samples, ascend {cl["ascending"]}, descend {cl["descending"]}, {cl["phases"]} cycle phases; client observer-camera video shows grab, climb, descent and drop. Hand-to-chain contact approximate (ladder stand-in clips).'
    cases.append(dict(take=take,passed=passed,dll=dll,scope=scope))
videos=rd(N/'runtime-video-manifest.json')
for c in cases:
    T=N/c['take']
    for f in sorted(T.glob('*.mp4')):
        path=f"{c['take']}/{f.name}";role='host' if f.name.startswith(('host','chain')) else 'client'
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f"Native chain climbing / {c['take']} / {f.name}",travelling=True,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=c['scope'],review='Client sheet inspected for 1043 (grab, climb, descent, drop); raw otherwise.' ,dll=c['dll'],runtime_test_passed=c['passed']))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'climb-audit-v1044.json',dict(at=at,candidate='candidate-npc-combat-v1042',dll=C42['dll'],protocol=72,bank='climb-banks-v1042 (70/369)',cases=cases,complete=False,
   outstanding=['Chain-specific hand-over-hand acting (ladder-contact stand-ins), grab entry and release/drop transitions (clips 367/368 reserved, not bound).','Emily as climber; ladders in other maps; top exit/mantle from a chain.']))
wr(N/'runtime-video-manifest.json',videos)
p=N/'review-evidence-annotations.json';an=rd(p);it=an['items']['ladder_up_down_entry_exit']
it['runtime_binding']='partial_native_steam_chain_climb_corvo_source_mission_map'
it['notes']='1041/1043: native arkClimbable chain climbing in Edge of the World (ordinary interact -> alternate-mode state29 arkPlayerStateClimb) is sent as action state23 (protocol72); the receiver presents bank70 authored ladder-contact ascend/descend cycles advanced by replicated vertical distance. 1043 passes with client video (grab, climb, descent, drop). Chain hand-over-hand acting, entry/exit transitions, Emily as climber and true ladders remain open.'
it.setdefault('evidence',[]).append('../movement-integration/climb-audit-v1044.json');wr(p,an)
C42.update(runtimeVerified=True,scopedClimbPresentationVerified=True,runtimeEvidence='../climb-audit-v1044.json');wr(W/'candidate-npc-combat-v1042/identity.json',C42)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['MovementExecutionPriority']['current']='1042 live: remote native chain climbing (state23, bank70) verified with video in Edge of the World, on top of swimming (state22) and map-agnostic action stream; missions load natively with intro skip.';wr(R/'work/current-task-state.json',s)
print(json.dumps(dict(recordings=len(videos),cases=[(c['take'],c['passed']) for c in cases])))
