"""Repeat the observed native cancellation in the observed crouched context."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeCancel-v122.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a));s=s.replace(a,b)
s=s.replace('mesmerize-banks-v77/emily-mesmerize-bank.bin','mesmerize-context-banks-v132/emily-mesmerize-context-bank.bin').replace('$m.version-eq45','$m.version-eq46').replace('$m.version-ne45','$m.version-ne46').replace('bank45/276','bank46/285')
c=(P/'Test-MesmerizeCrouchNetwork-v133.ps1').read_text(encoding='utf-8-sig')
a=c.index(' & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null')
b=c.index(' $hwnd=',a)
at=' $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();'
rep(at,c[a:b]+at)
at=" $checks.Add('Native X while LT remains held cancels the aim; remote body returns to its normal movement bank without stale power weight')"
rep(at,at+r'''
 $aimRows=@($rows|Where-Object {$_.movement.powerVariant-in16,17-and$_.movement.powerWeight-gt.5})
 $recovered=@($rows|Where-Object {$_.ms-gt6500-and$_.movement.powerWeight-lt.02})
 if(!$aimRows.Count-or!$recovered.Count-or@($rows|Where-Object {$_.player.stance-ne1-or$_.movement.stanceMix-lt.98}).Count){throw 'Native or remote crouch changed during aim/cancel/recovery'}
 [ordered]@{aimSamples=$aimRows.Count;recoverySamples=$recovered.Count;allCrouched=$true}|ConvertTo-Json|Set-Content "$out/crouch-context-summary.json"
 $checks.Add('Native and remote remain crouched through aim, cancellation and return to the normal crouched movement bank')''')
rep('Cancelled aim did not recover the standing movement bank','Cancelled aim did not recover the crouched movement bank')
rep('Standing armed Emily Mesmerize native aim cancellation','Crouched armed Emily Mesmerize native aim cancellation')
rep('Visual acceptance, other stances, natural lifetime and shared NPC outcomes remain separate.','Visual acceptance and shared NPC outcomes remain incomplete; standing cancellation and natural lifetime have separate receipts.')
(P/'Test-MesmerizeCrouchCancel-v135.ps1').write_text(s,encoding='utf-8')
