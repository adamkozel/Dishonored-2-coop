"""Publish only completed receipts; preserve failed investigations and visual gaps."""
from pathlib import Path
import datetime, hashlib, json, struct, subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';A=W/'candidate-npc-combat-v954'
def rd(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']==sha(A/'dinput8.dll')
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
cases=[];videos=rd(N/'runtime-video-manifest.json')
matrix=[('npc-crossbow-host-v956',954,True),('npc-crossbow-client-v957',954,False),('npc-crossbow-client-v958',954,True)]
for name,build,expected in matrix:
    T=N/name;result=rd(T/'result.json');restoration=rd(T/'restoration.json');cp=rd(W/f'candidate-npc-combat-v{build}/identity.json')
    assert result['passed']==expected and restoration['passed'] and result['candidate']==cp['dll'],name
    case=dict(take=name,candidate=f'candidate-npc-combat-v{build}',dll=cp['dll'],passed=expected,source=result['source'],failure=result['failure'],restored=True)
    if expected:
        d=rd(T/'native-death.json');b=rd(T/'before-duplicate.json');dup=rd(T/'duplicate-latest.json')
        old=rd(T/'reload-before.json');new=rd(T/'reload-completed.json');fresh=rd(T/'rearmed-combat.json')
        assert d['host']['combat']['committed']==d['client']['combat']['applied']>=1
        for role in ('host','client'):
            c=d[role]['combat'];assert c['enabled'] and c['deaths']==d['host']['combat']['deaths'] and c['nativeLethalSubtractions']==c['deaths'] and c['preDeathReleased']==2*c['deaths']
            assert d[role]['health']['current']==dup[role]['health']['current'] and d[role]['health']['current']>=0
            assert (d[role]['health']['current']==0)==bool(c['deaths'])
            assert dup[role].get('powerFx',{}).get('started')==b[role].get('powerFx',{}).get('started')
            assert dup[role]['combat']['committed']==b[role]['combat']['committed'] and dup[role]['combat']['applied']==b[role]['combat']['applied']
            assert not c['pendingRequests'] and not c['fixture']['lost']
            assert new[role]['reset']['completed']==old[role]['reset']['completed']+1 and not new[role]['combat']['enabled']
            assert fresh[role]['enabled'] and fresh[role]['nonce']>c['nonce'] and not fresh[role]['deaths'] and not fresh[role]['applied'] and not fresh[role]['committed']
            assert rd(T/f'final-player-health-{role}.json')['current']==56
            assert not rd(T/f'final-combat-{role}.json')['enabled']
        assert dup['client']['combat']['duplicates']>b['client']['combat']['duplicates']
        if result['source']=='client':assert dup['host']['combat']['duplicates']>b['host']['combat']['duplicates']
        for h in rd(T/'world-health.json'):assert h['host']['current']==h['client']['current'] and h['host']['maximum']==h['client']['maximum']
        case.update(deaths=d['host']['combat']['deaths'],remainingHealth=d['host']['health']['current'],commits=d['host']['combat']['committed'],pendingHighWater=d['client']['combat'].get('pendingHighWater',0),worldOutcomes=rd(T/'world-outcomes.json'))
        if True:
            trace=rd(T/'source-trace.json');trace=trace if isinstance(trace,list) else [trace];flags=[struct.unpack_from('<Q',bytes.fromhex(e['rawInfo']),16)[0] for e in trace]
            assert flags and all(f==0x100 for f in flags)
            assert 1<=rd(T/'verified.json')['inputAttempts']<=2
            for e in trace:
                f=struct.unpack_from('<Q',bytes.fromhex(e['rawInfo']),16)[0]
                if f==0x100:assert e['attacker']['valid'] and e['attacker']['handle']==1 and e['inflictor']['valid'] and e['inflictor']['localOwned']
        if name.endswith(('v956','v958')):
            p=rd(T/'presentation-after-0.json');assert p['movement']['active'] and p['movement']['version']==67 and p['movement']['rangedWeight']>.9 and not p['movement']['refused']
            assert p['prop']['active'] and p['prop']['network'] and not p['prop']['refused']
            assert not p['fx']['refused'];case['scopedPresentationCountersPassed']=True
        case['scope']=f"Native health outcomes with {case['deaths']} death(s) and {case['remainingHealth']} remaining target health; "+'scoped native damage,  duplicates, shared checkpoint and restoration passed; complete world combat, paired corpse/limb physics and all hit modifiers are not established.'
    else:case['scope']='Failed combined test retained: both native bolt contacts matched health40-to25-to10, but scripted reposition before the second shot caused a movement clock rejection. Full test not accepted; independently restored.'
    if name=='npc-ranged-client-v938':assert case['pendingHighWater']==2
    if name=='npc-combat-fixture-v940':
        assert case['worldOutcomes']['host']==case['worldOutcomes']['client']==2 and case['worldOutcomes']['replicaSuppressed']>=1
        case['scope']+=' Reconstructed native ignition and burning death; no secondary corpse contact in this take and no natural trigger-activation proof.'
    if name=='npc-ranged-host-presentation-v941':case['scope']+=' Presentation attempt incomplete: equipment returned prematurely after death, muzzle effect scope refused, initial teleport caused one movement rejection. Gameplay assertions alone passed.'
    review=rd(T/'visual-review-v959.json') if (T/'visual-review-v959.json').exists() else rd(T/'visual-review-v943.json') if (T/'visual-review-v943.json').exists() else {}
    case['visualReview']=review;cases.append(case)
    for role in ('host','client'):
        path=f'{name}/{role}.mp4';assert (N/path).exists()
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0]
        a,b=map(float,meta['avg_frame_rate'].split('/'));videos=[v for v in videos if v['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'NPC crossbow /  {name} / {role}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2),container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=case['scope'],review=review.get(role,'Retained recording; no new visual acceptance.'),candidate=case['candidate'],dll=cp['dll'],runtime_test_passed=expected))
