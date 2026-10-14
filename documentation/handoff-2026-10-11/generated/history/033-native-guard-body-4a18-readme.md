# Historical note: work/native-guard-body-4A18/README.md

Original: [work/native-guard-body-4A18/README.md](<../../../../work/native-guard-body-4A18/README.md>)

SHA-256: `3A1792552FC56D3FDDE421ECDABF8FDCCB7329E93A11520A33D20650CF468194`; 70398 bytes.

````markdown
# Dishonored 2 co-op lab

This development lab provides a source-built loader, two-instance testing, autonomous input, native Steam movement exchange, a guarded native cutscene skip relay, and a bounded opening rendezvous. The opt-in actor experiment includes walking adapters for Corvo and Emily and a bounded host-to-client pose link for one Dunwall guard. Shared checkpoint and death experiments now coordinate both games through native Steam, restore Corvo/Emily before gameplay, and wait for both loads. These use one pre-staged Dunwall checkpoint. It is **not a playable co-op mod**. Enemies, combat, inventory and story progression still have independent simulation; general cutscene entry and playback timelines are not synchronized.

## Experimental body animation, October 9

Build `28E6FAA0` adds an opt-in body-pose stream over the existing native Steam connection. `coop_probe_body on` maps the actual player's 20-joint body pose onto 17 torso/leg joints in the separately spawned Corvo or Emily. It uses each rig's bind transforms and hierarchy, including the NPC's extra pelvis, and preserves unmapped native arms and bone lengths. The game applies the local transforms through its own joint setter and rebuild routine immediately after native animation evaluation. Native IK runs afterward. Standing/walking use the existing native gait; crouch, crouched walking and airborne states use the streamed body pose.

`Test-D2BodyStream.ps1` and `research/body-stream-verification.json` record both directions, 255 independently compared mapped-joint observations, crouch walking articulation, both jump postures, return to native standing, and isolation of the other counterpart. `Test-D2BodyStreamGuards.ps1 -DisconnectClient` passed disable/re-enable, both source pauses, explicit follow rebinding and actual peer termination. Body ownership expires after 250 ms without fresh usable data. All 14 targeted reports now pass on this build, including both shared reloads, both natural deaths, native walking, original-file integrity and a second body-pose check after resets. Evidence is archived in research/milestones/28e6/body-stream-summary.json; earlier verified builds remain separately attributed.

This is experimental pose replication, not complete player behavior. Corvo's crouch and standing poses were visually reviewed on build `42BEAD3D`; the captured views show a lowered crouch with native idle arms. Emily's full-pose visual review is still outstanding. Weapon and arm actions, combat, powers, damage and shared world authority remain unfinished. Native body/physics alignment remains a separate check; earlier discrepancies near props are retained as unresolved evidence. No additional player-facing UI has been added.

The read-only `--skeleton HANDLE` probe exposes native bind hierarchy, local/global poses and cache phases with lifetime and concurrent-change guards. Synthetic tests cover malformed hierarchies, stale pose allocations, invalid transforms and preservation of unmapped joints. The shorter `coop_body_probe 2000` command is a local, automatically expiring research experiment and refuses to run while the remote body stream is enabled.

## Native health and damage research, October 9

The read-only `--health HANDLE` probe reconstructs current and maximum health from the registered native health component, its declaration, suppression flags and bounded attribute modifiers. NPC observation protocol v2 carries those values through Steam without applying them to the receiving game. Build `CE4BC75E` passed 336 matched packets, independent checks on 12 NPCs, native pause/reload invalidation, body-stream and shared-death regressions. Its four reports are archived in `research/milestones/ce4b/world-health-summary.json`.

Build `81B64DBA` adds `coop_damage_observe 60000`, a bounded local callback recorder. Both natural player fall-death tests passed with the original damage function called exactly once, health falling from 56 to zero, the surviving peer retaining 56 health, and one coordinated checkpoint retry. Damage callbacks run on native worker threads; copied observations are protected separately from engine-thread commands. This recorder applies no remote damage. The original Corvo and Emily Game Over screens were visually reviewed and preserved in `research/screens/native-shared-death-81B6-host.jpg` and `native-shared-death-81B6-client.jpg`.

Build `42BEAD3D` adds a separate nonfatal health experiment to the eight-second single-guard link. Both ends explicitly arm the existing binding; `coop_guard_health 35` then changes only the inactive tutorial guard from its initial 40 health to 35 through the native health setter. The client applies that same value once through its native setter. Protocol v2 includes validated health values, permits neither healing nor fatal values, and retains nonce, identity, freshness, role and lifetime checks. Independent reads verified both changes, unchanged unrelated actors, duplicate suppression, refusal without a binding or from the client, release on pause, and checkpoint restoration to 40. This does not reproduce hit reactions, armor, native death, loot or combat decisions.

Eight targeted reports for `42BEAD3D` are archived in `research/milestones/42be/guard-health-summary.json`, including the guard, world observation, both natural damage/death tests, body poses, restoration and original-file integrity. The body test now records the crouch transition separately before checking stationary joints; the first earlier sample crossed the native transition. Its numeric tolerance is unchanged and the failed raw observations are retained. Native damage reflection also corrected older recorder field labels: the first source handle is the inflictor, the second is the attacker, result offset 4 is mana damage, and offset 8 is protected hit regions. Historical reports retain their original field names.

Research commands share a single mailbox. Use `Send-D2Command.ps1 -WaitConsumed` when issuing commands in quick succession; it waits for the current sequence to be consumed, which prevents a later command from replacing it before the engine reads it. Consumption does not establish that a native action succeeded.

## Native partner perception, October 9

Build `963920C0` adds an opt-in native relationship experiment. The story-character counterparts belong to Loyalists, while the local player belongs to Player; the tested guards initially regard the counterparts as allies. `coop_partner_enemy 8000` changes only the tutorial guard's relationship toward the host's partner for eight seconds. The `escape 8000` variant targets the active escape guard. Both use the native per-entity relationship setter and removal routine. Independent reads verified unrelated relationships, expiry, duplicate refusal, both pauses, follow disable and checkpoint cleanup.

`coop_probe_follow exact` uses the remote player's actual world coordinates. The active escape guard then perceived remote Emily as an enemy, selected her as its combat target, and activated its native attack scheduler in 13 independent samples. One actual sword hit reached Emily's native health callback. Her story-NPC definition ignored that damage; her health stayed at 1. The parallel native player control received real sword damage. This establishes perception and a landed counterpart hit, but no damage relay or shared combat authority.

Twelve reports for this build are archived in `research/milestones/9639/partner-perception-summary.json`. Exact coordinates persisted through both shared reloads and both natural-death retries; native health, body poses and original-file integrity also passed. The read-only `--faction HANDLE` and `--ai-targets HANDLE` probes expose independently validated directional relationships, attention records and combat targets. Native target records can outlive the entity they refer to, so target resolution is reported separately.

