from pathlib import Path
P=Path(__file__).resolve().parent;s=(P/'Test-OutOfMana.ps1').read_text();target=P/'Test-OutOfManaContexts.ps1';assert not target.exists()
def swap(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a));s=s.replace(a,b)
swap("[switch]$FrontView,","[switch]$FrontView,[switch]$Crouched,[switch]$Holstered,")
swap('$Crouched=$false;$Holstered=$false;','')
swap('outofmana-bank-manifest-v45.json','outofmana-context-bank-manifest-v46.json')
swap('$m.version-eq40-and$m.weight-gt.9-and$m.presentedDrawn','$m.version-eq41-and$m.weight-gt.9-and$m.presentedDrawn-ne[bool]$Holstered-and[Math]::Abs($m.stanceMix-[int][bool]$Crouched)-lt.02')
swap("if(!$m.active-or$m.version-ne40-or$m.weight-lt.9-or!$m.presentedDrawn){throw 'Native v40 armed body not ready'}", "if(!$m.active-or$m.version-ne41-or$m.weight-lt.9-or$m.presentedDrawn-eq[bool]$Holstered-or[Math]::Abs($m.stanceMix-[int][bool]$Crouched)-ge.02){throw 'Native v41 stance/equipment body not ready'}")
swap('Exact v40 bank active','Exact v41 stance/equipment bank active')
swap("if(!$sword.equipped-or!(Q host '--player-sword').equipped){throw 'Failed cast changed sword state'}", "if($sword.equipped-eq[bool]$Holstered-or(Q host '--player-sword').equipped-eq[bool]$Holstered){throw 'Failed cast changed sword state'}\n if(@($rows|Where-Object {$_.player.stance-ne[int][bool]$Crouched-or[Math]::Abs($_.movement.stanceMix-[int][bool]$Crouched)-gt.05}).Count){throw 'Failed cast changed native or remote stance'}\n $checks.Add('Every observed frame retains native and remote stance through failure and recovery')")
swap('Failure did not recover to normal native standing','Failure did not recover to original native stance')
swap('Remote failure recovers to standing with original sword','Remote failure recovers to original stance with original sword')
swap('Corvo standing armed Bend Time failure response from exact native scene ownership. No generic failure, crouch/air/slide, other powers or Emily binding claimed.','Corvo Bend Time standing/crouched and drawn/holstered failure response from exact native scene ownership; only the context named in this take is validated. No air/slide, other powers or Emily binding claimed.')
target.write_text(s);print(target)
