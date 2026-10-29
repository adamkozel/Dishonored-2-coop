# coop-anything slice games-1: notes for the D2 co-op host-authority migration

Read in full: games/autonauts, baldis-basics-plus, call-of-duty, celeste, clive-barkers-jericho, code-vein,
command-and-conquer-3 (incl. mission-survey.md), crime-scene-cleaner, crimson-desert (incl. tools/ as text),
crysis, cyberpunk-2077, dark-messiah-of-might-and-magic, dave-the-diver, dead-cells,
deep-rock-galactic-survivor (README, NOTES, STATUS, notes/01-08). Paths below are relative to `games/`.

## 1. Per game

### Autonauts (Unity 2018 Mono, BepInEx/Harmony) — automated, beta, one real 2-PC Steam session
Host-authoritative; joiner sim frozen and mirrors host; joiner's FarmerPlayer is a real game object; TCP and
Steam ISteamNetworkingMessages via the game's bundled Steamworks.NET.
- Join = host `SaveJSON.Capture` -> gzip -> joiner loads it through the game's own loader, served from memory under a reserved save name (`README.md` "How the game works").
- Deltas are the game's own per-object save JSON; anti-entropy digest of (uid, hash) every 10 s, joiner re-requests drift; volatile fields (state timers, path rotation) excluded from hashes; hash after a parse round trip (`NOTES.md` 0.1.0, 0.9.0, 0.10.0).
- Joiner's local UID counter moved to 1e9; any savable object in the local range is a simulation side effect and is swept every 0.5 s (`NOTES.md` 0.3.0, 0.11).
- Joiner input forwarded at choke points and replayed on the host; host redirects building menus to the engaging peer; host buffers the latest click 3 s when the action is not abortable (`NOTES.md` 0.3.0, 0.6.0).
- In-place reload traps: `Load` re-runs state transitions (Converter re-fires `EndConverting`), screens holding a rebuilt object lose their disengage, floats lose precision per round trip, test early on a real late-game save (`NOTES.md` 0.11-0.12.37).
- Peer timeout 60 s because a world load freezes the main thread; flush before closing TCP (`NOTES.md` 0.2.0, 0.10.0).

### Baldi's Basics Plus (Unity Mono) — automated with mutants, alpha
Host-authoritative; length-prefixed TCP frames with version+layout-hash handshake; Steam via the game's own Steamworks (no Spacewar).
- Keepalive from the socket writer thread, not `Update`; "host not responding" (5 s) separate from "host gone" (60 s); queues capped by bytes (README "Link liveness").
- Seeded TCP chaos proxy (delay/jitter, Gilbert-Elliott loss as retransmission stall, bandwidth, stall/cut control port) (README "A TCP chaos proxy").
- Only lobby members may connect; hold an unknown caller up to 5 s for the member list (README "Only lobby members").
- One-account Steam tests: `SteamAppId` env var, `CreateSocketPair`, a real hidden `bt=lab` lobby; found "local close posts no callback -> ghost peer" (README "Testing Steam with one account").
- Random events: the host's schedule only; clients block their own timers, run the event's `Begin` for per-player effects with the end timer disabled so only the host's End stops it (README "Random events").

### Call of Duty 1/UO (IW1, id Tech 3) — research only
- SP exe keeps a 4-slot Q3 server; transport cut to loopback; `SV_SendClientMessages` sends to client 0 only (NOTES Phase 3 pass 2).
- Plan: keep the host as `level.player`; give partners another classname so `getent("player","classname")` still returns one entity; patch partner in script by script; any player counts for triggers (NOTES "Plan for player 2").
- Per-map count table of `level.player` refs, trigger waits, follow-player, vehicle words to size and order the work (NOTES "Per-map counts").

### Celeste (FNA, Everest) — community reports, docs only
- CelesteNet shares space, not story; shared goals only via purpose-built synced entities (README "Known issues"). Copied install needs `steam_appid.txt` or Steam hijacks the launch (SETUP).

