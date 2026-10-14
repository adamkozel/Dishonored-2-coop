$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out="$PSScriptRoot/door-catalog-v721"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
function W($o,$offset){[Convert]::ToUInt64(($o.words|Where-Object offset -eq $offset).word.Substring(2),16)}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique evidence required'};New-Item -ItemType Directory $out|Out-Null
 $pin=Get-Content "$PSScriptRoot/candidate-door-sync-v716/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Unexpected candidate'}
 $all=@()
 foreach($role in 'host','client'){
  $launch=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json;$process=Get-Process -Id $launch.pid
  if(!$launch.assetExperiment-or$process.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
  function Q($mode,$arg=0){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $launch.pid $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $launch.pid $mode};if($LASTEXITCODE){throw 'Native read failed'};$raw|ConvertFrom-Json}
  $doors=@((Q '--entities').entities|Where-Object type -eq '.?AVarkDoor@@');if($doors.Count-gt128){throw 'Unexpected door catalogue size'}
  foreach($door in $doors){
   $cps=Q '--components' $door.handle;$cp=@($cps.components|Where-Object type -eq '.?AVarkCpntOpenable@@');if($cp.Count-ne1-or$cp[0].owner-ne$door.pointer){throw 'Door owner mismatch'}
   $o=Q '--object' $cp[0].pointer;$o|ConvertTo-Json -Depth 7|Set-Content "$out/$role-$($door.name)-openable.json"
   $flags=W $o '0x1e8';$states=W $o '0x48';$door|ConvertTo-Json -Depth 7|Set-Content "$out/$role-$($door.name)-entity.json"
   $all+=[pscustomobject]@{role=$role;pid=$launch.pid;name=$door.name;handle=$door.handle;position=$door.position;current=[uint32]($states-band[uint64]4294967295);pending=[uint32]($states-shr32);locked=[int]($flags-band255);blocked=[int]((W $o '0x118')-band255);broken=[int](($flags-shr40)-band255);linked=('0x{0:x}'-f(W $o '0xf0'));entries=('0x{0:x}'-f(W $o '0x1e0'))}
  }
 }
 $all|ConvertTo-Json -Depth 6|Set-Content "$out/summary.json";$all|ConvertTo-Json -Depth 5
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
