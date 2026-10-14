from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab/src'
p=S/'shadow_action_protocol_tests.cpp';s=p.read_text();needle='        constexpr std::uintptr_t base=0x140000000;';assert s.count(needle)==1
s=s.replace(needle,'''        check(nativeHumanPossessionEquipment(1,false,false)&&nativeHumanPossessionEquipment(8,false,true),"native stopped transfer and restarted unselected occupied component distinguished");
        check(!nativeHumanPossessionEquipment(8,true,true)&&!nativeHumanPossessionEquipment(8,false,false)&&!nativeHumanPossessionEquipment(1,false,true),"unobserved possession equipment lifecycle refused");
'''+needle);p.write_text(s,encoding='utf-8')
s=(P/'Test-PossessionVisibilityNetwork-v337.ps1').read_text()
s=s.replace("power=(O host '--player-possession');isolation=", "power=(O host '--player-possession');playerAfter=(O host '--player-states');isolation=")
old="$occupiedRows=@($rows|Where-Object {$_.player.stance-eq18-and!$_.player.defaultMode})";assert s.count(old)==1
s=s.replace(old,"""$occupiedRows=@($rows|Where-Object {$_.player.stance-eq18-and!$_.player.defaultMode-and$_.playerAfter.stance-eq18-and!$_.playerAfter.defaultMode-and$_.power.readOnly})
 $transitionReads=@($rows|Where-Object {$_.player.stance-eq18-and(!$_.power.readOnly-or$_.playerAfter.stance-ne18)})
 [ordered]@{stableOccupied=$occupiedRows.Count;transitionReads=$transitionReads.Count;scope='Independent reads are bracketed by occupied player snapshots; invalid transition reads are unknown, never zero quantity or proof of revocation'}|ConvertTo-Json|Set-Content "$out/occupied-read-stability.json"
 if($occupiedRows.Count-lt3-or@($occupiedRows|Where-Object {!$_.visibility.hidden-or!$_.visibility.active-or!$_.stream.ready-or$_.stream.remoteState-ne17}).Count){throw 'Remote body visible or stream stale during bracketed native human occupation'}
 $checks.Add('All bracketed stable occupied-player observations retain hidden clone and fresh semantic17; transition read gaps reported separately')""")
(P/'Test-PossessionVisibilityNetwork-v338.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace('Test-PossessionVisibilityNetwork-v337.ps1','Test-PossessionVisibilityNetwork-v338.ps1');p.write_text(s,encoding='utf-8')
print('338 distinguishes native restarted occupied equipment; stronger hide continuity and bracketed quantity checks')
