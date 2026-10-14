from pathlib import Path
import datetime,hashlib,json,shutil,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11';A=W/'candidate-npc-stream-v847';T=N/'npc-recovery-v848b';V=N/'npc-visible-menu-v849'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();pin=rd(A/'identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll'] and sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==pin['probe']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
for take in (T,V):
    for f in ('result.json','restoration.json'):assert rd(take/f)['passed']
assert not rd(N/'npc-recovery-v848/result.json')['passed'] and rd(N/'npc-recovery-v848/restoration.json')['passed']
catalog=rd(T/'fresh-host-catalog.json')['entries'];assert len(catalog)==rd(T/'result.json')['freshActorCount']==7
assert {'ld_spawner_vista_5','ld_spawner_alexi_escape'}.issubset({e['name'] for e in catalog})
fresh=rd(T/'ready-fresh-checkpoint.json');assert len(fresh['host']['entries'])==len(fresh['client']['entries'])==7
for e in fresh['host']['entries']:
    peer=next(p for p in fresh['client']['entries'] if p['handle']==e['peerHandle'])
    assert peer['peerHandle']==e['handle'] and peer['peerArm']==e['arm'] and peer['arm']==e['peerArm'] and peer['rig']==e['rig']
for role in ('host','client'):
    initial=rd(T/'ready-initial.json')[role]
    for index,source in enumerate(('host','client'),1):
        start=rd(T/f'menu-{source}-entered.json')[role];held=rd(T/f'menu-{source}-held.json')[role];resumed=rd(T/f'menu-{source}-resumed-{role}.json')
        assert held['session']['menuSuspended'] and start['bodyApplied']==held['bodyApplied']
        for e in held['entries']:
            b=next(x for x in start['entries'] if x['arm']==e['arm'])
            assert (e['roots'],e['poses'],e['references'])==(b['roots'],b['poses'],b['references'])
        assert resumed['session']['menuSuspensions']==resumed['session']['menuResumes']==index and resumed['session']['menuPausedMs']>=6000*index
        assert not resumed['session']['released'] and not resumed['session']['lost']
        for e in resumed['entries']:
            b=next(x for x in held['entries'] if x['arm']==e['arm']);assert e['poses']>b['poses']+10
    before=rd(T/f'reload-before-{role}.json');after=rd(T/f'reload-after-{role}.json');s=after['npc']['session']
    assert after['bridge']['localEpoch']!=before['bridge']['localEpoch']
    assert after['reset']['loads']==before['reset']['loads']+1 and after['reset']['completed']==before['reset']['completed']+1 and after['reset']['aborted']==before['reset']['aborted']
    assert after['character']['character']==(1 if role=='host' else 2)
    assert s['reloads']==s['rearms']==1 and not s['reloadPending'] and not s['lost']
    for e in after['npc']['entries']:
        assert e['enabled'] and e['ready'] and e['poses']>=20
        old=[x for x in before['npc']['entries'] if x['handle']==e['handle']]
        if old:assert e['arm']>old[0]['arm']
    if role=='client':assert s['acquired']==24 and s['released']==10 and not s['retired']
    final=rd(T/f'final-stream-{role}.json');assert not final['enabled'] and not final['session']['enabled'] and final['session']['acquired']==final['session']['released'] and not final['session']['lost']
    for take in (T,V):
        suffix='restored' if take==T else 'after'
        beforeInv=rd(take/f'inventory-before-{role}.json')['items'];afterInv=rd(take/f'inventory-{suffix}-{role}.json')['items']
        assert sorted((e['name'],e['quantity']) for e in beforeInv)==sorted((e['name'],e['quantity']) for e in afterInv)
        assert rd(take/f'health-{suffix}-{role}.json')['current']==56
