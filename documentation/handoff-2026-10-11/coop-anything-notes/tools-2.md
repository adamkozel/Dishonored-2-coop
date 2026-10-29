# coop-anything tools, slice 2: bots, scenarios, bridge, inspect, fuzz, fakeworld, hotreload, gamepool, gameslot

Source: `work/external-references/coop-anything/tools/<name>` (MIT, "Copyright (c) 2026 cranebeaker and contributors";
no per-file SPDX or copyright headers anywhere in this slice). All nine were ported from cranebeaker/fnv-coop
(commits f57aa78 / e85460f, Fallout NV, 32-bit xNVSE). Every tool follows one house pattern: game-independent core,
one adapter class you write, a fake for game-free tests, and "mutation twins" (deliberately broken copies that a
test must catch). Python tools are stdlib only (3.8+); C++ tools are C++17 + CMake, built with VS2022, one job at a time.
Nothing was run or built; everything below is confirmed static (read from source) unless marked hypothesis.

License obligation if we copy code: keep the MIT notice (copyright line + permission text) with the copied
files, e.g. a `third_party/coop-anything/LICENSE` next to vendored headers, or a header comment naming the source and
license. No NOTICE or attribution beyond MIT is required. Our repo is public, so this is the only obligation.

Our context in one line: D2 Void Engine x64 DLL in-process; control today = console `coop_*` commands sent by the
PowerShell lab + status JSON files (`coop-status.json`, `.online.json`); lab UDP on 127.0.0.1, product Steam P2P;
VM is the second PC; evidence = archived takes + video.

---

## 1. tools/bots: multi-bot co-op playbooks (Python)

**Purpose.** Drive 2 to 4 already-running, already-joined game instances with one bot each; after every step settle,
compare a shared snapshot host-vs-client, and check desync counters. Never launches a game.

**Files.** `coopbots/adapter.py` (ActionAdapter base, stubs raise `MissingAction`), `core.py` (runner: `Ctx`,
`Playbook`, `run_playbook`, settle/quiesce/snapshot/counters, failure evidence), `results.py` (`ActionResult`, `Fail`,
`Why`, `ensure`), `playbooks/*.py` (12 playbooks), `registry.py` (playbook table + twin list), `fakecoop.py`
(in-memory host + lagging clients, 30 switchable bugs), `links.py` (impaired-link options; does not impair anything
itself), `refs.py` (refs from a scenario file), `run_coop.py` (CLI), `run_parallel.py` (one runner per host/client
pair as subprocesses), `adapters/fnv/fnv_adapter.py` (~150-line console adapter example), tests (138 unit tests,
27 playbook twins, 15 engine twins via `COOPBOTS_MUTANT`).

**Adapter interface we would implement** (subclass `ActionAdapter`; anything left out makes dependent playbooks SKIP,
never PASS):
- Engine: `status() -> {"seq": int, "synced": bool (host), "counters": {mismatches, repairs, repaired_effects,
  bad_messages, apply_failures, rebases}, "acks": {client: seq}}`; `snapshot() -> {"shared": {...}, "per_player": {...}}`
  (shared must be equal on all machines once synced; empty shared is refused); optional `capture(dir)`,
  `blockers() -> [(code,text)]`, `explain()`.
- Actions (return `Result(ok, reason, data)`; check postconditions with `self.act(do, pre, post, free)`):
  goto_ref, goto_pos, equip, aim_at, fire, activate, open_door, close_door, unlock, open_container, close_container,
  take, put, pickup, drop, talk, pick_topic, end_dialogue, use_device, set_stage, start_quest, barrier_pass,
  answer_prompt, consent, fast_travel, spawn, save_game, load_game, session_leave, session_join, set_timescale.
- Queries: cell, distance_to, inventory_count, death_count, target_alive, ref_enabled, in_dialogue, spectating,
  is_locked, door_open, device_entry_used, quest_stage, stage_done, quest_running, objective_displayed,
  objective_completed, owner_of -> (owner, epoch), ref_count, consent_pending, rebases, timescale.
- Counter semantics: REPAIR_COUNTERS (mismatches, repairs, repaired_effects) may rise only on impaired links (then
  quiet-window + repairs/min budget, budget > 0 needs a written reason); HARD_COUNTERS (bad_messages, apply_failures,
  rebases) must never rise except `allow_rebase` steps.

