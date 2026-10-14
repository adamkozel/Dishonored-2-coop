param([ValidateSet('corvo','emily')][string]$Character='corvo',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='ranged-prop-switch-corvo-v1')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$peer=if($Character-eq'corvo'){'client'}else{'host'};$source=if($peer-eq'client'){'host'}else{'client'}
$pids=@{};$before=@{};$checks=[Collections.Generic.List[string]]::new();$rows=[Collections.Generic.List[object]]::new();$failure=$null;$capture=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $r $mode"}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid
  & "$lab/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null
  foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_review_clip off','coop_ranged_prop off','coop_ranged_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  $before[$r]=Q $r '--player-inventory';if((Q $r '--health' 1).current-ne56){throw 'Living fixture required'}
 }
 C $source 'setviewpos -33.5 -153 68.1 90 0';C $peer 'setviewpos -33.5 -150 68.1 270 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $peer -Pitch 18 -Evidence "$out/camera.json"
 $initial=S $peer '.ranged-prop.json';$initial|ConvertTo-Json|Set-Content -Encoding utf8 "$out/before.json"
 $window=(Get-Process -Id $pids[$peer]).MainWindowHandle.ToInt64();if(!$window){throw 'No observer game window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','18','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $watch=[Diagnostics.Stopwatch]::StartNew();$handles=@{};$sequence=@('gun','crossbow','gun','crossbow','gun','crossbow');$index=0
 foreach($kind in $sequence){
  C $peer 'coop_review_clip off'
  Copy-Item "$PSScriptRoot/crouched-ranged-sources-v3/ranged_crouched_${kind}_idle-$Character.bin" "$root/work/lab-instances/$peer/coop-status.json.review.bin" -Force
  C $peer 'coop_review_clip hold 0';Start-Sleep -Milliseconds 150
  C $peer "coop_ranged_prop $kind";$ready=[Diagnostics.Stopwatch]::StartNew();$expected=if($kind-eq'gun'){1}else{2}
  do{$s=S $peer '.ranged-prop.json';if($s.active-and$s.kind-eq$expected){break};Start-Sleep -Milliseconds 30}while($ready.ElapsedMilliseconds-lt3000)
  if(!$s.active-or$s.kind-ne$expected-or$s.refused-ne$initial.refused){throw "Switch $index failed: $($s.result)"}
  if($index-gt0-and(!$s.parkedHandle-or!$s.parkedHidden-or$s.parkedKind-eq$s.kind)){throw 'Previous weapon was not exclusively hidden'}
  if($handles.ContainsKey($kind)){if($handles[$kind]-ne$s.handle){throw 'Weapon switch created a replacement instead of reusing the cached entity'}}else{$handles[$kind]=$s.handle}
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;selection=$kind;prop=$s})
  if($index-lt2){(Q $peer '--components' $s.handle)|ConvertTo-Json -Depth 12|Set-Content -Encoding utf8 "$out/components-$kind.json"}
  Start-Sleep -Milliseconds 1300;$index++
 }
 $last=S $peer '.ranged-prop.json'
 if($handles.Count-ne2-or$last.created-$initial.created-ne2-or$last.switched-$initial.switched-ne5-or$last.reused-$initial.reused-ne4){throw 'Expected two entities, five switches and four cache reuses'}
 $checks.Add('Six alternating weapon selections reuse exactly two native cosmetic entities in one unchanged world')
 $checks.Add('Every switch hides the previous entity before showing its replacement; parked owner/tag/component identities stay valid')
 C $peer 'coop_ranged_prop off';Start-Sleep -Milliseconds 150;$last=S $peer '.ranged-prop.json'
 if($last.active-or$last.pendingHide-or!$last.parkedHidden-or$last.hidden-$initial.hidden-ne6){throw 'Final hide did not release the active prop and retain the hidden cache'}
 $last|ConvertTo-Json|Set-Content -Encoding utf8 "$out/after.json";$checks.Add('Explicit cleanup hides the final active prop; both cached props remain hidden')
 foreach($r in 'host','client'){
  $after=Q $r '--player-inventory'
  if(Compare-Object @($before[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($after.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory changed'}
  if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Player or avatar health changed'}
 }
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
 $checks.Add('Both player inventories, player/avatar health and protected saves remain unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{C $r 'coop_ranged_prop off';C $r 'coop_review_clip off';C $r 'coop_movement_fixture off'}catch{}}
 if($capture){$capture.WaitForExit(15000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Capture incomplete'}}
 $rows|ConvertTo-Json -Depth 8|Set-Content -Encoding utf8 "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;character=$Character;checks=$checks;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;gameplayBound=$false;visualAccepted=$false;scope='Isolated native cosmetic cache/attachment switch test using held authored body poses. No native source inventory selection, Steam selection transitions, projectiles or gameplay animation acceptance claimed.'}|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/result.json"
}
if($failure){throw $failure};"RANGED_PROP_CACHE_VERIFIED $Character $($checks.Count) checks"
