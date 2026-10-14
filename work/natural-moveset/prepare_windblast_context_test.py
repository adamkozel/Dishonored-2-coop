from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Test-WindblastBody.ps1').read_text(encoding='utf-8-sig')
s=s.replace("$Take='windblast-corvo-body-v27-v1')\n$Crouched=$false;$Holstered=$false;$Moving=$false", "$Take='windblast-corvo-context-v28-v1',[switch]$Crouched,[switch]$Holstered,[switch]$Moving)")
s=s.replace('windblast-banks-v1/$character-windblast-bank.bin','windblast-context-banks-v1/$character-windblast-context-bank.bin').replace('version-eq27','version-eq28').replace('version-ne27','version-ne28').replace('v27','v28')
s=s.replace('Standing armed Corvo Windblast body integration only.','Corvo Windblast ground stance/equipment/travel body integration.')
(P/'Test-WindblastContexts.ps1').write_text(s,encoding='utf-8')
