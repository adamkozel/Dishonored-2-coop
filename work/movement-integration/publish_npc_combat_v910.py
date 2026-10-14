"""UNEXECUTED DRAFT: host909 failed on a secondary corpse event.

Do not publish this planned all-pass matrix. Continue with candidate911 and
the later audit; retain this file as an unexecuted intermediate proposal.
"""
from pathlib import Path
import datetime, hashlib, json, subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';A=W/'candidate-npc-combat-v907'
def rd(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']==sha(A/'dinput8.dll')
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
cases=[];videos=rd(N/'runtime-video-manifest.json')
for name,build,source in [('npc-combat-client-v906',905,'client'),('npc-combat-client-v908',907,'client'),('npc-combat-host-v909',907,'host')]:
    T=N/name;candidate=W/f'candidate-npc-combat-v{build}';cp=rd(candidate/'identity.json');result=rd(T/'result.json')
    assert result['passed'] and result['candidate']==cp['dll'] and rd(T/'restoration.json')['passed']
    death=rd(T/'native-death.json');dup=rd(T/'duplicate-death.json');old=rd(T/'reload-before.json');reloaded=rd(T/'reload-completed.json');fresh=rd(T/'rearmed-combat.json')
    commits=death['host']['combat']['committed'];assert commits>0 and commits==death['client']['combat']['applied']
    for role in ('host','client'):
        c=death[role]['combat'];assert c['enabled'] and c['deaths']==1 and c['nativeSubtractions']==commits and c['nativeLethalSubtractions']==1
        assert death[role]['health']['current']==dup[role]['health']['current']==0 and c['preDeathReleased']==2
        assert dup[role]['combat']['deaths']==1 and dup[role]['combat']['nativeSubtractions']==commits
        assert not c.get('pendingRequests',0) and not any(x['pending'] for x in c['entries'])
        for ai in ('decision','reflex'):
            x=rd(T/f'dead-{role}-{ai}.json');assert not x['schedulerActive'] and not x['pauseReferences']
        assert reloaded[role]['reset']['completed']==old[role]['reset']['completed']+1 and reloaded[role]['bridge']['localGameLogicState']==1
        assert not reloaded[role]['combat']['enabled'] and fresh[role]['enabled'] and fresh[role]['result']=='active_native_melee'
        assert fresh[role]['nonce']>c['nonce'] and not fresh[role]['committed'] and not fresh[role]['applied'] and not fresh[role]['deaths']
        assert rd(T/f'final-player-health-{role}.json')['current']==56 and not rd(T/f'final-combat-{role}.json')['enabled']
    assert dup['client']['combat']['duplicates']>death['client']['combat']['duplicates']
    if source=='client':
        assert commits==4 and dup['host']['combat']['duplicates']>death['host']['combat']['duplicates']
        for i,hp in enumerate((30,20,10,0)):
            p=rd(T/f'contact-{i}.json');assert p['host']['health']['current']==p['client']['health']['current']==hp
    scope=f'{source}-initiated native melee; {commits} ordered native subtractions and one lethal ownership release in each game; duplicates, shared checkpoint and fresh combat pass. Finite AI fixture. No ranged or compound-request burst acceptance.'
    reviews=rd(T/'visual-review-v910.json')
    cases.append(dict(take=name,candidate=candidate.name,dll=cp['dll'],passed=True,commits=commits,source=source,scope=scope,visualReview=reviews))
    for role in ('host','client'):
        path=f'{name}/{role}.mp4';meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0]
        a,b=map(float,meta['avg_frame_rate'].split('/'));videos=[v for v in videos if v['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Native evaluated damage / {source} source / {role} view / {name}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2),container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review=reviews[role],candidate=candidate.name,dll=cp['dll']))
T=N/'npc-ranged-observe-v903';assert rd(T/'result.json')['passed'] and rd(T/'restoration.json')['passed']
obs=rd(T/'damage.json');assert len(obs['events'])==3
assert [(e['before'],e['after']) for e in obs['events']]==[(40,24.8662033),(24.8662033,0),(50,49)]
assert rd(T/'isolation-after.json')['lost']==2
path=f'{T.name}/host.mp4';meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
videos=[v for v in videos if v['video']!=path];videos.append(dict(video=path,role='host',character='corvo',label='Native pistol compound damage observation903',travelling=False,gameplay_bound=False,actual_capture_hz=round(a/b,2),container_fps=meta['avg_frame_rate'],timing='Native wall-time recording',scope='Local observation only: spread, bullet and secondary corpse contact. No replicated ranged acceptance. Old observation fixture lost two release confirmations on death; independent checkpoint restoration passed.',review='Source contact sheet inspected: actual pistol shot, victim death, nearby NPC and restoration loading visible.',candidate='candidate-npc-combat-v896',dll=rd(T/'result.json')['candidate']))
for x in rd(W/'protected-save-hashes-before-v4.json'):assert sha(Path(x['Path']))==x['Hash']
at=datetime.datetime.now().astimezone().isoformat();limits='Complete combat/co-op and full movement acceptance remain unfinished. Only existing native melee profiles are enabled; actual compound ranged request bursts, blocked/zero outcomes, dynamic ranged/source metadata, host-owned environment damage, AI targeting both players and paired corpse/limb physics remain unverified. Handshake882 and native save-busy retry positive coverage remain open.'
audit=dict(at=at,candidate=A.name,dll=pin['dll'],scopedNativeDamageBoundaryVerified=True,complete=False,cases=cases,observation='npc-ranged-observe-v903',compoundQueue='Bounded32 FIFO, malformed/order/wrap/overflow/clear policy tests pass; real multi-contact native ranged burst not tested.',limitations=limits)
wr(W/'npc-combat-audit-v910.json',audit);wr(N/'runtime-video-manifest.json',videos)
pin.update(runtimeVerified=False,nativeMeleeFixtureVerified=True,scopedCombatRecoveryVerified=True,scopedNativeDamageBoundaryVerified=True,runtimeEvidence='../npc-combat-audit-v910.json',runtimeScope=limits);wr(A/'identity.json',pin)
p=W/'candidate-npc-combat-v905/identity.json';v=rd(p);v.update(runtimeVerified=False,scopedNativeDamageBoundaryVerified=True,runtimeEvidence='../npc-combat-audit-v910.json',runtimeScope='Client906 native subtraction/death/duplicate/checkpoint regression passes. '+limits);wr(p,v)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['NpcCombatContinuation'].update(at=at,candidate=A.name,runtimeVerified=False,complete=False,receipt='work/movement-integration/npc-combat-audit-v910.json',latest='Exact native final-damage ownership boundary passes client906/908 and host909, including shared checkpoint/rearm. Bounded32 request FIFO replaces dropped additional pending contacts; policy tests pass but a real compound ranged burst is not yet replicated. Pistol903 observes spread+bullet+secondary corpse damage; 85 exact selected asset declarations exported. Both isolated games restored. '+limits,next='Implement shared ranged/spread damage descriptor and callback-time source ownership; support native protected/zero outcomes and separate host-owned world damage from player requests. Use native final-damage retirement and bounded request FIFO already in907. Read handoff chapter74 for observed fields and scope; no new weapon profile is runtime accepted.');s['MovementExecutionPriority']['current']=s['NpcCombatContinuation']['latest'];wr(R/'work/current-task-state.json',s)
b=rd(R/'work/handoff-npc-combat-bookmark-v907.json');assert json.loads(b['receipt']['structuredContent']['result'])['success'] and json.loads(b['saved']['structuredContent']['result'])['success']
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
    p=R/'work'/name;items=rd(p)
    for entry in b['bookmarks']:
        items=[x for x in items if (x.get('address'),x.get('category'))!=(entry['address'],entry['category'])];items.append(dict(entry,type='Note'))
    wr(p,items)
print(json.dumps(dict(passed=True,audit='npc-combat-audit-v910.json',videos=len(videos),complete=False)))