Build `52590039` relayed one real escape-guard sword hit from the host's remote Emily to the client's actual player over native Steam. Both sides explicitly arm a 15-second lease with fresh local tokens. The client resolves the authored guard and sword damage declaration locally, builds native damage parameters and result data, and invokes native health handling once. The isolated client lost 15 health; replaying the hit packet caused no extra damage. Nineteen lifetime checks covered duplicate arms, both native pauses, capture disable, expiry, checkpoint replacement and actual client termination. The real-hit test passed again after restart. Five reports are archived in `research/milestones/5259/sword-relay-summary.json`.

Build `ABC00D06` extends the sword relay with a separately armed 60-second mode, capped at twelve ordered native hits. Protocol v2 binds the hit limit to both local arm tokens and rejects duplicates, gaps and mismatched modes. In `sword-death-verification.json`, five real guard hits reduced the client's 56 health to zero through native damage handling. Both native GameOver states and Flash bindings appeared; overlapping retry commands caused one canonical load each, preserved Corvo/Emily, recreated one counterpart each and respected the both-ready gate. The surviving host remained at 56 health. Both relay durations passed 21 lifecycle samples, including mismatched limits, nonrenewing duplicate arms, pause, expiry, capture invalidation, new world identities and an actual client shutdown in the cumulative run.

That fatal test exposed a checkpoint-health bug in native character replacement: the newly created Emily starts at 70 even when the loaded checkpoint contains 56. The role-restoration path now captures the loaded player's validated health before replacement and restores it with the native setter while still in loading state 15. A failed identity, maximum-health or readback check prevents shared gameplay release. The separate loading-character test independently confirmed that checkpoint health survives conversion and gameplay release. The original failed `9677` result is preserved in `work/sword-death-9677`; its completed retry did not pass health equality. This correction covers current health in the tested checkpoint, not complete role-specific inventory, powers or progression preservation.

The test temporarily leaves the counterpart near the host guard while its real player remains in the safe starting room, using the existing offset-follow fixture to exclude local attacks. This is a combat transport experiment, not shared-world authority. The receiving hit has a locally resolved attacker but no weapon inflictor, and uses a body hit region. Full weapon ownership, block/parry behavior, powers, hit-region mapping and campaign combat remain unfinished. Earlier local native tests on `3AE6A684` verified the difficulty's directional scaling: a 20-point sword hit inflicted 15 from the front and 5 from behind in this fixture. The relay preserves incoming direction.

## Active guard and animation experiments, October 9

Build `2935909D` adds `coop_guard_link escape 60000`. One authored escape guard keeps its native host AI, while the corresponding client guard's decision and reflex components hold two native pause references. Root motion follows the host, and the host can perceive and attack remote Emily. A native sword hit reduced client health from 56 to 41; disabling guard ownership also cancelled its dependent perception and damage links. Five reports are archived in `research/milestones/2935/combat-guard-summary.json`. This is authority over one guard, not the surrounding NPCs.

Build `4A1862BB` completed the combined five-hit fatal combat test: both native death screens, one coordinated retry, two released AI references with none lost, preserved Corvo/Emily and checkpoint health 56 on both sides. Checkpoint coordination must arm before the guard experiment acquires AI. Cumulative damage also permits independently observed upward native health changes within the fixture's initial bound and accounts for them separately; the passing fatal run observed none. The earlier `2935` health mismatch has no established cause and is not evidence that regeneration occurred.

The separately enabled `coop_guard_body on` streams the known guard's 223 local joint transforms through native Steam. The client validates the asset rig, binding, world, role, pose freshness and native AI ownership, then calls the native joint setter and hierarchy rebuild after native animation evaluation. In the passing visible test, 270 external observations of the first 27 core joints matched bounded received-pose history, including six native attack samples; the largest compared matrix-element error was below 0.0000002. Source disable, expiry and re-enable were exercised. Offscreen packet reception alone does not prove native animation application because the game culls animation evaluation.

The visible test also recorded six independent local hits from Ramsey and the second escape guard, reducing the observer's health to zero. None came from the controlled replica. This is retained as a concrete shared-world limitation; the report passes only its animation and attribution assertions. It is not a passing normal co-op combat scenario. Weapon draw state, equipment attachments, all face/finger joints, blocking/parrying and additional NPCs remain unverified or unimplemented. The earlier failed observations are preserved in `work/guard-body-*-4A18.json`.

`Test-D2GuardBodyGuards.ps1 -DisconnectHost` passed eight checks: both animation opt-ins, local disable without losing guard ownership, refusal for the inactive tutorial-guard mode, root-pose expiry despite fresh Steam traffic, both pause menus, checkpoint replacement with new world/binding/request identities, and actual host shutdown. Every invalidation scenario first established actual native animation application while the guard was visible. Both owned AI references were released on world or session loss.

## Product requirements

- Two players share the single-player campaign, one as Corvo and one as Emily.
- Cutscene entry, playback/skip, and completion are synchronized.
- Either player's death ends the shared attempt. Both instances show the existing native death screen, restore the same shared checkpoint/world state while preserving Corvo/Emily roles, and resume gameplay only after both finish loading.
- A manual checkpoint reload from either player's existing native menu also reloads both instances to that shared checkpoint, with a single coordinated reset and the same both-ready barrier.
- All player-facing controls reuse actual game UI resources and bindings.
- Development tests run on one PC for now.
- Keep the existing Dunwall checkpoint as the development test area; a new map or a switch to another level is not part of the current work.
- Test games always run windowed, with the lowest graphics presets and startup intro skipping requested.
- Routine testing must run without physical input from the user. Each instance has a separate simulated controller; physical keyboard/mouse still follow window focus.

## Inspected build

- Steam app: 403640.
- Executable version: 1.77.9.0.
- Executable SHA-256: `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`.
- Installed game: `C:\Program Files (x86)\Steam\steamapps\common\Dishonored2`.

The launcher rejects a different executable hash. Version numbers alone are insufficient for future engine hooks.

## Run

From a PowerShell 7 terminal:

```powershell
./Build-D2Lab.ps1
./Start-D2Lab.ps1 -Role host
# Allow the host to initialize before launching the client.
./Start-D2Lab.ps1 -Role client
```

Run these commands from this directory, or use the scripts' absolute paths. The Steam client must already be running and the game must be available to the signed-in user. No Steam replacement or authentication bypass is included.

The launcher stages the executable and root DLL dependencies in the workspace's `work/lab-game` directory. Its `base` junction reuses installed game assets. The junction does not enforce read-only filesystem access; do not edit resources through it. Our loader is placed only in the workspace copy. The original Steam executable is not patched.

Host and client saves/settings live separately in `work/lab-instances/host` and `work/lab-instances/client`. They are seeded from the baseline once, then kept independently. The original baseline was copied to `work/baseline-backup` before testing. Do not start additional games using the original save folder during these tests.

Each test client area is 944×540. On the current 1920×1080 desktop, host/client native positions are X=8/968, Y=40. Observed outer window origins were (7,9) and (967,9), with 946×572 captured dimensions; the windows fit side by side without overlapping. This layout currently assumes that desktop size.

Decals, textures, view distance, models, water, shadows, fog quality, and antialiasing are configured at their minimum preset values. The runtime probe confirmed the main graphics presets at 0 and camera motion blur disabled. Startup skip uses the native `+com_showLoadingScreen 0` argument; a complete fresh-launch intro audit remains outstanding. `r_skipSSAO` resets to 0, so this does not establish that every individual rendering feature is disabled.

