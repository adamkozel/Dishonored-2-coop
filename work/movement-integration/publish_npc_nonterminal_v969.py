"""Publish completed nonterminal investigations, including failures and recovery."""
from pathlib import Path
import datetime, hashlib, json, subprocess
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'
def rd(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v): p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f: return hashlib.file_digest(f,'sha256').hexdigest().upper()
A=W/'candidate-npc-combat-v966'; pin=rd(A/'identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']==sha(A/'dinput8.dll')
assert sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==pin['probe']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file(): assert sha(p)==sha(A/'src'/p.name),p
matrix=[('npc-crossbow-body-v960',954,False,True),('npc-crossbow-body-v961',954,False,False),('npc-crossbow-body-v965',954,False,True),('npc-crossbow-exact-body-v967',966,True,True),('npc-crossbow-exact-body-v968',966,False,True)]
videos=rd(N/'runtime-video-manifest.json'); cases=[]
for name,build,passed,restored in matrix:
    T=N/name; result=rd(T/'result.json'); cleanup=rd(T/'restoration.json'); cp=rd(W/f'candidate-npc-combat-v{build}/identity.json')
    assert result['passed']==passed and cleanup['passed']==restored and result['candidate']==cp['dll'],name
    c=dict(take=name,candidate=f'candidate-npc-combat-v{build}',dll=cp['dll'],source=result['source'],passed=passed,restorationPassed=restored,failure=result['failure'])
    d=rd(T/'contact-0.json')
    c.update(health={r:d[r]['health']['current'] for r in ('host','client')},deaths={r:d[r]['combat']['deaths'] for r in ('host','client')})
    assert c['health']['host']==c['health']['client']
    if passed:
        assert name.endswith('v967') and c['health']['host']==25 and not any(c['deaths'].values())
        before=rd(T/'before-duplicate.json'); dup=rd(T/'duplicate-latest.json'); old=rd(T/'reload-before.json'); new=rd(T/'reload-completed.json'); fresh=rd(T/'rearmed-combat.json')
        assert d['host']['combat']['committed']==d['client']['combat']['applied']==1
        for role in ('host','client'):
            combat=d[role]['combat']; assert combat['enabled'] and not combat['nativeLethalSubtractions'] and not combat['preDeathReleased'] and not combat['pendingRequests'] and not combat['fixture']['lost']
            assert dup[role]['health']['current']==25 and not dup[role]['combat']['deaths']
            for field in ('committed','applied'): assert dup[role]['combat'][field]==before[role]['combat'][field]
            assert dup[role]['powerFx']['started']==before[role]['powerFx']['started']
            assert dup[role]['combat']['duplicates']>before[role]['combat']['duplicates']
            assert new[role]['reset']['completed']==old[role]['reset']['completed']+1 and not new[role]['combat']['enabled']
            assert fresh[role]['enabled'] and fresh[role]['nonce']>combat['nonce'] and not fresh[role]['deaths'] and not fresh[role]['committed'] and not fresh[role]['applied']
            assert rd(T/f'final-player-health-{role}.json')['current']==56 and not rd(T/f'final-combat-{role}.json')['enabled']
        for h in rd(T/'world-health.json'): assert h['host']['current']==h['client']['current'] and h['host']['maximum']==h['client']['maximum']
        p=rd(T/'presentation-after-0.json'); assert p['movement']['active'] and p['movement']['rangedWeight']>.9 and not p['movement']['refused'] and p['prop']['active'] and not p['prop']['refused']
        assert rd(T/'capture-ended-before-reload.json')['passed'] and rd(T/'verified.json')['inputAttempts']==1
        c['scope']='Client native single regular bolt: both guards40-to25, no death; duplicates, body/prop counters, complete shared reload/rearm and protected restoration pass. One living-victim reload, not a fix for crash961 or full active combat certification.'
        review=dict(client='Six inspected frames at0.4/0.6/0.9/1.4/3/8s show ordinary first-person bolt firing, blood/hit reaction, and upright living recovery. No complete animation or embedded-bolt acceptance.',host='Corresponding visible guard reaction and upright recovery. Remote Emily is partly cut off/obstructed by furniture at left; full-body grip alignment is not accepted.',completePairedChoreographyAccepted=False)
        wr(T/'visual-review-v969.json',review)
    elif name.endswith('v961'):
        recovery=rd(T/'manual-recovery-v963.json'); assert recovery['recovered']
        c['manualRecovery']=recovery
        c['scope']='Native living bolt hit40-to25 matched. Full test and automatic cleanup FAILED: client access violation in nvwgf2umx.dll+300d7a during shared reload. Separate isolated recovery963 passed. Root cause remains open; location alone does not assign blame.'
        review={}
    else:
        assert c['health']['host']==0
        c['scope']='Requested living-victim coverage FAILED: native shot killed both targets. Outcome matched; automatic protected restoration passed. No nonterminal or full checkpoint-test acceptance.'
        review={}
    if (T/'aim-before.json').exists():
        ab=rd(T/'aim-before.json'); ae=rd(T/'aim-exact.json'); ar=rd(T/'aim-restored.json')
        assert ab==ar and ae['controller']==0 and ae['mouse']==ab['mouse']
        c['aim']={'before':ab,'during':ae,'restored':ar}
        if name.endswith('v968'): c['scope']+=' Controller mode0 still selected region2; snapping mode alone does not explain the targeting.'
    c['visualReview']=review; cases.append(c)
    for role in ('host','client'):
        path=f'{name}/{role}.mp4'; assert (N/path).exists()
        m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0]
        a,b=map(float,m['avg_frame_rate'].split('/')); videos=[v for v in videos if v['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'NPC nonterminal investigation / {name} / {role}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2),container_fps=m['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=c['scope'],review=review.get(role,'Recording retained; no new visual acceptance.'),candidate=c['candidate'],dll=cp['dll'],runtime_test_passed=passed))
for f in rd(W/'protected-save-hashes-before-v4.json'): assert sha(Path(f['Path']))==f['Hash']
at=datetime.datetime.now().astimezone().isoformat()
limits='Candidate954 both-role regular bolt milestone and948 pistol milestone remain preserved. Exact-aim nonterminal plus shared reload verified only for client967. Host968 still hit head; aim feedback investigation needed. Reload crash961 remains unresolved. Complete world combat, source-class/AI modifiers, projectile physics, movement/campaign/native UI and separate-account Steam validation remain open.'
audit=dict(at=at,candidate=A.name,dll=pin['dll'],probe=pin['probe'],complete=False,combatProtocol=6,combatPacketBytes=276,cases=cases,limitations=limits)
wr(W/'npc-nonterminal-audit-v969.json',audit); wr(N/'runtime-video-manifest.json',videos)
pin.update(runtimeVerified=False,scopedClientNonterminalReloadVerified=True,runtimeEvidence='../npc-nonterminal-audit-v969.json',scope=limits); wr(A/'identity.json',pin)
s=rd(R/'work/current-task-state.json'); s['UpdatedAt']=at
s['NpcCombatContinuation'].update(at=at,candidate=A.name,receipt='work/movement-integration/npc-nonterminal-audit-v969.json',runtimeVerified=False,complete=False,latest='Client967: one native regular bolt40-to25, duplicate rejection, complete shared checkpoint/rearm and restoration passed. Host968 remains a failed living-hit attempt. Original aim preferences restored in both. '+limits,next='Read actual native camera feedback using existing measured aim helper before another shot; then advance simultaneous/active NPC combat. Avoid blind repeated targeting.'); s['NpcCombatContinuation'].pop('inProgress',None)
s['MovementExecutionPriority']['current']=s['NpcCombatContinuation']['latest']; wr(R/'work/current-task-state.json',s)
print(json.dumps(dict(audit='npc-nonterminal-audit-v969.json',cases=len(cases),recordings=len(videos),complete=False)))
