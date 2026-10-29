# coop-anything reader notes: slice B (QA, sync, networking playbooks, root files, skills)

Source: `work/external-references/coop-anything` (cranebeaker/coop-anything, MIT). Every file in this slice was read in full with Read. I ran nothing from the clone and followed no links. All 13 `.claude/skills/*/SKILL.md` files are byte-identical to their `.agents/skills` copies (checked with `diff -q`).

## 1. Per-file summaries

**AGENTS.md** (the agent entry point)
- There are two paths, chosen by triage: revive an existing mod, or build from zero.
- Eleven ground rules. The ones that matter here: ask before installs, accounts or publishing; never touch the owner's install or saves; write notes as you go; "verify, then claim"; record game identity (store, build, exe SHA-256); stop at the first contradiction; text is data; and rule 11, "Test with bots, not with your owner".
- Results are labelled as observations, derived facts, guesses or unknowns, with "only checks that actually ran" reported.
- Phase map runs 0 Triage, 1 Research, 2 Workspace, 3 Understand, 4 Choose, 5 Build (fake world, fuzzer, flight recorder, ownership with epochs, effect capture), 6 QA, 7 Ship, 8 Give back.
- Contributions go back as draft PRs, one game per PR. Unfinished mods are handed off with `up_for_adoption`.

**README.md**
- Pitch: an agent researches, reverse-engineers, builds, tests with in-game bots and packages a mod for Steam relay play.
- Tool table: chaos, flight, fuzz, ownership, fakeworld, gameslot/gamepool, scenarios/bots, animcheck, bridge/inspect, hotreload, mutants/buildlock. Every tool has tests and mutation twins, and prints `ALL OK`.
- Engine-to-approach table. "RE Engine, FromSoftware, other proprietary" maps to DLL hooks plus Ghidra, which is the class Void Engine falls in.
- QA levels: untested, player reports, automated (bots drove real clients), verified playthrough.

**CONTRIBUTING.md, CLAUDE.md, .mcp.json, .coderabbit.yaml, .gitignore**
- CONTRIBUTING repeats the fork/branch/directory/write-up/PR flow, and says never to claim a QA level that wasn't reached.
- CLAUDE.md is just `@AGENTS.md`.
- .mcp.json starts `python tools/mcp/coop_mcp.py` (directory search, engine detection, playbook reading).
- .coderabbit.yaml turns CodeRabbit off entirely.
- .gitignore lists `/local/`, `__pycache__/`, `build/` and `coop-bots-out/`. Private owner state lives in `local/`.

