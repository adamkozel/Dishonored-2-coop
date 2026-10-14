from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Observe-NativeDominoTargets-v199.ps1').read_text(encoding='utf-8-sig')
s=s.replace('48.59','44.39').replace('155.07','153.27').replace("'coop_domino_fixture 20000'","'coop_domino_fixture 40000'")
s=s.replace('$cast=$false;$networkSnapshot=$false','$cast=$false;$secondCast=$false;$networkSnapshot=$false')
key="  $scene=Q client '--midnight';"
insert="""  if($cast-and!$secondCast-and$w.ElapsedMilliseconds-ge5500){
   $catalog2=Q client '--world-npcs';$catalog2|ConvertTo-Json -Depth 8|Set-Content \"$out/second-target-catalog.json\"
   $lower=@($catalog2.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-eq'ld_spawner_guard_tutorial_1'-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'-and[Math]::Abs([double]$_.position[2]-61.0278)-lt.5})
   if($lower.Count-ne1){throw 'Exact lower-floor tutorial guard unavailable'}
   $target2=$lower[0];$dx=[double]$target2.position[0]+44.39;$dy=[double]$target2.position[1]+153.27;$dz=68.1-([double]$target2.position[2]+1.65)
   $yaw2=[Math]::Atan2($dy,$dx)*180/[Math]::PI;$pitch2=[Math]::Round([Math]::Atan2($dz,[Math]::Sqrt($dx*$dx+$dy*$dy))*180/[Math]::PI)
   if($pitch2-lt0-or$pitch2-gt40){throw 'Second native target outside bounded balcony camera pitch'}
   C client ('setviewpos -44.39 -153.27 68.1 {0} 0'-f$yaw2.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
   & \"$PSScriptRoot/Set-ObserverPitch.ps1\" -Role client -Pitch $pitch2 -Evidence \"$out/second-target-pitch.json\" -Quick
   [ordered]@{target=$target2;yaw=$yaw2;pitch=$pitch2;note='Native controller aim from unchanged upper balcony source; no NPC relocation or spawn'}|ConvertTo-Json -Depth 6|Set-Content \"$out/second-target-aim.json\"
   Start-Sleep -Milliseconds 350;I client 255 1800;$secondCast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Second native LT hold1800ms toward lower tutorial guard'})
  }
"""
assert key in s;s=s.replace(key,insert+key)
s=s.replace('if(!$networkSnapshot-and$w.ElapsedMilliseconds-ge4500)','if(!$networkSnapshot-and$secondCast-and$w.ElapsedMilliseconds-ge10500)')
key="   Q client '--entities'|"
insert="""   $connections=Q client '--object' ('0x{0:x}'-f([Convert]::ToUInt64($networkAddress.Substring(2),16)+0x15b0));$connections|ConvertTo-Json -Depth 10|Set-Content \"$out/network-connections-header.json\"
   $connectionList=($connections.words|Where-Object offset -eq '0x0').word;$connectionCount=[Convert]::ToUInt64(($connections.words|Where-Object offset -eq '0x8').word.Substring(2),16)-band0xffffff
   if($connectionCount-gt16){throw 'Native connection count exceeds bounded observation'}
   if($connectionCount){$connectionTable=Q client '--object' $connectionList;$connectionTable|ConvertTo-Json -Depth 10|Set-Content \"$out/network-connections-table.json\"
    for($n=0;$n-lt$connectionCount;$n++){$pointer=($connectionTable.words|Where-Object offset -eq ('0x{0:x}'-f($n*8))).word;Q client '--object' $pointer|ConvertTo-Json -Depth 10|Set-Content \"$out/network-connection-$n.json\"}
   }
"""
assert key in s;s=s.replace(key,insert+key,1)
s=s.replace("if(@($markedRows|Where-Object {$_.targets.Count-ne1}).Count){throw 'Single-target experiment has an unexpected target count'}", "if(!$secondCast-or!@($markedRows|Where-Object {$_.targets.Count-eq2}).Count){throw 'Two native selected targets not established'}\n if($count-ne2-or$connectionCount-ne2){throw 'Expected two selected nodes with two directed native connections'}")
s=s.replace('in at least ten real samples','in at least ten real samples, including two selected targets and two directed raw connections')
s=s.replace("scope='Initial native Domino observation:","scope='Two-target native Domino link feasibility on existing upper/lower tutorial guards. Raw connection records are not yet trusted for replication. Initial native Domino observation:")
(P/'Observe-NativeDominoLinks-v214.ps1').write_text(s,encoding='utf-8')
f=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=f.read_text(encoding='utf-8-sig')
s=s.replace("'DominoWorld','DominoPartner','DominoNetwork'","'DominoWorld','DominoPartner','DominoLinksObserve','DominoNetwork'")
key="  'DominoWorld'{";assert key in s
s=s.replace(key,"  'DominoLinksObserve'{& \"$root/work/natural-moveset/Observe-NativeDominoLinks-v214.ps1\" -Take $Take -Pitch $Pitch -AimGuard}\n"+key)
f.write_text(s,encoding='utf-8')
