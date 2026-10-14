# Shared death and checkpoint restart on mission maps (candidates 1147–1153, stage 6)

Updated 2026-10-14. Generalises the fixed Dunwall SLOT82 recovery from [chapter 62](62-continuous-checkpoint-death-recovery.md) to mission maps. Audit: [mission-checkpoint-audit-v1152](../../work/movement-integration/mission-checkpoint-audit-v1152.json). Final harness: [`Test-MissionCheckpoint-v1151.ps1`](../../work/natural-moveset/Test-MissionCheckpoint-v1151.ps1).

## How the game restarts a mission (1146, confirmed runtime)

In a read-only take, the host died from a native fall on Edge of the World and chose the native GameOver option "Restart From Last Checkpoint". The always-on command observers recorded two commands:
1. `RestartFromCheckpoint` (kind 0), which caches the last autosave slot (61, the mission-start autosave);
2. `RestartFromMemoryCheckpoint` (kind 1), accepted for that slot.

Native facts found in Ghidra and in the live console registry:

| Item | Detail |
|---|---|
| Console commands | `RestartFromCheckpoint` `0x65da50`, `RestartFromMemoryCheckpoint` `0x65db00`, `LoadGame` `0x658d00`, `SaveGame` (quick save) `0x65f520` |
| Save request | `RequestSave` `0x14065cc50` (manager = `game+0x2e75d8`, slotType, slotNumber, isMissionStart, autoRetry, securityDelay). Slot types: 1 quick, 2 manual or mission start, 3 autosave. |
| Last autosave slot | `game+0x2e75d8+0x1c` |
| Autosave trigger | Mission scripts, through `arkKisculeNodeActionAutosave` (vtable `0x141b7c9e8`) |

**Autosave slots.** Autosaves rotate between SLOT61 and SLOT62 inside each isolated profile. Every instance keeps its own per-character autosaves: Corvo saves are character 1, Emily saves character 2. They also differ in time and checksum. A file-hash identity like SLOT82's can therefore never match on a mission.

## Mission checkpoint mode (1147b)

A checkpoint session armed on a non-Dunwall map runs in mission mode. The Dunwall SLOT82 path is unchanged.

**Identity.** Each peer's last autosave must belong to the current map. The identity is that autosave's authored `mapName|chapter|mission`. Arming is refused until the peer's mission-start autosave has been written; in 1148 the client still held its Dunwall autosave, so the harness re-arms. Autosave write counts are recorded but are not part of the identity, because they depend on timing.

**Death.** The existing shared-death logic applies unchanged. When either player reaches native GameOver (logic 13), the other peer is switched to its own native GameOver, and both death screens must be ready.

**Restart.**
- Any restart press is intercepted and becomes one shared transaction; duplicate presses are absorbed.
- After Prepare, Accept and Begin, each peer dispatches its own native `RestartFromCheckpoint`. The memory continuation is permitted only for that peer's exact cached slot.
- The client keeps its own Emily save, so the SLOT82 Corvo-to-Emily role swap isn't used.
- Both loaded worlds are held at the barrier and released together, and the existing door, coin, usable and NPC renewal hooks then run.

## Runtime

| Take | Result |
|---|---|
| 1148 (1147) | The arm did not pair: the client's last autosave was still from Dunwall, and generation counts differed. |
| 1149 (1147b) | Shared death was reached, but the restart aborted (host timeout). The harness pressed load prompts on a shared timer and lost its timeline. Cleanup could not disable the client session while its load boundary was pending, so recovery was done by hand: A on the client, sessions off, B out of pause, then `Recover-InterruptedTake`. The first attempt failed on the pause menu and is kept as `recovery-attempt1.json`; the second passed. |
| 1150 | Clean flow in about 8 s: death, both GameOver screens, host restart, both loading, both held, released, gameplay. Each peer loaded its own slot 61 at its exact spawn, and the usable session renewed. Marked failed only because a cumulative count check included 1149's abort. |
| **1151 (pass)** | Two cycles: the host dies and requests the restart, then the client dies and requests it, with the other peer pressing as a duplicate. Each cycle has both native GameOver screens and exactly one load and one completion per peer, with no abort. World epochs are new, characters are correct (1 and 2), both players are back at the exact mission-start spawn, and the usable session renewed. |

**Visual acceptance (1151), both roles.** In both cycles the contact sheets show the fall or the shared native GameOver screen with each player's own character art, the native "Edge of the World" loading screen, and the return to the docks spawn.