**CLI.** `run_coop.py --adapter mod:factory --players N --scenario f.json|--refs f.json --playbook name|all
[--link lan|good_wifi|bad_wifi|mobile_tether|transatlantic|hell] [--from-step N|--only-step N] [--timescale X]
[--settle-stall S] [--instances 2,3] [--outdir] [--keep-going] [--no-compare]`; `--fake --bug NAME` for game-free.
Exit 0 = >=1 pass and no fail; all-SKIP exits 1. Output: `<out>/<playbook>/result.json`, `snap/<step>/<machine>.json`,
`FAIL-<n>-<step>/failure.json` (reason, kind, per-machine status, capture, recent_actions, snapshots), `summary.json`.
`run_parallel.py --pairs N [--port-base P] [--preflight mod:fn] [--timeout S] -- <runner args>`: env `COOP_PAIR`,
`COOP_INSTANCES`, `COOP_PORT`; exit 0/1/2/3 (3 = BLOCKED).

**The 12 playbooks (test checklist).**
| playbook | proves | resumable |
|---|---|---|
| fight | all shoot same enemies; dead everywhere, each dies exactly once, enemy count constant | R |
| ownership_handoff | host walks away: control moves once to client, epoch increases, no flapping on return, fight continues, no dupes | |
| loot_race | simultaneous pickup: exactly one winner, party holds exactly 1; drop and race again x2 | R |
| container_share | one "in use" lock; take/put replicate; conservation (nothing duplicated or lost) | |
| door_lock | open/close from host and client; locked door refuses; unlock with key; device entry unlocks box | |
| fast_travel_tether | refused without consent and in combat; all arrive together; tether holds a wanderer | |
| dialogue_spectator | one speaker, others spectate and are released, no second speaker, roles swap, end-script stage everywhere | |
| consent_vote | barrier holds until last player passes, applies once; ready check: decline, guest veto, all ready applies once | |
| consent_vote_guest_declines | veto keeps stage held everywhere | |
| quest_sync | stages and objective flags (displayed/completed) equal everywhere; repeating a stage is a no-op | R |
| save_load_rejoin | client leaves, host advances, rejoin catches up with each reward once; host loads older save, clients rebase; 2nd rejoin adds nothing | |
| spawn_once | host spawns one; exactly one on every machine and it stays one | |

**Fake bugs (30) = bug checklist:** no_stage_sync, no_objective_sync, dup_pickup, no_container_sync,
no_container_lock, door_unsynced, lock_unsynced, device_unsynced, dup_kill, stale_echo (dead actor revived by stale
health echo), double_spawn, spectator_stuck, dialogue_not_bridged, barrier_early, veto_ignored, tether_off,
consent_ignored, travel_in_combat, owner_split, flap_owner, counter_noise, no_fight_sync, no_retransmit,
repairs_in_flight, repair_after_quiet, no_rebase, reward_dup_on_rejoin, client_frozen, timescale_ignored,
speed_not_reset.

**Integration recipe.** Write `tools/lab/d2bots/d2_adapter.py` (`make_adapter(i)`), one adapter per instance that
(a) sends `coop_*` console commands through whatever channel the PowerShell lab uses today (`Send-D2Command`
equivalent; for Python either shell out to pwsh or, better, speak the bridge protocol in section 3), (b) reads
`coop-status.json` for `status()` (map our seq / sync / desync counters onto the six names), and (c) needs a new
`coop_dump_shared` command (or bridge `state`) for `snapshot()` (story flags, doors, safes, pickups taken, far-NPC
state, mission stage; exclude per-player inventory/position). Postconditions must re-read game state (not trust the
command echo). Dishonored mappings: ownership_handoff -> our far-NPC host authority; loot_race -> pickups; door_lock
-> doors/safes/keycodes (use_device = safe combination); consent_vote -> mission-end / level-transition barrier;
quest_sync -> mission objectives; save_load_rejoin -> host load + partner rejoin; dialogue_spectator -> NPC dialogue
(Corvo/Emily partner exclusion fits "no second speaker"); fast_travel_tether is likely N/A (mission transitions instead).
Steps that need real movement/aiming (goto_ref, aim_at, fire) need teleport/aim debug commands in our DLL.
**Effort: M** for status/snapshot + 6-8 actions (enough for quest_sync, loot_race, door_lock, spawn_once, fight);
**L** for all 12. **Complements** our harness: it replaces hand-written per-take PowerShell assertions with
reusable step-by-step convergence checks, but our launch/restore/video-take machinery stays. `run_parallel` is of
little use (one host PC + one VM, not N pairs on one PC).

