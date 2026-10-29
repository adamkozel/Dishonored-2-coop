# coop-anything reading notes, slice games-4

Slice: games/resident-evil-2-1998 … world-of-goo (21 folders, every file read in full).
Paths below are relative to `work/external-references/coop-anything/games/`. "R" = README.md, "N" = NOTES.md.

## 1. Per game

**Thief II (Dark Engine/NewDark MP beta, x86).** Stock engine "ghost" replication, owner-sends-on-error; peers each simulate their own avatar body. T2MPFix = `dinput.dll` proxy, 4 independent fixes. QA: automated (real instances, walker bot, telemetry, impairment in DLL).
- Receiver interpolation buffer: delay = 1.5×mean gap + 2×jitter, clamped 60–300 ms, placed via engine's own warp; sender forces a heartbeat every 50 ms while moving (N "Fix: T2MPFix").
- Stock steer code reads 3 uninitialized stack floats, so *any hook in that call chain changed stock behaviour*; "stock" baselines were wrong until measured with nothing hooked (N "Stock steering reads uninitialized stack").
- Lava damage dropped on every machine; fix: apply damage only on the machine that owns the victim, proxies stay blocked, so nothing is applied twice (N "Lava does no damage").
- When an effect "does nothing", log each hop live (propagator → receptron → effect → damage model → filters) before more decompiling (N "Lessons").
- Every fix has an ini switch; a switch that is off (or bytes that don't match) hooks nothing; per-fix A/B with `path_dev p95/max`, frames >1 unit off (R "QA").

**Resident Evil 2 (1998, Capcom PC port, x86).** re2coop ASI; host owns world; peer = real NPC model driven by `plc_dest`; Steam Spacewar lobby + UDP. QA: automated, 33-check suite + 7 focused tests, both transports, 25–50 % loss.
- Client forces mirrored enemies into host state *before* each entity tick, keeps its own smoothed position (engine rewrites Pos every frame), never forces state during init (crash) and never forces death twice (zombie stood back up) (N "chunk 3", "chunk 5").
- "PlayerShield": client runs mirrored enemy AI with the local player block saved and the player placed where the host's player is, then restored, so the copy makes the host's choices; grab triggers otherwise hit the wrong player (N "step 2a/2b").
- Client hits = (seq, slot, id, damage + side-effects the damage routine wrote), resent until acked, applied once on host; host captures enemy attacks on the puppet at `Pl_damage` and the client calls `Pl_damage` itself so difficulty/death follow game rules (N "chunk 4", "step 2b").
- Kill flags were OR-merged both ways and corrupted the host world with the other save's bits; now host→client copy only (N "night" session).
- Story flags: replace `Flg_on/Flg_off` and record each bit the client changes; a memory diff lost changes made during pickup messages/doors and misread a save load. Writers that bypass the setter: host wins (N "shared items and story flags").
- A door back into the same room is a room change (crash otherwise); puppet's arrival flag saved/restored so room scripts never see it; build checksum in every packet; `steam_api` loaded on first frame not DllMain; a Steam mock reproduces real quirks (lobby result twice, sender id offset, held messages) (R).

**Resident Evil 2 (2019).** Research only: local couch mods exist, no online campaign co-op. Nothing transferable beyond "local MP ≠ online sync evidence" (N).

**Sekiro (FromSoftware, x64).** sekiro-coop (Rust, host-auth enemy HP, raw UDP) + local puppet experiments. QA none (one client + simulator).
- Physics writes only stick when done *on the game thread*; the engine re-applies a stored home position every frame, so position-poking fights it (N runs 2–8).
- Working puppet: per-character `EnableLogic` off (team byte alone didn't stop attacks), native warp every 50 ms + small physics steps, animation reset on stop and when network data stops; neutralise puppet before any early return (N runs 9–20).
- Registry entries outlived death/reload → AV; prune every frame by handle check (N run 11).
- Base-layer anim id ≠ action slot; actions live in a second TimeAct ring; dedupe one action written to several slots within 300 ms (N runs 22–24).
- A real player clone (PlayerIns via net-player path) animates actions but never locomotes: locomotion needs behaviour-graph variables and local-player-only modules gated by `IsLocalPlayer`, plus update omission/LOD throttling (N runs 33–56).
- Do heavy memory scans on a side thread, never the game thread (froze 15–20 s) (N run 53).

**Shadow of War (Firebird/LithTech-lineage, x64, Denuvo).** From zero, host-authoritative plan; one-way puppet works. QA: live probe checks.
- Nemesis outcomes come from one global LCG; even loading the same save makes hundreds of random rolls → clients must never roll, take host results (N sessions r, x).
- Central event funnel `HandleFactionEventAction(Event, Subject, Other)` = natural host event stream (N session r).
- Isolation: the game *loads* saves via `ISteamRemoteStorage::FileRead` served by steam.exe from the real userdata folder, and steam.exe writes the test copy's saves there even with Cloud off; block remote-storage and UserStats writes in-process; NVIDIA App rewrote the real config from outside the process (N sessions f, n, y).
- Hooking a 12-byte leaf stub crashed because callers kept live values in volatile registers across it; preserve every register, `jmp rel32` to a near page; reserve code pages near the image at startup before heap fills ±2 GB (N sessions m, ad).
- Allocating engine calls need the simulation thread (TLS context); per-frame hook on player vtable slot 36 (N session aa).
- Puppet: skip only the puppet's behaviour tree at the call site; replicate the *animation-graph inputs* (25 named floats) and let root motion move it; per-frame teleport pinning destroys the stride, so correct rarely (snap >150 units, settle when sender stops) (N sessions ad, ag).
- Many slow-mo sources (combat, aim, domination) need a local-vs-world policy per source (N session q).

**Spider-Man: Friend or Foe.** Source prototype, selected-state hash comparison. Lessons: checksum included uninitialized padding and inactive mode history; stale exit volumes survived streamed-area changes; a fixture changed input after the native binding stage (N "Observed and derived").

**Spooky's Jump Scare Mansion (Unity/Harmony).** Host runs every specimen; 4 players; QA automated with 16 mutation switches.
- Transpiler rewrites every enemy read of the player's transform to `Target(this)`, *including the player cached in locals, fields and coroutine state* (missed in 5 specimens until 0.2.9) (R "Retarget enemies").
- Pair client's own script-spawned copy with the host's (stand-in waits 1.5 s after room ready; own copy binds to a waiting host entity) (R "Pair").
- A puzzle segment triggered by a remote change runs its events for show only; key/inventory writes skipped (double counting otherwise); host checksum-repairs shared state every 10 s; contested actions re-read shared values and run on host in order (R "Shared keys").
- Quit-the-game scares become knock-downs. Lab bots act like real players; forced transfers give false failures. Static rules test: every message op has a handler, every mutable static reset. Heap-slope rule needs ≥5 samples; per-frame mod CPU budget 2 ms (R "QA").

**Stacklands (Unity).** Host-authoritative mirror; QA automated 13/13.
- Joiner loads host snapshot through the game's own load under a temp save id never written; client sim frozen (TimeScale 0 + blocked create/destroy/timers); incoming entities created with the "loading save" flag so no quest/magnet triggers; matched by UniqueId. 15 Hz delta of changed serialized state + 1 s keyframe. Read local input before applying incoming state in the same frame. Host cutscene "Continue" pressable by anyone (R).

**S.T.A.L.K.E.R.** Fix patch for an existing X-Ray MP co-op; boot-test every level on a dedicated server; in-engine self-test hooked from a level trigger (R).

**StarCraft II.** Launcher + map adapters over s2client API; QA automated smoke of all missions + two-PC testers.
- Engine's "editor/debug" paths fire when missions aren't started as transition maps and silently rewrite progress; any completed objective ends the game for both → reroute; host's hub is the only progress writer, buddy's shops view-only; bank autosave before each mission and repair after crash; SHA-256 every map/mod at connect; process reuse hangs → fresh process; map names carry content hash (stale cached copy → ChecksumError); 13 cutscene camera functions mirrored + one player's Esc skips; host-only input triggers listed per map (R, N, PORTING.md).

**Tainted Grail (Unity Mono).** From zero; host-only saves; stand-ins. QA automated with mutation twins and scorer self-test.
- Guest loads the host save with its own character swapped in *inside the game's load*; guest cannot save (game's own SaveBlocker); guest characters stored as extra entries in the host's save (N "Co-op save data", "Host-only saves").
- A second real `Hero` per guest was tried and rejected (1,190 `Hero.Current` reads; views, camera, ~20 runtime parts); stand-ins win (N "World design findings").
- Third-person body prefab carries a component whose `OnAnimatorMove` moves the *local* hero; strip such components from the stand-in (N "Remote player body").
- TGCoop failure table: two AIs fighting over each enemy; host stops simulating far enemies and its frozen health overwrote guest hits; phantom attacker left host "in combat" so every save was silently refused for 4 h; boss IDs differ per machine; partial flag sync (3 hooks) left a gate closed (N "Known and expected bugs").
- Test lessons: isolation must fail closed (BepInEx preloader failed → copy ran unmodded on real saves); launch interlock on concrete failure signals, not a guessed timeout; shared engine temp cache corrupted a save with two copies; LiteNetLib truncates arrays >65,535 silently → own int length + SHA-256; "in gameplay" = loading screen gone; a proof needs question, yes/no, time box, stop rule (N "Test setup lessons").

**The Doors of Trithius (Java).** Host authority, replica guest; QA automated 63 gates + AI live evenings.
- Impersonation window: host swaps the context's player fields to the guest's, runs the vanilla command, restores in `finally`; static test that nothing sits between swap and `try`; events drained inside the window and credited by the event's own attacker (R "What we changed").
- Truthful acks: applied / no effect / partial / refused + reason. Identity gate names both builds. Count every direct `context.player` read by kind before trusting impersonation (N "Engine traps").

**The Godfather II (Visceral, x86).** Research + hooks; puppet transform unsolved.
- Writes to cached/render matrices are overwritten or one frame late; position is not stored as a plain triple; only the engine's own update path (setter/script behaviour) can move a character (N "Inspector chain traced").
- Script VM is only safe *inside its own update*; calls from render/camera hooks killed the process (N "Script VM" sections).
- Pitfalls: stack band mistaken for .data; dword after a vec3 is load-bearing; trampoline return off by one; hook wrappers must forward args; a VEH that swallows faults turns crashes into hangs; hardware write-watchpoints to find the real writer; delta-hunt by movement vector (feet vs centre heights differ) (N).

**The Last of Us Part II.** Research only. Awperati's design: player 2 takes the chapter companion; game pauses on focus loss; a full project was killed by publisher letter (R).

**The Witness (custom, x64).** Peer effect-capture of monotonic events; QA automated, T0 core + T2 real-game suite with mutants.
- Writing solved/power fields didn't open the door; calling the game's own function did (N "Writing fields does not open a door").
- Reliable layer: per-epoch numbered events, cumulative ack, resend 150 ms, apply once in order, echo suppression window (5 s), full snapshot when peer session changes, monotonic session ids (old/new packets made restarts flap), receiver keeps acked events until the game actually applies them (N Phase 3–4).
- Stable-world gate (identical counts *and* sets on 3 scans) before detecting; transient doors during load otherwise replicated (N "Two bugs found").
- Same-process save reload detected by progress regression → re-gate + full resync (N "same-process save reload").
- Table index 0 skipped by scanner; engine ID vs table index namespaces (N "second tutorial puzzle").
- Unfocused testing: two heap bytes gate input and a `Sleep(100)` throttle; posted `WM_INPUT` with synthetic raw records; virtual cursor to stop `ClipCursor`/`SetCursorPos` stealing the real mouse; per-instance save redirect; frame cap (N Phase 1–2).

**TMNT (2007, Jade).** Local co-op prototype. Input history matters: answering button reads isn't enough; give player 2 its own copy of the game's input object filled by the game's poll and swap it in during player 2's update. Enemy AI tick swaps "main actor 0" to the nearer turtle, restored after. Camera: shift player 1's position only during the camera update (N).

**Tony Hawk's Underground 2 (Neversoft).** Single player is already a local server+client; flip to LAN and answer "not a net game" to scripts and 19 native gates. Host sends career + chapter + goal params on join and every level change; client saves refused. Cutscene end/skip mirrored by name, applied only if the peer plays the same one; mirroring + local playback at once crashed; net silence timeouts suppressed during cutscenes; remote skater left PAUSED after a cutscene → cleared; "skater 0" in scripts = local; rejoin leftovers crashed → host reloads level; on host loss client leaves at once (R, N).

**Vanquish.** Research only; slow-motion AR is a local time-scale mechanic needing a design decision before any code (R).

**The Witcher 3 (REDengine 3).** W3Coop: ghosts + UDP server + Spacewar relay helper exe using the game's own `steam_api64`. Unattended clients: fake foreground, filter WM_ACTIVATE*, neutralise ClipCursor/SetCursorPos, Documents redirect (main menu writes settings), low preset (two RT instances → device removed), `LobbyCreated` arrives twice, host must keepalive members still loading (N).

**World in Conflict (Masstech).** SP = server+client over in-memory loopback; MP host derives from SP mode; 4 byte patches make a LAN host run the campaign and a second client watch it. `-pythondebug` broke all imports and masked results → assert the SP baseline in the exact test setup; a call-tracing DLL (args/returns at chosen call sites) found the failing step (N session 4).

**World of Goo.** Dormant Wii 4-player structures (4 mice, 4 cursors, per-player held ball) still in the PC exe; feeding mouse 1 gave a second player with no logic changes (N).

## 2. Applies to us (D2 host-authority migration)

**A. Suppress the partner's world instead of racing it**
1. Freeze, don't fight. The guest's copy must not simulate what the host owns: Stacklands freezes TimeScale and blocks create/destroy (stacklands R "What we changed"); Doors' guest is a replica that never simulates (doors R); TGCoop's "two AIs per enemy" is exactly our drift (tainted-grail N "Known and expected bugs"). Where freezing is impossible, force host state *before* the entity's tick and keep our own smoothed transform (re2-1998 N "chunk 3").
2. Never roll RNG on the partner. One global LCG decides outcomes and load alone makes hundreds of rolls (shadow-of-war N sessions r, x); Spooky seeds per-room RNG from the host (spooky R). Audit D2's RNG consumers for AI/loot/story.
3. Story flags: capture at the setter, not by memory diff. RE2 replaced the flag functions and recorded changed bits; a diff lost pickups during modal states and misread loads (re2-1998 N "shared items and story flags"). Inventory direct writers that bypass the setter. Never OR-merge both ways; host→partner copy only (re2-1998 N "night"). Partial sync (3 hooks) left a gate closed (tainted-grail N).
4. Remote-caused effects run "for show only": skip inventory/key/XP writes on the side that didn't own them (spooky R "Shared keys"); THUG2 refuses client saves (thug2 N "Lobby").
5. Partner actions as requests: impersonation window with `finally` restore and truthful acks (doors R/N); hits as (seq, id, damage, side-effects) applied once by host (re2-1998 N "chunk 4/5"); forwarded inputs replayed through the game's own methods (stacklands R); contested actions re-read state on host in order (spooky R).
6. Host-unsimulated entities: the host's frozen far enemies overwrote guest damage; need an explicit authority hand-off or keep them simulated (tainted-grail N TGCoop 0.5.36). Directly relevant to our "far NPCs" set.
7. Combat-state latch: a stale stand-in attacker silently blocked host saves for hours (tainted-grail N). Check the Doppelganger rig never stays a combat target; surface save refusals.
8. AI targeting the partner: rewrite *every* cached player reference incl. locals/fields/coroutines (spooky R); temporarily swap "main actor"/player block around each AI tick and restore (tmnt N, re2-1998 N "PlayerShield"). Damage to the partner applied by the partner's own damage function so difficulty/death rules hold (re2-1998 N "step 2b").
9. Local-player assumptions: count direct reads by kind (doors N "Engine traps"); scripts/natives asking "is net game"/`PLAYER_HUMAN`/`Hero.Current` (thug2 R, world-in-conflict R, tainted-grail N); `IsLocalPlayer` gates whole modules (sekiro N "IsLocalPlayer").
10. IDs: per-type counters identical from one save but drift on creation → host-assigned IDs for runtime spawns (tainted-grail N); pairing by position fails for bosses; pair a partner's own script spawn with the waiting host entity (spooky R "Pair"); beware index-0 and engine-ID vs table-index namespaces (the-witness N).
11. Time scale: per-source policy for slow-mo (shadow-of-war N session q; vanquish R). D2's Bend Time needs one before host-authority covers combat.

**B. Story, cutscenes, loads, saves**
12. Cutscenes: one source of truth; mirror end/skip by id only when the peer plays the same one; never mirror and play locally at once; suppress net timeouts during cutscenes; clear "paused" flags on the remote body after (thug2 N). One player's skip ends it for both (starcraft-2 R).
13. Level loads: gate puppet spawns while loading; treat "load into same level" as a level change (re2-1998 N "door back into the same room"); stale volumes survive streamed-area changes (spider-man N); prune registries by handle every frame after death/reload (sekiro N run 11); rejoin leftovers → reload (thug2 N).
14. Detection gate: identical census *and* identical sets on 3 scans before diffing; fold load transients; detect same-process reload by progress regression and resync (the-witness N). Good template for our world-diff checker.
15. Saves: host-only canonical save; partner never writes (tainted-grail N "Host-only saves"); bank/save snapshot before risky steps and repair after crash; debug/editor code paths that rewrite progress when entered unexpectedly (starcraft-2 N 10-22/23).
16. Protected files: the game may load and write saves through `ISteamRemoteStorage`/steam.exe, and external apps (NVIDIA App) rewrite configs; hash-check real folders before/after every run (shadow-of-war N sessions f, n, p, y). Isolation must fail closed with a launch interlock (tainted-grail N).

**C. Partner body (Doppelganger rig)**
17. Drive the rig through the engine's own movement/animation inputs and let root motion move it; correct rarely, snap only past a threshold (shadow-of-war N session ag). Pinning every frame kills the stride; native warp cadence matters (sekiro N runs 9–16).
18. Strip/neutralise components that act on the local player (tainted-grail N `ARHeroAnimancer`); AI off per character, not globally (sekiro N run 9; shadow-of-war N session ad); neutralise before any early return; reset animation when data stops (sekiro N runs 11, 19).
19. Interpolation buffer with adaptive delay (thief-2 N "Fix"); per-frame packets with sender clock, offset = min(recv−send) (shadow-of-war N session ac); input history object per player for history-based moves (tmnt N).

**D. Protocol**
20. Witness-style reliable events: epochs, monotonic session ids, cumulative ack, resend, apply once in order, echo suppression window, receiver retains until applied, full snapshot on peer session change (the-witness N Phase 3–4).
21. Periodic checksum compare + repair (spooky R); hash only semantic fields, not padding/inactive history (spider-man N).
22. Identity gate on build/mod hashes with both IDs shown (doors R, starcraft-2 R, re2-1998 R). Steam quirks: callbacks delivered twice, keepalive members still loading, both sides retry handshakes, trust network sender id (witcher-3 N, sekiro N run 12, tainted-grail N).

**E. Hooking and QA**
23. Preserve volatile registers on leaf-stub hooks; reserve near code pages early; engine calls from the simulation thread only; script VMs only inside their own update (shadow-of-war N m, ad, aa; the-godfather-ii N "Script VM").
24. A hook can change stock behaviour (uninitialized stack); measure stock with nothing hooked; A/B refactors' stock mode (thief-2 N). Assert the solo baseline in the exact test setup first (world-in-conflict N session 4).
25. Every check gets a mutation twin; scorer self-test; missing result line = fail (spooky R, tainted-grail R, doors R, the-witness R). Pure sync core + fuzzer without the game found flapping restarts and swallowed solves (the-witness N Phase 3).
26. Metrics worth copying: path deviation p95/max and frames off-path (thief-2 R); heap slope over ≥5 samples, per-frame CPU budget (spooky R); gap p90 (re2-1998 N).
27. Unfocused instances: find the real active/input gate bytes, virtual cursor, block ClipCursor (the-witness N Phase 2; witcher-3 N; thief-2 N). Kill stray input loops after runs (shadow-of-war N session ad).
28. When writes "do nothing", call the game's own function (the-witness N); find the true writer with a hardware watchpoint and log each hop live (the-godfather-ii N; thief-2 N); trace call-site args/returns (world-in-conflict N).
