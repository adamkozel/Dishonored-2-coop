# coop-anything reading notes, slice games-2

Slice: deus-ex, divinity-2-developers-cut (all files; vendored MinHook 1.3.3 skimmed, it is stock upstream), dragon-age-origins, dungeon-keeper, enter-the-gungeon, fallout-new-vegas, far-cry, far-cry-2, gedonia, genesis-alpha-one, ghostrunner, god-of-war-2018, gothic-3 (README, NOTES, OTHER-PROJECTS, RUNTIME-QA-G3TOGETHER). Everything below is what the write-ups say. I ran nothing and confirmed none of it.

## 1. Per game

### Deus Ex (UE1, HX + HXFix) — `deus-ex/README.md`
- **Approach:** HX runs single-player logic (conversations, missions, flags, travel) server-side over UE1 replication. The server is authoritative. HXFix is a subclass package swapped in through mutator hooks. Steam relay on AppID 480. **QA:** automated, 40+ tests with real clients.
- **Bug classes worth knowing:** stale owner pointers crash clients on a map change; an open inventory screen during a map change or exit crashes; a player who is dead at a map change arrives with 0 HP and no gear; notes and door codes are wiped on every map change; any player can drive a conversation; forced conversations knock each other out.
- **QA:** every fix was checked to fail when the fix is switched off. Packet lag and loss are emulated, and one variant runs a client install without HXFix.

### Divinity II DC (Gamebryo, 32-bit) — `NOTES.md`, `stage1/*`, `stage2/*`
- **Approach:** foundation only, with no co-op yet. A `dinput8` proxy hooks `SHGetFolderPathA` with MinHook to give each instance its own profile. **QA:** the isolation layer is automated.
- **Hook by export name:** `SHGetFolderPathA` is hooked by its shell32 export name, not by an exe address, so the hook survives other builds. The target is resolved with `GetProcAddress` and compared with the symbol address ("byProc == bySym MATCH"). This rules out hooking only the DLL's own import thunk (`stage2/dinput8proxy2.c`, `init_hook`).
- **Safe instance token:** redirection is enabled per instance by an environment variable. The token is validated (length, no separators, no `..`), and an invalid token disables redirection.
- **Isolation proof:** `test-hook.ps1` takes a SHA-256 snapshot of the real profile before and after each run (it must show 0 changed files). It reads the exit code to tell a crash (NTSTATUS) from a clean quit (0) and scans WER event 1000. Staggered launches separate an engine race (the main-menu movie NULL deref) from hook faults.
- **Build checks:** the build script verifies the PE machine type, the undecorated export name and the presence of `ret 20`. A stdcall stack imbalance crashed the game 10–40 s after start (NOTES "Toolchain journey").

### Dragon Age: Origins (Eclipse) — `README.md`, `NOTES.md`
- **Approach:** two processes. The host is authoritative for combat. The guest joins by save transfer: the host saves a slot and serves the folder over TCP, and the guest loads it. Both games then agree on every object id. Each player controls a different party member, and there are no spawned puppets. LAN UDP. **QA:** automated, two real clients, scripted suite.
- **Combat arbitration:** the guest's attack order is intercepted and turned into a request. The host validates it (known guest, monotonic request id, both creatures alive, same area, hostile target), issues its own attack order and returns a result. The host repeats the order every ~120 frames so the swing completes. Position mirroring is skipped for the first 30 frames of a swing (NOTES "2026-10-23").
- **Rejoin:** when the host loads another save, it sets a world-reloaded flag and re-serves the save. The guest resets its join state and reloads.
- **Transport bug:** a separate receive call per packet kind made the calls starve each other, and the pending buffer was sized for the smallest packet. The fix is one `ReceiveAny` dispatch loop with the buffer sized for the largest packet (NOTES "later").
- **Shared-avatar bug:** a guest with no assignment controlled the host's Warden, and the mirror kept snapping it back. The guest now takes companion 1 by default, and mirroring waits until the switch is done.
- **Shared event log:** the session event log is shared between both games.

### Dungeon Keeper (KeeperFX, lockstep) — `README.md`
- Several users share one keeper ("archon"), with per-player state moved into per-user state. The QA bot writes the same input packets a human would. Per-turn checksums are the oracle (0 desyncs). Covers loss, retry and hard-kill disconnects.

