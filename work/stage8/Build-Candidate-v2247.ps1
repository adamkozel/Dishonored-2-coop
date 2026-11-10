param([string]$Name='progress-v2247')
# 2247 (#281, diagnostic): candidate progress-v2247 = progress-v2246 with failDetail in each door-sync channel: which check stopped it at
# native_door_identity_or_eligibility_changed (institute-keydoor-vm-v2245a: ld_door_200_metal_rich_1_4 stopped on the host).
# 2246 (#281): candidate progress-v2246 = progress-v2244 with the key unlock of a linked door leaf applied while linked doors are
# synced (coop_door_linked): institute-keydoor-vm-v2245a, the partner refused the host's unlock of the Institute key door (native_unlock_refused).
$ErrorActionPreference='Stop'
# 2244 (#263, lab): candidate progress-v2244 = progress-v2240 plus the lab switch coop_saves stress on|off: every co-op saves switch calls
# SaveProfile, clean or not, so one session can repeat the path all 18 stalls took (the watchdog then keeps the stacks).
# 2240 (#263): candidate progress-v2240 = progress-v2239 plus the co-op saves watchdog (coop_saves_watchdog.h): step breadcrumbs
# in the switch, and when the switch service stops for 5 s mid-switch, every thread's stack in <status>.coop-saves-stall.json
# (18 of 672 VM takes stopped in profile_saving with the host's status never written again).
# 2239 (#277): candidate progress-v2239 = progress-v2233 with the probe digest writing each NPC key once from all its NPCs: a pooled
# spare (dormant) counts only when the key has nothing else, one active NPC reads as its state (corpse-possess-eotw-v2238f: the host's
# checkpoint spawners each held the active reinforcement and a pooled spare, and the double write dropped the key).
# 2233 (#277): candidate progress-v2233 = progress-v2232 with npcUnbound in the probe digest (NPC actors that failed the identity read;
# corpse-possess-eotw-v2232y: npcUnread was empty while the host digest still lacked both checkpoint reinforcements).
# 2232 (#277): candidate progress-v2232 = progress-v2231 with the probe digest listing npcUnread (spawners of NPCs whose health it could not
# read; corpse-possess-eotw-v2231x: the host roster had both checkpoint reinforcements alive while its digest had neither).
# 2231 (#277): candidate progress-v2231 = progress-v2229 with packDebug in both roles (actor name, dormant byte, presentSent): in
# corpse-possess-eotw-v2229v the host kept announcing both checkpoint keys while its digest found no NPC under them.
# 2229 (#277): candidate progress-v2229 = progress-v2228 plus packDebug in the NPC stream status (every NPC of a reinforcement key on
# the partner, once a second; corpse-possess-eotw-v2227t kept 2 + 2 of her copies with no own-only removal).
# 2228 (#279): candidate progress-v2228 = progress-v2227 plus linked doors in the door sync (each leaf on its own channel), lab switch
# coop_door_linked on|off, and the lab command coop_lab_door_native <name> open|close (a native transition in this world only).
# 2227 (#277): candidate progress-v2227 = progress-v2226 with the own-only late-spawn test per NPC, not per key (corpse-possess-eotw-v2226t:
# a reinforcement spawner is active for a moment at the load).
# 2226 (#277): candidate progress-v2226 = progress-v2225 with one own-spawn capture per spawn (corpse-possess-eotw-v2224t: 4 spawns,
# 2 captured and removed, 2 copies left).
# 2225 (#269): candidate progress-v2225 = progress-v2224 with the direction crossfade and phase match on any jump over 60 degrees
# (animquality-eotw-v2223c: back to sideways popped a toe 0.5 m).
# 2224 (#277): candidate progress-v2224 = progress-v2223 with the own-only late-spawn test for every key again (corpse-possess-eotw-v2222t:
# without it level NPCs streamed by the host were removed and spawned again).
# 2223 (#269): candidate progress-v2223 = progress-v2222 with the reversal phase match on the walk/run blend the rig plays
# (animquality-eotw-v2221a: 10.3 and 8.0 cm at the two reversals, walk clips only).
# 2222 (#277): candidate progress-v2222 = progress-v2221 plus the own-only rule on keys the host once announced (every copy goes 45 s after
# its last present, whoever spawned it) and its skip counters (corpse-possess-eotw-v2219t: 6 copies left, own-only 0).
# 2221 (#269): candidate progress-v2221 = progress-v2220 plus the reversal phase match: the new clips continue from the phase whose
# pose is nearest the old one, so the crossfade blends similar poses (animquality-eotw-v2218a: a 19 cm toe slide in the crossfade).
# 2220 (#277): candidate progress-v2220 = progress-v2219 plus her dead or unconscious own spawns removed with the host pack when the
# host never reported a death or knockout under the key (corpse-possess-eotw-v2217a).
# 2219 (#277): candidate progress-v2219 = progress-v2218 plus a list of every NPC she spawned for a key, all removed once the host
# stops announcing it (corpse-possess-eotw-v2217t: 7 spawns over 4 keys, only the last per key was tracked).
# 2218 (#269): candidate progress-v2218 = progress-v2217 with the reversal crossfade started by any frame moving over 2 mm against the
# last direction (animquality-eotw-v2216a: serial 0, the flip happens in the slow frames of a reversal).
# 2217 (#277): candidate progress-v2217 = progress-v2216 plus pack NPCs (several under one spawner) in the life roster, so the
# pack counts and the own-only rule see them (corpse-possess-eotw-v2214t); coop_npc_life_packs on|off.
# 2216 (#269): candidate progress-v2216 = progress-v2211 plus the direction crossfade: a reversal of the travel direction blends
# the rig from its last locomotion pose over 0.18 s (animquality-eotw-v2212c: both toes jumped 0.2-0.4 m in one sample); coop_movement_direction_blend on|off.
# 2211 (#244): candidate progress-v2211 = progress-v2210 plus D2SI: the host int story variables mirrored like its bools
# (storyflags/global.kiscule songs_listened differed in corpse-eotw-v2194c, v2202c and corpse-possess-eotw-v2207t).
# 2210 (#277): candidate progress-v2210 = progress-v2209 with the own-only rule on recent host activity only (counts within 60 s,
# a pending host state applied first), present spawn tries 15 s apart, and a spawn_failed try marking her next NPC (v2207t); 1.0.0 version resources.
# 2209 (#277): candidate progress-v2209 = progress-v2208 plus her own spawns the host never had: an active unbound NPC whose key
# the host has not announced for 60 s (no state, no counts) leaves her world (corpse-possess-eotw-v2207a); coop_npc_own_only on|off.
# 2208 (#244): candidate progress-v2208 = progress-v2207 plus the story reflow: her own flow reaching a run the host message already
# applied out of flow runs it again for state-setting classes, so her flow continues (soak-hour-vm-v2206h); coop_kiscule_reflow on|off.
# 2207 (#278): candidate progress-v2207 = progress-v2205 plus corpse possession presence: a partner occupying a corpse (controller 4/5,
# kind 2) holds the host body as a human possession does (paired variant 1000) and counts as occupied (semantic 17); coop_corpse_possession.
# 2205 (#277): candidate progress-v2205 = progress-v2201 plus pack counts: the host sends the alive, unconscious and dead counts of
# a spawner key holding several NPCs (life state 5, reserved2), and the partner brings her unbound copies to them (coop_npc_counts).
# 2201 (#244): candidate progress-v2201 = progress-v2200 plus: her own Continue only when her world cached a next mission start save
# (0x142318198 slot campaign*30+21..29) at its result screen, else the follow at once (campaign-bunker-vm-v2201n). Status cachedSlot.
# 2200 (#261, lab): candidate progress-v2200 = progress-v2199 plus the lab corpse command taking an unconscious body too.
# 2199 (#244): candidate progress-v2199 = progress-v2198 plus: after her own Continue lands on another map the partner loads the host
# mission herself, from gameplay or a result screen, and the product mission follow is quiet for 90 s (one load; campaign-bunker-vm-v2197n).
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
[ordered]@{candidate=$Name;card=281;dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;previousCandidate='progress-v2246';sourceCommit=$commit;sourceDiff='source-change.diff'
 change='progress-v2247: progress-v2246 with failDetail in each door-sync channel status (the check that stopped it). progress-v2246: progress-v2244 with the key unlock of linked door leaves (coop_door_linked). progress-v2244: progress-v2240 plus the lab switch coop_saves stress on|off (SaveProfile on every switch). progress-v2240: progress-v2239 plus the co-op saves watchdog (step breadcrumbs, all thread stacks after a 5 s stop mid-switch). progress-v2239: progress-v2233 with each NPC key written once in the probe digest (pooled spares only when alone; one active NPC as its state). progress-v2233: progress-v2232 with npcUnbound in the probe digest. progress-v2232: progress-v2231 with npcUnread in the probe digest. progress-v2231: progress-v2229 with packDebug in both roles. progress-v2229: progress-v2228 plus packDebug diagnostics. progress-v2228: progress-v2227 plus linked doors in the door sync (coop_door_linked on|off) and coop_lab_door_native. progress-v2227: progress-v2226 with the own-only late-spawn test per NPC. progress-v2226: progress-v2225 with one own-spawn capture per spawn. progress-v2225: progress-v2224 with the direction crossfade on jumps over 60 degrees. progress-v2224: progress-v2223 with the own-only late-spawn test for every key. progress-v2223: progress-v2222 with the reversal phase match on the walk/run blend. progress-v2222: progress-v2221 plus the own-only rule on keys the host once announced, with skip counters. progress-v2221: progress-v2220 plus the reversal phase match. progress-v2220: progress-v2219 plus dead or unconscious own spawns removed too. progress-v2219: progress-v2218 plus every own spawn per key tracked and removed. progress-v2218: progress-v2217 with the reversal crossfade trigger on any moving frame against the last direction. progress-v2217: progress-v2216 plus pack NPCs in the life roster (coop_npc_life_packs on|off). progress-v2216: progress-v2211 plus the direction crossfade on a reversal (coop_movement_direction_blend on|off). progress-v2211: progress-v2210 plus the host int story variables mirrored (D2SI). progress-v2210: progress-v2209 with the own-only rule on recent host activity, present spawn tries 15 s apart and spawn_failed counted as hers, and version 1.0.0. progress-v2209: progress-v2208 plus her own spawns the host never had removed (coop_npc_own_only on|off). progress-v2208: progress-v2207 plus the story reflow for state-setting classes (coop_kiscule_reflow on|off). progress-v2207: progress-v2205 plus corpse possession presence (paired hold and semantic 17; coop_corpse_possession on|off). progress-v2205: progress-v2201 plus pack counts for multi-NPC spawner keys (kNpcLifeCounts, coop_npc_counts on|off). progress-v2201: progress-v2200 plus her own Continue only with a cached next mission start save, else the follow at once. progress-v2200: progress-v2199 plus the lab corpse command on unconscious bodies too. progress-v2199: progress-v2198 plus the decision follow from gameplay or a result screen with the product mission follow quiet for 90 s. progress-v2198: progress-v2197 plus the present spawn fixes (a local corpse counts; her own spawn leaves 45 s after the host present stops). progress-v2197: progress-v2196 plus the host save load (its own save loads; the partner reloads the host mission on the same map; coop_host_save_follow on|off). progress-v2196: progress-v2195 plus the death-ended present (no partner respawn of a dead NPC) and the lab corpse command on the world catalog within 15 m. progress-v2195: progress-v2194 with the other-map load after her own Continue left to the product mission follow (no second campaign load). progress-v2194: progress-v2193 plus the Dreadful Wale audiograph character counterpart (script_kiscule_node_template_1 1407129762 / 288370032). progress-v2193: progress-v2192 with the partner own Continue first, then the host map when it differs (the hold releases at once on another map; coop_continue_own on|off). progress-v2192: progress-v2191 plus the story pickup catch-up with the Heart out. progress-v2191: progress-v2190 plus the partner own Continue when her world finished every mission the host finished, else the host follow (coop_continue_own on|off). progress-v2190: progress-v2189 plus a finished mission of any state (3 or more) the host has, written into a partner world that has it at 0 (The Clockwork Mansion ends at 4). progress-v2189: progress-v2188 plus the lab corpse possession probe and cast (coop_lab_corpse_probe, coop_lab_possess_corpse). progress-v2188: progress-v2180 plus the possessed fish shown as the cosmetic fish (swarm_fish_visual.def, pose bit 0x1000) and the partner Deadly Shade on the host as our deadlyshade_puppet.def, free-standing (coop_shade_native puppet_on|puppet_off); needs the decls of Build-D2PowerVisualAssets-v2187.ps1 in the lab game'}|ConvertTo-Json|Set-Content -LiteralPath "$archive/identity.json" -Encoding utf8
"Built and archived candidate $Name"
