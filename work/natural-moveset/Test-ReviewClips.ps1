param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='review-test-1',[ValidatePattern('^[a-z0-9-]+\.json$')][string]$Manifest='native-retarget-manifest.json',[string[]]$Only=@())
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
function B($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function ReviewState($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.review.json" -Raw|ConvertFrom-Json}
function C($r,$s){& "$lab/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
function I($r,$button='None'){if($button-eq'None'){& "$lab/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null}else{& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs 200|Out-Null}}
$cases=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne"$root/work/asset-game/Dishonored2.exe".Replace('/','\')){throw 'Wrong isolated process'}
  I $r;C $r 'coop_review_clip off';C $r 'coop_movement_bank off';C $r 'coop_authoring_proof off';C $r 'coop_shadow_stream off';C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on'
 }
 $clips=Get-Content (Join-Path $PSScriptRoot $Manifest) -Raw|ConvertFrom-Json
 if($Only.Count){$clips=@($clips|Where-Object {$_.selection.alias -in $Only});if($clips.Count-ne$Only.Count*2){throw 'Expected both roles for every requested alias'}}
 foreach($clip in $clips){
  $r=if($clip.role-eq'corvo'){'client'}else{'host'};$s=B $r;$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  C $r 'coop_review_clip off';Copy-Item -LiteralPath (Join-Path $PSScriptRoot $clip.binary) -Destination "$root/work/lab-instances/$r/coop-status.json.review.bin" -Force
  $beforeRoot=$s.probeRenderPosition;$frame=[int][Math]::Floor(($clip.frames-1)/2)
  C $r "coop_review_clip hold $frame";Start-Sleep -Milliseconds 1100;$active=ReviewState $r
  if(!$active.active-or$active.applied-lt5-or$active.refused){throw "Review did not apply $($clip.id)"}
  $file="$out/$($clip.id)-hold.json"
  & "$root/work/fullbody-proof/build/Release/d2_capture_pose.exe" $l.pid $s.probeActorHandle 1 $file actor $s.bodyPublicationAddress|Out-Null
  if($LASTEXITCODE){throw 'Final-published pose capture failed'}
  $after=B $r;$travel=[Math]::Sqrt([Math]::Pow($after.probeRenderPosition[0]-$beforeRoot[0],2)+[Math]::Pow($after.probeRenderPosition[1]-$beforeRoot[1],2))
  if($travel-gt.02){throw 'Pose-only review changed world root'}
  C $r 'coop_review_clip off';Start-Sleep -Milliseconds 300;$cancelled=ReviewState $r;Start-Sleep -Milliseconds 200
  if($cancelled.active-or(ReviewState $r).applied-ne$cancelled.applied){throw 'Review writes continued after cancellation'}
  C $r 'coop_review_clip play';Start-Sleep -Milliseconds 850;$play=ReviewState $r
  if(!$play.active-or$play.applied-lt1){throw 'Review playback did not start'}
  Start-Sleep -Milliseconds ([int]($clip.duration*1000+650));$finished=ReviewState $r
  if($finished.active-or$finished.refused){throw 'Review playback failed to expire cleanly'}
  $cases.Add([pscustomobject]@{id=$clip.id;role=$clip.role;alias=$clip.selection.alias;frame=$frame;hold=$active;finished=$finished;rootTravel=$travel;poseFile=$file;bankSha=(Get-FileHash "$root/work/lab-instances/$r/coop-status.json.review.bin").Hash})
 }
 # Pause interrupts the presentation lease; resuming does not replay an action.
 C 'client' 'coop_review_clip hold 0';Start-Sleep -Milliseconds 900
 if(!(ReviewState 'client').active){throw 'Pause test did not arm'}
 I 'host' 'Start';Start-Sleep -Milliseconds 700
 if((ReviewState 'client').active){throw 'Peer pause did not retire review'}
 $checks.Add('Peer native pause retires review playback')
 I 'host' 'Start';Start-Sleep -Milliseconds 900
 if((ReviewState 'client').active){throw 'Review resumed without a new request'}
 $checks.Add('Native resume leaves review disabled')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_review_clip off';C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on'}catch{}}
 [ordered]@{passed=(!$failure);failure=$failure;manifest=$Manifest;cases=@($cases);checks=@($checks);scope='Pose-only library review: fixed frame, native final publication, original root, cancellation, expiry and pause. No gameplay action binding or visual acceptance claim. Hands-free donor equipment has not been bound by this test.';dll=(Get-FileHash "$lab/bin/dinput8.dll").Hash}|ConvertTo-Json -Depth 12|Set-Content "$out/result.json"
}
[pscustomobject]@{Passed=(!$failure);Failure=$failure;Cases=$cases.Count;Checks=$checks.Count}
if($failure){throw $failure}
