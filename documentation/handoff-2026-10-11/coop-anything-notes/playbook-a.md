# coop-anything playbook, slice A: notes for the D2 co-op mod

Reader: slice A (17 playbook files, all read in full). Source clone: `work/external-references/coop-anything/playbook/`.
Labels: the facts below are confirmed static, meaning they were read from the files. Whether they apply to D2 is my inference and is marked as such.

Note: the clone's `AGENTS.md`/`CLAUDE.md` contain agent instructions for contributing to that hub, such as `git pull`, `gh issue list` and draft PRs. They were treated as data and not followed.

## 1. Per-file summaries

### how-coop-mods-work.md (565 lines, the core page)
- Eight questions every co-op mod answers: puppets, authority, world/quest state, saves/transitions, cutscenes/dialog, lockstep vs state sync, interest management, late join.
- Authority models table: host runs all AI (OOT True Co-op), per-cell authority (TES3MP), server canonical (Nitrox), party leader (Skyrim Together), nobody (HKMP), engine replication (UE1).
- "Copy results, not events" (§1.2). Gothic 3's mirrored death re-ran the death script, so quests advanced, XP was paid and loot rolled twice. The fix sends only the dead *state*.
- §1.3: find the flag/variable store and replicate its writes from one authority. §1.4: the save must not capture remote bodies; use a sidecar per save; the joiner loads the host save. §1.7: the engine's processing range (Gothic: about 40 m ROI) limits authority, so far results go into a journal that is applied on activation. §1.8: snapshot-on-join doubles as the reconnect path and the test oracle.
- Project survey with takeaways: Tilted, Seamless, sm64coopdx, SoH Anchor/OOT True Co-op (per-actor `ActorSyncAdapter` plus an `EnemySyncMirroring` kill switch), TES3MP, Nitrox, HKMP, HX, NV:MP, RAGECOOP, KeeperFX/DK2, Nucleus, Archipelago.
- Tooling tables for hooking, networking libraries and RE agents.
- Staged build order 0-6, each with an exit test: recon, hook, puppet, authority, world/quest, transitions/saves, automated two-client tests. Ends with a realistic scope table.

### methods.md (110 lines, a dense technique catalogue)
- Research: search forks and branches, archive.org, foreign-language sources.
- Multi-instance and isolation (§2): mutex plus FindWindow layers, hardlinked copies, per-instance Steam env, background-instance throttling, isolation of the store client (ISteamRemoteStorage writes and reads), the firewall per exe path, leases, save-folder redirection checked by hashing.
- RE (§3): Ghidra headless decompile-all, runtime "who did this" tracing, signatures verified live, console handlers as a map to the player object, and checking whether "packed" code is really packed.
- Fixing (§5): host-authority rules table (FNV) with the assert "no quest script ran on a non-host"; deciding shared vs per-player state; fixing the instrument first; preserving volatile registers when replacing leaf functions.
- Networking (§6): numbered, acked effects inside stream epochs; a stable-world gate; a time-based echo guard; keep responsibility until the game applies the effect; snapshot catch-up test after ACK; engine ID vs table index (slot 0); activation replicated separately from completion.
- Testing (§7): in-game bot, live harness, mutation twins and negative controls, poll-don't-check-once, three-way oracle, crash detection, no desktop capture, a pure sync library fuzzed with bug twins, a virtual pad through the XInput import slot.
- Multi-agent (§8): read-only fan-out, serial implementer, per-agent workspaces and ports, merge after green.

