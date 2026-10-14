from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';A=W/'candidate-npc-combat-v1002'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
videos=rd(N/'runtime-video-manifest.json');cases=[]
for num,candidate in ((993,992),(995,994),(998,997),(999,997),(1003,1002),(1004,1002)):
    take=f'npc-native-{"block" if num in (999,1004) else "death"}-v{num}';T=N/take;localPin=rd(W/f'candidate-npc-combat-v{candidate}'/'identity.json');r=rd(T/'result.json')
    assert r['candidate']==localPin['dll'] and rd(T/'restoration.json')['passed']
    if num in (993,995,998,999):
        assert not r['passed'];scope=('Failed native experiment: '+r['failure']+'. Full isolated restoration passed; inspect its evidence for the last established stage.');summary=dict(failure=r['failure'])
    elif num==1003:
        assert r['passed'];v=rd(T/'verified.json');assert v['passed'];scope=v['scope']
        pair=rd(T/'death-pair.json');fresh=rd(T/'rearmed-attack.json');off=rd(T/'one-sided-stop.json');old=rd(T/'before-death.json');reloaded=rd(T/'reload-completed.json')
        assert pair['host']['attack']['event']==pair['client']['attack']['applied'] and pair['client']['health']['current']==0 and pair['host']['health']['current']==56
        for role in ('host','client'):
            assert fresh[role]['session']['renewals']==1 and fresh[role]['nonce']>old[role]['attack']['nonce'] and fresh[role]['applied']==0
            assert reloaded[role]['reset']['loads']==old[role]['reset']['loads']+1 and reloaded[role]['reset']['completed']==old[role]['reset']['completed']+1
            assert not off[role]['enabled'] and not off[role]['session']['enabled']
        assert off['client']['result']=='peer_stopped'
        summary=dict(applications=pair['client']['attack']['applied'],hostHealth=56,clientHealth=0,renewalsEach=1,oneSidedStop=True)
    else:
        scope=rd(T/'verified.json')['scope'] if r['passed'] else 'Native held-block experiment failed its acceptance assertion; inspect failure and evidence. Restoration passed.'
        summary=dict(failure=r.get('failure'))
        if (T/'defense-results.json').exists():
            d=rd(T/'defense-results.json');summary.update(protectedZeroDamage=len(d['blocked']),nativeBlockSamples=d['nativeBlockSamples'],playerHealth=d['health']['current'])
    cases.append(dict(take=take,candidate=candidate,passed=r['passed'],restored=True,scope=scope,result=summary))
    for role in ('host','client'):
        path=f'{take}/{role}.mp4';meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        review='Raw native diagnostic, visual review not yet entered.'
        note=T/f'{role}-visual-review.txt'
        if note.exists():review=note.read_text()
        videos=[x for x in videos if x['video']!=path];videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Native combat lifecycle / {take} / {role}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2),container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review=review,candidate=f'candidate-npc-combat-v{candidate}',dll=localPin['dll'],runtime_test_passed=r['passed']))
at=datetime.datetime.now().astimezone().isoformat();wr(W/'npc-recovery-audit-v1006.json',dict(at=at,candidate=A.name,dll=pin['dll'],cases=cases,complete=False));wr(N/'runtime-video-manifest.json',videos)
block=cases[-1]['passed'];pin.update(scopedEnemySwordRelayVerified=True,scopedEnemySharedDeathAutoRenewVerified=True,scopedHeldBlockVerified=block,runtimeEvidence='../npc-recovery-audit-v1006.json',scope='1002 native terminal result and automatic coordinated-checkpoint attack renewal pass1003; one-sided stop passes. Held-block1004 result is recorded separately. Menu renewal, timed parry/attacker response and broader combat remain incomplete.');wr(A/'identity.json',pin)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['NpcCombatContinuation'].update(at=at,candidate=A.name,receipt='work/movement-integration/npc-recovery-audit-v1006.json',details='documentation/handoff-2026-10-11/79-enemy-combat-recovery-and-defense.md',latest=f'1002 passes1003 native terminal outcome, both death screens, one coordinated retry, automatic fresh attack renewal and one-sided stop. Failed993/995/998/999 retained and restored. Held-block1004 passed={block}.',inProgress='Continue native defense and combat session lifecycle.',next='Native held-block/timed parry and attacker response, menu continuity, live NPC membership changes, other damage families and broader campaign. Full playable co-op remains unfinished.');s['MovementExecutionPriority']['current']=s['NpcCombatContinuation']['latest'];wr(R/'work/current-task-state.json',s)
print(json.dumps(dict(recordings=len(videos),cases=cases),indent=2))