visible=rd(V/'visible-before-menu.json');start=rd(V/'menu-start.json');held=rd(V/'menu-end.json');resumed=rd(V/'visible-after-menu.json')
assert sum(e['applied']>30 for e in visible['bodyEntries'])==2
assert start['client']['bodyApplied']==held['client']['bodyApplied']==5967
assert resumed['bodyApplied']==6211 and resumed['session']['menuResumes']==1 and resumed['session']['acquired']==10 and not resumed['session']['released']
assert rd(V/'final-stream-client.json')['session']['released']==10
scope='Verified native host/client menu suspension and resume on retained NPC arms; same-map coordinated checkpoint retires old AI leases and rebinds seven fresh native actors after both-complete/role restoration. Visible menu proof has two223-joint body callbacks before/after, none during held pause. General combat, mid-world spawning/death and campaign transitions remain incomplete.'
audit=dict(at=at,passed=True,complete=False,candidate=A.name,dll=pin['dll'],probe=pin['probe'],scope=scope,initialActors=5,reloadedActors=7,clientTotalAcquired=24,clientTotalReleased=24,clientLost=0,visibleBodiesBefore=visible['bodyApplied'],visibleBodiesHeld=5967,visibleBodiesAfter=6211,evidence='work/natural-moveset/npc-recovery-v848b/result.json',visualEvidence='work/natural-moveset/npc-visible-menu-v849/contact.png',visualJudgment='Paired sampled frames show native host Pause screen, held client NPC posture and return to animation afterward. Two visible small-guard rigs only; no224-joint or complete movement-quality acceptance.',failedTake=dict(path='work/natural-moveset/npc-recovery-v848/result.json',reason='Harness assumed five actors after checkpoint. Both native catalogs/streams actually contained seven; source unchanged in repeat848b, expected count now derived from fresh host catalog.',restorationPassed=True))
wr(W/'npc-lifecycle-audit-v849.json',audit)
pin.update(runtimeVerified=True,runtimeAudit='../npc-lifecycle-audit-v849.json',runtimeScope=scope);wr(A/'identity.json',pin)
for name in ('Test-NpcRecovery-v848.ps1','Test-NpcVisibleMenu-v849.ps1'):shutil.copy2(N/name,A/name)
latest='847–849: NPC stream sessions now retain bindings and pause native writes during ordinary menus, resume with fresh interpolation/replay floor, and release before shared checkpoint teardown. Test848b verifies both menus >6s then fresh catalog of seven actors, two player roles retained; client acquired/released24/lost0. Visible849 shows live body updates before and after host Pause, zero during suspension. Failed848 fixed only the harness five-actor assumption.'
next_work='Generalize ongoing NPC lifecycle and shared combat outcomes: new/retired actor negotiation, repeated hit requests and authoritative health/death/equipment, AI perception of both players. Validate remaining224-joint visible rigs and sustained bandwidth. Continue campaign/progression, functional movement/VFX gaps and native session UI/separate-account Steam validation.'
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['NpcAuthorityContinuation']=dict(at=at,candidate=A.name,receipt='work/movement-integration/npc-lifecycle-audit-v849.json',details='documentation/handoff-2026-10-11/72-npc-menu-and-checkpoint-lifecycle.md',runtimeVerified=True,presentationStreamVerified=True,checkpointRenewalVerified=True,networkAuthorityVerified=False,complete=False,latest=latest,next=next_work,scope=scope)
state['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',state)
bms=rd(R/'work/handoff-npc-lifecycle-bookmark-v849.json')['bookmarks'];keys={(b['address'],b['category']) for b in bms}
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
    p=R/'work'/name;wr(p,sorted([b for b in rd(p) if (b['address'],b['category']) not in keys]+bms,key=lambda b:(b['address'],b['category'])))
videos=rd(N/'runtime-video-manifest.json')
for role in ('host','client'):
    path=V/f'{role}.mp4';info=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate','-of','json',str(path)]))['streams'][0]
    rel=path.relative_to(N).as_posix();videos=[e for e in videos if e['video']!=rel]
    videos.append(dict(video=rel,character='corvo' if role=='host' else 'emily',role=role,label='NPC menu suspension and resume · '+role,travelling=True,gameplay_bound=True,actual_capture_hz=int(info['nb_frames'])/float(info['duration']),container_fps=info['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,candidate=A.name,dll=pin['dll'],review='Paired frames inspected: native host menu, frozen client guard and resumed pose; scope is menu lifecycle, not full rig or combat acceptance.'))
wr(N/'runtime-video-manifest.json',videos)
p=D/'README.md';s=p.read_text();line='- [NPC menu and checkpoint lifecycle](72-npc-menu-and-checkpoint-lifecycle.md)'
if line not in s:p.write_text(s.rstrip()+'\n\n'+line+'\n')
print(json.dumps(dict(passed=True,candidate=pin['dll'],initialActors=5,reloadedActors=7,acquiredReleased=24,visibleMenu=True,videos=len(videos))))
