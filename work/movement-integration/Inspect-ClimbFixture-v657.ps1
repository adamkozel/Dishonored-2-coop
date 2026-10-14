$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$out="$PSScriptRoot/climb-fixture-v657"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path -LiteralPath $out){throw 'Unique observation directory required'};New-Item -ItemType Directory $out|Out-Null
 $launch=Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json
 if(!$launch.assetExperiment-or(Get-Process -Id $launch.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
 function Q($mode,$arg=0){$raw=if($arg){& "$root/outputs/d2-coop-lab/bin/d2_runtime_probe.exe" $launch.pid $mode $arg}else{& "$root/outputs/d2-coop-lab/bin/d2_runtime_probe.exe" $launch.pid $mode};if($LASTEXITCODE){throw "Read failed $mode"};$raw|ConvertFrom-Json}
 $all=Q '--entities';$all|ConvertTo-Json -Depth 8|Set-Content "$out/entities.json"
 $chain=@($all.entities|Where-Object {$_.name-eq'ld_mesh_chain_1'});if($chain.Count-ne1){throw 'Unique native chain absent'}
 $chain|ConvertTo-Json -Depth 8|Set-Content "$out/chain.json"
 $near=@($all.entities|Where-Object {$p=$_.position;$p-and[math]::Abs($p[0]-$chain[0].position[0])-lt4-and[math]::Abs($p[1]-$chain[0].position[1])-lt4})
 $near|ConvertTo-Json -Depth 8|Set-Content "$out/near-chain.json"
 $h=[Convert]::ToUInt32($chain[0].handle.Replace('0x',''),16);$cp=Q '--components' $h;$cp|ConvertTo-Json -Depth 8|Set-Content "$out/chain-components.json"
 $states=Q '--player-states';$states|ConvertTo-Json -Depth 8|Set-Content "$out/player-states.json"
 $machine=Q '--object' $states.machine;$machine|ConvertTo-Json -Depth 8|Set-Content "$out/machine.json"
 [pscustomobject]@{readOnly=$true;chain=$chain[0];near=$near|Select-Object name,type,handle,position;components=$cp.components|Select-Object type,pointer,vtableRva}|ConvertTo-Json -Depth 5
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
