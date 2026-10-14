from pathlib import Path
W=Path(__file__).resolve().parent
p=W/'archive_doppelganger_air_v512.py';assert not p.exists()
s=(W/'archive_doppelganger_air_v508.py').read_text(encoding='utf-8-sig')
s=s.replace("A = W / 'candidate-doppelganger-air-v508'", "A = W / 'candidate-doppelganger-air-v512'")
s=s.replace("previous = W / 'candidate-doppelganger-holstered-v495'", "previous = W / 'candidate-doppelganger-air-v508'")
s=s.replace('-v508.log','-v512.log').replace('Air-v508.ps1','Air-v512.ps1').replace("'doppelganger-air-v508'","'doppelganger-air-v512'").replace('DoppelgangerAirDeploy508','DoppelgangerAirDeploy512')
a=s.index('                scope=');b=s.index('\n                banks=',a)
s=s[:a]+"                scope='Diagnostic512 freezes at most32 movement inputs and16 root samples on the first clock rejection.509 failed landing continuity; no guard relaxation or gameplay changes. Native airborne extension508 remains experimental until corrected runtime checks pass.',"+s[b:]
p.write_text(s,encoding='utf-8')
