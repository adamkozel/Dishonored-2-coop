from pathlib import Path
import datetime,hashlib,json
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';T=N/'npc-combat-v866';A=W/'candidate-npc-combat-v865'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
failed=[]
for v in (851,853,854,856,858,859,861,864,866,867):
    take=N/f'npc-combat-v{v}';result=rd(take/'result.json');assert not result['passed'] and rd(take/'restoration.json')['passed']
    failed.append(dict(take=take.relative_to(R).as_posix(),failure=result['failure'],restored=True))
hit=rd(T/'hit-0-host.json');duplicate=rd(T/'duplicate-0.json');h=hit['host'];c=hit['client']
assert h['combat']['committed']==c['combat']['applied']==1
assert h['health']['current']==c['health']['current']==30
assert h['player']['current']==c['player']['current']==56
assert h['combat']['captured']==h['combat']['workerCaptured']==1
assert h['combat']['lastCaptureThread']!=h['combat']['engineThread']
assert h['combat']['nativeAttempt']['declaration']=='components/characters/player/base/damage/player_sword.damage'
for role in ('host','client'):
    assert duplicate[role]['health']['current']==30
    assert duplicate[role]['combat']['applied']==hit[role]['combat']['applied']
    assert duplicate[role]['combat']['committed']==hit[role]['combat']['committed']
assert duplicate['client']['combat']['duplicates']==1
failure=rd(T/'failure-state.json')
assert failure['client']['combat']['nativeAttempt']['declaration']=='components/characters/player/base/damage/generic_fatality.damage'
assert failure['host']['health']['current']==failure['client']['health']['current']==30
for role in ('host','client'):
    final=rd(N/f'npc-combat-v867/final-combat-{role}.json');s=rd(N/f'npc-combat-v867/final-stream-{role}.json')
    assert not final['enabled'] and not final['ordinaryTestUntil'] and not final['fixture']['active'] and not final['fixture']['lost']
    assert final['fixture']['acquired']==final['fixture']['released'] and not s['enabled'] and all(not e['references'] and not e['lost'] for e in s['entries'])
for f in rd(W/'protected-save-hashes-before-v4.json'):assert sha(Path(f['Path']))==f['Hash']
scope='Partial only: one worker-origin native player_sword damage event in take866 is reconstructed on the engine thread, host ordered over native Steam, and applies40-to30 once in both games. Duplicate commit leaves health unchanged. The larger repeated-controller test fails on an unsupported generic_fatality action; recordings show close finisher behavior, not an accepted complete ordinary attack. Client-source requests, repeated hits, full attack feedback and lethal outcomes remain unverified.'
at=datetime.datetime.now().astimezone().isoformat()
audit=dict(at=at,partialEvidencePassed=True,fullTestPassed=False,complete=False,candidate=A.name,dll=pin['dll'],probe=pin['probe'],scope=scope,sourceWorkerThread=h['combat']['lastCaptureThread'],sourceEngineThread=h['combat']['engineThread'],hostNativeCommits=1,clientNativeApplications=1,duplicateRejections=1,healthAfter=30,fixtures=['finite source AI and root hold','finite ordinary-assassination eligibility exclusion'],visualJudgment='Paired contact sheets inspected. Close face/finisher behavior and the later refused client fatality are visible. This is native event diagnostic evidence, not complete combat or animation-quality acceptance.',failedTakes=failed,next='Add native fatality/assassination and lethal outcome lifecycle to the actor protocol; pre-death frame/AI retirement and deduplicated tombstones. Verify client-source ordering and complete attacks. Do not continue tuning test distance to avoid genuine native action types.')
wr(W/'npc-combat-partial-audit-v868.json',audit)
pin.update(runtimeVerified=False,runtimePartialEvidence='../npc-combat-partial-audit-v868.json',runtimeScope=scope);wr(A/'identity.json',pin)
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['NpcCombatContinuation']=dict(at=at,candidate=A.name,runtimeVerified=False,partialWorkerEventVerified=True,complete=False,receipt='work/movement-integration/npc-combat-partial-audit-v868.json',details='documentation/handoff-2026-10-11/73-npc-combat-development.md',latest=scope,next=audit['next'],previousAcceptedNpcLifecycle='candidate-npc-stream-v847')
state['MovementExecutionPriority']['current']='NPC combat continuation850-868: worker-thread native damage captured and one host-to-client10HP event deduplicated in both games. Full attack test remains incomplete because native fatality selection is unsupported. All failed takes restored; candidate865 remains an opt-in experiment.'
wr(R/'work/current-task-state.json',state)
print(json.dumps(audit))