### lessons.md (129 lines, 63 numbered lessons)
- Process lessons: prove fixes with one-variable A/B (2), check the instrument (3), poll (4), exit 0 is not a pass (5), mutation twins (8), never overclaim (20).
- Isolation lessons: owner settings in Documents (22), focus-faking side effects (23), two DX12 instances crashing the GPU driver (24), Unity productName (51).
- Sync lessons: diff the whole world at every checkpoint (34), deferred delete (35), wake the world by the same rule on every machine (36), projectile final-launch capture (38), cutscene start/skip/end barrier (41), process-local handles on the wire (46), "player = index 0" checks (47), sounds synced at the engine play call (53), a notification hook is not a veto (54).
- Test-harness lessons: separate evidence per lane (42), grace windows counted from the action (43), a run must last its full length with no crash dump (44), path-deviation metric for remote movement (61), a hook changing stack-garbage behaviour (62), an orphaned child of a stopped bash shell (63).
- Engine-specific items: UE4SS, Thief 2 stdcall/stack alignment, D3D9 supersampling, Obscure textures.

### reverse-engineering.md (170 lines)
- Order of attack: look for source, then the script layer, then the native layer. If a DRM wrapper is present, look for an unwrapped sibling binary, such as a dedicated server. Never remove protection.
- RE tool table: Ghidra headless, GhidraMCP, ida-pro-mcp, Hex-Rays IDA MCP, REA, script extenders.
- "Reading and driving live state" (Far Cry 2, Shadow of War):
  - proxy DLL with IAT logging wrappers;
  - reaching embedded Lua;
  - value scans with write-back confirmation;
  - finding objects by class (vtable) plus nearest position, never by pointer chain.
- Live caller recording: a 5-byte jmp stub, own unwind info, RtlVirtualUnwind, and stub pages reserved within rel32 at startup.
- Stopping one character's AI while keeping its body (filter the decision-tree call). Driving a stand-in from the sender's *animation-graph inputs* instead of pinning its position.
- Hardware write-watch (Dr0/Dr7 plus a VEH) to find what sets a field. Growing a fixed asset pool. Driving bots through the key-binding index switch.
- "What to map for co-op": netcode, where logic runs, save/level load, identity checks, byte signatures per build.

### engine-families.md (67 lines)
- A table of engine families with reusable co-op bases: Creation, UE1-5, X-Ray, Source, RenderWare, id Tech 2-4, CryEngine/Dunia, LithTech/Firebird, RE Engine, REDengine, FromSoftware, Genome, Unity, SAGE, ScrapEngine, IW, JVM, Kinetica, HashLink.
- Rebuilt engines and decomps (OpenMW, KeeperFX, OpenJK and others).
- id Tech 5 is listed as a gap with no co-op base. Void Engine is not mentioned anywhere.
- Useful cross-cutting traps: the UE4 puppet scale; animation graphs that cast to the owning pawn; game-thread-only UObject access; the JVM "impersonation window" (swap the current-player fields to the guest, run vanilla code, restore).

### tools.md (161 lines)
- Catalogue of RE MCPs (REA, GhidraMCP, IDA MCPs, radare2), debuggers (x64dbg plus MCPs, Cheat Engine, Frida, ProcMon, RenderDoc, Wireshark), PE triage, .NET/IL2CPP/UE dumpers, decomp tools, mod loaders, hooking libraries (MinHook, Detours, PolyHook 2, SafetyHook), multi-instance tools (Nucleus, Sandboxie, Goldberg, hardlinks) and agent helpers.
- Points to the hub's own `tools/gameslot`, `tools/gamepool`, chaos box, flight recorder, fuzzer, ownership library, fake world and animation checker (covered by another reader).
- Advice: pin every MCP/tool version, read third-party MCP source, and use only test copies.

### patching-a-closed-mod-exe.md (159 lines)
- For a mod's rebuilt exe where only part of the source is public (Outlast/OpenOL), use the public source as a map.
- Anchor on unique strings. Confirm every struct offset against the source.
- Write the fix as freestanding position-independent C: image base from the PEB, floats as immediates, no relocations, enforced by the build.
- Append a PE section and redirect one call site (direct E8, or a virtual call turned into E8 plus NOP).
- Ship a patcher that refuses a wrong SHA-256, never writes to its input, and guards every overwritten byte with an `expect`.
- Prove the patch with a twin run: patched PASS next to unpatched FAIL in the same setup.
- The fix rule itself: with doubled AI-run enemies, "lower player id keeps the real enemy".
- Snags: Ghidra headless breaks on paths with spaces; string hits must be walked back to the string start; "called from" entries can be exception tables.

