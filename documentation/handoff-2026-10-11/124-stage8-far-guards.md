# 124. Stage 8: guards far from the host (area 4.5, #241)

Area 4.5 keeps guards near the partner alive and in step when the host is far away. The design notes are in [far-guards-design.md](stage8-step1-notes/far-guards-design.md). Labels: confirmed static, confirmed runtime, hypothesis.

## The problem (step 1)

The engine pauses an NPC's AI at LOD level 7: when it is not rendered and more than 50 m from the camera. The camera is the host's only. In `activation-range-v2001a` the host's bound guards near a partner 205 m away were 100 % paused, so the partner's replicas stood still ([chapter 116](116-stage8-measure-before-changing.md)).

## Keep-awake (candidate keepawake-v2066)

**What it does:** the host gives a bound NPC that is more than 50 m from the host and within 55 m of the partner a native LOD request at level 6.
- The request goes on the NPC's LOD component, the one `arkCpntPossession::Begin` asks for level 0 (confirmed static, d2-ghidra):
  - get the component with `0x877720(entity, image+0x39b4480, 2)`;
  - acquire with `0x720780(component, &id, name, level)`;
  - release with `0x721490(component, {2, id})`.
- The component's entity is at +0x10. The lowest requested level becomes the entity's clamp (+0x29c, via `0x879ae0`).
- The request is released when the host is back within 40 m, when the partner is more than 70 m away, or when the channel stops.

**Lab switch:** `coop_npc_keep_awake on|off`. Counters: `keepAwakeHeld`, `keepAwakeReleased` and `keepAwakeRefused` in the NPC stream status.

**Test:** `work/stage8/Test-ActivationRange-v2066.ps1`, on the local two-instance lab and Edge of the World, with the same positions as v2001a. Analysis: `analyze_activation_range_v2066.py`.

| Take | Candidate | Result |
|---|---|---|
| `keepawake-range-v2066a` | keepawake-v2066 | **Keep-awake works** (confirmed runtime). The host held 13 requests and refused none. Near the partner: `script_spawner_addermire_roof_03` went from 100 % paused (v2001a) to 0 %, and walked 21.3 m on the host and 21.1 m on the partner, ending 0.0 m apart. `addermirestation_entrance_guard_02` went to 0 % paused and ended 0.0 m apart. The host's copies read LOD 4 to 6 instead of 7. The unbound shopkeeper (a civilian at 15.4 m, past the 15 m civilian bind radius) still runs in each world on its own, 0.9 m apart at the end. The twin phase was refused: the lab command was not yet in `Send-D2Command`'s set on that tree |
| `keepawake-range-v2066b` | keepawake-v2066 | **Keep-awake, twin in the same take: red as required.** With keep-awake on, `addermire_roof_03` is 0 % paused and walked 22.5 m on both worlds (0.0 m apart), and `addermirestation_entrance_guard_02` is 0 % paused. With `coop_npc_keep_awake off`, both guards fall back to LOD 7 and are 100 % paused on the host. All 26 requests were released at the switch and none was refused. The lab was restored |

## Anti-flap, handoff and the death journal

- **Anti-flap:**
  - The bind and drop radii are 55 and 75 m, 36 % apart. The drop scan runs every 2 s (confirmed static).
  - Candidate **farguards-v2067** also keeps an eviction from removing a bind younger than 1.5 s (`dwellHeld`).
- **Handoff:**
  - Unbinding never spawns or removes a guard: it only releases the partner's AI lease, so a guard cannot be created twice by a handoff (confirmed static).
  - Its life state reaches the partner through the combat channel while bound, and through the life roster when unbound.
  - Whether a final pose snap and Ack are needed is left to the split-up take's diffs.
- **The death journal** (re-applying a life state after a guard resets on waking) is not built.
  - Hypothesis: a bound guard no longer freezes with keep-awake on, so it has no stale state to wake from.
  - Re-applying blindly could knock out again a guard that the other world legitimately woke.
  - It will be added only if a take shows a reset.

## Split-up take on two Steam accounts (2067)

Harness v2067 adds `partnerattack`: the VM partner attacks an NPC in its own world from behind with the right trigger, until its own health probe reads it dead.

**Route `farguards-eotw-v2067`, Edge of the World:**
- The host stays at the docks; the partner stands at the far point.
- `--npc-activity` probes run, with required diffs at spawn, far, after the kill and at the end.
- The partner kills `addermirestation_entrance_guard_02`.
- The twin turns keep-awake off.

| Take | Candidate | Result |
|---|---|---|
| `farguards-eotw-v2067a` | farguards-v2067 | **Keep-awake on two Steam accounts** (confirmed runtime). The partner stood about 205 m from the host. Both `--npc-activity` probes on the host show `addermire_roof_02`, `addermire_roof_03` and `addermirestation_entrance_guard_02` clamped to LOD 4 or 6, with no pause refs. Diffs at spawn (2278) and far (2138) equal. Harness fail at the kill: `partnerattack` searched `--entities` names, which are entity names, not spawner names |
| `farguards-eotw-v2067b` | farguards-v2067 | **Twin, red as required** on activity: with `coop_npc_keep_awake off` the same three guards are LOD 7, clamp 8, paused (refs 1, 1, 0) in both probes. Diffs at spawn and far equal, because the diff compares life states only. The same harness fail at the kill |

Harness v2068: `partnerattack` finds the NPC by spawner name through `--npc-activity`, then the entity by its handle.