**Risks/limits.** Snapshot quality decides everything (a too-small `shared` passes vacuously; a per-player field in it
fails always). Settle needs a monotonic host seq applied by clients; we must expose one. Lab-only: Steam P2P product
path is exercised only if the adapter runs against VM+host (the adapter can call into the VM via our
`Send-D2LabVMCommand`). `--timescale` needs a timescale console command.

---

## 2. tools/scenarios: start-state + steps + checks as JSON (Python)

**Purpose.** Describe a starting state and test as one JSON file, validate it game-free, "jump" every instance to the
state by in-place reset (reload the save only when needed), run steps and per-player checks; status
pass/fail/error/partial/invalid. Also the `refs` source for bots.

**Files.** `SCHEMA.md` (format, schema 1), `scenlib.py` (validate, resolve, footprint/plan_jump, compare, DataIndex),
`runner.py` (Runner: jump, steps, checks with polling, event-driven sync waits, popup clearing, prepared saves),
`gameadapter.py` (GameAdapter base), `prepare_save.py` (CLI: make popup-free "ready" saves, manifest
`scen_prepared.json`), `fakegame.py` (virtual-clock fake, flags desync_guest/double_spawn/dup_loot/no_handoff/
popups_persist/name_prefix, `SyncSim`), `scen.py` (CLI list/validate/plan/run), `examples/fnv/` (4 scenarios + adapter).
120 tests, 20 twins (`SCEN_MUTANT`).

**Schema.** Top: schema=1, name (= file stem), title, description, players 1-4, tags, `start.save`, `start.reload`,
`start.prepared`, `reset.keys`, `objects` (alias -> {id, kind actor|thing|item|base|place|state, base?}), `refs`,
`setup`, `steps`. Ops: set_state, move, spawn, give, take, kill, revive, enable, disable, activate, lock, unlock,
wait, save, load, leave, join, action(name,args). Step keys: id, op, on (host|guest|all|index), parallel, note, needs,
dirties. Checks: state, state_reached, item_count, dead, disabled, locked, in_area, distance, stat, follower, count,
owner, value(query) with cmp (== != < <= > >= same), value, agg (sum|min|max), tol, timeout<=120. A missing
capability = `blocked` -> scenario `partial`, never pass.

**Adapter interface.** `OPS` set of `op:x`, `check:y`, `action:z`; `do(op,args)`, `read(kind,args) -> float`,
`load_save`, `save_game`, `reset_state(ids)`; optional `sync_status()` ({role, head, applied, joined, syncing,
connected, rejected, digests{received,ok,mismatch,stale,rateLimited,repairs}, desyncs[...]}), `popup_count`,
`dismiss_popup`, `prepared_names`, `save_exists`, `save_stamp`, `ROLLING_SAVES`, `data_index()`.

**Integration recipe.** Python `D2Game(GameAdapter)` translating ops to `coop_*`/debug console lines and checks to a
`coop_query <kind> <id>` command whose answer lands in status JSON (or via bridge `eval`). `sync_status()` maps
directly onto our join/baseline/applied counters and gives fail-fast "client 0: applied 50 of 100" reasons.
`data_index()` could be built from our Ghidra/resource-reader dumps of entity names. Jump planning is valuable for
us: it reloads only when a prior scenario dirtied what this one touches, cutting load-screen time. Prepared saves map
to our isolated lab saves (must never be the 14 protected files; `save_exists/save_stamp` against lab save folder).
**Effort: M.** **Complements**; it could replace ad-hoc per-take setup scripts with declarative files that double as
bots `refs`. Risk: reset_state must truly reset (Dishonored mission state is broad); unnameable changes (`action`,
`load`) force reloads; checks only see what `read` exposes.

---

## 3. tools/bridge: in-game JSON-lines RPC server + Python client

