from pathlib import Path
R=Path(__file__).resolve().parents[2]
s=(R/'work/natural-moveset/Test-GrenadeNetwork-v56.ps1').read_text(encoding='utf-8-sig')
def replace(a,b):
    global s
    assert a in s,a
    s=s.replace(a,b)
replace("$Interrupt='none')", "$Interrupt='none',[ValidateRange(0,15000)][int]$Move=0)")
replace("$initialDll=", "$stood=$false;$tokens=@{}\n$initialDll=")
replace("$lt=0){", "$lt=0,$ly=0){")
replace("-LeftTrigger $lt|", "-LeftTrigger $lt -LeftY $ly|")
replace(" $held=Q $Source '--player-grenade';if(!$held.equipped-or$held.quantity-ne1){throw 'Native grenade presentation not selected'}", " $held=Q $Source '--player-grenade';if(!$held.equipped-or$held.quantity-ne1){throw 'Native grenade presentation not selected'}\n I $Source B 180;Start-Sleep -Milliseconds 650")
replace('grenade-bank-manifest-v56.json','crouched-grenade-bank-manifest-v62.json')
replace('version-eq42','version-eq44');replace('version-ne42','version-ne44')
replace(" $checks.Add('One native grenade", " if((Q $Source '--player-states').stance-ne1){throw 'Source crouch did not survive preparation'}\n $checks.Add('One native grenade")
replace(" $prop|ConvertTo-Json|Set-Content", " foreach($r in 'host','client'){$tokens[$r]=S $r '.shadow-stream.json';if(!$tokens[$r].ready){throw 'Initial stream not ready'}}\n $tokens|ConvertTo-Json -Depth 10|Set-Content \"$out/stream-before.json\"\n $prop|ConvertTo-Json|Set-Content")
replace("I $Source Neutral 1300 255", "I $Source Neutral 1300 255 $Move")
replace("input='LT held for 1300ms then native release';pitch=$Pitch;actualPitch=$actualPitch", "input='Crouched LT held for 1300ms then native release';leftY=$Move;pitch=$Pitch;actualPitch=$actualPitch")
replace("  Start-Sleep -Milliseconds 45\n }\n if($monitor)", """  if(!$stood-and$watch.ElapsedMilliseconds-gt6200){
   I $Source B 180;$stood=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native stand after crouched throw'})
  }
  Start-Sleep -Milliseconds 45
 }
 if($monitor)""")
replace(" C $Source 'coop_grenade_fixture off';Start-Sleep", """ foreach($v in $expected){if(!@($rows|Where-Object {$_.movement.grenadeVariant-eq$v-and$_.movement.grenadeWeight-gt.75-and$_.movement.stanceMix-gt.95}).Count){throw "Remote crouched phase $v absent"}}
 if($Move-and!@($rows|Where-Object {$_.movement.grenadeWeight-gt.75-and$_.movement.speed-gt.15-and$_.movement.stanceMix-gt.95}).Count){throw 'Native crouched travel did not advance remote gait'}
 $checks.Add('All observed cooking/release/recovery phases retain complete crouched body; requested travel advances remote gait')
 foreach($row in $rows){foreach($r in 'host','client'){$ss=if($r-eq$Source){$row.sourceStream}else{$row.stream};if(!$ss.ready-or!$ss.armed-or$ss.token-ne$tokens[$r].token-or$ss.peerToken-ne$tokens[$r].peerToken){throw 'Crouched action retired the Steam stream'}}}
 if(!$stood-or(Q $Source '--player-states').stance-ne0-or!@($rows|Where-Object {$_.ms-gt7500-and$_.movement.active-and$_.movement.stanceMix-lt.01-and$_.movement.grenadeWeight-lt.01}).Count){throw 'Native stand did not recover ordinary remote posture'}
 $checks.Add('Native standing recovery works while both Steam streams retain the same tokens')
 C $Source 'coop_grenade_fixture off';Start-Sleep""")
replace("pitch=$Pitch;interruption=$Interrupt;checks=", "pitch=$Pitch;stance='crouched';leftY=$Move;interruption=$Interrupt;checks=")
replace("scope='Standing armed native grenade", "scope='Crouched native grenade")
replace('PASS_GRENADE_NETWORK','PASS_CROUCHED_GRENADE_NETWORK')
(R/'work/natural-moveset/Test-CrouchedGrenadeNetwork-v62.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';t=runner.read_text(encoding='utf-8-sig')
t=t.replace("'GrenadeNetwork','StunNetwork'", "'GrenadeNetwork','CrouchedGrenadeNetwork','StunNetwork'")
t=t.replace("  'StunNetwork'{", "  'CrouchedGrenadeNetwork'{& \"$root/work/natural-moveset/Test-CrouchedGrenadeNetwork-v62.ps1\" -Source $Source -Take $Take -Pitch $Pitch -ValidateReader -ValidateMechanism -ValidateFx -FrontView -Move $Move}\n  'StunNetwork'{")
runner.write_text(t,encoding='utf-8');print('Prepared isolated crouched grenade network harness')
