# 103. Far Reach pulls in both worlds (#115)

Emily's Far Reach pulls map movables and bodies toward her. Before this work, a pull changed only her world: the
pulled object stayed put in Corvo's world, and the body pull looked refused. Candidates 1214fc to 1214fe make both
pulls end the same way in both worlds; 1214fh is the live candidate (its source equals 1214ff). A separate bug found on
the way, Corvo's standing copy of Emily's lethal victim, is open as #127 (see the last section).

## Native pull (confirmed runtime)

- Aiming is UsePower (63). Its power component, `arkCpntPowerGrapple`, is at UsePower +0x60 and holds the aimed
  entity's handle at +0xdc (object-carry-v1616b).
- The pull is GrabGrappledEntity (84, VT `1c3f8e8`). It holds the pulled entity's handle at +0x38: a movable in
  1616b, a corpse in 1628a. In the other body pulls (1623a, 1625b) the harness read `0x3fff0010` there while the DLL
  saw the body, so the field can be empty during part of a body pull.
- `pullCond` is a path condition in the `power_grapple_pull` midnight scene (confirmed static): true plays the pull loop,
  false goes on to pullOut. A pull without a target takes the false branch.
- **The body pull is not refused.** Emily pulls an unconscious body or a corpse whenever her reticle is on it:
  - her own choked victim (1623a, 1625b, 1627a, 1629b);
  - a host-streamed corpse after her lethal takedown (1628a).

  The earlier "refused" body pulls (1427a, 1430b, 1442a, 1621a, 1622a) never had the body under the reticle. The lab
  body harness placed Emily facing a cabinet (1622a film), and its pitch helper released the held trigger, because a lab
  input packet is the whole pad state (1629a).

## Changes

All in [shadow_stream.h](../../outputs/d2-coop-lab/src/shadow_stream.h) and
[held_object_presentation.h](../../outputs/d2-coop-lab/src/held_object_presentation.h), except where noted.

- **1214fc:** the source reports the entity at GrabGrappledEntity +0x38 as a flying held object (phase 2) from the start of
  the pull. The partner's copy then follows it through the existing held-object presentation (1214ea-ec):
  - it moves with the flight;
  - it settles where the source's copy rests;
  - it breaks when the source's copy is gone.
