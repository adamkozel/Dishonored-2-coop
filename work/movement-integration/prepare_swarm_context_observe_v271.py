from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Observe-NativeSwarm-v265.ps1').read_text()
s=s.replace("[int]$Pitch=30)","[int]$Pitch=30,[ValidateSet('standing','crouched')][string]$Stance='standing',[switch]$Holstered)")
needle=' $hwnd=(Get-Process -Id $pids.host).MainWindowHandle.ToInt64();'
insert=''' $expectedStance=if($Stance-eq'crouched'){1}else{0}
 if($expectedStance){& "$lab/Send-D2Input.ps1" -Role host -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 500}
 if($Holstered){
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch -60 -Evidence "$out/holster-look-away.json" -Quick
  & "$lab/Send-D2Input.ps1" -Role host -Button X -DurationMs 1000|Out-Null;Start-Sleep -Milliseconds 1200
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch $Pitch -Evidence "$out/holster-look-return.json" -Quick
 }
 $context=Q host '--player-states';$sword=Q host '--player-sword'
 [ordered]@{stance=$Stance;holstered=[bool]$Holstered;native=$context;sword=$sword}|ConvertTo-Json -Depth 10|Set-Content "$out/context-before.json"
 if($context.stance-ne$expectedStance-or!$context.motionValid-or$context.motion-ne$expectedStance-or$sword.equipped-eq[bool]$Holstered){throw 'Requested native grounded equipment context not established'}
 $checks.Add('Requested native stance/sword context confirmed before input; cast may natively change stance')
'''
assert needle in s;s=s.replace(needle,insert+needle)
s=s.replace('$w.Restart();$cast=$false','$w.Restart();$cast=$false;$worldSnapshots=0')
needle='  Start-Sleep -Milliseconds 30'
insert='''  if($w.ElapsedMilliseconds-ge5000-and$worldSnapshots-lt2){
   $swarm=@($entities.entities|Where-Object {$_.definitionName-eq'models/characters/animals/rat/devouring_swarm.def'})
   if($swarm.Count-eq1){
    $components=Q host '--components' $swarm[0].handle
    $rat=@($components.components|Where-Object vtableRva -eq '0x1c42ab8')
    if($rat.Count-ne1-or$rat[0].owner-ne$swarm[0].pointer){
     # Preserve raw identities rather than authorizing a guessed ownership link.
     [ordered]@{entity=$swarm[0];components=$components;note='Raw ownership fields need inspection'}|ConvertTo-Json -Depth 12|Set-Content "$out/world-identity-$worldSnapshots.json"
    }
    if($rat.Count-eq1){
     $object=Q host '--object' $rat[0].pointer
     $array=@($object.words|Where-Object offset -eq '0x68')
     $agents=if($array.Count-eq1-and$array[0].word-ne'0x0'){Q host '--object' $array[0].word}else{$null}
     [ordered]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;entity=$swarm[0];components=$components;ratComponent=$object;agentPrefix=$agents;note='Read-only static candidate +68 pointer/+70 stride/+78 count, first1024 bytes only; no world mutation or semantic claim'}|ConvertTo-Json -Depth 16|Set-Content "$out/world-components-$worldSnapshots.json"
    }
    $worldSnapshots++
   }
  }
'''
assert needle in s;s=s.replace(needle,insert+needle)
s=s.replace("passed=(!$failure);failure=$failure;source='host';", "passed=(!$failure);failure=$failure;source='host';stance=$Stance;holstered=[bool]$Holstered;")
s=s.replace('armed native input','native input')
(N/'Observe-NativeSwarmContext-v271.ps1').write_text(s)
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=p.read_text().replace("'SwarmObserve','SwarmNetwork',","'SwarmObserve','SwarmNetwork','SwarmContextObserve',")
needle="  'SwarmObserve'{";assert needle in s;s=s.replace(needle,"  'SwarmContextObserve'{& \"$root/work/natural-moveset/Observe-NativeSwarmContext-v271.ps1\" -Take $Take -Pitch $Pitch -Stance $Stance -Holstered:$Holstered}\n"+needle);p.write_text(s)
print('SWARM_CONTEXT_OBSERVATION_HARNESS_READY')
