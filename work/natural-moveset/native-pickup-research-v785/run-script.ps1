$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out="$PSScriptRoot/native-pickup-research-v785";$m=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$ids=@{}
function Q($r,$mode,$arg=0){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $ids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $ids[$r] $mode};if($LASTEXITCODE){throw "Native read failed $mode"};return($raw|ConvertFrom-Json)}
function Save($p,$v){$v|ConvertTo-Json -Depth 14|Set-Content -LiteralPath $p}
try{
 try{$owned=$m.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Evidence already exists'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[DateTime]$l.startedAt).TotalSeconds)-gt5){throw 'Unexpected isolated incarnation'};$ids[$r]=$l.pid
  $e=@((Q $r '--entities').entities|Where-Object name -eq 'pickups_coin_emily_lowvalue_4');if($e.Count-ne1-or$e[0].type-ne'.?AVarkPickup@@'-or$e[0].vtableRva-ne'0x1c5e258'){throw 'Exact coin identity missing'}
  Save "$out/entity-$r.json" $e[0];Save "$out/entity-object-$r.json" (Q $r '--object' $e[0].pointer)
  $cs=Q $r '--components' $e[0].handle;Save "$out/components-$r.json" $cs
  foreach($type in '.?AVarkCpntLoot@@','.?AVarkCpntPickable_Simple@@'){
   $c=@($cs.components|Where-Object type -eq $type);if($c.Count-ne1-or$c[0].owner-ne$e[0].pointer){throw 'Component owner mismatch'}
   $tag=if($type-eq'.?AVarkCpntLoot@@'){'loot'}else{'pickable'};$o=Q $r '--object' $c[0].pointer;Save "$out/$tag-$r.json" $o
   $decl=@($o.words|Where-Object offset -eq '0x8');if($decl.Count-ne1){throw 'Component declaration field missing'};$d=Q $r '--object' $decl[0].word;Save "$out/$tag-declaration-$r.json" $d
   if($tag-eq'pickable'){$item=@($d.words|Where-Object offset -eq '0xf8');if($item.Count-eq1-and$item[0].word-ne'0x0'){Save "$out/item-declaration-$r.json" (Q $r '--object' $item[0].word)}}
  }
  Save "$out/inventory-$r.json" (Q $r '--player-inventory')
 }
 Save "$out/result.json" @{readOnly=$true;passed=$true;at=(Get-Date).ToString('o');pids=$ids;scope='Exact authored coin entity, native loot/pickable components and declarations in both isolated worlds. No pickup command, inventory change or network authority implemented.'}
 'PASS_READ_ONLY_PICKUP_IDENTITIES'
}finally{if($owned){$m.ReleaseMutex()};$m.Dispose()}
