param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='native-cabinet-input-v828',[ValidateSet('host','client')][string]$Source='host',[ValidateSet('esc_env_electric_cabinet_doors_01','esc_env_safe_door_4','ld_rich_small_01_sndctr_4')][string]$TargetName='esc_env_electric_cabinet_doors_01')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$dirty=$false;$failure=$null;$pids=@{};$inventory=@{};$binding=@{};$captures=[Collections.Generic.List[object]]::new();$samples=[Collections.Generic.List[object]]::new();$name=$TargetName
function Save($p,$v){$v|ConvertTo-Json -Depth 18|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I($r,$b='Neutral',$ms=100){& "$lab/Send-D2Input.ps1" -Role $r -Button $b -DurationMs $ms|Out-Null}
function S($r){Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json}
function Q($r,$mode,$arg=0){for($a=0;$a-lt8;$a++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 20};throw "Read failed $r $mode"}
function Field($o,$offset){$w=@($o.words|Where-Object offset -eq $offset);if($w.Count-ne1){throw "Missing field $offset"};return $w[0]}
function W($o,$offset){[Convert]::ToUInt64((Field $o $offset).word.Substring(2),16)}
function Signed32($v){[BitConverter]::ToInt32([BitConverter]::GetBytes([uint32]($v-band[uint64]4294967295)),0)}
function U($r){
 $b=$binding[$r];$s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState-or$s.localEpoch-ne$b.epoch){throw 'Usable world changed'}
 $o=Q $r '--object' $b.component
 if($o.type-ne'.?AVarkCpntUsableWithStateList@@'-or(Field $o '0x10').word-ne$b.actor-or(Field $o '0x8').word-ne$b.declaration-or((W $o '0x18')-band[uint64]4294967295)-ne$b.generation){throw 'Usable incarnation changed'}
 $state=W $o '0x28';$flags=(W $o '0x30')-band255
 return [ordered]@{tick=[Environment]::TickCount64;current=(Signed32 $state);pending=(Signed32 ($state-shr32));flags=$flags;transition=($flags-band2)-ne0;enabled=((W $o '0x20')-band255);instigator=('0x{0:x}'-f((W $o '0xd0')-band[uint64]4294967295));animation=(Field $o '0xd8').word}
}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='NativeUsable828';state='running';take=$Take}
 $pin=Get-Content "$root/work/movement-integration/candidate-coin-sync-v821/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected821 candidate'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid;if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated incarnation'};$pids[$r]=$l.pid
  $s=S $r;if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState){throw 'Gameplay required'}
  $inventory[$r]=Q $r '--player-inventory';Save "$out/inventory-before-$r.json" $inventory[$r];Save "$out/food-before-$r.json" (Q $r '--player-food')
  $e=@((Q $r '--entities').entities|Where-Object name -eq $name);if($e.Count-ne1){throw 'Unique cabinet required'};$e=$e[0];Save "$out/entity-$r.json" $e
  $cs=Q $r '--components' $e.handle;Save "$out/components-$r.json" $cs;$c=@($cs.components|Where-Object type -eq '.?AVarkCpntUsableWithStateList@@');if($c.Count-ne1-or$c[0].owner-ne$e.pointer){throw 'Usable ownership mismatch'}
  $o=Q $r '--object' $c[0].pointer;Save "$out/component-before-$r.json" $o;$d=Q $r '--object' (Field $o '0x8').word;Save "$out/declaration-$r.json" $d
  if($d.type-ne'.?AVarkDeclCpntUsableWithStateList@@'-or((W $d '0x78')-band0xffffff)-ne2){throw 'Two-state cabinet declaration required'}
  Save "$out/state-entries-$r.json" (Q $r '--object' (Field $d '0x70').word)
  $binding[$r]=@{epoch=$s.localEpoch;handle=$e.handle;actor=$e.pointer;component=$c[0].pointer;declaration=(Field $o '0x8').word;generation=((W $o '0x18')-band[uint64]4294967295)}
  Save "$out/state-before-$r.json" (U $r)
 }
 $dirty=$true
 foreach($r in 'host','client'){I $r;foreach($cmd in 'coop_coin_sync off','coop_door_sync off','coop_checkpoint_sync off','coop_movement_resume off','coop_movement_bank off','coop_review_clip off','coop_shadow_stream off','coop_movement_fixture off','coop_movement_fixture 60000'){C $r $cmd}}
 $peer=if($Source-eq'host'){'client'}else{'host'};C $peer 'setviewpos -38.7 -155 68.1 145 0'
 if($name-eq'ld_rich_small_01_sndctr_4'){
  C $Source 'setviewpos -27.85 -149.123 67.0 0 0';Start-Sleep -Milliseconds 1000
  & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $Source -X -26.5714 -Y -149.123 -Z 68.0 -Evidence "$out/aim-$Source.json"
 }elseif($name-eq'esc_env_safe_door_4'){
  C $Source 'setviewpos -51.5 -170.35 65.2 0 0';Start-Sleep -Milliseconds 1000
  & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $Source -X -50.1714 -Y -170.35 -Z 64.5 -Evidence "$out/aim-$Source.json"
 }else{
  C $Source 'setviewpos -42.2 -174.1 65.2 0 0';Start-Sleep -Milliseconds 1000
  & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $Source -X -40.92 -Y -174.1 -Z 64.4 -Evidence "$out/aim-$Source.json"
 }
 Save "$out/source-before-input.json" (Q $Source '--player');Save "$out/states-before-input.json" (Q $Source '--player-states')
 foreach($r in 'host','client'){
  if((Q $r '--health' 1).current-ne56){throw 'Unexpected health before native use'};$hwnd=(Get-Process -Id $pids[$r]).MainWindowHandle.ToInt64()
  $args=@('-hide_banner','-loglevel','warning','-nostdin','-f','lavfi','-i',"gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30:width=-2:height=-2,hwdownload,format=bgra",'-t','18','-an','-c:v','libx264','-preset','veryfast','-crf','18','-pix_fmt','yuv420p','-fps_mode','passthrough','-movflags','+faststart',"$out/$r.mp4")
  $captures.Add((Start-Process (Get-Command ffmpeg).Source -ArgumentList $args -WindowStyle Hidden -RedirectStandardError "$out/$r-capture.log" -RedirectStandardOutput "$out/$r-capture.stdout" -PassThru))
 }
 Start-Sleep -Milliseconds 900;$initial=U $Source;Save "$out/state-preinput-$Source.json" $initial
 foreach($step in 1,2){
  if($step-eq2-and$name-eq'ld_rich_small_01_sndctr_4'){
   & "$PSScriptRoot/Set-NativeInteractionAim-v691.ps1" -Role $Source -X -26.5714 -Y -149.123 -Z 68.65 -Evidence "$out/aim-second-$Source.json"
  }
  I $Source X 160;$wait=[Diagnostics.Stopwatch]::StartNew()
  do{$a=U $Source;$samples.Add(@{step=$step;state=$a});Start-Sleep -Milliseconds 75}while($wait.ElapsedMilliseconds-lt2600)
  $expected=if($step-eq1){1-$initial.current}else{$initial.current}
  Save "$out/state-step-$step-$Source.json" $a;Save "$out/state-step-$step-$peer.json" (U $peer)
  if($a.current-ne$expected-or$a.transition){throw "Native controller use did not settle at expected state for step$step"}
  if((U $peer).current-ne$initial.current){throw 'Peer cabinet changed without a usable replication channel'}
 }
 foreach($r in 'host','client'){Save "$out/inventory-after-$r.json" (Q $r '--player-inventory');Save "$out/food-after-$r.json" (Q $r '--player-food');Save "$out/component-after-$r.json" (Q $r '--object' $binding[$r].component)}
 'PASS_NATIVE_USABLE_CYCLE'
}catch{$failure=$_.Exception.Message;foreach($r in 'host','client'){try{Save "$out/failure-status-$r.json" (S $r);Save "$out/failure-player-$r.json" (Q $r '--player');Save "$out/failure-state-$r.json" (U $r)}catch{}}}finally{
 if($owned){
  foreach($p in $captures){$p.WaitForExit(25000)|Out-Null;if(!$p.HasExited-or$p.ExitCode){$failure=($failure+'; capture failed').TrimStart('; ')}}
  foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_fixture off'}catch{}}
  if($dirty){try{
   & "$root/work/PrepareShadowSword.ps1" *> "$out/checkpoint-restore.log";& "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline $out -Evidence "$out/note-restoration"
   foreach($r in 'host','client'){$restored=Q $r '--player-inventory';Save "$out/inventory-restored-$r.json" $restored;if(Compare-Object @($inventory[$r].items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($restored.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Inventory restore differs'};if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Health restore differs'}}
  }catch{$failure=($failure+'; restore: '+$_.Exception.Message).TrimStart('; ')}}
  try{foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}}catch{$failure=($failure+'; '+$_.Exception.Message).TrimStart('; ')}
  Save "$out/samples.json" @($samples);Save "$out/result.json" @{passed=(!$failure);failure=$failure;source=$Source;name=$name;scope='Two ordinary native controller interactions with a two-state cabinet. Exact owned state snapshots and actual native recordings; peer intentionally has no usable replication channel. Baseline/protected restoration. This is local native behavior evidence, not shared interaction certification.'}
  Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='NativeUsable828';state='finished';take=$Take};$mutex.ReleaseMutex()
 };$mutex.Dispose()
}
if($failure){throw $failure};'PASS_NATIVE_USABLE'
