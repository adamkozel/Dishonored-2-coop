from pathlib import Path
import json,hashlib,datetime,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11';A=W/'candidate-npc-authority-v837';T=N/'npc-authority-hold-v838b'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
pin=rd(A/'identity.json');at=datetime.datetime.now().astimezone().isoformat()
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
assert sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==pin['probe']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
assert rd(T/'result.json')['passed'] and rd(T/'restoration.json')['passed'] and rd(T/'final-readback.json')['passed']
assert not rd(N/'npc-authority-hold-v838/result.json')['passed']
ready=rd(T/'hold-ready.json');duplicate=rd(T/'hold-after-observations.json')
assert ready['actors']==5 and ready['acquired']==10 and ready['ready'] and not ready['released']
assert duplicate['ready'] and duplicate['acquired']==10 and duplicate['refused']==1
for tag,reason in [('expired','expired'),('cancelled','cancelled')]:
    h=rd(T/f'hold-{tag}.json');assert not h['active'] and h['result']==reason and h['released']==10 and h['acquired']==10 and not h['lost'] and not h['retired']
for role,tag,paused in [('client','held',True),('host','client-held',False),('client','after',False),('host','after',False)]:
    rows=rd(T/f'ai-{role}-{tag}.json');assert len(rows)==15
    for row in rows:
        s=row['value']
        if row['kind']=='attack':assert not s['started'] and not s['schedulerActive']
        else:assert s['started'] and s['schedulerActive']!=paused and s['pauseReferences']==int(paused) and s['pauseState']==(2 if paused else 0)
script=rd(T/'script-during.json');assert len(script)==16
for s in script:assert s['before']['flags']==s['after']['flags'] and s['before']['schedulerActive']==s['after']['schedulerActive']
for role in ('host','client'):
    before=rd(T/f'catalog-before-{role}.json');after=rd(T/f'catalog-after-{role}.json')
    assert len(before['entries'])==len(after['entries'])==5 and sum(e['maximum']==50 for e in before['entries'])==1
    assert before['entries']==after['entries']
    b=rd(T/f'before-{role}.json');a=rd(T/f'after-{role}.json');assert b['localEpoch']==a['localEpoch'] and a['localGameLogicState']==1 and a['localPendingGameLogicState']==0
    inv=lambda stage:sorted((i['name'],i['quantity']) for i in rd(T/f'inventory-{stage}-{role}.json')['items'])
    assert inv('before')==inv('after')
    assert rd(T/f'health-before-{role}.json')['current']==rd(T/f'health-after-{role}.json')['current']
final=rd(T/'final-readback.json')
for row in final['rows']:assert len(row['catalog']['entries'])==5
scope='General authored-NPC identity and component-incarnation binding; client-only batch ownership of five native actors including50HP Ramsey; ten pause references; duplicate rejection, natural expiry, explicit cancellation, host and sampled script preservation. No new root/body/combat network authority, visual proof or campaign certification.'
audit=dict(at=at,passed=True,complete=False,candidate=A.name,dll=pin['dll'],probe=pin['probe'],scope=scope,actors=5,references=10,scriptComponents=16,evidence='work/natural-moveset/npc-authority-hold-v838b/result.json',failedTake='work/natural-moveset/npc-authority-hold-v838/result.json',failureExplanation='PowerShell H alias resolved Get-History instead of the readiness reader. Same native candidate released10/lost0. Renamed reader and full independent take838b passed.',finalReadback=final)
wr(W/'npc-authority-audit-v838.json',audit)
pin.update(runtimeVerified=True,runtimeAudit='../npc-authority-audit-v838.json',runtimeScope=scope);wr(A/'identity.json',pin)
shutil.copy2(N/'Test-NpcAuthority-v838.ps1',A/'Test-NpcAuthority-v838.ps1')
latest='836–838: general authored-NPC/component-incarnation reader selects all five active Dunwall NPCs without per-name or40HP assumptions. Client batch ownership pauses ten Decision/Reflex components; duplicate acquisition rejected; natural expiry and cancellation release10/lost0; host AI and16script component samples unchanged. Local ownership verified; general NPC transport still pending.'
next_work='Connect generic per-NPC bindings/leases to a separately scoped native Steam authority channel, generalize frame/body presentation, then repeated combat/outcomes and world lifecycle. Preserve script-held actors and reject duplicate gameplay damage. Continue grouped family coverage, functional movement/VFX gaps, campaign/progression and native session UI/separate-account testing.'
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['NpcAuthorityContinuation']=dict(at=at,candidate=A.name,receipt='work/movement-integration/npc-authority-audit-v838.json',details='documentation/handoff-2026-10-11/70-generic-npc-binding-and-ai-ownership.md',runtimeVerified=True,networkAuthorityVerified=False,complete=False,latest=latest,next=next_work,scope=scope)
state['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',state)
bms=rd(R/'work/handoff-npc-authority-bookmark-v837.json')['bookmarks'];keys={(b['address'],b['category']) for b in bms}
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
    p=R/'work'/name;wr(p,sorted([b for b in rd(p) if (b['address'],b['category']) not in keys]+bms,key=lambda b:(b['address'],b['category'])))
p=D/'README.md';s=p.read_text();line='- [Generic NPC binding and native AI ownership](70-generic-npc-binding-and-ai-ownership.md)'
if line not in s:p.write_text(s.rstrip()+'\n\n'+line+'\n')
print(json.dumps(dict(passed=True,actors=5,references=10,scriptComponents=len(script),candidate=pin['dll'],generalNetworkAuthority=False)))
