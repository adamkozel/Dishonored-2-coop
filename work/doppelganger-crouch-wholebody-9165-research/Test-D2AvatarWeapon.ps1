[CmdletBinding()]
param()
$ErrorActionPreference='Stop';$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$samples=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null;$pids=@{}
function Command($role,$text){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $text -WaitConsumed|Out-Null}
function State($role){Get-Content "$root/work/lab-instances/$role/coop-status.json.avatar-weapon.json" -Raw|ConvertFrom-Json}
function Sword{Get-Content "$root/work/lab-instances/client/coop-status.json.avatar-sword.json" -Raw|ConvertFrom-Json}
function Bridge($role){Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle=''){
 $arguments=@($pids[$role],$option);if($handle){$arguments+=$handle}
 for($i=0;$i-lt8;$i++){$raw=& $probe @arguments;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Native weapon read failed: $role $option"
}
function Capture($stage){
 $handle=(Bridge client).probeActorHandle;$r=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Host=(State host);Client=(State client);Sword=(Sword);Source=(Native host '--player-sword');Target=(Native client '--avatar-sword' $handle);Attachment=(Native client '--avatar-sword-attachment' $handle);Arm=(Get-Content "$root/work/lab-instances/client/coop-status.json.arm-stream.json" -Raw|ConvertFrom-Json)}
 $samples.Add($r);return $r
}
function Await($drawn,$stage){
 $w=[Diagnostics.Stopwatch]::StartNew();do{
  Start-Sleep -Milliseconds 90;$r=Capture $stage
  $statusReady=if($drawn){$r.Host.ready-and$r.Client.ready-and$r.Sword.active-and$r.Sword.observed-eq$r.Sword.started}else{!$r.Sword.active}
 }while((!$statusReady-or$r.Target.equipped-ne$drawn-or$r.Target.started-ne$drawn-or$r.Sword.restoring)-and$w.ElapsedMilliseconds-lt2500)
 if(!$statusReady-or$r.Target.equipped-ne$drawn-or$r.Target.started-ne$drawn-or$r.Sword.restoring-or$r.Target.attachedTag-ne$(if($drawn){'right_hand_wpn'}else{'holster_wpn'})){throw "Remote sword did not reach $stage"};return $r
}
function ArmBoth{
 foreach($role in 'host','client'){Command $role 'coop_avatar_weapon 15000'}
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 100;$h=State host;$c=State client}while((!$h.ready-or!$c.ready)-and$w.ElapsedMilliseconds-lt1800)
 if(!$h.ready-or!$c.ready-or$h.token-ne$c.peerToken-or$c.token-ne$h.peerToken){throw 'Both native equipment opt-in tokens did not agree'}
}
try{
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected process'};$pids[$role]=$l.pid
  Command $role 'coop_avatar_weapon off';Command $role 'coop_avatar_sword off';Command $role 'coop_probe_arm off';Command $role 'coop_probe_body off';Command $role 'coop_damage_observe off';Command $role 'coop_damage_observe 60000'
 }
 Command host 'setviewpos -33 -153 68.1 90 0';Command client 'setviewpos -33 -148 68.1 270 8'
 foreach($role in 'host','client'){Command $role 'coop_probe_follow exact';Command $role 'coop_probe_arm on'}
 Start-Sleep -Milliseconds 400;$before=Capture Before
 if(!$before.Source.equipped-or$before.Target.equipped){throw 'Expected drawn local source and holstered counterpart'}
 Command client 'coop_avatar_weapon 15000';Start-Sleep -Milliseconds 500;$one=Capture ReceiverOnly
 if($one.Sword.active-or$one.Target.equipped-or$one.Sword.started-ne$before.Sword.started){throw 'Receiver-only opt-in equipped a weapon'}
 Command host 'coop_avatar_weapon 15000';$drawn=Await $true Drawn
 if(!$drawn.Host.ready-or!$drawn.Client.ready-or$drawn.Sword.started-ne$before.Sword.started+1){throw 'Initial native equipment binding failed'}
 $checks.Add('Receiver-only arming makes no equipment change; both fresh opt-in tokens permit one native equip of Corvo existing sword')
 & "$PSScriptRoot/Send-D2Input.ps1" -Role host -RightTrigger 255 -DurationMs 220|Out-Null
 for($i=0;$i-lt10;$i++){Capture Drawn|Out-Null;Start-Sleep -Milliseconds 60}
 $preDuplicate=Capture PreDuplicate;Command host 'coop_avatar_weapon_duplicate';Start-Sleep -Milliseconds 250;$duplicate=Capture Duplicate
 if($duplicate.Client.rejected-le$preDuplicate.Client.rejected-or$duplicate.Sword.started-ne$preDuplicate.Sword.started-or$duplicate.Sword.released-ne$preDuplicate.Sword.released){throw 'Duplicate state was not rejected without native equipment callbacks'}
 foreach($role in 'host','client'){Command $role 'coop_avatar_weapon 15000'}
 Start-Sleep -Milliseconds 250;$rearm=Capture DuplicateArm
 if($rearm.Host.started-ne$drawn.Host.started-or$rearm.Client.started-ne$drawn.Client.started-or$rearm.Host.remainingMs-ge$drawn.Host.remainingMs-or$rearm.Client.remainingMs-ge$drawn.Client.remainingMs){throw 'Duplicate local arm renewed fixed expiry'}
 Command client 'coop_avatar_sword 2000';Start-Sleep -Milliseconds 250;$localRefused=Capture LocalExperimentRefused
 if($localRefused.Sword.started-ne$duplicate.Sword.started-or$localRefused.Sword.refused-le$duplicate.Sword.refused){throw 'Local equip experiment overlapped remote ownership'}
 $checks.Add('Replayed state, duplicate arming and overlapping local equipment experiments cause no additional native equip or renewed deadline')
 & "$PSScriptRoot/Send-D2Input.ps1" -Role host -Button X -DurationMs 1500|Out-Null
 Start-Sleep -Milliseconds 2100;$holster=Await $false Holstered
 if($holster.Source.equipped-or$holster.Sword.restored-ne$before.Sword.restored+1-or$holster.Sword.released-ne$before.Sword.released+1-or$holster.Target.weaponHandle-ne$before.Target.weaponHandle){throw 'Native source holster was not mirrored exactly once'}
 & "$PSScriptRoot/Send-D2Input.ps1" -Role host -RightTrigger 255 -DurationMs 220|Out-Null
 $redrawn=Await $true Redrawn
 if($redrawn.Sword.started-ne$before.Sword.started+2-or$redrawn.Target.weaponHandle-ne$before.Target.weaponHandle){throw 'Native source redraw did not reuse the same sword'}
 $checks.Add('Native player holster and redraw propagate through Steam to the same counterpart sword entity without inventory creation')
 Command host 'coop_avatar_weapon off';$off=Await $false SourceStopped
 if(!(Bridge client).remoteFresh-or$off.Sword.lost-ne$before.Sword.lost-or$off.Sword.restored-ne$before.Sword.restored+2){throw 'Source-only stream expiry did not restore native holster while root transport remained fresh'}
 $checks.Add('Source-only stream stop expires remote equipment and restores the original holster while ordinary Steam root and arm traffic continue')
 foreach($role in 'host','client'){Command $role 'coop_avatar_weapon off'}
 Start-Sleep -Milliseconds 400;ArmBoth;$fresh=Await $true NewTokens
 if($fresh.Host.token-eq$drawn.Host.token-or$fresh.Client.token-eq$drawn.Client.token){throw 'Rearming reused prior opt-in tokens'}
 $w=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 400;$s=Capture ExpiryWaiting}while(($s.Host.armed-or$s.Client.armed-or$s.Sword.active-or$s.Sword.restoring)-and$w.ElapsedMilliseconds-lt17500)
 $expired=Await $false Expired
 if($expired.Host.armed-or$expired.Client.armed-or$expired.Sword.active-or$expired.Sword.lost-ne$before.Sword.lost){throw 'Fixed 15-second equipment opt-in did not expire cleanly'}
 $checks.Add('A fresh two-token binding expires at its fixed fifteen-second deadline and restores native holster')
 foreach($role in 'host','client'){
  $health=Native $role '--health' 1;$damage=Get-Content "$root/work/lab-instances/$role/coop-status.json.damage.json" -Raw|ConvertFrom-Json
  if($health.current-ne56-or@($damage.events).Count){throw 'Presentation-only equipment test introduced native damage'}
 }
 $checks.Add('Both real players retain 56 health and no native damage events occur')
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){Command $role 'coop_avatar_weapon off';Command $role 'coop_probe_arm off';Command $role 'coop_damage_observe off'}
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 Scope='Corvo actual native draw/holster replicated over Steam to his owned counterpart existing tutorial sword. Bounded two-token opt-in and native equipment cleanup; independent render verification required. No Emily equipment, collision, finger or combat parity claim.';Checks=$checks;Samples=$samples}|ConvertTo-Json -Depth 22|Set-Content "$PSScriptRoot/research/avatar-weapon-observations.json"
if($failure){throw $failure};[pscustomobject]@{Passed=$true;Checks=$checks.Count;Samples=$samples.Count}