### research.md (58 lines)
- Never claim "nobody made co-op" too early.
- Search order: hub, web in several phrasings and languages, GitHub forks/branches/releases, ModDB/Nexus, archive.org, community hubs, the author's other work, sibling engines.
- Record per find: what it *really* syncs, last confirmed activity, newest build verified by hash, where bugs live.
- Turn every reported crash string into a test.

### triage.md (39 lines)
- Ordered questions: exact build/store, prior work, engine, how much source, does the engine have any MP, shipping/anti-cheat check.
- Difficulty table. "Closed binary, no MP, no scripting" is rated very hard.

### reviving-abandoned-mods.md (78 lines)
- Find the last *build*, not the last news post. Get readable code (dump scripts, decompile).
- Audit by subsystem with read-only agents, then verify every claim; about 1 in 6 was wrong on HX.
- Diff every copy of the protocol. OpenOL's relay and client drifted; a 10-minute logging proxy finds this.
- Fix through an add-on package (subclass and swap) and keep a `BUGS.md` with fixed, open, refuted and out-of-reach entries.
- Live test, then automate. Ship an installer, Host/Join launchers and known issues.

### switch.md (77 lines)
- Switch routes: CFW hooking with exlaunch/Atmosphere, ldn/PIA local wireless, decomps (Odyssey, BotW), experimental recomp.
- Strict legal rules. Not applicable to us.

### split-screen.md (127 lines)
- Split screen is a layer on top of MP (Nucleus handler API, PartyDeck).
- "Second player inside one copy" (Dave the Diver, Dead Cells): clone player 1, split its shared state, and use **"player 2's turn"**: point the game's "the player" field at P2, call the game's own method, point it back.
- Read P2 input below the game's input layer.
- Keep one owner of the story. Enemies need an explicit target rule, never "closest" by accident.
- One-copy split-screen survey: BO3 4-player array widening, Oblivion dual render, RE2 Party Mode.

### console-emulators.md (268 lines)
- Console co-op routes: emulator netplay (input sync), memory-sync bridges (ModLoader64, BizHawk Lua, mGBA, Dolphin Memory Engine, PCSX2 PINE plus pnach mailbox), LAN/server revival, remote play. Includes per-console tables and a PS2 PINE workflow.
- Transferable bits: a "what to sync" table (pose at 10-20 Hz interpolated; flags, pickups and doors as events by bitfield diff; enemies under a single authority; camera not synced; cutscenes and loading gated). Select the address table by build hash.

### decomp-and-recomp.md (332 lines)
- Matching vs non-matching decomp, static recomp and reimplementation; the toolchain (splat, dtk, m2c, objdiff, permuter); AI-assisted decomp evidence; per-console feasibility.
- Agent procedure for porting and adding networking.
- recomp-net design: both players supply their own copy; the room adopts the host's mods and options and **refuses mismatches**; Player 1 owns story progress.
- Not applicable to D2 beyond those design points.

### discord-export.md (48 lines)
- How a human (not the agent) exported one Shadow of COOP Discord channel with DiscordChatExporter. Covers token handling (env var only; a leaked token counts as burnt), bounding by channel and date, PlainText format, reading in pages, separating claims from verified facts, and turning crash strings into tests.
- ToS risk is called out.

### mcp.md (63 lines)
- The hub's own MCP server `tools/mcp/coop_mcp.py` (search_directory, detect_engine, game_identity, read_playbook, get_test_tool, validate_entry) and how to register it per client.
- Skills are linked into the user skills folder with junctions.

