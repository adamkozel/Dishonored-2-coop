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

## Deaths after a level transition (1214bl–1214bo, [audit 1302](../../work/movement-integration/campaign-eotw-audit-v1302.json))

In the campaign takes, a death in Karnaca after arriving from the Dreadful Wale left both players on GameOver indefinitely. Four causes were found and fixed:

| Candidate | Cause (confirmed runtime and static) | Fix |
|---|---|---|
| 1214bl | The session's map scope (`checkpointMission.mapId`) is fixed when it is armed, and `checkpointWorldReady` requires the current map. After any level transition, every restart was refused (take 1302i: requests 0→1, rejected +1). The maintenance re-issue of `coop_checkpoint_sync on` is itself rejected while a session is enabled. | An idle or failed session re-arms when the world reaches another map. The reset sidecar reports `remaps`. |
| 1214bm | After re-arming, the peers' autosave identities differed in the `mission` field (host −254924741, client −464858806; map and chapter equal). The field depends on whether the mission-start autosave lands before or after the map's mission activation, so hellos were rejected (takes 1302j/1302k). | The identity digest is `map|chapter`. The details keep all three fields. |
| 1214bn | Once, the client refused the host's Prepare and its own request (take 1302l). The host then sat in Preparing for 45 s, the client failed when hellos stopped, and nothing re-armed. | Preparing and Requested end in 8 s, while loading keeps 45 s from Accept/Begin. A failed idle session re-arms after 3 s, so pressing Restart again works. The reset sidecar reports `recoveries`. |
| 1214bo | On two accounts (take campaign-eotw-vm-v1302o), the client's paired-autosave receiver rejected every host packet after the host re-armed (the host re-armed four times, the client twice). The pair never settled, so the client refused the Prepare. | A newer peer arm or a new session restarts the local pair counts from the autosave each side holds, and older arms stay rejected. The sidecar reports `peerRearms`. |

Take route-eotw-boatdeath-v1302n (pass): the host falls in Karnaca after the boat. Both reach the death screens in 6.2 s, both restart together and are back in gameplay 7.0 s after the press, then the carriage takes both to the Good Doctor with equal objectives. The route harness's `die` step follows the 1151 press sequence, traces the session, and presses Restart again every 12 s while both stay on GameOver.

## Simultaneous requests (candidate 1214jn, #188)

Updated 2026-10-24. Harness: [`Test-MissionSimultaneous-v1870.ps1`](../../work/natural-moveset/Test-MissionSimultaneous-v1870.ps1), Edge of the World in product mode, with checkpoint sync in mission mode and paired autosaves. A case passes when:
- each world ran exactly one transaction (one load, one completion, no abort);
- the paired autosave counters settled equal before the restart;
- each player came back within 1.5 m of where it stood at its last save.

No code change was needed: the host-led transaction and the paired autosave counters already deduplicate (confirmed runtime).

| Take | Case | Result |
|---|---|---|
| **1869a (pass)** | Both players die at the same moment, falling in the same instant, and both press restart together | Both GameOver screens in 5.8 s. One transaction in each world; the client counted its own press as a duplicate of the host's. Both back in gameplay in 6.9 s. |
| 1869b (fixture) | Simultaneous autosaves | The settle check passed before the saves landed, and the moved players stood over the harbour water, where a fall does not kill. |
| **1870a (pass)** | Both worlds autosave at the same moment (`coop_autosave_probe`, back to back), each player at its own docks-apartment stand | Each world originated one save and mirrored the other's (2 each), settled in 2.6 s. The host died from its mission start; one transaction each, and both players back at their own stands (0.0 m and 0.03 m). |
| **1870b (pass)** | The client saves through the pause menu (Save Game, first slot) while the host autosaves | The pair counts the manual save as an originated save, because it watches the game's last-slot field, so the host mirrored it as an autosave and the client mirrored the host's: settled at 2 each. One transaction each, both back at their stands (0.0 m and 0.58 m). |

