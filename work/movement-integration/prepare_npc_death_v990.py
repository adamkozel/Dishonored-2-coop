from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-NpcNativeAttack-v989.ps1').read_text().replace('npc-native-attack-v989','npc-native-death-v990').replace("'-t','12'","'-t','25'")
a=" $playerInitial=Q client '--health' 1;Save \"$out/player-initial.json\" $playerInitial\n";assert a in s;s=s.replace(a,a+''' foreach($r in 'host','client'){C $r 'coop_checkpoint_sync on';C $r 'coop_checkpoint_delay 0'}
 $cw=[Diagnostics.Stopwatch]::StartNew();do{$h=S host '.reset.json';$c=S client '.reset.json';if($h.enabled-and$c.enabled-and$h.peerArm-and$c.peerArm-and!$h.phase-and!$c.phase-and$h.digest-eq$c.digest){break};Start-Sleep -Milliseconds 50}while($cw.ElapsedMilliseconds-lt5000)
 if(!$h.peerArm-or!$c.peerArm-or$h.digest-ne$c.digest){throw 'Checkpoint negotiation failed'}
 $oldWorld=@{};foreach($r in 'host','client'){$oldWorld[$r]=@{bridge=(S $r);reset=(S $r '.reset.json');stream=(S $r '.npc-stream.json');attack=(S $r '.npc-attack.json')}}
 Save "$out/before-death.json" $oldWorld
''')
start=s.index(' $watch.Restart();$selected=0;$attacking=0');end=s.index('\n}catch{$failure=',start)
s=s[:start]+''' $watch.Restart();$screensReady=$false
 do{
  $pair=@{};$screensReady=$true
  foreach($r in 'host','client'){
   $rs=S $r '.reset.json';$br=S $r;$pair[$r]=@{bridge=$br;reset=$rs;attack=(S $r '.npc-attack.json');stream=(S $r '.npc-stream.json');health=(Q $r '--health' 1)}
   if($rs.phase-eq7){throw "Shared enemy death failed: $r $($rs.result)"}
   if($br.localGameLogicState-ne13-or!$rs.deathReady-or!$rs.peerDeathReady){$screensReady=$false}
  }
  $samples.Add(@{ms=$watch.ElapsedMilliseconds;pair=$pair})
  if(!$screensReady){Start-Sleep -Milliseconds 100}
 }while(!$screensReady-and$watch.ElapsedMilliseconds-lt23000)
 Save "$out/death-samples.json" @($samples);Save "$out/death-pair.json" $pair
 if(!$screensReady){throw 'Both native death screens were not reached from repeated relayed enemy hits'}
 foreach($r in 'host','client'){
  $g=Q $r '--gameover';Save "$out/gameover-$r.json" $g
  if($g.logic-ne13-or$g.pending-or$g.flash-eq'0x0'-or@($g.showFunction).Count-ne2-or$g.showFunction[0]-eq'0x0'-or$g.showFunction[1]-eq'0x0'){throw 'Native death UI identity incomplete'}
  if($pair[$r].health.current-ne$(if($r-eq'client'){0}else{56})-or$pair[$r].reset.deathCount-ne$oldWorld[$r].reset.deathCount+1-or$pair[$r].reset.loads-ne$oldWorld[$r].reset.loads-or$pair[$r].attack.enabled){throw 'Shared death applied wrong health or failed to retire attack capture'}
 }
 if($pair.client.attack.applied-lt2){throw 'Repeated native enemy applications not established'}
 foreach($r in 'host','client'){Save "$out/damage-final-$r.json" (S $r '.damage.json')}
 foreach($capture in $captures){if(!$capture.WaitForExit(15000)-or$capture.ExitCode){throw 'Death capture did not finish before reload'}}
 Save "$out/capture-ended-before-reload.json" @{passed=$true}
 C host 'coop_gameover_retry';C client 'RestartFromCheckpoint'
 $watch.Restart();$pressed=@{host=0;client=0}
 do{
  $ok=$true;$renewed=@{}
  foreach($r in 'host','client'){
   $rs=S $r '.reset.json';$br=S $r;$n=S $r '.npc-stream.json';$attack=S $r '.npc-attack.json'
   $renewed[$r]=@{reset=$rs;bridge=$br;stream=$n;attack=$attack}
   if($rs.phase-eq7){Save "$out/reload-failed-$r.json" $renewed;throw "Shared reload failed $($rs.result)"}
   if($rs.completed-ne$oldWorld[$r].reset.completed+1-or$br.localGameLogicState-ne1-or$br.localPendingGameLogicState-or!$rs.avatarReady-or$rs.avatarPending-or!$n.enabled-or$n.session.rearms-ne1-or@($n.entries|Where-Object {!$_.enabled-or!$_.ready}).Count){$ok=$false}
   if(!$rs.held-and$watch.ElapsedMilliseconds-gt6000-and$br.localGameLogicState-in8,15-and$watch.ElapsedMilliseconds-$pressed[$r]-gt700){& "$lab/Send-D2Input.ps1" -Role $r -Button A -DurationMs 100|Out-Null;$pressed[$r]=$watch.ElapsedMilliseconds}
  }
  if(!$ok){Start-Sleep -Milliseconds 80}
 }while(!$ok-and$watch.ElapsedMilliseconds-lt50000)
 Save "$out/reload-completed.json" $renewed;if(!$ok){throw 'NPC renewal after enemy-caused shared death did not finish'}
 foreach($r in 'host','client'){
  $new=$renewed[$r];$old=$oldWorld[$r]
  if($new.attack.enabled-or$new.stream.session.lost-or$new.bridge.localEpoch-eq$old.bridge.localEpoch-or$new.reset.loads-ne$old.reset.loads+1){throw 'Old attack ownership survived or native load count differs'}
  if(@($new.stream.entries|Where-Object {$_.arm-le($old.stream.entries.arm|Measure-Object -Maximum).Maximum}).Count){throw 'Fresh NPCs reused prior-world arms'}
  Save "$out/recovered-character-$r.json" (Q $r '--character')
  if((Q $r '--character').character-ne$(if($r-eq'host'){1}else{2})-or(Q $r '--health' 1).current-ne56-or(Q $r '--health' $new.bridge.probeActorHandle).current-ne40){throw 'Roles or native health not recovered'}
  C $r 'coop_npc_attack on'
 }
 $watch.Restart();do{$ready=$true;$fresh=@{};foreach($r in 'host','client'){$fresh[$r]=S $r '.npc-attack.json';if(!$fresh[$r].enabled-or$fresh[$r].result-ne'active_native_sword'){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 80}}while(!$ready-and$watch.ElapsedMilliseconds-lt7000)
 Save "$out/rearmed-attack.json" $fresh;if(!$ready){throw 'Fresh enemy attack negotiation failed'}
 foreach($r in 'host','client'){if($fresh[$r].nonce-le$oldWorld[$r].attack.nonce-or$fresh[$r].event-or$fresh[$r].consumed-or$fresh[$r].applied-or$fresh[$r].pending){throw 'Old damage contaminated fresh attack scope'}}
 Save "$out/verified.json" @{passed=$true;applications=$pair.client.attack.applied;nativeOverrideUsed=$false;scope='Repeated host-owned enemy sword contacts cause actual client death, both native death screens without harming host, one coordinated checkpoint retry, fresh roles/avatars/NPC arms and explicit attack rearm without prior hits. Not automatic attack-session renewal, all damage types, or full campaign.'}
''' +s[end:]
s=s.replace("'coop_npc_attack off','coop_avatar_perception off'","'coop_checkpoint_sync off','coop_checkpoint_delay 0','coop_npc_attack off','coop_avatar_perception off'")
s=s.replace("scope='Bounded generic native enemy sword routing to actual remote player; restored isolated lab.'", "scope='Repeated native enemy hits, shared death screens and one coordinated checkpoint; restored isolated lab.'")
(N/'Test-NpcNativeDeath-v990.ps1').write_text(s)
