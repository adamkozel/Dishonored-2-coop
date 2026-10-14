from pathlib import Path
W=Path(__file__).resolve().parent;N=W.parent/'natural-moveset'
s=(N/'Test-DoppelgangerFx-v469.ps1').read_text(encoding='utf-8-sig')
s=s.replace("C host 'setviewpos -47.39 -156.87 68.1 90 0'", "C host 'setviewpos -45 -116 62.7 240 0'")
# The existing host pitch10 is retained. One stationary camera placement occurs
# before streaming starts; no teleport follows the clone during a take.
(N/'Test-DoppelgangerFx-v470.ps1').write_text(s,encoding='utf-8')
s=(W/'Run-DoppelgangerFx-v455.ps1').read_text(encoding='utf-8-sig')
s=s.replace('v455','v470').replace('v456','v471').replace('v454','v470').replace('Reuse454','Reuse470')
(W/'Run-DoppelgangerFx-v470.ps1').write_text(s,encoding='utf-8')