The lab policy preserves native window placement instead of the game's automatic centering. It also bypasses the specific focus-loss branch that opens the native pause menu. `win_pauseOnFocusLoss` still reports 1, so that cvar alone does not describe the patched behavior. Both loaded scenes continued while unfocused; background movement was verified directly. Deliberate simulated Start opened the existing native pause menu, and A on its highlighted Resume returned to gameplay, including while the host was inactive. Campaign-wide background and cutscene behavior remain untested.

Close test instances through their own native menus when finished. Relaunching the same live role is rejected.

## Loader behavior and verification

The executable contains `win_allowMultipleInstances`, but its retail startup creates the `Dishonored 2` mutex before normal launch settings are applied. `dinput8.dll` replaces only this executable's imported `CreateMutexA` function and scopes that exact game mutex to the `host` or `client` role. Unrelated mutexes are forwarded unchanged. The exported `DirectInput8Create` forwards to Windows' system DLL, loaded outside `DllMain`.

The optional lab policy checks the executable timestamp, image size, and exact expected instruction bytes before modifying process memory. It changes this build's inactive-input default, focus-loss pause branch, automatic centering calls, and native window creation coordinates. These patches are applied only to the workspace game process, not to the executable on disk. Unsupported instruction bytes fail loading instead of being patched speculatively.

The compiled smoke test verified that the game mutex is scoped, an unrelated mutex is unaffected, and the real DirectInput interface can still be created. It also checked simulated controller packet decoding and rejection of expired, malformed, and excessive input leases.

The read-only runtime probe accepts a game PID:

```powershell
./bin/d2_runtime_probe.exe GAME_PID
./bin/d2_runtime_probe.exe GAME_PID --player
./bin/d2_runtime_probe.exe GAME_PID --input
```

Its private cvar layout and window reference are research for this inspected build, not a supported SDK. It scans for actual cvar name pointers rather than using offsets from a different release. Reading another process may require running the probe at the same integrity level as the game. It does not write game memory or call game functions.

Two workspace processes and native windows were observed concurrently. Runtime reads verified distinct `fs_savepath` values, windowed mode, 944×540 dimensions, and minimum graphics preset values in both. Both isolated baseline checkpoints were loaded through native prompts and menus using simulated controller input, without further physical input from the user.

## Autonomous input and tests

The launcher assigns each process its own `input.bin` file. The loader hooks that executable's `XInputGetState` import and reserves controller index 0 for the simulated controller. Other controller indices forward to Windows. An absent or expired packet produces a connected neutral controller. No virtual controller driver, OS input injection, or new player-facing UI is installed.

The game's joystick acquisition callback normally disables its backend on focus loss. The lab replaces only that joystick callback with one that retains acquisition, allowing the native controller polling path to keep running while unfocused. Keyboard/mouse acquisition and the game's actual focus state remain native.

Developer commands:

```powershell
# Press the native confirm button for a finite interval.
./Send-D2Input.ps1 -Role host -Button A -DurationMs 150
# Turn only the client.
./Send-D2Input.ps1 -Role client -RightX 15000 -DurationMs 400
# Stop controls immediately.
./Send-D2Input.ps1 -Role client -DurationMs 100
# With both instances in the loaded baseline, verify routing and release.
./Test-D2Lab.ps1
```

`Send-D2Input.ps1` supports face buttons, native pause/back buttons, D-pad, both sticks, and triggers. Packets are published atomically with a finite expiry. The game releases held controls automatically even if the test sender stops. Physical controller index 0 is reserved while this test mode is enabled.

Input and developer-command publication retry transient Windows sharing/access errors for at most 150 ms, subject to the packet's earlier expiry. The original expiry and bytes are preserved; retrying never extends a held control. `Test-D2PacketPublication.ps1` verifies temporary and persistent file locks, expiry while blocked, intact prior packets, and temporary-file cleanup. The observed walking regression that encountered a publication failure is retained separately in `research/locomotion-input-publish-failed.json`.

`research/autonomous-input-verification.json` records the successful regression: each instance turned independently, rotation stopped after expiry, and the other instance's player position stayed unchanged. Separate background movement evidence showed the inactive host moving approximately 0.38 game units while the focused client's position stayed unchanged. These checks establish input routing and background simulation in the loaded opening scene; they do not establish multiplayer replication.

## Next feasibility gates

1. Identify the local player/entity APIs and create a second visible, independently controlled character in a small test scene.
2. Replicate position, animation, health, powers, and authoritative interactions over localhost.
3. Synchronize one scripted interaction, one cutscene, and one level transition before expanding campaign coverage.
4. Bind character selection and connection states to native game UI, then add shared-save handling.
5. Expand automated coverage to character choice, powers, transitions, cutscenes, saves, and fresh-launch intro skipping.

The resource scan found `ui/mainmenu.iggyfile`, the Corvo/Emily pause menus, `ui/gamelogic/state_characterchoice`, campaign menu sections, and native notification resources. These are candidates for investigation; discovering their names does not establish that they can be extended safely.

## Current native Steam transport

The original bundled `steam_api64.dll` remains unchanged. The mod uses its existing Steam user/pipe handles and `SteamClient017` generic-interface entry point to obtain `SteamNetworkingSockets012` from the running Steam client. It shares the game's Steam initialization lifecycle; it does not call SteamAPI_Init or SteamAPI_Shutdown inside the game.

The one-PC harness creates a native SteamNetworkingSockets listener on 127.0.0.1:31426 and accepts one loopback peer. It exchanges build-tagged position/orientation snapshots at approximately 30 Hz. This is the actual Steam interface, not a standalone GameNetworkingSockets library or a legacy SteamNetworking fallback. Internet P2P, lobbies, invites, and two-account testing are still outstanding.

`Test-D2Steam.ps1` moves each actual game through its own simulated controller and checks that the other process receives the changed position while its own local player stays still. It rejects stale status files and requires connected, fresh native Steam traffic with advancing counters. `research/steam-transport-verification.json` records the result. This proves transport only, not shared world authority.

The 1,156-byte version 10 pose protocol rejects incompatible builds and protocol versions, own-role packets, nonfinite coordinates, malformed sizes/flags, invalid game-logic IDs, and nonorthogonal/reflected rotation matrices. It carries bounded story-observer and active cinematic segment identities, native sequence time/pause state, and opening rendezvous state. The reliable skip packet is version 3, 96 bytes; the separate opening release is version 1, 96 bytes. Contract smoke tests run during the build. Campaign poses require the native loaded-world state and a campaign map name; menu scenes send no valid pose. Map identity and world epochs invalidate follow mode across map, player, session, or loading transitions. Follow additionally requires both peers' native active state to be InGame with no pending state switch and a verified empty cinematic observer set. Matching map names do not establish matching story progress.

## Native engine and actor experiments

The optional engine bridge runs bounded developer requests on the game's own command thread. Requests have a ten-second expiry and are cleared on launch. Bridge diagnostics go to the isolated `engine.log`; it adds no player-facing UI. The experimental capture of the game's retail logging stub is disabled by default: enabling it caused a reproducible access violation when selecting an empty New Game slot. With capture disabled, the native difficulty menu and campaign start succeeded.

