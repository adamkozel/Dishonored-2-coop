param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-ranged-corvo-gun-actions-v1',[ValidateSet('host','client')][string]$Source='host',[ValidateSet('gun','crossbow')][string]$Kind='gun',[switch]$Crouched,[switch]$ObserveOnly,[switch]$Airborne,[switch]$AirReload)
if($Airborne-and(!$ObserveOnly-or$Crouched)){throw 'Airborne ranged investigation requires observation-only standing source'}
if($AirReload-and!$Airborne){throw 'Air reload requires the airborne investigation'}
$peer=if($Source-eq'host'){'client'}else{'host'};$Holstered=$false;$initialStance=$null
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
  foreach($c in 'coop_movement_resume off','coop_movement_bank off','coop_shadow_stream off','coop_power_fixture off','coop_darkvision_fixture off','coop_ranged_fixture off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $c}
  if((Q $r '--health' 1).current-ne56){throw 'Clean living fixture required'}
 }
 C $Source 'setviewpos -33 -153 68.1 180 0';C $peer 'setviewpos -31 -148 68.1 180 0'
 $initialStance=(Q $Source '--player-states').stance
 if($initialStance-ne[int][bool]$Crouched){I $Source 0 100 B;Start-Sleep -Milliseconds 700}
 if((Q $Source '--player-sword').equipped-eq[bool]$Holstered){$duration=if($Holstered){1500}else{650};I $Source 0 $duration X;Start-Sleep -Milliseconds 2300}
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Source -Pitch 0 -Evidence "$out/camera.json"
 $before=Q $Source "--player-$Kind";$inventory=Q $Source '--player-inventory';$peerBefore=Q $peer '--player-inventory'
 $before|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/before.json";$inventory|ConvertTo-Json -Depth 8|Set-Content -Encoding utf8 "$out/inventory-before.json"
 if($before.quantity-or$before.equipped-or$before.started){throw 'Native ranged weapon already owned or active'}
 C $Source "coop_ranged_fixture ${Kind}_loaded";$w=[Diagnostics.Stopwatch]::StartNew();do{$s=S $Source '.ranged-fixture.json';if($s.result-eq'native_ranged_ready'){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt6000)
 if($s.result-ne'native_ranged_ready'){throw "ranged weapon did not equip: $($s.result)"}
 $power=Q $Source "--player-$Kind";$power|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/enabled.json"
 if(!$power.equipped-or!$power.started-or$power.quantity-ne1){throw 'ranged weapon acquisition unconfirmed'}
 # Inventory acquisition alone does not enter the native left-arm equipment
 # presentation. Use ordinary native holster/draw input before recording.
 I $Source 0 1500 X;$w.Restart();do{Start-Sleep -Milliseconds 100;$weapon=Q $Source "--player-$Kind";$sword=Q $Source '--player-sword'}while(($weapon.equipped-or$sword.equipped)-and$w.ElapsedMilliseconds-lt5000)
 if($weapon.equipped-or$sword.equipped){throw 'Initial native holster incomplete'}
 I $Source 0 650 X;$w.Restart();do{Start-Sleep -Milliseconds 100;$weapon=Q $Source "--player-$Kind";$sword=Q $Source '--player-sword'}while((!$weapon.equipped-or!$sword.equipped)-and$w.ElapsedMilliseconds-lt5000)
 if(!$weapon.equipped-or!$sword.equipped){throw 'Initial native draw incomplete'}
 Start-Sleep -Milliseconds 1200
 if((Q $Source '--player-states').stance-ne[int][bool]$Crouched){throw 'Requested native stance did not remain active after equipment setup'}
 $checks.Add('Exact native ranged equipment acquired; normal controller holster/draw initializes native arm presentation')
 $window=(Get-Process -Id $pids[$Source]).MainWindowHandle.ToInt64();if(!$window){throw 'No source game window'}
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${window}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','18','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$PSScriptRoot/$Take.mp4")
 $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();$capture=Start-Process -FilePath (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 Start-Sleep -Milliseconds 700;if($capture.HasExited){throw 'Recorder failed'}
 $watch=[Diagnostics.Stopwatch]::StartNew();$next=0;$times=@(1000,4000,7000,10000,13000,15000);$sceneCount=0;$fireCount=0;$emptyCount=0;$scenes=[Collections.Generic.HashSet[string]]::new();$reloadJumped=$false;$native=$null
 while($watch.ElapsedMilliseconds-lt16300){
  if($next-lt$times.Count-and$watch.ElapsedMilliseconds-ge$times[$next]){if($next-eq3){I $Source 0 1500 X;$action='Native X hold 1500ms'}elseif($next-eq4){I $Source 0 650 X;$action='Native X draw 650ms'}else{
   if($Airborne){I $Source 0 220 A;Start-Sleep -Milliseconds 80;$jump=Q $Source '--player-states';$jump|ConvertTo-Json -Depth 10|Set-Content -Encoding utf8 "$out/jump-before-trigger-$next.json";if(!@($jump.active|Where-Object {$_.id-in@(2,3)}).Count){throw 'Native jump was not observed before trigger'}}
   I $Source 255 250;$action=if($Airborne){'Native A jump, then LT trigger after observing native air state'}else{'Native LT press 250ms'}
  };$events.Add([pscustomobject]@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action=$action;index=$next});$next++}
  if($AirReload-and!$reloadJumped-and$null-ne$native-and@($native.active|Where-Object {$_.id-eq59-and$_.vtableRva-eq'0x1c7d608'}).Count){
   I $Source 0 220 A;$reloadJumped=$true;$events.Add([pscustomobject]@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action='Native A jump during observed Reload59';index=-1})
  }
  $native=Q $Source '--player-states';$midnight=Q $Source '--midnight';$ranged=$null;try{$ranged=Q $Source "--player-$Kind"}catch{};foreach($scene in $midnight.instances){if($scene.asset-match'/guns/'){$null=$scenes.Add("$($scene.asset)|$($scene.subscene)|$($scene.sceneDurationMs)");if($scene.asset-match('/'+$Kind+'_fire')){$fireCount++};if($scene.asset-match('/'+$Kind+'_ooa')){$emptyCount++}}}
  $rows.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=$native;ranged=$ranged;midnight=$midnight;component=(Q $Source '--object' $power.component)})
  if(($midnight|ConvertTo-Json -Depth 12 -Compress)-match('guns/'+$Kind+'_(ooa|fire)')){$sceneCount++}
  if(!(S $Source '.ranged-fixture.json').active){throw 'ranged weapon fixture retired during native capture'};Start-Sleep -Milliseconds 25
 }
 if(!$fireCount-or!$emptyCount){throw 'Native fire and empty action sequence incomplete'};$checks.Add('Two native shots and a subsequent native empty trigger action observed');
 $expectedVariants=if($Kind-eq'gun'){@(1,3,4,5,6)}else{@(2,7,8,9,10)}
 $observedVariants=@($rows|Where-Object {$_.player.rangedSceneValid}|ForEach-Object {$_.player.rangedScene.variant}|Sort-Object -Unique)
 if(!$ObserveOnly){
  foreach($variant in $expectedVariants){if($variant-notin$observedVariants){throw "Owned ranged scene reader missed variant $variant"}}
  $checks.Add('Owned native scene reader validates fire, reload, empty, equip and unequip identities and clocks')
 }else{
  foreach($part in 'fire','reload','ooa','equip','unequip'){if(!@($scenes|Where-Object {$_-match("/"+$Kind+"_"+$part+".midnightscene")}).Count){throw "Native observation missed $part"}}
  if(@($rows|Where-Object {$_.player.stance-ne[int][bool]$Crouched}).Count){throw 'Native source left the requested stance during observation'}
  $checks.Add('Read-only native scenes and requested stance observed; mapped reader and remote playback are not acceptance criteria for this observation')
  if($Airborne){
   $airRows=@($rows|Where-Object {@($_.player.active|Where-Object {$_.id-in@(2,3)}).Count-and@($_.midnight.instances|Where-Object {$_.asset-match('/guns/'+$Kind+'_(fire|reload|ooa).midnightscene')}).Count})
   if($airRows.Count-lt2){throw 'Native Jump/Fall and ranged scenes were not observed together'}
   $checks.Add('Native Jump/Fall and ranged scenes overlap under ordinary controller input; no remote airborne playback claimed')
   if($AirReload){
    if(!$reloadJumped-or!@($airRows|Where-Object {@($_.midnight.instances|Where-Object {$_.asset-match('/guns/'+$Kind+'_reload.midnightscene')}).Count}).Count){throw 'Native reload and airborne state did not overlap'}
    $checks.Add('Observed native Reload59 accepted ordinary jump input and overlapped a native airborne state')
   }
  }
 }
 $scenes|Sort-Object|Set-Content -Encoding utf8 "$out/scenes.txt"
 I $Source;C $Source 'coop_ranged_fixture off';$w.Restart();do{$s=S $Source '.ranged-fixture.json';if(!$s.active){break};Start-Sleep -Milliseconds 80}while($w.ElapsedMilliseconds-lt7000)
 if($s.active-or$s.result-ne'quantity_and_selection_restored'){throw "ranged weapon cleanup unconfirmed: $($s.result)"}
 $after=Q $Source "--player-$Kind";$after|ConvertTo-Json -Depth 6|Set-Content -Encoding utf8 "$out/after.json"
 if($after.quantity-or$after.equipped-or$after.started){throw 'ranged weapon remained owned, equipped or active'}
 $afterInventory=Q $Source '--player-inventory';$afterInventory|ConvertTo-Json -Depth 8|Set-Content -Encoding utf8 "$out/inventory-after.json"
 $a=@($inventory.items|ForEach-Object {"$($_.name):$($_.quantity)"});$b=@($afterInventory.items|ForEach-Object {"$($_.name):$($_.quantity)"});if(!$a.Count-or(Compare-Object $a $b)){throw 'Inventory quantities changed after cleanup'}
 $peerAfter=Q $peer '--player-inventory';if(Compare-Object @($peerBefore.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($peerAfter.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Peer inventory changed'}
 $checks.Add('Native cleanup restores source inventory quantity/selection; peer inventory unchanged')
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56){throw 'Native player health changed'}}
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){throw 'Protected save changed'}}
 $checks.Add('Both player health and protected files unchanged; fixture restores its two-round ammunition grant')
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_ranged_fixture off';Start-Sleep -Milliseconds 1500;C $r 'coop_movement_fixture off'}catch{}}
 if($null-ne$initialStance){try{if((Q $Source '--player-states').stance-ne$initialStance){I $Source 0 100 B;Start-Sleep -Milliseconds 700};if((Q $Source '--player-states').stance-ne$initialStance){throw 'Stance restore failed'}}catch{$failure='Native initial stance restoration failed'}}
 if($capture){$capture.WaitForExit(16000)|Out-Null;if(!$capture.HasExited-or$capture.ExitCode-ne0){$failure='Native capture incomplete'}}
 $rows|ConvertTo-Json -Depth 18|Set-Content -Encoding utf8 "$out/states.json"
 [ordered]@{passed=(!$failure);observationOnly=[bool]$ObserveOnly;airborneExperiment=[bool]$Airborne;airReloadExperiment=[bool]$AirReload;mappedVariants=$observedVariants;source=$Source;kind=$Kind;crouched=[bool]$Crouched;holstered=[bool]$Holstered;failure=$failure;checks=$checks;events=$events;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;capture='Actual native source POV and read-only state/scene/entity observations';scope='Two-round native ranged weapon/action investigation only; no remote body/effect integration claimed. Native gameplay may move destructible objects; reload the isolated checkpoint before other tests. Native projectile and impact state requires an isolated checkpoint reload after this test. No saves or original assets modified.'}|ConvertTo-Json -Depth 7|Set-Content -Encoding utf8 "$out/result.json"
}
if($failure){throw $failure};"OBSERVED_NATIVE_RANGED $Source $Kind $($rows.Count) samples"
