$ErrorActionPreference='Stop'
$lab=Join-Path $PSScriptRoot '../outputs/d2-coop-lab';$backup=Join-Path $PSScriptRoot 'verified-partner-perception-9639';$dest=Join-Path $lab 'research/milestones/9639'
$hash='963920C046F5B8EBCC3883FD6A6EF8166E21469C532653FC7271312EB1049CCB'
if((Get-FileHash "$backup/bin/dinput8.dll").Hash-ne$hash){throw 'Archived DLL mismatch'}
New-Item -ItemType Directory -Path $dest -Force|Out-Null
$names=@('partner-relationship','partner-relationship-escape','partner-perception','exact-partner-recovery','shared-checkpoint-client-delay-client','shared-checkpoint-host-delay-host','shared-death-client-health-damage','shared-death-host-health-damage','guard-health','body-stream','baseline-integrity','lab-restoration')
$reports=@(foreach($name in $names){
 $file="$name-verification.json";$r=Get-Content "$lab/research/$file" -Raw|ConvertFrom-Json;$actual=if($r.DllSha256){$r.DllSha256}else{$r.LoaderSha256}
 if(!$r.Passed-or$actual-ne$hash){throw "Wrong evidence: $file"};Copy-Item "$lab/research/$file" $dest -Force
 [ordered]@{Report=$file;Passed=$true;ObservedAt=$r.ObservedAt;Sha256=(Get-FileHash "$dest/$file").Hash}
})
Copy-Item "$PSScriptRoot/body-stream-observations.json","$PSScriptRoot/partner-perception-escape-capture.json" $backup -Force
$summary=[ordered]@{Passed=$true;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=$hash;ProbeSha256=(Get-FileHash "$backup/bin/d2_runtime_probe.exe").Hash;
 Scope='Native single-guard directional faction override and cleanup. The active escape guard selects remote Emily as combat target and starts its attack scheduler; one real sword hit reached her native health callback and was ignored by the story-NPC health definition. Exact partner coordinates survive both shared reloads and natural deaths.';
 Reports=$reports;PartnerTargetedSamples=13;PartnerSwordHits=1;PartnerDamageApplied=0;DamageReplicated=$false;PlayableCoop=$false;OnlineTwoAccounts=$false;SourceBackup='work/verified-partner-perception-9639'}
$summary|ConvertTo-Json -Depth 8|Set-Content "$dest/partner-perception-summary.json"
$state=Get-Content "$PSScriptRoot/current-task-state.json" -Raw|ConvertFrom-Json;$state.UpdatedAt=$summary.ObservedAt
$state.Next='9639 perception milestone archived. Native sword damage reconstruction implemented in source, not yet built or tested. No active game tests.'
$state.WorkInProgress.PendingTest=$state.Next;$state.WorkInProgress.Next=$state.Next
$state|Add-Member -NotePropertyName PartnerPerception -NotePropertyValue $summary -Force
$state|ConvertTo-Json -Depth 30|Set-Content "$PSScriptRoot/current-task-state.json"
$summary|Select-Object Passed,DllSha256,PartnerTargetedSamples,PartnerSwordHits,DamageReplicated
