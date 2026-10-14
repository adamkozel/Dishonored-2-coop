param([string]$Take='mission-join-follow-v1189',[string]$MapInfo='campaign/edge_of_the_world/eotw.mapinfo',[ValidateSet('low','high')][string]$Chaos='low')
# Stage 7 join-follow (candidate 1188): only the HOST is told to load Edge of
# the World; the client, still in Dunwall, must follow natively into the same
# mission as Emily from the host's published mapinfo (coop_follow_host on).
# The harness only presses native continue prompts.
# Dunwall fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$captures=[Collections.Generic.List[object]]::new();$rows=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function I($r,[string]$b='',[int]$ms=100){$a=@{Role=$r;DurationMs=$ms};if($b){$a.Button=$b};& "$lab/Send-D2Input.ps1" @a|Out-Null}
function W($o,$offset){$w=@($o.words|Where-Object offset -eq $offset);if($w.Count-ne1){throw "Missing object field $offset"};[Convert]::ToUInt64($w[0].word.Substring(2),16)}
function Bind($r,$name){$door=@((Q $r '--entities').entities|Where-Object name -eq $name);if($door.Count-ne1){return $null}
 $cp=@((Q $r '--components' $door[0].handle).components|Where-Object type -eq '.?AVarkCpntOpenable@@');if($cp.Count-ne1-or$cp[0].owner-ne$door[0].pointer){return $null}
 $o=Q $r '--object' $cp[0].pointer;@{name=$name;position=$door[0].position;component=$cp[0].pointer;owner=[Convert]::ToUInt64($door[0].pointer.Substring(2),16);generation=((W $o '0x18')-band [uint64]4294967295)}}
function NativeDoor($r,$b){$o=Q $r '--object' $b.component;if($o.type-ne'.?AVarkCpntOpenable@@'-or(W $o '0x10')-ne$b.owner-or((W $o '0x18')-band [uint64]4294967295)-ne$b.generation){throw 'Native door lifetime changed'};$s=W $o '0x48';[pscustomobject]@{current=[uint32]($s-band [uint64]4294967295);pending=[uint32]($s-shr32);side=[int]((W $o '0xc8')-band255)}}
function Entry($r,$name){@((S $r '.door-sync.json').entries|Where-Object name -eq $name)[0]}
function Capture($name,$seconds){foreach($r in 'host','client'){$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t',[string]$seconds,'-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$name-$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$name-$r-capture.log" -PassThru))}}
function UseSweep($r,$b,$want,[int[]]$Offsets=@(0,20,-20,40,-40)){
 # The door origin is the hinge: sweep the bearing until native use changes it.
 $v=Q $r '--player';$here=$v.candidatePosition
 foreach($off in $Offsets){
  if($off){C $r ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$here[0],$here[1],($here[2]+1.7),($script:baseYaw+$off));Start-Sleep -Milliseconds 500}
  I $r X 200;$t=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 80;$n=NativeDoor $r $b}while($n.current-ne$want-and$n.pending-ne$want-and$t.ElapsedMilliseconds-lt1200)
  if($n.current-eq$want-or$n.pending-eq$want){return $off}}
 return $null}
function WaitCaptures(){foreach($c in @($captures)){if(!$c.WaitForExit(120000)-or$c.ExitCode){throw 'Recording failed'}};$captures.Clear()}
function Approach($r,$door,$from){
 $p=$door.position;$dx=$from[0]-$p[0];$dy=$from[1]-$p[1];$n=[Math]::Max([Math]::Sqrt($dx*$dx+$dy*$dy),.001);$ux=$dx/$n;$uy=$dy/$n
 $script:standX=$p[0]+1.4*$ux;$script:standY=$p[1]+1.4*$uy;$script:standZ=$p[2]+1.7;$script:baseYaw=[Math]::Atan2(-$uy,-$ux)*180/[Math]::PI
 C $r ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$standX,$standY,$standZ,$baseYaw);Start-Sleep -Milliseconds 1300
 $v=Q $r '--player';$eye=$v.nativeViewPosition;$hd=[Math]::Sqrt(($p[0]-$eye[0])*($p[0]-$eye[0])+($p[1]-$eye[1])*($p[1]-$eye[1]))
 $pitch=[Math]::Max(-45,[Math]::Min(45,[Math]::Atan2($eye[2]-($p[2]+1.0),$hd)*180/[Math]::PI))
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $r -Pitch $pitch -Quick|Out-Null;return @{eye=$eye;pitch=$pitch}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v1188/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected 1188 candidate'}
 foreach($r in 'host','client'){C $r 'coop_follow_host on'}
 Capture 'follow' 150;Start-Sleep -Milliseconds 800
 Save "$out/identity.json" @{dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;mapInfo=$MapInfo}
 $dirty=$true
 C host "campaign $MapInfo $Chaos corvo" # client receives NO load command
 $w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0}
 do{
  Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-gt5000-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq5-and$w.ElapsedMilliseconds-$last[$r]-gt4000){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}}
 }while(!$ready-and$w.ElapsedMilliseconds-lt240000)
 Save "$out/load-log.json" @($log)
 if(!$ready){throw 'Both instances did not reach mission gameplay'}
 Start-Sleep -Seconds 2
 if(!(S host).remoteFresh-or!(S client).remoteFresh){throw 'Transport not fresh on mission map'}
 foreach($r in 'host','client'){Save "$out/follow-$r.json" (S $r '.follow.json');Save "$out/player-$r.json" (Q $r '--player')}
 $h=S host '.follow.json';$c=S client '.follow.json'
 $hs=S host;$cs=S client
 WaitCaptures
 if($hs.localMapId-and$cs.localMapId-and$hs.localMapId-ne$cs.localMapId){throw 'Peers ended on different maps'}
 $rows.Add(@{hostFollow=$h;clientFollow=$c;clientLoads=$c.loads;sameMap=($c.result-eq'same_mission')})
 if($c.loads-lt1-or$c.result-ne'same_mission'){throw "Client did not follow the host mission: $($c.result) loads=$($c.loads)"}
 Save "$out/verified.json" @{passed=$true;scope='Join-follow.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  foreach($c in @($captures)){try{if(!$c.HasExited-and!$c.WaitForExit(30000)){Stop-Process -Id $c.Id}}catch{}}
  if($dirty){
   foreach($r in 'host','client'){try{I $r;C $r 'coop_follow_host off'}catch{}}
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-eq12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;Save "$out/final-$r.json" @{bridge=$s;health=$hp};if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;cases=@($rows);scope='Client follows the host into Edge of the World natively (host-only load command); Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