**Purpose.** A header-only C++17 RPC server embedded in the mod (TCP loopback and/or Windows named pipe) so tests
run console lines, eval expressions, wait for conditions, dump state, load/save, screenshot and read an event log;
plus a robust Python client.

**Files/API.** `include/coopbridge/server.h`: `coopbridge::Adapter` (virtual `capabilities()`, `Exec(line, ref) ->
ExecResult{ok, output}`, `Eval(expr, ref) -> json::Value`, `State(sections)`, `Load(name)`, `Save(name, overwrite)`,
`Screenshot(path)`), `Options` (game, build, tcpPort -1/0/N, tcpHost loopback-only unless allowRemote, pipeName,
endpointFile, defaultTimeoutMs 30000, slackMs 2000, maxCallsPerPump 16, maxPending 256, eventCapacity 1024,
maxLineBytes 4 MiB), `Server::Start/Stop/Pump/Register(name, fn, onMainThread)/PushEvent/NotifyLoaded/NotifySaved/
SetReady/stats`. `json.h`: small Value/parse/dump (UTF-8 sanitising). `bridge_client.py`: `Bridge.connect(endpoint,
wait_s)`, `call`, `ping`, `version`, `methods`, `require`, `exec`, `eval`, `wait_for`, `state`, `state_to_file`,
`load`, `save`, `screenshot`, `events`, `wait_event`; errors RpcError, CallTimeout, WaitTimeout, ConnectionLost,
GameCrashed (pid gone), ProtocolError; CLI exit 0/1/2/3; `COOPBRIDGE_ENDPOINT`, `endpoint_from_file`.
`fake_game.py` (fault injection: delay, wrong_id, garbage, close, split), `cpp/test_server.cpp`, tests: 28 client,
22 C++ server, 13+12 twins (run_tests.py makes broken header copies from outside).

**Protocol (PROTOCOL.md v1).** One JSON object per line; `{"id","method","params"}` -> `{"id","result"}` or
`{"id","error":{code,message}}`; replies in order per connection; ping/version/methods/events on transport thread,
everything else on the game main thread inside `Pump()`. Methods: ping (pid, frames, mainLoopAgeMs, ready,
loadedCount), version, methods, exec, eval, waitFor (expr|cmd, op, value, contains, timeoutMs, pollMs), state
(sections, path), save, load, screenshot, events (since, limit -> events, next, dropped, last; ring of 1024; built-in
game.loaded/game.saved). Errors -32700/-32600/-32601/-32602/-32000/-32001 main-thread timeout/-32002 load-save
timeout/-32003 busy/-32004 stopping/-32010..-32099 game. Endpoint file `{"protocol":1,"game","pid","endpoints":[...]}`.

**Integration recipe.** Vendor `server.h` + `json.h` into `third_party/coopbridge/` (MIT notice), build only in lab
configuration (`#ifdef COOP_LAB_BRIDGE`; README says never ship it). Implement `D2BridgeAdapter`: `Exec` routes to our
existing console-command dispatcher (so every `coop_*` command works unchanged), `State` returns the same object we
write into `coop-status.json` (sections: session, sync, world, player), `Load/Save` call the game's load/save and
`NotifyLoaded/NotifySaved` from our existing load-complete/save hooks, `Screenshot` optional. Call `Pump()` from the
per-frame hook we already have on the main thread. Use `pipeName = "d2coop-<role>"` and `endpointFile` inside the lab
instance folder. `PushEvent` for desync/digest/ownership events (feeds inspect and wait_event). PowerShell side can
keep working: the client CLI exits with codes, or pwsh can speak the pipe directly. VM: TCP needs allowRemote (avoid);
better keep bridge local inside the VM and relay through our existing `Send-D2LabVMCommand`/probe channel.
**Effort: M** (S for exec/state/events; load/save notify needs our hooks). **Replaces** the file-polling half of the
command channel (synchronous replies, real timeouts, crash vs disconnect, no stale JSON reads); status JSON can remain
as the recorded evidence. Prerequisite for bots, scenarios, inspect, gamepool readiness (`named_pipe` probe).

**Risks.** Requires C++ exceptions enabled in the mod build (/EHsc). No auth: lab builds only. Requests run in Pump(),
so a loading screen without frame hooks yields -32001 (good: explicit). One request per connection at a time.

