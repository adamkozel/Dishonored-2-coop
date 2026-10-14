"""Derive the isolated native interruption test from the verified beam test."""
from pathlib import Path
R=Path(__file__).resolve().parents[2]; P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeLinks-v106.ps1').read_text(encoding='utf-8-sig')
old=(P/'Test-MesmerizeTargets-v90.ps1').read_text(encoding='utf-8-sig')
def replace(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a))
 s=s.replace(a,b)
replace('[int]$Pitch=12)',"[int]$Pitch=12,[ValidateSet('receiver-stop','source-pause','receiver-pause')][string]$Interrupt='receiver-pause')")
replace('$dirty=$false;$failure=$null','$dirty=$false;$failure=$null;$pausedRole=$null')
replace('$worldFramed=$false','$worldFramed=$false;$interrupted=$false')
block=old[old.index('  if(!$interrupted-and$w.ElapsedMilliseconds-ge10000){'):old.index("  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'",old.index('  if(!$interrupted-and$w.ElapsedMilliseconds-ge10000){'))]
block=block.replace("   $events.Add", "   $activeLinks=S host '.mesmerize-links.json';if(!@($activeLinks.links|Where-Object kind -eq 1).Count-or!@($activeLinks.links|Where-Object kind -eq 3).Count){throw 'Beam and target head not active at interruption'}\n   $events.Add")
replace("  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'",block+"  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'")
replace('if(!$m.active-or!$stream.ready-or!$stream.armed){throw', 'if(!$interrupted-and(!$m.active-or!$stream.ready-or!$stream.armed)){throw')
replace("if($m.powerWeight-gt.02-or$m.weight-lt.9-or$stream.remoteState-ne0){throw 'Remote cast did not recover to standing'}", "if(!$interrupted){throw 'Interruption stimulus not applied'}\n if($m.powerWeight-gt.02){throw 'Interrupted remote cast retained weight'}")
start=s.index(" if($visual.refused-ne$visualBefore.refused")
end=s.index(" if($world.rejected-le$worldBefore.rejected)",start)
replacement=old[old.index(" if($visual.refused-ne$visualBefore.refused"):old.index(" if($world.rejected-le$worldBefore.rejected)")]
s=s[:start]+replacement+s[end:]
replace(" $checks.Add('Native committed world incarnation and transform drive one cosmetic model, all three scene phases and one owned body effect through native Steam; source retirement hides the model and requests native effect stop; the native particle tail then leaves the manager')", " $checks.Add('Native menu or stream interruption retires the cosmetic model and body effect; native tails finish without stale revival')")
replace(" $checks.Add('Remote body recovers, original sword selections and all avatar health retained; remote model/effect has retired')", " $checks.Add('Interrupted cast releases body weight; original sword selections and avatar health retained')")
replace(" $links=S host '.mesmerize-links.json';$links|ConvertTo-Json", " if(@($rows|Where-Object {$_.ms-gt15000-and(@($_.eyes.eyes).Count-or@($_.links.links).Count)}).Count){throw 'Old eye or target link revived after interruption'}\n $checks.Add('All source-referenced eye and target effects retire after interruption without stale revival')\n $links=S host '.mesmerize-links.json';$links|ConvertTo-Json")
replace("}catch{$failure=$_.Exception.Message}finally{", "}catch{$failure=$_.Exception.Message}finally{\n if($pausedRole){try{& \"$lab/Send-D2Input.ps1\" -Role $pausedRole -Button A -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 600}catch{}}")
replace("source='client';checks=$checks", "source='client';interruption=$Interrupt;checks=$checks")
replace('Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified.', 'Committed-world interruption tests finite cleanup with automatic stream resumption disabled. Same-world reacquisition, aiming cancellation and natural lifetime remain unverified.')
(P/'Test-MesmerizeLinksInterrupt-v108.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1'
t=runner.read_text(encoding='utf-8-sig')
a="'MesmerizeLinks'{& \"$root/work/natural-moveset/Test-MesmerizeLinks-v106.ps1\" -Take $Take -Pitch $Pitch}"
b="'MesmerizeLinks'{if($Interrupt-ne'none'){& \"$root/work/natural-moveset/Test-MesmerizeLinksInterrupt-v108.ps1\" -Take $Take -Pitch $Pitch -Interrupt $Interrupt}else{& \"$root/work/natural-moveset/Test-MesmerizeLinks-v106.ps1\" -Take $Take -Pitch $Pitch}}"
assert t.count(a)==1
runner.write_text(t.replace(a,b),encoding='utf-8')
print('Prepared isolated beam interruption harness and runner')
