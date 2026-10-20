# 106. Partner-led story steps and the mid-mission join (#134, #135)

Every campaign route before this chapter was led by the host (chapter 97). Two questions remained:
- **#134:** when the partner (Emily) does the story work, do the host's world and objectives follow?
- **#135:** when the partner joins a host who is already deep in a mission, does the world converge, and where does the
  partner start?

Harnesses: [Route-Mission-v1665](../../work/natural-moveset/Route-Mission-v1665.ps1) (`-LeadRole client`),
[v1666](../../work/natural-moveset/Route-Mission-v1666.ps1) (`-JoinAfterStep`) and
[v1667](../../work/natural-moveset/Route-Mission-v1667.ps1) (`-JoinFromMap`, NPC picks and loot stands).

## Partner-led Dunwall before 1214fr (route-dunwall-lead-v1665b)

- Emily led the Dunwall route (dunwall-full-v1300t) in her own world. The step that needs her to search Ramsey's body
  (step 6, `expect` 6) never completed, and she ended the take in the throne room.
- **Cause (confirmed static, `serviceGameflowMirror` in
  [mission_end_mirror.h](../../outputs/d2-coop-lab/src/mission_end_mirror.h)):**
  - the host publishes its non-mission gameflow node states (`D2GF`) about twice a second;
  - the partner sets every differing node back to the host's state through the native set-state routine (`0x4e9bd0`);
  - so an objective the partner completed in her own world was reverted unless the host's world completed it too.

## Partner advances (1214fr, gameflow_advance.h)

[gameflow_advance.h](../../outputs/d2-coop-lab/src/gameflow_advance.h):
- **Detection (partner):** a node that differs from a node the host has non-zero, and that the mirror did not set itself,
  is the partner's own advance.
  - It is held against the mirror for 10 s (`kGameflowAdvanceHoldMs`).
  - It is reported to the host with the host state it advanced from (`D2GA`, 424 bytes, up to 32 entries).
- **Rule:** both states are non-zero and below 16, and differ. Activations from 0 and deactivations to 0 are never
  reported, so each world's character branches (the `asCorvo` and `asEmily` nodes) stay in their own world.
- **Host:** applies an entry through the same native set-state routine when its node still has the `from` state, so the
  host's scripts react; `Already` when it has the `to` state; otherwise `Refuse`.
- **Counters** (the mirror's status): `partnerSent`, `partnerHeld`, `partnerExpired`, `partnerReceived`,
  `partnerApplied`, `partnerAlready` and `partnerRefused`.

## The join placement (1214fr, join_warp.h)

- In route-dunwall-join-v1666b the partner joined 27.5 m from the host, behind doors the host had already passed.
- [join_warp.h](../../outputs/d2-coop-lab/src/join_warp.h): a partner whose game followed the host into a mission
  (join-follow, `D2MF`, chapter 90) is placed 1.2 m behind the host, facing the host's way, with the game's own
  `setviewpos` at eye height (1.7 m above the feet).
  - It happens once per followed load, 4 s after both worlds have been in gameplay on that map together.
  - A partner already within 25 m stays where it is.
- The follow status (`.follow.json`) reports `warps` and `warpDistance`.

## Takes

- **route-dunwall-join-v1668a (fail, harness):** the partner waited in Edge of the World and never loaded the host's
  Dunwall mission ("The partner did not load the host mission").
  - **Cause (confirmed static):** a joiner follows only a host that publishes its mission (`D2MF`). A host publishes
    when it runs in product mode (`D2COOP_PRODUCT=1`) or after `coop_follow_host on`.
    - The lab runs without product mode, and the join (Route-Mission-v1666, v1667) turned follow on only for the
      partner. So the host published nothing.
    - This also explains route-dunwall-join-v1666b, whose partner was already on the host's map: nothing loaded, and
      the partner stayed 27.5 m away.
  - A product host always publishes, so this is not a product failure.
  - Fix: [Route-Mission-v1672](../../work/natural-moveset/Route-Mission-v1672.ps1) turns on the host's publishing
    before the partner's follow, and saves both follow statuses at the join and after it. The take is repeated as
    route-dunwall-join-v1672a.
- **route-dunwall-lead-v1668b (fail, harness):** the partner led Dunwall (route dunwall-full-v1301c) on 1214fr. Steps
  1-3 completed in both worlds, with the gameflow counts equal.
  - During step 4 (a 15 s wait) the partner's journal opened (logic 7, Lore & Maps; confirmed runtime from the frame).
  - The route's settle has no case for logic 7, so it timed out silently. All 40 attacks on Ramsey then went into the
    journal ("mortimer_ramsey still alive after 40 attacks").
  - Nothing in the route presses the journal's button. What opened it is not known (hypothesis: a late skip press).
  - Fix: [Route-Mission-v1673](../../work/natural-moveset/Route-Mission-v1673.ps1) closes the journal with B (counted in
    `notes.menuClosed`), and an attack settles the leading role whenever it is not in gameplay.
  - The take is repeated as route-dunwall-lead-v1673a. The remaining takes (1669b, 1671a and 1672a) also run on v1673.
