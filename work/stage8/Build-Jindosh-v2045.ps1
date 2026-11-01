$ErrorActionPreference='Stop'
# 2045 (#239, stage 8.4.3): candidate jindosh-v2045 = loadmap-v2041m plus jindosh_mirror.h (one Jindosh lock answer:
# the host's five answer lists and the six riddle/solution journal lists, written raw on the partner). Builds the proxy,
# the runtime probe and the unit tests, runs the action tests (the Jindosh protocol tests are in them), and archives the
# candidate in work/stage8/candidate-jindosh-v2045 (DLL, probe, identity, source diff). Deploying is separate.
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$here=$PSScriptRoot
$cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
$node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
if(!(Test-Path -LiteralPath $cmake)-or!(Test-Path -LiteralPath $node)){throw 'Pinned build tools missing'}
$name='jindosh-v2045';$archive=Join-Path $here "candidate-$name";if(Test-Path $archive){throw "Candidate $name exists"}
& $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_lab_proxy d2_runtime_probe d2_shadow_action_tests d2_world_identity_tests *> "$here/jindosh-build-v2045.log"
if($LASTEXITCODE){Get-Content "$here/jindosh-build-v2045.log" -Tail 40;throw 'Build failed'}
foreach($t in 'd2_shadow_action_tests','d2_world_identity_tests'){& "$root/work/build/Release/$t.exe" *> "$here/jindosh-$t-v2045.log";if($LASTEXITCODE){Get-Content "$here/jindosh-$t-v2045.log" -Tail 20;throw "Test failed: $t"}}
New-Item -ItemType Directory $archive|Out-Null
foreach($f in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item -LiteralPath "$root/work/build/Release/$f" -Destination "$archive/$f"}
Push-Location $root;try{$commit=(git rev-parse HEAD).Trim();git diff HEAD -- outputs/d2-coop-lab/src|Set-Content -LiteralPath "$archive/source-change.diff" -Encoding utf8
 $untracked=@(git ls-files --others --exclude-standard -- outputs/d2-coop-lab/src);foreach($u in $untracked){Add-Content -LiteralPath "$archive/source-change.diff" -Value "--- new file $u" -Encoding utf8;Get-Content -LiteralPath $u|Add-Content -LiteralPath "$archive/source-change.diff" -Encoding utf8}}finally{Pop-Location}
Set-Content -LiteralPath "$archive/source-commit.txt" -Value $commit -Encoding ascii
[ordered]@{candidate=$name;card=239;dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;previousCandidate='loadmap-v2041m';sourceCommit=$commit;sourceDiff='source-change.diff'
 change='loadmap-v2041m plus jindosh_mirror.h: the host sends its Jindosh lock answer (five ListInt lists of jindosh_enigma.kiscule, D2JL every 2 s, at least every 6 s); the partner, once its own roll has finished, writes them raw into the main decl and the five seat copies and rewrites the riddle and solution journal lists (15, 14, 19, 20, 21, 25) in place. It follows the safe-code switch. Protocol unit tests in d2_shadow_action_tests.'}|ConvertTo-Json|Set-Content -LiteralPath "$archive/identity.json"
"Built and archived candidate $name"