- **A manual save pairs like an autosave:** the other world makes its own autosave at the same moment, so a shared restart brings both players back to the same point.
- **Two restart requests at once:** the host's transaction wins and the other press is a duplicate (1869a). Two autosave origins at once become two pairs (1870a). Nothing is lost or run twice.

## A native save lost to the autosave reader ([#225](https://github.com/adamkozel/Dishonored-2-coop/issues/225), candidate 1214km)

Updated 2026-10-25.

- **The failure:** in the 0.9.5 regression take [campaign-dunwall-vm-v1915a](../../work/natural-moveset/campaign-dunwall-vm-v1915a) (1214kl), the host's checkpoint sync never armed (`enable_refused`, native slot -1). The host fell alone and the shared death screens never came.
  - The host's co-op save folder already held autosaves of earlier takes, as a player's does.
  - The host's first Dunwall autosave went to `SLOT02`, the slot of its earlier Institute autosave. It wrote `checkpoint.dat` and `screenshot.png`, but `game.details.temp` was never renamed over `game.details`, so the save was lost.
- **The save path:** the native save writes `game.details` as `game.details.temp` and then renames it over the old file. That is confirmed runtime by the leftover `.temp` of a lost save. In the binary, the format string `%s.temp` lies beside the `idSaveGameThread` name; its code references are not analyzed.
- **The cause:** the mod's mission autosave reader (`readMissionAutosave`, [checkpoint_mission.h](../../outputs/d2-coop-lab/src/checkpoint_mission.h)) opened the slot's `game.details` without delete sharing. It did so at every checkpoint enable attempt (several a second while refused) and every 250 ms in the paired autosaves.
  - A rename over a file that another handle holds without delete sharing fails.
  - So a save into the slot being read can be lost: hypothesis for the take above; the mechanism itself is confirmed runtime (below).
- **Take [save-share-v1916a](../../work/natural-moveset/save-share-v1916a)** ([Test-SaveShare-v1916.ps1](../../work/natural-moveset/Test-SaveShare-v1916.ps1), 1214km, confirmed runtime). The lab host, on its own saves and the Dunwall fixture, requested native autosaves with the lab fixture `coop_autosave_probe force` while the take held every slot's `game.details` open:

  | Phase | `game.details` held | Result |
  |---|---|---|
  | Control before | no | Landed in SLOT62 |
  | Held without delete sharing (the reader of 1214kl and earlier) | 22 files | Lost: SLOT61's `checkpoint.dat` was rewritten, `game.details` was not, and a fresh `game.details.temp` stayed |
  | Held with delete sharing | 22 files | Landed in SLOT62 |
  | Control after (twice) | no | Landed in SLOT61, then SLOT62 |

  - In this lab take the native slot moved to the lost slot (62 to 61). In the Dunwall take it stayed -1 (hypothesis: it was the first save of a new game).
- **1214km:**
  - The reader lists the slot's folder, which opens neither file.
  - It skips a slot whose `game.details.temp` is less than 30 s old (a save in progress).
  - It opens `game.details` once per new write (the listing's write time and size), with delete sharing.
  - The reset status reports `autosaveDetailsReads` and `autosaveDetailsBusy`.
- **Players:** without the fix, an autosave could be lost whenever the game overwrote the autosave slot that the mod was reading. That happens in the normal rotation once a co-op folder holds autosaves.

## Open

- **Real script trigger (done, #136, [chapter 106](106-partner-led-story-and-mid-mission-join.md)).** A real mission-script autosave crossed by either player alone pairs both worlds, and one shared restart brings both back from it (confirmed runtime):
  - the host alone: route-dunwall-autosave-v1669a (the Dunwall street balcony; the partner mirrored it natively; both restarted from slot 62 in 6.7 s);
  - the partner alone: route-dunwall-autosave-v1687b (the same trigger; both worlds saved the pair; both restarted in 7.0 s with gameflow equal).
- **Manual saves:** a manual save pairs like an autosave (#188, above). Loading a chosen save from the menu is [chapter 107](107-coop-menu-and-saves.md); not started here: disconnect and rejoin. Transitions between and within missions are covered in [chapter 97](97-campaign-map-sequence.md).
