param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='stance-emily-video-1',[ValidateSet('corvo','emily')][string]$Character='emily',[ValidatePattern('^[a-z0-9-]+\.json$')][string]$Manifest='stance-retarget-manifest.json',[ValidatePattern('^[a-z0-9_]+$')][string]$Alias='crouch_sequence',[switch]$KeepCamera)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$observer=if($Character-eq'emily'){'host'}else{'client'};$source=if($observer-eq'host'){'client'}else{'host'}
function C($r,$s){& "$lab/Send-D2Command.ps1" -Role $r -Command $s -WaitConsumed|Out-Null}
$failure=$null;$rows=[Collections.Generic.List[object]]::new();$start=0
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne("$root/work/asset-game/Dishonored2.exe".Replace('/','\'))){throw 'Wrong process'}
  & "$lab/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null
  C $r 'coop_review_clip off';C $r 'coop_movement_bank off';C $r 'coop_authoring_proof off';C $r 'coop_shadow_stream off';C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on'
 }
 $clip=Get-Content (Join-Path $PSScriptRoot $Manifest) -Raw|ConvertFrom-Json|Where-Object {$_.role-eq$Character-and$_.selection.alias-eq$Alias}
 if(!$clip-or@($clip).Count-ne1){throw 'Clip not unique'}
 Copy-Item -LiteralPath (Join-Path $PSScriptRoot $clip.binary) -Destination "$root/work/lab-instances/$observer/coop-status.json.review.bin" -Force
 if(!$KeepCamera){C $source 'setviewpos -33 -153 68.1 0 0';C $observer 'setviewpos -28.5 -151.2 68.1 202 14'}
 $deadline=[DateTime]::UtcNow.AddSeconds(45)
 while(!(Test-Path "$out/record.ready")){if([DateTime]::UtcNow-gt$deadline){throw 'Recorder not ready'};Start-Sleep -Milliseconds 50}
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();C $observer 'coop_review_clip play';$watch=[Diagnostics.Stopwatch]::StartNew()
 while($watch.Elapsed.TotalSeconds-lt($clip.duration+2)){
  $rows.Add([pscustomobject]@{time=$watch.ElapsedMilliseconds;state=(Get-Content "$root/work/lab-instances/$observer/coop-status.json.review.json" -Raw|ConvertFrom-Json)});Start-Sleep -Milliseconds 60
 }
 if(($rows.state.applied|Measure-Object -Maximum).Maximum-lt10){throw 'No authored poses applied'}
 if(($rows.state.seconds|Measure-Object -Maximum).Maximum-lt($clip.duration-.15)){throw 'Review lease ended before the authored sequence finished'}
 if(($rows.state.refused|Measure-Object -Maximum).Maximum-gt0){throw 'Native pose publication refused a frame'}
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{C $r 'coop_review_clip off'}catch{}}
 $rows|ConvertTo-Json -Depth 8|Set-Content "$out/states.json"
 [ordered]@{character=$Character;source=$source;observer=$observer;start=$start;end=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();failure=$failure;manifest=$Manifest;alias=$Alias;dll=(Get-FileHash "$lab/bin/dinput8.dll").Hash;clipSha=$clip.sha256;firstPersonChanged=$false;scope='Actual D2 pose-only sequence on the native remote character. Actor root stays still; this recording evaluates body, native correctives and weapon attachment, not travelling foot contact.'}|ConvertTo-Json -Depth 8|Set-Content "$out/take.json"
 'done'|Set-Content "$out/record.done"
}
if($failure){throw $failure};'REVIEW_RECORDING_COMPLETE'
