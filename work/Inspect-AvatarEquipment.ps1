$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent;$probe="$root/outputs/d2-coop-lab/bin/d2_runtime_probe.exe"
$rows=@(foreach($role in 'host','client'){
 $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json
 if((Get-Process -Id $launch.pid).Path-ne$launch.executable){throw 'Unexpected process'}
 $before=Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json
 if($before.localGameLogicState-ne1-or!$before.probeActorHandle){throw 'No current gameplay counterpart'}
 function Native($option,$address){$raw=& $probe $launch.pid $option $address;if($LASTEXITCODE){throw "Read failed: $role $option $address"};$raw|ConvertFrom-Json}
 function Word($obj,$offset){($obj.words|Where-Object offset -eq $offset).word}
 foreach($handle in $before.probeActorHandle,1){
  $components=Native '--components' $handle;$inventory=@($components.components|Where-Object type -eq '.?AVarkCpntInventory@@')
  if($inventory.Count-ne1){throw 'Missing/ambiguous inventory'}
  $obj=Native '--object' $inventory[0].pointer;$pointer=Word $obj '0x58';$count=[Convert]::ToUInt64((Word $obj '0x60').Substring(2),16)-band0xffffff
  if($count-gt16){throw 'Equipment count too large'}
  $equipmentList=if($count){Native '--object' $pointer}else{$null}
  $items=@(for($i=0;$i-lt$count;$i++){
   $entry=Native '--object' (Word $equipmentList ('0x{0:x}'-f($i*8)))
   $item=Native '--object' (Word $entry '0x0');$component=Word $entry '0x10';$declaration=Word $entry '0x18'
   $equipment=if($declaration-ne'0x0'){Native '--object' $declaration}else{$null}
   $cpnt=if($component-ne'0x0'){Native '--object' $component}else{$null}
   [pscustomobject]@{ItemName=($item.words|Where-Object offset -eq '0x8').pointerText;EquipmentName=($equipment.words|Where-Object offset -eq '0x8').pointerText;ComponentType=$cpnt.type;Entry=$entry;Component=$cpnt;Item=$item;Equipment=$equipment}
  })
  [pscustomobject]@{Role=$role;Handle=$handle;Epoch=$before.localEpoch;Components=$components;Inventory=$obj;EquipmentCount=$count;EquipmentList=$equipmentList;Items=$items}
 }
 $after=Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json
 if($before.localEpoch-ne$after.localEpoch-or$after.localGameLogicState-ne1){throw 'Inventory snapshot crossed a world boundary'}
})
$rows|ConvertTo-Json -Depth 16|Set-Content "$PSScriptRoot/avatar-equipment-770F.json"
@($rows|ForEach-Object{[pscustomobject]@{Role=$_.Role;Handle=$_.Handle;EquipmentCount=$_.EquipmentCount;Items=@($_.Items|Select-Object ItemName,EquipmentName,ComponentType)}})|ConvertTo-Json -Depth 6
