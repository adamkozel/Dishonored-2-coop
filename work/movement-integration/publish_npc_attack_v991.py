from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';A=W/'candidate-npc-combat-v988'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take in ('npc-native-attack-v989','npc-native-death-v990'):
    T=N/take;r=rd(T/'result.json');v=rd(T/'verified.json')
    assert r['passed'] and v['passed'] and r['candidate']==pin['dll'] and rd(T/'restoration.json')['passed']
    if take.endswith('989'):
        h=rd(T/'native-hit-result.json');d=rd(T/'duplicate.json')
        assert h['host']['event']==h['host']['acked']==h['client']['applied']==1 and h['host']['pending']==0
        assert h['player']['current']==41 and h['avatar']['current']==40 and d['health']['current']==41 and d['attack']['applied']==1 and d['attack']['duplicates']==1
        review='Client six frames inspected: native approaching enemy, red damage effect and lower actual player health, then scripted retreat. Host raw diagnostic not visually reviewed.'
        result=dict(actualPlayerBefore=56,actualPlayerAfter=41,cloneHealth=40,duplicateApplied=False)
    else:
        pair=rd(T/'death-pair.json');reload=rd(T/'reload-completed.json');fresh=rd(T/'rearmed-attack.json');before=rd(T/'before-death.json')
        assert pair['client']['health']['current']==0 and pair['host']['health']['current']==56 and pair['client']['attack']['applied']==4
        for role in ('host','client'):
            assert pair[role]['bridge']['localGameLogicState']==13 and pair[role]['reset']['deathReady'] and pair[role]['reset']['peerDeathReady']
            assert reload[role]['reset']['loads']==before[role]['reset']['loads']+1 and reload[role]['reset']['completed']==before[role]['reset']['completed']+1
            assert not reload[role]['attack']['enabled'] and fresh[role]['nonce']>before[role]['attack']['nonce'] and fresh[role]['applied']==0
        assert rd(T/'capture-ended-before-reload.json')['passed']
        review='Both six-frame sheets inspected: client native 56/41/26/11/0 health sequence and death fall; host stays in safe room then enters native Corvo Game Over; client native Emily Game Over. Native wall-time capture. Retry is validated by state evidence after recording ends.'
        result=dict(actualPlayerHealthSequence=[56,41,26,11,0],applications=4,hostHealth=56,loadsEach=1,explicitAttackRearm=True,postLethalContact='Host emitted event5 during death animation; client already at0 refused a further native application. No extra subtraction. Graceful terminal acknowledgement remains follow-up work.')
    cases.append(dict(take=take,passed=True,restored=True,scope=v['scope'],result=result))
    for role in ('host','client'):
        path=f'{take}/{role}.mp4';m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0];a,b=map(float,m['avg_frame_rate'].split('/'))
        videos=[x for x in videos if x['video']!=path];videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Native enemy combat / {take} / {role}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2),container_fps=m['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=v['scope'],review=review,candidate=A.name,dll=pin['dll'],runtime_test_passed=True))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'npc-attack-audit-v991.json',dict(at=at,candidate=A.name,dll=pin['dll'],cases=cases,complete=False))
pin.update(scopedEnemySwordRelayVerified=True,scopedEnemySharedDeathVerified=True,runtimeEvidence='../npc-attack-audit-v991.json',scope='Opt-in988 passes989 native enemy sword to actual client 56->41, clone40 and duplicate rejection;990 four hits to0, both native death screens, one shared checkpoint, explicit fresh rearm. Full lifecycle, other damage profiles and block/parry remain unaccepted.');wr(A/'identity.json',pin);wr(N/'runtime-video-manifest.json',videos)
b=rd(R/'work/handoff-perception-bookmark-v987.json');assert json.loads(b['saved']['structuredContent']['result'])['success']
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
    p=R/'work'/name;items=rd(p)
    for e in b['bookmarks']:items=[x for x in items if (x.get('address'),x.get('category'))!=(e['address'],e['category'])];items.append(dict(e,type='Note'))
    wr(p,items)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['NpcCombatContinuation'].update(at=at,candidate=A.name,receipt='work/movement-integration/npc-attack-audit-v991.json',inProgress='Combat lifecycle renewal and native defense investigation.',latest='988 passes989 actual remote-player sword damage and duplicate rejection with clone health preserved.990 four repeated hits cause actual client death, both native death screens and exactly one shared checkpoint, with roles/health restored and explicit fresh attack negotiation.',next='Preserve attack intent through native menus and coordinated checkpoints; handle post-lethal contacts cleanly; validate native block/parry before expanding damage families. Full campaign, physics, UI sessions and performance remain unfinished.');s['MovementExecutionPriority']['current']=s['NpcCombatContinuation']['latest'];wr(R/'work/current-task-state.json',s)
print(json.dumps(dict(recordings=len(videos),audit='npc-attack-audit-v991.json',passed=True)))
