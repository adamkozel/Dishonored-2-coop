param([ValidateRange(1,8)][int]$Trials=5)
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent;$lab="$root/outputs/d2-coop-lab";$probe="$lab/bin/d2_runtime_probe.exe"
$rows=[Collections.Generic.List[object]]::new();$failure=$null;$declarations=@{};$gamePid=0;$guardHandle=''
function Command($role,$command){& "$lab/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function State($suffix=''){Get-Content "$root/work/lab-instances/client/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Native($option,$handle=''){
 $args=@($gamePid,$option);if($handle){$args+=$handle}
 for($i=0;$i-lt8;$i++){$raw=& $probe @args;if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};return $null
}
function Capture($stage){
 $damage=State '.damage.json'
 foreach($hit in $damage.events){
  $bytes=[Convert]::FromHexString($hit.rawInfo);$address='0x{0:x}' -f [BitConverter]::ToUInt64($bytes,0)
  if(!$declarations.ContainsKey($address)){$declarations[$address]=Native '--object' $address}
 }
 $r=[pscustomobject]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o');Bridge=(State);Damage=$damage;
  Guard=(Native '--health' $guardHandle);Player=(Native '--health' 1);Attack=(Native '--ai-attack' $guardHandle);
  Targets=(Native '--ai-targets' $guardHandle);Isolation=(State '.observer-isolation.json')}
 $rows.Add($r);return $r
}
try{
 & "$lab/Restore-D2Lab.ps1"|Out-Null
 foreach($role in 'host','client'){Command $role 'coop_enemy_outcome off';Command $role 'coop_guard_link off';Command $role 'coop_sword_link off';Command $role 'coop_partner_enemy off'}
 $launch=Get-Content "$root/work/lab-instances/client/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $launch.pid).Path-ne$launch.executable){throw 'Unexpected client process'};$gamePid=$launch.pid
 Command client 'setviewpos -39.4 -161.4 68.1 270 20'
 & "$lab/Send-D2Input.ps1" -Role client -Button X -DurationMs 400|Out-Null
 $watch=[Diagnostics.Stopwatch]::StartNew();$saw=$false
 do{$s=State;if($s.localGameLogicState-eq5){$saw=$true};$ready=$saw-and$s.localGameLogicState-eq1-and!$s.localPendingGameLogicState;if(!$ready){Start-Sleep -Milliseconds 150}}while(!$ready-and$watch.ElapsedMilliseconds-lt45000)
 if(!$ready){throw 'Native Mayhew scene did not finish'}
 Command client 'setviewpos -28.0921 -153.962 68.1 270 0'
 $cat=Native '--world-npcs';$n=@($cat.npcs|Where-Object spawnerName -eq ld_spawner_guard_escape_1)
 if($n.Count-ne1-or!$n[0].unique-or!$n[0].reciprocal){throw 'Ambiguous guard'};$guardHandle=$n[0].handle
 Command client 'coop_damage_observe off';Command client 'coop_damage_observe 60000';Command client 'coop_observer_isolation 30000'
 Start-Sleep -Milliseconds 300;$before=Capture Before
 if(!$before.Isolation.ready-or$before.Isolation.heldReferences-ne4){throw 'Observer isolation unavailable'}
 Command client 'setviewpos -42.9563 -122 62.7 90 0'
 for($i=0;$i-lt10;$i++){
  Start-Sleep -Milliseconds 350;$r=Capture "Awareness-$i"
  if(!$r.Guard-or!$r.Player-or$r.Player.current-lt20){throw 'Insufficient health or native identity during awareness observation'}
  if($r.Attack.schedulerActive){break}
 }
 if(!$r.Attack.schedulerActive){throw 'Guard did not enter native attack scheduling'}
 for($i=0;$i-lt$Trials;$i++){
  & "$lab/Send-D2Input.ps1" -Role client -RightTrigger 255 -DurationMs 220|Out-Null
  Start-Sleep -Milliseconds 900;$r=Capture "Strike-$i"
  if(!$r.Guard-or!$r.Player-or$r.Guard.current-le0-or$r.Player.current-lt20){break}
 }
 Command client 'setviewpos -28.0921 -153.962 68.1 270 0';Start-Sleep -Milliseconds 300;Capture Settled|Out-Null
}catch{$failure=$_.Exception.Message}finally{
 [ordered]@{ObservedAt=[DateTimeOffset]::Now.ToString('o');Failure=$failure;DllSha256=(Get-FileHash "$lab/bin/dinput8.dll").Hash;GuardHandle=$guardHandle;
  Scope='Local observation only: native player sword against an alerted native guard, with the two interfering client NPCs temporarily isolated. No enemy damage relay is armed.';
  Declarations=$declarations;Samples=$rows}|ConvertTo-Json -Depth 22|Set-Content "$root/work/alerted-player-strike-observations.json"
 Command client 'coop_damage_observe off';Command client 'coop_observer_isolation off'
 & "$lab/Restore-D2Lab.ps1"|Out-Null
}
if($failure){throw $failure}
$rows|ForEach-Object{[pscustomobject]@{Stage=$_.Stage;GuardHealth=$_.Guard.current;PlayerHealth=$_.Player.current;Attack=$_.Attack.schedulerActive;Hits=$_.Damage.events.Count}}