### Enter the Gungeon (Unity) — `README.md`, `NOTES.md`
- **Approach (Gungeon Online):** the host is authoritative for run state with a shared seed. Enemies are simulated per client. Steam lobby or LAN UDP. **QA:** automated through the mod's own CLI test flags (`--gungeon-online-test host|join`).
- **Desync check:** a floor fingerprint (room count plus a hash) is compared between host and client.
- **Lessons:** never replace the shipped `steam_api64.dll`. Launching the exe directly triggered `SteamAPI_RestartAppIfNecessary`, which started the owner's real install. LocalLow saves are shared across every copy of the game. Protocol version mismatches disconnect.

### Fallout: New Vegas (Gamebryo) — `README.md`, `NOTES.md`
- **NV:MP:** worlds, avatars and combat, but quest actors stay local and each player keeps their own quest log. A quest fails if the other player kills a quest NPC.
- **fnv-coop:** a host-authoritative story layer. **QA:** automated suite. Its harness has a lossy, reordering simulator, seeded chaos fuzzing, a 4-player soak, mutation twins, a flight analyzer, questline sweeps and actor-ownership proofs.
- **Effect capture:** quest stage results are captured on the host as one transaction and applied on clients **without re-running the stage scripts**, so rewards happen once.
- **Encoding pitfall:** Steam VDF files are written in the ANSI code page. Reading them as UTF-8 silently mangles non-ASCII library paths.

