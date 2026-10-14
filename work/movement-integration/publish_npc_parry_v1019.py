from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';A=W/'candidate-npc-combat-v1018';B=W/'candidate-npc-combat-v1016'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
pin=rd(A/'identity.json');old=rd(B/'identity.json');base=rd(W/'candidate-npc-combat-v1012/identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
# Live source equals the 1018 archive except the read-only probe source, which
# gained parry table row values afterwards (archived in probe-block-v1016b).
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if not p.is_file():continue
    if p.name=='runtime_probe.cpp':assert sha(p)==sha(W/'probe-block-v1016b/runtime_probe.cpp');continue
    assert sha(p)==sha(A/'src'/p.name),p
probes={'probe-block-v1016':sha(W/'probe-block-v1016/d2_runtime_probe.exe'),'probe-block-v1016b':sha(W/'probe-block-v1016b/d2_runtime_probe.exe')}
videos=rd(N/'runtime-video-manifest.json');cases=[]
plan=[('block-window-v1016',base,'Harness failure before any input: Send-D2Input minimum duration is 100 ms (80 requested). No game state touched.'),
      ('block-window-v1016b',base,'Harness failure before any input: integer array passed through pwsh -File became one value. No game state touched.'),
      ('block-window-v1016c',base,None),
      ('npc-native-parry-v1017',old,'Candidate1016 status defect: new object key parryWin duplicated the existing parryWin counter, so the harness read an object; cleanup also waited only 20 s for 46 s recordings. Restoration of gameplay/health/ownership and protected files passed.'),
      ('npc-native-parry-v1017b',pin,'Validator defect: summed health loss ignored native health regeneration (21 -> 35 before the final read). Per-event health was consistent; 9 contacts natively defended (5 block, 4 timed parry result1), 3 applied, host acks/reactions matched. No parry-win (native chance 0). Restoration passed.'),
      ('npc-native-parry-v1017c',pin,'Validator defect: compared process-lifetime observer counters with per-session counts (deltas matched: 2 block, 1 parry). Restoration passed.'),
      ('npc-native-parry-v1017d',pin,'Validator defect: required host lastInflicted 0 although no parry-win occurred (sentinel -1). 1 block + 2 timed parries, counts consistent. Restoration passed.'),
      ('npc-native-parry-v1017e',pin,'No defended contact: all three relayed contacts landed outside a raised block (600/350 ms cycle) and reached the three-hit stop. Health/acks consistent. Restoration passed.'),
      ('npc-native-parry-v1017f',pin,None)]
for take,localPin,failureScope in plan:
    T=N/take;passed=False
    if take=='block-window-v1016c':
        passed=True;scope='Read-only native parry-window timing (client, build1012, probe --watch-block-state 5 ms): arkPlayerStateBlock+58 opens at block entry and closes ~265 ms later; re-entry cooldown +70 = 0.266 s; linger +74 = 0.'
        summary=dict(records=len(rd(T/'watch.json')['records']),probe=probes['probe-block-v1016'])
    elif take=='npc-native-parry-v1017f':
        r=rd(T/'result.json');v=rd(T/'verified.json');assert r['passed'] and v['passed'] and rd(T/'restoration.json')['passed'];passed=True;scope=v['scope']
        d=rd(T/'defense-results.json');summary=dict(d['blocked'][0],parry=rd(T/'parry-results.json'),nativeBlockSamples=d['nativeBlockSamples'],duplicateRejected=True,inputCycle='900/320 ms')
    else:
        r=rd(T/'result.json') if (T/'result.json').exists() else {};scope='Failed run: '+failureScope;summary=dict(failure=r.get('failure'))
    cases.append(dict(take=take,candidate=localPin['dll'],passed=passed,scope=scope,result=summary))
    for role in ('host','client'):
        path=f'{take}/{role}.mp4'
        if not (N/path).exists() or (N/path).stat().st_size==0:continue
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        review='Raw native diagnostic, visual review not yet entered.'
        if (T/'visual-review.json').exists():vr=rd(T/'visual-review.json')[role];review=vr['observed']+' Limits: '+vr['limits']
        cand={pin['dll']:A.name,old['dll']:B.name}.get(localPin['dll'],'candidate-npc-combat-v1012')
        videos=[x for x in videos if x['video']!=path];videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Native combat timed parry / {take} / {role}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review=review,candidate=cand,dll=localPin['dll'],runtime_test_passed=passed))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'npc-parry-audit-v1019.json',dict(at=at,candidate=A.name,dll=pin['dll'],probes=probes,findings=['npc-parry-ghidra-v1015','../../documentation/handoff-2026-10-11/80-native-timed-parry.md'],cases=cases,complete=False,
   incidents=['The first Build-NpcAttack-v1016 run overwrote shadowwalk-body-{world,action,resume,checkpoint}-tests-v1012.log (unarchived 1012 build logs) with 1016 output at 08:11. 1012 runtime evidence, audit1014, archive, smoke and bank logs are unaffected.'],
   outstanding=['Parry-win (result2) host blowback path is implemented but unexercised: the native parry-win percent row (stat row 67) reads 0 for both characters in this fixture; it needs a fixture whose upgrades/charms give a nonzero chance.','Host attacker parry reaction is supported by posted native 0xe6 counts and the client-side streamed off-balance pose, not a host-camera view.','Relay latency plus 80 ms NPC body interpolation shifts effective parry timing relative to the on-screen cue; not measured.','Heavy/unblockable attacks, close-combat kicks (npc_closecombat), several simultaneous attackers and other enemy damage families.']))
wr(N/'runtime-video-manifest.json',videos)
pin.update(runtimeVerified=True,scopedTimedParryVerified=True,runtimeEvidence='../npc-parry-audit-v1019.json',
   scope='1018: passes timed parry 1017f (8 relayed contacts: 4 native blocks, 1 native timed parry result1, 3 applied; no 717950 call for defended contacts; host acks and 5 native 0xe6 attacker events, zero failures; passive 859980 observer deltas match and show no native replica resolution; duplicate rejected). Parry-win blowback implemented but unexercised (0% native chance in fixture).')
wr(A/'identity.json',pin)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['NpcCombatContinuation'].update(at=at,candidate=A.name,receipt='work/movement-integration/npc-parry-audit-v1019.json',details='documentation/handoff-2026-10-11/80-native-timed-parry.md',
   latest='1018 passes timed parry 1017f: relayed enemy sword contacts resolved by the native block state on the actual remote player, including a native timed parry (result1), with matching host acknowledgements and native attacker events. Native parry window ~265 ms measured; parry-win chance is 0% in this fixture so the implemented host blowback path is unexercised.',
   inProgress='Close-combat (kick/shove) relay to the remote player.',next='npc_closecombat relay, heavy attacks, simultaneous attackers, parry-win fixture, host-camera reaction capture, then broader world/campaign authority. Full playable co-op remains unfinished.')
s['MovementExecutionPriority']['current']=s['NpcCombatContinuation']['latest'];wr(R/'work/current-task-state.json',s)
print(json.dumps(dict(recordings=len(videos),cases=[(c['take'],c['passed']) for c in cases]),indent=2))
