param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='partner-motion-v2082a',[string]$Candidate='candidate-animsample-v2082',[switch]$Twin)
# 2082 (#243): partner movement quality in numbers on the local lab (the Dunwall fixture, both games in gameplay).
# For each direction (the client moves while the host observes its rig, then the reverse), the mover runs seven cases
# from the same start pose: walk, run, sprint, turn on the spot, stop and start, crouch walk, jump. The observer samples
# the mover's rig with d2_runtime_probe --anim-sample (its probeActorHandle) and tools/diag/d2_anim_quality.py measures
# foot skate, jitter, snaps and rubber-banding. -Twin turns the observer's pose smoothing off (coop_probe_smoothing off;
# must go red). The lab is restored (PrepareShadowSword and the note inventory) at the end.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$rows=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode;if($LASTEXITCODE){throw "probe $mode failed ($LASTEXITCODE)"};$raw|ConvertFrom-Json}
function I($r,[hashtable]$a,[int]$ms){$p=@{Role=$r;DurationMs=$ms}+$a;& "$lab/Send-D2Input.ps1" @p|Out-Null}
$cases=@(
 @{name='walk';inputs=@(@{a=@{LeftY=16000};ms=3000})},
 @{name='run';inputs=@(@{a=@{LeftY=32000};ms=3000})},
 @{name='sprint';inputs=@(@{a=@{LeftY=32000;Button='LeftThumb'};ms=300},@{a=@{LeftY=32000};ms=2700})},
 @{name='turn';inputs=@(@{a=@{RightX=24000};ms=2500})},
 @{name='stopstart';inputs=@(@{a=@{LeftY=32000};ms=1000},@{a=@{};ms=700},@{a=@{LeftY=32000};ms=1000},@{a=@{};ms=700},@{a=@{LeftY=32000};ms=1000})},
 @{name='crouchwalk';inputs=@(@{a=@{Button='B'};ms=200},@{a=@{LeftY=20000};ms=3000},@{a=@{Button='B'};ms=200})},
 @{name='jump';inputs=@(@{a=@{LeftY=24000;Button='A'};ms=300},@{a=@{LeftY=24000};ms=1500})}
)
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw "$r not in gameplay"}}
 $pin=Get-Content "$PSScriptRoot/$Candidate/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected the $Candidate DLL"}
 Save "$out/identity.json" @{candidate=$Candidate;dll=$pin.dll;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;twin=[bool]$Twin;pids=$pids}
 foreach($r in 'host','client'){if(!(S $r).remoteFresh){throw "Transport not fresh ($r)"}}
 $dirty=$true
 foreach($mover in 'client','host'){$observer=if($mover-eq'client'){'host'}else{'client'}
  if($Twin){C $observer 'coop_probe_smoothing off'}
  $start=(Q $mover '--player').candidatePosition
  foreach($case in $cases){$tag="$mover-$($case.name)"
   C $mover ("setviewpos {0:F3} {1:F3} {2:F3} 90 0"-f$start[0],$start[1],([double]$start[2]+1.7));Start-Sleep -Milliseconds 900
   $handle=(S $observer).probeActorHandle;if(!$handle){throw "No probe actor on $observer"}
   $total=($case.inputs|ForEach-Object {$_.ms}|Measure-Object -Sum).Sum+1500
   $p=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids[$observer],'--anim-sample',$handle,$total) -RedirectStandardOutput "$out/$tag.jsonl" -RedirectStandardError "$out/$tag.err" -WindowStyle Hidden -PassThru
   Start-Sleep -Milliseconds 300
   foreach($step in $case.inputs){I $mover $step.a $step.ms;Start-Sleep -Milliseconds $step.ms}
   if(!$p.WaitForExit($total+10000)){Stop-Process -Id $p.Id;throw "Sampler did not finish ($tag)"}
   $res=& python -I "$root/tools/diag/d2_anim_quality.py" "$out/$tag.jsonl" --out "$out/$tag-quality.json"|Select-Object -Last 1|ConvertFrom-Json
   $rows.Add([ordered]@{mover=$mover;observer=$observer;case=$case.name;status=$res.status;samples=$res.samples;hz=$res.hz;travel=$res.travel;contacts=$res.contacts;footSkateP95=$res.footSkateP95;footSkateMax=$res.footSkateMax;jitter=$res.jitterEpisodes;snaps=$res.snaps;rubberBands=$res.rubberBands;failures=$res.failures})
   Save "$out/rows.json" @($rows)}
  if($Twin){C $observer 'coop_probe_smoothing on'}
  C $mover ("setviewpos {0:F3} {1:F3} {2:F3} 90 0"-f$start[0],$start[1],([double]$start[2]+1.7))}
 $failed=@($rows|Where-Object {$_.status-ne'PASS'})
 Save "$out/summary.json" ([ordered]@{cases=$rows.Count;passed=@($rows|Where-Object status -eq 'PASS').Count;failed=@($failed|ForEach-Object {"$($_.mover)-$($_.case): $($_.status) $($_.failures -join ',')"})})
 if($failed.Count-and!$Twin){throw "Movement quality failed in $($failed.Count) of $($rows.Count) cases"}
 if($Twin-and!$failed.Count){throw 'Twin: every case passed with pose smoothing off'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($dirty){
   foreach($r in 'host','client'){try{C $r 'coop_probe_smoothing on'}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$root/work/natural-moveset/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;if($s.localGameLogicState-ne1-or!$s.remoteFresh){throw 'Final gameplay/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;twin=[bool]$Twin;rows=@($rows);scope='Partner movement quality on the local lab (2082, #243): seven cases per direction, the observer samples the mover rig (--anim-sample), d2_anim_quality.py measures foot skate, jitter, snaps and rubber-banding.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
