[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent;$probe="$PSScriptRoot/bin/d2_runtime_probe.exe"
$pids=@{};$handles=@{};$samples=[Collections.Generic.List[object]]::new();$checks=[Collections.Generic.List[string]]::new();$failure=$null
function Command($role,$command){& "$PSScriptRoot/Send-D2Command.ps1" -Role $role -Command $command -WaitConsumed|Out-Null}
function Status($role,$suffix=''){Get-Content "$root/work/lab-instances/$role/coop-status.json$suffix" -Raw|ConvertFrom-Json}
function Native($role,$option,$handle=''){
 $args=@($pids[$role],$option);if($handle){$args+=$handle}
 for($n=0;$n-lt15;$n++){$r=& $probe @args;if(!$LASTEXITCODE){return($r|ConvertFrom-Json)};Start-Sleep -Milliseconds 20}
 throw "Native read unavailable: $role $option"
}
function Sample($stage){
 $s=[ordered]@{Stage=$stage;At=[DateTimeOffset]::Now.ToString('o')}
 foreach($role in 'host','client'){
  $s[$role]=[pscustomobject]@{Bridge=(Status $role);Guard=(Status $role '.guard.json');Weapon=(Status $role '.guard-weapon.json');Equipment=(Status $role '.guard-equipment.json');
   Native=(Native $role '--sword-equipment' $handles[$role]);Attack=(Native $role '--ai-attack' $handles[$role]);Health=(Native $role '--health' '1')}
 }
 $r=[pscustomobject]$s;$samples.Add($r);return $r
}
function Held($s){
 if($s.host.Guard.phase-ne7-or$s.client.Guard.phase-ne7-or$s.client.Guard.heldReferences-ne2-or$s.client.Attack.schedulerActive-or$s.client.Health.current-ne56-or$s.host.Health.current-ne56){throw "Guard ownership or isolated player health changed: $($s.Stage)"}
}
function IsDrawn($s){$s.equipped-and$s.started-and$s.attachedTag-eq'right_hand_wpn'}
function IsHolstered($s){!$s.equipped-and!$s.started-and$s.attachedTag-eq'holster_wpn'}
function WaitDrawn($stage){
 $w=[Diagnostics.Stopwatch]::StartNew()
 do{Start-Sleep -Milliseconds 100;$s=Sample $stage;Held $s}while((!(IsDrawn $s.host.Native)-or!(IsDrawn $s.client.Native)-or!$s.client.Equipment.active)-and$w.ElapsedMilliseconds-lt5000)
 if(!(IsDrawn $s.host.Native)-or!(IsDrawn $s.client.Native)-or!$s.client.Weapon.active-or!$s.client.Weapon.drawn-or!$s.client.Equipment.active-or$s.client.Equipment.localExperiment){throw 'Native host draw did not replicate to the existing client sword'}
 return $s
}
try{
 & "$PSScriptRoot/Restore-D2Lab.ps1"|Out-Null
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
  if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected lab process'};$pids[$role]=$l.pid
  foreach($cmd in 'coop_guard_weapon off','coop_guard_equipment off','coop_guard_body off','coop_guard_link off','coop_sword_link off','coop_partner_enemy off','coop_damage_observe 60000'){Command $role $cmd}
  $n=@((Native $role '--world-npcs').npcs|Where-Object spawnerName -eq ld_spawner_guard_escape_1)
  if($n.Count-ne1-or!$n[0].unique-or!$n[0].reciprocal){throw 'Ambiguous native guard'};$handles[$role]=$n[0].handle
 }
 $before=Sample Before
 if(!(IsHolstered $before.host.Native)-or!(IsHolstered $before.client.Native)){throw 'Initial sword state is not holstered'}
 foreach($role in 'host','client'){Command $role 'coop_guard_link escape 60000'}
 Start-Sleep -Milliseconds 500;$bound=Sample Bound;Held $bound
 Command host 'coop_guard_weapon on';Start-Sleep -Milliseconds 500;$hostOnly=Sample HostOnly;Held $hostOnly
 if($hostOnly.client.Weapon.active-or$hostOnly.client.Weapon.accepted-ne$bound.client.Weapon.accepted-or$hostOnly.client.Weapon.received-le$bound.client.Weapon.received-or!(IsHolstered $hostOnly.client.Native)){throw 'Client opt-in was not enforced'}
 $checks.Add('Host stream alone cannot control client equipment')
 Command client 'coop_guard_weapon on';Start-Sleep -Milliseconds 450;$holstered=Sample HolsteredStream;Held $holstered
 if(!$holstered.client.Weapon.active-or$holstered.client.Weapon.drawn-or$holstered.client.Weapon.accepted-le$hostOnly.client.Weapon.accepted-or!(IsHolstered $holstered.client.Native)){throw 'Native holstered source state did not transmit'}
 $checks.Add('Native holstered state is received under the current guard binding')

 # Place only the host counterpart near the native combat guard. The actual
 # client remains in the starting room, isolated from independent local NPCs.
 Command client 'setviewpos -42.9563 -117.27 62.7 270 0'
 Command host 'coop_probe_follow exact';Start-Sleep -Milliseconds 150;Command host 'coop_probe_follow off'
 Command client 'setviewpos -28.0921 -153.962 68.1 270 0';Start-Sleep -Milliseconds 400;Command host 'coop_probe_follow on';Start-Sleep -Milliseconds 300
 Command host 'coop_partner_enemy escape 60000'
 $drawn=WaitDrawn NativeCombatDraw
 for($i=0;$i-lt6;$i++){Start-Sleep -Milliseconds 130;$s=Sample Sustained;Held $s;if(!(IsDrawn $s.client.Native)){throw 'Replicated native weapon did not remain drawn'}}
 $sustained=Sample SustainedEnd;Held $sustained
 foreach($field in 'weaponHandle','entity','component','parent'){
  if($sustained.client.Native.$field-ne$before.client.Native.$field){throw "Client weapon identity changed: $field"}
 }
 if($sustained.client.Native.joint-eq$before.client.Native.joint-or$sustained.client.Equipment.started-ne$before.client.Equipment.started+1-or$sustained.client.Equipment.observed-ne$before.client.Equipment.observed+1-or
    $sustained.host.Weapon.sentDrawn-lt3-or$sustained.client.Equipment.lost-ne$before.client.Equipment.lost){throw 'Native attachment or stable ownership was not confirmed'}
 $checks.Add('Natural host combat draws the existing client sword into its native hand joint while client attack AI stays inactive')
 Command client 'coop_guard_equipment 2000';Start-Sleep -Milliseconds 200;$conflict=Sample LocalExperimentRefused;Held $conflict
 if($conflict.client.Equipment.started-ne$sustained.client.Equipment.started-or!$conflict.client.Equipment.active-or$conflict.client.Equipment.localExperiment){throw 'Local experiment stole streamed equipment ownership'}
 $checks.Add('Local timed experiment cannot replace active Steam equipment ownership')

 Command host 'coop_guard_weapon off';Start-Sleep -Milliseconds 650;$stale=Sample SourceOff;Held $stale
 if($stale.client.Weapon.active-or$stale.client.Equipment.active-or$stale.client.Equipment.restoring-or!(IsHolstered $stale.client.Native)-or!(IsDrawn $stale.host.Native)-or
    $stale.client.Equipment.restored-ne$before.client.Equipment.restored+1-or$stale.client.Equipment.lost-ne$before.client.Equipment.lost-or!$stale.client.Bridge.remoteFresh-or$stale.client.Guard.poseAgeMs-ge250){throw 'Equipment expiry did not restore while guard poses stayed fresh'}
 Start-Sleep -Milliseconds 350;$clientOnly=Sample ClientOnly;Held $clientOnly
 if($clientOnly.client.Equipment.active-or!(IsHolstered $clientOnly.client.Native)){throw 'Client alone revived expired equipment'}
 $checks.Add('Source disable expires equipment and restores the holster while the host remains drawn and root/Steam traffic stay fresh')

 Command host 'coop_guard_weapon on';$redrawn=WaitDrawn SourceResumed
 Command client 'coop_guard_weapon off';Start-Sleep -Milliseconds 500;$disabled=Sample ClientDisabled;Held $disabled
 if($disabled.client.Equipment.active-or$disabled.client.Equipment.restoring-or!(IsHolstered $disabled.client.Native)-or$disabled.client.Equipment.restored-ne$redrawn.client.Equipment.restored+1){throw 'Client disable did not restore native holster'}
 $checks.Add('Explicit client disable restores the native sword without releasing guard AI ownership')
 Command client 'coop_guard_weapon on';$redrawn=WaitDrawn ClientResumed
 Command client 'coop_guard_link off';Start-Sleep -Milliseconds 600;$released=Sample GuardReleased
 $decision=Native client '--ai-decision' $handles.client;$reflex=Native client '--ai-reflex' $handles.client
 # The strict in-process reader confirms a native holster transition before
 # incrementing restored. Once AI references are returned, native behavior
 # may draw the same weapon again. Preserve that external observation rather
 # than asserting persistent holstering after authority has been surrendered.
 if($released.client.Guard.phase-or$released.client.Guard.heldReferences-or$released.client.Weapon.active-or$released.client.Equipment.active-or$released.client.Equipment.restoring-or
    $released.client.Equipment.restored-ne$redrawn.client.Equipment.restored+1-or$released.client.Equipment.lost-ne$before.client.Equipment.lost-or$released.client.Equipment.started-ne$redrawn.client.Equipment.started-or
    !$decision.schedulerActive-or$decision.pauseReferences-or!$reflex.schedulerActive-or$reflex.pauseReferences){throw 'Guard release failed to finish native equipment cleanup and return AI ownership'}
 $handoff=[pscustomobject]@{NativeSwordDrawnAfterAiResume=(IsDrawn $released.client.Native);Decision=$decision;Reflex=$reflex;RestorationConfirmedBy='Strict native reader in module; external holster confirmation separately required while AI remains held'}
 $checks.Add('Guard release ends Steam equipment ownership, confirms cleanup and returns native AI; subsequent native equipment state is recorded separately')
 $damage=Status client '.damage.json'
 if(@($damage.events|Where-Object handle -eq 1).Count-or$released.client.Health.current-ne56){throw 'Equipment-only replication caused a local player damage event'}
}catch{$failure=$_.Exception.Message}finally{
 foreach($role in 'host','client'){foreach($cmd in 'coop_guard_weapon off','coop_sword_link off','coop_partner_enemy off','coop_guard_link off','coop_damage_observe off'){try{Command $role $cmd}catch{if(!$failure){$failure=$_.Exception.Message}}}}
}
[ordered]@{Passed=(!$failure);Failure=$failure;ObservedAt=[DateTimeOffset]::Now.ToString('o');DllSha256=(Get-FileHash "$PSScriptRoot/bin/dinput8.dll").Hash;ProbeSha256=(Get-FileHash $probe).Hash;
 Scope='One authored native guard sword state over Steam; original local weapon identity, native inventory slot, component activation, attachment tag and joint. Isolated observer fixture; no blade collision, visual alignment, player weapons or general inventory authority claim.';
 Checks=$checks;Damage=$damage;Handoff=$handoff;Samples=$samples}|ConvertTo-Json -Depth 18|Set-Content "$PSScriptRoot/research/guard-weapon-verification.json"
if($failure){throw $failure}
[pscustomobject]@{Passed=$true;Checks=$checks.Count;Samples=$samples.Count}
