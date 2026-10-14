param([ValidateSet('corvo','emily')][string]$Character='corvo',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-corvo-1',[switch]$AwaitRecording)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$target=if($Character-eq'corvo'){'client'}else{'host'};$source=if($target-eq'host'){'client'}else{'host'}
function State($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function C($r,$s){& "$lab/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
$clips=@(Get-Content "$PSScriptRoot/native-retarget-manifest.json" -Raw|ConvertFrom-Json|Where-Object role -eq $Character)
$events=[Collections.Generic.List[object]]::new();$failure=$null;$start=0
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne("$root/work/asset-game/Dishonored2.exe".Replace('/','\'))){throw 'Wrong isolated process'}
  & "$lab/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null
  C $r 'coop_review_clip off';C $r 'coop_movement_bank off';C $r 'coop_authoring_proof off';C $r 'coop_shadow_stream off';C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_shadow_loco combat'
 }
 C $source 'setviewpos -34.3 -154.8 68.1 0 0';C $target 'setviewpos -30.7 -152.3 68.1 214 14';Start-Sleep -Milliseconds 700
 if($AwaitRecording){$deadline=[DateTime]::UtcNow.AddSeconds(45);while(!(Test-Path "$out/record.ready")){if([DateTime]::UtcNow-gt$deadline){throw 'Recorder not ready'};Start-Sleep -Milliseconds 50}}
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()
 foreach($clip in $clips){
  C $target 'coop_review_clip off';Copy-Item (Join-Path $PSScriptRoot $clip.binary) "$root/work/lab-instances/$target/coop-status.json.review.bin" -Force
  for($repeat=0;$repeat-lt2;$repeat++){
   C $target 'coop_review_clip play';Start-Sleep -Milliseconds 950
   $s=Get-Content "$root/work/lab-instances/$target/coop-status.json.review.json" -Raw|ConvertFrom-Json
   if(!$s.active-or$s.refused-or!$s.applied){throw "Review failed: $($clip.id)"}
   $events.Add([pscustomobject]@{time=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();clip=$clip.id;alias=$clip.source.aliases[0];repeat=$repeat;review=$s})
   Start-Sleep -Milliseconds ([int]($clip.duration*1000+550))
  }
 }
}catch{$failure=$_.Exception.Message}finally{
 C $target 'coop_review_clip off';[ordered]@{character=$Character;source=$source;observer=$target;start=$start;end=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();events=@($events);failure=$failure;scope='Native D2 pose review. World-root translation and rotation intentionally remain controlled by the local player; clips are not gameplay bound.'}|ConvertTo-Json -Depth 8|Set-Content "$out/take.json"
 'done'|Set-Content "$out/record.done"
}
if($failure){throw $failure};'REVIEW_RECORDING_COMPLETE'
