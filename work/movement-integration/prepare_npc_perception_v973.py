from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab'
p=L/'src/engine_bridge.cpp';s=p.read_text(encoding='utf-8-sig');a='serviceNpcCombat(tick);serviceMesmerizeFixture(tick);';assert s.count(a)==1;s=s.replace(a,'serviceNpcCombat(tick);serviceNpcPerception(tick);serviceMesmerizeFixture(tick);');p.write_text(s,encoding='utf-8')
p=L/'Send-D2Command.ps1';s=p.read_text(encoding='utf-8-sig');a='|coop_npc_combat|';assert s.count(a)==1;s=s.replace(a,'|coop_npc_combat|coop_npc_perception|');s+="\n" if not s.endswith('\n') else '';p.write_text(s,encoding='utf-8')
for stem in ('Build-NpcCombat','Deploy-NpcCombat'):
    p=W/f'{stem}-v973.ps1';assert not p.exists();p.write_text((W/f'{stem}-v966.ps1').read_text(encoding='utf-8-sig').replace('966','973'),encoding='utf-8')
p=W/'stage_npc_combat_v973.py';assert not p.exists();s=(W/'stage_npc_combat_v966.py').read_text(encoding='utf-8-sig').replace('v966','v973').replace("P=W/'candidate-npc-combat-v954'","P=W/'candidate-npc-combat-v966'")
a=s.index("scope='");b=s.index("')\n",a)
s=s[:a]+"scope='Extends966 with opt-in host NPC perception across negotiated stream identities. Only NPC-to-avatar per-entity relation overrides, and only native enemies of player1 without foreign override. Native API/readback and component generations; release on menu, stream retirement, death and checkpoint. No automatic damage relay or AI resume. Runtime pending; preserves966 dual native crossbow milestone972.'"+s[b+1:]
p.write_text(s,encoding='utf-8');print('Prepared candidate973 build/deploy; source changed, live966 retained')
