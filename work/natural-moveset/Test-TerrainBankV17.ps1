param([ValidateSet('host','client')][string]$Source='host',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='terrain-corvo-v17-v1',[switch]$Record,[switch]$AuthoredFeet,[switch]$Unarmed,[switch]$CloseCamera)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$target=if($Source-eq'host'){'client'}else{'host'};$character=if($Source-eq'host'){'corvo'}else{'emily'};$pids=@{};$handles=@{};$rows=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$y=0,$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -DurationMs $ms -LeftY $y|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$id=0){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if($LASTEXITCODE){throw 'Native read failed'};return($raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
  $pids[$r]=$l.pid;$handles[$r]=(S $r).probeActorHandle;I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_power_fixture off'
  C $r 'coop_probe_follow exact';C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'
  if((Q $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
  if((S $r).localPlayerStance-eq1){& "$lab/Send-D2Input.ps1" -Role $r -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 900}
 }
 if($CloseCamera){C $target 'setviewpos -44.3 -176.5 65.1 300 0'}else{C $target 'setviewpos -47.6 -175.9 65.1 325 0'};C $Source 'setviewpos -45.3 -179 65.1 0 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $target -Pitch -3 -Evidence "$out/observer-camera.json"
 Copy-Item "$PSScriptRoot/terrain-banks-v2/$character-terrain-bank.bin" "$root/work/lab-instances/$target/coop-status.json.movement.bin"
 foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 1000;C $target ('coop_movement_feet '+$(if($AuthoredFeet){'authored'}else{'off'}));C $target 'coop_movement_bank on'
 $w=[Diagnostics.Stopwatch]::StartNew();do{$m=S $target '.movement.json';if($m.active-and$m.version-eq17-and$m.weight-gt.9){break};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt5000)
 if(!$m.active-or$m.weight-lt.9){throw 'Movement bank not ready'};$checks.Add('Native Steam and both native body/weapon identities ready on safe-room stairs')
 if($Unarmed){
  & "$lab/Send-D2Input.ps1" -Role $Source -Button X -DurationMs 1500|Out-Null;Start-Sleep -Milliseconds 3200
  $m=S $target '.movement.json'
  if($m.armedMix-gt.01-or(Q $Source '--player-sword').equipped-or(Q $target '--avatar-sword' $handles[$target]).equipped){throw 'Native source and remote sword did not holster'}
  $checks.Add('Native hands-free stance replicated; unarmed full-body bank active')
 }
 if($Record){
  $window=(Get-Process -Id $pids[$target]).MainWindowHandle.ToInt64();$args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','24','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
  $capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru;Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}
 }
 $overall=[Diagnostics.Stopwatch]::StartNew()
 foreach($trial in 'up','down'){
  if($trial-eq'down'){C $Source 'setviewpos -37.5 -179 68.1 180 0';Start-Sleep -Milliseconds 1400}
  I $Source 18000 8000;$w.Restart();$part=[Collections.Generic.List[object]]::new();$sampled=$false
  while($w.ElapsedMilliseconds-lt9600){
   if($w.ElapsedMilliseconds-gt8000){I $Source}
   $m=S $target '.movement.json';$s=S $Source;$remote=S $target;$native=Q $Source '--player-states'
   $row=[pscustomobject]@{trial=$trial;ms=$w.ElapsedMilliseconds;videoTime=(.7+$overall.Elapsed.TotalSeconds);source=$s;target=$remote;native=$native;movement=$m};$rows.Add($row);$part.Add($row)
   if(!$m.active){throw 'Movement bank retired on staircase'}
   if(!$sampled-and$w.ElapsedMilliseconds-ge2300){
    $components=Q $target '--components' $handles[$target];$anim=($components.components|Where-Object vtableRva -eq '0x1b80408').pointer
    $runtime=Q $target '--object' ('0x{0:x}' -f ([Convert]::ToUInt64($anim.Substring(2),16)+0x2d08));$feet=$runtime.words[0].word
    @{runtime=$runtime;feet=@((Q $target '--object' $feet),(Q $target '--object' ('0x{0:x}' -f ([Convert]::ToUInt64($feet.Substring(2),16)+0x280))))}|ConvertTo-Json -Depth 7|Set-Content "$out/feet-$trial.json"
    for($attempt=0;$attempt-lt8;$attempt++){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$target] --skeleton-final $handles[$target] $remote.bodyPublicationAddress;if(!$LASTEXITCODE){$sk=$raw|ConvertFrom-Json;if($sk.stable-and$sk.finalPublicationComplete){$raw|Set-Content "$out/pose-$trial.json";$sampled=$true;break}}}
   }
   Start-Sleep -Milliseconds 30
  }
  $heights=@($part|ForEach-Object {$_.source.localPosition[2]});$change=[double]$heights[-1]-[double]$heights[0]
  if(($trial-eq'up'-and$change-lt1.5)-or($trial-eq'down'-and$change-gt-1.5)){throw "Did not traverse staircase: $trial change=$change"}
  if(@($part|Where-Object {$_.movement.weight-lt.85}).Count-gt5){throw 'Movement weight dropped repeatedly on stairs'}
  $engaged=@($part|Where-Object {$_.movement.terrainWeight-gt.85-and$(if($trial-eq'up'){$_.movement.terrainDown-lt.1}else{$_.movement.terrainDown-gt.9})})
  if($engaged.Count-lt10){throw "Native $trial stair gait not selected for enough samples: $($engaged.Count)"}
  if($m.terrainWeight-gt.08){throw 'Stair stepping continued after physical movement stopped'}
  $checks.Add("$trial gait selected from delayed root grade for $($engaged.Count) samples and stopped without extra steps")
  if(!$sampled){throw 'No complete pose sample'};$checks.Add("$character $trial traversed native staircase with active body bank and complete pose sample; height change $change")
 }
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}}
 if((Q $Source '--player-sword').equipped-eq[bool]$Unarmed-or(Q $target '--avatar-sword' $handles[$target]).equipped-eq[bool]$Unarmed){throw 'Sword attachment state differs'};$checks.Add('Native health and sword attachments preserved')
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}};$checks.Add('Original Steam files and protected saves unchanged')
 if($capture){$capture.WaitForExit(10000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){throw 'Capture incomplete'}}
}catch{$failure=$_.Exception.Message}finally{
 if($Unarmed){try{& "$lab/Send-D2Input.ps1" -Role $Source -Button X -DurationMs 650|Out-Null;Start-Sleep -Milliseconds 2200}catch{}}
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 if($capture-and!$capture.HasExited){$capture.WaitForExit(20000)|Out-Null}
 $rows|ConvertTo-Json -Depth 14|Set-Content "$out/states.json"
 [ordered]@{source=$Source;character=$character;failure=$failure;checks=$checks;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;nativeFeet=[bool]$AuthoredFeet;unarmed=[bool]$Unarmed;closeCamera=[bool]$CloseCamera;bank=(Get-FileHash "$PSScriptRoot/terrain-banks-v2/$character-terrain-bank.bin").Hash;recorded=[bool]$Record;scope='Actual Imperial Safe Room staircase in current Dunwall map; native player physics owns height. Dedicated measured native up/down cycles selected by filtered root grade; native casts correct authored clone feet. Full terrain and visual acceptance remain separate.'}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_TERRAIN_FEET $character $($checks.Count) checks"
