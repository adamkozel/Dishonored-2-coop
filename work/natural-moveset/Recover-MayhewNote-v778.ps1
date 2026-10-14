$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out="$PSScriptRoot/native-continuous-recovery-v778/note-recovery";$m=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed|Out-Null}
function Q($r,$mode){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if((Get-Process -Id $l.pid).Path-ne$l.executable){throw 'Isolated identity differs'};$j=& "$lab/bin/d2_runtime_probe.exe" $l.pid $mode;if($LASTEXITCODE){throw "Read failed $mode"};$j|ConvertFrom-Json}
try{
 try{$owned=$m.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Existing recovery evidence'};New-Item -ItemType Directory $out|Out-Null
 foreach($r in 'host','client'){C $r 'coop_movement_fixture 60000';(Q $r '--player-inventory')|ConvertTo-Json -Depth 9|Set-Content "$out/before-$r.json"}
 C client 'setviewpos -39.4 -161.4 68.1 270 20'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 20 -Evidence "$out/aim.json" -Quick
 & "$lab/Send-D2Input.ps1" -Role client -Button X -DurationMs 180|Out-Null
 Start-Sleep -Milliseconds 1500
 $logic=Q client '--logic';$logic|ConvertTo-Json -Depth 5|Set-Content "$out/logic.json"
 if(@($logic.stack|Where-Object id -eq 4).Count){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 600}
 foreach($r in 'host','client'){(Q $r '--player-inventory')|ConvertTo-Json -Depth 9|Set-Content "$out/after-$r.json"}
 foreach($r in 'host','client'){$expected=Get-Content "$PSScriptRoot/native-continuous-recovery-v778/inventory-before-$r.json" -Raw|ConvertFrom-Json;$actual=Get-Content "$out/after-$r.json" -Raw|ConvertFrom-Json;$diff=Compare-Object @($expected.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($actual.items|ForEach-Object {"$($_.name):$($_.quantity)"});$diff|ConvertTo-Json -Depth 5|Set-Content "$out/difference-$r.json";if($diff){$diff|Format-Table;throw "Native note restoration incomplete for $r"}}
 @{passed=$true;scope='Native X on the already visited Mayhew corpse restores only the initial Emily note; full role inventories compared to original778 baseline'}|ConvertTo-Json|Set-Content "$out/result.json"
}finally{if($owned){foreach($r in 'host','client'){& "$lab/Send-D2Input.ps1" -Role $r -DurationMs 100|Out-Null};C host 'setviewpos -33 -153 68.1 90 0';C client 'setviewpos -33 -148 68.1 270 0';foreach($r in 'host','client'){C $r 'coop_movement_fixture off'};$m.ReleaseMutex()};$m.Dispose()}