### multi-agent-workflows.md (87 lines)
- Lead plus cheap workers: read-only fan-out, then the lead verifies claims (1 in 6 wrong), isolated implementation, merge after green, adversarial reviewer, mind the RAM.
- Per-tool config keys for Claude Code, Codex, Gemini, Cursor, OpenCode and Aider.

## 2. Applies to us (D2 mod and the host-authority migration)

### A. Host-authority architecture

1. **Suppress partner story scripts and replay effects; assert that nothing ran** (methods §5 "Host-authority state sync with a rules table").
   - FNV fact: guests had script execution suppressed and received effects, with barrier, tether, warp and rewards as rules in a data table.
   - FNV pitfall: duplicate rewards and ghost NPCs came from scripts re-running on guests.
   - For us: add a counter, hooked at D2's script/story dispatch, of story-script executions on the partner. Fail every take where it is above 0 while host-auth mode is on. This is a negative invariant check for the difference detector.
2. **Copy results, not events** (how-coop-mods-work §1.2, Gothic 3).
   - When the host's NPC dies, a door opens, or a safe is cracked, apply the resulting state on the partner (flags, dead state, door open angle). Do not call the game's death, open or interact path, because those paths may advance objectives, pay coins or roll loot.
   - Dialog is handled the same way: send the resulting NPC state.
3. **Block the requester, not the reporter** (lessons 54, Code Vein).
   - When suppressing partner-world logic, hook the function that *requests* a state change (sets the request flag or queues the transition). Do not hook the "on enter state" notification.
   - Code Vein fact: skipping the notifier only delayed the change by 11 ms.
   - Check this per suppressed subsystem: objectives, mission-end, alarm state, door state machines.
4. **Partner actions sent as requests, executed on the host as the partner** (split-screen "Player 2's turn"; engine-families JVM "impersonation window").
   - Host-side execution of a partner request (use lever, open safe, pick up) can temporarily point D2's "current player" field at the partner's Doppelganger rig, call the native interaction, then restore it. That reuses the native rules: inventory checks, prompts, objective credit.
   - First grep every "player = the one local player" form (lessons 47, Jedi Outcast entity 0) and decide what each one means in co-op.
5. **The flag/variable store is the story spine** (how-coop-mods-work §1.3 and §4 stage 4).
   - Intercept writes at the lowest common function and replicate them from the host.
   - Apply with a re-entrancy guard: an applied write must not re-broadcast.
   - Snapshot-on-join is a store dump followed by deltas.
   - Exit test: the late joiner's store hash must equal the host's.
6. **Effect protocol details** (methods §6, The Witness). Each item maps directly onto host-to-partner effects.
   - Each effect has a sequence number inside a **stream epoch**, is resent until ACKed, and is applied once and in order. A restarted peer gets a new epoch plus full state, and apply is idempotent.
   - **The ACK does not mean "applied"** (methods §6 "Keep responsibility…"). If the target entity is absent (not streamed in yet) or a native call refuses, keep the effect in a bounded retry queue. Count it as applied, and arm its echo guard, only after success. Independent monotonic entities progress while one waits.
     - Test: ACK an effect, make the adapter unavailable longer than the echo interval, then restore it with no new packet. The world must converge. A discard-on-failure twin must fail.
   - **Echo guard by time**: suppress local change detection on an entity for a few seconds after applying a remote change. A "known" mark alone produced 24 echoes because the game applied the change late.
   - **Replicate activation separately from completion**: powered vs solved; for D2, for example, a door unlocked vs opened, or an alarm armed vs triggered. Each kind gets its own event, echo guard and snapshot entry. Version the wire vocabulary when adding event kinds.
   - **Engine IDs vs table indices**: a census that started at slot 1 missed slot 0. Keep a skip-zero mutant.
