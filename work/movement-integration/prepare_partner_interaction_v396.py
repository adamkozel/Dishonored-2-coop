from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration'
p=W/'Build-PartnerInteraction-v396.ps1';assert not p.exists()
p.write_text((W/'Build-PartnerAssassination-v387.ps1').read_text(encoding='utf-8-sig').replace('partner-assassination','partner-interaction').replace('v387','v396').replace('Build387','Build396'),encoding='utf-8')
p=W/'archive_partner_interaction_v396.py';assert not p.exists()
s=(W/'archive_partner_assassination_v387.py').read_text(encoding='utf-8-sig').replace('candidate-partner-assassination-v387','candidate-partner-interaction-v396').replace('candidate-possession-partner-v374','candidate-partner-assassination-v387').replace('partner-assassination','partner-interaction').replace('v387.log','v396.log').replace('Build-PartnerAssassination-v387','Build-PartnerInteraction-v396')
s=s.replace('rva=0x3b00a0','rva=0x3b8080').replace("4053574883ec38488bfa488bd94885d2","84d2750cf681b800000001740333c0c3")
s=s.replace('instructions=6','instructions=6')
start=s.index("scope='Candidate387");end=s.index("')",start)
s=s[:start]+"scope='Candidate396 excludes returned exact current partner from both native interaction-sensor sword/fatality getters3b8020/3b8080 after native result and reciprocal local source/sensor generation proof. Existing387 rear-choke filter and all NPC results retained. Bank54/319, native movement collision and assets unchanged. Static tests passed; runtime pending."+s[end:]
p.write_text(s,encoding='utf-8')
print('Prepared396 build and archive scripts; no game state changed')
