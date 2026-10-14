"""One-time derivation from the preserved v62 harness; inspect generated script before use."""
from pathlib import Path
R=Path(__file__).resolve().parents[2]
s=(R/'work/natural-moveset/Test-CrouchedGrenadeNetwork-v62.ps1').read_text(encoding='utf-8-sig')
def replace(a,b):
    global s
    assert a in s,a
    s=s.replace(a,b)
replace("$stood=$false;$tokens=@{}", "$tokens=@{};$idleJumped=$false;$jumped=$false;$released=$false;$jumpAt=0")
replace(" I $Source B 180;Start-Sleep -Milliseconds 650\n", "")
replace("if((Q $Source '--player-states').stance-ne1){throw 'Source crouch did not survive preparation'}", "if((Q $Source '--player-states').stance-ne0){throw 'Source standing stance did not survive preparation'}")
replace("  if(!$fired-and$watch.ElapsedMilliseconds-ge350){", """  if(!$idleJumped-and$watch.ElapsedMilliseconds-ge250){
   I $Source A 250 0 $Move;$idleJumped=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native jump with idle held grenade'})
  }
  if(!$fired-and$watch.ElapsedMilliseconds-ge1700){""")
replace("input='Crouched LT held for 1300ms then native release'", "input='Cook grenade after idle jump landing'")
replace("  $m=S $peer '.movement.json';$prop=S $peer '.grenade-prop.json'\n", """  if($fired-and!$jumped-and$watch.ElapsedMilliseconds-ge2300){
   I $Source A 350 255 $Move;$jumped=$true;$jumpAt=$watch.ElapsedMilliseconds
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native second jump while cooking'})
  }
  if($jumped-and!$released-and$watch.ElapsedMilliseconds-ge($jumpAt+350)){
   I $Source Neutral 100 0 $Move;$released=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native trigger release during jump/fall'})
  }
  $m=S $peer '.movement.json';$prop=S $peer '.grenade-prop.json'
""")
replace(";fixture=(S $Source '.grenade-fixture.json')})", ";player=$(if($watch.ElapsedMilliseconds-lt4500){Q $Source '--player-states'}else{$null});fixture=(S $Source '.grenade-fixture.json')})")
start=s.index("  if(!$stood-and$watch.ElapsedMilliseconds-gt6200){")
end=s.index("  Start-Sleep -Milliseconds 45",start)
s=s[:start]+s[end:]
start=s.index(' foreach($v in $expected){if(!@($rows|Where-Object {$_.movement.grenadeVariant-eq$v-and$_.movement.grenadeWeight-gt.75-and$_.movement.stanceMix-gt.95}')
end=s.index(" C $Source 'coop_grenade_fixture off';Start-Sleep",start)
s=s[:start]+""" if(!$idleJumped-or!$jumped-or!$released){throw 'Air input sequence incomplete'}
 if(!@($rows|Where-Object {$_.ms-lt1700-and$_.movement.airWeight-gt.75-and$_.movement.grenadeVariant-eq0-and$_.movement.grenadeWeight-gt.75-and$_.prop.active}).Count){throw 'Idle airborne grenade hold missing'}
 $checks.Add('Native idle jump retains the held grenade and existing airborne body motion')
 foreach($v in @(4,$release)){
  if(!@($rows|Where-Object {$_.movement.grenadeVariant-eq$v-and$_.movement.grenadeWeight-gt.75-and$_.movement.airWeight-gt.6}).Count){throw "Airborne remote grenade phase $v missing"}
 }
 foreach($id in @(2,3)){if(!@($rows|Where-Object {@($_.player.active|Where-Object {$_.id-eq$id}).Count-and@($_.player.active|Where-Object {$_.id-eq62}).Count}).Count){throw "Native airborne throw state $id missing"}}
 $checks.Add('Exact native jump and fall overlap cooking/release; receiver combines grenade gesture with airborne body motion')
 if(!@($rows|Where-Object {$_.ms-gt6500-and$_.movement.active-and$_.movement.airWeight-lt.01-and$_.movement.grenadeWeight-lt.01}).Count){throw 'Landing did not recover ordinary movement'}
 foreach($row in $rows){foreach($r in 'host','client'){$ss=if($r-eq$Source){$row.sourceStream}else{$row.stream};if(!$ss.ready-or!$ss.armed-or$ss.token-ne$tokens[$r].token-or$ss.peerToken-ne$tokens[$r].peerToken){throw 'Airborne action retired the Steam stream'}}}
 $checks.Add('Landing and empty recovery finish while both Steam streams retain the same tokens')
"""+s[end:]
replace("stance='crouched'", "stance='airborne'")
replace("scope='Crouched native grenade", "scope='Airborne native grenade")
replace('PASS_CROUCHED_GRENADE_NETWORK','PASS_AIRBORNE_GRENADE_NETWORK')
(R/'work/natural-moveset/Test-GrenadeAirNetwork-v63.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';t=runner.read_text(encoding='utf-8-sig')
t=t.replace("'CrouchedGrenadeNetwork','StunNetwork'", "'CrouchedGrenadeNetwork','GrenadeAirNetwork','StunNetwork'")
t=t.replace("  'StunNetwork'{", "  'GrenadeAirNetwork'{& \"$root/work/natural-moveset/Test-GrenadeAirNetwork-v63.ps1\" -Source $Source -Take $Take -Pitch $Pitch -ValidateReader -ValidateMechanism -ValidateFx -FrontView -Move $Move}\n  'StunNetwork'{")
runner.write_text(t,encoding='utf-8')
print('Prepared airborne grenade network harness')