---

## 4. tools/inspect: read-only live table of all copies (Python)

**Purpose.** One column per running copy, one row per field from a JSON field list, refresh with `--watch`, plus a
"problems" list (menu open, copy behind, mismatch counter rose, copies differ). Read-only by construction
(`ReadOnlyBridge`: only ping/version/methods/events/state/eval + listed read methods; exec/save/load/screenshot/
waitFor/quit hard-denied; eval only for expressions whose first word is in `eval_allow`).

**Files.** `inspect_live.py` (Spec, Inspector, CLI), `demo.py`, `examples/demo_fields.json`, `examples/fnv/`
(fields + README), tests (23 checks, 11 twins). Depends on `../bridge/bridge_client.py`.

**Field list schema.** `name`, `eval_allow[]`, `read_methods[]`, `fields[]` {label, source ping|version|state|eval|
events|call, section, path "a.b.0", expr, ref, method, params, types, last, format, template, empty}, `problems[]`
{field, op ==|!=|<|<=|>|>=|nonempty|empty, value, say} or {differs, say}. CLI `--fields F --endpoint name=tcp://..|
pipe://.. --endpoint-file name=path --watch S --count N --json`; exit 0/1/2.

**Integration.** Free once the bridge exists: write `d2_fields.json` (role, mission, lastSeq/applied, acks, digest
mismatch/repairs, menu/UI state, health, position, far-NPC owner, recent desync events). **Effort: S** after bridge.
**Complements** (live debugging view during a take; `--json` snapshots could be archived per take). Without the bridge
it is unusable (it cannot read our status JSON files directly; a hypothesis-level shim would be a fake bridge that
serves our JSON files).

---

## 5. tools/fuzz: coopfuzz, seeded session fuzzer with oracles (C++17)

**Purpose.** Run 2-4 copies of the sync core in one process over a lossy virtual-time link, play a random session,
heal, wait for quiescence, check invariant oracles, shrink a failure (ddmin) to a few ops, write a replayable scenario
+ JSON report.

**API (`include/coopfuzz/fuzz.h`).** `OpKind` Move, Hit, Pickup, Drop, Spawn, Activate, Join, Leave, Save, Load,
SetLink, Stale (slot-number references, modulo live things); `Scenario{seed, machines, durationMs, link, bugs,
profile, target, expect, ops}`; `ITarget` (apply, advance, now, heal, machines, connected, quiescent, view(m) ->
Thing{netId,type,ident,x,y,cell,hp,dead,owner,epoch,holder,open}, digest(m,cat 0..3), repairs, applyLog, ownLog,
stats, markQuiescent, nativeViolations); `TargetBase::logApply(machine, idem, ref, epoch, seq, origin, ApplyKind
Pos|PlayerPos|Hp|Owner|Holder|Door|Spawn|Remove|Reset|Intent, accepted, worldEpoch, why)` and `logOwn(Grant|
Transfer|Release|WorldEpoch, ref, owner, prev, epoch)`; `TargetRegistrar`; `run`, `shrink`, `writeReport`. `net.h`
LossyLink (latency/jitter/loss/duplicate/corrupt; presets lan, good_wifi, bad_wifi, mobile, hell). `rng.h` splitmix64.
CLI `coopfuzz` / `run.py`: --minutes/--count, --seed, --players, --duration, --profile, --target, --bug, --replay,
--fixture, --out, --no-shrink, --max-fail, --list-bugs, --list-targets. Formats: `coopfuzz-1` scenario JSON,
`coopfuzz-report-1` report (violations, stats, ownership_log, apply_log capped 5000).

**Oracles (8):** converge, double_apply, multi_owner, stale_applied, echo_adopted, dup_thing,
repairs_after_quiescence, not_quiescent.

