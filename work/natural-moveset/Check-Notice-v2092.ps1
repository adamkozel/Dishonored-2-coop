param([Parameter(Mandatory)][string]$Take)
# 2092 (#243): after a notice-eotw take, count the co-op notice lines (COOP: notice shown) in both games' engine logs and
# keep them in <take>/notice-check.json. PASS needs one notice shown on each side (the take sends coop_notice_test to
# each role once). The twin (the previous candidate, without native_notice.h) must show none.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
. "$root/tools/vm/D2LabVM.Common.ps1";. "$root/tools/vm/D2LabVM.Lab.ps1"
$out=Join-Path $PSScriptRoot $Take;if(!(Test-Path $out)){throw "No take $Take"};if(Test-Path "$out/notice-check.json"){throw 'Already checked'}
$hostLines=@(Get-Content "$root/work/lab-instances/host/engine.log" -ErrorAction SilentlyContinue|Where-Object {$_-match'COOP: notice'})
$vmLines=@(Invoke-D2LabVM {$p='C:\D2Lab\ws\work\lab-instances\client\engine.log';if(Test-Path $p){@(Get-Content $p|Where-Object {$_-match'COOP: notice'})}else{@()}})
$shownHost=@($hostLines|Where-Object {$_-match'notice shown'}).Count;$shownVm=@($vmLines|Where-Object {$_-match'notice shown'}).Count
$r=[ordered]@{take=$Take;hostLines=$hostLines;vmLines=$vmLines;shownHost=$shownHost;shownVm=$shownVm;status=$(if($shownHost-eq1-and$shownVm-eq1){'PASS'}else{'FAIL'})}
$r|ConvertTo-Json -Depth 5|Set-Content -LiteralPath "$out/notice-check.json"
$r|ConvertTo-Json -Compress
