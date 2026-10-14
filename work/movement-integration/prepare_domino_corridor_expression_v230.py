from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoNativeCancel-v229.ps1').read_text(encoding='utf-8-sig')
old='$anchor=@([double]$initialTargets[0].position[0],[double]$initialTargets[0].position[1]-2.5,[double]$initialTargets[0].position[2])'
new='$anchor=@(([double]$initialTargets[0].position[0]),([double]$initialTargets[0].position[1]-2.5),([double]$initialTargets[0].position[2]))'
assert old in s;s=s.replace(old,new)
(P/'Test-DominoNativeCancel-v230.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig').replace('Test-DominoNativeCancel-v229.ps1','Test-DominoNativeCancel-v230.ps1');runner.write_text(s,encoding='utf-8')
