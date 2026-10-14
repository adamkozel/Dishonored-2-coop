param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-bendtime-corvo-v1',[switch]$Crouched,[switch]$Holstered)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$rows=[Collections.Generic.List[object]]::new();$events=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$capture=$null;$start=0
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$trigger=0,$ms=100,$button='Neutral'){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftTrigger $trigger|Out-Null}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Native read $r $mode"}
function S($r,$suffix=''){return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;I $r
  foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_power_fixture off','coop_darkvision_fixture off','coop_bendtime_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  if((Q $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
 }
 C host 'setviewpos -33.5 -148 68.1 90 0';C client 'setviewpos -28 -151 68.1 160 0'
 if((Q host '--player-states').stance-ne[int][bool]$Crouched){I host 0 100 B;Start-Sleep -Milliseconds 700}
 if((Q host '--player-sword').equipped-eq[bool]$Holstered){$duration=if($Holstered){1500}else{650};I host 0 $duration X;Start-Sleep -Milliseconds 2300}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 0 -Evidence "$out/camera.json"
 $before=Q host '--player-bendtime';$inventory=Q host '--player-inventory';$peerBefore=Q client '--player-inventory'
 $before|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/before.json";$inventory|ConvertTo-Json -Depth 8|Set-Content -Encoding utf8 "$out/inventory-before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Native BendTime already owned or active'}
 C host 'coop_bendtime_fixture 20000';$w=[Diagnostics.Stopwatch]::StartNew();do{$s=S host '.bendtime-fixture.json';if($s.result-eq'native_bendtime_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6000)
 if($s.result-ne'native_bendtime_ready'){throw "BendTime did not equip: $($s.result)"}
 $power=Q host '--player-bendtime';$power|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/enabled.json"
 if(!$power.equipped-or!$power.started-or!$power.instantiated-or$power.quantity-ne1){throw 'BendTime acquisition unconfirmed'}
 $checks.Add('Exact native Corvo BendTime equipment acquired and selected')
 $window=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();if(!$window){throw 'No source game window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','12','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}
 $watch=[Diagnostics.Stopwatch]::StartNew();$next=0;$times=@(1000,5000);$sceneCount=0
 while($watch.ElapsedMilliseconds-lt9500){
  if($next-lt$times.Count-and$watch.ElapsedMilliseconds-ge$times[$next]){I host 255 250;$events.Add([pscustomobject]@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action='Native LT press 250ms';index=$next});$next++}
  $native=Q host '--player-states';$midnight=Q host '--midnight';$entities=Q host '--entities'
  $wind=@($entities.entities|Where-Object {$_.definitionName-match'bendtime'})
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=$native;midnight=$midnight;wind=$wind;component=(Q host '--object' $power.component)})
  if(($midnight|ConvertTo-Json -Depth 12 -Compress)-match'power_bendtime_cast'){$sceneCount++}
  if(!(S host '.bendtime-fixture.json').active){throw 'BendTime fixture retired during native capture'};Start-Sleep -Milliseconds 25
 }
 if(!$sceneCount){throw 'No native BendTime cast scene observed'};$checks.Add('Normal controller casting produced native BendTime scene observations')
 I host;C host 'coop_bendtime_fixture off';$w.Restart();do{$s=S host '.bendtime-fixture.json';if(!$s.active){break};Start-Sleep -Milliseconds 80}while($w.ElapsedMilliseconds-lt7000)
 if($s.active-or$s.result-ne'quantity_and_selection_restored'){throw "BendTime cleanup unconfirmed: $($s.result)"}
 $after=Q host '--player-bendtime';$after|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/after.json"
 if($after.quantity-or$after.equipped-or$after.started-or$after.powerActive){throw 'BendTime remained owned, equipped or active'}
 $afterInventory=Q host '--player-inventory';$afterInventory|ConvertTo-Json -Depth 8|Set-Content -Encoding utf8 "$out/inventory-after.json"
 $a=@($inventory.items|ForEach-Object {"$($_.name):$($_.quantity)"});$b=@($afterInventory.items|ForEach-Object {"$($_.name):$($_.quantity)"});if(!$a.Count-or(Compare-Object $a $b)){throw 'Inventory quantities changed after cleanup'}
 $peerAfter=Q client '--player-inventory';if(Compare-Object @($peerBefore.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($peerAfter.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Peer inventory changed'}
 $checks.Add('Native cleanup restores source inventory quantity/selection; peer inventory unchanged')
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56){throw 'Native player health changed'}}
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected save changed'}}
 $checks.Add('Both player health and protected files unchanged')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_bendtime_fixture off';Start-Sleep -Milliseconds 1500;C $r 'coop_movement_fixture off'}catch{}}
 if($capture){$capture.WaitForExit(16000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Native capture incomplete'}}
 $rows|ConvertTo-Json -Depth 18|Set-Content -Encoding utf8 "$out/states.json"
 [ordered]@{passed=(!$failure);source='host';crouched=[bool]$Crouched;holstered=[bool]$Holstered;failure=$failure;checks=$checks;events=$events;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;capture='Actual native source POV and read-only state/scene/entity observations';scope='Native BendTime investigation only; no remote body/effect integration claimed. Native time dilation remains source-local in this research take; reload the isolated checkpoint before other tests. No saves or original assets modified.'}|ConvertTo-Json -Depth 7|Set-Content -Encoding utf8 "$out/result.json"
}
if($failure){throw $failure};"OBSERVED_NATIVE_BENDTIME $($rows.Count) samples"