```powershell
./Send-D2Command.ps1 -Role host -Command 'getviewpos'
# Only supported in the inspected opening checkpoint, with its story resources loaded.
./Send-D2Command.ps1 -Role host -Command 'coop_spawn_probe emily'
./Send-D2Command.ps1 -Role client -Command 'coop_spawn_probe corvo'
# Native entity/component transforms follow received Steam poses with a fixed test offset.
./Send-D2Command.ps1 -Role host -Command 'coop_probe_follow on'
./Send-D2Command.ps1 -Role host -Command 'coop_probe_follow off'
./Test-D2Steam.ps1
# Independent external memory checks of cached/render poses and native physics bodies.
./Test-D2VisualProbe.ps1
./Test-D2ActorPose.ps1
# Immediate native collision-body translation and rotation, before the next animation frame.
./Test-D2NativeTransform.ps1
```

The spawn experiment creates a new native arkNPC with a separate handle, using loaded character definitions and native spawner presentation/overrides. It does not repurpose the original story actors. The first SpawnAt-only experiment produced a visible placeholder; presentation and visibility initialization are separate native steps. Native Corvo and Emily models/materials were subsequently observed in the two game windows. Follow mode now uses the native entity-wide transform dispatch, which updates active components, including physics, before updating the actor's cached and rendered pose. It does not replicate full player animation state, health, AI, powers, or interactions. The optional walking adapter is described below. Its fixed offset is for observing the probe in the same-PC test scene and must not become campaign coordinate mapping.

Opening completion can remove both original character NPCs. Probe creation now resolves the exact living character definition from the retained native spawner variation when no original NPC remains, validating owner, declaration types and asset name. It refuses the dead-body variant and requires native gameplay. `Test-D2PostOpeningSpawn.ps1` passed for Emily and Corvo with zero original matching NPCs, preserved spawner handles, 17 native bodies per new avatar and duplicate requests refused. Cached/rendered follow passed again on this build.

**Cached and rendered transform follow passed independent verification in both directions with native Corvo and Emily players.** `Test-D2VisualProbe.ps1` checks cached and rendered actor movement from a separate process. `Test-D2ActorPose.ps1` additionally checks all 17 native Havok bodies, yaw rotation, persistence after follow is stopped, and preservation of the original story actors. The latter passed with the two original Corvo baselines, but the latest Corvo/Emily regression fails its body translation check near props. This remains unresolved; do not treat the physics path as generally verified. Stationary skeleton motion is recorded separately, and the failed rigid-body assumption is preserved in `research/corvo-emily-rigid-body-assumption-failed.json`. Earlier render-only and reference-frame-only failures remain in separate research files. `coop-status.json` remains a pump-time observation and is not accepted as proof by itself. These results establish limited NPC transform replication, not shared world authority.

The bounded `coop_probe_trace_transform` diagnostic samples all native collision bodies immediately before and after the entity-wide native transform dispatch. `Test-D2NativeTransform.ps1` independently calculates the expected world-space translation and rotation for each sampled body. Both directions passed, with 799 body observations and a maximum immediate error around 0.000009 game units. This verifies the native call's immediate effect; animation, collision response, and hit detection over subsequent frames remain unverified. The earlier long-interval rigid-body failures are retained rather than relaxed into passes.

The separate `--bones HANDLE` read-only probe reconstructs current animated body positions from the native animation cache, joint remapping, body binding offsets and entity transform. It reports snapshots whose animation cache or root changed during the read. `Test-D2AnimatedBodies.ps1` passed for both avatars during idle, approximately 1.02 units of Steam-driven translation, and turning: 1,394 body observations from 82 coherent snapshots, with maximum separation 0.013717 game units against a predefined 0.08 tolerance. This checks bodies against their actual changing local animation rather than a rigid skeleton assumption. It does not establish remote animation replication, hit detection, ragdolls or shared physics authority. The earlier rigid-body test remains a separate failed record.

The client was started as Emily through the actual New Game, difficulty, tutorial, and character choice flow, without physical user input. The native `models/characters/player/emily/body.def` and host Corvo definition were independently verified and persisted after reloading their separate saves. This is still two independent campaigns.

`Test-D2WorldGuard.ps1` records a loaded matching-map state, then verifies the client returning through the native main menu: menu poses are suppressed, its world epoch advances and clone reference clears, and host follow stops. `research/world-transition-verification.json` records a pass with the current loader. The native game-logic stack identifies gameplay (1), pause (2), cinematic (5), main menu (6), and character choice (14). The common native session keeps this stack available when the gameplay-only pointer is null during the intro. A separate state guard stops follow and refuses new follow requests outside gameplay, including pending switches. Pause/resume passed with preserved clone identities. The native intro movie runs in MapTransition (15), with Cinematic (5) pending. It bypasses command execution while polling input on another thread. The failed input-based transport fallback was removed. A fingerprint-checked native common screen-update callback now pumps Steam and read-only state sampling only on the established engine thread; entity transforms and developer commands remain in command execution. Body retargeting runs in the owned native animation callback using a finite snapshot published by the engine thread. `Test-D2MovieTransport.ps1` verified advancing traffic in both directions across four independent samples over six seconds. This does not synchronize story progress or cutscene playback.

The developer-only `coop_probe_trace_pose` diagnostic places a three-second hardware data watch on the clone's cached position in this process's threads. It records native writer addresses and bounded stacks, then restores the debug registers. It refuses threads with existing hardware breakpoints. It is inactive unless explicitly requested and adds no game UI. Its purpose is to identify simulation writes that compete with replication.

Read-only entity, component, world, native UI state, and cinematic observer inspection is available through `d2_runtime_probe.exe PID --entities`, `--components HANDLE`, `--world`, `--logic`, and `--cinematic`. Exploratory `--object ADDRESS` reads at most 1024 bytes; fields beyond the actual object size may belong to adjacent allocations and are not established object members. `--kiscule` performs bounded lookup-table inspection for this build. The bridge suppresses menu poses using native world state and map identity. Initial loading should follow native menus; invoking LoadGame before title-screen initialization previously left a loading prompt stuck.

The simulated controller acknowledges native rumble requests for its reserved slot without physical motors; other slots retain Windows behavior. No original save/settings path or installed game assets are edited by these experiments.

## Native locomotion experiment

Starting with version 9, the protocol carries the actual player's physical standing/crouch value and verified default-mode ground, sneak, jump and fall state. These come from validated native player physics and state-machine components, with reciprocal ownership and stable snapshot checks. Unknown classes, non-default modes, loading and pause clear the metadata. `Test-D2PlayerMotion.ps1` observes standing and crouched jumps for both roles, including the short falling phase through accepted-packet counters, landing height and isolation of the other local player. `Test-D2PlayerStance.ps1` separately checks standing/crouch and pause invalidation. These tests verify action metadata. The version 10 body stream and its separate animation tests are described above.

