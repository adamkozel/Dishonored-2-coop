from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';A=W/'candidate-npc-combat-v1012'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
pin=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
old=rd(W/'candidate-npc-combat-v1002/identity.json')
videos=rd(N/'runtime-video-manifest.json');cases=[]
plan=[('local-native-block-v1011',old,'Harness failure before gameplay sampling: PowerShell alias H shadowed the handle helper.'),
      ('local-native-block-v1011b',old,'Harness failure before gameplay sampling: probed unspawned audience NPCs.'),
      ('local-native-block-v1011c',old,None),
      ('npc-native-block-v1013',pin,'Harness failure before any relayed contact: PowerShell 7 read-only $Error assignment in the aim helper. Restoration passed.'),
      ('npc-native-block-v1013b',pin,'Harness failure: run under Windows PowerShell 5.1, whose File.Move overload is missing; commands and fixture restore failed and its protected-file report was a 5.1 false positive. Independent PowerShell 7 recheck found all 14 protected files unchanged; fixture was restored before 1013c.'),
      ('npc-native-block-v1013c',pin,None)]
for take,localPin,failureScope in plan:
    T=N/take;r=rd(T/'result.json')
    if take=='local-native-block-v1011c':
        passed=True;scope='Local native ground truth (no relay, NPC stream off, build1002): held block against three engaged native enemies for ~12s, health56 unchanged, zero npc_sword damage calls; two zero-amount npc_closecombat calls. Observation, not a co-op acceptance.'
        summary=dict(healthBefore=56,healthAfter=rd(T/'health-after.json')['current'],npcSwordDamageCalls=0,closeCombatZeroCalls=len(rd(T/'damage.json')['events']))
    elif take=='npc-native-block-v1013c':
        v=rd(T/'verified.json');assert r['passed'] and v['passed'] and rd(T/'restoration.json')['passed'];passed=True;scope=v['scope']
        d=rd(T/'defense-results.json');summary=dict(d['blocked'][0],nativeBlockSamples=d['nativeBlockSamples'],duplicateRejected=True)
    else:
        passed=False;scope='Failed harness run: '+failureScope;summary=dict(failure=r.get('failure'))
    cases.append(dict(take=take,candidate=localPin['dll'],passed=passed,scope=scope,result=summary))
    for role in ('host','client'):
        path=f'{take}/{role}.mp4'
        if not (N/path).exists() or (N/path).stat().st_size==0:continue
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        review='Raw native diagnostic, visual review not yet entered.'
        if (T/'visual-review.json').exists():review=rd(T/'visual-review.json')[role]['observed']+' Limits: '+rd(T/'visual-review.json')[role]['limits']
        elif take=='local-native-block-v1011c':review='Viewed 20-frame sheet: Emily holds block as Ramsey and two guards attack; Defense Blocking tutorial appears; health unchanged; final frames after scripted retreat.'
        videos=[x for x in videos if x['video']!=path];videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Native combat defense / {take} / {role}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review=review,candidate=A.name if localPin is pin else 'candidate-npc-combat-v1002',dll=localPin['dll'],runtime_test_passed=passed))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'npc-block-audit-v1014.json',dict(at=at,candidate=A.name,dll=pin['dll'],findings='npc-block-native-v1011.json',cases=cases,complete=False,
   outstanding=['Timed parry (parry-lose/parry-win) not exercised; parry-win attacker stagger (636140) is not mirrored to the host.','Host attacker block reaction is a posted native event count, not visually verified.','Heavy/unblockable attacks, side/rear arcs under movement and latency, multiple simultaneous attackers, other damage families (npc_closecombat kicks) remain unverified.']))
wr(N/'runtime-video-manifest.json',videos)
pin.update(runtimeVerified=True,scopedEnemySwordRelayVerified=True,scopedHeldBlockVerified=True,runtimeEvidence='../npc-block-audit-v1014.json',
   scope='1012: receiver-native held block passes1013c (two relayed Ramsey contacts natively blocked, no 717950 call, health56 kept, host acks Blocked x2 and posts two native 0xe6 attacker events, duplicate rejected, clone40). Timed parry, parry-win attacker stagger, heavy attacks and broader combat remain incomplete.')
wr(A/'identity.json',pin)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['NpcCombatContinuation'].update(at=at,candidate=A.name,receipt='work/movement-integration/npc-block-audit-v1014.json',details='documentation/handoff-2026-10-11/79-enemy-combat-recovery-and-defense.md',
   latest='1012 passes held-block1013c: relayed enemy sword contacts resolved by the native pre-damage block path on the actual remote player; both blocked, health unchanged, host attacker events posted. Local control1011c confirms native block never reaches 717950. Harness failures1011/1011b/1013/1013b retained.',
   inProgress='Native timed parry and host attacker response.',next='Timed parry incl. parry-win stagger on host, visual check of host attacker reaction, heavy attacks, close-combat kicks, menu continuity, other damage families and broader campaign. Full playable co-op remains unfinished.')
s['MovementExecutionPriority']['current']=s['NpcCombatContinuation']['latest'];wr(R/'work/current-task-state.json',s)
print(json.dumps(dict(recordings=len(videos),cases=[(c['take'],c['passed']) for c in cases]),indent=2))