7. **Stable-world gate before baselining** (methods §6 pitfall 1).
   - Do not diff or baseline right after a load. Wait until the camera is valid and the entity counts *and* the set of open/solved items are identical on three scans a few seconds apart. Fold changes from a short settling window into the baseline, and queue remote events until then.
   - The difference detector also needs a stable-world gate. Lessons 34 says the same: the client finishes loading later and early dumps show transient differences.
8. **Simulation/activation range** (how-coop-mods-work §1.7; lessons 36).
   - Measure the Void Engine's NPC processing/activation range. Gothic fact: entities beyond about 40 m are frozen, and state written into them was reset when they were processed again.
   - Far NPC results therefore need a journal applied when the partner's engine activates that entity.
   - Wake rule: each machine must activate the world by the same rule. MDK2 fact: the host woke rooms by its camera, the client by the host's body, and items spawned on one side only.
   - This is directly relevant to the "far NPCs run in both worlds" drift.
9. **Deferred deletes** (lessons 35): send despawns before spawns. Keep a "deleted this frame" set that every lookup honours, because a same-frame name lookup can find the dying object.
10. **Never put process-local handles on the wire** (lessons 46): sound, model and string-table handles and pointers. Send names or a both-sides-filled table slot. Audit the existing D2 packet set for any raw pointer or handle.
11. **Projectiles** (lessons 38): hook the *final launch* step, not the constructor, and send transform, velocity and fuse. While replaying, suppress the constructor's auto-launch. Assert exactly one launch per peer and no damage from the visual copy. Candidates in D2 (inference): crossbow bolts, grenades and spring razors.
12. **Sounds at the engine play call** (lessons 53): hook the engine's play call for actors this machine simulates, de-duplicate clips already played locally within about 1 s, and cap the rate. Mirrored NPCs whose AI is suppressed lose their attack and hurt sounds otherwise.
13. **Cutscenes** (lessons 41): if every machine runs the same scripted sequence, sync start, skip and end only. The host says go, skip is a host-tallied vote, and the next level waits for everyone to reach the end. Messages sent inside the level-load call carry the old epoch and are dropped. Our rules allow cutscene skipping.
14. **Authority hand-off and kill switch** (how-coop-mods-work §2.4 and stage 3).
   - Ship a per-subsystem kill switch like `EnemySyncMirroring` so host-auth for each class (NPCs, doors, pickups, safes, story) can be turned off back to the current mirrored mode.
   - Define a disconnect hand-off rule. OOT fact: pause about 1.5 s, then resume under local AI.
   - Also consider a per-actor adapter interface (`ActorSyncAdapter`) for new archetypes.
15. **Doubled-entity rule** (patching-a-closed-mod-exe §1, OpenOL): when both worlds spawn an AI copy, a deterministic rule (lower id keeps the real one) dedupes during migration. It is a fallback for subsystems not yet host-owned.
16. **Saves with a remote body present** (how-coop-mods-work §1.4).
   - Gothic fact: a save taken with the remote hero copy aborted on load. Verify that D2 saves taken while the Doppelganger rig exists load cleanly, including menu saves and autosaves. If not, hook the save to remove the rig first.
   - Sidecar file per save for co-op state.
   - The joiner loads the host's save sent over the mod connection. This fits the isolated co-op saves scope.
17. **Transitions** (how-coop-mods-work stage 5; lessons 7): re-sync the full snapshot after every load and assume all entity ids change.
   - Exit test: 15 consecutive scripted level transitions with checksums matching after each.
   - Most real bugs were in transitions (map, briefing, stats, retry, disconnect, late join), not in plain play.
18. **Shared vs per-player decision table** (methods §5 "Make progress explicit"): decide per piece of state, for example coins, runes, bone charms, chaos, notes, held items and mission stats. Undecided defaults caused bugs.
19. **Version/mod handshake** (decomp-and-recomp, recomp-net): the room adopts the host's options and refuses mismatches. Compare the DLL build hash and protocol version at join.

### B. Difference detector, flight recorder, chaos, twins

