$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$archive="$PSScriptRoot/candidate-npc-combat-v966"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 $current=Get-Content "$PSScriptRoot/candidate-npc-combat-v973/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$current.dll){throw 'Unexpected live candidate'}
 # Restore only our changed files after verifying that the experiment archive contains them.
 foreach($name in 'engine_bridge.cpp','npc_authority_stream.h','world_identity_tests.cpp','npc_perception.h','npc_perception_policy.h','npc_perception_policy_tests.h'){
  $path=Join-Path "$lab/src" $name;$saved=Join-Path "$PSScriptRoot/candidate-npc-combat-v973/src" $name
  if((Get-FileHash -LiteralPath $path).Hash-ne(Get-FileHash -LiteralPath $saved).Hash){throw "Source changed since archive: $name"}
 }
 foreach($name in 'engine_bridge.cpp','npc_authority_stream.h','world_identity_tests.cpp'){Copy-Item -LiteralPath "$archive/src/$name" -Destination "$lab/src/$name"}
 foreach($name in 'npc_perception.h','npc_perception_policy.h','npc_perception_policy_tests.h'){Remove-Item -LiteralPath "$lab/src/$name"}
 Copy-Item -LiteralPath "$archive/Send-D2Command.ps1" -Destination "$lab/Send-D2Command.ps1"
 foreach($name in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item -LiteralPath "$archive/$name" -Destination "$lab/bin/$name"}
 & "$PSScriptRoot/Deploy-SourceClock.ps1"
 if(!$?){throw 'Verified candidate recovery failed'}
 & "$root/work/natural-moveset/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$PSScriptRoot/npc-perception-recovery-v975"
 @{restored=$true;candidate='candidate-npc-combat-v966';reason='Read-only faction evidence974 shows native Doppelganger already has Player faction and all5 eligible enemies already hostile without overrides. Candidate973 archived as unused experiment; no need for relation writes.'}|ConvertTo-Json|Set-Content "$PSScriptRoot/npc-perception-recovery-v975.json"
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
