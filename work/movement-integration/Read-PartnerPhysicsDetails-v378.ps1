$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$out="$PSScriptRoot/partner-physics-details-v378";$probe="$root/outputs/d2-coop-lab/bin/d2_runtime_probe.exe"
$m=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
function Field($o,$offset){($o.words|Where-Object offset -eq $offset).word}
function Obj($at,$name){$raw=& $probe $l.pid --object $at;if($LASTEXITCODE){throw 'Object read failed'};$raw|Set-Content "$out/$role-$name.json";return($raw|ConvertFrom-Json)}
try{
 try{$owned=$m.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Preserve prior read'};New-Item -ItemType Directory $out|Out-Null
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json;$s=Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Unexpected process'}
  $components=(& $probe $l.pid --components 1)|ConvertFrom-Json;$pp=@($components.components|Where-Object type -eq '.?AVarkCpntPlayerPhysics@@');if($pp.Count-ne1){throw 'Physics owner ambiguous'}
  $p=Obj $pp[0].pointer 'player-physics';$physics=Obj (Field $p '0x48') 'player-physics-inner';$proxy=Obj (Field $physics '0x148') 'player-havok-proxy'
  $userData=Obj ('0x{0:x}'-f([Convert]::ToUInt64($physics.pointer.Substring(2),16)+0x160)) 'player-body-userdata'
  $world=Obj (Field $physics '0x138') 'physics-world';$havok=Obj (Field $world '0x190') 'havok-world'
  $bodyId=([Convert]::ToUInt64((Field $proxy '0x38').Substring(2),16)-band0xffffffffL)
  $bodyTable=[Convert]::ToUInt64((Field $havok '0x28').Substring(2),16);$body=Obj ('0x{0:x}'-f($bodyTable+($bodyId-band0xffffff)*0xb0)) 'player-body'
  $components=(& $probe $l.pid --components $s.probeActorHandle)|ConvertFrom-Json;$np=@($components.components|Where-Object type -eq '.?AVarkCpntPhysicsSystemEvolved@@');if($np.Count-ne1){throw 'Partner physics ambiguous'}
  $p=Obj $np[0].pointer 'partner-physics';$sys=Obj (Field $p '0x100') 'partner-havok-system';$ud=Obj (Field $p '0x1d8') 'partner-body-userdata'
  [ordered]@{playerBodyId=('0x{0:x}'-f$bodyId);bodyTable=('0x{0:x}'-f$bodyTable);partner=$s.probeActorHandle}|ConvertTo-Json|Set-Content "$out/$role-summary.json"
 }
 'Physics ownership details saved read-only'
}finally{if($owned){$m.ReleaseMutex()};$m.Dispose()}
