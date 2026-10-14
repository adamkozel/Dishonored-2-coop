from pathlib import Path
import datetime, json, subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take in ('npc-native-perception-v976','npc-native-perception-v978','npc-native-perception-v981','npc-native-perception-v982'):
    T=N/take;r=rd(T/'result.json');restore=rd(T/'restoration.json');assert not r['passed'] and restore['passed']
    hits=[{k:e[k] for k in ('tick','handle','attackerHandle','inflictorHandle','damageAmount','before','after')} for e in rd(T/'damage-host.json')['events']]
    cases.append(dict(take=take,passed=False,restored=True,failure=r['failure'],nativeDamage=hits))
    scope='Failed sustained target selection of the specifically watched escape guard; lab restoration passes. No perception/combat acceptance.'
    if take.endswith('982'):scope+=' Diagnostic freezes remote follow after arrival; separate native enemies Ramsey and escape guard2 strike clone40-to20-to0. Does not prove continuously moving perception or relay damage to client player.'
    for role in ('host','client'):
        path=f'{take}/{role}.mp4';meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        review='Not yet visually reviewed; raw failed diagnostic recording.'
        if take.endswith('982') and role=='client':review='Six inspected frames show native enemies approach, swing, and withdraw after clone death; actual client health remains unchanged. No full paired combat acceptance.'
        if take.endswith('976') and role=='client':review='Four inspected frames show guard/Ramsey idle; scripted encounter may not yet be active, so this alone cannot establish clone blindness.'
        videos=[v for v in videos if v['video']!=path];videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'NPC perception diagnostic / {take} / {role}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2),container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review=review,candidate='candidate-npc-combat-v966',dll=r['candidate'],runtime_test_passed=False))
b=rd(R/'work/handoff-perception-bookmark-v979.json');assert json.loads(b['saved']['structuredContent']['result'])['success']
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
    p=R/'work'/name;items=rd(p)
    for e in b['bookmarks']:items=[x for x in items if (x.get('address'),x.get('category'))!=(e['address'],e['category'])];items.append(dict(e,type='Note'))
    wr(p,items)
wr(N/'runtime-video-manifest.json',videos)
at=datetime.datetime.now().astimezone().isoformat();wr(W/'npc-perception-investigation-v983.json',dict(at=at,cases=cases,unnecessaryExperiment='candidate-npc-combat-v973',verifiedRollback='npc-perception-recovery-v975.json',currentExperiment='candidate-npc-combat-v983',currentFallback='candidate-npc-combat-v966',complete=False))
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['NpcCombatContinuation'].update(at=at,details='documentation/handoff-2026-10-11/77-native-remote-perception.md',inProgress='Candidate983 opt-in native Observable retention under runtime984 evaluation; fallback966 preserved. No native vision setting, faction, local controls or original assets changed.',latest='Combat972 accepted two-source contacts; unnecessary faction experiment973 rolled back975. Native clone bounds/physics correct. Target-selection failures976/978/981 retained. Frozen-follow982 remains failed watched-guard test but two other enemies hit clone40-to20-to0; automatic restoration passed.',next='Validate same-area continuous-transform Observable retention; then native enemy-to-player outcomes. General moving perception, lifecycle, active combat and complete co-op remain unfinished.')
s['MovementExecutionPriority']['current']=s['NpcCombatContinuation']['latest'];wr(R/'work/current-task-state.json',s)
print(json.dumps(dict(videos=len(videos),failedDiagnostics=len(cases),complete=False)))