The verified movement build `6EC73DA91716ABD8AF2238ABA261E284195363D7B23D5DE5F1D6D8247D13753C` passed 13 reports, archived in `research/milestones/6ec7/player-motion-summary.json`. They include both shared reload directions, both natural deaths, native pause, actual peer loss, counterpart walking and original-file integrity. The earlier 24-report shared-recovery suite remains separately attributed to build 7623. A development lab is still the current deliverable; shared combat, world authority, campaign support and two-account online sessions remain unfinished.

The owned counterpart now disables its native idle-break autoplay through the existing engine setter while following and restores the changed bit on release. NPC decision pause alone did not prevent scripted idle gestures from overriding locomotion. The original failed Emily idle samples had full native Midnight animation weight despite zero locomotion speed; that failure is retained in `research/locomotion-idle-break-failure.json`. Autoplay control passed the unchanged foot-motion and body-alignment assertions, balanced ownership across resets, and actual peer-loss cleanup. It does not take over the idle-break component's own scheduler references or alter ordinary mission NPCs.

`coop_probe_locomotion on` enables a developer-only walking adapter for the independently spawned avatar in that instance; `off` disables it. It defaults off and requires the existing guarded follow mode. The adapter derives horizontal velocity from fresh Steam position samples, rejects teleport-sized deltas, and feeds the avatar's existing native locomotion graph immediately before animation evaluation. Position, facing and movement request overrides are temporary and restored after each evaluation. No custom animation assets or player-facing UI are added.

The inherited story NPC repeatedly requests a locomotion reset. Supplying velocity alone left the feet almost stationary. Suppressing that reset only during an owned evaluation allowed the native walking cycle to advance. Read-only graph inspection verifies the selected modern graph, its actual consumed movement request, and its output. `d2_runtime_probe.exe PID --animation HANDLE` exposes these bindings and the bounded native declaration lookup; `--bones HANDLE` independently samples the resulting animated skeleton and collision bodies.

`Test-D2Locomotion.ps1` checks both characters during idle, movement in both directions, stopping, and an adapter-disabled movement comparison. It requires measurable local foot excursion, native controller output, actual root movement, stable bone snapshots, and animated-body separation within 0.08 game units. The disabled comparison must still translate the avatar while its gait stays idle. It also checks that input leaves the other local player stationary and that the owned actor identity remains unchanged. Records include the DLL and probe hashes; unavailable and changing bone snapshots are retained rather than counted as passes. The earlier velocity-only experiments remain scratch research and are not passing gait evidence.

`Test-D2LocomotionGuards.ps1` verifies that a native client pause stops both adapters, rejects follow during the pause, and resumes with the same owned actors after explicit follow requests. Its optional `-DisconnectClient` switch deliberately closes the validated isolated client and checks that the host stops applying animation inputs after peer loss. The guard regression passed again with NPC observations enabled on DLL `0EC33CB7215D2EF72F6685F630246A41C13162021372FD86CC8634BB9776331B`. The test handles the intentionally absent process during cleanup.

The walking adapter derives horizontal gait from network movement. The separate body stream adds crouch and airborne poses; powers, combat, hit detection and shared world authority remain unimplemented. The tests establish native graph and skeleton behavior. Desktop capture initially failed with `foreground window did not report a process id` and recovered after restarting the lab pair. Subsequent screenshots show both native models and walking poses, but story NPC upper-body gestures remain. Continuous visual quality is not established by these snapshots. All tests use normal game speed.

## World NPC identity and observation

`coop_world_observe on` enables a developer-only, read-only NPC transport experiment. Each game validates native NPC-to-spawner handles and reciprocal spawner child lists. A match requires one child, a unique authored spawner name in the world, and the exact NPC definition name. Generated NPC names, proximity, and equal process-local handles are not used to establish identity. `d2_runtime_probe.exe PID --world-npcs` independently reads these relationships without injection or game calls.

The host sends version 1, 504-byte NPC observation packets over the same native `SteamNetworkingSockets012` connection as the players. Packets include both sessions, both world epochs, the map, the full authored identity, and a validated position/orientation. The client resolves its own local entity from that identity. World reads run on the engine command thread; NPC samples are sent twice per second. Catalogs are bounded to 128 entries. State dumps at `coop-status.json.world.json` expose exact sent/received samples for testing. This adds no player-facing UI.

`Test-D2WorldObservation.ps1 -ReloadClient` compares sent and received packet sequences, independently checks both native catalogs, verifies that pause clears observations and stops transfer, then reloads the client's disposable slot 81 checkpoint and requires a new world epoch and new observations. The first successful run compared 336 packets with zero pose difference across 48 NPC identities. During reload, old-scope packets are deliberately rejected. The retained failed run records the test harness's initially unhandled native loading continuation prompt; the corrected test sends the existing controller continue action for native state 15.

This is **observation only**. No NPC memory is modified by this subsystem. The source handle marks a native incarnation; reliable birth/despawn negotiation, host-authoritative AI, damage, loot, doors, quests, and client interaction requests remain to be implemented before received world state can safely drive gameplay. Synthetic tests include different local handles, stale handles, duplicate spawner names, multiple children, broken reciprocal links, wrong sessions/epochs, pause and malformed transforms.

Actual client termination also cleared the host catalog and observation entries and stopped further NPC sends after peer freshness expired (`world-disconnect-verification.json`). Both windowed instances were restored afterward. `Test-D2Baseline.ps1` verifies the eight preserved original save/config files, original executable and Steam DLL hashes, absence of an installed proxy, and equality of the built and staged lab proxy.

## Bounded native guard decision control

The developer command `coop_world_hold_ai 2000` temporarily pauses the decision component of the client's uniquely identified `ld_spawner_guard_tutorial_1` guard in the existing Dunwall checkpoint. `coop_world_hold_ai off` cancels it. The command requires connected, fresh, matching gameplay scope, rejects the host role and duplicate holds, and uses the engine's reference-counted pause/resume requests. It never edits component flags or calls scheduler callbacks directly. The two-second lease is released on the native command thread when it expires, when either player leaves gameplay, when the peer disappears, or before a lab `LoadGame` request. World epochs, native actor generations, component ownership, manager identity and instruction fingerprints guard lifetime-sensitive calls.

`d2_runtime_probe.exe PID --ai-decision HANDLE` independently reads the native component pool and active update list. It checks the target's active/inactive membership, pause-reference count, component ownership, behavior-tree identity, list bounds and snapshot consistency. Synthetic tests reject stale handles, wrong owners/types, misaligned pool entries, malformed lists and changing snapshots.

`Test-D2AiDecisionHold.ps1 -ReloadClient -DisconnectHost` passed on loader `46A10BA57DB2561BAB0CA34E50D78792595253B772318CD28C18AE041181D895`: seven acquired holds and seven releases, with no lost identities. The test verifies automatic expiry, no extension from a duplicate request, explicit cancellation, refusal on the host, both native pause menus, client checkpoint reload and fresh rebinding, and actual host-process loss. During the steady gameplay hold, only the selected component leaves the client's scheduler, the peer scheduler stays unchanged, and Steam traffic advances. The disconnect switch intentionally leaves the host closed; both instances were subsequently restored by the lab harness.

