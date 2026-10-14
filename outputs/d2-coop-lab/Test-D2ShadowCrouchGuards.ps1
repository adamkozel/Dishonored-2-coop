[CmdletBinding()]
param()
$ErrorActionPreference='Stop';$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$checks=[Collections.Generic.List[string]]::new();$events=[Collections.Generic.List[object]]::new();$failure=$null
function C($r,$s){& "$PSScriptRoot/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function I($r,$s){& "$PSScriptRoot/Send-D2Input.ps1" -Role $r -Button $s -DurationMs 150|Out-Null}
try{
 foreach($r in 'host','client'){C $r 'coop_probe_body off';C $r 'coop_shadow_stream off';C $r 'coop_shadow_action off';if((B $r).localPlayerStance){I $r B}}
 foreach($source in 'host','client'){
  $target=if($source-eq'host'){'client'}else{'host'}
  foreach($r in 'host','client'){C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on'}
  C $source 'setviewpos -33 -153 68.1 90 0';C $target 'setviewpos -28.0921 -153.962 68.1 169 10'
  if(!(B $target).nativeCrouchReferenceReady){throw 'Run full-body crouch test to capture this incarnation first'}
  C $target 'coop_probe_body on';I $source B;Start-Sleep -Milliseconds 1200
  $a=B $target;Start-Sleep -Milliseconds 300;$b=B $target
  if($b.bodyStreamApplied-le$a.bodyStreamApplied){throw 'No live full-body application before pause'}
  I $source Start;Start-Sleep -Milliseconds 800;$paused=B $target;Start-Sleep -Milliseconds 300;$settled=B $target
  $events.Add([pscustomobject]@{Role=$source;Before=$b;Paused=$paused;Settled=$settled;Source=(B $source)})
  if((B $source).localGameLogicState-ne2-or$settled.bodyStreamApplied-ne$paused.bodyStreamApplied){throw 'Native pause did not stop remote pose writes'}
  C $target 'coop_probe_body off';I $source A;Start-Sleep -Milliseconds 400
  if((B $source).localGameLogicState-ne1){throw 'Native resume failed'}
  if((B $source).localPlayerStance){I $source B}
  $checks.Add("$source native pause stops complete crouch pose writes")
 }
 $old=@{host=(B host).localEpoch;client=(B client).localEpoch}
 & "$PSScriptRoot/Restore-D2Lab.ps1" -Doppelganger|Out-Null
 foreach($r in 'host','client'){
  $s=B $r;if($s.localEpoch-eq$old[$r]-or$s.nativeCrouchReferenceReady){throw 'Reload retained a stale native crouch reference'}
 }
 C host 'setviewpos -33 -153 68.1 90 0';C client 'setviewpos -28.0921 -153.962 68.1 169 10'
 $before=B client;C client 'coop_probe_body on';I host B;Start-Sleep -Milliseconds 1100;$after=B client
 if($after.bodyStreamApplied-ne$before.bodyStreamApplied-or$after.nativeCrouchReferenceReady){throw 'Old captured pose reused after reload'}
 $events.Add([pscustomobject]@{Stage='OldReferenceRefused';Before=$before;After=$after})
 $checks.Add('Checkpoint restoration invalidates captured rig; old reference cannot drive the replacement clone')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r Neutral;C $r 'coop_probe_body off';if((B $r).localGameLogicState-eq1-and(B $r).localPlayerStance){I $r B}}catch{}}
}
[ordered]@{At=[DateTimeOffset]::Now.ToString('o');Dll=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;Passed=(!$failure);Failure=$failure;Checks=$checks;Events=$events}|ConvertTo-Json -Depth 12|Set-Content "$PSScriptRoot/research/doppelganger-crouch-guards.json"
[pscustomobject]@{Passed=(!$failure);Failure=$failure;Checks=$checks.Count}
if($failure){throw $failure}
