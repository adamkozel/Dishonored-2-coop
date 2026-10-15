# Shared NPC authority on mission maps (candidate 1091, stage-4 bridge)

Updated 2026-10-14. Continues the Dunwall NPC work in chapters [70–72](70-generic-npc-binding-and-ai-ownership.md) and follows [chapter 85](85-mission-equipment-and-item-wheel.md). Audit: [mission-npc-audit-v1093](../../work/movement-integration/mission-npc-audit-v1093.json).

## Census (1090, read-only)

**Edge of the World** has 141 authored NPCs.
- The sets are identical for the Corvo and Emily loads (unlike pickups in [chapter 83](83-mission-shared-pickups.md)).
- 133 have unique, reciprocal spawners; 22 are scripted corpses.
- Near the spawn: 5 within 20 m, 17 within 40 m, 34 within 80 m.

## The gate

`guardPairReady` required the Dunwall tower and gated 27 call sites. Candidate 1091 splits it:
- **`coopNpcPairReady`** (the same check without the map) is used by the generic systems: NPC authority hold and stream, NPC combat (chapter 73's generic adapter) and partner AI perception (chapter 77).
- **`guardPairReady`** (Dunwall-only) remains for the legacy guard-link, enemy-melee, enemy-outcome, native enemy/sword damage, sword-link, friendly-fire and observer-isolation experiments.

**Relevance:** on both peers, the NPC stream binds the nearest eligible authored NPCs to the host (role 0) position, at most 24 within 60 m, so both peers choose the same set. The catalog no longer fails when more than 128 NPCs are eligible.

## Runtime (1092)

- Both peers pair 9 dock NPCs (the eligible ones within 60 m): host channels are `host_authority`, client channels `client_replica`.
- Each channel carried about 760 roots and 192 poses; the client applied 6,710 body frames.
- Client replicas stay on the host authority (maximum deviation 5 mm), including two NPCs that moved 0.1–0.19 m.
- 2 of the 132 unbound NPCs drifted apart between the two worlds by up to 2.3 m within 10 s. That's the divergence authority prevents.
- Dunwall fixture restored.

## Open (stage 4)

- **Eligibility:** NPCs whose decision or reflex AI isn't idle are excluded, e.g. scripted walkers such as `docks_docker_10`, which walked 9.3 m. The AI-flag semantics need research before widening.
- **Dynamic relevance:** rebind as players move, instead of a fixed set chosen at bind time.
- **Combat:** NPC combat against hostile guards on missions, perception of both players, and one authoritative outcome per event.
- **Legacy experiments:** retire or generalise them.

## Mission combat discovery (candidate 1097, audit 1100)

[Audit 1100](../../work/movement-integration/mission-combat-audit-v1100.json). Harness: [`Test-MissionCombat-v1099.ps1`](../../work/natural-moveset/Test-MissionCombat-v1099.ps1). It places both players by the windmill guard in Edge of the World, then arms the NPC stream, NPC combat and NPC attack; the host provokes with ordinary RT and blocks with LT.

**Tutorial modals (confirmed runtime).** Entering the Wall of Light area opens a native tutorial modal ("Play Your Way #2", logic 12). It invalidates follow and retires partner presentation. The harness closes it with B and re-arms; gameplay needs automatic re-arm or modal tolerance.

**Pose bandwidth (1096 → 1097).**
- At 20 Hz × 12 NPCs, about 2.8 KB poses saturated the 512 KB/s Steam send rate; the queue reached about 970 ms and every client replica went stale.
- 1097 budgets NPC body poses at about 160 KB/s, so each NPC is sent every 50–400 ms (241 ms here), reported as `poseIntervalMs`.
- Result: the queue stays at 0 and the client's guard tracks the host's guard exactly through the fight.

**Combat (1098–1099).**
- The guard engages and damages the host natively; it killed him in 1098.
- Blocking (LT) reduces damage to chip damage (70 → 64.75 → 59.5).
- Enemy-attack replication is active on both roles.

## Authoritative combat outcomes on missions (candidates 1102–1108, audit 1110)

[Audit 1110](../../work/movement-integration/mission-outcome-audit-v1110.json). With the host facing the guard (RT strikes, RB block), each run removed one blocker:

1. **1101:** the host's hits landed (guard 30 → 10 on the host only), but the guard channel retired and the client's guard diverged.
2. **1102 diagnostic:** `lifecycleDiff` 0x200 showed the guard's decision behaviour tree swapped as it entered combat. **1104** stops ongoing channels from treating that as a new NPC; acquisition and double reads stay strict.
3. **1105:** the first hit was a **native player kick** (`player_kick.damage`, amount 10, type 0x80, reactions 1/1, inflictor = the player, not the sword). Nothing bound that family. **1106** adds `NpcCombatDamage::Kick` (NPC combat wire 7), applied on the client with the kicker as inflictor.
4. **1107:** the kick hit has a **zero normal** (region 1, regions 2), which the unit-normal melee rule refuses. **1108** adds a kick geometry rule.
5. **1109 (pass):** the host committed 2 outcomes (kick and sword) and the client applied 2. Guard health converges at 30 → 10 on both peers, NPC combat stays active, and the host's native block takes only chip damage.

6. **1111 (pass, client-sourced):** the client's native strike on the unaware guard was a fatality (`generic_fatality.damage`).
   - Flow: the client sent a request, the host committed it once, and the client applied it once.
   - Result: both peers record one death, and the guard is at 0 health in both worlds.
   - Gap: the corpses settle 1.38 m apart, because the channel retires at death and nothing yet owns the corpse.

**Checklist:** `kick_or_shove_if_present` → `partial_native_steam_player_kick_outcome_host_source_mission_map` (outcome only; the kick body presentation on the remote player isn't verified).

**Open:**
- Client-sourced hits on missions.
- The death outcome before channel retirement (in 1105 the kill retired the channel first).
- Guards engaging the client (partner perception) and relayed player damage.
- Shared death and checkpoints on missions; tutorial-modal tolerance.

## Corpse authority (candidates 1113–1113d, audit 1118)

[Audit 1118](../../work/movement-integration/mission-corpse-audit-v1118.json).

**The problem.** After a shared native death, each world settles its own ragdoll, and the corpses ended 0.9–1.4 m apart.

**What changed in 1113d:**
- **Corpse mode.** A bound NPC that dies with its identity intact keeps its channel as a corpse stream; it no longer retires.
- **Pose health.** Poses may carry health 0, but pose health is never applied. Death only ever comes from the native combat outcome.
- **AI handling.** The NPC-combat pre-death release still frees the AI for the native death, but it keeps the channel (1113b). Corpse channels never re-acquire AI (1113c).
- **Client corpse.** The client switches its corpse's physics component to animated (native `0x736ef0` on the idle ragdoll) and applies the host's corpse poses.
- **Release.** Ragdoll release (`0x734d90` then `0x734a10`) happens only when the channel stops. Releasing mid-session moved the corpse 0.27–0.86 m (1116), so in 1113d the corpse stays streamed for the life of the stream, within the pose budget.

**Runtime (1117, pass).** A client-sourced native fatality produces one death on both peers and identical corpses (0.0 m apart throughout). The client's video shows the kill, the collapse, and a natural slumped corpse against the railing.

**Checklist:** `death_variants` and `ragdoll_handoff` are partially bound for NPC corpses on mission maps.

**Open:**
- Retiring distant corpses (dynamic relevance).
- Physics interaction with a held client corpse, and carrying a streamed corpse.
- Guards engaging both players.
- Shared player death and checkpoints on missions.

## Guards engage both players (audit 1122)

[Audit 1122](../../work/movement-integration/mission-engagement-audit-v1122.json). This uses chapter 77's avatar perception (`coop_avatar_perception on`, which has no map gate) together with NPC attack.

| Take | Result |
|---|---|
| 1119 (failed) | The client's block-plus-attack from behind an unaware guard missed, so the guard was never provoked. |
| 1120 (pass) | The host alerts the guard (30 → 20, replicated) and moves 25 m away; the client faces the guard holding block. On the host, the guard's native combat target is the client's clone, with the host player still in its attention list. NPC attack captured one hit on the host and relayed it; the client consumed it once and blocked it natively (no damage). |
| 1121 (pass) | Same without block: the guard keeps the client's clone as combat target in all 8 samples, and the relayed hit damages the real client player (70 → 55). |

**Stage-4 summary on Edge of the World (audits 1110, 1118, 1122):**
- Enemies perceive and fight both players.
- Each player's hit or kill commits once and applies once.
- Deaths and corpses are shared.
- Enemy hits on either player are native, either directly or relayed.

**Open:**
- Sustained fights with several enemies, and both players attacked at once.
- Ranged and power damage on missions.
- Player death and shared checkpoints on missions.
- Dynamic NPC relevance.
- Tolerating tutorial modals.

**Earlier open items (1099):**
- The host's strikes didn't change the guard's health. This was harness facing; resolved in 1101.
- The client's NPC combat retires all bound actors once the guard is in combat.
- The guard engages only the host; perception of the second player on missions is unsolved.
- There is no shared death or checkpoint on missions.

## Dynamic NPC relevance (candidates 1166–1166b, audit 1169)

[Audit 1169](../../work/movement-integration/mission-npc-relevance-audit-v1169.json). Harness: [`Test-MissionNpcRelevance-v1168.ps1`](../../work/natural-moveset/Test-MissionNpcRelevance-v1168.ps1).

Until now, the bound NPC set was fixed when the stream was armed: the 24 nearest NPCs within 60 m of the host. From 1166 the **host owns a dynamic set**:
- **Every 2 s** the host binds eligible NPCs within 55 m of either player (nearest first, at most 24 bound).
- **Retiring:** channels whose NPC is more than 75 m from both players are retired, and the host sends Stop to the client.
- **Client:** it starts empty and binds an NPC when the host's Hello names one it doesn't have yet, so both sets are always the host's.
- **Slots:** retired slots are reused for the per-actor body counters.
- **Empty areas:** an area with no NPCs keeps the session alive.

| Take | Result |
|---|---|
| 1167 (1166) | At spawn, 9 bound on both peers. After a ~205 m move, the host dropped those 9 and added 11, and the client bound them. Then the NPC session ended with `session_pose_stale`, because the long move caused more than 1 s without partner poses. **1166b**: a stall of 1–10 s suspends NPC, door and usable sessions like a native menu, and they resume afterwards. |
| **1168 (pass)** | At spawn, 9 bound. In the far interior, 11 bound (9 dropped, 11 added). Back at spawn, 9 bound (11 dropped, 9 re-added). The host's and client's ready sets were identical at every stage within about 1 s. |

**Not exercised:** no transport stall occurred in 1168, so the 1166b stall-suspend path is implemented but runtime-unverified. This harness recorded no video.

## Engine-thread stalls from NPC catalog reads (#43; candidate 1213n, audit 1242)

Found while measuring partner presentation on Edge of the World (take 1235e). Both peers' engine threads stalled on native NPC catalog reads ([audit 1242](../../work/movement-integration/npc-catalog-stalls-audit-v1242.json)):
- **Client (confirmed runtime):** every host Hello for an NPC the client hadn't bound re-read the whole catalog. A refused bind repeated on the next Hello: 27 binds and 55 refusals gave 46 `exchangePoses` stalls of 79–103 ms (worst 925 ms).
- **Host (confirmed runtime):** the 2 s relevance scan cost 90–162 ms.
- **Why it is slow (confirmed static):** `memoryRead` is one `ReadProcessMemory` syscall per read. The walk read every live entity twice, and each NPC frame is read twice.

1213n changes three things:
- one catalog snapshot serves Hellos for up to 1 s (`bindNpcChannel` still re-reads the NPC's skeleton and identity);
- a refused NPC is retried after 3 s;
- the world walk reads each entity once.

**Take [1235f](../../work/natural-moveset/window-traversal-v1235f)** passed in both roles, and restoration passed. It archives both peers' timings per case:
- **Client:** catalog reads fell to 10 (101 Hellos deferred) and stalls to 10. Each read still costs 70–86 ms.
- **Host:** the scan still stalls for 80–207 ms. **Open:** the catalog read has to move off the engine thread.