### Clive Barker's Jericho (ScrapEngine, Lua 5.1, D3D9) — automated whole campaign 33/33 over LAN; one human session
Host runs enemies, story triggers, scenes, saves, level changes; clients get 20 Hz timestamped interpolated snapshots, events, late-join catch-up; client hits/uses/revives/powers/doors sent to host; remote players are native squad bodies with AI off driven by `InCtrl_*`; ENet or Steam P2P from a `dinput8.dll` proxy.
- Story steps per machine for own body; steps that would lock/turn/walk a remote player's body are left to that player; invisible blockers switched off at end of a talk follow the host (README "Story steps per machine (0.2.2)").
- Unreliable states sent just before a level change arrive after the new load: host accepts a peer's states only after its "level loaded" message; client ignores its first second; never apply relayed copies of own bodies (README "Body states across a level change (0.2.7)").
- Client mirrors host enemy animations, so animation events run there and call into a disabled AI -> crash; skip the AI call on clients, keep the effects (README "Enemy events that call the AI").
- Watchdogs: controls taken outside a scene restored after 12 s; body restored after a skipped scene; player cut off by a story-closed door offered a rejoin (README "Robustness").
- Remote body: follow owner height every frame (net-placed body not kept on floor); crouch via `ForceAction`; root-motion actions must not move a remote body (NOTES 2026-10-22; README "Squad").
- Engine fix by vectored exception handler resuming on the function's own "no target" path only when bytes match retail (README "One engine fix (0.2.4)").
- QA: F11 keeps last 30 s as MJPEG in memory; suites checked for full run length (one suite passed while the game crashed after its checks); mock engine must error on writes-only fields like the real one (README QA 0.2.2; NOTES 0.2.4).

### Code Vein (UE4.18, UE4SS + C hook DLL) — manual 2-PC only
Native vanilla session kept; mod re-hosts/re-joins automatically.
- Reconciler acts only after 8 s of "calm" (not busy, no lock, no boss, alive, no sequence, channels open) (README "Automatic step out and rejoin").
- First veto attempt hooked the "on enter state" handler and fired 11 ms too late; finding the real decision point mattered (README "Native experiments").
- UE4SS Lua hook on a trigger overlap froze the game; injected key presses do not fire keybinds -> poll a trigger file (README "UE4SS").
- Save isolation by folder swap with SHA-256 of real saves before/after (README QA).

### Command & Conquer 3 (SAGE, lockstep) — smoke tests + one human mission
- Receive-side anti-spoof dropped every command from a second slot acting for the same side; found by logging UI-created, sent and per-frame commands on both clients (NOTES §12).
- `<Local Player>` in shared logic must mean the campaign side on both PCs or the simulation desyncs; presentation stays per PC (`mission-survey.md` Summary).
- Script survey classified every reference to the human side into 7 kinds with one generic rule each (either/sum/apply-to-both) instead of editing 39 maps (`mission-survey.md` "Own factions").
- A multiplayer rule (elimination) killed allied AI sides, whose deaths cascaded through mission scripts into a defeat; traced with hooks on the script runner and condition check (NOTES §14).
- Harness: per-instance AppData via `SHGetSpecialFolderPath` hook, background-active hooks, per-instance loopback IP (NOTES §6, §9).

### Crime Scene Cleaner (Unity IL2CPP, first person) — automated suites + chapters 1-6 by two players
Host-arbitrated events; item/body ownership by holder; custom procedural partner figure; Steam flat API.
- Identity by hierarchy path (+start position for items), grouped into tables with per-table fingerprints; a differing table is switched off alone, and the status label must say so (README "Identity").
- Ownership epochs, host breaks ties, returns to host at rest; "rest" = asleep and not moving, bones kept awake after a throw (README "Ownership").
- State events via host, newest per key, replayed to late joiners; a whitelist of 58 `Type.Method` calls from a census bot that loaded every scene (510 component types) (README "World events", "How the game works").
- Objectives: only flags/text/story objects cross, never counts; counters add to what they read, so merged counts doubled (README "Objectives").

### Crimson Desert (BlackSpace, in-process client/server "Troc" messages) — one-PC tests only
Revival of cdmp; experimental quest stage sync and quest-accept sharing.
- Replay only (stage, branch) pairs the host's game actually sent, mapped from static sequencer key to the guest's runtime instance id; a made-up branch crashes the client (NOTES 16:18, 16:46).
- Runtime ids change per load (stages, NPCs); translate by static key; a guest needs the meeting scene's branches before the accept or the NPC stays "unknown" (NOTES 14:25-15:05, 15:00-15:30).
- Echo race: a stage whose own branch request is unanswered is "busy" and not touched (NOTES 17:08).
- Accept vs hand-in detector (mission-row change without item removal = accept; with removal = hand-in, never shared); never apply twice; wait for NPC loaded and host's "before" state (`README` "Quest accept sharing", NOTES 15:50).
- A day lost on a "random variant" that was the accept timestamp: a value was never checked against the data tables; a partial row compare gave a false PASS (NOTES 12:54, 15:15 CORRECTION).
- Move thinning (drop moves < 22 ms apart, hold-and-release) removed puppet stall-then-jump; motion meter counts jumps/stalls (NOTES 17:32).
- Tools (`tools/re/*.py`): `.pdata`-bounded function disassembly, stub/live classification, capture diffing, record files from captures; `tools/proton/*.sh` restore saves and diff-check before/after every run.

