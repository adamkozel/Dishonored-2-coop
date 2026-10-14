from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Test-NativeGrenadeEquip-v52.ps1').read_text(encoding='utf-8-sig')
s=s.replace('"$lab/bin/d2_runtime_probe.exe"','"$root/work/build/Release/d2_runtime_probe.exe"')
s=s.replace("$before=Q $Source '--player-grenade';", "if((Q $Source '--player-grenade-scene').valid){throw 'Unexpected baseline scene'}\n $before=Q $Source '--player-grenade';")
s=s.replace("$midnight=Q $Source '--midnight';", "$ownedScene=Q $Source '--player-grenade-scene';$midnight=Q $Source '--midnight';")
s=s.replace('midnight=$midnight;item=$item;', 'ownedScene=$ownedScene;midnight=$midnight;item=$item;')
s=s.replace("if(!$equipped){", "foreach($v in 1,2){if(@($rows|Where-Object {$_.ownedScene.valid-and$_.ownedScene.variant-eq$v}).Count-lt1){throw \"Owned scene variant $v not observed\"}};$checks.Add('Read-only exact owned arm/item scene reader observes both native equip variants')\n if(!$equipped){")
s=s.replace("$after=Q $Source '--player-grenade';", "if((Q $Source '--player-grenade-scene').valid){throw 'Scene reader did not clear'};$checks.Add('Scene reader refuses baseline and clears after native rollback')\n $after=Q $Source '--player-grenade';")
s=s.replace('probe=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash', 'probe=(Get-FileHash "$root/work/build/Release/d2_runtime_probe.exe").Hash')
(P/'Test-NativeGrenadeScene-v53.ps1').write_text(s,encoding='utf-8')
print('Prepared separate v53 read-only scene verification harness')
