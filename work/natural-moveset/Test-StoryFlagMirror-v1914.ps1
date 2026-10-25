param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='story-flag-mirror-v1914a',[string]$Candidate='candidate-npc-combat-v1214kk')
# 1914 (#220): the story flag mirror's write path in the lab pair (1214kk). Both lab games on the Dunwall fixture, the
# product controller off as left by the restore. The partner flips her first bool story flag with the test fixture
# coop_story_flags flip; within 5 s the mirror must write it back to the host's value (writes grows, the result matched,
# no flag differing). Reads only the two status files; the lab is left as it was (the flipped byte restored by the mirror).
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 8|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function F($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.story-flags.json" -Raw|ConvertFrom-Json}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$root/work/movement-integration/$Candidate/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw "Expected $Candidate"}
 foreach($r in 'host','client'){$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if($s.localGameLogicState-ne1-or!$s.remoteFresh){throw "$r not in a connected gameplay"}}
 Start-Sleep 3;$notes.before=[ordered]@{host=F host;client=F client}
 if($notes.before.host.result-ne'publishing'-or$notes.before.client.result-ne'matched'){throw "Mirror not settled before the flip (host $($notes.before.host.result), client $($notes.before.client.result))"}
 C client 'coop_story_flags flip'
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 250;$c=F client}while($w.ElapsedMilliseconds-lt5000-and!([int64]$c.flips-gt[int64]$notes.before.client.flips-and[int64]$c.writes-gt[int64]$notes.before.client.writes))
 $notes.restoreMs=$w.ElapsedMilliseconds;Start-Sleep 2;$notes.after=[ordered]@{host=F host;client=F client}
 $c=$notes.after.client;$notes.flipped=[int64]$c.flips-[int64]$notes.before.client.flips;$notes.writtenBack=[int64]$c.writes-[int64]$notes.before.client.writes
 if($notes.flipped-ne1){throw "The fixture did not flip a flag ($($notes.flipped))"}
 if($notes.writtenBack-lt1){throw 'The mirror did not write the flipped flag back'}
 if($c.result-ne'matched'-or[int]$c.differing){throw "The partner's flags did not match the host's after the write ($($c.result), differing $($c.differing))"}
}catch{$failure=$_.Exception.Message}
finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()
 if(Test-Path $out){Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope='Story flag mirror write path (1214kk, lab pair, Dunwall fixture): the partner flips her first bool flag; the host value is written back'})}}
Get-Content "$out/result.json";if($failure){throw $failure}