### Crysis (CryEngine 2) — QA none (doc review)
- New mod runs SP levels as network maps with SP logic restored on host; syncs AI shots, flow-graph/HUD effects, cutscenes, objectives; team checkpoint reset and revive (README "How Crysis works"). Warns: relay protocol tests are not game-sync QA.

### Cyberpunk 2077 (REDengine 4) — desk research
- Host's save is the world; host pushes time/weather/tracked quest every 2 s, guest clock only moves forward (oxylol); RedSync relays a whitelist of quest facts host-authoritatively with late-join snapshots; nobody solved shared NPC AI (README "How the game works").

### Dark Messiah (Source) — community reports
- SP maps on a listen server. Portcullis wheel softlocks if Use is held until the end; chain-climb lag with `cl_predict 0` (README Known issues).

### Dave the Diver (Unity IL2CPP) — automated bot with mutation pass (local co-op)
- "Player 2's turn": point the singleton `playerCharacter` at P2, call the game's own method, restore (README "What we changed").
- Cloning P1 overwrote single-slot input callbacks; shared command assets needed per-player copies; a grab sequence only works on "the player", so P2's grab is mod-side (README table).

### Dead Cells (Haxe/HashLink, DCCM) — manual playtests, playable run
- P2's turn swap of `game.hero`; never swap HUD globally (single minimap reached through it; crashed twice); never share HUD sub-objects between instances (NOTES Playtests 2-5).
- Bytecode scan for every function reading the HUD global (39) and for key checks on `game.hero.inventory`, instead of fixing one report at a time (NOTES Playtest 6, v0.7.1).
- Menus take an exclusive controller access; handing a menu to P2 must dispose the original (NOTES Playtest 1).
- Enum singletons compared by pointer; a mod-constructed value matched nothing (NOTES Playtest 8).
- Doors/boss doors that close behind "the hero" separate players: bring the partner through; plus a regroup button (NOTES Playtests 7-9).
- Deploy under a versioned folder, verify SHA-256 and the "loaded vX" log line (NOTES Playtest 1).

### Deep Rock Galactic: Survivor (Unity 6 IL2CPP, LiteNetLib UDP) — automated one PC; first 2-PC test contradicted several greens
Host-authoritative enemies/loot/XP/objectives/stage ends; puppet = cloned player visual subtree driven by synced state.
- Client spawner silenced at all four entry points; `SpawnAt` was not the choke point, `OnSpawn` saw every enemy; registry completed from the game's own list each tick with a counter (notes/02 "Suppressing SpawnAt", "Client spawner silenced").
- Network callbacks only enqueue; main thread drains; early messages park in a pending-retry list (notes/02 "Stage 1 enemy transport").
- `ApplyingRemote` guard so replayed events never echo (README "Stage ends reach the partner").
- Client reports the hp delta its own `TakeDamage` produced; host applies via the game's own path with `ignoreReduction` (notes/03 "Stage 1 damage").
- Patching a virtual and its override kills the game; never hook a method the game calls during title startup; install late; one patch failure must not disable the mod (notes/03 "Cost", notes/08 "Startup crash", notes/02 "Lesson").
- Mirroring objective *fields* never fired completion events, so the next objective never existed on the client; apply through the game's own `AddProgress` plus a type byte per objective (STATUS mission-sync ledger; notes/08 G12).
- Host "lends" the partner's position to `Bobby.Update` for one call so the partner counts (STATUS mission-sync).
- Arm events ran inside native code and hooks never fired: poll the object's state instead (STATUS, detonator row).
- Barriers: counting picks, not a flag that `StartFlow` cleared (45 s freezes); ready packets carry a run epoch; a timeout releases like success, so count `timedOut` (notes/05 G1; notes/03 "shop barrier"; notes/03 "Run-start load barrier").
- Peer loss releases every barrier, removes the puppet, shows a notice; client ends run if host lost (notes/04 "Partner lost mid-run").
- `worlddiff` census, sync contract (A-E, authority/bar/proof), claim audit (P/W/M), lag proxy with mutant self-test (notes/05, 06, 07; STATUS).

