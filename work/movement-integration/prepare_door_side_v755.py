from pathlib import Path
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'
p=R/'outputs/d2-coop-lab/src/door_sync.h'; t=p.read_text()
old=r'\"nativeCalls\":%llu,\"converged\":%llu,'
assert t.count(old)==1
t=t.replace(old,r'\"nativeCalls\":%llu,\"converged\":%llu,\"sideCorrections\":%llu,')
assert t.count('s.calls,s.converged,s.result')==1
p.write_text(t.replace('s.calls,s.converged,s.result','s.calls,s.converged,s.sideCorrections,s.result'))
for kind in ['Build','Deploy']:
    t=(W/f'{kind}-CheckpointRetry-v742.ps1').read_text().replace('CheckpointRetry','DoorSide').replace('checkpoint-retry','door-side').replace('checkpoint_retry','door_side').replace('742','755')
    (W/f'{kind}-DoorSide-v755.ps1').write_text(t)
t=(W/'stage_checkpoint_retry_v742.py').read_text().replace('CheckpointRetry','DoorSide').replace('checkpoint-retry','door-side').replace('checkpoint_retry','door_side').replace('742','755')
t=t.replace("P=W/'candidate-door-side-v740'","P=W/'candidate-checkpoint-retry-v742'")
start=t.index("scope='"); end=t.index("')",start)
t=t[:start]+"scope='Native ordinary door convergence now checks actual Open side. Wrong-side Open closes using the actual native side and reopens using the latest host order, suppressing intermediate correction as local input. No transform or native state-field copying. Same protocol69/bank67/357 and checkpoint retry742. Runtime pending."+t[end:]
(W/'stage_door_side_v755.py').write_text(t)
t=(N/'Test-DoorSimultaneous-v754.ps1').read_text().replace('Simultaneous754','Simultaneous756').replace('sides-v754','sides-host-v756').replace('candidate-checkpoint-retry-v742','candidate-door-side-v755').replace('archived742','archived755')
anchor="   $catalogAfter=@{host=(DAll host);client=(DAll client)};"
assert t.count(anchor)==1
t=t.replace(anchor,"   if(($ds.sideCorrections+$dp.sideCorrections)-ne1-or($ds.nativeCalls+$dp.nativeCalls)-ne2){throw 'Expected exactly one native close/reopen side correction'}\n"+anchor)
(N/'Test-DoorSimultaneous-v756.ps1').write_text(t)
print('Prepared candidate755 build/deploy and756 simultaneous side verification')
