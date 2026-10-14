from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Test-NativePowerFx.ps1').read_text(encoding='utf-8-sig')
s=s.replace("$Take='native-blink-fx-corvo-v1'","$Take='native-windblast-fx-corvo-v1'").replace("[ValidateSet('blink','grapple')][string]$Family='blink'","[ValidateSet('windblast')][string]$Family='windblast'")
a=s.index("$first=if(");b=s.index('\nfunction C',a)
s=s[:a]+"$first='windblast_probe';$second='windblast_ground_probe';$kinds=@('6','7')"+s[b:]
s=s.replace("Native effects/lifecycle mismatch", "Native Windblast effects/lifecycle mismatch")
s=s.replace("Isolated native particle/sound API feasibility. Manually triggered test effects, not network-bound power outcomes.","Isolated Windblast screen-particle/viewer scoping and ground-dust feasibility. Manual finite native visual effects, without casting or gameplay outcomes. Screen distortion and visible world effects require visual review before any network integration.")
(P/'Test-WindblastFxProbe.ps1').write_text(s,encoding='utf-8')
