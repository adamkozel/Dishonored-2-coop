from pathlib import Path
p=Path('work/natural-moveset/Test-MesmerizeWorldNetwork-v78.ps1');s=p.read_text().replace('setviewpos -30.7 -151.8 68.1 203 0','setviewpos -30.7 -151.8 68.1 230 0')
s=s.replace("C client 'coop_mesmerize_visual duplicate';$duplicated=$true", "Q host '--mesmerize-effects'|ConvertTo-Json -Depth 10|Set-Content \"$out/observer-effects-active.json\";C client 'coop_mesmerize_visual duplicate';$duplicated=$true")
s=s.replace(" if(@((Q host '--mesmerize-effects').effects).Count){throw 'Observer native body effect remains after retirement'}", " $remainingFx=Q host '--mesmerize-effects';$remainingFx|ConvertTo-Json -Depth 10|Set-Content \"$out/observer-effects-after-retirement.json\";if(@($remainingFx.effects).Count){throw 'Observer native body effect remains after retirement'}")
Path('work/natural-moveset/Test-MesmerizeWorldNetwork-v79.ps1').write_text(s)
p=Path('work/movement-integration/Invoke-NativeLabOperation.ps1');s=p.read_text().replace('Test-MesmerizeWorldNetwork-v78.ps1','Test-MesmerizeWorldNetwork-v79.ps1');p.write_text(s)
