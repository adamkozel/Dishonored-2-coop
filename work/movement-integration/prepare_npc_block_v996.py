from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-NpcNativeAttack-v989.ps1').read_text().replace('npc-native-attack-v989','npc-native-block-v996').replace('candidate-npc-combat-v988','candidate-npc-combat-v994').replace("'-t','12'","'-t','16'")
needle=" C client 'setviewpos -42.9563 -122 62.7 90 0'";assert s.count(needle)==1
s=s.replace(needle,''' $ramsey=@((Get-Content "$out/world-before-host.json" -Raw|ConvertFrom-Json).npcs|Where-Object definition -like '*mortimer_ramsey*')
 if($ramsey.Count-ne1){throw 'Unique native Ramsey required for frontal defense setup'}
 $ramseyHandle=[Convert]::ToUInt32($ramsey[0].handle.Replace('0x',''),16);$rp=Q host '--npc-pose' $ramseyHandle
 $faceYaw=[Math]::Atan2($rp.root[1]+122,$rp.root[0]+42.9563)*180/[Math]::PI
 & "$lab/Send-D2Input.ps1" -Role client -Button RightShoulder -DurationMs 10000|Out-Null
 C client ("setviewpos -42.9563 -122 62.7 {0} 0"-f$faceYaw)
 Save "$out/defense-setup.json" @{input='Ordinary native RightShoulder hold,10000ms';ramsey=$rp;faceYaw=$faceYaw}
''')
s=s.replace("clientPlayerHealth=(Q client '--health' 1)","clientPlayerHealth=(Q client '--health' 1);playerStates=(Q client '--player-states');view=(Q client '--player');incoming=(S client '.npc-attack.json')")
s=s.replace('if($row.clientPlayerHealth.current-lt$playerInitial.current){','if($row.incoming.applied-ge3-or$row.clientPlayerHealth.current-le16){')
needle=' Save "$out/native-samples.json" @($samples)';idx=s.index(needle)
s=s[:idx]+''' & "$lab/Send-D2Input.ps1" -Role client -DurationMs 100|Out-Null
 C client 'setviewpos -33 -148 68.1 270 0'
''' +s[idx:]
s=s.replace('-or$beforeHealth.current-ge$playerInitial.current','')
needle=' C host \'coop_npc_attack duplicate\'';idx=s.index(needle)
s=s[:idx]+''' $defenseEvents=@((S client '.damage.json').events|Where-Object {$_.handle-eq1-and$_.damageType-eq'80'-and$_.protectedRegions-and$_.inflictedDamage-eq0})
 $nativePoses=@($samples|Where-Object {$_.playerStates.combatSceneValid-and$_.playerStates.combatScene.variant-in10,11,13})
 Save "$out/defense-results.json" @{blocked=$defenseEvents;nativeBlockSamples=$nativePoses.Count;host=$hs;client=$cs;health=$beforeHealth}
 if(!$defenseEvents.Count-or!$nativePoses.Count){throw 'No native protected zero-damage contact and held-block animation established'}
''' +s[idx:]
s=s.replace("scope='Opt-in988: native bound-enemy sword contact to owned host clone routes through Steam to actual client player, native damage/health/reaction once, clone health40 retained, duplicate rejected. Not all damage types or full lifecycle/combat.'", "scope='Opt-in994: actual client native held-block scene, at least one protected zero-damage relayed enemy sword contact, clone40 and duplicate rejection. Not timed parry or host-attacker recoil/stagger acceptance.'")
s=s.replace("   foreach($r in 'host','client'){foreach($cmd", "   try{& \"$lab/Send-D2Input.ps1\" -Role client -DurationMs 100|Out-Null}catch{$errors.Add($_.Exception.Message)}\n   foreach($r in 'host','client'){foreach($cmd")
(N/'Test-NpcNativeBlock-v996.ps1').write_text(s)
print('Prepared bounded held-block experiment996')
