from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Test-DarkVisionContexts.ps1').read_text(encoding='utf-8-sig')
s=s.replace('darkvision-context-banks-v1/$character-darkvision-context-bank.bin','balanced-crouch-power-banks-v3/$character-balanced-crouch-power-bank.bin').replace('$m.version-eq26','$m.version-eq28').replace('$m.version-ne26','$m.version-ne28').replace('exact v26','exact v28')
(P/'Test-DarkVisionBalanced.ps1').write_text(s,encoding='utf-8-sig')
print('Created candidate Dark Vision regression harness')