- **1214fd:** GrabGrappledEntity has no action of its own, so every source read during the pull failed and fell back to
  the presentation fallback packet (state 18 or 24). That packet discarded the held object, and the partner stopped
  following for the whole pull (object-carry-v1619b: Corvo's remote phase was 0 from 3.7 to 5.3 s). The fallback now
  carries the held or pulled object, revalidated on its own (handle, name key and world transform).
- **1214fe:** a body crosses as a held object too (mission-coverage-v1625b). Bodies are guarded:
  - A pulled entity with character health (`0x1c51588`, not a movable's `0x1c51268`) is a body. It is never reported
    broken, and a lost body ends its report. Its fate belongs to NPC authority.
  - The partner break refuses character health. Its explosion hit would kill the body; 1625b had counted a break on
    one.
  - While Emily's held tracker reports a body, her client does not present the host's pose on it
    ([npc_authority_stream.h](../../outputs/d2-coop-lab/src/npc_authority_stream.h)).
- **1214ff** ([npc_authority_stream.h](../../outputs/d2-coop-lab/src/npc_authority_stream.h)):
  - During a client's carry or paired corpse window, the host holds its corpse's physics animated so the client's poses
    drive it. Only a stopped channel released that hold.
  - The hold now ends with its window, or when the client's poses have stopped for 300 ms outside a carry. The corpse
    then falls from its last presented pose.
  - It was aimed at #127, but that corpse is never held (corpse hold releases 0 in 1630a and 1631a), so the change
    only covers the hold windows.
- **1214fg, reverted in 1214fh:** a ragdoll request for the host's corpse (see #127 below).
- Status: the held object counts pulls, fallback packets, bodies and lost bodies. The NPC stream counts held
  presentation skips and corpse hold releases.

## Results (confirmed runtime; films reviewed)

- **Objects.** In object-carry-v1620a (1214fd), Far Reach caught a whiskey tumbler:
  - 32 fallback packets carried it, and Corvo's copy followed all 6 flight samples and settled with Emily's.
  - When the tumbler later vanished in Emily's world, Corvo's copy broke too.
  - In Corvo's window the tumbler flies toward Emily and comes to rest.
- **Emily's own victim** (mission-coverage-v1627a on 1214fe; v1629b, v1630b on 1214ff; v1632b on 1214fg): she chokes
  the Edge of the World docks photographer and pulls her from 4.5 m.
  - The body moves 6.3-8.6 m in her world.
  - Corvo's copy follows through the held-object path and ends within 0.01 m.
  - The body is unconscious at full health in both worlds; there is one body report, with no break or loss.
  - In Emily's window the tendrils yank the body from beside the tripod. In Corvo's, it lifts off and flies toward her.
- **A host-streamed corpse** (1628a on 1214fe; 1630a, 1631a on 1214ff): after Emily's lethal takedown her copy is a
  host-streamed corpse (`client_corpse`, pairing ended).
  - Her aim and pull held its handle.
  - It moved 6.5-9.1 m in both worlds, with a 0.00 m gap, and is dead in both.
  - In Emily's window the corpse lies on the deck, then is yanked to her.

## Fixtures

- `Record-MissionCoverage -FarReachBody` (v1623-v1629): after a verified client choke or lethal takedown, Emily gets the
  pull upgrades and Far Reach. She tries up to eight spots 4.5 m from the body, starting along her approach line.
  - Each spot holds the trigger and reads the aimed handle.
  - From v1629 the aim walks thirteen offsets of about 2 degrees around the body, re-publishing the pad state so the
    trigger stays held.
  - The pull is then sampled for 7 s in both worlds: player states, aimed and pulled handles, body positions, NPC entries
    and held status. Health is read at the end.
  - v1630 also reads the target's ragdoll physics (mode and pending) in both worlds, at the takedown verdict and 3 s
    later.
- The 'corpsepull' case (v1626-v1627) targets pre-placed map corpses. Their places were cluttered or on roofs, and blind
  casts carried Emily into danger (1626a/b, 1627b/c), so it is no fixture for this.

## Remaining

- A body Corvo knocks out, pulled by Emily, is untested. Corvo's chokes left the photographer in the water (1624a), and
  the inland checkpoint guard's spots had no line of sight (1625a). His victim is a host-presented replica in Emily's
  world. 1214fe pauses that presentation while the body is pulled, which the host-streamed corpses of 1628a-1631a
  exercised.
- The spot search is a lab fixture: a spot can lose the body behind the partner (1632a), and Far Reach's failed casts
  move Emily.

## Corvo's copy of Emily's lethal victim stands up (#127, open)

- **Seen:** in Corvo's window, after Emily's lethal takedown of the photographer, the corpse falls, then stands in an
  idle pose and shifts it until something moves it (1628a, 1630a). Both worlds read it dead (health 0).
- **Cause (confirmed runtime, 1631a):**
  - Corvo's corpse never enters its ragdoll: its physics component (VT `1c54008`) stays in mode 3 at the takedown verdict
    and 3 s later, while Emily's copy is in ragdoll mode 0.
  - The host follows a client's lethal scene under an AI lease (`host_paired_hold`, then `host_paired_released`), and
    the death arrives through the combat relay (`native_death_complete`) while that lease holds.
- **Tried, 1214fg:** 300 ms after Emily's last victim pose, the host copied the presented pose into the ragdoll bodies and
  requested ragdoll mode 0 (0x734d90, then 0x734a10).
  - In mission-coverage-v1632a Corvo's corpse reached mode 0 and lay on the deck.
  - But Emily's copy, which follows the host's streamed pose, then balanced upside down beside the tripod. Without the
    call, her copy lay correctly (1630a, 1631a).
  - 1214fh reverts the call. The fix needs the host's ragdoll and its streamed pose to agree, or the death to reach the
    ragdoll natively.
