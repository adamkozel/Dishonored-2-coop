from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-NpcCombat-v867.ps1').read_text()
def rep(a,b):
 global s
 assert a in s,a[:80]
 s=s.replace(a,b)
rep("param([string]$Take='npc-combat-v867')","param([ValidateSet('host','client')][string]$Source='host',[string]$Take='npc-combat-host-v870')")
rep('candidate-npc-combat-v865','candidate-npc-combat-v869')
rep("'active_nonfatal_sword'","'active_native_melee'")
rep(" foreach($r in 'host','client'){C $r 'coop_npc_combat ordinary40000'}\n",'')
rep("'-t','30'","'-t','36'")
a=s.index(' $hit=0\n');b=s.index('\n}catch{$failure=',a)
s=s[:a]+''' $rig=Q host '--npc-pose' $handles.host;Save "$out/target-before.json" $rig;$t=$rig.root
 # Natural rear sword action; no action-selection exclusion.
 $x=$t[0]-0.95*$t[3];$y=$t[1]-0.95*$t[4];$z=$t[2]+1.7;$yaw=[Math]::Atan2($t[4],$t[3])*180/[Math]::PI
 C $Source ("setviewpos {0} {1} {2} {3} 0"-f$x,$y,$z,$yaw)
 $observer=if($Source-eq'host'){'client'}else{'host'}
 C $observer ("setviewpos {0} {1} {2} {3} 0"-f($x-1.8*$t[4]),($y+1.8*$t[3]),$z,($yaw-45))
 Start-Sleep -Milliseconds 350
 & "$lab/Send-D2Input.ps1" -Role $Source -RightTrigger 255 -DurationMs 220|Out-Null
 $watch.Restart();do{
  $hs=S host '.npc-combat.json';$cs=S client '.npc-combat.json'
  if(!$hs.enabled-or!$cs.enabled){break}
  if($hs.deaths-eq1-and$cs.deaths-eq1){break};Start-Sleep -Milliseconds 80
 }while($watch.ElapsedMilliseconds-lt8000)
 $after=Snap 'native-death'
 if(!$hs.enabled-or!$cs.enabled-or$hs.deaths-ne1-or$cs.deaths-ne1-or$after.host.health.current-ne0-or$after.client.health.current-ne0-or$hs.committed-ne$cs.applied){throw "Native death did not converge: host $($hs.result); client $($cs.result)"}
 foreach($r in 'host','client'){
  $e=@($after[$r].stream.entries|Where-Object handle -eq $handles[$r])
  if($e.Count-ne1-or$e[0].enabled-or$e[0].references-or$e[0].lost-or$e[0].retired){throw "Pre-death release failed $r"}
  $terminal=@($after[$r].combat.entries|Where-Object arm -eq $e[0].arm)
  if($terminal.Count-ne1-or!$terminal[0].terminal){throw "Missing death tombstone $r"}
  foreach($ai in 'decision','reflex'){$a=Q $r "--ai-$ai" $handles[$r];Save "$out/dead-$r-$ai.json" $a;if($a.schedulerActive){throw "Dead AI still active $r $ai"}}
 }
 C $Source 'coop_npc_combat duplicate';if($Source-eq'client'){C host 'coop_npc_combat duplicate'};Start-Sleep -Milliseconds 500
 $dup=Snap 'duplicate-death'
 foreach($r in 'host','client'){
  if($dup[$r].health.current-ne0-or$dup[$r].combat.committed-ne$after[$r].combat.committed-or$dup[$r].combat.applied-ne$after[$r].combat.applied-or$dup[$r].combat.deaths-ne1){throw 'Duplicate death reapplied'}
 }
 if($dup.client.combat.duplicates-le$after.client.combat.duplicates){throw 'Duplicate terminal commit was not recognized'}
 if($Source-eq'client'-and$dup.host.combat.duplicates-le$after.host.combat.duplicates){throw 'Duplicate terminal request was not recognized'}
 Start-Sleep -Milliseconds 3500
 Snap 'settled-death'|Out-Null
 Save "$out/verified.json" @{source=$Source;hostCommits=$hs.committed;clientNativeApplications=$cs.applied;scope='One real controller-triggered native lethal action with exact actor release, native stopped AI and duplicate rejection. Full paired victim choreography and corpse physics require visual judgment and separate coverage.'}
''' + s[b:]
rep("scope='Bounded repeated nonfatal sword authority on generic NPC stream, both player sources and duplicate delivery.'","source=$Source;scope='Experimental native lethal action, actor ownership retirement and duplicate delivery.'")
(N/'Test-NpcCombat-v870.ps1').write_text(s)
print('Prepared real input lethal harness870; protected baseline restoration retained')
