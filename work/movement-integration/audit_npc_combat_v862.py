from pathlib import Path
import datetime,hashlib,json,shutil,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';T=N/'npc-combat-v861';A=W/'candidate-npc-combat-v860'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
assert rd(T/'result.json')['passed'] and rd(T/'restoration.json')['passed']
assert rd(T/'verified.json')['hits']==3
rows=[]
for index,role in enumerate(('host','client','client')):
    hit=rd(T/f'hit-{index}-{role}.json');duplicate=rd(T/f'duplicate-{index}.json');h=hit['host'];c=hit['client']
    assert h['combat']['enabled'] and c['combat']['enabled']
    assert h['combat']['committed']==c['combat']['applied']==index+1
    expected=30-index*10
    assert h['health']['current']==c['health']['current']==expected
    assert h['player']['current']==c['player']['current']==56
    assert h['combat']['fixture']['active'] and h['combat']['fixture']['ready'] and h['combat']['fixtureFrameHolds']>0
    attempt=hit[role]['combat']['nativeAttempt']
    assert attempt['declaration']=='components/characters/player/base/damage/player_sword.damage'
    for peer in ('host','client'):
        assert duplicate[peer]['health']['current']==hit[peer]['health']['current']
        assert duplicate[peer]['combat']['committed']==hit[peer]['combat']['committed']
        assert duplicate[peer]['combat']['applied']==hit[peer]['combat']['applied']
    assert duplicate['client']['combat']['duplicates']>c['combat']['duplicates']
    if role=='client':assert duplicate['host']['combat']['duplicates']>h['combat']['duplicates']
    rows.append(dict(index=index+1,source=role,health=expected,hostCommits=h['combat']['committed'],clientApplications=c['combat']['applied'],hostDuplicates=duplicate['host']['combat']['duplicates'],clientDuplicates=duplicate['client']['combat']['duplicates']))
for role in ('host','client'):
    s=rd(T/f'final-stream-{role}.json');combat=rd(T/f'final-combat-{role}.json');state=rd(T/f'final-{role}.json')
    assert not s['enabled'] and not combat['enabled'] and not combat['fixture']['active']
    assert not combat['fixture']['lost'] and combat['fixture']['acquired']==combat['fixture']['released']
    assert all(not e['references'] and not e['lost'] for e in s['entries'])
    assert state['localGameLogicState']==1 and state['remoteFresh'] and rd(T/f'final-player-health-{role}.json')['current']==56
for f in rd(W/'protected-save-hashes-before-v4.json'):assert sha(Path(f['Path']))==f['Hash']
scope='Three real controller sword contacts (host/client/client) on a generic Dunwall guard binding, producing ordered native40-to30-to20-to10 health in both games; replayed requests/commits cause no extra damage. Source AI and root movement are held by an explicit finite fixture. Does not verify autonomous combat, fatal hits, other damage families, simultaneous attacks or campaign behavior.'
audit=dict(at=datetime.datetime.now().astimezone().isoformat(),passed=True,complete=False,candidate=A.name,dll=pin['dll'],probe=pin['probe'],scope=scope,rows=rows,sourceFixture=True,source='work/natural-moveset/npc-combat-v861/result.json',visualReviewPending=True)
wr(W/'npc-combat-audit-v862.json',audit)
pin.update(runtimeVerified=True,runtimeAudit='../npc-combat-audit-v862.json',runtimeScope=scope);wr(A/'identity.json',pin)
shutil.copy2(N/'Test-NpcCombat-v861.ps1',A/'Test-NpcCombat-v861.ps1')
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=audit['at']
state['NpcCombatContinuation']=dict(at=audit['at'],candidate=A.name,runtimeVerified=True,complete=False,receipt='work/movement-integration/npc-combat-audit-v862.json',details='documentation/handoff-2026-10-11/73-npc-combat-development.md',scope=scope,latest='Repeated host-ordered nonfatal native sword outcomes and duplicate rejection verified under finite AI/root test isolation. Full combat remains incomplete.',next='Verify autonomous contact/combat and both-player targeting; native death/ownership retirement, additional damage families, repeated/simultaneous requests, pending transport and reload lifecycle.')
wr(R/'work/current-task-state.json',state)
print(json.dumps(audit))
