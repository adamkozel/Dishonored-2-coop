# coop-anything games slice 3: notes for the D2 host-authority migration

Source: `work/external-references/coop-anything/games/<slug>/{README,NOTES}.md`. Every file in the slice was read in full. All labels are the repo authors' claims, read statically. Nothing was run.

## 1. Per game

**Graveyard Keeper** (Unity Mono, BepInEx/Harmony). Mod: Back From The Grave. Host-authoritative over Steam lobby + P2P, with its own reliable layer (RNET). Each player has their own inventory. QA: automated, 9/9 two-client suite with three upstream fixes.
- Invites that arrive while the game is loading are lost because the UI throws an NRE. The fix gates on "a game is running, or the main menu has been shown once and no loading screen is up". The first version checked "main menu visible now" and broke normal joins.
- Transport-route race: a late hello on native Steam locked the route to native while the joiner had already moved to RNET, so all host traffic was dropped. The host now always consumes the hello, and handling it is idempotent.
- The host checks a client's consumable use against the client's inventory snapshot, but the sync filter dropped the player object from that snapshot, so every use was rejected (issue #22).
- QA builds a Steam mock with Harmony prefixes on the Steamworks.NET calls (localhost TCP). Tests use fixture saves (35 s setup instead of 140 s), assert their preconditions, and include a mutation check that each test fails without its fix.

**Jedi Outcast** (OpenJK source port). The host runs the real game. The joiner runs cgame against placeholder entities filled from snapshots ("dual-load"). UDP. QA: an automated 26/26-map crawl; gameplay checked by hand in split screen.
- "The player is entity 0" is assumed everywhere (`s.number==0`, `g_entities[0]`). Most joiner bugs came from this. The fix is `G_IsCoopPlayer()` on the host and the local clientNum in cgame.
- Level scripts address the player by name (`kyle`) and the last registration wins. Joiners registered as `kyle` and stole scripted events. Fix: player script names always resolve to the host, and joiner task managers run.
- Engine handles are process-local. Movers sent raw sound handles, which mean a different sound on the joiner, so the fix sends the sound set's name instead. The renderer also called server-side traces on a joiner that has no server, so those fall back to the local collision map.
- Some single-player shortcuts affect the whole game: Force Speed changes global `timescale`, and the gamma ramp brightens the whole desktop.
- The crawl harness has a `-SelfTest` that feeds known-bad logs and must FAIL, and `-TwinKillJoiner` that must report FAIL. A cutscene stuck for 20 s is ended by a breaker.

**Little Inferno** (in-house C++, D3D11). Local co-op only, through a winmm.dll proxy. QA: community reports plus scripted screenshot checks.
- The engine already had a dormant second pointer (a touch/Joy-Con leftover). Writing its unbound input channels right after the poll gave a full player 2 with no logic changes.
- The Steam overlay also hooks Present, and patching the swap-chain vtable crashed in the overlay. The mod hooks the game's own render command instead.
- The proxy refuses any exe other than the known build (it checks timestamp and image size).

**LotR Two Towers** (Xbox static recompilation). Single-process couch co-op, no network. QA: automated, 14 scenarios, 73 checks.
- P2 is a "twin" built from P1's placement record during load. Each frame its update runs with P1's player globals swapped to P2's copies (context swap). After that, most of the game works for P2 unchanged.
- The hard part is every "is it the player?" site: hit credit, kill award, pickups, enemy target choice, camera, and scripts that name the hero. The kill award runs in whatever context is current, so it is wrapped by the attacker.
- The game's own setup scripts were replayed for the twin instead of copying fields; copying was "guesswork" and failed. The actor heap only allocates during load, so nothing can be built in play.
- A target field alone did not make enemies attack P2, because their brain scripts act on "the player". Running the enemy's brain messages in P2's context did.
- Health guard: health is raised by 100000 around each hit, then the real damage is applied, so the game never runs its own death for a downed co-op player.
- Each fix has its own `TT_COOP_NO*` off switch, and situations are set up through env test switches.

**Marvel's Spider-Man Remastered**. Closed paid beta (hbgda); nothing known or tested. Free roam works, but story missions "not built for co-op" have bugs, crashes and broken cutscenes, which is the cost of skipping story authority.

**MDK2 HD** (BioWare in-house, Lua 3.1 plus a dinput8 proxy). Host-authoritative enemies, spawns, damage and checkpoints. Remote players are puppet gobs. UDP or Steam friends-only lobby, up to 16 players with the host relaying. QA: automated, 31 gates, all 96 checkpoints with host and client, mutants.
- Every level load carries an **epoch**. Player packets carry level plus load count, and states from before the partner's load are ignored. The epoch is adopted only when the load completes; eager adoption was a caught bug.
- Clients hold incoming events until their own load finishes. Events a client sends while loading carry the old epoch and are dropped, so it sends from the next tick. The same checkpoint number does not mean the same world: a host reset to the same checkpoint has to force a reload.
- Rooms in stasis run `OnCreate` when they wake. The host woke rooms by camera and the client by body, so items spawned on one side only. Fix: every machine wakes rooms around every player by the same rule.
- Mirrored gobs and puppets skip their class think **and** their anim-loop handler. Copied animations otherwise fire side effects, for example Max's handler deleting a held gob (a crash) or duplicate projectile launches. The client ignores local damage to its own player and takes only the host's HURT.
- Projectiles: capture the finished launch state (velocity, fuse, owner), not the creation arguments. Replay without calling the shooter's scripts.
- `gob_delete` only marks the gob, so a same-name respawn in the same frame bound to the dying gob. Fix: despawn first, then treat deleted gobs as gone for the rest of the frame. Info-less sub-gobs share names and must be ignored. Scripted deletes of host-owned enemies are wrapped to skip gobs that are already gone.
- `coop_state` dumps plus `statecheck.py` (a host/client diff) found desyncs the per-feature checks missed. Dumps taken right after a load are not trusted, and a difference that is gone 1.5 s later counts as transient. NaN bypasses distance thresholds, so the scorer must also fail on missing rows.
- Cutscenes: the same scripts run everywhere, so the host only sends the start (paused-script barrier), the result of the skip vote, and the end barrier (wrapped `Done`). An early GO that arrived before the local start skipped the clock init and caused Lua errors. Esc called `StopScript` directly, bypassing the vote. Triggered movies are wrapped so the host announces them.
- Bandwidth: sending only on change took host upload from 27 to 1.3 Mbit/s. Clients accept packets only from their configured host.

**MiSide** (Unity IL2CPP). The host leads and story *events* are relayed (not state). Remote players appear as the main-character body. Steam/LAN, 2-4 players. QA: automated, 21 tours, host migration, a 120 ms run.
- Before syncing, the team **inventoried the whole game**: every script, dialogue line, choice and timeline across 19 chapters went into SYNC-INVENTORY.md with 0 unclassified. Each row says who owns it and which tour proves it.
- A cutscene a client starts becomes a request to the host; the host starts it on its own clock. The client ran 0.13 s from the host.
- A cutscene can be "playing" without having been started (a timeline that plays on awake). "Already playing" counts only if `Play` really ran on that machine. Starts and presses are retried for 5 s, matched by key with the object name as fallback, and only presses that worked on the sender are shared.
- The host reports every animation its story starts on its own player. A client that has not started the same one within 1 s plays it.
- Held items follow a rule read from the game's interaction data, not a per-scene list. The first version watched the wrong object and earlier passes were false; the check must look at what is really under the hand bone.
- A copy of the first-person body has `receiveShadows` off, so the puppet looked flat and over-lit. A renderer dump (`MT-LOOK`) found it.

**Modern Warfare 2** (IW4 x64, winmm proxy). The game's own Spec Ops co-op netcode, which is **lockstep**: both machines run the level scripts. QA: automated two-copy scenarios plus a 19-level tour; parts of the campaign played on two PCs.
- Lockstep pitfalls: builtins that work only for the local player failed on one machine only (silent drift). `getdvar` is filtered in co-op, and script-set dvars read as "". Retail `Scr_Error` drops its message, so the DLL logs `file::function +offset` GSC call stacks.
- The game's scripts are never shipped. Patches are applied to the zone text at load (`@@@ replace/append/subst <function>`), produced by generator scripts. Each changed script's hash is logged on both machines.
- A per-player rework of single-player set pieces turns `level.player` into the acting player per function, adds per-player flags fed by the same trigger volumes, and reads usercmd buttons instead of local `ButtonPressed`. Patterns for scenes that only work for one player: the first to act plays the scene and the other is "stunned" with a black overlay or a ground camera; a named NPC gives up his seat.
- Handing a turret between players in one server frame crashed both machines; spread over 3 frames it works. Weapon sync took a climber's ice picks, which needed a per-player own-weapons exception.
- QA bots press real binds through the engine key path. A session recorder's logs from both machines matched line for line. A spawn and tour audit of every start point found players inside walls, frozen by intros that held only player 1, and unreachable starts.
- Lab hazard: Steam relaunched the real install from a test copy, so test copies are started with `SteamAppId`/`SteamGameId` env vars. A 12 s lobby drop was an artifact of loopback on one PC.

**Modern Warfare 3** (IW5). Reverse-engineering stage only; no co-op test has run. Lessons: the 2026 64-bit update silently broke every 32-bit tool, so check pointer width first. The campaign has about 23.8k script flags as the story state to sync.

**NieR:Automata** (Platinum in-house, AutomataMP/ENet fork). The host's story graph drives the client's. QA: automated two-copy scenarios (commander, hangar, world, midjoin).
- Writing the visible "story step" global did nothing, because the game re-sets it every frame from the active graph action. The mirror has to be driven at its source.
- The **StoryActionSync** design: hook the action-state setter to learn the graphs. The host scans for actions that have just finished and sends (phase, graph hash, action id, state hash). The client applies a state only once its own graph has started that action, with a 0.5 s grace. The client then plays its own lines, cutscene and phase change with no knowledge of what each action checks.
- The client's own phase-change request (a flag) is **held** and released when the host's phase arrives (0.04 s after it).
- Personal graphs (shops, chests, vending machines, achievements, side-quest givers) are kept local, filtered by name. A whitelist of "main quest" graphs would have broken the flight scene, which is driven by event and enemy-set graphs.
- A mid-join client receives every action finished so far in the phase.
- Tools: in-process hardware breakpoints (`SetThreadContext` on all threads, a VEH handler, `RtlVirtualUnwind`) and snapshot diffs of writable sections. Graph state is changed only on the game thread. Test copies get no mouse (a hovered window changed menu picks) and are kept away from the NVIDIA App, which wrote the owner's saves.

**Night in the Woods** (Unity Mono). One shared Mae; the host is authoritative over scene, position and the numeric story-variable dictionary. QA: none (one-off checks).
- Story variables travel in UDP fragments and are applied only from a complete snapshot. A whole-dictionary hash is compared across machines.
- Dialogue choices carry an option-session id and a commit id, so a stale commit is never applied to a new menu.
- A received packet is not an applied state: the guest status needed a `lastAppliedChoiceCommitId` field to prove application.

**Obscure** (Hydravision, xinput1_3 proxy). Host-authoritative mirroring. The remote player is the game's own local player 2, through a virtual pad. Steam P2P on the game's own appid, friends only. QA: automated up to 1.0, later builds by playtest.
- The guest's `Update` is patched to apply the host's requested state before the game's state machine runs, and the host's position after it.
- One-frame requested states (a swing lasts one frame) are recorded on the host with a sequence number and the last 4 values, then replayed in order. Only a whitelist of safe states is mirrored; a door state sent without its object hung the guest.
- Forcing a door state every frame caused 41,509 swallowed level changes. The state is now forced once per door use.
- After the game's death swap of player slots, the host follows the joined real-player **object**, not the slot (this fixed a crash).
- Movement is camera-relative. The guest sends its own camera axis, and the host swaps it in only for the duration of the guest's update.
- Smoothing: snapshots are stamped with host time and shown at an adaptive delay (p90 jitter plus one send interval) with a fixed 60 Hz send tick. Per-frame sends had produced about 600 packets/s.
- Breakables and props sync as "which index is broken" and call the game's own break or setter function. A texture swap keyed by address drew letters on the ground once the address was reused. A watchdog logs where the main loop stalls for 5 s.

**Ocarina of Time** (Ship of Harkinian, Anchor). Flags are synced and authority is per room ("roommaster"). QA: compile only.
- A synced flag did not update a switch actor that was already open, because the actor never re-reads it. The fix re-runs the actor's own state transition.
- Freezing non-authority actors has side effects; water-level actors never moved.
- Orphaned cutscenes are released by a watchdog.

**Outlast** (UE3, OpenOL plus a relay). Peer-to-peer mirroring. Triggers sync by Kismet path; each client owns the enemies it spawns. QA: automated, 67 PASS and 4 XFAIL.
- Any trigger anyone touches is force-activated on every client, wherever the player is. That advanced the story **and saved a checkpoint** for a player in another room. A relay trigger radius mitigates it.
- **Duplicated map enemies**: each client runs its own AI copy, and both attack the same player for double damage. Hiding one copy at the relay was rejected because the copy's AI keeps dealing damage, leaving an invisible attacker. A native one-owner rule (lower player id keeps the enemy) fixed it, but within 150 units it merged two genuinely different enemies.
- The late-join snapshot queue was never drained (a missing call in the CLI loop). `IsTickableWhenPaused=false`, so a paused game stopped heartbeats. A minimized window stalled init. Logs need `-FORCELOGFLUSH`.
- QA: XFAIL encodes a known bug, and XPASS fails the run so the expectation must be flipped. Mutation twins guard the door and trigger tests. A "regression" turned out to be a harness bug, found by reading packets. `census.py` reads enemy lists from memory. A scene's "missing enemy" was a timing artifact of the test.

**Poppy Playtime** (UE4.27, UE4SS Lua plus a Python relay). Puppet movement only. QA: automated movement test.
- Every UObject mutation must go through `ExecuteInGameThread`. Off-thread writes died minutes later with no report. The state file's mtime is the tick heartbeat for detecting a blocked thread.
- The procedural AnimBP casts to the owning pawn, so it can't run on a puppet; a single-node walk sequence is used instead. The puppet must copy the component scale (14.37), or it renders tiny.
- A first-person mesh reports `WasRecentlyRendered=false`, so render checks on it mean nothing. Pixel diffs are below the scene's own noise floor; the animation clock serves as the oracle.

## 2. Applies to us (D2 host-authority migration)

**Story and scripts (the core of the migration)**
1. Drive the partner's story at the source: send action/graph completion ids, not mirrored globals. Apply an id only after the partner's own graph has reached that action, with a grace period. (NieR NOTES "2026-10-19 evening", "2026-10-20")
2. Hold the partner's own level/phase-transition request and release it when the host's arrives. This is a cheap, generic gate for D2 mission-section and level loads. (NieR NOTES "2026-10-20: holding the client's phase change")
3. Classify story graphs or scripts as shared vs personal (shops, chests, pickups, achievements) and keep the personal ones local. Use a deny-list, because a "main quest only" allow-list missed event and enemy-set graphs. (NieR "2026-10-20")
4. Before building sync, inventory every script, trigger, dialogue line and cutscene into one table with an owner and a proving test, and keep 0 unclassified. This is the companion to our world-diff work. (MiSide README "Inventory the whole game")
5. Triggers touched by the partner must become host requests that the host gates by context. Outlast's blind replication skipped story ahead and saved checkpoints for a far player. (Outlast README problem 4; NOTES test run 7)
6. A flag change that arrives after an actor has latched its state must re-run the actor's own transition; applying the flag is not enough. This matters for D2 doors, levers and safes. (OoT README "What we changed")
7. Identity hazards: code and scripts that assume "the player" (entity 0, a named hero, `level.player`) or process-local handles (sound/asset ids). Audit every "is it the player?" site. Send names or stable keys, never raw handles. (Jedi Outcast "How the game works"; LotR NOTES §5; MW2 NOTES "Ice climb" design)

**Ownership, enemies, damage**
8. Exactly one simulator per enemy. Hiding a duplicate does not work if its AI still runs and deals damage. A distance-based dedupe falsely merges real neighbours, so use deterministic ids. (Outlast README problem 5; NOTES test runs 12 and 24)
9. On the partner, mirrored actors must skip their think **and** their animation-event handlers. Copied animations otherwise fire gameplay side effects: deletes, launches, damage. The partner takes damage only from the host's HURT. (MDK2 NOTES "Session 1h", "Fifth manual session", "Projectile launch replication")
10. Projectiles: replicate the finished launch state, not the spawn call. (MDK2 "Projectile launch replication")
11. Running AI "in the target player's context" can be what makes enemies actually engage a second player. A target field alone may not be enough. (LotR NOTES §6.3)

**Loads, epochs, saves, late join**
12. Tag every message with a level/load epoch. Adopt the epoch only when the load completes, queue events until then, and treat "same checkpoint number" as different worlds after a reset. (MDK2 NOTES "Session 2c", "Epoch regression")
13. Wake the world around every player by one shared rule. D2 streaming and AI LOD may fire on-create/spawn logic on one side only. (MDK2 "Session 2b" 12-6)
14. Late join replays everything finished so far in the phase. Verify that the snapshot queue is actually sent. (NieR "midjoin"; Outlast problem 7)
15. Save/load: detect the host's state-machine load request and send the save, then have the guest wait for the file. Isolate the partner's saves from the real ones. (Obscure README "Loading a save"; MiSide "Saves")

**Puppet, animation, input**
16. Apply the host's requested-state events before the partner's state machine and the transform after it. Record one-frame inputs with a sequence number and replay them in order. Mirror only a whitelist of safe states. (Obscure NOTES §5)
17. Force a world object's state once per use, never every frame. (Obscure README "Door loop")
18. Track the player object, not a slot index, across slot swaps on death or revive. (Obscure "Deaths")
19. A copy of a first-person body needs its first-person-only render flags undone (shadows). The first-person mesh is never "rendered", so it is a useless oracle. Use the animation clock or state numbers, not pixels. (MiSide "shadows"; Poppy NOTES fifth slice)
20. Host-reported animations: if the partner's story did not start the same animation on its player within 1 s, play it. (MiSide "A cutscene can be playing without having been started")
21. Engine mutations happen only on the game thread, with a heartbeat to detect a blocked hook. (Poppy fifth slice; NieR "Pitfalls")

**Cutscenes**
22. A partner-side cutscene start is a request: the host starts it on its own clock, with a skip vote and an end barrier. Guard against an early GO that arrives before the local start, and against engine skip keys (Esc) that bypass the vote. (MiSide; MDK2 "cutscenes together", "Esc skipped", "Deterministic movie-race")
23. Add an orphaned-cutscene watchdog. (OoT; Jedi Outcast stuck-cutscene breaker)

**Diagnostics and QA**
24. Per-tick world-state dumps on both machines, diffed by a scorer that ignores load windows and transients under 1.5 s, rejects NaN, and fails on missing rows. (MDK2 "Session 2a", "regression ladder")
25. Session recorder on both machines; line-for-line equality is the desync test. (MW2 "Playtest 2026-10-23")
26. Mutation twins and XFAIL/XPASS discipline, scorer self-tests fed known-bad logs, and per-fix off switches. (Outlast QA; Jedi Outcast `-SelfTest`; LotR §9; MDK2 MUTANT)
27. A received packet is not an applied state: expose "last applied id" in status. (Night in the Woods "v0.6.4")
28. In-process hardware breakpoints plus snapshot diffs of writable sections find writers without a debugger. A thread that dumps all stacks on a trigger file debugs hangs. (NieR "route and Chapter Select"; MW2 "v0.5.0-beta")
29. Spread handovers of linked or used objects (turrets, vehicles) over several frames. (MW2 README "Turrets")

**Transport and lab hygiene**
30. Make hello/handshake handling idempotent and route-agnostic. Gate invites until the UI is ready, retrying rather than dropping. Accept packets only from the session peer. (Graveyard Keeper B1/B2; MDK2 "Session 2e"; MW2 v0.5.0)
31. Use a fixed send tick, send only on change, and smooth with host-timestamped snapshots at an adaptive delay. (Obscure "Smoothing"; MDK2 "network traffic")
32. Lab hazards: Steam relaunches the real install (use `SteamAppId` env), the NVIDIA App writes real saves for new install dirs, Steam Cloud uploads on exit, loopback-only lobby artifacts, `PreferSystem32Images` bypasses proxy DLLs, a paused game stops ticking, kills must be path-scoped, and running scripts must never be edited. (MW2 NOTES "Phase 2/3"; NieR "isolated test copies"; MDK2 "2026-10-22"; Outlast NOTES test run 2)
