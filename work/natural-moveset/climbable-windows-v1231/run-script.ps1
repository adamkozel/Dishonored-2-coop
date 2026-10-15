param([string]$Take='climbable-windows-v1231',[string]$Windows='eotw-openable-windows-v1231.json',[int]$MaxWindows=41,[string]$Candidate='candidate-npc-combat-v1213h')
# 1231 (#7, window_enter_exit): find a GENUINELY climbable window. The 1201 (Dunwall Tower) and 1206 (EotW
# docks) windows were not climbable. Input: the 41 openable Edge of the World windows from the static scan
# (eotw-openable-windows-v1231.json: name, position, local axes). Host only (the client idles):
#  1) floors: for each horizontal local axis and both signs, teleport the host 1.1 m out at sill height
#     and record where it lands (blocked when pushed > 0.4 m; floor = landing z);
#  2) for every axis whose two sides both have a floor within 2.2 m below the window origin (not a
#     sheer drop), stand 0.9 m out on the side with the higher floor, face the window, open it natively
#     (Use while the interaction sensor is focused on it), then push forward and jump for 1.6 s;
#  3) a crossing (the host ends > 0.4 m past the window plane) is a climbable window; native player
#     state types seen during the push are recorded (mantle evidence). Read-only probes + ordinary input.
# Native modals are dismissed (ClearModals). Dunwall fixture restored.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$rows=[Collections.Generic.List[object]]::new();$modals=[Collections.Generic.List[object]]::new()
function Save($p,$v){$v|ConvertTo-Json -Depth 15|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r"}
function Q($r,$mode,$arg=0){for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Native query failed $r $mode"}
function F($r){for($i=0;$i-lt6;$i++){$raw=& "$lab/bin/d2_runtime_probe.exe" $pids[$r] --focus;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 50};throw "Focus query failed $r"}
function I($r,[string]$b='Neutral',[int]$ms=100,[int]$ly=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms -LeftY $ly|Out-Null}
function ClearModals($tag){for($i=0;$i-lt4;$i++){$hit=$false;foreach($r in 'host','client'){$l=(S $r).localGameLogicState;if($l-in12,2){$hit=$true;$modals.Add(@{tag=$tag;role=$r;logic=$l});I $r B 200;Start-Sleep -Milliseconds 300;I $r}};if(!$hit){return};Start-Sleep -Milliseconds 900}}
function Pos{(Q host '--player').candidatePosition}
function Place($x,$y,$z,$yaw){C host ("setviewpos {0:F3} {1:F3} {2:F3} {3:F2} 0"-f$x,$y,$z,$yaw);Start-Sleep -Milliseconds 900;ClearModals 'place';Pos}
function WaitGameplay($limitMs){$w=[Diagnostics.Stopwatch]::StartNew();$log=[Collections.Generic.List[object]]::new();$last=@{host=0;client=0}
 do{Start-Sleep -Milliseconds 500;$ready=$true
  foreach($r in 'host','client'){$s=S $r;$log.Add(@{ms=$w.ElapsedMilliseconds;role=$r;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMapId})
   if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){$ready=$false}
   if($s.localGameLogicState-in8,15-and$w.ElapsedMilliseconds-gt5000-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r A 300;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq5-and$w.ElapsedMilliseconds-$last[$r]-gt4000){I $r X 3000;$last[$r]=$w.ElapsedMilliseconds}
   elseif($s.localGameLogicState-eq12-and$w.ElapsedMilliseconds-$last[$r]-gt1500){I $r B 200;$last[$r]=$w.ElapsedMilliseconds}}
  if($ready-and(S host).localMapId-ne(S client).localMapId){$ready=$false}
 }while(!$ready-and$w.ElapsedMilliseconds-lt$limitMs);return @{ready=$ready;log=@($log)}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'};$pids[$r]=$l.pid;if((S $r).localGameLogicState-ne1){throw 'Gameplay required'}}
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 Save "$out/identity.json" @{candidate=$Candidate;dll=$pin.dll;probe=$pin.probe;windows=$Windows}
 $list=@((Get-Content (Join-Path $PSScriptRoot $Windows) -Raw|ConvertFrom-Json).windows)|Select-Object -First $MaxWindows
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_checkpoint_sync off','coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off'){C $r $cmd}}
 $dirty=$true
 C host 'campaign campaign/edge_of_the_world/eotw.mapinfo low corvo';C client 'campaign campaign/edge_of_the_world/eotw.mapinfo low emily'
 $g=WaitGameplay 300000;Save "$out/load-log.json" $g.log;if(!$g.ready){throw 'Both instances did not reach the same mission gameplay'}
 $live=@{};foreach($e in @((Q host '--entities').entities)){$live[$e.name]=$e}
 foreach($w in $list){$row=[ordered]@{name=$w.name;position=$w.position;sides=@();attempts=@();climbable=$false}
  # Guards can kill the host (1228e): stop at a death screen (logic 13) instead of probing on.
  if((S host).localGameLogicState-eq13-or(S client).localGameLogicState-eq13){$failure='A player died during the probe (logic 13); remaining windows not tested';break}
  try{
   if(!$live.ContainsKey($w.name)){$row.missing=$true;$rows.Add($row);continue}
   $p=$live[$w.name].position;$sides=[Collections.Generic.List[object]]::new()
   foreach($ax in 0,1){$a=$w.axes[$ax];$len=[Math]::Sqrt($a[0]*$a[0]+$a[1]*$a[1]);if($len-lt.5){continue};$nx=$a[0]/$len;$ny=$a[1]/$len
    foreach($sg in 1,-1){$tx=$p[0]+$sg*1.1*$nx;$ty=$p[1]+$sg*1.1*$ny;$yaw=[Math]::Atan2(-$sg*$ny,-$sg*$nx)*180/[Math]::PI
     $c=Place $tx $ty ($p[2]+0.5) $yaw;$push=[Math]::Sqrt(($c[0]-$tx)*($c[0]-$tx)+($c[1]-$ty)*($c[1]-$ty))
     $sides.Add([ordered]@{axis=$ax;sign=$sg;normal=@($nx,$ny);landing=$c;push=[Math]::Round($push,3);floorBelowOrigin=[Math]::Round($p[2]-$c[2],3);ok=($push-lt.4-and($p[2]-$c[2])-ge-0.2-and($p[2]-$c[2])-le2.2)})}}
   $row.sides=@($sides)
   foreach($ax in 0,1){$pair=@($sides|Where-Object {$_.axis-eq$ax});if($pair.Count-ne2-or!($pair[0].ok-and$pair[1].ok)){continue}
    $start=if($pair[0].landing[2]-ge$pair[1].landing[2]){$pair[0]}else{$pair[1]};$nx=$start.normal[0]*$start.sign;$ny=$start.normal[1]*$start.sign
    $yaw=[Math]::Atan2(-$ny,-$nx)*180/[Math]::PI;$c=Place ($p[0]+0.9*$nx) ($p[1]+0.9*$ny) ($p[2]+0.5) $yaw
    & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role host -X $p[0] -Y $p[1] -Z $p[2] -Evidence "$out/aim-$($w.name)-$ax.json" 2>$null|Out-Null
    $f=F host;$opened=$false
    if($f.focused-and$f.name-match'window|sndctrl'){I host X 400;Start-Sleep -Milliseconds 450;I host;Start-Sleep -Milliseconds 1200;$opened=$true}
    $before=Pos;$d0=($before[0]-$p[0])*$nx+($before[1]-$p[1])*$ny;$types=@{}
    $sw=[Diagnostics.Stopwatch]::StartNew();I host A 1600 -32767
    while($sw.ElapsedMilliseconds-lt1900){try{foreach($s in (Q host '--player-states').active){$types[$s.type]=1}}catch{};Start-Sleep -Milliseconds 120}
    I host;Start-Sleep -Milliseconds 600;$after=Pos;$d1=($after[0]-$p[0])*$nx+($after[1]-$p[1])*$ny
    $crossed=$d0-gt0-and$d1-lt-0.4
    $row.attempts+=[ordered]@{axis=$ax;focus=$f.name;opened=$opened;before=$before;after=$after;signedBefore=[Math]::Round($d0,3);signedAfter=[Math]::Round($d1,3);crossed=$crossed;states=@($types.Keys)}
    if($crossed){$row.climbable=$true}}
  }catch{$row.error=$_.Exception.Message}
  $rows.Add($row);Save "$out/windows-progress.json" @($rows)}
 Save "$out/modals.json" @($modals)
 $hits=@($rows|Where-Object {$_.climbable})
 Save "$out/verified.json" @{climbable=@($hits|ForEach-Object {$_.name});tested=$rows.Count}
 if(!$hits.Count-and!$failure){$failure='No climbable window found among the tested Edge of the World windows'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace}}
finally{
 if($owned){
  $errors=[Collections.Generic.List[string]]::new()
  if($dirty){
   $cw=[Diagnostics.Stopwatch]::StartNew()
   do{$off=$true;foreach($r in 'host','client'){try{I $r;C $r 'coop_product_auto off';C $r 'coop_movement_resume off';foreach($cmd in 'coop_usable_sync off','coop_door_sync off','coop_coin_sync off','coop_npc_stream off','coop_shadow_stream off','coop_movement_bank off','coop_trigger_mirror off'){C $r $cmd};C $r 'coop_checkpoint_sync off';$st=S $r
     if((S $r '.reset.json').enabled){$off=$false;if($st.localGameLogicState-in8,15){I $r A 300}elseif($st.localGameLogicState-eq2){I $r B 200}}}catch{}}
    if(!$off){Start-Sleep -Milliseconds 1500}}while(!$off-and$cw.ElapsedMilliseconds-lt60000)
   foreach($r in 'host','client'){try{if((S $r).localGameLogicState-in2,12){I $r B 200;Start-Sleep -Milliseconds 800}}catch{}}
   try{& "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/restored-setup.json"|Out-Null;& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null}catch{$errors.Add($_.Exception.Message)}
  }
  foreach($r in 'host','client'){try{$s=S $r;$hp=Q $r '--health' 1;if($s.localGameLogicState-ne1-or$hp.current-ne56-or!$s.remoteFresh){throw 'Final gameplay/health/transport not restored'}}catch{$errors.Add("${r}: $($_.Exception.Message)")}}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors.Add('Protected file changed')}}
  Save "$out/restoration.json" @{passed=(!$errors.Count);errors=@($errors)};if($errors.Count-and!$failure){$failure=$errors-join '; '}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;windows=@($rows);scope='Edge of the World openable windows: floors on both sides, native open, forward+jump crossing (host only); Dunwall fixture restored.'}
  $mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
