from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
for a,b in [('Build-NpcCombat-v918.ps1','Build-NpcCombat-v922.ps1'),('Deploy-NpcCombat-v918.ps1','Deploy-NpcCombat-v922.ps1')]:
 (W/b).write_text((W/a).read_text().replace('918','922'))
t=(W/'stage_npc_combat_v918.py').read_text().replace('v918','v922').replace("P=W/'candidate-npc-combat-v915'","P=W/'candidate-npc-combat-v918'")
t=t.replace("scope='Adds generic", "scope='Adds an explicit once-per-combat-session native event reconstruction fixture for the observed map trigger damage, requiring the active finite isolated test lease. Source activation remains separately unverified. Adds generic")
(W/'stage_npc_combat_v922.py').write_text(t)
t=(N/'Test-NpcCombat-v920.ps1').read_text().replace('v920','v923').replace('candidate-npc-combat-v918','candidate-npc-combat-v922')
t=t.replace(' # Observe the active map near the target; no scripted damage injection.',''' # Reconstruct the observed native trigger callback once through the native
 # entry hook. This is an explicit synthetic event fixture, not natural map
 # trigger activation. Native evaluation/outcomes/effects remain real.
 C host 'coop_npc_combat trigger_fixture_once'
 Save "$out/trigger-fixture.json" @{scope='Reconstructed observed native damage event, once, host only. Does not prove native trigger activation or general campaign behavior.'}''')
t=t.replace("scope='Natural map wait, no event injection. Zero outcomes is explicitly unexercised.'","scope='Reconstructed native event fixture; requires positive world counters for acceptance.'")
t=t.replace(' $attempt=0', " if(!$natural.host.combat.worldCommitted-or$natural.host.combat.worldCommitted-ne$natural.client.combat.worldApplied){throw 'Native world fixture not exercised'}\n $attempt=0",1)
(N/'Test-NpcCombat-v923.ps1').write_text(t)