### Far Cry (CryEngine 1) — `far-cry/README.md`
- **Approach:** the stock maps run on a listen server. A Lua mod plus an ASI hooks `CryGame.dll`. **QA:** none (the author's manual runs, a Lua monitor and a log diff).
- **Area triggers:** they fire only for `IsMyPlayer()`. A patch runs `UpdatePlayer` for remote players on the server.
- **Snapshots:** writes past the fixed 1124-byte snapshot buffer were dropped silently. The fix sorts entities by priority × age and holds back anything past an 8800-bit budget.
- **Server-only simulation:** dedicated-server AI sleeps until woken. Animation events never tick without rendering, so AI melee keyframes never fire. Cutscenes crash on NULL input or sound subsystems. A checkpoint reads a nil `_localplayer` (fix: use the nearest player).
- **Moves:** the server resends a remote player's move until the player arrives.
- **Tick rate:** `timeBeginPeriod(1)` raised the server from 17 to 35 snapshots/s.

### Far Cry 2 (Dunia) — `README.md`, `NOTES.md`
- **Approach:** a position and heading puppet only. A `dinput8` probe uses its own UDP link plus a Steam relay over a friends-only lobby on AppID 480. **QA:** automated two-copy walk test, plus one manual run on two PCs.
- **Moving a body:** writing the entity matrix does not stick, because the engine recomputes it from the physics body every frame. The body is found by its class vtable and nearest position. A pointer chain was not stable across sessions.
- **Avatar binding bug:** the avatar was spawned at the same spot as the local player, and the body search bound the local body. Only the check "the receiving copy's own player stays put (drift ≤ 0.5 m)" catches this; the "avatar follows" check passed with the bug (README QA, NOTES "Automated two-copy walk test").
- **Shared game over:** an avatar made from the main-character archetype counts as the main character, so its death caused a game over in both copies. The fix sends the engine's invincible event.
- **Steam join:** Join Game on a friend's joinable lobby arrives as callback 333 (GameLobbyJoinRequested), the same callback as an accepted invite.
- **Probe reads:** memory scans copy in 64 KiB chunks with `ReadProcessMemory`, so a decommitted page skips the chunk instead of killing the process.

### Gedonia (Unity) — `gedonia/README.md`
- **Approach:** host plus clients. NPC ownership uses epochs. Story runs as host-numbered function calls. Steam P2P with a lobby, or UDP with its own reliable channel. **QA:** 35 automated two-client scenarios, plus human sessions.
- **Puppet construction:** the player root is cloned under an inactive holder so no Awake runs. It is stripped to transform, animator and renderers, then activated with root motion and animation events off. Appearance code reads the *local* player, so those inputs are set on the puppet first.
- **Enemy ownership:** each enemy has one owner, chosen with epochs and a hold time. `check-ownership.ps1` fails if an enemy is owned twice. An exception on one enemy in an unloaded cell aborted the rest of the ownership message.
- **Loot and world state:** looting wrote "0" into the world DB, and host world-state sync then deleted the body for everyone. Bodies are now per player.
- **XP:** XP multipliers differ per player, so the machine where the kill happened sends the amount it gained.
- **Story:** a transpiler swaps `SendMessage` for a dispatcher. The host numbers story calls, and every machine runs them in that order. Quest digests repair anyone who fell behind. World-DB deltas go to late joiners.
- **Shared dialog:** clicks are votes. One member pays a cost, and the others skip the item removal or get the gold back. Only the leader dispatches the story function.
- **Cutscenes:** a cutscene starts only after every player holds the key. The host starts it for all, and the others are hidden while it plays.
- **Death and generation:** dying in single player reloads, which would turn back the shared world, so co-op uses respawn crystals at measured spots instead. Generation is seeded with stored ids, because DunGen ignored the stored seed.
- **Steam mock:** a mock of Steam over loopback UDP follows Steam's rules (sessions held until accepted, size limits, loss).

### Genesis Alpha One (UE 4.23, UE4SS Lua) — `README.md`, `NOTES.md`
- **Approach:** the listen server is revived by rewriting `NetDriverDefinitions` to IpNetDriver. The guest joins over UE netcode. **QA:** none.
- **Black screen:** the cause was a HUD `FadeOutBorder` left opaque. The method was to rule out fades, world rendering, post-process and view targets one at a time. `HighResShot` (scene only) proved the scene still rendered. Then every full-screen black widget was listed.
- **Unreplicated levels:** runtime level instances are not replicated. Loading them in the same order gives identical names.
- **Guest initialization:** the guest pawn is never initialized (`IsPlayerInitialized=false`, input blocked).
- **Player-0 problem:** forwarded guest actions run single-player Blueprints that call `GetPlayerController(0)` and use the shared HUD. The host must hook and redirect them, an "impersonation window". Both pawns point at one HUD.
- **Message channel:** existing RPCs carry it (`ServerChangeName("COOP|...")` and `ClientMessage`).
- **Multi-instance keys:** UE4SS key binds fire in every instance, so each instance needs a role lock.
- **Hook safety:** a batch of native hooks crashed the game, probably through identical-code folding. Add hooks one at a time, and syntax-check scripts before deploying.

### Ghostrunner (UE 4.26) — `README.md`, `NOTES.md`
- **Status:** research only, QA none. The listen-server path (Model A) is closed. `listen` and `open ?listen` into a loaded standalone world do nothing, and `OpenLevel(...,"listen")` crashes. `open <map>` that bypasses the game's level flow crashes.
- **Puppet route:** `CreatePlayer` adds a second local player, and its pawn can be driven by `K2_SetActorLocation`.
- **Hardlinked test copies:** never overwrite a hardlinked file, because that edits the original install.
- **Input automation:** focus-based input is fragile. Drive the game from in-game state instead.

### God of War 2018 (Kinetica, Lua 5.2) — `README.md`, `NOTES.md`
- **Approach:** asymmetric. The host is Kratos and authoritative. P2 drives the **existing companion** Atreus through his Lua VM, over the game's built-in `socket.core` and `cmsgpack`. **QA:** automated sync suite with mutation twins, live chaos relay, and two instances.
- **Companion takeover:** Atreus's AI actuator is suppressed with an override-until timestamp. Control **pauses while `IsDoingSyncMove()`** (cutscenes, paired moves) so scripted sequences cannot soft-lock. A 35 m leash applies.
- **Commands:** discrete commands are piggybacked 6–8× with sequence dedup. They applied exactly once under bad_wifi (60 % burst loss).
- **Lab hooks:** `GetForegroundWindow` is hooked so a background instance keeps rendering and taking input. The Lua error `__debugbreak` is patched so errors log instead of crashing. Logos are skipped (boot to world in ~12 s).

### Gothic 3 (Genome, G3 Together) — README, NOTES, RUNTIME-QA, OTHER-PROJECTS
- **Approach:** a native SDK DLL with ENet. A host plus clients. **Regional NPC ownership:** the machine that processes an NPC owns it, and the host keeps a ledger of quests, dialogs, story events, item claims, kill counts, the clock and a death journal. Remote players are puppets made from the hero template. The campaign is the host's native save. **QA:** automated, 187 bot runs. Every stage needs 3 passes in a row plus a failing negative control, and invariant monitors run after each suite. A human evening found what the bots had missed.
- **Closest analogue to our project:** a first-person-adjacent RPG with story scripts, dialogs, a single "player" identity, saves and puppets. Details are in section 2.

## 2. Applies to us (D2 host-authority migration)

### Authority model
1. **Pure host authority fails once players split up. Use regional ownership plus a host ledger.** Genome simulates only within ~45 m of the camera. Far entities exist but are frozen, and writes to them do not hold. The owner of an NPC is whichever machine simulates it, and ownership is "sticky" (with hysteresis) to avoid hand-overs mid-fight. The host keeps the ledger: quests, claims, deaths, clock. (gothic-3 NOTES "exploration 1", "outside review 2", "stage E"; README "Who simulates what".) Before assuming the host can own far NPCs, we should measure the Void engine's AI/LOD radius.
2. **Record far results in a journal keyed by stable id and apply them lazily.** Apply each entry when the entity enters local simulation, after a delay of 0.7 s, because entering the processing range resets HP. Never write into frozen copies. (gothic-3 NOTES "exploration 5", "stage C".)
3. **Suppress the partner's world by wrapping the NPC state machine, not by undoing results each frame.** Undoing per frame left one-frame windows and crashed. G3 wrapped all 441 AI states and functions through the script admin's name maps, so mirrors run only idle. Callbacks that are not AI states (OnTick) must be gated separately. A counter of wrapper calls that name no wrapped state should stay at 0. (gothic-3 NOTES "stage 3a"; README "Architecture".)
4. **Script-table wrappers miss direct native callers.** `GetAttitude` has 29 direct callers, and `GiveXP`, `Shoot` and OnTick→`AddHitPoints` are also called directly. Hook the native entry in place after a prologue-byte check, so other builds are refused instead of crashing. (gothic-3 NOTES "stage V1/V2", "P2 arrows", "agent C".) For D2 this means: inventory every caller of each script/native we intercept.
5. **Use one damage choke point, but do not trust it alone.** Hits are decided by the target's owner. Hits caused by puppets or mirrors are dropped. A hit on a puppet goes to that player and is judged against their real body. But death can also be scheduled before the filter (AssessHit, ragdoll writing HP 0, area spells, damage-over-time ticks), so guard those paths too. (gothic-3 README "Damage"; NOTES "hit-path probe", "agent C death attribution".)
6. **A mirrored death must not run the death bookkeeping.** In G3 it doubled quest events and XP. Set the dead *state* directly so the death task only lays the body down, and mark mirrors "defeated by player" so they pay no XP. Rewards come only from the machine where the death happened. (gothic-3 NOTES "stage 3c", "stage C correction".)
7. **Send results, not flags or commands.** Dialog lines are commands run on the machine that holds the dialog. Syncing the `InfoGiven` flag replays nothing. G3 reads the entity's resulting state 0.7 s after a world-changing command and sends that. fnv-coop captures a quest stage's effects as one transaction and applies them without re-running scripts. (gothic-3 NOTES "stage H"; fallout-new-vegas README "How the game works".) This maps directly to our two-world story scripts: the host executes the script, and the partner applies the captured effects.
8. **Partner actions become validated requests.** In DAO the guest's attack order is intercepted and becomes a request with a monotonic id. The host checks the attacker, the target, liveness, the area and hostility, then issues the order itself and returns a result. It re-issues the order until the action completes. (dragon-age-origins NOTES "combat arbitration".) In Genesis the guest's input actions are forwarded and run on the guest's pawn on the host (NOTES "Phase 4c").
9. **Number story calls on the host and run them in the same order everywhere.** Add quest digests to repair a peer that fell behind, and send world-DB deltas to late joiners. (gedonia README "Story".)
10. **Use first-claim-wins transactions for pickups, containers and trade.** If the host refuses a claim, the client undoes the local effect, including later unreported changes. Key containers by template id, not by item name or stack index. (gothic-3 NOTES "stage 5/5b/5c".) Gedonia's pitfall: the host's world-state sync deleted a body that someone else had looted, so bodies are per player (gedonia "Enemies").

### Partner body (Doppelganger rig) and identity
11. **NPCs decide targets by identity with "the player".** A second body is judged by faction tables and ignored. G3 wraps the attitude check so the puppet is judged as the local hero, except for the flee/panic case. In Gedonia, enemies want the "Player" tag, so puppets carry a disabled NPC component. (gothic-3 NOTES "stage V1", "agent A V5"; gedonia "How the game works".)
12. **The puppet must have collision on, or no weapon ever touches it.** Every earlier "NPC hits client" test in G3 had forced the attack. (gothic-3 NOTES "stage H3".)
13. **Before destroying a proxy body, clear every reference to it** (targets, attackers, spell targets) and restart the routines of the NPCs that were after it. Skipping this caused crashes. (gothic-3 NOTES "stage E".) A related risk: a puppet removed right after a kill crashed the dying NPC's states (same section).
14. **Player-0 lookups.** Single-player code resolves "the player" or the HUD via index 0 or a cached pointer, and some AI loops fetch it late (after a timer). Forwarded partner actions need an impersonation window or targeted redirects. (genesis-alpha-one NOTES "Phase 4d/4e"; gothic-3 NOTES "agent C social".) Keep session identity, persistent participant identity and the current visible body as three separate things (same section). Transformation and possession make "the player" a different entity (gothic-3 README "The player is one entity").
15. **Animation replay.** A motion started generically was replaced by the stance idle within 150 ms. What works: replay attacks as the game's own combat-move instruction, play gestures as a play-ani instruction, and drive walking through the movement controller with a goal. **Measure "motion running", not "name on slot".** One fix that only kept the name on the slot hid T-poses. Dead mirrors need an explicit lie-down. (gothic-3 README "Animation"; NOTES "stage V2", "owner's second look", "standing dead orcs".) This feeds directly into our animation checks.
16. **Avoid the shared-entity mistakes.** The partner must never end up driving the host's own avatar (dragon-age-origins "Players bound to one position"; far-cry-2 "avatar body binding"). A puppet built from the main-character template can trigger a game over or block saves (far-cry-2 "Game over"; gothic-3 "more than one hero").
17. **Use the game's existing companion, the asymmetric option.** GoW lets P2 drive Atreus, suppresses AI only while the partner is in control, and yields during sync moves and cutscenes. DAO gives the guest a different party member. (god-of-war-2018 README "What we built"; dragon-age-origins "Chosen approach".)

### Saves, level loads, cutscenes, time
18. **Remove puppets before any save.** A save with a second hero cannot be loaded. Load can activate a new world without the old one being torn down first, so drop all held entity handles on world activate. Guard against loading a nonexistent slot, which hangs. (gothic-3 NOTES "exploration 3", "stage D1".)
19. **The campaign is the host's native save.** The host sends it in CRC-checked chunks, and the client loads it and applies its own per-player record on top. The client stores the transferred save under its own prefix and never overwrites its own saves. Rejoin must also work after the host loads another save. (gothic-3 NOTES "stage D3b"; dragon-age-origins M9.) Check: a restore that merges skills instead of replacing them leaks host progress (gothic-3 NOTES "agent C second").
20. **Map changes are a crash and state-loss hotspot.** Known cases: stale owner pointers, open UI during the change, a dead player arriving with 0 HP, per-player notes wiped. (deus-ex README "What we changed".) Area triggers and checkpoints may assume the local player (far-cry README rows 2 and 7). D2 trigger volumes should be checked for "local player only" logic.
21. **Cutscenes.** Gedonia holds a cutscene until all players hold the key, starts it on the host for everyone and hides the others. GoW yields companion control during sync moves. G3's intro video blocks the per-frame callback, so network timeouts must survive loads and videos (60–180 s), and stale-puppet thresholds must exceed load time. (gedonia "Cutscenes"; gothic-3 NOTES "stage 2", "stage 4c".)
22. **Clock.** The host sends its clock every 5 s, and a client off by more than a threshold is corrected through the game's own setters. A client-side time jump (sleeping) is forwarded to the host, and the host only ever moves time forward. There is a race in which a host update in the same frame undoes the jump. (gothic-3 NOTES "stage G/G2", "clock race".)
23. **Single-player death reload rewinds the shared world.** Use down-and-revive (gothic-3 "stage A/A2", the knock-out task with a partner revive) or respawn points at measured spots (gedonia).

### Transport and Steam
24. **Loads and video stall the network.** ENet's default timeouts dropped a client after about 9 s because the engine loop stopped during the world load and intro video. Comparing two clocks with unsigned arithmetic underflowed, made fresh states look stale and respawned the puppet more than 400 times. Oversized "unreliable" packets were promoted to reliable fragments and held back later snapshots. The fix set MTU 1200 and capped batches at 18 NPCs per message. (gothic-3 NOTES "stage 2", "release-day review".) Fixed snapshot buffers truncated silently, so prioritize by priority × age within a budget (far-cry README row 1).
25. **One receive dispatch loop with the buffer sized for the largest packet** (DAO). An exception while handling one record must not abort the rest of a batch message (gedonia).
26. **Steam:** never replace `steam_api64` (EtG; this matches our rule). Watch `SteamAPI_RestartAppIfNecessary` when launching directly (EtG NOTES "Incident"; GoW hooks it to run two instances). Join Game arrives as callback 333 (far-cry-2 NOTES "2026-10-25 afternoon"). Gedonia's Steam mock follows Steam's session and size rules for one-PC tests.
27. **Install parity at join.** Fingerprint the game DLLs, script DLLs, rules archives and the engine options struct. The host refuses a mismatch and names the part that differs. (gothic-3 NOTES "exploration 4".)

### QA, flight recorder, world diff, chaos
28. **Invariant monitors are a world-diff template.** The rules are `dead_mismatch`, `double_sim`, `foreign_ai`, `drift>5m`, `hp_mismatch` and `no_motion`. A finding counts only if it holds in 3 samples 3 s apart. A self-test cuts the snapshot stream and must report `double_sim`. (gothic-3 RUNTIME-QA "Invariant monitors".)
29. **Pass rule:** 3 green runs in a row, a negative control that switches the feature off and must fail, and the earlier suites still green. Every run also checks a clean shutdown and unchanged real saves. Divinity's SHA snapshot plus crash-event scan follows the same pattern. (gothic-3 RUNTIME-QA "How a suite runs"; divinity `test-hook.ps1`.) Mutation twins: every check has a mutant that it must catch (god-of-war-2018 QA; fallout-new-vegas QA).
30. **The flight recorder should log the caller.** G3's runtime trace hooks the motion start and the task setters and logs the body, the motion or task, and the caller as module+RVA. Static reading could not tell which caller set a task on a mirror; the trace could. (gothic-3 RUNTIME-QA "Runtime trace"; NOTES "rare puppet turn".)
31. **Chaos:** GoW's bad_wifi profile (35±25 ms, 2 % plus 60 % burst loss, reorder, duplicates) proved commands applied exactly once. The G3 review notes that turning off a handler is not loss testing (gothic-3 NOTES "release-day review"). A seeded soak (55 min, 738 actions including loads and saves) also tracked the working set.
32. **Bots miss what the other player sees.** The first human look found that enemies never chose the remote player and that mirrors were T-posing. Suites must assert what is visible (running motion, held item, stance) and *unscripted* AI choices, not only replicated fields. (gothic-3 NOTES "first human look".) Make the negative-control drift check the one that catches a wrong binding (far-cry-2).
33. **When a test fails on game behaviour** (for example an enemy parries), repeat the action the way a player would. Do not loosen the assert. Test-tool actions such as AI goto to arbitrary points or long teleports can crash the game themselves, so attribute faults carefully. (gothic-3 NOTES "agent A Derived", "soak".)
34. **Lab hygiene:**
    - Release the single-instance mutex once the bot answers, and retry early exits (gothic-3).
    - Stagger launches to separate engine races (divinity).
    - Hook `GetForegroundWindow` so a background instance runs, and skip the boot logos (GoW).
    - Global hotkeys fire in every instance, so lock roles per instance (genesis).
    - Take screenshots with PrintWindow of the game window only, never the desktop (gothic-3, far-cry-2).
    - Read probe memory in chunks (far-cry-2).
