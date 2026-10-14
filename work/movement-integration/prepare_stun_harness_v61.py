from pathlib import Path
r=Path(__file__).resolve().parents[2]
p=r/'work/natural-moveset/Test-GrenadeNetwork-v56.ps1'
s=p.read_text(encoding='utf-8-sig')
def replace(a,b):
    global s
    assert a in s,a
    s=s.replace(a,b)
replace("$initialDll=", "$moved=$false;$streamBefore=@{}\n$initialDll=")
replace("$lt=0){", "$lt=0,$ly=0){")
replace("-LeftTrigger $lt|", "-LeftTrigger $lt -LeftY $ly|")
replace('grenade-bank-manifest-v56.json','stun-bank-manifest-v61.json')
replace('version-eq42','version-eq43')
replace('version-ne42','version-ne43')
replace(" $prop|ConvertTo-Json|Set-Content", " foreach($r in 'host','client'){$streamBefore[$r]=S $r '.shadow-stream.json';if(!$streamBefore[$r].ready){throw 'Initial stream not ready'}}\n $streamBefore|ConvertTo-Json -Depth 10|Set-Content \"$out/stream-before.json\"\n $prop|ConvertTo-Json|Set-Content")
replace("  Start-Sleep -Milliseconds 45\n }\n if($monitor)", """  if(!$moved-and$watch.ElapsedMilliseconds-gt6200){
   I $Source Neutral 900 0 -12000;$moved=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native backward movement after grenade stun without restarting stream'})
  }
  Start-Sleep -Milliseconds 45
 }
 if($monitor)""")
replace(" C $Source 'coop_grenade_fixture off';Start-Sleep", """ $nativeStun=@($samples|Where-Object {$_.stun.valid})
 if(!$nativeStun.Count){throw 'Exact owned native stun scene not observed'}
 foreach($v in @($nativeStun.stun.variant|Select-Object -Unique)){
  $reaction=@($rows|Where-Object {$_.movement.active-and$_.movement.stunVariant-eq$v-and$_.movement.stunWeight-gt.75})
  if(!$reaction.Count){throw "Remote stun variant $v absent"}
  if(@($reaction.movement.stunAmount|Select-Object -Unique).Count-lt2){throw 'Stun source clock did not advance'}
 }
 $checks.Add('Owned native stun scene advances a directional whole-body remote reaction over Steam')
 foreach($row in $rows){foreach($r in 'host','client'){
  $ss=if($r-eq$Source){$row.sourceStream}else{$row.stream}
  if(!$ss.ready-or!$ss.armed-or$ss.token-ne$streamBefore[$r].token-or$ss.peerToken-ne$streamBefore[$r].peerToken){throw 'Stun interrupted stream or replaced session tokens'}
 }}
 $after=@($rows|Where-Object {$_.ms-gt6000})
 if(!$after.Count-or@($after|Where-Object {!$_.movement.active-or$_.movement.stunWeight-gt.01}).Count){throw 'Stun body did not settle into active ordinary movement'}
 if(!$moved-or!@($after|Where-Object {$_.movement.speed-gt.2}).Count){throw 'Post-stun ordinary movement did not advance the remote gait'}
 $checks.Add('Both Steam streams remain ready with unchanged tokens throughout stun; native movement works afterward')
 if((Q $peer '--health' 1).current-ne56-or(Q $peer '--health' (S $peer).probeActorHandle).current-ne40){throw 'Remote gameplay health changed during cosmetic stun'}
 $checks.Add('Remote local player and cosmetic avatar retain their health; reaction executes no damage events')
 C $Source 'coop_grenade_fixture off';Start-Sleep""")
replace("scope='Standing armed native grenade body/held-prop presentation over Steam.","scope='Native standing grenade self-stun reaction and uninterrupted source-clock movement over Steam. Directional native NPC body clips are cosmetic and require visual review.")
replace('PASS_GRENADE_NETWORK','PASS_STUN_NETWORK')
(r/'work/natural-moveset/Test-StunNetwork-v61.ps1').write_text(s,encoding='utf-8')
runner=r/'work/movement-integration/Invoke-NativeLabOperation.ps1'
t=runner.read_text(encoding='utf-8-sig')
t=t.replace("'GrenadeNetwork','GrenadeResume'", "'GrenadeNetwork','StunNetwork','GrenadeResume'")
t=t.replace("  'GrenadeResume'{", "  'StunNetwork'{& \"$root/work/natural-moveset/Test-StunNetwork-v61.ps1\" -Source $Source -Take $Take -Pitch $Pitch -ValidateReader -ValidateMechanism -ValidateFx -Closeup}\n  'GrenadeResume'{")
runner.write_text(t,encoding='utf-8')
print('Prepared isolated stun network harness and serialized operation')
