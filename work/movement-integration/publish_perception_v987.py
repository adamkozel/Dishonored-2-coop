from pathlib import Path
import datetime,hashlib,json,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';A=W/'candidate-npc-combat-v983'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
cases=[];videos=rd(N/'runtime-video-manifest.json')
for number in (984,985,986):
    take=f'npc-native-perception-v{number}';T=N/take;r=rd(T/'result.json');assert r['candidate']==pin['dll'] and rd(T/'restoration.json')['passed']
    assert r['passed']==(number!=984)
    ap=rd(T/'perception-after-host.json');assert ap['enabled'] and ap['installed'] and ap['avatarAiHeld'] and ap['retained']>100 and ap['refused']==0
    samples=rd(T/'native-samples.json');avatar=samples[0]['bridge']['probeActorHandle'];targets=sorted({t['handle'] for x in samples for t in x['allTargets'] if t['combat']['resolved'] and t['combat']['handle']==avatar})
    assert all(x['bridge']['probeFollowEnabled'] for x in samples)
    if number!=984:assert len(targets)>=2 and rd(T/'verified.json')['selectedSamples']>0
    movement=None
    if number==986:
        movement=rd(T/'moving-perception.json');assert movement['maxDistance']>=2 and movement['selectedAfterMoving']>=2
    review='Raw diagnostic not yet visually reviewed.'
    scope='Experimental983 native targeting with owned continuous-transform observable retention; no faction override. Both games restored. No enemy damage relay or full lifecycle acceptance.'
    if number==984:scope='Failed harness: strict Array.Contains type comparison produced zero watched handles; one real guard-to-clone hit recorded, but target-selection assertion invalid. Retained as failed; cleanup passed.'
    if number==986:review='Six inspected client frames show guard pursuit while player backs diagonally away about7m, close combat approach then withdrawal after clone death. Source FPS overlay drops during combat; capture is native wall time. No client-health relay or paired-body polish acceptance.'
    cases.append(dict(take=take,passed=r['passed'],restored=True,selectedEnemies=targets,retention=ap,movement=movement,scope=scope))
    for role in ('host','client'):
        path=f'{take}/{role}.mp4';m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0];a,b=map(float,m['avg_frame_rate'].split('/'))
        videos=[v for v in videos if v['video']!=path];videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'NPC native pursuit / {take} / {role}',travelling=number==986,gameplay_bound=True,actual_capture_hz=round(a/b,2),container_fps=m['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review=review if role=='client' else 'Raw observer view not yet visually reviewed.',candidate=A.name,dll=pin['dll'],runtime_test_passed=r['passed']))
at=datetime.datetime.now().astimezone().isoformat();wr(W/'npc-perception-audit-v987.json',dict(at=at,candidate=A.name,dll=pin['dll'],cases=cases,complete=False))
pin.update(runtimeVerified=False,scopedRemoteNativeTargetingVerified=True,scopedMovingNativeTargetingVerified=True,runtimeEvidence='../npc-perception-audit-v987.json',scope='Opt-in983 observed native target selection985 and target retention during about7m ordinary diagonal retreat986. No enemy-to-player relay, arbitrary area/lifecycle or full combat acceptance. Fallback966 retained.');wr(A/'identity.json',pin);wr(N/'runtime-video-manifest.json',videos)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['NpcCombatContinuation'].update(at=at,candidate=A.name,receipt='work/movement-integration/npc-perception-audit-v987.json',inProgress='Implementing native enemy sword-hit routing from the owned clone to the actual remote player; existing fixed-guard prototype is insufficient.',latest='983 scoped Observable retention passes985 native target selection by Ramsey/escape guard2 and986 retained targeting during about7m backward/sideways ordinary movement. 984 failed logging retained; all restore. Native clone health currently absorbs enemy hits independently; actual remote player damage remains unfinished.',next='Generalize host enemy-to-player contacts over existing NPC bindings; native local-player damage application, duplicate/stale rejection, shared death recovery. Also validate perception across menus/checkpoints/areas and sustain performance.');s['MovementExecutionPriority']['current']=s['NpcCombatContinuation']['latest'];wr(R/'work/current-task-state.json',s)
print(json.dumps(dict(recordings=len(videos),cases=cases),indent=2))
