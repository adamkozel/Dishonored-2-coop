"""Reduce redundant research probes during short runtime combat transitions."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-CombatTarget-v612.ps1').read_text().replace('CombatTarget612','CombatTarget614')
line='    $objects=@(foreach($c in $sensors){[pscustomobject]@{type=$c.type;pointer=$c.pointer;object=(Q $source \'--object\' $c.pointer)}})\n'
assert line in s;s=s.replace(line,'')
a="sensors=$objects;midnight=(Q $source '--midnight');";assert a in s;s=s.replace(a,'')
a=';sourceStream=(Get-Content "$root/work/lab-instances/$source/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json)';assert a in s;s=s.replace(a,'')
s=s.replace('Start-Sleep -Milliseconds 80','Start-Sleep -Milliseconds 30')
(N/'Test-CombatTarget-v614.ps1').write_text(s)
print('Prepared614: same assertions and gameplay input; independent scene monitor remains. Removed five unused sensor subprocesses and duplicate midnight reads per sample.')