The first reload checks assumed that every other component's update-list membership remained fixed as soon as gameplay appeared. Native post-load activity changes those lists, so the two failed records are retained. Whole-list isolation is tested in settled gameplay; transition checks track the owned component's identity, reference count and membership. The control run in `ai-postload-control.json` separately observes post-load scheduler activity with no AI hold applied.

This pauses **AIDecision only**. Reflexes, combat components, motion, scripts and other simulation systems remain native and independent. This particular experiment does not apply host NPC poses, establish complete host AI authority or synchronize damage. The read-only NPC packet remains observation-only.

## Explicitly bound guard pose link

The developer-only `coop_guard_link 8000` experiment links the existing tutorial guard for at most eight seconds. Arm the host first, then the client; `coop_guard_link off` ends the experiment. Both ends must have fresh Steam traffic and matching native Dunwall gameplay. A reliable request/offer/accept/commit/ready exchange binds a new client request nonce, a host binding generation, both process sessions and world epochs, the exact authored identity, and both native actor handles. The client confirms its own incarnation before accepting a commit. Equal handles or proximity never establish identity. This is a separate version 1, 152-byte protocol; read-only NPC observation packets retain their original meaning.

The client acquires one native pause reference on AIDecision and one on AIReflex, waits for both to leave their own update lists, and acknowledges readiness. Host poses then travel at approximately 30 Hz and enter the client's native entity/component transform path. Only that guard's validated reference frame is controlled, with an independent worker-thread expiry. The host's decision and reflex components keep running. The experiment requires the attack component to remain inactive and ends if it activates; combat authority is not implemented.

The link ends on its fixed deadline, explicit cancellation, a 250 ms guard-pose freshness failure, native phase/world/session changes, reload, or peer loss. Duplicate arming does not renew it. Native AI references are released through the engine's reference-counted API; no saved pointer is reused in a new world. The client resumes native simulation at its last replicated pose. Reloading the isolated checkpoints restores the original scenario. The experiment does not preserve or synchronize AI behavior-tree state, animations, health, perception, damage, interactions, quests or saves.

`coop_guard_nudge 1000` is a bounded host-only test stimulus during an active link: it moves the guard 0.35 game units and restores its native transform after one second (or early termination while that incarnation remains valid). `coop_guard_stall 1000` withholds only guard poses to test expiry while ordinary Steam pose traffic continues. These commands add no game UI and do not alter game speed.

`Test-D2GuardLink.ps1 -ReloadClient -DisconnectHost` verifies no mutation without both peers arming, both component pauses and host AI preservation, native cached/rendered transform agreement, animated physics-body alignment, fixed expiry, duplicate refusal, stale-stream cleanup, both native pause menus, reload with a fresh binding/nonce/epoch, and actual host termination. Each result records the exact DLL/probe hashes in `research/guard-link-verification.json`. The initial passing record is retained separately. Combat activation, naturally navigating host AI, other guards, and full campaign authority remain unverified. The movement stimulus is an explicit native transform nudge, not evidence of shared combat.

Loader `31D5221EC8FC5360249FC49F2B28C5D3027973CB44B25B0BC13F55B5C73C3916` passed all eight checks in `research/guard-link-full-verification.json`, including 68 animated-body observations. Maximum root/render transform error was approximately `2.55e-7`; maximum body-position error was `2.56e-5` game units. A separate six-check run with NPC observation and avatar features enabled passed in `guard-link-coexistence-verification.json`. The host's transform nudge is only a controlled movement stimulus.

Two failed harness records are retained: `guard-link-postload-settling-failed.json` shows unrelated AI stopping on both peers before the link starts; the test now observes settled decision/reflex lists and takes a fresh pre-binding baseline. `guard-link-native-resume-queued.json` shows zero owned references with native resume callbacks queued while the pause menu suspends scheduling. Tests require references to be released in the menu and scheduler membership restored after Resume. Neither correction relaxes pose matching, component identity, exact reference ownership, or steady-gameplay scheduler isolation.

Read-only `--ai-reflex HANDLE` and `--ai-attack HANDLE` probes expose their distinct native registries. `--npc-pose HANDLE` reads cached and rendered transforms with a repeated-read consistency check. The existing `--bones HANDLE` probe compares native physics bodies with the current local animated skeleton. Synthetic tests also cover unrelated native handles, stale incarnations, earlier arming nonces, wrong roles/sessions/epochs, invalid pose bases and pre-commit pose refusal.

## Repeatable test areas and campaign coverage

Use the existing checkpoint for short, repeatable co-op scenarios: two character spawns, a single guard, a door or pickup, damage, either-player death, and a coordinated checkpoint reset. Test setup runs through the developer harness and native engine operations, with existing game UI and normal game speed. Each scenario needs a known initial state, explicit completion checks, and restoration of the isolated lab state. Character, single-guard and fixed-fixture shared recovery tests are implemented; arbitrary spawning, shared damage, shared interactions and general checkpoint support remain unfinished.

The read-only installed-resource audit in `research/test-map-inventory.json` parsed all 28 game indexes and found 36 distinct map entity roots. None has a separate test/developer/sandbox path. This filename inventory does not prove that hidden test content is absent or that every listed map can be loaded independently. The shipped `campaign/tutorial/tutorial_p` exists, but its world entity defaults to Emily's tutorial player and sets the Corvo player definition to `NULL`. It cannot yet be treated as a verified two-character fixture. The existing Dunwall checkpoint already supports the separately verified Corvo and Emily test instances and is the first candidate for a controlled area.

The user selected the existing Dunwall checkpoint as the ongoing test area. Continue with bounded, reversible native actor control there; no custom map authoring or tutorial migration is planned. Keep mission testing alongside it as campaign support expands: authored cutscenes and script events refer to mission actors, graph state, resources and trigger ordering. Passing isolated tests cannot establish campaign-wide quest, save/load, streaming or cutscene correctness.

## Native checkpoint and character restoration experiments

The loader observes the registered `LoadGame`, `RestartFromCheckpoint` and `RestartFromMemoryCheckpoint` commands. With shared recovery disabled, it preserves their arguments and original callbacks while releasing owned guard/AI references and invalidating avatar frame control. When shared recovery is explicitly enabled, it intercepts these requests for one coordinated canonical load. `coop-status.json.checkpoint.json` records native command kind, thread, world epoch, logic state and slot. One load can produce additional internal commands; callback count is not a count of player reset requests.

The existing Emily pause menu → Load Game → Yes flow was exercised autonomously. Its `LoadGame 81` callback reached the observer and the native loading screen completed (`research/native-menu-reload-verification.json`, loader `AE9111A8173F524D29136B525564318791129BEC4A47D2D52530F517A9BF7679`). The F9 injection and pause-menu Quick-load action produced no load event in this fixture and are not recorded as passing. The GameOver retry callback has since been exercised by the shared-death tests below.

Loading an unchanged copy of Corvo's checkpoint on the client loads Corvo. Changing only the metadata's character field was rejected by the native save loader; both experiments are retained. A separate native route now works in the Dunwall fixture: RVA `0x8a7650`, called by the shipped character-choice script, updates the native character setting and queues replacement of the local player with the corresponding native definition. No save bytes or metadata are changed by this route.

