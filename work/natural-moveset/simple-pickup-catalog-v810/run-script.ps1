param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='simple-pickup-catalog-v810')
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$pids=@{};$rows=@{};$cache=@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 18|Set-Content -LiteralPath $p}
function Q($r,$mode,$arg=0){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if($LASTEXITCODE){throw "Probe failed $r $mode"};return($raw|ConvertFrom-Json)}
function O($r,$ptr){$key="$r-$ptr";if(!$cache.ContainsKey($key)){$cache[$key]=Q $r '--object' $ptr;Save "$out/$key.json" $cache[$key]};return $cache[$key]}
function Field($o,$offset){$w=@($o.words|Where-Object offset -eq $offset);if($w.Count-ne1){throw "Missing $offset"};return $w[0]}
function W($o,$offset){[Convert]::ToUInt64((Field $o $offset).word.Substring(2),16)}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique evidence directory required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='PickupFamilies810';state='running';take=$Take}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong native instance'};$pids[$r]=$l.pid
  $before=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($before.localGameLogicState-ne1-or$before.localPendingGameLogicState){throw 'Gameplay required for registry inventory'}
  $entities=Q $r '--entities';Save "$out/entities-$r.json" $entities;$coins=@($entities.entities|Where-Object {$_.type-eq'.?AVarkPickup@@'})
  if(!$coins.Count-or$coins.Count-gt128){throw 'Coin-name catalog unexpectedly empty or unbounded'};$list=[Collections.Generic.List[object]]::new()
  foreach($e in $coins){
   $cs=Q $r '--components' $e.handle;Save "$out/components-$r-$($e.handle).json" $cs;$pick=@($cs.components|Where-Object type -eq '.?AVarkCpntPickable_Simple@@')
   if($pick.Count-eq0){continue};if($pick.Count-ne1-or$pick[0].owner-ne$e.pointer){throw 'Simple pickup component ownership mismatch'}
   $c=O $r $pick[0].pointer;$d=O $r (Field $c '0x8').word;$item=O $r (Field $d '0xf8').word
   $list.Add([ordered]@{name=$e.name;handle=$e.handle;actor=$e.pointer;position=$e.position;component=$pick[0].pointer;generation=((W $c '0x18')-band[uint64]4294967295);componentFlags=((W $c '0x18')-shr32);pickupFlags=((W $c '0xd0')-band255);declaration=(Field $c '0x8').word;declarationType=$d.type;declarationName=(Field $d '0x8').pointerText;carried=((W $d '0xa0')-band255);quantity=((W $d '0x100')-band[uint64]4294967295);item=(Field $d '0xf8').word;itemType=$item.type;itemName=(Field $item '0x8').pointerText;typeId=((W $item '0x60')-band[uint64]4294967295);special=((W $item '0xc0')-shr32)})
  }
  $after=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json
  if($before.localEpoch-ne$after.localEpoch-or$after.localGameLogicState-ne1-or$after.localPendingGameLogicState){throw 'World changed during read-only inspection'}
  $again=Q $r '--entities';foreach($row in $list){$match=@($again.entities|Where-Object {$_.name-eq$row.name-and$_.handle-eq$row.handle-and$_.pointer-eq$row.actor});if($match.Count-ne1){throw 'Authored coin retired during read-only inspection'}}
  $rows[$r]=$list;Save "$out/catalog-$r.json" $list
 }
 $hostKeys=@($rows.host|ForEach-Object {"$($_.name)|$($_.itemName)|$($_.quantity)|$($_.typeId)|$($_.carried)"});$clientKeys=@($rows.client|ForEach-Object {"$($_.name)|$($_.itemName)|$($_.quantity)|$($_.typeId)|$($_.carried)"})
 if(Compare-Object $hostKeys $clientKeys){throw 'Paired authored coin catalogs differ'}
}catch{$failure=$_.Exception.Message}finally{
 if($owned){if(Test-Path $out){Save "$out/result.json" @{passed=(!$failure);failure=$failure;readOnly=$true;hostCount=@($rows.host).Count;clientCount=@($rows.client).Count;scope='All arkPickup candidates with one exact Simple component; independent declaration types and item types captured read-only in both restored worlds.'}}
 Save "$root/work/movement-integration/native-operation-owner.json" @{pid=$PID;operation='PickupFamilies810';state='finished';take=$Take};$mutex.ReleaseMutex()};$mutex.Dispose()
}
if($failure){throw $failure};$rows.host|ForEach-Object {[pscustomobject]$_}|Group-Object itemName,quantity|Select-Object Name,Count|ConvertTo-Json

