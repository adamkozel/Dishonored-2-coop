from pathlib import Path
R=Path(__file__).resolve().parents[2]; N=R/'work/natural-moveset'; out=N/'Test-NpcDualRanged-v971.ps1'
assert not out.exists()
s=(N/'Test-NpcRangedProfile-v970.ps1').read_text(encoding='utf-8-sig')
s=s[s.index('\n')+1:]
s="param([Parameter(Mandatory)][string]$Take)\n$Source='both';$Weapon='crossbow';$ExactAim=$true;$RequireNonterminal=$true\n"+s
s=s.replace('$aimBefore=$null;$aimChanged=$false','$aimBefore=@{};$aimChanged=@{host=$false;client=$false}')
a=s.index('function RestoreAim {'); b=s.index('function Retreat',a)
s=s[:a]+'''function RestoreAim {
 foreach($r in 'host','client'){
  if(!$script:aimChanged[$r]){continue}
  C $r ("ark_player_autoaim_model_pad {0}"-f$script:aimBefore[$r].controller)
  $a=Q $r '--aim-settings';Save "$out/aim-restored-$r.json" $a
  if($a.controller-ne$script:aimBefore[$r].controller-or$a.mouse-ne$script:aimBefore[$r].mouse){throw "Original native aim preference not restored $r"}
  $script:aimChanged[$r]=$false
 }
}
'''+s[b:]
s=s.replace("Save \"$out/test-profile.json\" @{weapon=$Weapon;aimPitch=$AimPitch;maximumAttempts=$MaximumAttempts;requireNonterminal=[bool]$RequireNonterminal;exactAim=[bool]$ExactAim}","Save \"$out/test-profile.json\" @{weapon=$Weapon;source='both';requireNonterminal=$true;expectedHealth=10;ordinaryInputs=2;scope='Two closely spaced ordinary inputs; wall-clock input and callback ticks recorded. No claim of exact simultaneous native execution.'}")
a=s.index(' C $Source ("coop_ranged_fixture'); b=s.index(" foreach($r in 'host','client'){\n  $hwnd=",a)
s=s[:a]+''' foreach($r in 'host','client'){C $r 'coop_ranged_fixture crossbow_loaded'}
 $watch.Restart();do{
  $ready=$true;foreach($r in 'host','client'){$f=S $r '.ranged-fixture.json';if($f.result-ne'native_ranged_ready'){$ready=$false}}
  if(!$ready){Start-Sleep -Milliseconds 60}
 }while(!$ready-and$watch.ElapsedMilliseconds-lt5000)
 if(!$ready){throw 'Both native crossbows did not become ready'}
 foreach($r in 'host','client'){& "$lab/Send-D2Input.ps1" -Role $r -Button X -DurationMs 1500|Out-Null};Start-Sleep -Milliseconds 1800
 foreach($r in 'host','client'){& "$lab/Send-D2Input.ps1" -Role $r -Button X -DurationMs 650|Out-Null};Start-Sleep -Milliseconds 1800
 $rig=Q host '--npc-pose' $handles.host;Save "$out/target-before.json" $rig;$t=$rig.root
 foreach($r in 'host','client'){
  $aimBefore[$r]=Q $r '--aim-settings';Save "$out/aim-before-$r.json" $aimBefore[$r]
  $aimChanged[$r]=$true;C $r 'ark_player_autoaim_model_pad 0';$a=Q $r '--aim-settings';Save "$out/aim-exact-$r.json" $a
  if($a.controller-ne0-or$a.mouse-ne$aimBefore[$r].mouse){throw "Native exact-aim setting did not apply $r"}
  Save "$out/weapon-$r.json" (Q $r '--player-crossbow')
  $side=if($r-eq'host'){-1.1}else{1.1}
  $x=$t[0]-3*$t[3]+$side*$t[4];$y=$t[1]-3*$t[4]-$side*$t[3];$z=$t[2]+1.7
  $yaw=[Math]::Atan2(($t[1]-$y),($t[0]-$x))*180/[Math]::PI
  C $r ("setviewpos {0} {1} {2} {3} 0"-f$x,$y,$z,$yaw)
 }
 Start-Sleep -Milliseconds 350
 foreach($r in 'host','client'){
  Save "$out/camera-before-aim-$r.json" (Q $r '--player')
  & "$PSScriptRoot/Set-NativeInteractionAim-v663.ps1" -Role $r -X $t[0] -Y $t[1] -Z ($t[2]+1.05) -Evidence "$out/measured-body-aim-$r.json"
  Save "$out/camera-after-aim-$r.json" (Q $r '--player')
  $bank=$pin.banks|Where-Object {$_.path -like $(if($r-eq'host'){'*emily*'}else{'*corvo*'})}
  if(@($bank).Count-ne1-or(Get-FileHash "$root/work/lab-instances/$r/coop-status.json.movement.bin").Hash-ne$bank.sha256){throw 'Wrong installed movement bank'}
  C $r 'coop_shadow_stream on'
 }
 Start-Sleep -Milliseconds 550
 foreach($r in 'host','client'){C $r 'coop_movement_bank on'}
 $watch.Restart();do{
  $ready=$true;foreach($r in 'host','client'){$m=S $r '.movement.json';$prop=S $r '.ranged-prop.json';if(!$m.active-or$m.version-ne67-or$m.rangedWeight-lt.9-or!$prop.active){$ready=$false}}
  if(!$ready){Start-Sleep -Milliseconds 70}
 }while(!$ready-and$watch.ElapsedMilliseconds-lt5000)
 foreach($r in 'host','client'){Save "$out/presentation-ready-$r.json" @{movement=(S $r '.movement.json');prop=(S $r '.ranged-prop.json');fx=(S $r '.power-fx.json')}}
 if(!$ready){throw 'Both remote presentations did not become ready'}

'''+s[b:]
a=s.index(' $attempt=0'); b=s.index(" $after=Snap 'native-death'",a)
s=s[:a]+''' $attempt=1
 foreach($r in 'host','client'){
  $view=Q $r '--player';Save "$out/camera-before-shot-$r.json" $view
  $eye=$view.nativeViewPosition;$forward=$view.nativeViewOrientation
  $delta=@(($t[0]-$eye[0]),($t[1]-$eye[1]),($t[2]+1.05-$eye[2]));$length=[Math]::Sqrt($delta[0]*$delta[0]+$delta[1]*$delta[1]+$delta[2]*$delta[2])
  $dot=($delta[0]*$forward[0]+$delta[1]*$forward[1]+$delta[2]*$forward[2])/$length
  if($dot-lt[Math]::Cos(1.5*[Math]::PI/180)){throw "Measured body aim drifted $r"}
 }
 $inputs=@{}
 foreach($r in 'host','client'){
  $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()
  $receipt=& "$lab/Send-D2Input.ps1" -Role $r -LeftTrigger 255 -DurationMs 220
  $inputs[$r]=@{start=$start;end=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();receipt=$receipt}
 }
 Save "$out/dual-inputs.json" $inputs
 $watch.Restart();do{
  $hs=S host '.npc-combat.json';$cs=S client '.npc-combat.json'
  if(!$hs.enabled-or!$cs.enabled-or($hs.committed-ge2-and$cs.applied-eq$hs.committed)){break};Start-Sleep -Milliseconds 60
 }while($watch.ElapsedMilliseconds-lt5000)
 $contact=Snap 'contact-0'
 foreach($r in 'host','client'){
  $present=@{movement=(S $r '.movement.json');prop=(S $r '.ranged-prop.json');fx=(S $r '.power-fx.json')};Save "$out/presentation-after-$r.json" $present
  if(!$present.movement.active-or$present.movement.rangedWeight-lt.9-or$present.movement.refused-or!$present.prop.active-or$present.prop.refused){throw "Combined ranged presentation failed $r"}
 }
 if(!$hs.enabled-or!$cs.enabled-or$hs.committed-ne2-or$cs.applied-ne2-or$hs.requests-ne1-or$cs.pendingRequests-or$contact.host.health.current-ne10-or$contact.client.health.current-ne10){throw "Two-source native outcomes did not converge: $($hs.result) / $($cs.result)"}
'''+s[b:]
s=s.replace(" C $Source 'coop_npc_combat duplicate';if($Source-eq'client'){C host 'coop_npc_combat duplicate'};Start-Sleep -Milliseconds 500"," C client 'coop_npc_combat duplicate';C host 'coop_npc_combat duplicate';Start-Sleep -Milliseconds 500")
s=s.replace("if($Source-eq'client'-and$dup.host.combat.duplicates-le",'if($dup.host.combat.duplicates-le')
a=s.index(' $sourceTrace=@(');b=s.index('\n}catch',a)
s=s[:a]+''' $sourceTrace=@{}
 foreach($r in 'host','client'){$sourceTrace[$r]=@($after[$r].combat.sourceHistory|Where-Object nonce -eq $before[$r].combat.nonce)}
 Save "$out/source-trace.json" $sourceTrace
 if(!$sourceTrace.host.Count-or!$sourceTrace.client.Count){throw 'Missing callback source proof for one player'}
 Save "$out/verified.json" @{source='both';inputAttempts=2;hostCommits=$hs.committed;hostClientRequests=$hs.requests;pendingHighWater=$cs.pendingHighWater;sourceCaptures=@{host=$sourceTrace.host.Count;client=$sourceTrace.client.Count};clientNativeApplications=$cs.applied;scope='Two ordinary crossbow inputs from separate player roles, ordered native damage40-to25-to10, duplicates, shared checkpoint/rearm and restoration. Close-input ordering is measured; exact simultaneity and active NPC combat are not established.'}
'''+s[b:]
out.write_text(s,encoding='utf-8'); print(out)
