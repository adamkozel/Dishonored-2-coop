from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoWorld-v209.ps1').read_text(encoding='utf-8-sig')
s=s.replace("C host 'setviewpos -44.39 -153.27 68.1 203 0'", "C host 'setviewpos -47.4 -156.3 68.1 203 0'")
s=s.replace('48.59','44.39').replace('155.07','153.27')
s=s.replace(" $before=Q client '--player-domino';", " $filterBefore=S client '.domino-partner-filter.json';$filterBefore|ConvertTo-Json|Set-Content \"$out/filter-before.json\"\n $before=Q client '--player-domino';",1)
s=s.replace(" $checks.Add('Native validated mark ID", " $filterAfter=S client '.domino-partner-filter.json';$filterAfter|ConvertTo-Json|Set-Content \"$out/filter-after.json\"\n if(!$filterAfter.installed-or$filterAfter.otherAccepted-le$filterBefore.otherAccepted-or$filterAfter.readFailed-ne$filterBefore.readFailed){throw 'Ordinary guard picker forwarding not verified'}\n $checks.Add('Native picker still forwards ordinary guard candidates with partner filter installed and no ownership read failures')\n $checks.Add('Native validated mark ID",1)
(P/'Test-DominoWorld-v215.ps1').write_text(s,encoding='utf-8')
f=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=f.read_text(encoding='utf-8-sig');assert 'Test-DominoWorld-v209.ps1' in s
f.write_text(s.replace('Test-DominoWorld-v209.ps1','Test-DominoWorld-v215.ps1'),encoding='utf-8')