## 2. Applies to us (D2 host-authority migration)

**Authority model and suppression**
1. Freeze/suppress the partner's own simulation at *every* entry point, verified by counters, not by the one you believe is the choke point. DRG `SpawnAt` caught 8 of 804 spawns; `OnSpawn` saw all; registry completed from the game's own list each tick (deep-rock-galactic-survivor/notes/02 "Suppressing SpawnAt..."). Apply to D2 NPC spawners, pickups, story scripts.
2. Overwrite with the game's own deserializer, not hand-written fields: Autonauts uses per-object save JSON as the delta format and the game's loader for join (autonauts/README "How the game works"). Watch for `Load` re-running transitions; neutralise state first (autonauts/NOTES 0.12.37).
3. Partner-local side effects must be detectable: give partner-created objects their own ID range and sweep any persistent object in it (autonauts/NOTES 0.3.0, 0.11).
4. Apply host events through the game's own logic entry, not by writing result fields, or downstream events never fire (DRG STATUS mission-sync, `AddProgress`; DRG README "Stage ends" via `TriggerStageEnd`).
5. Story: keep the host as the script's "player" and patch the partner in per trigger/check (call-of-duty/NOTES "Plan for player 2"; C&C3 `mission-survey.md` `<Local Player>` trap). Survey and classify every script reference to the player into a few kinds with one generic rule each (command-and-conquer-3/mission-survey.md).
6. "Partner's turn": run a host-side game function with the player pointer temporarily swapped to the partner so the game's own logic counts them (dave-the-diver/README; dead-cells/NOTES; DRG Bobby lend). Do not swap globals the UI depends on (dead-cells/README "One hero everywhere").
7. Scripted steps that would lock, turn or walk the remote body belong to that player; blockers switched at end of dialogue follow host (clive-barkers-jericho/README 0.2.2).
8. Client mirrors of host NPCs still run animation events locally; any event that calls into a disabled AI must be skipped on clients (clive-barkers-jericho/README 0.2.7). Same for D2 Doppelganger/NPC puppets with AI off.
9. Per-player random rolls: host's schedule only; seed per-copy picks from run seed; clients run local-effect `Begin` with end timer disabled (baldis-basics-plus/README "Random events").

**Requests and replay**
10. Forward partner actions at input choke points and replay through the same game calls on the host (autonauts/README "Joiner input"); buffer a click while the host's body is in a non-abortable action (autonauts/NOTES 0.3.0). Redirect host UI that opens for the actor to the requesting peer (autonauts/NOTES 0.6.0).
11. Replay only what the real game produced; never synthesize indices (crimson-desert/NOTES 16:18). Translate runtime ids by static keys every load (crimson-desert/NOTES 14:25). Mark an entity "busy" while its own request is in flight to avoid echo double-apply (crimson-desert/NOTES 17:08).
12. Never cross counters, only flags/events; game counters add to what they read (crime-scene-cleaner/README "Objectives"). Guard against the game re-calling collect after a replay (same, "Double counts").
13. Whitelisted `Type.Method` replay, generated from a census bot that loads every level and lists component types (crime-scene-cleaner/README "World events"). IL2CPP/compiler folded bodies: check type before replay.
14. Held objects: ownership with epochs, host tie-break, returned at a defined "rest" (asleep and not moving) (crime-scene-cleaner/README "Ownership"). D2 carried bodies, thrown objects.
15. Client-reported damage: send the final hp delta and apply with reduction off, through the game's damage path so death/loot/credit follow (DRG notes/03 "Stage 1 damage"). Snapshot hp damage-only (`min`) so stale data never heals (DRG notes/03 "Enemy hp sync").