20. **Whole-world diff at every checkpoint** (lessons 34, MDK2): this is the model for our difference detector.
   - Each game dumps players, puppets, enemies with hp, actors, items and doors on command. A script diffs host vs partner, and a sweep visits every checkpoint after a little play. It found six desyncs in its first runs.
   - Dump a few seconds after load. Before fixing, confirm which side is wrong with a single-player run.
21. **Diff self-checks** (methods §7 "Mutation twins"): two dumps of the same state must diff EQUAL, and a known injected change must be named *exactly* in the diff.
22. **Three-way oracle** (methods §7): expected (static), captured (hooks/flight recorder), observed (snapshot diff). Classify every mismatch as an oracle bug, capture gap, harness artifact or real bug.
23. **Runtime "who did this" trace** (methods §3; reverse-engineering "Find out who calls a function, live" and "Find what sets a field").
   - Flight-recorder entries for state setters should log body, value and caller as module+RVA, switched on by an INI and size-capped.
   - For a drifting field with no direct writer, use a hardware write watch (Dr0/Dr7 plus a VEH on the writing thread).
   - For callers through virtual calls, record stacks with a short-lived 5-byte jmp stub that has its own unwind info.
   - Reserve stub pages within rel32 reach **at startup**: the heap later fills ±2 GB of the image.
   - Check prologue bytes before patching so another build is refused.
24. **Pure sync library plus fuzzer with bug twins** (methods §7, The Witness).
   - Put the protocol (events, acks, epochs, gates) behind a `World` interface with a thin game adapter.
   - Fuzz two instances over a lossy link against a fake world that has the real game's timing: loading, state changing by itself after load, effects arriving a fraction of a second after the action.
   - Every protection gets a switch, and a twin that is not caught means the profile never exercised it.
   - Fact: 500 sessions of 150 s ran in 3 s and found a restart flap and a wrong offset before any game ran.
   - Fits our unit-tests setup.
25. **Snapshot catch-up test** (methods §6): act on A, wait for its ACKs, then restart only B. Compare the full shared state read from both processes. A mutant with full-state resend disabled must leave the specific item missing on B.
26. **Chaos** (methods §6 "Packet loss and lag emulation"; how-coop-mods-work stage 6.3): run a lossy, reordering, delaying link and long soaks. Also test a deliberately slow side, because loading screens longer than a timeout dropped connections.
27. **Mutation twins everywhere** (lessons 8; patching §7): only patched-PASS next to twin-FAIL in the same setup counts.
28. **Test-harness hygiene**:
   - assert the mode at run start (lessons 3: a flag silently fell back to SP);
   - poll with a timeout (lessons 4);
   - a missing executable or "is not recognized" counts as FAIL (lessons 5);
   - fail a run that ends early or leaves a crash dump, and log the faulting address with registers from a VEH (lessons 44);
   - delete or rotate the log per run (lessons 15);
   - write our own flushed log;
   - measure frame time, not capped FPS (lessons 3; methods §7).
29. **Movement metric for the rig** (lessons 61):
   - measure the displayed partner position against the nearest point of the sender's *true path* over the last ~1.5 s (path deviation), not only lag;
   - fail runs where the sender barely moved;
   - re-measure stock baselines if a hook refactor changes them (lessons 62).
30. **Animcheck / rig drive** (reverse-engineering "Drive the stand-in's graph with the sender's graph inputs"): drive the rig from the sender's animation-graph inputs and let root motion move it, correcting drift rarely.
   - Fact: per-frame teleport gave a stretched pose, and every-few-frames teleport made it slide with its legs together.
   - Verify that the receive-only side really applies the values; a same-process test hid this.
   - If the rig is pinned by per-frame writes, it must be invulnerable and written every frame with smoothing (lessons 27). Fact: Far Cry 2's pinned puppet took damage and ended the mission.
