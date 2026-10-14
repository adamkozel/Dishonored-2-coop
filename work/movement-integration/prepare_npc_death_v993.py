from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(N/'Test-NpcNativeDeath-v990.ps1').read_text().replace('npc-native-death-v990','npc-native-death-v993').replace('candidate-npc-combat-v988','candidate-npc-combat-v992')
s=s.replace("$new.attack.enabled-or$new.stream.session.lost", "$new.stream.session.lost")
s=s.replace("  C $r 'coop_npc_attack on'\n","")
s=s.replace("$fresh[$r].pending){throw 'Old damage contaminated fresh attack scope'}", "$fresh[$r].pending-or$fresh[$r].session.renewals-ne1-or!$fresh[$r].session.enabled-or$fresh[$r].session.waitingCheckpoint){throw 'Old damage contaminated fresh attack scope or automatic renewal failed'}")
needle=' if($pair.client.attack.applied-lt2)';assert needle in s
s=s.replace(needle,''' foreach($r in 'host','client'){if(!$pair[$r].attack.session.enabled-or!$pair[$r].attack.session.waitingCheckpoint-or!$pair[$r].attack.session.terminal-or$pair[$r].attack.pending){throw 'Native terminal outcome did not retain clean checkpoint intent'}}
 if($pair.host.attack.event-ne$pair.client.attack.applied){throw 'Extra host contact was emitted after the lethal client result'}
''' +needle)
needle=' Save "$out/verified.json"';a=s.index(needle)
s=s[:a]+''' C host 'coop_npc_attack off';$watch.Restart();do{$stopped=@{};foreach($r in 'host','client'){$stopped[$r]=S $r '.npc-attack.json'};if(!$stopped.host.enabled-and!$stopped.client.enabled-and!$stopped.client.session.enabled){break};Start-Sleep -Milliseconds 80}while($watch.ElapsedMilliseconds-lt5000)
 Save "$out/one-sided-stop.json" $stopped
 if($stopped.host.enabled-or$stopped.client.enabled-or$stopped.host.session.enabled-or$stopped.client.session.enabled-or$stopped.client.result-ne'peer_stopped'){throw 'One-sided stop failed to retire both attack scopes'}
''' +s[a:]
s=s.replace('explicit attack rearm without prior hits. Not automatic attack-session renewal, all damage types, or full campaign.', 'automatic attack renewal without prior hits and one-sided peer stop. Not native-menu renewal, all damage types, or full campaign.')
(N/'Test-NpcNativeDeath-v993.ps1').write_text(s)
p=W/'stage_npc_attack_v992.py';p.write_text(p.read_text().replace('packet176 v1','packet176 v2'))
print('Prepared993: native terminal, automatic checkpoint renewal, one-sided off')