**Level loads, cutscenes, sessions**
14b. Discard peer state from the previous level: accept only after the peer's "level loaded"; ignore first second; never apply relayed copies of own entities (clive-barkers-jericho/README 0.2.7). Ready messages carry a run/level epoch (DRG notes/03 "Run-start load barrier").
16. Load barrier using the game's own pause plus the native loading screen held with a wait message (DRG notes/04 "P2 loading-screen hold").
17. Barriers: count events not flags, keep an early "done", count timeouts as failures, release on peer loss (DRG notes/05 G1, notes/03 shop barrier, notes/04 peer loss).
18. Watchdogs: restore controls held outside a scene after N s; restore body after a skipped scene; offer rejoin when a story door separates players (clive-barkers-jericho/README "Robustness"; dead-cells/NOTES Playtest 7-9 regroup).
19. Cutscenes skipped only when every player skips (clive-barkers-jericho/README "Cutscenes").
20. Liveness off the game loop; "busy" vs "gone" thresholds; level loads freeze main thread (baldis-basics-plus/README; autonauts/NOTES 0.2.0).
21. Lobby-member-only connections, held 5 s for the member list; one-account Steam tests with `CreateSocketPair` and hidden lab lobbies (baldis-basics-plus/README).
22. Late join: newest state per key replayed (crime-scene-cleaner); story quest-fact whitelist + late-join snapshot (cyberpunk-2077/README, RedSync).

**Puppets and animation checks**
23. Sync animator state hash plus normalized phase, correct only beyond a threshold (DRG README "walk cycle"; notes/03 "AnimPhase"). Death pose may be a trigger: reset with `Rebind` + `Update(0)` (notes/03 "corpse").
24. Remote body: follow owner height each frame; play crouch actions instead of input; block root motion (clive-barkers-jericho/NOTES 0.2.2; README "Squad").
25. Thin incoming moves (< 22 ms apart) to stop interpolation-buffer overflow; measure with a jump/stall meter (crimson-desert/NOTES 17:32).

**World-diff, flight recorder, chaos, QA discipline**
26. World-diff census at game-time samples, per-category counts, noise threshold, "persistent = same sign in 3 samples", ignore list that reports how much it hid, negative control by hiding one name, "not comparable" when the two sides are in different stages (DRG notes/05 Method). Per-table fingerprints that disable only the differing table (crime-scene-cleaner).
27. Anti-entropy digest of (id, hash) every N s with re-request; exclude volatile fields; hash after round trip (autonauts/NOTES 0.1.0, 0.9.0).
28. Flight recorder: in-memory last-30-s video (clive-barkers-jericho/README "Bug reports"); one-click bug zip with both logs (autonauts/README); record/replay files from packet captures as regression tests (crimson-desert/tools/re/rec_from_capture.py).
29. Chaos: TCP chaos proxy with control port (baldis-basics-plus/README); UDP lag proxy with p50/p150/p300/blackout profiles and a mutant self-test that must fail (DRG STATUS step 9).
30. Every check needs a twin switch that turns it red; verdict from the mod (`RESULT: test PASS|FAIL reason=`), timeouts are not evidence; peer evidence required on both sides (DRG README "Two habits"; STATUS "Definition of verified"; notes/03 "stage end").
31. Blind spots of one-PC harnesses: zero latency, friendly start order, test-only flags, one run per level; add late-start twin, shipped-config mode, second-run test, and keep a separate human-confirmed column (DRG notes/07). The first 2-PC test contradicted many greens (DRG STATUS ‡).
32. Compare whole records and log differing bytes; check every assumed meaning against the data before building on it (crimson-desert/NOTES 12:54, 15:15).
33. Suites must assert full run length and no crash after checks (clive-barkers-jericho/README QA 0.2.2). Mocks must reject what the engine rejects (NOTES 0.2.4).
34. Deploy verification by SHA-256 and a "loaded vX" log line; stale binaries look like failed fixes (DRG notes/02 "A stale binary"; dead-cells/NOTES Playtest 1). Save restore verified by hash plus a canary (DRG notes/03 "Two separate save profiles", "canary").

**Hooking pitfalls (native)**
35. Never hook a function called during title startup; install hooks late; isolate each patch's install (DRG notes/08, notes/02). Never detour a virtual plus its override (DRG notes/03).
36. Engine crash on co-op-only paths: resume at the function's own null path from a vectored handler, only when bytes match (clive-barkers-jericho/README 0.2.4). Use `.pdata` bounds when classifying functions (crimson-desert/NOTES CORRECTION).
37. Decision points: the visible state change may be 11 ms after the real decision; hook the decision (code-vein/README).
