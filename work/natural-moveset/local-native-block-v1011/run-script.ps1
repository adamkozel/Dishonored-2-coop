param([string]$Take='local-native-block-v1011',[ValidateSet('host','client')][string]$Role='client',[switch]$NoBlock)
# Ground truth for native defense: no co-op relay, no NPC stream. A local
# native guard/Ramsey engages the actual local player while ordinary held
# RightShoulder block is applied and the view follows the engaged attacker.
# Records every native 717950 call (damage observer) plus health and
# player-state samples, so a block resolved before the damage entry can be
# distinguished from one resolved inside it.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$samples=[Collections.Generic.List[object]]::new();$capture=$null
function Save($p,$v){$v|ConvertTo-Json -Depth 22|Set-Content -LiteralPath $p}
function C($cmd){& "$lab/Send-D2Command.ps1" -Role $Role -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($suffix=''){Get-Process -Id $gamePid -ErrorAction Stop|Out-Null;Get-Content "$root/work/lab-instances/$Role/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Q($mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $gamePid $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $gamePid $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 25};throw "Native query failed $mode"}
function H($h){[Convert]::ToUInt32($h.Replace('0x',''),16)}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $l=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
 if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated process'};$gamePid=$l.pid
 Save "$out/identity.json" @{dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;exe=(Get-FileHash "$root/work/asset-game/Dishonored2.exe").Hash;role=$Role;block=(!$NoBlock)}
 if((S).localGameLogicState-ne1){throw 'Gameplay required'}
 if((S '.npc-stream.json').enabled){throw 'NPC stream must be off for a purely local control'}
 $world=Q '--world-npcs';Save "$out/world-before.json" $world
 $enemies=@($world.npcs|Where-Object {$_.spawnerName -like '*guard*' -or $_.definition -like '*mortimer_ramsey*'})
 if($enemies.Count-lt2){throw 'Native enemies unavailable'}
 Save "$out/health-before.json" (Q '--health' 1);$dirty=$true
 C 'coop_damage_observe 60000'
 C 'setviewpos -42.9563 -122 62.7 90 0'
 $hwnd=$p.MainWindowHandle.ToInt64()
 $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','16','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$Role.mp4")
 $capture=Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/capture.log" -RedirectStandardOutput "$out/capture.stdout" -PassThru
 $watch=[Diagnostics.Stopwatch]::StartNew();$lastAim=-1000
 do{
  $view=Q '--player';$engaged=@()
  foreach($e in $enemies){$t=Q '--ai-targets' (H $e.handle);if($t.combat.resolved-and$t.combat.handle-eq1){$pose=Q '--npc-pose' (H $e.handle);$dx=$pose.root[0]-$view.nativeViewPosition[0];$dy=$pose.root[1]-$view.nativeViewPosition[1];$engaged+=@{handle=(H $e.handle);dx=$dx;dy=$dy;d=[Math]::Sqrt($dx*$dx+$dy*$dy);attack=(Q '--ai-attack' (H $e.handle))}}}
  $closest=$engaged|Sort-Object d|Select-Object -First 1
  if($closest-and$watch.ElapsedMilliseconds-$lastAim-gt700){$yaw=[Math]::Atan2($closest.dy,$closest.dx)*180/[Math]::PI;C ("setviewpos -42.9563 -122 62.7 {0} 0"-f$yaw);$lastAim=$watch.ElapsedMilliseconds}
  if(!$NoBlock-and$watch.ElapsedMilliseconds-gt1500){& "$lab/Send-D2Input.ps1" -Role $Role -Button RightShoulder -DurationMs 1500|Out-Null}
  $row=@{ms=$watch.ElapsedMilliseconds;engaged=$engaged;view=$view;health=(Q '--health' 1);states=(Q '--player-states')};$samples.Add($row)
  if($row.health.current-le26){break}
  Start-Sleep -Milliseconds 60
 }while($watch.ElapsedMilliseconds-lt14000)
 & "$lab/Send-D2Input.ps1" -Role $Role -DurationMs 100|Out-Null
 C 'setviewpos -33 -148 68.1 270 0'
 Save "$out/samples.json" @($samples);Save "$out/damage.json" (S '.damage.json');Save "$out/health-after.json" (Q '--health' 1)
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};Save "$out/samples.json" @($samples)}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($capture){try{if(!$capture.WaitForExit(20000)-or$capture.ExitCode){throw 'Recording failed'}}catch{$errors.Add($_.Exception.Message)}}
  if($dirty){
   try{& "$lab/Send-D2Input.ps1" -Role $Role -DurationMs 100|Out-Null;C 'coop_damage_observe off'}catch{$errors.Add($_.Exception.Message)}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{completed=(!$failure);failure=$failure;scope='Local native control only: actual local player vs local native enemies, no relay. Observation, not a pass/fail block acceptance.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
