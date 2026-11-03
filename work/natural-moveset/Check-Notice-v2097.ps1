param([Parameter(Mandatory)][string]$Take,[Parameter(Mandatory)][int]$HostFrom,[Parameter(Mandatory)][int]$VmFrom,[switch]$Twin)
# 2097 (#243): Check-Notice-v2092.ps1 read the whole engine logs, which are appended across launches, so twin
# notice-eotw-v2092b counted take a's lines (the host log's launch markers show the pair before b's three launches). This
# check counts only the lines after the take began (-HostFrom, -VmFrom: each log's line count before the take, from the
# runner). PASS: one notice shown and answered on each side, none shown before the pause (the route opens it from the
# pause menu, frames step05 and step12). The twin (no notice code) must show none. Writes <take>/notice-check.json.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
. "$root/tools/vm/D2LabVM.Common.ps1";. "$root/tools/vm/D2LabVM.Lab.ps1"
$out=Join-Path $PSScriptRoot $Take;if(!(Test-Path $out)){throw "No take $Take"};if(Test-Path "$out/notice-check.json"){throw 'Already checked'}
$hostLines=@(Get-Content "$root/work/lab-instances/host/engine.log"|Select-Object -Skip $HostFrom|Where-Object {$_-match'COOP: notice'})
$vmLines=@(Invoke-D2LabVM -ArgumentList $VmFrom -ScriptBlock {param($from);$p='C:\D2Lab\ws\work\lab-instances\client\engine.log';if(Test-Path $p){@(Get-Content $p|Select-Object -Skip $from|Where-Object {$_-match'COOP: notice'})}else{@()}})
$count={param($l,$k)@($l|Where-Object {$_-match$k}).Count}
$r=[ordered]@{take=$Take;twin=[bool]$Twin;hostFrom=$HostFrom;vmFrom=$VmFrom;hostLines=$hostLines;vmLines=$vmLines
 shownHost=(& $count $hostLines 'notice shown');answeredHost=(& $count $hostLines 'notice answered');shownVm=(& $count $vmLines 'notice shown');answeredVm=(& $count $vmLines 'notice answered')}
$r.inPause=@($hostLines+$vmLines|Where-Object {$_-match'notice shown'-and$_-notmatch'in logic 2$'}).Count-eq0
$r.status=if($Twin){if($r.shownHost+$r.shownVm-eq0){'RED'}else{'NOT_RED'}}elseif($r.shownHost-eq1-and$r.shownVm-eq1-and$r.answeredHost-eq1-and$r.answeredVm-eq1-and$r.inPause){'PASS'}else{'FAIL'}
$r|ConvertTo-Json -Depth 5|Set-Content -LiteralPath "$out/notice-check.json"
$r|ConvertTo-Json -Compress
