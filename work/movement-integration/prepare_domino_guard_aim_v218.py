from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoWorld-v216.ps1').read_text(encoding='utf-8-sig')
s=s.replace("$_.spawnerName-in@('ld_spawner_guard_tutorial_1','ld_spawner_guard_tutorial_2')","$_.spawnerName-eq'ld_spawner_guard_tutorial_2'")
key=' $worldBaseline=S host'
assert key in s
s=s.replace(key,''' & "$PSScriptRoot/Aim-NativeDominoGuard-v218.ps1" -Role client -Evidence "$out/native-guard-aim.json"
'''+key)
(P/'Test-DominoWorld-v218.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1'
s=runner.read_text(encoding='utf-8-sig');assert 'Test-DominoWorld-v216.ps1' in s
runner.write_text(s.replace('Test-DominoWorld-v216.ps1','Test-DominoWorld-v218.ps1'),encoding='utf-8')
