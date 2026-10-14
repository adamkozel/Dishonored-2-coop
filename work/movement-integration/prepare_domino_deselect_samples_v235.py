from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoNativeDeselect-v234.ps1').read_text(encoding='utf-8-sig')
old='while($w.ElapsedMilliseconds-lt18000)'
assert old in s;s=s.replace(old,'while($w.ElapsedMilliseconds-lt18000-or($networkStarted-and$peerRows.Count-lt12-and$w.ElapsedMilliseconds-lt23000))')
(P/'Test-DominoNativeDeselect-v235.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig').replace('Test-DominoNativeDeselect-v234.ps1','Test-DominoNativeDeselect-v235.ps1');runner.write_text(s,encoding='utf-8')
