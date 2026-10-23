# Shared doors on mission maps (candidates 1059c, 1214jf)

Updated 2026-10-14. Continues the Dunwall door work in chapters [56–61](56-native-door-transition-api.md) and follows [shared pickups on missions](83-mission-shared-pickups.md). Audit: [mission-door-audit-v1070](../../work/movement-integration/mission-door-audit-v1070.json). No code change was needed.

## Why it already worked

- `door_sync.h` has no Dunwall map gate.
- Each door channel becomes ready on its own peer Hello (matching name and definition). Per-character worlds therefore intersect naturally, unlike the old whole-catalog digest the pickups used.
- **Confirmed runtime:** Edge of the World catalogs and pairs 16 doors on both peers, and `coop_door_sync session` is active on both.

## Runtime

| Take | Result |
|---|---|
| 1066 | The only candidate tried was not opened: door entity origins are hinges, so the aim hit the frame. Restoration reported a native inventory read failure; a re-run of `Restore-LabNoteInventory` passed (`note-restoration-repair`), and both inventories match the baseline. |
| 1067 | With a yaw sweep, the host's ordinary open (−20°) converged natively on the client (revision 2 applied). The client's close never targeted the panel, which swung into the room. |
| 1068 | A ±90° close sweep still can't reach an inward-swung panel from outside. |
| **1069 (pass)** | Host opened `env_interiors_door_interior_350cm_01_19` and the client's native door opened. Client opened `env_interiors_door_medium_template_19` and the host's native door opened. Five other paired doors were already open on both peers. Dunwall fixture restored. |

**Harness:** [`Test-MissionDoor-v1069.ps1`](../../work/natural-moveset/Test-MissionDoor-v1069.ps1).
- Doors are found by name with `--entities`.
- Native state is read at openable component `+0x48` (0 closed, 2 open).
- The source approaches the door, aims with native look input, and uses X with a yaw sweep.
- The peer body steps back to its spawn first.

## Locked doors and keys (candidates 1214jd-1214jf, #185)

Updated 2026-10-24. Harnesses: [`Test-MissionLockedDoor-v1844.ps1`](../../work/natural-moveset/Test-MissionLockedDoor-v1844.ps1) (granted key), [`Test-MissionKeyPickupDoor-v1848.ps1`](../../work/natural-moveset/Test-MissionKeyPickupDoor-v1848.ps1) (native key pickup), [`Test-MissionLockedDoorRestart-v1843.ps1`](../../work/natural-moveset/Test-MissionLockedDoorRestart-v1843.ps1) (shared restart) and [`Test-MissionDoorSides-v1849.ps1`](../../work/natural-moveset/Test-MissionDoorSides-v1849.ps1) (opposite sides).

**Survey (1214jd, probe `--openables`, [mission-doors-v1837a](../../work/natural-moveset/mission-doors-v1837a)):** Edge of the World has 27 native doors. Three are locked and otherwise ordinary (not blocked, broken or linked): the black market back door, the Overseer outpost's interrogation room and Hypatia's apartment. Both worlds have the same doors and keys.

**The gap (confirmed runtime, [mission-locked-door-v1841b](../../work/natural-moveset/mission-locked-door-v1841b)):** her key unlocked her door, and the host's stayed locked. The door catalog left locked doors out ([chapter 57](57-native-door-catalog-and-traversal.md)), and keys are personal, so the player without the key was stuck.

**Fix (1214jf):**
- Locked doors that are otherwise ordinary join the door catalog (19 doors on Edge of the World instead of 16). Their native transitions stay refused until the lock byte clears.
- The lock is a channel state of its own. A native unlock seen in one world (lock byte `+0x1e8` from 1 to 0) sends the door message `Unlock` (kind 4, from either role, with no desired state).
- The other world calls the native lock routine `0x14074e4c0` with (component, 0, 0), exactly as the key branch of the interaction routine `0x14074c5c0` does after its key check `0x14074bea0`. **Confirmed static:** [decompile 800](generated/decompilation/800-decompile-14074e4c0-a9c0032.md) and [chapter 47](47-native-door-interactions.md). The routine writes the lock byte, emits the native unlock event, plays the unlock sound and dirties the owner and visual child. Its 32 prologue bytes are checked before the call, and linked doors stay excluded, so it never recurses.
- An unlock applied from the peer is not sent back. An unlock the peer dropped at a menu boundary is sent again on resume.
- An order to open a door that is still locked in this world waits for the unlock, bounded by the 5 s desired-state deadline, instead of retiring the channel.
- **Unit tests:**
  - a locked ordinary door is cataloged, and its transition is refused until it is unlocked;
  - a door that is both locked and blocked stays out;
  - `Unlock` is accepted from either role and carries no desired state.

| Take | Result |
|---|---|
| 1841b | The gap above (1214je, before the fix). |
| **1842a (pass, partner source)** | Key granted (setup). Her unlock reached the host's door natively 485 ms later; the host had no key. Her open converged in both worlds. |
| 1842b (fixture) | Host source: the unlock reached her door in 480 ms. The host's four open presses at the unlock yaw missed the door. |
| **1844a (pass, host source)** | 1842 with a yaw sweep for the open: her door unlocked 482 ms after the host's, then both were open. |
| **1843a (pass, shared restart)** | Her unlock reached the host (474 ms). The host died in a fall, both games restarted from the mission-start checkpoint (6.9 s) and the door session renewed once in each world. The door was locked again in both, because the checkpoint precedes the unlock. A second unlock was shared (470 ms), and her open converged. |
| 1845a, 1847a (fixture) | Native key pickup: the inventory probe refused every read for a few seconds after the pickup press. |
| **1848a (pass, native pickup)** | She picked Hypatia's key up from the street: native aim, use focus on the pickup, X. Her door unlocked, the host's 493 ms later, and both opened. |
| 1846a (fixture) | Opposite sides: a PowerShell precedence error in the stand coordinates, after the host's unlock had reached her door. |
| **1849a (pass, side correction)** | After the host's shared unlock, the host and the client each focused the door from opposite sides and pressed use back to back. Both doors started moving within 0.5 s. Her press became the latest order (revision 3, side 0). The host's door, which had opened from the other side, made one native side correction (close, then reopen) and was open on side 0 at 4.2 s. Both channels converged on revision 3. |

- **Keys are not shared pickups (confirmed runtime, 1848a).** The shared pickup catalog ([chapter 83](83-mission-shared-pickups.md)) does not include them: the key stayed in the host's world, where the host can still take their own copy. With the shared unlock they don't need to.
- **The inventory probe and pickups (hypothesis):** `--player-inventory` reads through the sword equipment, which the pickup animation interrupts. The same read worked again 10 s later and showed her key.
- **Not reconciled:** a lock that already differs when a channel arms. Only unlocks seen during a session are shared; shared checkpoints keep both worlds equal (1843a). A native relock is not shared either, because door scripts run in both worlds.

## Open

- **Closing on missions:** closing works in Dunwall; an outside approach can't target inward-swinging panels, so mission closing still needs a fixture.
- **Locked doors, keys, side corrections and checkpoint renewal** on missions are covered above (#185).
- **Usable sync** (windows, levers) on missions: done in [chapter 87](87-mission-shared-usables.md).
