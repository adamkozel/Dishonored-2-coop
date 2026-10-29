# 114. The coop-anything playbook, applied to this mod

On 2026-10-29 the user asked us to use [cranebeaker/coop-anything](https://github.com/cranebeaker/coop-anything) (MIT) to take the mod from 0.9.6 to a flawless 1.0. It is a hub for agent-built co-op mods: a playbook (32 pages), 64 game write-ups, sync test tools, a directory of 302 co-op projects and a knowledge MCP server. This chapter is what we took from it. The full reading notes, nine slices that cover every file in `playbook/`, `research/`, `games/`, `tools/`, `templates/` and `data/`, are in [coop-anything-notes](coop-anything-notes/). They were written from the clone at commit `3dbf6dd`. Everything here is a reading of the hub (confirmed static for what the hub says). None of it is yet confirmed runtime for Dishonored 2.

Setup (local, see [work/external-references/README.md](../../work/external-references/README.md)):
- the clone in `work/external-references/coop-anything`, untracked;
- its knowledge MCP `coop-anything` and its `coop-*` skills;
- the REA MCP on the lab's Ghidra 12.1.4.

The hub's directory lists a third-party Dishonored 2 co-op mod. We never open it.

## The core diagnosis

The hub separates three authorities: story belongs to the host, each actor to one owner with an epoch, and each avatar to its player ([effect capture](coop-anything-notes/playbook-b.md), section 3.1). Our mod gets the second and third right for combat and nearby NPCs. For story, pickups, doors, levers, safes and far NPCs it runs the game in both worlds and mirrors triggers peer to peer. That is exactly the failure mode the hub describes as "replay the trigger on every client": duplicated rewards and spawns, RNG that diverges, and drift that cannot be fixed one bug at a time. The release notes' known limitations are instances of it: safe codes that differ, late-join leftovers, a partner-only step that does not count (#220), and the partner's credits nodes (#94).

The projects in the directory that reached full story co-op share one shape: the host owns the world, story is sent as captured results rather than re-run, each actor has a single owner with an epoch, and bots drive real clients. None of the 302 reached `verified-playthrough` ([data notes](coop-anything-notes/data-research.md)).

## Rules we adopt

**Story and world state**
- **Replay results, not scripts.** Story scripts run only in the host's world. The host records each primitive write (objective, story flag, entity enable, kill, door lock and open, safe state, item give and take, chaos counters) as a numbered transaction. The partner applies it through a "set without side scripts" path and never runs the script. Each take counts the story scripts the partner ran and fails above zero.
- **Capture at the writer, block the requester.** Hook the function that sets a flag or requests a state change, not the one that reports it (Code Vein: skipping the notifier only delayed the change by 11 ms). Survey a map's script commands before guessing.
- **Partner actions are intents.** A lever, door, safe keypad, pickup or trigger volume used by the partner becomes `intent{epoch, n, kind, target}`. The host runs it through the game's own interaction code with the partner's rig as the acting player, then sends the result as a normal transaction. The host refuses partner-on-partner intents, which keeps the rule that partners are excluded from each other's interactions.
- **RNG runs once, on the host.** Loot and bonecharm rolls and random spawns go across as results.
- **Merge rules and share policy as data.** Stage takes the max, flags are OR'd, kills are a union, globals go to the host, and mutually exclusive outcomes go to whichever commits first. Each shared-or-per-player choice (coins, runes, bonecharms, chaos, notes) is one entry in a table.

**Ordering and staleness**
- **Three epochs plus a session id.** The world epoch bumps on every host load; a quickload is the commonest rewind. The intent epoch bumps when the partner's world is replaced, and the ownership epoch on every ownership change. A per-connect session id drops traffic from an older session. Messages sent inside a level load carry the old epoch and are dropped.
- **A reliable story log.** Sequence numbers are dense and applied in order, and acks are cumulative. A live send must not refresh the resend timer. An ack is not "applied": an effect whose target is not streamed in stays queued until it applies.

**NPC ownership**
- **One owner per NPC.** The host decides who owns each NPC.
- **Handoff:** Transfer, then the final state, then Ack, then Commit, with the host able to force an outcome on timeout.
- **Anti-flap:** an NPC waits 1.5 s before moving owner, and moves only for a player at least 25 % closer.
- **No respawns:** never respawn or rebuild an NPC during a handoff.
- **Simulation range comes first.** Gothic 3's engine simulates only about 45 m around the camera, so pure host authority failed once players split up. Its fix was regional ownership plus a host ledger, and a death journal applied when an NPC re-enters simulation ([games-2](coop-anything-notes/games-2.md)). We must measure Void Engine's activation range, and whether the host simulates guards near a distant partner, before we design far-NPC authority. The wake rule must be the same on both machines (MDK2).

**Saves, joins and versions**
- **Saves and joins.** A fresh join gets a snapshot, then the log; a reconnect resumes from its last contiguous sequence number. A host quickload bumps the world epoch and the partner rebases. Test a save taken while the partner's rig exists: in Gothic 3, a save taken with the remote body present failed to load.
- **Versioned hello.** Compare protocol version, DLL build hash and content hash at join, and reject a mismatch with a readable reason. Bound every count and reject non-finite floats.

**Steam P2P**
- x64 `SteamNetworkingMessage_t`: size at offset 8, sender at 16.
- Steam gives no disconnect event. Use a 1 s keep-alive on a control channel and drop after 20 s, or on LobbyChatUpdate leave flags, or on SessionFailed.
- Answer keep-alives while the joiner is still loading.
- `LobbyCreated` arrives twice, so handle it idempotently.
- Split large messages into 60000-byte chunks.
- The status line should say whether game traffic has actually flowed, not just "connected".

## Proving it: the test side

- **Difference detector.** A 256-bucket additive digest of (stable key, value hash) per category, compared only at equal epoch and sequence number, and only after a stable-world gate: three equal scans a few seconds apart after a load. A mismatch dumps per-key detail. If the digests differ while every effect hash matches, something wrote locally outside the sync path, which in our design means a missing suppression hook.
- **Flight recorder.** A ring buffer of raw datagrams, effects, host commits, ownership changes, digests and markers, decoded at dump time. Peers are recorded as slots, never SteamIDs. The budget is about 100 µs per frame. The analyzer lines up the machines, finds the first divergence, shrinks it and writes a regression test.
- **Chaos.** A seeded loopback relay with the profiles lan, good_wifi, bad_wifi, mobile_tether, transatlantic and hell, plus one-way partitions. It belongs on the lab's UDP leg, never on Steam, and the lab peer address has to become configurable first. A take asserts that the impairment really happened, that requests ran exactly once, that no stale update won, and how long recovery from a 5 s blackout took.
- **Animcheck.** A probe on the third-person rig: first-person arms alone would read as a T-pose. It measures pose and position error, sliding, jitter and rubber-banding. Jitter means two writers move the rig.
- **Mutation twins and honest takes.** Every fix ships with a switch that restores the bug, and the take must go red with it. Takes assert their mode first: two peers, the map, both roles, and a live session ("connected is not live"). Skips are recorded as SKIP. A pass with zero comparisons is not a pass. Crash dialogs and dumps fail the take.
- **Bridge.** Our `coop_*` channel can become a main-thread RPC with numbered replies, `waitFor` and an events ring. A `ping` answered from the transport thread tells loading from hung.
- **Bot playbooks as a checklist:** fight, loot_race, door_lock, quest_sync, ownership_handoff, save_load_rejoin, spawn_once and consent_vote (the mission-end barrier).

## Where the hub conflicts with our rules (our rules win)

- **Steam:** no mock `steam_api`, Spacewar AppID 480 or `steam_appid.txt`. The lab and the VM cover two accounts.
- **Human playtest:** the hub calls it "a gap to automate". We automate everything, and the final human playtest stays a Needs-you step.
- **Hard-linked installs** would let a lab write reach the protected Steam install; our lab copies stay as they are.
- **The tools' code** is generalised from fnv-coop, a third-party co-op mod for another game. It is MIT, but the user decides whether we vendor it or write our own from the design.
- **REA** drives Ghidra headless, which needs a change to the "Ghidra only through d2-ghidra" rule. On Windows it is read-only and cannot reuse our annotated project.
- **Video capture:** the hub records from inside the process, never the desktop. Our takes must capture the game window only; still to audit.
