from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionFxLifecycle-v358.ps1').read_text()
a=s.index(' # Keep the observer in the corridor interior');b=s.index(' $observerYaw=',a)
s=s[:a]+s[b:]
s=s.replace(" $checks.Add('Native source recovers normal mode; explicit cancellation preserves mana')"," $checks.Add($(if($Case-eq'cancel'){'Native source cancellation recovers normal mode and preserves mana'}else{'Native source returns to normal mode after interruption and cleanup input'}))")
(P/'Test-PossessionFxLifecycle-v359.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert s.count('Test-PossessionFxLifecycle-v358.ps1')==2;s=s.replace('Test-PossessionFxLifecycle-v358.ps1','Test-PossessionFxLifecycle-v359.ps1');p.write_text(s)
print('359 keeps per-take measured camera floor for lifecycle tests; no static corridor override')
