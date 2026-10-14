from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Observe-NativeDomino-v189.ps1').read_text(encoding='utf-8-sig').replace('$w.Restart();$cast=$false','$w.Restart();$cast=$false;$networkSnapshot=$false')
a='  Start-Sleep -Milliseconds 25';assert s.count(a)==1
s=s.replace(a,'''  if(!$networkSnapshot-and$w.ElapsedMilliseconds-ge4500){
   $cp=Q client '--object' $power.component;$cp|ConvertTo-Json -Depth 10|Set-Content "$out/network-equipment.json"
   $networkAddress=($cp.words|Where-Object offset -eq '0xb8').word
   if(!$networkAddress-or$networkAddress-eq'0x0'){throw 'No native Domino network component to inspect'}
   $net=Q client '--object' $networkAddress;$net|ConvertTo-Json -Depth 10|Set-Content "$out/network-component.json"
   if($net.type-ne'.?AVarkCpntPhantasmalNetwork@@'){throw 'Unexpected native network component type'}
   foreach($field in '0x8','0x10'){$ptr=($net.words|Where-Object offset -eq $field).word;Q client '--object' $ptr|ConvertTo-Json -Depth 10|Set-Content "$out/network-owner-$field.json"}
   $list=($net.words|Where-Object offset -eq '0x28').word;$count=[Convert]::ToUInt64(($net.words|Where-Object offset -eq '0x30').word.Substring(2),16)-band0xffffff
   if($count-gt4){throw 'Native selected-target count outside bounded research scope'}
   if($count){
    $table=Q client '--object' $list;$table|ConvertTo-Json -Depth 10|Set-Content "$out/network-selected-table.json"
    for($index=0;$index-lt$count;$index++){
     $ptr=($table.words|Where-Object offset -eq ('0x{0:x}'-f($index*8))).word
     Q client '--object' $ptr|ConvertTo-Json -Depth 10|Set-Content "$out/network-node-$index.json"
    }
   }
   Q client '--entities'|ConvertTo-Json -Depth 10|Set-Content "$out/network-entities.json"
   Q client '--world-npcs'|ConvertTo-Json -Depth 10|Set-Content "$out/network-npcs.json"
   Q client '--domino-effects'|ConvertTo-Json -Depth 10|Set-Content "$out/network-effects.json"
   [ordered]@{nativeComponent=$networkAddress;count=$count;list=$list;note='Read-only raw objects after native controller selection. Field semantics and target ownership must be validated before any network serialization.'}|ConvertTo-Json|Set-Content "$out/network-snapshot.json"
   $networkSnapshot=$true
  }
'''+a)
a=" $checks.Add('Native player state, equipment, raw component/entity and filtered effect samples retained; mana change and targeting outcome remain observations')";assert s.count(a)==1
s=s.replace(a,a+"\n if(!$networkSnapshot){throw 'Native network snapshot absent'}; $checks.Add('Read-only native network, selected-node and target/effect objects preserved without invoking link or damage methods')")
(P/'Observe-NativeDominoNetwork-v195.ps1').write_text(s,encoding='utf-8')
f=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=f.read_text(encoding='utf-8-sig');s=s.replace("'DominoObserve','DominoNetwork'","'DominoObserve','DominoNetworkObserve','DominoNetwork'")
a="  'DominoObserve'{";assert s.count(a)==1
s=s.replace(a,"  'DominoNetworkObserve'{& \"$root/work/natural-moveset/Observe-NativeDominoNetwork-v195.ps1\" -Take $Take -Pitch $Pitch -AimGuard}\n"+a);f.write_text(s,encoding='utf-8')
print('Read-only Domino network snapshot harness ready')
