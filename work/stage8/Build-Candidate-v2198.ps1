param([string]$Name='progress-v2198')
$ErrorActionPreference='Stop'
# 2198 (#244, #260): candidate progress-v2198 = progress-v2197 plus: her own active NPC under a key counts in any state (no
# respawn of a bound copy that died), and an NPC she spawned for a host present leaves when the host stops announcing it for 45 s.
# 2197 (#244): candidate progress-v2197 = progress-v2196 plus the host save load: a host Load Game of its own save loads it, and
# the partner on the same map loads the host mission again as a join does (D2MR reserved 1; coop_host_save_follow on|off).
# 2196 (#244, #261): candidate progress-v2196 = progress-v2195 plus: a death or knockout ends the host present for its key (no more
# partner spawns: corpse-eotw-v2194c made three extra guards), and the lab corpse command takes the nearest world corpse within 15 m.
# 2195 (#244): candidate progress-v2195 = progress-v2194, but after her own Continue lands on another map the partner does not
# load the host mission herself: the product mission follow does (soak-hour-vm-v2186b: both queued a campaign load in one frame).
# 2194 (#244): candidate progress-v2194 = progress-v2193 plus the boat audiograph character counterpart (kiscule_action_mirror.h:
# the host hide of the other character audiograph maps to the partner own; campaign-curator-vm-v2191k, -v2190c).
# 2193 (#244): candidate progress-v2193 = progress-v2192 with the partner Continue reworked: her own native Continue first,
# then the host map when the host announces another one (the load-end hold releases at once on another map). Replaces
# v2191 rule (campaign-madinventor-vm-v2191m: a node she had finished was lost by the follow). coop_continue_own on|off.
# 2192 (#244): candidate progress-v2192 = progress-v2191 plus: a story pickup is caught up with the Heart out too, not only
# idle (coin_native_api.h; campaign-curator-vm-v2190c: the note deferred 1009 times while the Corvo partner held the Heart).
# 2191 (#244): candidate progress-v2191 = progress-v2190 plus: at a shared mission end the partner takes her own Continue when
# every mission the host finished is finished in her own world too (it carries the hub state), else follows the host
# (mission_decision.h; campaign-curator-vm-v2181b, campaign-bunker-vm-v2181a). Lab switch coop_continue_own on|off.
# 2190 (#244): candidate progress-v2190 = progress-v2189 plus any finished mission state the host has written into a partner
# world that reached the next mission by following the host (mission_end_mirror.h; campaign-madinventor-vm-v2181b).
# 2189 (#261): candidate progress-v2189 = progress-v2188 plus the lab corpse possession commands coop_lab_corpse_probe and
# coop_lab_possess_corpse (.corpse-possession.json; research-corpse-possession-v2188.md). 2188: progress-v2180 plus
# - the possessed fish: the other world shows the cosmetic fish (models/powers/d2coop/swarm_fish_visual.def, the rat
#   visual's decls on the trout model) at the player's root while she possesses a fish (pose bit 0x1000), as the rat;
# - her Deadly Shade on the host: always our puppet decl (models/powers/d2coop/deadlyshade_puppet.def, the decoy entity
#   with the deadly shadow's spawn, despawn and death scenes), free-standing; lab switch coop_shade_native puppet_on|puppet_off
#   (off: v2079's path, the host's clone definition or the plain decoy).
# The decls are installed in the lab game by outputs/d2-coop-lab/Build-D2PowerVisualAssets-v2187.ps1. Builds the proxy, the
# runtime probe and two test suites into work/build (never bin/), runs the tests and archives work/stage8/candidate-<Name>.
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$here=$PSScriptRoot
$cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
$node='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/node/bin/node.exe'
if(!(Test-Path -LiteralPath $cmake)-or!(Test-Path -LiteralPath $node)){throw 'Pinned build tools missing'}
$archive=Join-Path $here "candidate-$Name";if(Test-Path $archive){throw "Candidate $Name exists"}
& $node "$root/outputs/d2-coop-lab/tooling/run-clean.cjs" $cmake --build "$root/work/build" --config Release --target d2_lab_proxy d2_runtime_probe d2_shadow_action_tests d2_world_identity_tests *> "$here/build-$Name.log"
if($LASTEXITCODE){Get-Content "$here/build-$Name.log" -Tail 40;throw 'Build failed'}
foreach($t in 'd2_shadow_action_tests','d2_world_identity_tests'){& "$root/work/build/Release/$t.exe" *> "$here/$t-$Name.log";if($LASTEXITCODE){Get-Content "$here/$t-$Name.log" -Tail 20;throw "Test failed: $t"}}
New-Item -ItemType Directory $archive|Out-Null
foreach($f in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item -LiteralPath "$root/work/build/Release/$f" -Destination "$archive/$f"}
Push-Location $root;try{$commit=(git rev-parse HEAD).Trim();git diff HEAD -- outputs/d2-coop-lab/src|Set-Content -LiteralPath "$archive/source-change.diff" -Encoding utf8
 $untracked=@(git ls-files --others --exclude-standard -- outputs/d2-coop-lab/src);foreach($u in $untracked){Add-Content -LiteralPath "$archive/source-change.diff" -Value "--- new file $u" -Encoding utf8;Get-Content -LiteralPath $u|Add-Content -LiteralPath "$archive/source-change.diff" -Encoding utf8}}finally{Pop-Location}
Set-Content -LiteralPath "$archive/source-commit.txt" -Value $commit -Encoding ascii
[ordered]@{candidate=$Name;card=244;dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;previousCandidate='progress-v2197';sourceCommit=$commit;sourceDiff='source-change.diff'
 change='progress-v2197 plus the present spawn fixes (a local corpse counts; her own spawn leaves 45 s after the host present stops). progress-v2197: progress-v2196 plus the host save load (its own save loads; the partner reloads the host mission on the same map; coop_host_save_follow on|off). progress-v2196: progress-v2195 plus the death-ended present (no partner respawn of a dead NPC) and the lab corpse command on the world catalog within 15 m. progress-v2195: progress-v2194 with the other-map load after her own Continue left to the product mission follow (no second campaign load). progress-v2194: progress-v2193 plus the Dreadful Wale audiograph character counterpart (script_kiscule_node_template_1 1407129762 / 288370032). progress-v2193: progress-v2192 with the partner own Continue first, then the host map when it differs (the hold releases at once on another map; coop_continue_own on|off). progress-v2192: progress-v2191 plus the story pickup catch-up with the Heart out. progress-v2191: progress-v2190 plus the partner own Continue when her world finished every mission the host finished, else the host follow (coop_continue_own on|off). progress-v2190: progress-v2189 plus a finished mission of any state (3 or more) the host has, written into a partner world that has it at 0 (The Clockwork Mansion ends at 4). progress-v2189: progress-v2188 plus the lab corpse possession probe and cast (coop_lab_corpse_probe, coop_lab_possess_corpse). progress-v2188: progress-v2180 plus the possessed fish shown as the cosmetic fish (swarm_fish_visual.def, pose bit 0x1000) and the partner Deadly Shade on the host as our deadlyshade_puppet.def, free-standing (coop_shade_native puppet_on|puppet_off); needs the decls of Build-D2PowerVisualAssets-v2187.ps1 in the lab game'}|ConvertTo-Json|Set-Content -LiteralPath "$archive/identity.json" -Encoding utf8
"Built and archived candidate $Name"
