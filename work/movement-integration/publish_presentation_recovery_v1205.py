from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
takes=[('mission-presentation-resume-v1204','candidate-npc-combat-v1203','Whole-presentation watchdog re-ran follow+bank on both peers; every bank (re)load stalled the committing engine thread ~1-2 s (full-bank bind check over ~270 MB), the other peer went stale; ping-pong (7 repairs each).'),
       ('mission-presentation-resume-v1204b','candidate-npc-combat-v1203b','Bank commit 0.5 ms and in-place reactivation removed the stalls; the shadow stream never stayed ready: its token handshake rejected a unilaterally restarted peer and the two watchdogs restarted at different times.'),
       ('mission-presentation-resume-v1204c','candidate-npc-combat-v1203c','Handshake re-pin/peer-hello restart re-established the stream each time and the controller armed (13 quick repairs), but streams retired within ~1 s: retireReason 2 at retireRootAge 250 - partner pose gaps >=250 ms every few seconds.'),
       ('mission-presentation-resume-v1204d','candidate-npc-combat-v1203d','Product presentation freshness 1 s: arming stable (1 quick repair); failed on a post-arming 60 m harness warp (host partner left 60 m off).'),
       ('mission-presentation-resume-v1204e','candidate-npc-combat-v1203d','Players staged before arming: facing live and client pause passed; host pause failed - the client saw 1 s+ stale gaps (host send queue) and its repair backoff had grown to 48 s.'),
       ('mission-presentation-resume-v1204f','candidate-npc-combat-v1203e','PASS: bulk congestion gate + 3-15 s backoff. Partner presentation (avatar at the remote player, follow, movement bank, shadow stream) restored by itself after the client pause (19.0 s), host pause (17.4 s) and the native tutorial modal (14.5 s), then followed a 3 m move each time; also recovered after a 60 m warp (15.5 s).')]
cases=[]
for take,cand,scope in takes:
    T=N/take;r=rd(T/'result.json');rest=rd(T/'restoration.json')
    cases.append(dict(take=f'../natural-moveset/{take}',candidate=cand,passed=r['passed'],failure=r.get('failure'),restoration=rest['passed'],scope=scope))
assert cases[-1]['passed'] and all(c['restoration'] for c in cases)
F=N/'mission-presentation-resume-v1204f'
end={r:rd(F/f'product-end-{r}.json') for r in ('host','client')}
warp=rd(F/'warp-diagnostic.json')
stuck=[(w['leg'],w['ms'],w['host']['distance']) for w in warp if 'host' in w and w['host']['distance']>10]
at=datetime.datetime.now().astimezone().isoformat()
C=rd(W/'candidate-npc-combat-v1203e/identity.json')
wr(W/'presentation-recovery-audit-v1205.json',dict(at=at,candidate='candidate-npc-combat-v1203e',dll=C['dll'],harness='../natural-moveset/Test-PresentationResume-v1204.ps1',cases=cases,
  endCounters={r:{k:end[r].get(k) for k in ('repairs','quickRepairs','bulkCongestionSkips','bulkQueueUsMax')} for r in end},
  warpDiagnostic=dict(samplesOver10m=len(stuck),first=stuck[:3],note='After a 60 m harness warp of the client the host-side partner stayed at the pre-warp spot for 7-15 s while follow/bank were live (movement clock rejected the >10 m/s root step; exact hold mechanism not isolated). Not a gameplay path (no in-world warps; Blink is a powered step; restarts change the world).'),
  findings=['Confirmed runtime (lab, product controller): the partner presentation recovers by itself in the same world after either player\'s native pause menu and after the native tutorial modal, and keeps following (1204f).',
            'Root causes fixed on the way: engine-thread full-bank validation stall (1203b), unilateral stream restarts rejected by the token handshake (1203c), 250/350 ms presentation freshness too strict for jitter (1203d, product only: 1 s/1.5 s), bulk NPC/world traffic delaying partner packets (1203e queue gate, product only).',
            'The movement resume policy suspends on pause but did not complete a restore in these takes (faults on the first transient loss); the controller watchdog performed every restore. Restore latency is 14-19 s - too slow for final quality.',
            'The host send queue still peaked at ~1.05 s with ~81k bulk skips: pending reliable data (~0.45 MB bursts) dominates; attribution pending (1203f send tally).',
            'The facing recordings used spots separated by a wall: they do not show the partner (no visual evidence from this take).'],
  complete=False,outstanding=['Attribute and reduce the reliable backlog (send tally), then cut restore latency (resume policy should restore without faulting).','Visual evidence from line-of-sight spots (mission-start positions).','Cinematic (logic 5) case with a real mid-mission scene.']))
for k,res in (('v1203','superseded'),('v1203b','superseded'),('v1203c','superseded'),('v1203d','superseded')):
    p=W/f'candidate-npc-combat-{k}/identity.json';c=rd(p);c.setdefault('auditEvidence','../presentation-recovery-audit-v1205.json');wr(p,c)
C.update(runtimeVerified=True,scopedPresentationRecoveryVerified=True,runtimeEvidence='../presentation-recovery-audit-v1205.json',
  result='1204f pass: product partner presentation restored by itself after pause (both roles), tutorial modal and a 60 m warp; restore 14-19 s; reliable backlog remains.')
wr(W/'candidate-npc-combat-v1203e/identity.json',C)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1203e live: product partner presentation self-recovers after pause/modal (1204f, restore 14-19 s) after fixing bank-commit stalls, stream handshake re-pin, product freshness and a bulk congestion gate. Lifecycle 1199c (map load + shared restart, line-of-sight filming) running.'
s['MovementExecutionPriority']['next']='Attribute the reliable send backlog (1203f send tally) and cut restore latency; window_enter_exit probe (1201); cutscene_enter_exit; remaining stage-3 items.'
wr(R/'work/current-task-state.json',s);print('ok',len(stuck))
