$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab"
$out="$PSScriptRoot/npc-camera-recovery-v844"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique evidence required'};New-Item -ItemType Directory $out|Out-Null
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-stream-v842/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Candidate changed'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[datetime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated process'}
  Copy-Item "$root/work/lab-instances/$r/coop-status.json" "$out/before-$r.json"
  & "$lab/Send-D2Command.ps1" -Role $r -Command 'coop_npc_stream off' -WaitConsumed|Out-Null
 }
 & "$root/work/PrepareShadowSword.ps1" -EvidencePath "$out/native-setup.json"
 & "$PSScriptRoot/Restore-LabNoteInventory-v779.ps1" -Baseline "$PSScriptRoot/native-coin-reward-v786c" -Evidence "$out/note-restoration"
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  $s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;$s|ConvertTo-Json -Depth 20|Set-Content "$out/after-$r.json"
  if($s.localGameLogicState-ne1-or$s.localPendingGameLogicState-or!$s.remoteFresh){throw 'Paired gameplay not restored'}
  $hp=& "$lab/bin/d2_runtime_probe.exe" $l.pid --health 1;if($LASTEXITCODE){throw 'Health query failed'};$hp|Set-Content "$out/health-$r.json"
  if(($hp|ConvertFrom-Json).current-ne56){throw 'Baseline health not restored'}
  & "$lab/bin/d2_runtime_probe.exe" $l.pid --player|Set-Content "$out/player-$r.json";if($LASTEXITCODE){throw 'Player query failed'}
 }
 foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
 @{passed=$true;cause='843b harness passed root XYZ and reversed yaw/pitch to native setviewpos. Host fell below floor; stream correctly stopped on native game-over. Both isolated SLOT81 fixtures and inventory restored.'}|ConvertTo-Json|Set-Content "$out/result.json"
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
