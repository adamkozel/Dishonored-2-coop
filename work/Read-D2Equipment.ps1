param([ValidateSet('host','client')][string]$Role='host',[string]$Handle='0xb5f077b',[string]$Tag='current')
$ErrorActionPreference='Stop'
$root=Split-Path $PSScriptRoot -Parent;$probe="$root/outputs/d2-coop-lab/bin/d2_runtime_probe.exe"
$l=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json
if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Unexpected game process'}
function Read($option,$arg){$r=& $probe $l.pid $option $arg;if($LASTEXITCODE){throw "Read failed: $option $arg"};$r|ConvertFrom-Json}
$components=Read '--components' $Handle
$inventory=@($components.components|Where-Object type -eq '.?AVarkCpntInventory@@')
if($inventory.Count-ne1){throw 'Missing or ambiguous inventory'}
$object=Read '--object' $inventory[0].pointer
$equipmentPointer=($object.words|Where-Object offset -eq '0x58').word
$equipmentList=Read '--object' $equipmentPointer
$count=[int]([Convert]::ToUInt64(($object.words|Where-Object offset -eq '0x60').word.Substring(2),16)-band0xffffff)
if($count-lt1-or$count-gt16){throw 'Unexpected equipment count'}
$items=@(for($i=0;$i-lt$count;$i++){
 $entryPointer=($equipmentList.words|Where-Object offset -eq ('0x{0:x}'-f($i*8))).word
 $entry=Read '--object' $entryPointer
 $cpntPointer=($entry.words|Where-Object offset -eq '0x10').word
 $cpnt=Read '--object' $cpntPointer
 $itemDecl=Read '--object' $entry.words[0].word
 $equipDecl=Read '--object' ($entry.words|Where-Object offset -eq '0x18').word
 $entity=Read '--object' ($cpnt.words|Where-Object offset -eq '0x10').word
 $entityHandle='0x{0:x}'-f([Convert]::ToUInt64(($entry.words|Where-Object offset -eq '0x8').word.Substring(2),16)-band[uint64]4294967295)
 $entityComponents=Read '--components' $entityHandle
 [pscustomobject]@{Entry=$entry;Component=$cpnt;ItemDeclaration=$itemDecl;EquipmentDeclaration=$equipDecl;Entity=$entity;EntityComponents=$entityComponents}
})
[ordered]@{Role=$Role;At=[DateTimeOffset]::Now.ToString('o');Handle=$Handle;Components=$components;Inventory=$object;EquipmentList=$equipmentList;Items=$items}|
 ConvertTo-Json -Depth 12|Set-Content "$PSScriptRoot/guard-equipment-$Tag-$Role.json"
$object.words|Where-Object {[Convert]::ToUInt32($_.offset.Substring(2),16)-lt240}|Select-Object offset,word,pointerType|Format-Table
$equipmentList.words|Select-Object -First 20 offset,word,pointerType,pointerText|Format-Table
foreach($item in $items){$item.Entry.words|Select-Object -First 7 offset,word,pointerType,pointerText|Format-Table;$item.Component.words|Select-Object -First 30 offset,word,pointerType,pointerText|Format-Table}