31. **Evidence lanes** (lessons 42): unique evidence-folder suffixes even for variants started in the same second. This is already enforced by our `check-evidence.sh`; keep the per-variant suffix in mind for parallel takes.
32. **Bots through native input paths** (methods §7 "Unattended menus through a virtual pad"; reverse-engineering "Give QA bots real buttons through the key bindings").
   - An alternative or backup to ViGEm: point the game's `XInputGetState` import slot at a function that adds scheduled presses to whatever a real pad reports. It works unfocused and needs no driver.
   - Or call the engine's key-binding index switch so the input travels the game's own path.
   - Pitfall: stop on the world-start log line, not the first post-menu line.

### C. Lab and isolation

33. **The Steam client writes saves outside the game process** (methods §2 "Isolation must cover the store client"; "Saves can load from the store's cloud API").
   - Facts: `ISteamRemoteStorage::FileWrite` lands in `Steam\userdata\<id>\<appid>\remote` via steam.exe even with cloud off, and `FileRead` can serve the *real* cloud save to a test copy.
   - For us: check whether D2 uses ISteamRemoteStorage (static check via d2-ghidra imports and flat-API calls). If it does, hash the real userdata remote folder and the 14 protected files before and after every lab run.
   - Optionally log read calls with pass-through wrappers on the vtable slots of the interface the game already holds. That does not replace `steam_api64`.
   - The NVIDIA App also rewrote real settings on game start.
34. **Hash-verify protected files around every run** (methods §2 "Redirect the save folder… test it by hashing"; lessons 22): if save/config paths come from `SHGetKnownFolderPath`/`SHGetFolderPath`, redirect them in-process per instance, and make the DLL refuse to start in lab mode if the sandbox resolves to the real folder.
35. **Two GPU instances** (lessons 24): two DX12 instances caused a device-removed driver reset. Apply the lowest preset, cap FPS and check free VRAM. This matches our VM bugcheck history (`0x50` in nvlddmkm), so keep enforcing lab settings.
36. **Background-instance throttling** (methods §2 "Measure the frame rate of a background game"): measure it rather than assume; posting WM_ACTIVATE did not help, but holding the loop's own focus bytes from a Present hook did.
   - When faking focus, also neutralize ClipCursor, SetCursorPos, SetForegroundWindow and GetAsyncKeyState, and mute audio (lessons 23).
37. **Firewall** (methods §2): one elevated rule per lab exe path, limited to 127.0.0.1, needs a single UAC approval. That makes it a Needs-you step if prompts recur.
38. **Process hygiene**: kill only processes registered to our own lab folders, never by image name (lessons 37; methods §2 and §8). Stopping a background bash can orphan `sh.exe` (lessons 63; this matches our memory note).
39. **Memory scans must not fault** (lessons 28): read in chunks with ReadProcessMemory and keep a VEH logger. Find objects by vtable plus position, not by pointer chain (lessons 29; reverse-engineering).
40. **Tool-call correctness**:
   - check `ret N` and calling convention before calling engine functions from hooks (lessons 60);
   - preserve volatile registers when replacing tiny leaf functions in whole-program-optimised code, and test with generated register-loading callers plus a mutant (methods §5, Shadow of War);
   - do no I/O in `DllMain` (methods §4).
41. **Signatures per build** (methods §3; reverse-engineering "What to map"): wildcard only rel32/disp32 targets, grow each signature until it is unique, verify it live at the same RVA, and refuse unknown builds (1.77.9.0).
   - Anchor tiny inlined functions through a unique caller.
   - Bound functions by `.pdata`, not int3 padding (lessons 31).
   - "Called from" entries may be exception-table slots (patching snags).
42. **Self-describing builds** (methods §3 "Let a release build describe itself"): catalogue the console/cvar registry and any compiled-in debug print with channel gates, and hook the print to log the game's own narration.
   - Inference: D2's cvar system could expose AI and script state cheaply. We already have a cvar reader.
