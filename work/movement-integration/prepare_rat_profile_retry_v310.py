from pathlib import Path
import json,hashlib
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
original=P/'Test-SwarmRatVisualNetwork-v309.ps1';s=original.read_text(encoding='utf-8-sig');assert s.count('Where-Object activeModels -gt0')==2
s=s.replace('Where-Object activeModels -gt0','Where-Object {$_.activeModels -gt 0}')
(P/'Test-SwarmRatVisualNetwork-v310.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v309.ps1' in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v309.ps1','Test-SwarmRatVisualNetwork-v310.ps1'))
d={'dllUnchanged':'7BFF5AB49C2DDB38E4F9278018AE4485B98E7EED7202F7B07C1F713161D2E23D','failure':'309 quiet active25-rat window completed; simplified Where-Object activeModels -gt0 was parsed as nonexistent cmdlet parameter. No gameplay failure inferred. Native checkpoint/protected14 cleanup passes.','correction':'Use a scriptblock comparison for both active and hidden guards; retain all checks and candidate309 binary.'}
(W/'rat-profile-harness-correction-v310.json').write_text(json.dumps(d,indent=2))
