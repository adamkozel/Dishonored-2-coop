from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
s=(P/'Observe-NativePossessionContext-v361.ps1').read_text()
s=s.replace("[int]$Pitch=30", "[int]$Pitch=12")
s=s.replace("C host 'setviewpos -48.59 -155.07 68.1 90 0';Start-Sleep -Milliseconds 800", "C host 'setviewpos -33.5 -153 68.1 90 0';C client 'setviewpos -33.5 -150 68.1 270 0';Start-Sleep -Milliseconds 1000")
s=s.replace("C host 'coop_possession_fixture 40000'", "C host 'coop_possession_fixture human40000'")
old=" $expectedStance=if($Stance-eq'crouched'){1}else{0}"
assert old in s
s=s.replace(old,""" $upgrade=@((Q host '--player-inventory').items|Where-Object name -eq 'components/characters/player/base/attributes/possession_parasite.inventoryitem')
 if($upgrade.Count-ne1-or$upgrade[0].quantity-ne1){throw 'Native human upgrade missing'}
 $partner=S host;$partner|ConvertTo-Json -Depth 8|Set-Content "$out/partner-before.json"
 if([Math]::Abs([double]$partner.probeRenderPosition[0]+33.5)-gt.15-or[Math]::Abs([double]$partner.probeRenderPosition[1]+150)-gt.15){throw 'Partner not in measured native aim corridor'}
 Q host '--components' $partner.probeActorHandle|ConvertTo-Json -Depth 12|Set-Content "$out/partner-components.json"
 $selected=[Collections.Generic.List[object]]::new()
"""+old)
s=s.replace('$w.Restart();$cast=$false', '$w.Restart();$cast=$false;$cancelled=$false')
old="  if(!$cast-and$w.ElapsedMilliseconds-ge1100){I host 255 1800;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native Corvo LT1800 then release at grounded floor'})}"
assert old in s
s=s.replace(old,"""  if(!$cast-and$w.ElapsedMilliseconds-ge1100){I host 255 8000;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native Corvo LT held toward exact partner, cancellation before release'})}
  if($cast-and!$cancelled-and$w.ElapsedMilliseconds-ge3600){
   $state=Q host '--player-states'
   if(!$state.defaultMode-or$state.active.id-notcontains63){throw 'Native aim scene absent before cancellation'}
   & "$lab/Send-D2Input.ps1" -Role host -Button B -LeftTrigger 255 -DurationMs 150|Out-Null
   Start-Sleep -Milliseconds 200;I host;$cancelled=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native B cancel while LT held, then neutral'})
  }
  $cp=Q host '--object' $power.component
  $targetPointer=($cp.words|Where-Object offset -eq '0xc0').word
  $targetObject=if($targetPointer-and$targetPointer-ne'0x0'){Q host '--object' $targetPointer}else{$null}
  $selected.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;component=$cp;target=$targetObject;partnerHandle=(S host).probeActorHandle;partnerActor=(S host).probeActor})
""")
s=s.replace(" $accepted=@($owned|Where-Object valid)"," $accepted=@($owned|Where-Object {$_.valid})")
s=s.replace(" $checks.Add('Raw native scenes", " $selected|ConvertTo-Json -Depth 14|Set-Content \"$out/selected-target-observations.json\"\n $checks.Add('Native selected-target references and partner component membership recorded; eligibility remains an observation, not an exclusion claim')\n $checks.Add('Raw native scenes")
s=s.replace(" C host 'coop_possession_fixture off';$w.Restart()", " if(!$cancelled){throw 'Native cancellation not sent'}\n $final=Q host '--player-states';if(!$final.defaultMode-or$final.stance-ne0){throw 'Native cancel did not retain standing player'}\n C host 'coop_possession_fixture off';$w.Restart()")
s=s.replace(" $rows|ConvertTo-Json -Depth 22|Set-Content", " if($selected){$selected|ConvertTo-Json -Depth 14|Set-Content \"$out/selected-target-observations.json\"}\n $rows|ConvertTo-Json -Depth 22|Set-Content")
s=s.replace('empty-target stance/equipment observation only.','Partner aim observation with native human upgrade and B cancellation before release. No partner exclusion implemented by this test.')
(P/'Observe-PossessionPartner-v373.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace("'PossessionObserve','SwarmObserve'","'PossessionObserve','PossessionPartnerObserve','SwarmObserve'")
old="  'PossessionObserve'{";assert s.count(old)==1
s=s.replace(old,"  'PossessionPartnerObserve'{& \"$root/work/natural-moveset/Observe-PossessionPartner-v373.ps1\" -Take $Take -Pitch $Pitch}\n"+old)
p.write_text(s)
print('Bounded partner aim observation prepared; no gameplay filter or asset mutation')
