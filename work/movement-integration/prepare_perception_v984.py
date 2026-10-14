from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-NpcNativePerception-v981.ps1').read_text().replace('npc-native-perception-v981','npc-native-perception-v984').replace('candidate-npc-combat-v966','candidate-npc-combat-v983').replace('read_observable_v981','read_observable_v984')
a=" $dirty=$true\n";assert s.count(a)==1;s=s.replace(a,a+" foreach($r in 'host','client'){C $r 'coop_avatar_perception on';Save \"$out/perception-before-$r.json\" (S $r '.avatar-perception.json')}\n")
a=" $avatar=(S host).probeActorHandle;";assert a in s;s=s.replace(a," $enemyHandles=@((Get-Content \"$out/world-before-host.json\" -Raw|ConvertFrom-Json).npcs|Where-Object {$_.spawnerName -like '*guard*' -or $_.definition -like '*mortimer_ramsey*'}|ForEach-Object {[Convert]::ToUInt32($_.handle.Replace('0x',''),16)})\n"+a)
a="  $targets=Q host '--ai-targets' $handles.host;";assert a in s;s=s.replace(a,"  $allTargets=@();foreach($eh in $enemyHandles){$allTargets+=Q host '--ai-targets' $eh}\n"+a)
a='$row=@{ms=$watch.ElapsedMilliseconds;targets=$targets;';assert a in s;s=s.replace(a,'$row=@{ms=$watch.ElapsedMilliseconds;allTargets=$allTargets;targets=$targets;')
a='if($targets.combat.resolved-and$targets.combat.handle-eq$avatar){$selected++;if($attack.schedulerActive){$attacking++}}';assert a in s;s=s.replace(a,'if(@($allTargets|Where-Object {$_.combat.resolved-and$_.combat.handle-eq$avatar}).Count){$selected++};if($attack.schedulerActive){$attacking++}')
a=' if($selected-lt3)';assert a in s;s=s.replace(a,''' foreach($r in 'host','client'){Save "$out/perception-after-$r.json" (S $r '.avatar-perception.json')}
 $ap=S host '.avatar-perception.json';if(!$ap.enabled-or!$ap.installed-or$ap.retained-lt100-or!$ap.avatarAiHeld){throw 'Perception retention not active or owned clone AI was resumed'}
 if(!(S host).probeFollowEnabled){throw 'Remote following must remain enabled'}
 if($selected-lt3)''')
s=s.replace("'coop_shadow_stream off','coop_npc_stream off','coop_damage_observe off'","'coop_avatar_perception off','coop_shadow_stream off','coop_npc_stream off','coop_damage_observe off'")
s=s.replace('Native enemy did not sustain remote-avatar combat target in this fixture','No native enemy sustained remote-avatar combat target in this fixture')
s=s.replace("scope='Native host NPC combat-target selection", "scope='Candidate983 owned continuous-transform observable retention. Native host NPC combat-target selection")
(N/'Test-NpcNativePerception-v984.ps1').write_text(s)
s=(R/'work/movement-integration/read_observable_v981.py').read_text().replace("out=W/'observable-readonly-v981'","out=W/'observable-readonly-v984'")
(R/'work/movement-integration/read_observable_v984.py').write_text(s)
