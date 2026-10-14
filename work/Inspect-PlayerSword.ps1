param([ValidateSet('host','client')][string]$Role='host',[string]$Label='snapshot')
$ErrorActionPreference='Stop';$root=Split-Path $PSScriptRoot -Parent;$probe="$root/outputs/d2-coop-lab/bin/d2_runtime_probe.exe"
$launch=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json
if((Get-Process -Id $launch.pid).Path-ne$launch.executable){throw 'Unexpected process'}
function Native($option,$address=''){$arguments=@($launch.pid,$option);if($address){$arguments+=$address};$raw=& $probe @arguments;if($LASTEXITCODE){throw "Read failed: $Role $option $address"};$raw|ConvertFrom-Json}
function Word($obj,$offset){($obj.words|Where-Object offset -eq $offset).word}
$before=Get-Content "$root/work/lab-instances/$Role/coop-status.json" -Raw|ConvertFrom-Json
$components=Native '--components' 1;$inventory=@($components.components|Where-Object type -eq '.?AVarkCpntInventory@@')
if($inventory.Count-ne1){throw 'Missing/ambiguous inventory'}
$inv=Native '--object' $inventory[0].pointer;$pointer=Word $inv '0x58';$count=[Convert]::ToUInt64((Word $inv '0x60').Substring(2),16)-band0xffffff
if(!$count-or$count-gt16){throw 'Equipment count outside observed scope'}
$list=Native '--object' $pointer;$sword=$null
for($i=0;$i-lt$count;$i++){
 $entry=Native '--object' (Word $list ('0x{0:x}'-f($i*8)));$item=Native '--object' (Word $entry '0x0')
 if(($item.words|Where-Object offset -eq '0x8').pointerText-ne'models/weapons/ply_sword/ply_sword'){continue}
 if($sword){throw 'Ambiguous sword'}
 $component=Native '--object' (Word $entry '0x10');$decl=Native '--object' (Word $entry '0x18');$entity=Native '--object' (Word $component '0x10')
 $frame=Native '--object' (Word $entity '0x358')
 $sword=[pscustomobject]@{Entry=$entry;Item=$item;Component=$component;Declaration=$decl;Entity=$entity;Frame=$frame}
}
$after=Get-Content "$root/work/lab-instances/$Role/coop-status.json" -Raw|ConvertFrom-Json
if($before.localEpoch-ne$after.localEpoch-or$before.localGameLogicState-ne1-or$after.localGameLogicState-ne1){throw 'Read crossed world boundary'}
[ordered]@{Role=$Role;Label=$Label;ObservedAt=[DateTimeOffset]::Now.ToString('o');Before=$before;After=$after;Inventory=$inv;Sword=$sword}|ConvertTo-Json -Depth 14|Set-Content "$PSScriptRoot/player-sword-$Role-$Label.json"
[pscustomobject]@{Role=$Role;Label=$Label;Slot=(Word $inv '0x68');Entry=$sword.Entry.pointer;Flags=(Word $sword.Component '0x18');EntityType=$sword.Entity.type;Parent=(Word $sword.Entity '0xa8');FrameType=$sword.Frame.type;FrameParent=(Word $sword.Frame '0x3c8');FrameTag=($sword.Frame.words|Where-Object offset -eq '0x3c0').pointerText;Joint=(Word $sword.Frame '0x3d0')}
