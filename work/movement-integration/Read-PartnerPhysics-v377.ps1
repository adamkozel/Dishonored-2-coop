$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$out="$PSScriptRoot/partner-physics-read-v377";$probe="$root/outputs/d2-coop-lab/bin/d2_runtime_probe.exe"
$m=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$m.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Preserve prior read'};New-Item -ItemType Directory $out|Out-Null
 foreach($role in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$role/launch.json" -Raw|ConvertFrom-Json;$s=Get-Content "$root/work/lab-instances/$role/coop-status.json" -Raw|ConvertFrom-Json
  if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Unexpected process'}
  foreach($who in 'player','partner'){
   $handle=if($who-eq'player'){1}else{$s.probeActorHandle}
   $raw=& $probe $l.pid --components $handle;if($LASTEXITCODE){throw 'Component read failed'};$raw|Set-Content "$out/$role-$who-components.json";$components=$raw|ConvertFrom-Json
   foreach($c in $components.components|Where-Object {$_.type-match'Physics'}){
    $name=($c.type-replace'[^a-zA-Z0-9]','');$raw=& $probe $l.pid --object $c.pointer;if($LASTEXITCODE){throw 'Physics object read failed'};$raw|Set-Content "$out/$role-$who-$name.json"
   }
  }
 }
 'Read-only player and partner physics components saved'
}finally{if($owned){$m.ReleaseMutex()};$m.Dispose()}
