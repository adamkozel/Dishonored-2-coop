# Shared doors on mission maps (candidate 1059c)

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

## Open

- **Closing on missions:** closing works in Dunwall; an outside approach can't target inward-swinging panels, so mission closing still needs a fixture.
- **Not yet covered on missions:** locked doors and keys, side corrections, and checkpoint renewal.
- **Usable sync** (windows, levers) is still gated to Dunwall with a 128-entry cap, while Edge of the World has 371 usables. It needs the identity-list intersection used for pickups.