**Bug classes / twins (12, BUG-CLASSES.md):** no_idempotency (double apply), no_retransmit (lost update),
stale_assignment_echo (owner re-applies assignment snapshot, resurrects, rediscovers as new; from Skyrim Together
Reborn PR #906), two_owners, stale_epoch_accepted, stale_seq_accepted, echo_adopted, spawn_dup, pickup_dup,
handoff_respawn, digest_unstable (repairs never settle), no_world_epoch (request from before a host load accepted).
Skeleton target adds no_dedupe.

**Integration recipe.** Only applicable to code that runs without the game: our sync core must take injected time,
injected transport (send callback + receive call), no globals, seeded RNG (GUIDE "What your core must allow"). Write
`tests/fuzz/d2_target.cpp` wrapping our host-authority layer (pickup arbitration, door/safe state, far-NPC ownership
epochs, story flags) with a fake world map standing in for engine entities; log every apply/reject; register; build
with `-DCOOPFUZZ_TARGET_SOURCES=...;-DCOOPFUZZ_TESTS=OFF` as an extra CMake target next to our unit tests; run
`--count 200` in CI on build-input changes only (CI minutes). Re-create our past fixed bugs as twins.
**Effort: L** if our core is entangled with engine calls (needs a seam), **M** if it already is engine-free.
**Complements** (finds protocol races the 2-instance lab never hits; cannot test engine hooks). Risk: oracles see
only what `view`/logs expose; a sloppy `quiescent()` or `idem` creates false failures.

---

## 6. tools/fakeworld: a full fake host-authoritative world (C++17, example to copy)

**Purpose.** Pattern example: model synced world parts as plain data (actors, cells, containers, items, doors, spawn
sites, quests, players), run N `Machine`s on an `InMemoryNetwork` with virtual clock, check oracles. Not a drop-in
library; depends on `tools/ownership` (`coop_ownership`, other slice).

**API.** `Cluster(ClusterConfig{seed, players, tickMs, link, machine})`: join, leave, saveHost, loadHost, step, run,
quiesce, inject, setObserver/setCapture (TraceCapture -> `replay()`), `checkAlways()` (one owner per epoch, one AI
driver per actor, nothing stale, nothing applied twice, no duplicates), `checkQuiescent()` (equal to host by digest,
exact actor state, ownership table equal, item conservation, no repairs after quiescence). `Machine` actions: move,
attackActor, pickup, drop, open/close/take/put container, activate, useDoor, host-only recruit, dismiss, setStage,
setLock, killByStory, commitEffects; hostSave/hostLoad. `Effect` variant (SetStage max-merge, Start/StopQuest,
KillActor, ResurrectActor, MoveRef, Enable/DisableRef, ObjectState, SetLock, SpawnRef, DespawnRef, CompanionOwner) =
"effect capture" story sync. Messages: MFullState, MPose, MVitals, MActorSnap, MActorEvent, MHit, MOwn, MEffects,
MDropReq, MItemCreated, MPickup, MItemTaken, MContainerReq/Res/Sync, MSpawnRoll, MSpawnActor, MSiteState, MActivate,
MDigest, MRepair. `IEngineObserver` hooks (flight recorder). `EngineFaults` test switches.

**Scenario tests (34) = checklist:** join/sync 2-4 players clean and lossy; fight; handoff when host walks away;
guest-to-guest handoff keeps last state; no flapping; companion follows recruiter; loot race with rollback; spawn
once; container lock/take/put/close with conservation; doors, locks, teleports; story NPC unkillable by combat but
killable by story effect; quest stage merge and re-delivered effects; host save/load + guest rejoin; silent
corruption found and repaired by digests; repeated drop request makes one item; forged and misrouted hits rejected;
late joiner gets ownership table; random sessions; deterministic seed; recorded run replays to identical digests;
garbage frames; reliable link ordering/dedupe; guest rejoin.
**Twins (18):** own_transfer_no_ack, own_no_hysteresis, own_follower_ignored, no_dedupe, hit_no_validation,
hit_by_non_owner, pickup_no_arbitration, pickup_no_rollback, spawn_no_arbitration, loot_per_machine,
pick_per_machine, container_no_lock, named_killable, no_session_epoch, heal_disabled, drop_no_idempotency,
join_no_ownership, owner_keeps_publishing.

**Integration.** Use as a design reference and test-shape donor, not as code: our moves toward host authority for
story/pickups/doors/safes/far NPCs map 1:1 to its Effect list, pickup claim/rollback, container lock, spawn
arbitration, session epoch on host load, digest+per-key repair. Concretely: copy the oracle set and the
`scenario passes clean / fails with fault` twin macro into our C++ unit tests around our own sync classes.
**Effort: M** to port the patterns; **L** to build a full fake world of our own. **Complements**. Risk: its
protocol is its own, not ours; copying `Machine` would mean adopting its architecture.

---

## 7. tools/hotreload: swap the mod DLL in a running game (C++17 + Python, Windows)

**Purpose.** Split the mod into a permanent loader and a reloadable core; swap a freshly built core into the running
game in seconds, preserving versioned state; drain proves no thread is inside the old image before unmapping.

**API.** `abi.h` (plain C): `HrLoaderApi{abi_version, size, generation, is_reload, game_dir, core_dir, user_context,
log, register_method, blob_put/get/erase, request_reload, veto_reason}`, `HrCoreApi{init, on_message, on_frame,
before_unload}`, export `HotCore_Init`. Loader: `hotreload_loader.dll` C API `HotReload_Start(HotReloadConfig{size,
core_path, cache_dir, game_dir, pipe_name, log_path, user_context})`, `HotReload_OnFrame`, `HotReload_OnMessage`,
`HotReload_RegisterMethod`, `HotReload_LastError`, `HotReload_Shutdown`; or static `hotreload_host` (`CoreHost`).
Core lib `hotreload_core`: `Participant{name, version, order, canUnload, save, restore, migrate, shutdown, fresh}`,
`COOP_RELOAD_STATE(...)`, `Writer/Reader`, `LeakProbe`, `ManagedThread`, `adoptThread/releaseThread`,
`SetDrainHooks{suspend, resume, extraRanges}`, `BeforeUnload(api)`, `AfterLoad(api)`, `reload.state`. Drain:
`waitDrained(ranges, ignore, timeoutMs)` scans IP and stack return addresses of all threads. Control pipe
(`hotreload_control`): JSON lines, one client, `ping` on pipe thread, `dev.status` (generation, buildId =
crc32hex-size, corePath, cachePath), `dev.reload`. Client `hotreload.py swap --pipe X [--pipe Y] --dll F [--dry-run]
[--force] [--root DIR] [--out]` / `status`; exit 0 / 1 failed (old core kept or rolled back) / 2 refused untouched.
Tests: 37 unit, 16 swap scenarios, 20 twins, 64- and 32-bit.

**Integration recipe.** Our DLL becomes: loader = the thing the game loads (keeps the frame hook, calls
`HotReload_OnFrame`, forwards messages), core = everything else (`HotCore_Init`). Every engine hook (MinHook-style
detours) needs `SetDrainHooks` suspend/resume/extraRanges and a leak probe; every thread (our Steam/UDP net worker,
any Steam callback pump) must be `ManagedThread` or `adoptThread`; sockets/Steam P2P sessions either torn down in
`shutdown` and reopened, or kept as integer handles. Session state (role, peer, lobby, epochs, ledgers) as
`COOP_RELOAD_STATE` participants. Lab only, since product must load a single native DLL through the game's own
mechanism. Use: `hotreload.py swap --pipe d2coop-host --pipe d2coop-client --dll build/core.dll --root
work/lab-instances` between takes instead of relaunching both instances. Evidence rule: build id (CRC-32 + size)
check gives us "the take ran this exact candidate" for the archive.
**Effort: L** (restructure DLL, hook suspend/resume, Steam API objects across reload; steam_api64 callbacks
registered by the core would dangle unless unregistered in shutdown). **Complements** (faster lab loop); never in
release packages. Risks: the game's own threads calling into our hooks block the drain (waits until they leave);
VM needs the same split build; the loader itself still needs a restart to change.

---

## 8. tools/gamepool: run N isolated copies of a game (Python, Windows)

**Purpose.** Hard-linked isolated installs (`provision`), minimized detached launches with slot locking (`up/down/
status/job`), warm pool daemon with leases, host/client pairs with per-pair ports and RAM/port checks, adapter-driven
`swap`, low-cost INI profile, smoke/probe, crash evidence (logs + Windows Application event 1000), measure RAM/CPU.

**Config (one JSON per game).** name, game_dir (read only), instances_root (must not contain/sit in game_dir;
same NTFS volume for hard links), process_name, launch{exe, args, env, window minimized|hidden|normal, spawns_game,
stagger_s, child_wait_s, watch_s}, link{modes, per_instance_files, empty_dirs, per_instance_dirs,
root_skip_suffixes, pinned_files (SHA-256 checked)}, config_files{base, fallback_base, encoding, edits[[sec,key,
val]]}, write_files, protected_paths (never written), ready{type process|window|file|log_line|tcp_port|named_pipe,
...}, crash{logs, markers, wer_match}, collect_logs, lowcost{rows, post_checks}, adapter "file.py:Class", pool{...},
pairs{port_base, copy_gb, reserve_gb, proto udp|tcp|both, edits with {port} {role} {pair} {host} {client}}.
Placeholders `<...>` in game_dir refuse to run. Env per copy: GAMEPOOL_INSTANCE, GAMEPOOL_INSTANCE_DIR,
GAMEPOOL_PORT, GAMEPOOL_ROLE, GAMEPOOL_PAIR.

**Adapter (`gplib/adapter.py`).** `prepare(i)`, `probe(i,pid)`, `after_ready(i,pid)`, `on_release(i)`,
`can_swap(i,pid,payload)`, `swap(i,pid,payload)`. Lease API `gplib.client.lease(owner, n, ttl_s, timeout_s)` ->
`L.instances`, `L.check(i)` (raises if restarted under you), `L.dirty(i, reason)`, `L.renew()`.

**Integration.** Our `work/lab-instances` + `D2Lab` scripts already provision and launch two copies; gamepool would
replace that layer, with config: game_dir = our `work/asset-game` (never the Steam install; `protected_paths` = Steam
install + the 14 protected save/config folders), per_instance_files = Dishonored config files the game writes,
empty_dirs = saves, ready = named_pipe (bridge) or file (our status JSON), crash = our logs + WER "Dishonored2",
pairs.proto udp with our lab port. Dishonored 2 single-instance behaviour, Steam DRM launch and the VM split are
unknowns for this tool (it assumes all copies on one PC; the VM copy is out of its model). The guard_write /
hard-link-refusal and pid+creation-time kill rules are worth copying into our PowerShell even if we keep it.
**Effort: M-L** to replace, **S** to borrow ideas (crashlog WER query, guard_write, pid identity). Mostly
**replaces** existing lab code, so low priority given our harness works and is pwsh-pinned.

---

## 9. tools/gameslot: lease lock for game copies (Python, single file)

**Purpose.** N slots per PC, TTL leases renewed by re-acquire, FIFO waiters, RAM guard (refuse a second game below
`min_free_gb`), pid-scoped kills of an expired owner's registered processes (pid + image name + creation time),
state changes under an exclusive lock file. CLI `acquire --owner X [--slots K|--exclusive] [--ttl] [--wait]` ->
`OK ...; SLOTS 0,1`, `register-pid`, `release`, `status`; exit 0/2/3 busy/4 not owner. Importable functions
acquire/release/register_pid/status. State `GAMESLOT_DIR` or `<repo>/local/gameslot`, config
`gameslot_config.json {slots, min_free_gb, process_name}`. Tests: 18 checks, 13 twins (incl. 6 real racing processes).

**Integration.** Useful if several agents/sessions share this PC's lab (it prevents two takes launching at once and
killing each other's games). Call from our lab start/stop wrappers via pwsh `& python -I gameslot.py acquire ...`, register
each launched D2 pid, release in the restore step (and in `Recover-InterruptedTake`). **Effort: S.** **Complements**.
Risk: it does not know the VM; set slots=2 locally. Its kill on lease takeover is the only side effect.

---

## Recommended order for us (hypothesis, based on reading only)

1. bridge (M): biggest leverage; unlocks inspect (S), scenarios (M), bots (M), gamepool readiness.
2. inspect (S) for live debugging during takes.
3. bots + scenarios (M each): adapters over bridge `exec`/`state`; start with quest_sync, loot_race, door_lock,
   spawn_once, save_load_rejoin, ownership_handoff (far NPCs). Use playbook list + fake bug list as stage-4/5/6 checklist.
4. fuzz (M-L) once the host-authority core has an engine-free seam; port fakeworld oracles/twins into our C++ unit tests.
5. gameslot (S) if parallel agent sessions share the lab.
6. hotreload (L) only if relaunch time dominates; gamepool largely duplicates our existing lab.
