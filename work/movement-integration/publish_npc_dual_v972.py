from pathlib import Path
import datetime, hashlib, json, subprocess
R=Path(__file__).resolve().parents[2]; N=R/'work/natural-moveset'; W=R/'work/movement-integration'; A=W/'candidate-npc-combat-v966'
def rd(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
pin=rd(A/'identity.json'); assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
cases=[]; videos=rd(N/'runtime-video-manifest.json')
for take,health,commits in [('npc-crossbow-measured-body-v970',25,1),('npc-dual-crossbow-v971',10,2)]:
    T=N/take; result=rd(T/'result.json'); assert result['passed'] and rd(T/'restoration.json')['passed'] and result['candidate']==pin['dll']
    before=rd(T/'before-duplicate.json'); dup=rd(T/'duplicate-latest.json'); old=rd(T/'reload-before.json'); new=rd(T/'reload-completed.json'); fresh=rd(T/'rearmed-combat.json'); contact=rd(T/'contact-0.json')
    assert contact['host']['combat']['committed']==contact['client']['combat']['applied']==commits
    for role in ('host','client'):
        assert contact[role]['health']['current']==health and not contact[role]['combat']['deaths'] and not contact[role]['combat']['pendingRequests']
        assert dup[role]['health']['current']==health and dup[role]['combat']['committed']==before[role]['combat']['committed'] and dup[role]['combat']['applied']==before[role]['combat']['applied']
        assert new[role]['reset']['completed']==old[role]['reset']['completed']+1 and not new[role]['combat']['enabled']
        assert fresh[role]['enabled'] and fresh[role]['nonce']>contact[role]['combat']['nonce'] and not fresh[role]['committed'] and not fresh[role]['applied']
        assert rd(T/f'final-player-health-{role}.json')['current']==56 and not rd(T/f'final-combat-{role}.json')['enabled']
    for h in rd(T/'world-health.json'): assert h['host']['current']==h['client']['current'] and h['host']['maximum']==h['client']['maximum']
    assert rd(T/'capture-ended-before-reload.json')['passed'] and dup['client']['combat']['duplicates']>before['client']['combat']['duplicates']
    roles=['host'] if commits==1 else ['host','client']
    for role in roles:
        suffix='' if commits==1 else '-'+role
        assert rd(T/f'aim-before{suffix}.json')==rd(T/f'aim-restored{suffix}.json') and rd(T/f'aim-exact{suffix}.json')['controller']==0
        assert rd(T/f'measured-body-aim{suffix}.json')['settled']
    c=dict(take=take,passed=True,restored=True,source=result['source'],remainingHealth=health,commits=commits,deaths=0)
    if commits==2:
        assert contact['host']['combat']['requests']==1 and dup['host']['combat']['duplicates']>before['host']['combat']['duplicates']
        inputs=rd(T/'dual-inputs.json'); c['inputStartSeparationMs']=inputs['client']['start']-inputs['host']['start']; trace=rd(T/'source-trace.json'); assert trace['host'] and trace['client']
        c['callbackTicks']={r:[e['tick'] for e in trace[r]] for r in roles}
        scope='Two ordinary crossbow inputs from Corvo and Emily, input starts12ms apart; host ordered two contacts, both targets40-to25-to10, one client request, no pending work/death. Duplicates, shared reload/rearm, native aim/settings and protected restoration passed. No proof of exact simultaneous callback execution, active AI combat or full paired visuals.'
        assert c['inputStartSeparationMs']==12
        review={'host':'Six inspected frames: normal crossbow fires; large nearby cloth/furniture blocks most target/body view. No full visual acceptance.','client':'Six inspected frames show native crossbow use, guard blood/reaction and upright recovery. Paired avatar visibility is insufficient for grip/choreography acceptance.'}
    else:
        scope='Host ordinary measured-camera torso shot left both guards25 health. Duplicate, shared reload/rearm and full restoration passed. Earlier client967 provides separate living-victim coverage. No complete AI or paired choreography acceptance.'
        review={'host':'Six inspected frames show torso hit and native blood/reaction, then upright living recovery.','client':'Observer is obstructed by nearby geometry throughout inspected frames; no paired-body visual acceptance.'}
    c.update(scope=scope,visualReview=review); cases.append(c); wr(T/'visual-review-v972.json',dict(**review,completePairedChoreographyAccepted=False))
    for role in ('host','client'):
        path=f'{take}/{role}.mp4'; m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0];a,b=map(float,m['avg_frame_rate'].split('/'))
        videos=[v for v in videos if v['video']!=path]; videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'NPC shared combat / {take} / {role}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2),container_fps=m['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review=review[role],candidate=A.name,dll=pin['dll'],runtime_test_passed=True))
at=datetime.datetime.now().astimezone().isoformat(); limits='Active enemy perception/attacks on both players, unrestricted native modifiers, projectile/physics ownership, sustained stability (including unresolved crash961), full campaign/movement/native session UI and separate-account Steam validation remain open.'
wr(W/'npc-dual-audit-v972.json',dict(at=at,candidate=A.name,dll=pin['dll'],cases=cases,complete=False,limitations=limits));wr(N/'runtime-video-manifest.json',videos)
pin.update(runtimeVerified=False,scopedBothRoleNonterminalReloadVerified=True,scopedTwoSourceContactsVerified=True,runtimeEvidence='../npc-dual-audit-v972.json',scope=limits);wr(A/'identity.json',pin)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['NpcCombatContinuation'].update(at=at,receipt='work/movement-integration/npc-dual-audit-v972.json',latest='Measured-camera host970 passes living hit and full reload. Dual971: two ordinary inputs12ms apart produce two ordered contacts40-to25-to10, one client request, duplicates and shared checkpoint/rearm; settings/saves restored. '+limits,next='Extend enemy perception from the fixed-guard experiment to currently bound host NPCs, with per-entity override ownership and cleanup. Then active targeting and enemy-to-player outcomes.');s['MovementExecutionPriority']['current']=s['NpcCombatContinuation']['latest'];wr(R/'work/current-task-state.json',s)
b=rd(R/'work/handoff-measured-aim-bookmark-v970.json');assert json.loads(b['saved']['structuredContent']['result'])['success']
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
    p=R/'work'/name;items=rd(p)
    for e in b['bookmarks']:items=[x for x in items if (x.get('address'),x.get('category'))!=(e['address'],e['category'])];items.append(dict(e,type='Note'))
    wr(p,items)
print(json.dumps(dict(audit='npc-dual-audit-v972.json',recordings=len(videos),complete=False)))
