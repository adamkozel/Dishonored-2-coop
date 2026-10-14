from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Test-WindblastFxProbe.ps1').read_text(encoding='utf-8-sig')
s=s.replace("$Take='native-windblast-fx-corvo-v1'","$Take='remote-windblast-fx-corvo-v1'")
s=s.replace("$first='windblast_probe'","$first='windblast_remote_probe'").replace("$kinds=@('6','7')","$kinds=@('8','7')")
s=s.replace('Isolated Windblast screen-particle/viewer scoping and ground-dust feasibility.','Isolated world-space reuse of native Windblast particles and sound, plus native ground-dust feasibility. Dedicated effect declaration omits first-person fullscreen distortion/rumble.')
(P/'Test-WindblastRemoteFx.ps1').write_text(s,encoding='utf-8')