for f in rd(W/'protected-save-hashes-before-v4.json'):assert sha(Path(f['Path']))==f['Hash']
at=datetime.datetime.now().astimezone().isoformat()
limits='Broader projectile families, source-class/AI-dependent damage modifiers, protected regions, wall impacts, authoritative limb/corpse physics, active enemy targeting both players and automatic full-world combat remain incomplete. Independent Steam-account online testing and complete movement/campaign/native session UI also remain open.'
audit=dict(at=at,candidate=A.name,dll=pin['dll'],combatProtocol=6,combatPacketBytes=276,complete=False,cases=cases,nativeDiagnostic='npc-crossbow-inspect-v953',limitations=limits)
wr(W/'npc-crossbow-audit-v959.json',audit);wr(N/'runtime-video-manifest.json',videos)
pin.update(runtimeVerified=False,scopedNativeCrossbowBothRolesVerified=True,scopedCrossbowBodyPropVerified=True,runtimeEvidence='../npc-crossbow-audit-v959.json',runtimeScope=limits,combatProtocol=6,combatPacketBytes=276);wr(A/'identity.json',pin)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
latest='Regular crossbow contacts now use the same native ranged profile pipeline as pistol spread/bullets. Both roles956/958 pass native damage, duplicates, existing remote movement/prop, shared reload and protected restoration on wire6/276. Earlier integrated pistol948 and audit952 remain preserved. '+limits
s['NpcCombatContinuation'].update(at=at,candidate=A.name,runtimeVerified=False,complete=False,receipt='work/movement-integration/npc-crossbow-audit-v959.json',details='documentation/handoff-2026-10-11/76-shared-ranged-profiles.md',latest=latest,next='Use the shared native ranged profiles for body-region/nonterminal coverage, then remove active-AI/source-state blockers. Broader ammo, powers and actual projectile/impact ownership remain unverified; preserve948/954 milestones.');s['NpcCombatContinuation'].pop('inProgress',None)
s['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',s)
b=rd(R/'work/handoff-npc-combat-bookmark-v931.json');assert json.loads(b['saved']['structuredContent']['result'])['success']
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
    p=R/'work'/name;items=rd(p)
    for e in b['bookmarks']:items=[x for x in items if (x.get('address'),x.get('category'))!=(e['address'],e['category'])];items.append(dict(e,type='Note'))
    wr(p,items)
print(json.dumps(dict(audit='npc-crossbow-audit-v959.json',cases=len(cases),recordings=len(videos),complete=False)))