The developer-only `coop_character_probe emily` / `coop_character_probe corvo` commands require the client role, native engine command thread, ready Dunwall gameplay, validated player/settings/definition identities and instruction bytes, an empty native replacement queue, and no owned avatar, guard, AI or cinematic hold. Duplicate requests and requests from pause or the host are refused. There is no network route to these commands. `d2_runtime_probe --character` independently reads the selected role, actual player definition, available character definitions and replacement queue.

`Test-D2CharacterRestore.ps1` passed on loader `DC5D01BF30EBD254CF0197625F911ED0A08078DFA9A144C9CD25102390975E70`: unchanged host checkpoint → actual Corvo → native Emily conversion → native movement → reverse conversion → restoration of the original Emily checkpoint. Conversion preserved position and all 64 cataloged NPC identities; movement covered about 0.92 game units. The independent entity read also found Emily's body and both arm definitions, and her existing native pause portrait was visually verified. Records: `native-character-restore-verification.json` and `native-character-ui-guards-verification.json`. The first refused run is retained; it exposed an incorrectly placed experiment guard, not a failed native conversion.

The role change replaces the native local player and increments the observed world epoch, invalidating earlier bindings. That original experiment runs after loading. The newer loading experiment below restores the role before gameplay. Later powers/inventory/quests and Steam save transfer remain unverified. Both profiles currently retain a disposable slot 82 containing the same unchanged Corvo checkpoint for shared-recovery tests; slot 81 preserves the independent original lab fixtures.

The client-only `coop_loading_probe 2000` command arms the next Dunwall load for a two-second hold immediately before its native loading-to-gameplay transition. It expires if unused, rejects duplicate arms, and keeps the native completion call stack intact while pumping Steam traffic. It creates no UI. Returning early from the native switch and attempting to commit it on a later frame was rejected after native completion crashes; that implementation is not retained in the loader. `Test-D2LoadingHold.ps1` independently reads the loaded world's state and position, applies expiring movement input, verifies no movement during the hold and continued host/Steam activity, then checks automatic release.

`coop_loading_probe emily` also restores Emily when that next load contains Corvo. At native world phase 3, before logic state 15 changes to Gameplay state 1, it enters the game's own scoped-world context, calls the shipped character-choice API, and runs the complete native entity-maintenance transaction at RVA `0x8ab380`. This includes replacement creation, entity/component initialization, saved-transform restoration and queue cleanup. An earlier phase-2 experiment failed during native cinematic cleanup. Calling only the inner replacement pass at phase 3 was also insufficient: it left the queue live and the new player's transform unfinished. Both failed experiments are preserved in the research records and are absent from the active path.

`Test-D2LoadingCharacter.ps1` passed on loader `CD1F0479923F520C490B0860AEF133927B5F683BF685FDE2A53F45B1B614F412`: the unchanged host checkpoint first loads Corvo without the probe, then loads as Emily with the probe armed. Independent samples see Emily's actual native body and an empty replacement queue while logic remains 15. The replacement preserves position exactly and all 64 cataloged authored NPC identities; Emily then moves about 0.88 game units after release. The original Emily fixture is restored afterward. No checkpoint data or metadata is rewritten. Records: `native-loading-character-verification.json` and `native-loading-hold-verification.json`.

The local two-second loading probe remains a separate explicitly armed experiment. Its native role-restoration path is also used by the shared-recovery protocol below. `coop-status.json.loading.json` records state-switch events and the hold; `.replacement.json` records native queue observations and restoration results. `d2_runtime_probe --loading-world ADDRESS` independently validates and reads a loaded world even while the active-game global is null. Its timestamps distinguish genuine held samples from reads crossing the automatic release boundary.

Native loopback reconnection also handles a killed client whose old Steam connection remains Connected: after five seconds without valid packets, a new loopback connection can replace that silent peer. A fresh peer cannot be displaced. `Test-D2Reconnect.ps1` verifies actual client termination/relaunch, a new native Steam connection, continued traffic, and an unchanged host world. The native Steam DLL remains untouched.

The preceding DC5D01BF build passed seven guard checks including client reload, Steam movement in both directions, NPC observation across reload, walking adapters for both characters, and actual client termination/reconnection. research/verification.json lists the latest build regressions explicitly; individual reports carry their loader hash. The historical eight-check guard record remains attributed to its older build. Restore-D2Lab.ps1 restores the two preserved role-specific slot 81 fixtures and reenables the existing developer experiments. It is an external test helper, not a session-wide reset feature.

## Shared death and checkpoint recovery (fixed Dunwall fixture)

`coop_checkpoint_sync on` opts both lab instances into coordinated recovery; it defaults off. The source is fixed to local SLOT82, whose checkpoint data and metadata must have the same SHA-256 identity on both peers. File sizes are included in the digest. No save data is transferred, and a network packet cannot supply a filesystem path or arbitrary command. The role-specific SLOT81 files are preserved.

A native reload request from either side reaches the host, which arbitrates a single transaction. Reliable Steam messages carry both process sessions, arm nonces, old world epochs, map, checkpoint digest, transaction number and readiness tokens. The mod queues one native `LoadGame 82` per peer. Cold loads permit the engine's asynchronous memory-restart continuation; cached loads can enter directly. Both paths must pass through the observed native loading initializer and finish-load boundary. Engine addresses alone do not identify a new world: the allocator can reuse them.

Before gameplay resumes, the client restores Emily through the native character route. Both native completion call stacks remain held until roles, loaded map, empty replacement queues, checkpoint identity and both readiness tokens agree. Native loading UI stays active and Steam continues pumping. A failed held load completes directly into the existing pause state without an intervening gameplay frame. Mismatched fixture digests are refused before loading. These experiments do not establish equality of every quest, inventory, physics or enemy field.

Natural fall death in either role is detected through the native GameOver state (13). The peer enters the same shipped state on its engine thread, without artificial survivor damage. Both native Flash objects and `_C_ShowGameOver` bindings must be ready before a retry can start. The existing GameOver retry callback feeds the coordinated checkpoint request; overlapping retries produce one load per peer. Native state and movement checks establish that the survivor stops. Both native Game Over screens were subsequently visually verified on `81B64DBA`; see the screenshots in the native-health section above.

`Test-D2SharedCheckpoint.ps1`, `Test-D2CheckpointMismatch.ps1` and `Test-D2CheckpointDisconnect.ps1` cover either request direction, delayed readiness, overlapping native commands, fixture mismatch and peer loss. `Test-D2SharedDeath.ps1` covers natural host/client fall deaths and concurrent native GameOver probe requests; concurrent probes are not a claim of simultaneous natural damage. Loader `C053761233178A7D00228EDA3FD25B84535AF6BAF7B4A4CD80CA839D21022B0C` completed seven consecutive shared transactions in the same processes: four alternating natural deaths, concurrent failure probes, a client reload and overlapping host/client requests. Individual research reports record their exact build; subsequent revisions require new evidence.