**.github/**
- The PR template has checkboxes for directory, write-up and playbook, an evidence section (QA level, harness, client count, assertions), and a privacy, assets and owner-review checklist.
- `workflows/directory.yml` runs `build_directory.py --check` and the MCP unit test on PRs and on pushes to main.
- Issue forms: "claim a game" (game, plan, repo) and "new co-op project" (link, what is *actually* shared, whether you played it). `config.yml` points to Discord.

**playbook/automated-qa.md**
- Policy: no human playtests. A needed human test counts as a gap to automate, not a step. The bot lives in the game's own scripting or code, never in fake input. Run 2+ real clients and diff their shared state. Use the game's own logs and screenshots, never desktop capture.
- Harness shape: isolated installs; a launcher that prints `RESULT: PASS/FAIL`; an in-game bot (wait for N players, act through the player RPC path, wait, assert); a suite list plus runner; one port and one install per parallel agent.
- An example table across 9 engines shows how each bot works and what its oracle is. Examples: Gothic 3's flag-file bot plus invariant monitors; Outlast's decoding UDP proxy; Trithius with its scorer self-test and an observer agent.
- Roughly 35 hard-won rules: mutation twins, hollow passes, crash-at-exit, polling, instruments that lie, silent teleports, the "connected is not live" check, positive process ownership, SKIP vs PASS, one-shot triggers consumed by earlier cases, and late-join dead code.

**playbook/animation-sync-checks.md**
- "Looks right" becomes numbers: `pose_err_p95` after aligning root and facing; anim/group mismatches; wrong weapon, aim or facing; stance mismatch; glitches (slide, T-pose, float, jitter, rubberband); `pos_err_p95` that allows for interpolation delay.
- Steps:
  1. A read-only probe writes JSONL at 10–30 Hz. Every raw read is guarded.
  2. A profile holds bone names and the anim group table, dumped from the game rather than guessed.
  3. A clean single-machine run calibrates the profile and thresholds.
  4. A two-machine run uses `--avatar owner:id>observer:id`.
  5. The assertions become suite tests.
  6. The detectors have their own mutation twins.
- A findings-to-cause table, e.g. jitter means two writers, rubberband means late or out-of-order packets, tpose means the graph isn't driven or the skeleton was swapped.
- Pitfalls: first-person arms-only bodies (read the third-person skeleton); 0 comparisons is not a PASS; clock offsets; ~100 ms delay with a ±150 ms search; probe cost; `units_per_meter`.

**playbook/effect-capture-story-sync.md**
- Only the host runs story scripts. Hooks record every primitive write as an ordered, numbered transaction (an "effect list"). Clients apply it directly and never run the script.
- Both naive fixes fail. Replaying triggers duplicates rewards and spawns, and makes RNG diverge. Sending only state numbers loses the side effects.
- Three authorities: story belongs to the host, actors to one owner each (with an epoch), and each avatar to its player.
- Effect vocabulary of about 50 kinds. Integer deltas, `before` values, recipient masks, a cause tag, and name-based ids rather than load-order ids.
- Story log with dense seqs: in-order apply, gap buffering, contiguous acks, exponential-backoff resend (a live send must not reset the retransmit timer), and ack-paced catch-up.
- Three epochs (world, intent, ownership) plus a per-connect session id. Clients gate scripts and use an echo guard, a "set without side scripts" path, and an allow-list for per-machine scripts.
- Intents run host-side in an "acting player" context, exactly once per intent epoch.
- Merge rules by type and a share policy kept as data. Join paths: fresh join, resume from N, or a hash-summary baseline diff when joining with one's own save; a host rewind triggers a rebase.
- Desync healer: 128-shard digest, then compare, repair, log, count. Also covered: wire versioning, a game-free test order, an NV worked example, limits, and a 10-step checklist.

**playbook/desync-hunting.md**
- Loop: record, dump, find the first divergence, narrow, reproduce, shrink, keep as a test, prove the tools can fail.
- Record raw datagrams (decode later), effects with content hash, seq and index, host decisions, ownership changes, digests at quiet points, and markers. Overhead budget is about 100 µs per frame.
- Hashes must be stable: content not pointers, sorted maps, floats hashed by bits.
- Bucketed digest: 256 buckets of summed (key, valuehash), updated incrementally. Dump per-key detail on a mismatch. Compare only at equal (category, epoch, seq).
- A finding-type table: `txn-hash` means nondeterminism; `txn-double` and `txn-missing`; `ownership-conflict` and `ownership-stale`; a digest that differs while all effect hashes match means a local write outside the sync path.
- ddmin shrinking keeps the same failure signature, uses independent steps, and has a pinned final check. `gentest` writes a test that fails until the fix lands.
- Pitfalls: clock offsets need two-way traffic; peer id is not slot; a small ring loses the start; lag is not loss; traces contain names and chat.

**playbook/actor-ownership.md**
- One simulating owner per actor, decided by the host: host first, then the nearest player in the same or a neighbouring cell. Every grant, transfer or release bumps the epoch.
- Updates are accepted only from the owner at the current epoch. Older epochs, non-owners and own echoes are dropped; a newer epoch means asking the host.
- Messages are idempotent and order-independent.
- Handoff is a handshake: Transfer, then the old owner sends its final state, then the new owner Acks, then Commit. The new owner starts AI only after Commit. A timeout lets the host force the outcome.
- Anti-flap: hold for 1.5 s and only move for a 25% closer challenger. Leavers release at a higher epoch. Joiners get the full table, including released actors.
- Follower exception: a recruiter owns its companion wherever it goes.
- Bug class: stale or echoed assignment (Skyrim Together PR 906, an NPC duplication storm). Never rebuild or respawn an actor during a handoff.
- Library API (`OwnershipService`, `OwnershipView::check/mayPublish/handoffIncoming`). A session counter covers host loads.

**playbook/fake-world-testing.md**
- Model only the synced state as plain data. Seeded world build, virtual clock, lossy link (latency, jitter, loss, dup, bit flips) with a reliable layer on top. Machines take a single `doAction` entry point, run as a cluster, and are compared by quantized digests with repair.
- Oracles valid at every step: one owner per epoch, nothing stale, nothing applied twice, no duplicate spawns or drops.
- Oracles valid after `quiesce`: equal to the host, same ownership table, item conservation, zero repairs.
- Scenario list: grab races, container lock, three players loading one cell, the host leaving mid-fight, a story kill vs combat, late or duplicate messages, rejoin, save load, silent corruption, and a random session.
- Twins are one `if` behind a flag. A weak scenario was exposed when a twin stayed green: count only the deliveries made after the handoff under test.
- Build order: data, then link, then host machine, then oracles before the second feature, then a scenario, flag and twin per feature, then the random session last.

**playbook/fast-iteration.md**
- A 15-minute manual round becomes 1–2 minutes by removing waits, not by speeding up code: core-only build with a cache, hot swap instead of restart, prepared saves, event-driven settle, `--from-step`, `--timescale`, and failures that state their reason.
- Pieces: bridge, loader/core split, hard-linked copies with their own ports, self-explaining bot actions, resumable playbooks, parallel pairs, `from_failure.py` for offline repro, twins.
- Rules: never restart to pick up a core change; no fixed sleeps; a failure without a reason is a test bug; take the gameslot lease.

**playbook/fuzzing-coop-sync.md**
- A fake session of 2–4 cores in one process. A seeded random plan (move, hit, pickup, door, join, leave, save/load, link degrade, stale replay). Then heal, freeze, wait for quiescence and compare, with oracles and a shrinker (400 ops becomes 3–10).
- Oracles: converge; nothing applied twice; one owner per epoch with monotonic epochs; nothing stale; no echo; no duplicates; repairs settle; "quiet means quiet" (traffic stops by itself).
- Adoption order:
  1. Make the core instantiable N times, with injected time and transport and seeded RNG.
  2. Wrap it as a target.
  3. Fix the adapter first, and trust results only after 200 clean sessions.
  4. Twin a past bug.
  5. Add coverage counters, since a counter stuck at 0 means an untested defence.
  6. Run 2 minutes per change and long runs overnight.
- Lessons: shrink before reading; the most specific broken rule is reported first; log rejections as evidence; rare twins need more seeds; never loosen an oracle.

**playbook/hot-reloading-your-mod.md**
- A permanent loader DLL plus a reloadable core mapped from a cache copy.
- Versioned state "participants" go into a blob, with `migrate` or a declared fresh start.
- A drain waits until no thread's IP or return address is in the core.
- Build id: CRC-32 plus size of the mapped copy. A refusal is clean, leaving the old core running.
- Core-owned threads must be registered (`ManagedThread`, `adoptThread`), otherwise parked workers block the drain forever.
- Mistakes table: unregistered statics; layout changed without a version bump; a `save` that mutates; hooks not removed; sockets closed (keep the handle in the blob); chained hooks; game or loader pointers into the core; C++ types across the boundary; static destructors joining threads; building over the live core.
- Code patchers need `suspend` (restore bytes), `extraRanges` (trampolines) and `resume`, plus a leak probe for hooks.

**playbook/in-game-bridge.md**
- An RPC server inside the game: a transport thread receives newline JSON over loopback TCP or a named pipe, a main-thread queue is pumped once per frame, there is a method registry, and a game adapter does the work.
- Rules:
  - Game calls run only on the main thread, and a test proves it.
  - `ping` is answered from the transport thread with the time since the last frame, so "loading" can be told apart from "dead".
  - Timeouts on both sides; numbered requests so late replies are dropped.
  - Errors are preserved, never swallowed.
  - Per-frame cap and queue cap.
  - `waitFor` polls on the game side.
  - "Method not found" is normal.
  - Absolute paths and valid UTF-8.
  - Loopback only, test builds only.
- Method order: ping/version, exec, eval, waitFor, state, events ring, load/save (refuse the owner's save names), screenshot, custom. Read-only methods are kept apart from mutating ones.
- The read-only `inspect` table has one column per copy.
- Build order: client plus fake game first, then a tiny C++ host, then the real adapter. Twins exist for every guard.

**playbook/multi-instance-testing.md**
- Short list: take a slot; give each copy a hard-linked private install; solve single-instance checks in-process or in a separate session; launch detached, minimized and sequentially with a readiness probe; kill only by pid plus creation time; hash the owner's files before and after.
- Hard links are the same file. Every written file must be private (delete, then write), so refuse files with link count > 1, and hash the exe and main data.
- Mutex and FindWindow checks are fixed by a gated in-process rename hook. Never patch the exe, and never close another process's handle.
- `CREATE_BREAKAWAY_FROM_JOB`. Find the real child under a loader by process tree. Start minimized without activation, but a minimized UE3 start stalled until it was restored once.
- RAM floor of 3 GB. Warm pool with generation numbers. Crash triage by exit code, logs and Event ID 1000.
- Test the harness itself with a decoy process of the same image name and twins.

**playbook/network-chaos-testing.md**
- Failure table per link problem: loss, bursts, reorder, dup, corruption, jitter, small pipe, one-way outage, disconnect. Loopback hides all of them.
- Three tool layers: engine or library emulation, system-wide clumsy, and the seeded scripted UDP relay with a C++ mirror (`impair.h`). The relay and the mirror share one RNG stream, so "profile X, seed N" means the same in both.
- Order: simulator, then real copies via the relay, then outages at chosen steps, then keep failing seeds.
- Profiles: lan, good_wifi, bad_wifi (the main bug finder), mobile_tether, transatlantic, hell.
- Scenario template:
  1. Assert the mode and that the relay counts traffic both ways.
  2. Set the profile.
  3. Act through the player path.
  4. Optionally partition s2c or c2s.
  5. End the chaos and wait for quiet (host last seq equals client ack).
  6. Diff the copies.
  7. Assert that the chaos really happened (drops and dups counted).
- Assert convergence, no double apply, no stale win, exactly-once requests, bounded repairs (0 on lan), and a documented recovery time after a 5 s blackout. Game timeouts are expected outcomes to assert.
- The relay belongs on the loopback leg, never on the Steam leg.

**playbook/networking.md**
- Default is host-authoritative state sync with client-owned avatars. Lockstep and rollback are rarely realistic for mods.
- Techniques:
  - quantization, delta compression, priority accumulator;
  - reliable for events, unreliable for movement, never reliable-ordered for movement;
  - send-time stamps with an adaptive playback delay (p90 lateness plus one interval, clamped 20–200 ms, snapping on big jumps; Obscure);
  - a fixed 60 Hz send tick, because per-frame sending at 275 fps meant about 600 packets/s.
- Story-sync layering table, and late join, level transitions and saves. Load mid-session: hook the single "request load" site; the host sends load plus the save-transfer id, and the guest waits for the file.
- Transport and library survey, connection options (Spacewar, EOS, VPN), and the local-proxy pattern.
- Test matrix: join/leave/rejoin, kill -9 client, kill host, map change, late join mid-quest, save/reload, loss profiles, a 30–60 minute soak with periodic hashes.

**playbook/networking-steam-spacewar.md**
- AppID 480 trick: flat C API via LoadLibrary/GetProcAddress, manual callback dispatch, callback ids (333, 337, 504, 506, 513, 703, 1251, 1252) and struct layouts.
- x64 `SteamNetworkingMessage_t`: data at 0, size at 8, sender identity at 16. On 32-bit the identity is at 12, not 16. Only a real two-account test found that.
- Accept sessions in 1251 (the host accepts lobby members, clients accept only the host).
- No disconnect event exists: control channel 1 carries goodbye, kick and a 1 s keep-alive; drop after 20 s of silence, on LobbyChatUpdate flags (0x2/0x4/0x8/0x10) and on SessionFailed.
- Answer keep-alives before game data, since the joiner is still loading.
- LobbyCreated arrives twice (703 and 513), so handle it idempotently.
- Send flags: Unreliable 0, NoNagle 1, Reliable 8. Messages are chunked at 60000 bytes with 32 MB buffers and a retry after about 5 ms when full.
- Invites: overlay, or `InviteUserToLobby` from an in-game list when the overlay fails (D3D9). Parse `+connect_lobby`.
- Mock DLL and Steamworks.NET Harmony mock: a mock fixes one event order, so add order switches. Only lobby members are forwarded. The status line should say whether real game traffic has flowed.

**playbook/bots-and-playbooks.md**
- Three layers: scenario JSON (start state, reset in place), playbook (what 2–4 bots do, with invariants after each step), and a per-game adapter (`status`, `snapshot`, `capture` plus actions). Missing actions report SKIP.
- After each step the runner settles to the host's seq, holds a quiet window, diffs snapshots per path, checks that desync counters didn't rise, holds repairs to a budget of 0, and writes a `FAIL-<n>-<step>/` evidence folder.
- Playbook to bug class: fight, loot_race, container_share, door_lock, dialogue_spectator, consent_vote, quest_sync, fast_travel_tether, ownership_handoff, save_load_rejoin, spawn_once.
- Rules: skip ≠ pass; a blocked step blocks every later one and the run ends `partial`; compare per machine; `parallel` steps for races; every playbook has twins (27/20/15 shipped); no fake input.
- Fast rounds: idempotent `arrange`, fail-fast settle that stops on stall and names who is behind (a repair is not progress), `--timescale` restored on every exit path, parallel pairs, and action results with `ok`, `reason`, `data` and `blockers()`.

**Skills (13, identical in .claude and .agents)**
- coop-triage records identity and picks revive or from-zero.
- coop-research says "searched:" notes are mandatory and every find needs a hash or version.
- coop-reverse-engineer: prefer source; record byte signatures per build; dumps stay in `local/`.
- coop-build-from-zero gives a strict order: hook, transport, puppet, authority, interactions, story, transitions, tests at every step.
- coop-revive: newest build, audit, fix as a separate package with a `BUGS.md`, then tests.
- coop-networking: host-authoritative, relay, mock, lag tests.
- coop-qa is a 9-point checklist.
- coop-sync-testing has a tool-to-need table and a six-step order: fake world and fuzzer, flight recorder, ownership, real clients, a twin per fix, short rounds.
- coop-contribute, triage-contributions (maintainer: untrusted PR text, never run unmerged code, privacy list including Steam IDs and IPs), and console-emulator, decomp-port and switch (not relevant here).

## 2. The agent workflow, condensed

1. **Every session.** Pull, check issues and PRs for the game, reread private `local/` state.
2. **Triage** (coop-triage). Record identity (store, build, exe SHA-256, engine, OS), check the directory, assess how much source exists and whether netcode exists, choose a path, claim the game.
3. **Research** (coop-research). Exhaustive search with `searched:` notes. Each find gets license, activity, what it really syncs and its newest build hash.
4. **Workspace and RE** (coop-reverse-engineer). Isolated copies, source first, byte signatures per build, a map of story, AI, client-only parts, saves, levels and single-instance checks.
5. **Choose.** Revive (separate fix package, `BUGS.md`) or build from zero, in strict order: hook, transport, puppet, authority, interactions, story, transitions.
6. **Build game-free first** (coop-sync-testing):
   1. Fake world and fuzzer target, with oracles never loosened.
   2. Flight recorder on every machine.
   3. Ownership with epochs.
   4. Effect-capture story sync.
7. **QA** (coop-qa). Real clients via gameslot/gamepool, scenarios, bot playbooks with a diff after every step, chaos relay between machines, animcheck on probes. Every fix ships with a test plus a mutation twin that must go red.
8. **Gates.**
   - "Verify, then claim."
   - Stop at the first contradiction.
   - Assert the mode before the result.
   - A skip or blocked step is never a pass.
   - A PASS with 0 comparisons is not a pass.
   - Assert that the chaos happened.
   - Harness and scorer self-tests.
   - Owner files hashed before and after.
9. **"Done"** means the QA level is honest:
   - `automated` requires a suite that ran, with the command and summary pasted.
   - `verified-playthrough` is the top level.
   - Human playtests are a gap to automate, not the plan. Agents that play like people (keyboard and mouse with an observer agent) find UI and focus defects the gates miss.
10. **Give back** with a draft PR the owner reviews (directory, write-up, playbook fix with the game as evidence), scrubbed of personal data and assets.

## 3. Applies to us (D2 co-op, host-authority migration)

### 3.1 Story sync: replace "run in both worlds and mirror" with effect capture
- **Diagnosis** (effect-capture §1). Our current P2P mirroring of story, pickups, doors and safes is the "replay the trigger on every client" failure mode, so drift is expected, not incidental.
- **Target** (§2, §7). Story scripts run only on the host.
  - On the partner, hook the Void script dispatch point and suppress it.
  - Keep an allow-list of per-machine scripts: HUD, fades, objective UI, Heart and Outsider-mark UI, achievements, rumble, plus anything that writes only local presentation. Nothing those scripts change may be sent as shared state.
- **Capture at the write, not the call** (§16 step 3). Find the primitive writers in Ghidra: mission objective/stage set, script or global variable set, entity enable/disable/spawn/kill, door lock and open, safe state, item give/remove, chaos and kill counters, and lever or electrical state (wall of light, arc pylons, watchtower states). Survey before guessing (§15): dump every command used by the mission scripts and log unknown commands loudly.
- **Apply path** (§7). A "set without running side scripts" path for each effect kind, or a hook that suppresses the side scripts while an echo-guard flag is set.
- **Intents** (§8).
  - A partner activates a door, lever, safe keypad, pickup, trigger volume, Jindosh lock or Clockwork Mansion lever. It sends `intent{epoch, n, kind, target}` and applies only the UI part locally.
  - The host runs the interaction in an "acting player" context, so script references to the player resolve to the partner's Doppelganger avatar. This also fits our "partners excluded from each other's direct interactions" rule, because the host simply refuses those intents.
  - The result comes back as a normal transaction.
- **RNG once** (§2, §9). Loot rolls, bonecharm crafting traits and any random spawns are made on the host and sent as results.
- **Merge rules as data** (§9): stage = max, flags = OR, kills = union, globals = host wins, mutually exclusive outcomes = first committed wins. Examples of the last: the Jindosh/Sokolov choice, Stilton saved or not, and Delilah's fate.
- **Share policy as data**, with one flag per category. Decide for chaos level, coins, runes, bonecharms, mission stats and blueprints. Mission-end boat transitions and other points of no return go through a party barrier (`consent_vote` playbook). This needs the user's input, as the page says to decide it with the owner.
- **Rewards** (§4, §10). Integer deltas with a recipient mask. A per-player reward ledger on the host avoids double grants after rejoin (§15 "rewards after a restart").

### 3.2 Epochs, sequence numbers and session ids
- Three epochs are needed (effect-capture §6):
  - **World epoch:** bump on every host load. D2 players quickload constantly, so this is the most frequent rewind trigger.
  - **Intent epoch:** bump when the partner's world is replaced.
  - **Ownership epoch:** bump on every ownership change.
- Add a per-connect **session id** echoed in every message, so stale datagrams from a previous Steam session or lab run are dropped (effect-capture §6; actor-ownership "Using the library": epochs restart at 1 in a new service).
- Story log (§5): dense seq, in-order apply with gap buffering, cumulative ack, exponential-backoff resend that a live send must not refresh, and ack-paced catch-up.
- Wire envelope (§12): magic, version, type, body, CRC. The hello and reject messages are version-independent and append-only. At join, compare protocol version, build, rules-file hash and a content hash, and give a readable reject. Bound every count and reject non-finite floats.

### 3.3 Ownership of NPCs (combat and nearby now; far NPCs next)
- Adopt rules 1–9 from actor-ownership.md:
  - The host decides.
  - Host first, then the nearest player in the same or an adjacent streamed section.
  - Updates are accepted only from (owner, current epoch).
  - Handoff is Transfer, then a final state, then Ack, then Commit, with a host timeout.
  - Anti-flap is 1.5 s and 25%.
  - Leavers release at a higher epoch.
  - Joiners get the whole table.
- Far NPCs that only the partner has streamed are the "cells only one machine has loaded" limit (effect-capture §15). Either the host keeps that section simulated, or the partner owns those NPCs under host-granted epochs.
- Follower exception, applied to D2 (actor-ownership "follower exception"):
  - A possessed NPC or animal belongs to the possessor for the whole possession.
  - Domino-linked NPCs and Doppelganger clones must be host-arbitrated effects, because one player's action changes NPCs the other may own.
  - Both of these are our inference.
- Never respawn or rebuild an NPC during a handoff (Skyrim Together #906 lesson). Test it with a stale-assignment replay and its twin.
- Combat (networking §1.3): the attacker reports a hit with a hit id, and the target's owner applies it, exactly once.

### 3.4 World-diff "difference detector", flight recorder, desync hunting
- Digest design (desync-hunting §2; effect-capture §11):
  - 256-bucket additive digest of (stable key, value hash), maintained incrementally.
  - Keys are stable D2 entity names or ids plus a category, never pointers. Instances allocate differently.
  - Floats are quantized or hashed by bits.
  - Compare only at the same (category, world epoch, seq) and only when quiet.
  - On a mismatch, dump per-key detail lines.
  - Repair from the host, skipping keys touched since the digest, only when the epoch matches.
  - Count repairs per minute by category; the target is 0 in scripted takes.
- Flight recorder (desync-hunting §1):
  - Ring buffer of raw datagrams, effects with content hash and seq, host commits, ownership changes, digests and markers (join, save, load, mission change, quickload).
  - Decode at dump time. Budget is ≤100 µs per frame, enforced by a test.
  - Dump on a detected desync, on a crash and on request (an in-game command plus a `coop_*` console verb).
- Reading findings (desync-hunting §3): a digest mismatch with equal effect hashes means a local write outside the sync path. Exactly that is what our suppressed partner world should never produce, so every such finding is a missed suppression hook.
- Shrinking (desync-hunting §4–5): steps are independent with absolute times, the final check is pinned, and ddmin keeps the failure signature. The regression test goes in before the fix.

### 3.5 Late join, reconnect, saves, quickload
- Fresh join (effect-capture §10) is a snapshot plus log. A reconnect resumes from the partner's last contiguous seq N.
- A partner joining with its own isolated co-op save gets a baseline transfer:
  1. The partner sends (key, hash) chunks.
  2. The host streams back force-set effects and revert keys.
  3. Chunks are idempotent and acknowledged.
  4. Floats are canonicalized to 32-bit and rounded.
- A host rewind (quickload) bumps the world epoch, and the partner rebases (§10). Document the sticky one-way flags that can't be unset (§15).
- Load mid-session (networking §1.4, Obscure): hook the single site where D2 requests a load. The host broadcasts "load" plus a save-transfer id, and the partner waits until that file is in its isolated co-op save dir. Partner saves are a no-op or a separate slot. This fits our isolated-saves and Leave-co-op scope.
- Test matrix (networking §5.3): join, leave, rejoin; kill -9 the partner; kill the host; mission change; late join mid-mission; save and reload while connected; a 30–60 minute soak with periodic hashes.
- Late-join dead-code check (automated-qa, last bullet): a fake peer sends state and a second peer joins afterwards and lists what it receives. Add a blind-mirror twin.

### 3.6 Steam P2P lifecycle (we use the game's own steam_api64)
- x64 `SteamNetworkingMessage_t`: size at offset 8, sender identity at 16 (Spacewar "Struct layouts"). Verify against our code. Log which offset decodes a non-zero id.
- Accept sessions in callback 1251 only for lobby members, and forward traffic only from current lobby members (Spacewar "HXCoop pattern").
- Liveness ("Messaging rules"):
  - No disconnect event exists. Use a control-channel keep-alive at 1 s, drop after 20 s of silence, and also drop on LobbyChatUpdate flags 0x2/0x4/0x8/0x10 and on SessionFailed (1252).
  - Answer keep-alives while the joiner is still loading.
  - Handle LobbyCreated arriving twice (703 and 513) idempotently.
  - Parse `+connect_lobby` for a cold Join Game.
- Large snapshots (baseline, story log catch-up): 60000-byte chunks and raised buffer configs, with a retry after about 5 ms when full. Pace them so live traffic stays first (chaos §1, "small pipe").
- On exit, leave the lobby and clear rich presence. The status line should say whether real game traffic has flowed, not just "connected".
- A mock or lab transport fixes one event order. Our lab mode (localhost UDP) can hide Steam ordering races, so add order switches in lab mode and keep the VM two-account run as the layout and order oracle (Spacewar "Unity Mono" last bullet; Jericho offset story).
- Use a fixed send tick, not per-frame sends (networking §1.2). Use a send-timestamped adaptive playback delay for the Doppelganger and NPC puppets: p90 lateness plus an interval, clamped 20–200 ms, snapping on Blink or Far Reach style jumps.

### 3.7 Chaos network tests
- Lab mode is already loopback UDP, which is exactly where the chaos relay belongs (chaos §8: "between the two ends of your local tunnel, on loopback, not on the Steam leg").
- Our own seeded relay on 127.0.0.1 between the host and partner lab instances should implement:
  - the six profiles;
  - one-way partitions;
  - a packet log with verdicts;
  - stats that the take asserts on, so a "pass" with zero drops is hollow.
- Use the same seeded impairment model inside the unit-test simulator, so "profile X, seed N" replays in both places (chaos §3).
- Per-take assertions (chaos §6): convergence after quiet (host last seq equals partner ack), no double apply, no stale win, exactly-once intents, repairs 0 on lan and bounded on bad_wifi, and recovery time after a 5 s blackout written down. D2's own connection timeout under `hell` is an expected outcome to assert.

### 3.8 Animation checks (animcheck) for the Doppelganger rig
- Probe (animation-sync "Steps"): JSONL from our DLL at 10–30 Hz, with guarded reads, covering id, position, rotation, active anim, third-person bones, weapon and stance.
- First-person pitfall: D2 renders first-person arms only. Probe the third-person rig on the owner side, or the comparison reports a T-pose for the local player.
- Stances to cover: crouch/sneak, lean, mantle/climb, Blink/Far Reach (add to `--suppress` or treat as snaps), choke and carry body, sword block, pistol/crossbow aim, Emily's Shadow Walk and Corvo's Bend Time.
- Assertions: `avatar.compared>0` (otherwise not a pass), `pose_err_p95`, `glitches==0`, `wrong_weapon`, `wrong_aim`, with a shared clock or auto offset and `--delay-ms` matched to our interpolation buffer. Set `units_per_meter` for Void Engine. The scale is unknown, so this is a hypothesis to measure.
- Jitter means two writers (findings table). If native AI or physics on the Doppelganger and our position sync both move it, one must stop. This is relevant because the rig is a native NPC.
- The animcheck run complements video evidence. Visual acceptance stays separately recorded.

### 3.9 QA harness rules worth adopting verbatim (automated-qa "Rules learned the hard way")
- **Mutation twins** for every fix: a runtime switch that restores the bug, and the take must go red. They also catch test bugs. For example, one-shot D2 story triggers consumed by an earlier case make a before/after pass on both builds; order cases or use repeatable objects.
- **Assert the mode first:** lab vs online, 2 players, the mission/map, and host vs partner role.
- **Assert preconditions:** the partner really is in the trigger volume, and the enemy really attacked (a log line). A teleport can fail silently.
- **"Connected is not live":** count the partner as alive only while a value it alone advances keeps moving.
- **Crash-dialog detection:** EnumWindows, `#32770` class, child text. A crash at exit is still a crash.
- **Invariant monitors** at the end of each take, sampled several times seconds apart: simulated on both, dead on one only, health differs, mirror running its own AI, body with no motion.
- **Control run:** the host does the same action alone, which gives a single-player differential (effect-capture §13.5).
- **Pid-scoped kills** with creation time (multi-instance §6). Never taskkill by image name, since that could kill the VM's or the user's game.
- **Config override context manager** that always restores, then compares against a backup. This matches our "restore the lab after every test".
- **SKIP with a reason** when the case never happened, rather than PASS.
- **Scorer self-test** as the first gate. A timeout is reported as TIMEOUT, not FAIL.
- **Wall-clock dwell** when using timescale or cutscene skip.

### 3.10 Bridge, bots and fast iteration
- Bridge (in-game-bridge §2): `Send-D2Command` should go through a main-thread queue. `ping` answered off-thread with ms since the last frame tells loading from hung, which is useful for VM takes. Also needed: numbered requests, preserved errors, per-frame caps, `waitFor` on the game side, an `events` ring (loaded, saved, desync, message box), and a read-only method group for an inspector. `save` and `load` must refuse protected save names.
- Bots and playbooks (bots-and-playbooks): map to D2 playbooks:
  - fight;
  - loot_race (rune, coin, bonecharm);
  - door_lock (doors, levers, safes);
  - dialogue_spectator;
  - consent_vote (mission end);
  - quest_sync (objectives);
  - ownership_handoff;
  - save_load_rejoin (quickload);
  - spawn_once.
- Each playbook settles to the host seq, diffs per step, holds repairs to a budget of 0 and writes a FAIL folder into the take. Use idempotent `arrange` for `--from-step`, fail-fast settle that names who is behind, and action results that carry reasons and blockers.
- Hot reload (hot-reloading): a loader/core split for our DLL would skip restarting two lab instances plus the VM per round. Use the drain with registered threads (the net thread and the Steam callback pump). Keep Steam lobby and session state in the blob, not torn down. Hooks are removed in `suspend` with a leak probe.
- Game-free core (fuzzing §3, fake-world): make the host-authority core instantiable N times with injected clock, transport and RNG. Run the oracles in our existing unit-test executables. Coverage counters show which defences fired (stale rejects, dedups, handoffs). Trust results only after 200 clean sessions.

## 4. Conflicts and not applicable (our rules win)

- **Mock `steam_api.dll` swap and the Steamworks.NET Standalone download** (Spacewar "mock DLL", "Getting a steam_api.dll"; automated-qa "Second Steam account"; networking §5.2). These are forbidden: steam_api64 is never replaced, and downloads need permission. We already have the equivalents: lab mode on localhost UDP for one PC, and the Hyper-V VM's second account for real Steam. The page's "a real two-account test is a separate, human step" does not apply to us, because the VM automates it (except a lost login, which is Needs-you).
- **AppID 480 Spacewar and writing `steam_appid.txt`** are not applicable. We use the game's own Steam identity, and writing into the install would modify the Steam installation.
- **"Test with bots, not with your owner" / "Needs a human playtest is a gap, not a step"** (AGENTS rule 11, automated-qa Policy). We adopt the automation, but the human playtest stays our final Needs-you step, and visual acceptance is recorded separately. It is never marked done without the user.
- **Hard-linked instance installs** (multi-instance §1). A write through a hard link modifies the original Steam install, which is protected. If we ever adopt this, `work/asset-game`-based copies stay the source, and we enforce the link-count > 1 refusal plus delete-then-write. Hashing the owner's files before and after a take is compatible, and recommended for the 14 protected files.
- **Reusing the `tools/` code** (chaos, ownership, flight, fuzz, animcheck, hotreload). The code is MIT, but it is generalized from fnv-coop, which is a third-party co-op mod. Under "no third-party co-op mods", treat it as design reference and write our own implementations, or ask the user before vendoring any of it. Do not run anything from the clone.
- **clumsy/WinDivert or REA via `npx`** need downloads and installs, and WinDivert is a kernel driver, so it touches system settings. Prefer our own loopback relay. Ghidra stays MCP-only.
- **Exe patches for single-instance checks, or closing another process's mutex** (multi-instance §2) are forbidden by both their rules and ours. Only a gated in-DLL hook in lab instances is acceptable.
- **"Never desktop capture"** (automated-qa). Our video evidence takes must capture the game window only, never the Steam overlay or friends list. That is our SteamID and friends privacy rule. Flight traces hold raw datagrams, and Steam payloads or lobby data may carry SteamIDs (desync-hunting pitfalls), so scrub them before committing (the pre-commit SteamID guard).
- **Draft PRs back to coop-anything, `local/` convention, directory YAML, Discord** are not applicable. Our GitHub loop and board follow the repo CLAUDE.md, and release packages stay local.
- **Hot reload "not against anti-cheat games"** is fine for D2 (no anti-cheat), but only in lab copies, never the user's Steam play session.
- **Effect-capture policy examples** (karma, caps, companions) are NV-specific. Our share policy must be decided with the user, and the page says so too.
