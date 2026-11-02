# Guards far from the host (stage 8 area 4.5, #241): design notes

Read-only research, 2026-11-02. Labels: confirmed static, confirmed runtime, hypothesis. Paths are under `outputs/d2-coop-lab/src/` unless given in full.

## What step 1 measured

- **Take:** `work/stage8/activation-range-v2001a`, Edge of the World. The host stayed at the docks; the partner stood about 205 m away. Write-up in [chapter 116](../116-stage8-measure-before-changing.md) and [activation-range.md](activation-range.md).
- **The engine's rule** (confirmed runtime): an NPC that is not rendered and is more than 50 m from the camera is at LOD level 7 (`lod_dist_far`), and its AI auto-pauses there (decl +0x64). The camera is the only LOD reference; the partner's rig never counts (confirmed static).
- **Result with NPC authority on (0.9.6):** the host's guards near the partner were at level 7 and 100 % paused, and the partner's copies are replicas, so the partner saw frozen guards (confirmed runtime).
- **Conclusion of chapter 116:**
  - Pure host authority does not work as it is.
  - First try: a native LOD keep-awake request at level 6 on host-bound NPCs near the partner.
  - Fallback: closest-player ownership.

## How the code treats a guard near the partner today (confirmed static)

- **Relevance** (`npc_authority_stream.h:288-415`):
  - The host binds NPCs within 55 m of either player, at most 24, every 2 s, and drops them past 75 m. Civilians use 15 and 20 m.
  - The host binds AI-paused NPCs too.
- **On the partner:** a host Hello binds an NPC, and an AI lease pauses the partner's copy so it becomes a replica. `Stop` releases the lease with no final state.
- **Knockouts and kills by the partner:**
  - A bound guard goes through the combat channel and `Dead`.
  - An unbound guard goes through the life roster: states 0, 1 and 2, applied once, only when more severe. Under host story the host refuses only named characters.
- **Gaps:**
  - Once an entry is applied, it is never checked again. A frozen guard that resets when it wakes is not re-applied.
  - The world diff compares life states only, not positions or activity. A split-up take needs `--npc-activity` as well.

## Smallest design

1. **Keep-awake (host):** each bound channel more than 50 m from the host's camera and within 55 m of the partner gets a LOD request at level 6.
   - Acquire with `FUN_140720780(mgr, &handle, "d2coop::KeepAwake", 6)`, release with `FUN_140721490(mgr, &handle)`, and change the level with `FUN_1407219a0` (confirmed static: `mgr+0x10` is the entity, `mgr+0x20` the request list).
   - Release it in `stopNpcChannel` and on an epoch or map change, and when the camera is back within 40 m.
   - Lab switch: `coop_npc_keep_awake on|off`.
   - **Open:** where an NPC entity's request manager lives. Swarms use component +0x48; `arkCpntAnimEvolved` gets one from `FUN_1403da910(cpnt+0x448, 2)`.
2. **Anti-flap:** at least 1.5 s between a bind and a stop or eviction, and 1.5 s before a rebind after a stop.
3. **Handoff:**
   - `Stop` carries the host's final life state and root position.
   - The partner snaps the pose, applies the state, then releases its lease and answers with a new `StopAck`.
   - Until the Ack arrives, or 1.5 s passes, the host keeps the channel retiring and counts it as bound, so the guard is never created twice.
4. **Death journal:** in `processNpcLife`, an applied entry whose local state became less severe again is re-armed (`lifeReapplied`). Lab switch: `coop_npc_life_journal`.
5. **Fallback:** closest-player ownership, used only if level 6 does not keep the AI running.

## Take plan

**Harness changes:** an `attack` step for the client role, and an `activity` assertion: near the partner the host's copies are not paused, each guard's two copies end within 1.0 m, and each guard exists once in each world.

**The take:** Edge of the World.
1. A diff at spawn.
2. The partner goes to (62.2, 4.5, 33.5), then `--npc-activity` probes and an `activity` check.
3. The partner knocks out `script_spawner_addermirestation_entrance_guard_02` and kills `script_spawner_addermire_roof_03`, each followed by a required diff.
4. The partner leaves both guards (unbind handoff), then a flap test at 52 to 60 m.
5. A journal test, and a final diff.

**Mutation twins:**
- The primary twin turns keep-awake off: the `activity` check must go red.
- Twins for the journal and the flap rule follow.
