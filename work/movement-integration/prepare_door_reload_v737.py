from pathlib import Path
R=Path(__file__).resolve().parents[2]
s=(R/'work/natural-moveset/Test-DoorReload-v736.ps1').read_text(encoding='utf-8-sig')
old='   Start-Sleep -Milliseconds 800\n   $final=@{}'
assert old in s
s=s.replace(old,"""   $avatarWatch=[Diagnostics.Stopwatch]::StartNew();$avatarReady=$false
   do{
    $pair=@{};$avatarReady=$true
    foreach($r in 'host','client'){
     $reset=RS $r;$bridge=S $r;$pair[$r]=[pscustomobject]@{reset=$reset;bridge=$bridge;doors=(DAll $r)}
     if(!$reset.avatarReady-or$reset.avatarPending-or!$bridge.probeFollowEnabled){$avatarReady=$false}
     if($reset.avatarRefused-ne$resetBefore[$r].avatarRefused){throw 'Counterpart restore refused'}
    }
    $samples.Add([pscustomobject]@{ms=$watch.ElapsedMilliseconds;host=$pair.host;client=$pair.client})
    if(!$avatarReady){Start-Sleep -Milliseconds 80}
   }while(!$avatarReady-and$avatarWatch.ElapsedMilliseconds-lt6000)
   if(!$avatarReady){throw 'Both counterpart-ready signals did not arrive'}
   $final=@{}""")
old='$final[$r]=[pscustomobject]@{reset=$reset;bridge=$bridge;doors=$doors;character=$character}'
assert old in s
s=s.replace(old,old+"""
    $final[$r]|ConvertTo-Json -Depth 14|Set-Content "$out/reset-after-native-$r.json"
    $avatars=@($entities.entities|Where-Object name -match '^d2coop_probe_')
    if($avatars.Count-ne1-or$avatars[0].name-ne$(if($r-eq'host'){'d2coop_probe_emily'}else{'d2coop_probe_corvo'})-or$avatars[0].handle-ne('0x'+([uint32]$bridge.probeActorHandle).ToString('x'))-or$reset.avatarRestored-ne$resetBefore[$r].avatarRestored+1){throw 'Expected exactly one restored following counterpart'}""")
s=s.replace('NativeDoorReload736','NativeDoorReload737').replace('native-door-reload-host-v736','native-door-reload-host-v737')
(R/'work/natural-moveset/Test-DoorReload-v737.ps1').write_text(s,encoding='utf-8')
print('Created readiness-based native reload harness737')
