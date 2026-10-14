from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoNetwork-v193.ps1').read_text(encoding='utf-8-sig')
a=s.index('    $catalog=Q client');b=s.index('\n   I client 255 1800;',a);aim=s[a:b]
s=s[:a]+s[b:]
a=' Copy-Item "$PSScriptRoot/domino-banks-v192/emily-domino-bank.bin"';assert s.count(a)==1
s=s.replace(a,aim+'''\n Start-Sleep -Milliseconds 750
 $grounded=Q client '--player-states';$grounded|ConvertTo-Json -Depth 8|Set-Content "$out/source-grounded-after-aim.json"
 if(!$grounded.motionValid-or$grounded.motion-ne0-or[Math]::Abs([double]$grounded.position[2]-66.4370575)-gt.03){throw 'Source aim placement has not returned to proven native floor'}
'''+a)
a=" $checks.Add('Four native subscene transitions within the owned cast produce corresponding distinct remote phase events through native Steam')";assert s.count(a)==1
s=s.replace(a,a+'''
 $continuity=@($rows|Where-Object {$_.ms-ge900-and$_.ms-le5500})
 if($continuity.Count-lt30-or@($continuity|Where-Object {$_.movement.weight-lt.95-or$_.movement.clockRejection.count-ne0}).Count){throw 'Domino body root continuity failed during native cast'}
 [ordered]@{samples=$continuity.Count;minimumBodyWeight=($continuity|ForEach-Object {$_.movement.weight}|Measure-Object -Minimum).Minimum;rejectionCounts=@($continuity|ForEach-Object {$_.movement.clockRejection.count}|Sort-Object -Unique)}|ConvertTo-Json|Set-Content "$out/continuity.json"
 $checks.Add('Native cast keeps full root/body presentation without clock rejections; aiming placement settled before stream activation')
''')
(P/'Test-DominoNetwork-v196.ps1').write_text(s,encoding='utf-8')
f=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=f.read_text(encoding='utf-8-sig');assert s.count('Test-DominoNetwork-v193.ps1')==1;s=s.replace('Test-DominoNetwork-v193.ps1','Test-DominoNetwork-v196.ps1');f.write_text(s,encoding='utf-8')
print('Domino aiming now settles before network presentation')
