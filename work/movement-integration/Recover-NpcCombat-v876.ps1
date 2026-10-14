$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$out="$root/work/natural-moveset/npc-combat-reload-v876/recovery"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 New-Item -ItemType Directory -Path $out -ErrorAction Stop|Out-Null
 $pin=Get-Content "$PSScriptRoot/candidate-npc-combat-v871/identity.json" -Raw|ConvertFrom-Json
 if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Wrong recovery binary'}
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$p=Get-Process -Id $l.pid
  if(!$l.assetExperiment-or$p.Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')-or[Math]::Abs(($p.StartTime-[datetime]$l.startedAt).TotalSeconds)-gt5){throw 'Wrong isolated process'}
  foreach($file in 'coop-status.json','coop-status.json.reset.json','coop-status.json.npc-stream.json','coop-status.json.npc-combat.json','engine.log'){Copy-Item "$root/work/lab-instances/$r/$file" "$out/$r-$file"}
 }
 # Existing guarded restart affects only the two verified isolated processes.
 & "$PSScriptRoot/Deploy-SourceClock.ps1" *> "$out/restart.log"
 if(!$?){throw 'Isolated restart recovery failed'}
 & "$root/work/natural-moveset/Restore-LabNoteInventory-v779.ps1" -Baseline "$root/work/natural-moveset/native-coin-reward-v786c" -Evidence "$out/note-restoration"|Out-Null
 foreach($r in 'host','client'){
  $s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json
  $n=Get-Content "$root/work/lab-instances/$r/coop-status.json.npc-stream.json" -Raw|ConvertFrom-Json
  $c=Get-Content "$root/work/lab-instances/$r/coop-status.json.npc-combat.json" -Raw|ConvertFrom-Json
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json
  $h=& "$root/outputs/d2-coop-lab/bin/d2_runtime_probe.exe" $l.pid --health 1|ConvertFrom-Json
  if($LASTEXITCODE-or$s.localGameLogicState-ne1-or!$s.remoteFresh-or$s.connectionState-ne3-or$n.enabled-or$c.enabled-or$h.current-ne56){throw "Recovery state invalid $r"}
  @{bridge=$s;stream=$n;combat=$c;health=$h;launch=$l}|ConvertTo-Json -Depth 12|Set-Content "$out/after-$r.json"
 }
 foreach($f in Get-Content "$PSScriptRoot/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
 @{passed=$true;dll=$pin.dll;method='Guarded restart of exact isolated pair after checkpoint expectBoundary remained set in failed phase';originalFailurePreserved=$true}|ConvertTo-Json|Set-Content "$out/result.json"
 Get-Content "$out/result.json"
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
