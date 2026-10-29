# coop-anything tools, slice 1: flight, animcheck, chaos, mutants, ownership, mcp, buildlock (+ tools/README, test_all, build_directory)

Source: local clone `work/external-references/coop-anything`, commit 3dbf6dda2d6b (2026-10-28). Every file in the slice was read
with Read/cat; test files and golden data were skimmed. Nothing was run or built. Labels: **[static]** = read in the code,
**[hyp]** = my inference for D2, not verified.

## License (applies to every tool below)

- One root `LICENSE`: MIT, "Copyright (c) 2026 cranebeaker and contributors". **[static]** No per-file headers or SPDX lines
  in any file of this slice (grep for SPDX/Copyright/License finds only README mentions and coop_mcp.py's tool text).
- To vendor: copy the root LICENSE next to the vendored code (e.g. `third_party/coop-anything/LICENSE`), keep the copyright
  line, and record the source commit (3dbf6dda). Adding `// SPDX-License-Identifier: MIT` plus a pointer to that LICENSE
  in each vendored file is optional but tidy. Several READMEs credit fnv-coop (same author, MIT).
- Our rules: vendoring under `third_party/` changes CMake build inputs, so merges trigger the ~3-4 min `unit-tests` CI job.
  Running anything from the clone (including its test runners) counts as running untrusted files until it has been
  reviewed and vendored, so it needs the user's permission. `tools/mcp/__pycache__/coop_mcp.cpython-312.pyc` is a binary
  in the clone; do not vendor or run it.

## Hub files (tools/README.md, test_all.py, build_directory.py)

- `tools/README.md`: index of the 15 sync-test tools. All Python tools are stdlib-only; C++ tools are C++17, built with VS2022
  via CMake; every tool has `run_tests.py` printing `ALL OK` (tests + "mutation twins").
- `test_all.py`: runs `python -I tools/<name>/run_tests.py` for each of 15 suites (chaos, flight, ownership, fakeworld, fuzz,
  gameslot, gamepool, scenarios, bots, animcheck, bridge, inspect, hotreload, mutants, buildlock), passes only if exit 0 and
  last line `ALL OK`. Args select suites. mcp is not in the list (it has a plain unittest file).
- `build_directory.py`: validates `data/directory.yaml`, renders `DIRECTORY.md`; needs PyYAML; `--check` for CI. Hub-only,
  irrelevant to the mod unless we contribute a Dishonored 2 entry back (enums: kind/status/qa/networking; no em dashes).

---

## 1. flight: flight recorder (.cofr) + desync analyzer

**Purpose.** Each machine records what it sent, received, decided, applied, ownership events and state digests into an
in-memory ring, dumped to a `.cofr` file on desync/crash/request. A Python analyzer aligns 2 to 4 files, finds the first
divergence (down to digest bucket, key, value text), prints a causal chain, shrinks (ddmin) and writes regression cases.

**Files.**
- `cpp/observer.h`: the only header hooks need. Enums (RecType, flags, DumpReason, Role, OwnershipOp, MarkerKind, ApplyFlag),
  event structs, and `cofr::IObserver` whose virtuals all default to no-op (so a null/base observer is the release build).
- `cpp/cofr.h/.cpp`: `cofr::Recorder : IObserver`, `Config`, `BucketedState`, `bucketedDigest`, `hash64` (FNV-1a 64),
  `bucketMix` (splitmix64), a C++ reader (`readTrace/parseTrace/parseNet/parseDigest/...`), `Mutations` test switches.
- `cpp/CMakeLists.txt`: `add_library(cofr STATIC cofr.cpp)`, include dir PUBLIC; tests `cofr_tests` (40, 12 twins), `cofr_example`.
  MSVC flags `/W4 /permissive- /utf-8`.
- Python (3.x, stdlib only, tested 3.11): `cofr.py` (reader/writer), `schema.py`, `analyze.py` (align, find_divergence,
  narrow, causal_chain), `shrink.py` (ddmin, CommandOracle, PyOracle, fixture_from_traces), `gentest.py`, `regress.py`,
  `report.py` (self-contained HTML), `trace_replay.py` (in-process replay of `cofr-trace-1` fixtures), `from_failure.py`
  (bots failure.json to repro case), `toy_replay.py` (example hook), `flight.py` (CLI), `schemas/*.json`, `golden/*.cofr`.

**C++ API.**
- `Config{ringBytes=16MB, maxRawBytes=48000, slot, role, protoVersion, label, dir, autoDumpMinIntervalMs=30000, autoDumpMax=8,
  schemaJson, decoder}`; `NetDecoder = std::function<bool(const uint8_t*, size_t, NetMeta&)>` runs only at dump time.
- Recorder: `setClockSource(fn)`, `setFrame`, `setIdentity(slot, role)`, `setSession(session, epoch)`, `setEnabled`,
  `meta(k, v)`, `mapPeer(peerId, slot)`; recording `netOut(to, data, n, channel)`, `netIn(from, data, n)`,
  `captured(CapturedEvent)`, `applied(AppliedEvent)`, `ownership(OwnershipEvent)`, `digest(DigestEvent, flags)`,
  `marker(kind, text)`, `custom(type 32..255, bytes)`, `digestDetail(cat, epoch, seq, entries)`, `entryText(key, vh, text)`;
  dumping `dump(path)`, `dumpToDir()`, `onDesync(what)` (rate-limited), `dumpCrash()` (try_lock), `stats()`.
- `BucketedState::set/erase/digest/entries`: incremental order-independent digest (about 3 us per change on a 200k state).

**.cofr layout [static].** Little endian. 64-byte header: "COFR", u16 version=1, u16 headerSize=64, u8 slot, u8 role
(0 host, 1 client, 2 sim), u8 reason (0 manual, 1 desync, 2 crash, 3 shutdown, 4 test), u8 pad, u32 protoVersion,
u64 wallStartMs, u64 dumpTUs, u64 recordsDropped, u64 recordCount (offset 40), u32 sessionId, u32 epoch, u64 reserved.
Records: 16-byte header (u8 type, u8 flags, u16 payloadLen, u32 frame, u64 tUs) + payload (max 65535).
Types: 1 Meta (str16 pairs, first is `label`; `peer.<id>=<slot>` entries), 2/3 NetOut/NetIn (36-byte prefix: u32 peer,
u8 channel (0xFF on In), u8 mux=first byte, u8 msgType, u8 decode, u32 session, u32 epoch, u64 seqLo, u64 seqHi, u32 rawLen,
then bytes), 4 TxnCaptured, 5 TxnApplied (seq, epoch, origin, index, flags, cause, effectTag, hash), 6 Ownership
(op, u64 actor, epoch, from, to, reason, txnId), 7 Digest (cat, epoch, seq, hash, itemCount, u16 nShards + u32s,
u32 nKeys + (u64 key, u32 valueHash)), 8 Marker (u16 kind, str16), 9 Schema (JSON), 10-31 reserved, 32-255 game-defined.
Flags: 1 truncated, 2 derived, 4 detail, 8 bucketed. Crash-cut tails are tolerated.
Digest algorithm: `shard[key % 256] += low32(splitmix64(key ^ splitmix64(valueHash)))`, hash = FNV-1a64 over shards
seeded with offset basis + itemCount. Marker text `entry <16 hex key> <valueHash> <text>` gives value text; desync marker
text `ref=#<hex>` links an actor.
Schema JSON keys (each replaces its default): game, digest_categories, marker_kinds (1 desync and 4 test-step are fixed),
ownership_ops, ownership_owner_ops (default [0,1]), message_types, txn_messages, apply_flags, restate_mask (default 7),
custom_records {"40": {"name", "fields":[{name, type u8..f64|str16|bytes16|bytes}]}}.

**CLI.** `python -I flight.py print FILE [--type a,b] [--limit N]`, `align F F..`, `diverge F F.. [--json] [--check]`
(exit 1 on divergence), `report F F.. -o x.html [--shrink s.json]`, `fixture F F.. -o fx.json`,
`shrink fx.json (--replay-cmd "prog {fixture}" | --replay-py mod:fn [--replay-path D]) -o s.json [--max-runs --max-seconds
--list-key --tmpdir]`, `gentest s.json --name N [--out DIR --target --flag]`, `regress DIR [hook]`, `schema [FILE]`,
`from-failure [PATHS] [--instances DIR --converter mod:fn --pattern --schema --expect-converges --no-shrink --no-write --json
--report]` (exit 0 reproduced, 1 not, 2 missing). All read commands take `--schema FILE`.
Replay hook contract: exit 0 converged / 1 diverged / 2 bad fixture, optional last JSON line `{"diverged","violations","first"}`
or `VIOLATION <rule> <detail>` lines; shrink keeps a subset only if the failure signature is unchanged.
Divergence kinds, by priority: txn-hash, txn-double, txn-missing (2 s grace), ownership-conflict, ownership-stale, digest,
desync-marker. A host commit (`captured`, kind 0) counts as "applied" on the host.

**Thread safety / cost [static].** Every record call takes one mutex and memcpy's into the ring; Net records have no
allocation, but captured/applied/ownership/marker/digest build a small `std::vector` (heap) per call (README's "no allocation"
is only true for Net). Frame is an atomic. `dumpCrash()` proceeds without the lock if held (data race tolerated) and still
allocates (vectors, std::filesystem, fopen), so it is unsafe from a vectored exception handler with a corrupt heap.
`dumps_/dumpFailures_` are bumped outside the lock. Benchmarks claimed: 0.01-0.2 us idle frame, 7-22 us busy frame,
asserted < 100 us per frame.

**Integration recipe for D2 [hyp].**
1. Vendor `cpp/observer.h, cofr.h, cofr.cpp` into `third_party/coop-anything/flight/` and add `cofr.cpp` to the DLL target
   (or `add_subdirectory` but its CMakeLists also builds tests/example; better a small own target). Needs C++ exceptions on
   (it uses try/catch). One `Recorder` per process, created at session start, `IObserver*` passed to hooks; null in product
   unless a lab flag (e.g. `coop_flight 1`) enables it.
2. Hooks: transport layer wrapper used by BOTH Steam P2P (`SendMessageToUser`/receive loop via the game's steam_api64) and
   lab UDP: `netOut/netIn` with the application payload bytes (identical on both ends, so pairing by blake2b hash works).
   Peer ids must be slots (host 0, client 1): never put a SteamID into `peer` or Meta (privacy rule). `setFrame` from the
   per-frame game tick hook. Host: `captured()` per committed story/pickup/door/safe txn with a stable `seq`; clients:
   `applied()` per effect with a content hash (`hash64` over effect bytes, never pointers). Ownership: map our messages
   (see ownership section; enum values differ). Digests every few seconds at a quiet point via `BucketedState` keyed by
   stable ids (e.g. FNV-1a of entity name/decl name, story-flag name), and on our own mismatch call `digestDetail` +
   `entryText` + `onDesync`.
3. Schema JSON in code: our digest categories (story, npcs, pickups, doors, safes, players), our message type table,
   `txn_messages`, custom records for game-specific events (e.g. type 40 `ScriptWrite`/`QuestFlag` with a `seq` or `ref`
   field so they join causal chains). Optional decoder that parses our wire header (msgType, session, epoch, seq range).
4. Dump dir: `work/lab-instances/<role>/flight/` (host) and `C:\D2Lab\ws\...` on the VM; harness copies dumps into the take
   folder as evidence (new take folder per run). Then `flight.py diverge host.cofr client.cofr --check` and
   `report ... -o take/flight.html` as an audit step. Clock offsets are estimated from bidirectional message pairs, so host/VM
   clock skew is handled; one-way-only traffic falls back to wall clocks with a note.
5. Raw datagram bytes are stored (up to 48000 B): check our payloads contain no SteamID/persona names before committing
   dumps; the repo hook's SteamID scan will catch IDs, but keep `.cofr` binaries out of git per the manifest size rules.

**Limits.** Records only what hooks report; ring keeps newest records (old desyncs show as "lost N old records");
fixtures from traces replay what each machine received, not host decisions; retransmit pairing is a guess.
Tests: `python -I tools/flight/run_tests.py` (`--quick`, `--no-cpp`, `--no-twin`; `COFR_BUILD_DIR`), cmake >= 3.20.

---

## 2. animcheck: animation and pose checker (Python only)

**Purpose.** Reads per-machine JSONL probe streams and flags single-machine glitches (sliding, tpose, floating, sinking,
jitter, rubberband, stuck, deathpose) and owner-vs-observer mismatches (pose error after root alignment, position, facing,
anim group mismatch, wrong weapon/aim/facing/stance/dead episodes > 400 ms). PASS/FAIL with assertions, HTML and JSON
(`animcheck.report.v1`). Python 3 (tested 3.11), stdlib only.

**Files.** `cli.py` (analyze, collect, demo, profile, selftest), `model.py` (Actor/Stream, JSONL IO), `profile.py`
(game profile), `config.py` (Thresholds dataclass; `ANIMCHECK_MUTANT` twin env), `detectors.py`, `compare.py`,
`analyze.py`, `counters.py` (assert specs), `collect.py` (TCP collector), `report_html.py`, `synth.py` (synthetic streams),
`examples/fnv/profile.json`, tests.

**Probe format [static].** One JSON object per line: `t_ms` (game clock, monotonic; required unless every line has `rx_ms`),
optional `rx_ms` (collector clock), `machine`, `frame`, `v`, and `actors[]`. Actor: `ref` (int or hex string, stable),
`pos` [x,y,z] **Z up**, `rot` [pitch, roll, yaw] (rad unless profile says deg), recommended `cell`, `dead`, `anim.seqs[]`
({slot, group int|string, state 1=playing}), `bones` {name: world [x,y,z]} (10-16 bones, same set on both machines,
>= 3 common besides root), optional `valid3d`, `player`, `first_person`, `weapon {out, aiming, form}`,
`stance {knocked, swimming, ragdoll, sneaking}`, `move_flags`, `ground_z`, `ai`. Sample 10-30 Hz (gap > 600 ms breaks streaks).
Collector protocol: TCP, newline JSON, request `{"method":"anim.probe","params":{}}`, reply = one probe or `{"error"}`;
optional `anim.stats` for probe cost.
Profile JSON: name, bones {root, left/right_upper_arm, left/right_hand, ...}, groups (list or sparse dict), categories
(idle, walk, run, dodge, turn, aim, attack, reload, jam, equip, hit, death, jump, block, misc), category_prefixes,
default_category, angle_unit, units_per_meter (thresholds written for 70/m and scaled), move_flag_bits {sneak, swim},
thresholds overrides.

**CLI.** `python -I tools/animcheck/cli.py analyze [name=]host.jsonl [name=]client.jsonl --profile P --html --json
--avatar "player=host:14>client:FF000A" --assert "avatar.pose_err_p95<=12" --expect-moving REF:T0-T1
--suppress KIND:REF:T0-T1 --delay-ms 100 --latency-ms 150 --auto-offset --owner NAME --pose-tol`. Exit 0 PASS, 1 FAIL,
2 bad args. Default asserts: glitches==0, wrong_weapon==0, wrong_facing==0, pose_err_p95<=tol, plus avatar.* equivalents.
`collect --connect host=127.0.0.1:9100 --connect client=IP:PORT --seconds 10 --hz 20 --out DIR`. `profile P.json`. `demo`.
Python API: `analyze(streams, th, owner, ref_maps={observer: {src_ref: obs_ref}}, expect_moving, suppress, auto_offset,
avatars, profile)`; `counters.check(report, spec)` (unknown counter = failure).

**Integration recipe [hyp].**
- We implement a read-only probe in the DLL on the game thread: per tick (or on request) sample our players' Doppelganger
  puppets, the local player, and nearby NPCs: position, yaw/pitch, playing animation state per layer (needs RE of the
  Void animation controller), joint world positions (needs RE of the skeleton/joint transform API), weapon drawn/aim,
  dead/ragdoll/knocked, crouch. Either append JSONL to `work/lab-instances/<role>/anim.probe.jsonl` (simplest; then use
  `--auto-offset` or our own synchronized clock) or serve `anim.probe` on a TCP port and run `collect` from the host,
  which polls both instances and the VM and stamps one `rx_ms` clock (VM RTT adds some noise; the stamp is the request
  midpoint).
- `ref` must be equal across machines for NPCs: use our net id (same id we use for ownership), not entity pointers.
  Avatars: `--avatar corvo=host:<hostPlayerRef>>client:<puppetRef>` and the reverse for the joiner's character.
- Write a D2 profile: bone names from the D2 skeleton, animation state names as groups (string groups allowed), categories,
  units_per_meter (Void/idTech units per meter unknown: measure a known distance first), angle unit.
- Major gotcha: the local player is first person. The format requires the full third-person body in `bones`; if D2 does not
  animate a third-person body for the local player, report `valid3d:false` for it, which disables pose comparison for
  avatars; then compare only position/facing/anim-state/weapon fields that our sync sends.
- Tune on a clean single-machine run first (unknown groups list must be empty, otherwise `sliding` fires falsely).
- Tests: `python -I tools/animcheck/run_tests.py` (15 twins).

---

## 3. chaos: network impairment relay + C++ mirror

**Purpose.** UDP relay between client and host applying a seeded impairment model (latency +/- jitter, base loss,
Gilbert-Elliott burst loss, reorder, duplicate, bit corruption, bandwidth cap with tail drop, one-way partition, timed
disconnect), live-controlled over JSON-RPC, with a JSONL packet log. `cpp/impair.h` is a header-only bit-exact mirror for
in-process/simulator use (cross-checked over ~36,000 decisions).

**Files.** `impair.py` (model, `Params`, `Verdict`, SplitMix64 `Rng` seeded by FNV-1a of `"<seed>:<label>"`), `relay.py`
(`Relay`, `RpcServer`, `ChaosClient`, `PacketLog`), `chaos.py` (CLI), `client.py` (script helpers), `profiles.json`
(source of truth), `gen_header.py` (writes/checks `cpp/profiles_gen.h`), `cpp/impair.h`, `cpp/xcheck.cpp`, `xcheck.py`,
`cpp/CMakeLists.txt`, tests, `examples/fnv/README.md`. Python 3.8+, stdlib only.

**Profiles [static].** lan 0.5+/-0.2 ms; good_wifi 12+/-4, 0.5% loss; bad_wifi 35+/-25, 2% + bursts (enter 1%, exit 25%,
60%), 2% reorder, 0.5% dup; mobile_tether 70+/-40, 2 Mbit/s 300 ms queue; transatlantic 90+/-8, 0.3%; hell 200+/-150, 8% +
bursts 90%, 10% reorder, 5% dup, 512 kbit/s. Delays are one way per direction; each has a fixed seed; directions `c2s`/`s2c`
use independent streams.

**CLI / RPC.** `chaos.py profiles`; `chaos.py relay --target host:port [--listen 0] [--bind 127.0.0.1] [--profile]
[--seed] [--rpc-port 0] [--log f.jsonl] [--info-file f.json] [--seconds N]` prints `{"listen","rpc","log"}`;
`chaos.py call --rpc-port P <method> '<json>'`. RPC (newline JSON-RPC 2.0 over TCP, loopback only): status, stats,
reset_stats, profiles, set_profile {profile, seed?, dir?}, set_params {params, dir?}, partition {dir, on, duration_ms?},
disconnect {duration_ms}, reconnect, log_path, shutdown. `client.py`: set_profile, partition, disconnect, stats,
packet_log_path (rpc = port, "host:port" or info-file path). Packet log header
`{"format":"coopchaos-packetlog","version":1,...}`, per-datagram `t, dir, peer, n, len, head (16 bytes hex), verdict,
reason, copies, corrupt`; event lines.
C++: `coopchaos::profile(name)`, `Impairment(params, "c2s")`, `Verdict decide(nowMs, size)` (`delaysMs` empty = drop,
2 entries = duplicate), `partition(bool)`, `disconnectUntil(ms)`, `reconnect()`, `flipBit(vec)`, `setParams(p, reseed)`.
`COOPCHAOS_MUTANT` macro defaults to 0 (harmless).

**Topology constraint [static].** The relay is a client-initiated star: it binds a listen port, creates one upstream socket
(ephemeral port) per client source address, forwards to the fixed `--target`, and sends replies back to the client's
source address. So: host listens on a known port and must reply to the observed source endpoint; client must be pointed at
the relay port.

**Integration recipe [hyp].**
- *Lab UDP mode:* our lab peer is hard-coded to 127.0.0.1 (and presumably fixed ports per role). Make the lab peer
  endpoint configurable per role (console arg or lab-instance config, e.g. `coop_session lab --peer 127.0.0.1:<port>`),
  start `chaos.py relay --target 127.0.0.1:<hostPort> --info-file take/chaos.json --log take/chaos.jsonl`, point the client
  at `listen`, and make the host reply to the recvfrom source endpoint instead of a configured peer port (the relay's
  upstream port is ephemeral, so a host that sends to a fixed client port bypasses the relay; two relays back-to-back
  do not fix this). Validate on a source address change rather than rejecting unknown ports. Drive profiles/partitions from
  the PowerShell harness via `chaos.py call` (python -I) and archive the packet log with the take. The same relay with
  `--bind 0.0.0.0` could sit between host and VM over UDP only if a UDP (non-Steam) cross-machine mode exists; today it
  does not.
- *Steam P2P (product and the VM two-account path):* the relay cannot see Steam P2P (Valve relay/encrypted traffic). Use
  `impair.h` in-process: wrap our transport interface (both Steam and lab UDP backends) with one `Impairment` per direction
  plus a delay queue keyed by our clock, flush due messages each tick, controlled by a lab-only console command
  (`coop_chaos profile bad_wifi [seed]`, `partition s2c 3000`). Guard with a mutex (not thread safe) and compile it only into
  lab builds or behind a flag. Caveat: dropping/reordering a message we sent on a Steam *reliable* channel models a fault
  Steam never produces; apply loss/reorder/duplicate only to unreliable sends, and only delay/partition/disconnect to
  reliable ones (or impair at our own reliability layer if we have one).
- Keep `profiles.json` + `gen_header.py` with the vendored header so profiles stay identical to the relay; `/fp:precise`
  (their CMake) keeps C++ and Python bit-identical.
- Limits: Python relay benchmarked only below ~1000 pps; Windows 1 ms timer requested (`timeBeginPeriod`); UDP only;
  wall-clock in relay vs virtual time in impair.h.
- Tests: `python -I tools/chaos/run_tests.py [--no-twins] [--no-cpp]` (needs cmake for the cross-check, else SKIP).

---

## 4. mutants: runtime-switchable mutation twins

**Purpose.** Keep a deliberately broken variant behind a switch so one build serves all twins; a runner checks that each
twin makes its named test fail.

**Files / API [static].**
- `coop_mut.h` (header-only, C++17 std only): with `COOP_RUNTIME_MUT` defined, `COOP_MUT_IF("name"|number)` is true when
  env `COOP_MUT` (read once via `_dupenv_s`) or `--mut NAME`/`--mut=NAME` (`coop_mut::init_from_args(argc, argv)`, blanks the
  args) selects it; `COOP_MUT_ANY()`; `coop_mut::set/current/any/is`. Without the define both macros are `(false)` and the
  binary contains neither twin names nor "COOP_MUT" (tested). `is()` is `noinline` and does a std::string compare per call;
  `set()` is not thread safe.
- `coop_mut.py`: `mut(name)`, `ENABLED`, `set`, `active(name)` context manager, `init_from_args(argv)`, `current`, `any_active`.
- `run_twins.py TABLE.json [--jobs N] [--only ..] [--verbose] [--json F] [--list]`. Table keys: label, cmd (with {test},
  {twin}, {dir}), env_var (default COOP_MUT), build, twin_build (compile-time twins, COOP_MUT set in env; rebuild after),
  real_regex, caught_regex, timeout_s (300), jobs (2), cwd, scan (globs; reports UNLISTED/STALE twin names), tests,
  twins {name: test | [tests]} or list of objects. Results: caught / MISSED / WRONG TEST / BUILD CRASH; loader errors
  (0xC0000135 etc.) never count as caught; a hang counts as caught. Last line `<label>: ... ALL OK`; exit 0/1/2.

**Integration recipe [hyp].** Put `coop_mut.h` in our `src/` (or third_party) and use `COOP_MUT_IF` at fixed bugs in pure
sync code (txn apply, ownership checks, decoders). Compile the code under test twice: unit-test target with
`target_compile_definitions(... COOP_RUNTIME_MUT)` and the shipped DLL without it; add a guard (hook or CI step) that
fails if the release DLL contains the string `COOP_MUT`. A twin table per unit-test exe; our test exe must run one test by
name and print a parseable failure count for `real_regex`/`caught_regex`. Never use it in in-game code paths that run in
product. Tests: `python -I tools/mutants/run_tests.py [--no-cpp] [--build-dir]`.

---

## 5. ownership: actor ownership with epochs (C++17 library)

**Purpose.** Host decides which machine simulates each actor; every change bumps an epoch; all machines judge updates
(accept / reject with reason) so stale owners and echoes cannot win. No network, no clock, no deps.

**Files.** `include/coop/types.h` (`PlayerId` u8, `kMaxSlots` 8, `kHostPlayer` 0, `kNoPlayer` 0xFF, `hash64`),
`include/coop/ownership.h`, `src/ownership.cpp`, `CMakeLists.txt` (`coop_ownership` STATIC, cxx_std_17; tests only when top
level, `COOP_OWNERSHIP_BUILD_TESTS`), tests (minitest, scenarios, 7 twins), DESIGN.md.

**API [static].**
- `OwnMsg{Kind Grant=1, Transfer=2, Ack=3, Commit=4, Release=5; netId u32; epoch u32; owner; prevOwner}`;
  `encode(m, out)` / `decode(data, n, m)` (kind byte, LEB128 id, LEB128 epoch, owner, prevOwner; strict).
- `Presence{player, connected, inCell, adjacent, distance}`; `OwnershipConfig{minHoldMs 1500, switchMargin 0.25,
  handoffTimeoutMs 1000}`; `pickOwner(...)`: eligible = connected and (inCell or adjacent); host wins when eligible; else
  nearest (ties to lowest slot); hysteresis.
- `OwnershipService` (host): playerJoined, playerLeft (returns orphans, emits Release/Grant/Commit), track, untrack,
  evaluate, setRecruiter (follower exception), onAck, tick (timeouts: forced Commit or Grant back), snapshot, current,
  ownerOf/epochOf/handoffPending/forEach/stats.
- `OwnershipView` (every machine, host too): `apply(msg)` -> Applied/Duplicate/Stale (idempotent, order independent),
  `check(id, from, epoch)` -> Accept/NotOwner/StaleEpoch/FutureEpoch/NotYetActive/EchoOfSelf/UnknownActor,
  `mayPublish(id, epochOut)`, `handoffIncoming(id, epochOut)`, `digest()`, clear/forget/forEach/stats.
- `OwnershipFaults` + `OwnershipFaultScope`: process-global test switches read in production paths (all false by default).

**Integration recipe [hyp].**
- Link `coop_ownership` into the DLL (add_subdirectory; tests stay off as a subproject). Call only from the game thread,
  or wrap in a mutex (not thread safe). Messages go over our existing reliable channel; use `encode/decode` or our serializer.
- D2 has no "cells": define `inCell` = player loaded in the same mission and alive in session; `distance` = world distance.
  With both players in the same mission the host is always eligible, so `pickOwner` always returns the host: the stock
  rules give pure host authority. That matches our target (host authority for far NPCs, pickups, doors, safes). If we
  ever want client-simulated near NPCs we must change the host-preference rule (fork `pickOwner`), since host preference
  wins whenever the host is eligible.
- The immediately useful part is `OwnershipView` + epochs: stamp `(netId, epoch)` on every actor/object update and drop
  anything not `Accept`; on `FutureEpoch` ask the host (`current`). Ids must be stable 32-bit net ids shared by both
  machines (a table built at mission load; a truncated name hash risks collisions).
- **Save/load:** epochs restart at 1 with a new service. D2 quickloads, mission loads and checkpoint restores happen often:
  add a session/load counter to every message and drop older ones (DESIGN.md says so explicitly), and send `snapshot()`
  to the joiner after `view.clear()`.
- Flight mapping: cofr default op codes are grant 0, transfer 1, release 2, rejected 3-5, but `OwnMsg::Kind` is 1..5. Either
  write a schema with our own `ownership_ops` (`{"1":"grant","2":"transfer","3":"ack","4":"commit","5":"release"}`) and
  `ownership_owner_ops: [1, 4]` (Grant, Commit; Transfer does not change the owner yet), or translate at the hook.
- Tests: `python -I tools/ownership/run_tests.py [--build-dir]` (30 tests, 7 twins).

---

## 6. mcp: Co-op Anything MCP server (hub knowledge, not for the DLL)

**Purpose.** stdio JSON-RPC 2.0 MCP server (no SDK), protocol "2025-06-18", serving the hub's directory, playbook, skills,
game write-ups and tool READMEs, plus two local analysis tools. Python 3.9+, PyYAML for directory tools.
Tools: search_directory(query, status, qa, engine, limit), detect_engine(game_dir) (signature scan, depth < 4, 60k files,
also steam_api / anticheat hints), game_identity(exe_path) (SHA-256, PE machine/timestamp/sections, version resource,
Steam appid/buildid from appmanifest), list_docs, read_playbook(page), get_skill(name), get_game(slug),
get_test_tool(name), validate_entry(entry_yaml). Path escape guarded in `read()`.
**For us [hyp]:** optional knowledge MCP in `.mcp.json`; `detect_engine` has no Void Engine signature (expect "unknown").
`game_identity` only reads the exe and the appmanifest (no write to the protected install) and is a quick way to log
exe SHA-256 + Steam buildid in take metadata; its output contains absolute paths with the username, so keep it out of
commits. Test: `python -m unittest tools/mcp/test_coop_mcp.py` (needs the hub's data/ and PyYAML).

---

## 7. buildlock: lease lock for a shared build tree + CMake driver

**Purpose.** Several agents share one build tree safely. Python 3.8+, stdlib only.
**buildlock.py [static]:** lock = folder `<tree>.buildlock` made with atomic mkdir; `lock.json` {owner, label, pid,
pid_stamp (process start time via GetProcessTimes), token, acquired, heartbeat, lease_s, max_hold_s}; heartbeat thread every
lease/3 (default lease 120 s, max hold 7200 s); stale if pid gone, pid reused, heartbeat older than lease, held too long, or
no lock.json after 5 s; breaks only under a `.breaking` guard and only if the judged token is unchanged; FIFO tickets in
`.queue` (dropped when dead or idle > 30 s); only the token holder releases. API `BuildLock(path, owner, label, lease_s,
max_hold_s, poll_s, log).acquire(wait_s)` as context manager; `LockTimeout`. CLI `run --tree T [--owner --label --wait-s
--lease-s --max-hold-s --poll-s --quiet] -- cmd` (exit = cmd's, 3 timeout, 2 usage), `status [--json]`,
`break [--only-if-stale]`. Owner from `--owner` or `BUILDLOCK_OWNER` (never a personal name).
**build.py:** strips CXX/CC/CMAKE_TOOLCHAIN_FILE/CMAKE_GENERATOR/ANDROID_NDK* (unless `--keep-env`), loads vcvars via vswhere
for Ninja/Makefiles when `cl` missing (`--msvc auto|on|off`, `--vcvars`, `BUILDLOCK_VCVARS`), Ninja if on PATH, skips the lock
when `ninja -n` says no work, configures only when args change (`.buildlock-configure.json` stamp), builds, prints
`BUILD-TIME`. sccache (`--sccache auto|on|off`, `--sccache-stats`) adds compiler launchers, disables PCH, and sets
`CMP0141=NEW` + `CMAKE_MSVC_DEBUG_INFORMATION_FORMAT=Embedded` (/Z7, CMake >= 3.25); not usable with a Visual Studio
generator. Never installs sccache.
**For us [hyp]:** useful if several agents/sessions build `src/` or deploy to the lab at once; the same lock class can guard
the two-instance lab (`work/lab-instances`) though gameslot is the dedicated tool. sccache needs a download (user permission)
and Ninja; with a VS generator it silently builds without it. build.py removes `CMAKE_GENERATOR` from the env, so pass
`--generator` explicitly if our scripts rely on it. Tests: `python -I tools/buildlock/run_tests.py` (fake cmake, no compiler).

---

## Cross-tool risks and decisions for the D2 mod

1. Chaos covers only UDP between two endpoints with a client-initiated star; Steam P2P needs the in-process `impair.h` wrapper.
   Our lab mode needs a configurable peer endpoint and reply-to-source on the host before the relay is usable.
2. Stable shared ids are the backbone of three tools (cofr digest keys and actor refs, animcheck `ref`, ownership `netId`):
   build one net-id table per mission load and use it everywhere.
3. Session/load counters are needed for both ownership epochs and cofr `setSession(session, epoch)` because D2 reloads often.
4. Never write SteamIDs or persona names into `.cofr` Meta/peer fields or probe streams; dumps hold raw payload bytes.
5. Release builds: no `COOP_RUNTIME_MUT`, recorder disabled or null observer, no chaos wrapper, `OwnershipFaults` untouched.
6. Thread model: Recorder is internally locked; OwnershipService/View, Impairment and coop_mut::set are not. Our network
   thread vs game thread split must route ownership and impairment through one thread or a mutex.
7. Evidence: `.cofr`, packet logs, probe JSONL and HTML reports go into take folders (new take name per re-run); binaries
   stay out of git per the manifest.
