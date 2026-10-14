from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoLinks-v223.ps1').read_text(encoding='utf-8-sig')
assert '$observerX=$sourceX+3.5;$observerY=$sourceY-1' in s
s=s.replace('$observerX=$sourceX+3.5;$observerY=$sourceY-1','$observerX=$sourceX-.4;$observerY=$sourceY-.8')
s=s.replace('-Role host -Pitch -8 -Evidence', '-Role host -Pitch 0 -Evidence')
(P/'Test-DominoLinks-v225.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1'
s=runner.read_text(encoding='utf-8-sig');assert 'Test-DominoLinks-v223.ps1' in s
runner.write_text(s.replace('Test-DominoLinks-v223.ps1','Test-DominoLinks-v225.ps1'),encoding='utf-8')
