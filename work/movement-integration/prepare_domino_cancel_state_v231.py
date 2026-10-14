from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoNativeCancel-v230.ps1').read_text(encoding='utf-8-sig')
assert 'Where-Object id -eq72' in s
s=s.replace('Where-Object id -eq72','Where-Object id -eq 72')
(P/'Test-DominoNativeCancel-v231.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig').replace('Test-DominoNativeCancel-v230.ps1','Test-DominoNativeCancel-v231.ps1');runner.write_text(s,encoding='utf-8')