The verified shared-recovery build also recreates a counterpart presentation that was already following when the shared session was armed. It waits for both load-completion acknowledgments, spawns through the existing native route, allows normal initialization, and binds follow once. Native entity checks verify one correctly assigned counterpart per world after repeated retries. While following, the owned counterpart holds one native AIDecision pause reference so its independent NPC decisions cannot compete with remote movement. `Test-D2AvatarAi.ps1` independently verifies scheduler exclusion, duplicate-on handling, balanced off/on references, and native pause/resume. No ordinary world NPC is affected by that ownership rule.

Loader `7623D6B9FD668B533B936F1252B94D7EF0C393F1A2A4B108DAA3BF7F6E706FC6` passed both characters' walking, idle, stopped and disabled-adapter comparisons after repeated shared recovery. Earlier failed movement records are retained. The bone probe now checks body positions, phase and offsets as well as matrices for concurrent changes, and excludes the native deferred-physics window: native `0x76a180` can queue new collision transforms while an animation pose is already visible. Numeric alignment and gait tolerances were unchanged. These checks do not establish full player animation parity or sustained release stability.

The same `7623` build passed all 24 required reports in `research/milestones/7623/shared-recovery-summary.json`: both manual reload directions, either natural death, overlapping requests, paused-survivor death, mismatched saves, either peer lost while loading or held, idle-session/death-screen loss, avatar AI reference ownership, locomotion, native loading/role conversion, guarded NPC poses, world observation, reconnect, fixture restoration and original-file integrity. Cold-menu restoration uses the established 400 ms input and native setup wait after one shorter-timing timeout; that failure evidence remains in `work/restoration-timeout-client-*`.

Remaining work includes choosing/transferring campaign checkpoints, role-specific progression and powers, arbitrary loading/scene transitions, sustained fault testing and visual UI checks. Shared combat and general campaign recovery are not complete. Current session setup still uses developer commands; native player-facing hosting and character-selection integration remain outstanding.

## Native cinematic experiments

`coop_cinematic_sync on` enables a developer-only skip relay; `off` disables it. It defaults off. The actual game UI still handles the hold-to-skip prompt and confirmation. The native confirmation callback runs on a UI thread, so the hook preserves that callback and queues only an opaque event. The engine command thread validates the current scene and sends a reliable Steam message. The receiver calls the same original native confirmation function directly, preventing a relay echo. No controller input is sent to the peer.

Matching requires fresh connected Steam traffic, both peers opting in, matching map and broad story scene, matching active Midnight segments, native skip readiness, and the expected sessions and world/cinematic epochs. Invalid or unknown metadata prevents a relay. The movie screen fallback pumps transport and reads state but cannot apply a skip or other engine mutation.

The active player Midnight proxy has two channels. A bounded scan resolves their runtime handles to native instances, validates asset/subscene types and names, and hashes the asset name, subscene name, authored duration, and channel mask. Runtime handles are used only to detect local sequence restarts; they are not shared scene identities. One exact opening exception pairs `player_is_corvo` and `player_is_emily` in `coup_main_post_fight.midnightscene`, duration 28,000 ms, on the inspected Dunwall tower map. Emily also retains a native character-choice observer that Corvo releases; only those exact observed sets are paired. Other names, durations, maps, and observer sets are not normalized.

`Test-D2SequenceIdentity.ps1` independently computes signatures from external native reads and checks their Steam transfer. `Test-D2SharedSkip.ps1` checks a single source UI callback, a single reliable message and peer application, unchanged peer input, no echo, and arrival at character choice, another segment, or gameplay. Version 6 passed intro-to-choice in both directions, Emily-to-Corvo advancement to the taken-away segment, and Corvo-to-Emily completion into gameplay. `Test-D2SkipBoundaryGuard.ps1` passed a deliberate mismatch: both players had the same broad story scene, but different active segments; Corvo's local confirmation sent no skip and left Emily's segment untouched.

The post-choice story contains multiple native segments. An earlier version 5 test incorrectly expected a single early skip to finish the entire chain. Both games advanced to the next segment instead. Segment advancement and completion are now separate test destinations. Research records identify the tested DLL per entry; historical entries without a hash predate that attribution. Earlier movement, physics and phase regressions were run on version 4 and are not automatically reclassified as current-build results.

`coop_cinematic_hold 2000` is a bounded local research command using native Midnight manager pause/resume functions. It targets one validated player sequence and schedules release after two seconds, with world/instance validation before release. It is not a shared entry barrier. `Test-D2CinematicHold.ps1` passed for both Corvo and Emily: nine independent samples per hold saw the same clock frozen in the native paused list, then automatic return to the playing list and advancing time. The other instance and Steam traffic continued. Shared skip regression passed afterward. Audio continuity and other local cinematic tracks need separate verification.

The read-only probe's `--midnight` option exposes active native sequence metadata and raw clock words. Historical records called the field at `arkMidnightScene+0x28` `sceneNodeId`; disassembly established that it is the duration in milliseconds. The current name is `sceneDurationMs`. The signature bytes are unchanged by this naming correction. Clock-word meanings remain research until individually verified.

`coop_cinematic_barrier on` enables an opt-in rendezvous for the exact opening post-choice player segment. Both peers must opt in. Each native player sequence pauses within its first 100 ms for at most five seconds. Once matching Corvo and Emily sequences are both held, the host sends a reliable release and both use native resume. The held pose is sent reliably before the release so the receiver sees the new cinematic epoch first. No new game UI is introduced.

`Test-D2OpeningBarrier.ps1` passed with Emily arriving first and with Corvo arriving first on the corrected version 7 build: five independent frozen-clock samples in each test, followed by three running-clock comparisons with maximum observed differences of 25 ms and 17 ms respectively. The missing-peer test passed: Corvo resumed after the bounded timeout while Emily's native confirmation dialog remained open. These comparisons are sequential external observations, not simultaneous clock measurements or an ongoing drift guarantee. The earlier reverse-order failure and a wrong-character setup failure remain in the research record. The former motivated the reliable pose-before-release ordering fix. Native shared skip and Steam movement were also rerun on the corrected build.

This synchronizes only the observed player sequence at one opening boundary. Audio continuity, other local cinematic tracks, general entry, ongoing clock correction, completion barriers, campaign-wide coverage, world authority, and online Steam lobbies/invites remain outstanding. All synchronization evidence uses normal game speed.

## Research references

- [Dishonored2-Debug](https://github.com/LordRadai/Dishonored2-Debug): inspected for command/cvar and engine-interface research. Its source was not copied into this loader.
- [Void Explorer](https://www.nexusmods.com/dishonored2/mods/30): existing resource inspection/modding tool.
- [ZikQ's co-op beta](https://www.nexusmods.com/dishonored2/mods/222): evidence of a separate early co-op effort; not incorporated into this project.

No game assets, external mod binaries, or third-party mod source should be distributed with this lab source.

The original version 4 cinematic-observer tests remain in `research/native-cinematic-identity-verification.json`. They independently checked up to eight native player script observers using validated Kiscule links, owner names and authored graph-node IDs through gameplay, intro, choice, post-choice, and completion. Those broad observer identities remain part of version 7; the separate Midnight segment identity prevents their reuse across distinct skippable segments.






````