43. **Protocol drift between paths** (reviving §3): if lab mode (localhost UDP) and online (Steam lobby) carry separate code paths or packet tables, diff them, or put a logging proxy between the two peers for 10 minutes.
44. **Audit hygiene** (reviving §3; multi-agent; lessons 18): treat read-only agent findings as claims (about 1 in 6 was wrong). Keep a BUGS ledger with fixed, open, **refuted** and out-of-reach entries.
45. **Merging parallel features** (lessons 40): after each merge, list per feature pair the keys read, settings keys written, global names, screen space and window/input hooks. Confirm the test actually loaded the new DLL build.

## 3. Conflicts / not applicable

- **Steam emulation and replacement conflict with "never replace steam_api64"**: Goldberg/gbe_fork (tools §3; split-screen), the mock `steam_api.dll` for local tests and the Spacewar AppID 480 relay (methods §6). Our lab mode uses our own localhost UDP and online uses the game's own `steam_api64`. Do not adopt these.
  - The ISteamRemoteStorage vtable-slot wrapping (methods §2) leaves the DLL in place, but it alters Steam behaviour. Use it only for lab-only logging or read-only checks, never in release builds.
- **Third-party co-op mod code**: the hub cites Tilted, OOT True Co-op, G3 Together, OpenOL, MDK2-coop, FNV and others. Our rule bans ZikQ and third-party co-op mods for D2. Using generic *design patterns* described in this MIT playbook seems fine, but do not fetch, copy or run those mods' code. No D2 co-op mod is mentioned in my slice.
- **Ghidra headless decompile-all, REA (`npx rea-agents setup` edits agent config), GhidraMCP, IDA MCPs**: these conflict with "Ghidra only through the d2-ghidra MCP". d2-ghidra already has debugger, trace and watch tools, which cover the live-caller and write-watch techniques. Prefer those over a home-made Dr7 or stub where they suffice.
- **Downloads** (Frida, x64dbg, Cheat Engine, ProcMon, Nucleus, Sandboxie, DiscordChatExporter, the hub MCP server registration): each needs explicit user permission. None is required.
- **Patching an exe on disk** (patching-a-closed-mod-exe; methods §5 byte patch): never against the Steam install. It is acceptable only in `work/asset-game` copies, and our DLL approach makes it mostly unnecessary. The SHA-256 gate, never-write-input and expect-bytes discipline still apply to any in-memory patch.
- **Offline test copies blocking AFD sockets** (methods §2): would break the Steam lobby path. Not applicable to online takes, and unnecessary for lab mode.
- **Per-instance `steam_appid.txt` and `SteamAppId` env** (methods §2): acceptable inside lab copies only. Never write to the Steam install.
- **"Never desktop capture"** (lessons 12 and 26; methods §7): this tension affects our video evidence takes and `Save-D2LabVMScreen`. If any capture is screen-region based, it can record other windows. The playbook recommends back-buffer capture from a Present hook (or `PrintWindow`/`PW_RENDERFULLCONTENT`). Worth checking how our takes capture (unknown from this slice).
- **Lockstep material** (how-coop-mods-work §1.6, KeeperFX, MW2, emulator netplay): not applicable. D2 is a closed engine and must use host-authoritative state sync, as the page itself concludes.
- **Contribution flow** (AGENTS.md, research "Output", mcp.md, discord-export): not applicable, because we do not contribute to the hub. "Decompiled code is fine to publish" conflicts with our rule that game-derived material stays local. Never publish D2 decomp output, and never put a SteamID in docs.
- **Not applicable**: switch.md, console-emulators.md and decomp-and-recomp.md (console, emulator and decomp routes) apart from the few design points cited above. split-screen.md's in-one-copy rendering and Nucleus launchers also don't apply, except the "player 2's turn" pattern.
- **Engine families**: Void Engine (D2) is absent from engine-families.md, and id Tech 5 is listed as a gap with no base. No reusable framework exists; at most we could add a write-up there, which is not our task.
