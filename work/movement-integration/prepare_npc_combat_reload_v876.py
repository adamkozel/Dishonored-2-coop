from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';s=(N/'Test-NpcCombat-v874.ps1').read_text().replace('npc-combat-client-v874','npc-combat-reload-v876')
at=" Snap 'settled-death'|Out-Null"
assert at in s
s=s.replace(at,at+'''
 # Keep terminal combat identities present while the shared checkpoint owns
 # the world transition. Do not disable combat/stream ahead of this regression.
 $oldWorld=@{};foreach($r in 'host','client'){
  $oldWorld[$r]=@{bridge=(S $r);combat=(S $r '.npc-combat.json');stream=(S $r '.npc-stream.json');reset=(S $r '.reset.json')}
  C $r 'coop_probe_follow exact';C $r 'coop_probe_locomotion on';C $r 'coop_checkpoint_sync on';C $r 'coop_checkpoint_delay 0'
 }
 Save "$out/reload-before.json" $oldWorld
 $watch.Restart();do{$h=S host '.reset.json';$c=S client '.reset.json';if($h.enabled-and$c.enabled-and$h.peerArm-and$c.peerArm-and!$h.phase-and!$c.phase-and$h.digest-eq$c.digest){break};Start-Sleep -Milliseconds 60}while($watch.ElapsedMilliseconds-lt5000)
 if(!$h.peerArm-or!$c.peerArm-or$h.digest-ne$c.digest){throw 'Checkpoint negotiation failed'}
 C host 'RestartFromCheckpoint';C client 'RestartFromMemoryCheckpoint'
 $watch.Restart();$pressed=@{host=0;client=0}
 do{
  $ok=$true;$renewed=@{}
  foreach($r in 'host','client'){
   $rs=S $r '.reset.json';$br=S $r;$n=S $r '.npc-stream.json';$combat=S $r '.npc-combat.json'
   $renewed[$r]=@{reset=$rs;bridge=$br;stream=$n;combat=$combat}
   if($rs.phase-eq7){throw "Shared reload failed $($rs.result)"}
   if($rs.completed-ne$oldWorld[$r].reset.completed+1-or$br.localGameLogicState-ne1-or$br.localPendingGameLogicState-or!$rs.avatarReady-or$rs.avatarPending-or!$n.enabled-or$n.session.rearms-ne1-or@($n.entries|Where-Object {!$_.enabled-or!$_.ready}).Count){$ok=$false}
   if(!$rs.held-and$watch.ElapsedMilliseconds-gt6000-and$br.localGameLogicState-in8,15-and$watch.ElapsedMilliseconds-$pressed[$r]-gt700){& "$lab/Send-D2Input.ps1" -Role $r -Button A -DurationMs 100|Out-Null;$pressed[$r]=$watch.ElapsedMilliseconds}
  }
  if(!$ok){Start-Sleep -Milliseconds 80}
 }while(!$ok-and$watch.ElapsedMilliseconds-lt50000)
 Save "$out/reload-completed.json" $renewed;if(!$ok){throw 'NPC renewal after terminal combat did not finish'}
 foreach($r in 'host','client'){
  $new=$renewed[$r];$old=$oldWorld[$r]
  if($new.combat.enabled-or$new.combat.fixture.active-or$new.combat.fixture.lost-or$new.stream.session.lost-or$new.stream.session.retired-or$new.bridge.localEpoch-eq$old.bridge.localEpoch){throw 'Old combat or native ownership survived world change'}
  if($r-eq'client'-and$new.stream.session.released-ne$old.stream.session.acquired){throw 'Old client AI references were not released exactly once'}
  if(@($new.stream.entries|Where-Object {$_.arm-le($old.stream.entries.arm|Measure-Object -Maximum).Maximum}).Count){throw 'Fresh NPCs reused a terminal arm'}
  C $r 'coop_npc_combat duplicate';C $r 'coop_npc_combat on'
 }
 $watch.Restart();do{$ready=$true;$fresh=@{};foreach($r in 'host','client'){$fresh[$r]=S $r '.npc-combat.json';if(!$fresh[$r].enabled-or$fresh[$r].result-ne'active_native_melee'){$ready=$false}};if(!$ready){Start-Sleep -Milliseconds 80}}while(!$ready-and$watch.ElapsedMilliseconds-lt7000)
 Save "$out/rearmed-combat.json" $fresh;if(!$ready){throw 'Fresh combat negotiation failed'}
 foreach($r in 'host','client'){
  if($fresh[$r].nonce-le$oldWorld[$r].combat.nonce-or$fresh[$r].deaths-or$fresh[$r].committed-or$fresh[$r].applied-or@($fresh[$r].entries|Where-Object {$_.terminal-or$_.deathPending-or$_.event-or$_.pending}).Count){throw 'Terminal outcomes contaminated the new world'}
 }
 Save "$out/reload-verified.json" @{passed=$true;scope='Shared checkpoint after native death releases remaining old AI, discards terminal combat scope, restores fresh NPC arms and negotiates a new combat nonce with no stale damage.'}
''')
s=s.replace("'coop_npc_stream off','coop_damage_observe off'", "'coop_npc_stream off','coop_checkpoint_sync off','coop_checkpoint_delay 0','coop_damage_observe off'")
s=s.replace("scope='Experimental native lethal action, actor ownership retirement and duplicate delivery.'", "scope='Native lethal outcome and duplicates followed by shared checkpoint, old-reference release and fresh combat negotiation.'")
(N/'Test-NpcCombat-v876.ps1').write_text(s)
print('Prepared native lethal-to-shared-checkpoint regression876')
