from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
t=(W/'publish_npc_combat_v924.py').read_text()
t=t[t.index('from pathlib import Path'):]
t=t.replace("A=W/'candidate-npc-combat-v922'","A=W/'candidate-npc-combat-v926'").replace('npc-combat-audit-v924.json','npc-combat-audit-v928.json')
t=t.replace("('npc-combat-fixture-v923',922,None)]","('npc-combat-fixture-v923',922,False),('npc-combat-fixture-v927',926,None),('npc-combat-client-v929',926,None)]")
t=t.replace("name=='npc-combat-fixture-v923'","name in ('npc-combat-fixture-v923','npc-combat-fixture-v927')")
t=t.replace("latest=cases[-1]", "latest=cases[-1];fixture=cases[-2]")
t=t.replace("explicit native event fixture922 implemented.","explicit native event fixture922 and shared contact geometry926 implemented.")
t=t.replace("Latest923: '+latest['scope']", "Latest927 fixture: '+fixture['scope']+' Latest929 client regression: '+latest['scope']")
t=t.replace('Continue from audit924','Continue from audit928')
(W/'publish_npc_combat_v928.py').write_text('"""Audit measured results; do not infer untested trigger activation from fixture outcomes."""\n'+t)
t=(N/'Test-NpcCombat-v919.ps1').read_text().replace('v919','v929').replace('candidate-npc-combat-v918','candidate-npc-combat-v926')
(N/'Test-NpcCombat-v929.ps1').write_text(t)
