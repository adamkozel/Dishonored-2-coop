param([string]$Take='native-climb-eotw-v1041',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low',[ValidateSet('corvo','emily')][string]$Character='corvo',[ValidateSet('host','client')][string]$Role='host')
# Read-only mission fixture discovery in one isolated instance: native
# developer command `campaign <mapinfo> [low|high] [emily|corvo]`, ordinary A
# to pass the native load screen, then state/world census and a screenshot.
# Restores the Dunwall fixture for both roles afterwards.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 14|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function Frame($r,$name){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64();& (Get-Command ffmpeg).Source -hide_banner -loglevel error -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30,hwdownload,format=bgra" -frames:v 1 -y "$out/$name.png"|Out-Null}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid}
 Save "$out/identity.json" @{dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;mapInfo=$MapInfo;chaos=$Chaos;character=$Character;role=$Role}
 Save "$out/before.json" @{bridge=(S $Role);player=(Q $Role '--player')}
 $dirty=$true
 C $Role "campaign $MapInfo $Chaos $Character"
 $w=[Diagnostics.Stopwatch]::StartNew();$states=[Collections.Generic.List[object]]::new();$pressed=0
 do{
  Start-Sleep -Milliseconds 500;$s=$null;try{$s=S $Role}catch{}
  if($s){$states.Add(@{ms=$w.ElapsedMilliseconds;logic=$s.localGameLogicState;map=$s.mapId});if($s.localGameLogicState-eq15-and$w.ElapsedMilliseconds-gt5000-and$pressed-lt10){& "$lab/Send-D2Input.ps1" -Role $Role -Button A -DurationMs 200|Out-Null;$pressed++}}
 }while((!$s-or$s.localGameLogicState-ne1)-and$w.ElapsedMilliseconds-lt180000)
 Save "$out/load-states.json" @($states)
 if(!$s-or$s.localGameLogicState-ne1){Frame $Role 'stuck';throw "Mission did not reach gameplay (last logic $($s.localGameLogicState))"}
 Start-Sleep -Seconds 3
 Save "$out/player.json" (Q $Role '--player');Save "$out/player-states.json" (Q $Role '--player-states');Save "$out/bridge.json" (S $Role)
 Save "$out/inventory.json" (Q $Role '--player-inventory')
 $raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$Role] '--entities';$raw|Set-Content "$out/entities.json"
 $raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$Role] '--world-npcs';$raw|Set-Content "$out/world-npcs.json"
 Frame $Role 'arrival'
 # Native arkClimbable chains from the 1034 census. For each: stand at the
 # chain, look at it, try ordinary interact (X) / jump (A) and up/down input
 # while --watch-player-states records mode/state changes.
 $ents=@((Get-Content "$out/entities.json" -Raw|ConvertFrom-Json).entities|Where-Object {$_.type-eq'.?AVarkClimbable@@'})|Select-Object -First 2
 Save "$out/climbables.json" @($ents)
 $hwnd=(Get-Process -Id $pids[$Role]).MainWindowHandle.ToInt64();$i=0
 foreach($e in $ents){$i++
  $cap=Start-Process (Get-Command ffmpeg).Source -ArgumentList @('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','17','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/chain$i.mp4") -WindowStyle Hidden -RedirectStandardError "$out/chain$i-capture.log" -PassThru
  $watch=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids[$Role],'--watch-player-states','15000') -RedirectStandardOutput "$out/chain$i-states.json" -WindowStyle Hidden -PassThru
  $p=$e.position
  C $Role ("setviewpos {0:F3} {1:F3} {2:F3} 0 0"-f($p[0]-1.2),$p[1],($p[2]+0.5));Start-Sleep -Milliseconds 1500
  $land=Q $Role '--player';Save "$out/chain$i-landing.json" $land
  C $Role ("setviewpos {0:F3} {1:F3} {2:F3} 0 20"-f($land.candidatePosition[0]),($land.candidatePosition[1]),($land.candidatePosition[2]+1.7));Start-Sleep -Milliseconds 500
  & "$lab/Send-D2Input.ps1" -Role $Role -Button X -DurationMs 600|Out-Null;Start-Sleep -Milliseconds 1200
  Save "$out/chain$i-after-x.json" @{player=(Q $Role '--player');states=(Q $Role '--player-states')}
  & "$lab/Send-D2Input.ps1" -Role $Role -LeftY 32000 -DurationMs 3000|Out-Null;Start-Sleep -Milliseconds 3200
  Save "$out/chain$i-after-up.json" @{player=(Q $Role '--player');states=(Q $Role '--player-states')}
  & "$lab/Send-D2Input.ps1" -Role $Role -LeftY -32000 -DurationMs 3000|Out-Null;Start-Sleep -Milliseconds 3200
  Save "$out/chain$i-after-down.json" @{player=(Q $Role '--player');states=(Q $Role '--player-states')}
  & "$lab/Send-D2Input.ps1" -Role $Role -Button B -DurationMs 300|Out-Null;Start-Sleep -Milliseconds 1500
  Save "$out/chain$i-after-release.json" @{player=(Q $Role '--player');states=(Q $Role '--player-states')}
  & "$lab/Send-D2Input.ps1" -Role $Role -DurationMs 100|Out-Null
  if(!$watch.WaitForExit(20000)){Stop-Process -Id $watch.Id};if(!$cap.WaitForExit(25000)){Stop-Process -Id $cap.Id}
 }
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($dirty){
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp};if($s.localGameLogicState-ne1-or$hp.current-ne56){throw 'Final gameplay/health not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;scope='Native climbable chain observation in Edge of the World via native campaign command and ordinary input; Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
