from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionFxLifecycle-v357.ps1').read_text().replace('Where-Object ms -ge500','Where-Object ms -ge 500')
a=' & "$lab/Send-D2Input.ps1" -Role host -Button X -LeftTrigger 255 -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 250;I host'
assert s.count(a)==1
s=s.replace(a,' if($Case-ne\'cancel\'){'+a.strip()+'}')
a=" if(!$observerFound){throw 'No independently verified close observer floor'}"
assert a in s
s=s.replace(a,a+'''
 # Keep the observer in the corridor interior rather than beside a bookshelf pillar.
 # Based on viewed356 previews; floor is independently revalidated below.
 $observerX=if([double]$target.position[0]-lt-44){-46.4503}else{-40.5}
 $observerY=[Math]::Max(-155.449,[Math]::Min(-145.0,[double]$target.position[1]-3.0))
''')
(P/'Test-PossessionFxLifecycle-v358.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert s.count('Test-PossessionFxLifecycle-v357.ps1')==2;s=s.replace('Test-PossessionFxLifecycle-v357.ps1','Test-PossessionFxLifecycle-v358.ps1');p.write_text(s)
print('358 fixes PowerShell settled-row filter; no extra cancellation input; camera uses viewed corridor position and revalidates floor')
