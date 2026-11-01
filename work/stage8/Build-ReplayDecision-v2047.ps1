param([string]$Name='replaydecision-v2047d')
$ErrorActionPreference='Stop'
# 2047 (#253, stage 8): candidate replaydecision-v2047 = jindosh-v2045 plus mission_decision.h (one choice at a shared
# mission end, the host's: its Continue or Replay Mission runs and is sent as D2MD; the partner's own presses wait for it
# and the partner then dispatches the same native command with its own saves). Builds the proxy, the runtime probe and
# the unit tests, runs them (the D2MD protocol tests are in the action tests), and archives the candidate in
# work/stage8/candidate-replaydecision-v2047. Deploying is separate. v2047b adds control hello flag 16 (the host's setting holds for a product partner) and
# d2coop.cfg mission_decision=0; v2047 (without them) was archived and never deployed. v2047c also takes the Replay
# confirmation box (logic 3) as the result screen after a shared mission end (replay-decision-vm-v2047a).
# v2047d also lets the partner's dispatched Replay run the RestartFromMemoryCheckpoint its LoadGame queues
# (replay-decision-vm-v2047d: the partner's was consumed as her own press).
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$here=$PSScriptRoot
$cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
$node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
if(!(Test-Path -LiteralPath $cmake)-or!(Test-Path -LiteralPath $node)){throw 'Pinned build tools missing'}
$name=$Name;$archive=Join-Path $here "candidate-$name";if(Test-Path $archive){throw "Candidate $name exists"}
& $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_lab_proxy d2_runtime_probe d2_shadow_action_tests d2_world_identity_tests *> "$here/replaydecision-build-$Name.log"
if($LASTEXITCODE){Get-Content "$here/replaydecision-build-$Name.log" -Tail 40;throw 'Build failed'}
foreach($t in 'd2_shadow_action_tests','d2_world_identity_tests'){& "$root/work/build/Release/$t.exe" *> "$here/replaydecision-$t-$Name.log";if($LASTEXITCODE){Get-Content "$here/replaydecision-$t-$Name.log" -Tail 20;throw "Test failed: $t"}}
New-Item -ItemType Directory $archive|Out-Null
foreach($f in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item -LiteralPath "$root/work/build/Release/$f" -Destination "$archive/$f"}
Push-Location $root;try{$commit=(git rev-parse HEAD).Trim();git diff HEAD -- outputs/d2-coop-lab/src|Set-Content -LiteralPath "$archive/source-change.diff" -Encoding utf8
 $untracked=@(git ls-files --others --exclude-standard -- outputs/d2-coop-lab/src);foreach($u in $untracked){Add-Content -LiteralPath "$archive/source-change.diff" -Value "--- new file $u" -Encoding utf8;Get-Content -LiteralPath $u|Add-Content -LiteralPath "$archive/source-change.diff" -Encoding utf8}}finally{Pop-Location}
Set-Content -LiteralPath "$archive/source-commit.txt" -Value $commit -Encoding ascii
[ordered]@{candidate=$name;card=253;dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;previousCandidate=$(switch($Name){'replaydecision-v2047c'{'replaydecision-v2047b'}'replaydecision-v2047d'{'replaydecision-v2047c'}default{'jindosh-v2045'}});sourceCommit=$commit;sourceDiff='source-change.diff'
 change='jindosh-v2045 plus mission_decision.h (v2047c: the Replay confirmation box, logic 3, counts as the result screen after a shared end; v2047d: a dispatched Replay also permits the memory continuation its LoadGame queues): on a result screen whose mission end both worlds share, the host decides. Its Continue (RestartFromMemoryCheckpoint) or Replay Mission (LoadGame of its mission-start save) runs and is sent as D2MD with the slot and that save''s map; the connected partner''s own Continue and Replay are consumed until it arrives, and the partner then dispatches the same command, LoadGame of its own mission-start save of that map for a Replay. Both expect the transition for the both-ready hold. A host Replay is no longer consumed as a user reload. The host''s setting holds for both players (control hello flag 16; d2coop.cfg mission_decision=0, lab switch coop_mission_decision on|off in the host''s game). Protocol unit tests in d2_shadow_action_tests.'}|ConvertTo-Json|Set-Content -LiteralPath "$archive/identity.json"
"Built and archived candidate $name"
