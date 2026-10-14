$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out="$PSScriptRoot/door-native-api-v709"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
function W($o,$offset){[Convert]::ToUInt64(($o.words|Where-Object offset -eq $offset).word.Substring(2),16)}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique evidence required'};New-Item -ItemType Directory $out|Out-Null
 $launch=Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json
 if(!$launch.assetExperiment-or(Get-Process -Id $launch.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated game'}
 function Q($mode,$arg=0){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $launch.pid $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $launch.pid $mode};if($LASTEXITCODE){throw 'Native read failed'};$raw|ConvertFrom-Json}
 $door=@((Q '--entities').entities|Where-Object name -eq 'ld_door_350cm_int_rich_wood_01_3');if($door.Count-ne1){throw 'Exact door absent'};$door=$door[0]
 $actor=Q '--object' $door.pointer;$cp=Q '--object' (W $actor '0x370');$anim=Q '--object' (W $cp '0xf8');$def=Q '--object' (W $actor '0x2e8')
 $childHandle=[uint32]((W $cp '0xd8')-shr32);$childComponents=Q '--components' $childHandle;$doorComponents=Q '--components' $door.handle
 foreach($entry in @(@('actor',$actor),@('openable',$cp),@('animation',$anim),@('definition',$def),@('child-components',$childComponents),@('door-components',$doorComponents))){$entry[1]|ConvertTo-Json -Depth 9|Set-Content "$out/$($entry[0]).json"}
 $base=(W $actor '0x0')-0x1cb7c68
 [ordered]@{readOnly=$true;pid=$launch.pid;imageBase=('0x{0:x}'-f$base);door=$door;fields=@($cp.words|Where-Object offset -in @('0x0','0x10','0x18','0x40','0x48','0x60','0x68','0x78','0x80','0x88','0x98','0xb8','0xc0','0xc8','0xd0','0xf0','0xf8','0x118','0x1e0','0x1e8'));animation=$anim.words[0..4];definition=$def.words[0..1];child=$childComponents}|ConvertTo-Json -Depth 9|Set-Content "$out/summary.json"
 Get-Content "$out/summary.json"
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
