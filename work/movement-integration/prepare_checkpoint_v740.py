from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(W/'Build-DoorSync-v728.ps1').read_text(encoding='utf-8-sig').replace('v728','v740').replace('Build728','Build740')
s=s.replace('d2_movement_resume_tests *>','d2_movement_resume_tests d2_checkpoint_protocol_tests *>')
s=s.replace("@('d2_movement_resume_tests','resume-tests'))", "@('d2_movement_resume_tests','resume-tests'),@('d2_checkpoint_protocol_tests','checkpoint-tests'))")
(W/'Build-CheckpointRetry-v740.ps1').write_text(s)
s=(W/'stage_door_sync_v728.py').read_text(encoding='utf-8-sig')
s=s.replace("A=W/'candidate-door-catalog-v728'; P=W/'candidate-door-catalog-v725'","A=W/'candidate-checkpoint-retry-v740'; P=W/'candidate-door-catalog-v728'")
s=s.replace("'resume-tests')","'resume-tests','checkpoint-tests')").replace('-v728.log','-v740.log')
s=s.replace("'Build-DoorSync-v728.ps1','Deploy-DoorSync-v728.ps1','stage_door_sync_v728.py'", "'Build-CheckpointRetry-v740.ps1','Deploy-CheckpointRetry-v740.ps1','stage_checkpoint_retry_v740.py'")
start=s.index("scope='Authored ordinary-door catalog:");end=s.index("')",start)
s=s[:start]+"scope='Native checkpoint memory continuation preserves fixed SLOT82 permission when the exact native routine returns AL=0; consumes it at AL=1 or loading entry. Scoped diagnostics capture consumed callbacks and save-I/O state. Same ordinary door catalogue and protocol69/bank67/357. Runtime pending."+s[end:]
(W/'stage_checkpoint_retry_v740.py').write_text(s)
s=(W/'Deploy-DoorSync-v728.ps1').read_text(encoding='utf-8-sig').replace('DoorSyncDeploy728','CheckpointRetryDeploy740').replace('stage_door_sync_v728.py','stage_checkpoint_retry_v740.py').replace('candidate-door-catalog-v728','candidate-checkpoint-retry-v740')
(W/'Deploy-CheckpointRetry-v740.ps1').write_text(s)
s=(N/'Test-DoorReload-v737.ps1').read_text(encoding='utf-8-sig').replace('native-door-reload-host-v737','native-door-reload-client-v741').replace("$Source='host'","$Source='client'").replace('NativeDoorReload737','NativeDoorReload741').replace('candidate-door-catalog-v728','candidate-checkpoint-retry-v740').replace('Expected archived728 candidate','Expected archived740 candidate')
s=s.replace('if($owned){\n  foreach', '''if($owned){
  foreach($r in 'host','client'){
   foreach($suffix in 'checkpoint.json','reset.json','loading.json','loading-queue.json'){
    $trace="$root/work/lab-instances/$r/coop-status.json.$suffix"
    if(Test-Path -LiteralPath $trace){Copy-Item -LiteralPath $trace -Destination "$out/final-$r-$suffix"}
   }
   if($failure){foreach($name in 'engine.log','loader.log'){Copy-Item "$root/work/lab-instances/$r/$name" "$out/failed-$r-$name"}}
  }
  foreach''')
(N/'Test-DoorReload-v741.ps1').write_text(s)
print('Prepared isolated checkpoint continuation build and recorded retry harness')
