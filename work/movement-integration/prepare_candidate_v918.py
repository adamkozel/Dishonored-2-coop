from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration'
p=W/'stage_npc_combat_v918.py';assert not p.exists()
t=(W/'stage_npc_combat_v915.py').read_text().replace('v915','v918').replace("P=W/'candidate-npc-combat-v911'","P=W/'candidate-npc-combat-v915'")
a=t.index("scope='");b=t.index("')\n(A/",a)
t=t[:a]+"scope='Adds generic host-owned authored map-trigger damage with unique native trigger names/incarnations and an exact-installation declaration catalog, dynamic bounded amount, native absent hit geometry, duplicate and native zero-result validation. Combat wire4/256 bytes. Retains candidate915 corpse, melee, replica authority, evaluated subtraction and pending-request fixes. Runtime verification pending.'"+t[b+1:]
p.write_text(t)
(W/'Deploy-NpcCombat-v918.ps1').write_text((W/'Deploy-NpcCombat-v915.ps1').read_text().replace('915','918'))
t=(R/'work/natural-moveset/Test-NpcCombat-v916.ps1').read_text().replace('v916','v919').replace('candidate-npc-combat-v915','candidate-npc-combat-v918')
t=t.replace('Only positive counters establish an exercised native corpse outcome. Zero is a regression pass only.','Positive counters establish host-owned world outcomes; inspect declarations to distinguish authored trigger damage from corpse contact. Zero is a regression pass only.')
(R/'work/natural-moveset/Test-NpcCombat-v919.ps1').write_text(t)
print('Prepared918 deployment and919 integration harness; no native mutation performed.')
