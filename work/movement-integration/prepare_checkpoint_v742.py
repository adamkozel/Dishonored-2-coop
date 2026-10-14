from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
for name in ['Build-CheckpointRetry-v740.ps1','Deploy-CheckpointRetry-v740.ps1','stage_checkpoint_retry_v740.py']:
    s=(W/name).read_text().replace('v740','v742').replace('Build740','Build742').replace('Deploy740','Deploy742')
    s=s.replace("P=W/'candidate-door-catalog-v728'","P=W/'candidate-checkpoint-retry-v740'")
    s=s.replace('Scoped diagnostics capture consumed callbacks and save-I/O state.','Scoped diagnostics capture consumed callbacks and save-I/O state. Explicit isolated one-shot retry_probe appends two fixed native memory-restart commands immediately after fixed SLOT82 dispatch to exercise real save-I/O retry.')
    (W/name.replace('v740','v742')).write_text(s)
s=(N/'Test-DoorReload-v741.ps1').read_text().replace('native-door-reload-client-v741','native-door-retry-probe-client-v743').replace('NativeDoorReload741','NativeDoorRetryProbe743').replace('candidate-checkpoint-retry-v740','candidate-checkpoint-retry-v742').replace('Expected archived740 candidate','Expected archived742 candidate')
a="   C $source 'RestartFromCheckpoint';$inputs.Add"
assert a in s
s=s.replace(a,"""   foreach($r in 'host','client'){
    C $r 'coop_checkpoint_sync retry_probe'
    if(!(RS $r).retryProbeUntil){throw 'Native retry probe arm refused'}
   }
"""+a)
a="    if($reset.continuationAccepted-$resetBefore[$r].continuationAccepted-gt1){throw 'Multiple native memory continuations accepted'}"
assert a in s
s=s.replace(a,a+"""
    if($reset.retryProbeInjected-$resetBefore[$r].retryProbeInjected-ne2-or$reset.continuationDeferred-$resetBefore[$r].continuationDeferred-lt1-or$reset.continuationAccepted-$resetBefore[$r].continuationAccepted-ne1){throw 'Native deferred continuation path was not exercised exactly once to acceptance'}""")
(N/'Test-DoorReload-v743.ps1').write_text(s)
print('Prepared bounded native retry fixture and strict deferred-then-accepted regression')