The harness now:
- saves its timeline on failure;
- presses load prompts per role;
- steps through native prompts until a failed checkpoint session can be disabled before restoring.

## Paired mid-mission autosaves (candidate 1153, audit 1157)

[Audit 1157](../../work/movement-integration/mission-autosave-pair-audit-v1157.json). Harness: [`Test-MissionAutosavePair-v1156.ps1`](../../work/natural-moveset/Test-MissionAutosavePair-v1156.ps1).

Mission autosaves are per-player script events. Without pairing, a player who crossed an autosave trigger alone would restart from a later moment than the partner.

**How 1153 pairs them:**
- **Observe.** Each peer watches its own native autosaves through the same reader as the identity: the last-slot field and that slot's `game.details` write time.
- **Originate.** A save the peer did not request counts as *originated*.
- **Mirror.** The other peer mirrors it with the game's own `RequestSave(manager, slotType 3, slot −1 → next rotating autosave slot, autoRetry)` at `0x14065cc50`, after checking its exact prologue. Mirroring runs on the engine thread, during gameplay only, one request at a time.
- **Exchange.** Counters (saves, originated, mirrored, pending) travel every 250 ms on a small `D2AP` packet. They are monotonic within a world epoch and reset when the epoch changes after a restart, so a lost packet only delays mirroring.
- **Gate.** A mission restart is allowed only while both peers are settled: equal save counts, everything mirrored, and nothing in flight. Unit tests cover the settle rule and packet validation.
- **Fixture.** A developer command, `coop_autosave_probe`, issues one native autosave on one peer. It stands in for a script trigger crossed by one player and was added to the lab's command allowlist.

**Runtime:**

| Take | Result |
|---|---|
| 1154 | The command was not yet in the lab allowlist. Restoration hit the known inventory-read flake and was repaired. |
| 1155 | The host's native autosave was mirrored by the client in 1.9 s. The post-restart check crashed on a harness bug: `$p` and `$P` are the same PowerShell variable. |
| **1156 (pass)** | Both origins. Each player stood at a different spot in the docks apartment. One peer autosaved there, and the other mirrored it natively in 1.6–1.9 s. Both then moved away, the other peer died, and one shared restart returned **both** players to their own paired autosave positions (within 0.09 m), 57–60 m from the mission start. Each peer did exactly one load and one completion, characters were correct, and the pair counters reset and resettled after each restart. |

**Visual acceptance (1156), both roles.** The sheets show the apartment spot, the move to the docks, the death or the shared GameOver, the native loading screen, and both players back in the apartment.

## Renewing every system after a restart (candidates 1153b–1153c, audit 1161)

[Audit 1161](../../work/movement-integration/mission-renewal-audit-v1161.json). Harness: [`Test-MissionRenewal-v1160.ps1`](../../work/natural-moveset/Test-MissionRenewal-v1160.ps1). All world-authority systems were armed together: partner avatar follow, door, coin and usable sessions, and the NPC stream.

| Take | Result |
|---|---|
| 1158 (1153) | The restart completed, but every renewal stayed at `waiting_shared_checkpoint`. Two causes: `prepareCheckpointPresentation` still required the Dunwall map, so the partner avatar was refused, and renewal waited for the avatar to succeed. **1153b** removes the map gate, and renewal now proceeds once the avatar step has finished, whatever its outcome. |
| 1159 (1153b) | A pre-death readiness check found that one transient blip had retired all 16 client door channels. **1153c** makes door channels pause on transient context gaps and arm by identity scope, as usables already do (1123c/d). |
| **1160 (pass)** | Two cycles: the host dies, then the client dies. After each restart, both peers had the partner avatar restored (2 of 2, none refused), the door session renewed with 16 converged doors, coin sync re-armed and settled again before the next death, the usable session renewed, and the NPC stream renewed with 9 bound NPCs. Door sessions absorbed 48–224 transient pauses without retiring. |

**Visual acceptance (1160):** host role only. The host video shows the restored Emily partner standing on the docks after both restarts. In the client video the Corvo partner isn't in frame, so client-side partner visibility is unconfirmed, although the status reports it restored.

## Open

- **Real script trigger.** A mission-script autosave crossed by one player alone hasn't been exercised. The same reader observes it, but these takes used the probe fixture.
- **Not started:** transitions between missions, manual save and load selection, disconnect and rejoin.
