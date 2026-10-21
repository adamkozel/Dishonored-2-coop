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
  - It is reported to the host with the host state it advanced from (`D2GV` since 1214fs, 424 bytes, up to 32
    entries). 1214fr used `D2GA`, see below.
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
- **route-dunwall-join-v1672a (fail, harness; 1214fr, v1673):** the host now published its mission (182 packets), and
  the partner received it (179) and loaded Dunwall from Edge of the World (`loads` 1).
  - Its game then waited at the native mission card (logic 15, "press any key") until the 180 s wait ran out. A
    player presses a key there; the join's wait only compared maps.
  - Fix: [Route-Mission-v1675](../../work/natural-moveset/Route-Mission-v1675.ps1) settles the partner during the join's
    loading wait (A at the mission card, Emily at a character choice) for up to 240 s. Repeated as
    route-dunwall-join-v1675a on 1214fs.
- **route-dunwall-join-v1675a (fail, harness; 1214fs, v1675):** the partner loaded Dunwall again and showed the mission
  card ("Press any key", frame), but with a load still pending (logic 15, pending 5). v1675 pressed only with nothing
  pending, so the 240 s wait ran out.
  - [Route-Mission-v1676](../../work/natural-moveset/Route-Mission-v1676.ps1) presses through the card whatever the
    pending state, as the VM campaign harness does. Repeated as route-dunwall-join-v1679a on 1214ft.
- **route-dunwall-join-v1679a (pass, 1214ft, v1676): the mid-mission join works.** Confirmed runtime:
  - The host played Dunwall steps 1-6 alone (Ramsey killed and searched) while the partner waited in Edge of the World.
  - The partner's game followed the host's mission (`loads` 1), loaded Dunwall from its start and passed the mission
    card.
  - **The join placement moved her beside the host: `warps` 1, `warpDistance` 33.25 m, then 0.89 m from Corvo.**
  - The door, usable and NPC sessions were active on both sides at the join.
  - Objectives converged by the next step (both [237,3,3,4] after step 7).
  - Both worlds then played on together through the streets and into the Void.
  - **Not yet converged at the join (8 s after both settled):** two NPCs the host had killed before the join,
    `ld_spawner_ramsey_escape` and `ld_spawner_alexi_escape`, were alive in the partner's fresh world.
    - The life roster (1214ad) sends unbound deaths only while both worlds share the map, so nothing was sent before
      the join. Whether it caught up after the snapshot was not recorded.
    - route-dunwall-join-v1682a records Ramsey and Alexi in both worlds, with the roster's counters, after steps 7
      and 9.
- **route-dunwall-join-v1682a (pass, 1214fu, v1677):** the same join, now with authority records after steps 7 and 9.
  - Placement 2.04 m from Corvo. Gameflow equal at the end.
  - At the join, Ramsey and Alexi were alive in the partner's world, as in 1679a.
  - **Ramsey converged:** dead in both worlds by step 7. The life roster sent the host's death and the partner applied
    it natively (`lifeApplied` 1).
  - **Alexi (`ld_spawner_alexi_escape`) did not converge:** dead in Corvo's world, 40 health in the partner's at steps
    7 and 9. The partner refused the roster's death 5 times (`lifeRefused` 5, the attempt limit), then stopped trying.
    Hypothesis: she is held by a coup script in the fresh world, so the native kill is refused.
  - **Known limitation for the release:** after a mid-mission join, a few NPCs the host has already dealt with can
    remain in the partner's world.
- **route-dunwall-lead-v1668b (fail, harness):** the partner led Dunwall (route dunwall-full-v1301c) on 1214fr. Steps
  1-3 completed in both worlds, with the gameflow counts equal.
  - During step 4 (a 15 s wait) the partner's journal opened (logic 7, Lore & Maps; confirmed runtime from the frame).
  - The route's settle has no case for logic 7, so it timed out silently. All 40 attacks on Ramsey then went into the
    journal ("mortimer_ramsey still alive after 40 attacks").
  - Nothing in the route presses the journal's button. What opened it is not known (hypothesis: a late skip press).
  - Fix: [Route-Mission-v1673](../../work/natural-moveset/Route-Mission-v1673.ps1) closes the journal with B (counted in
    `notes.menuClosed`), and an attack settles the leading role whenever it is not in gameplay.
  - The take is repeated as route-dunwall-lead-v1673a. The remaining takes (1669b, 1671a and 1672a) also run on v1673.
- **route-dunwall-lead-v1673a and route-dunwall-autosave-v1669b (fail, both partner-led on 1214fr):** the attack on
  Ramsey now worked, but the loot of his body never registered ("Loot of mortimer_ramsey not registered").
  - The host-led take 1669a, at the same point: Corvo's kill completes `Obj_DW_EliminateRamsey` in both worlds at once
    (gameflow 239 to 237, then `Task_DW_GetRing`).
  - Emily's kill completed it in neither world at the step's end. A sampler of both worlds' mirror and combat status
    every 2 s during 1669b (confirmed runtime) showed why:
    - Emily's kill was an assassination (`assassination_damage`; Ramsey 50 to 0 in one press), and her world completed
      the objective natively (gameflow 237, `Task_DW_GetRing`).
    - The partner advances worked on her side: 3 nodes held, 20 reports sent. **The host received 0**, the holds expired
      after 10 s, and the mirror set her world back.
  - **Cause 1 (confirmed static): `D2GA` was already the guard body packet's magic** (guard_body_protocol.h). The host's
    receive dispatch tests that magic first and counted every advance as a rejected guard body.
    - **1214fs** gives the advance its own magic, `D2GV`.
    - `tools/repo/check-sources.sh` (run by CI) now refuses two packet structs with one default magic. Only `D2GA` was
      shared.
  - **Cause 2 (confirmed runtime, not yet explained): Ramsey's death ran only in Emily's world.**
    - Her combat channel for him retired with `initial_health_or_nonce_mismatch`, and the assassination stopped her
      combat session with `hit_range`.
    - The host's channels kept timing out in negotiation (`negotiation_timeout`), and her session rejected the host's
      packets from then on.
    - Corvo's Ramsey stayed alive. Even with the advance delivered, the two worlds would then differ on Ramsey himself.
    - [Route-Mission-v1674](../../work/natural-moveset/Route-Mission-v1674.ps1) records both worlds' NPC authority
      eligibility for Ramsey (`authority` step) before and after the fight. It runs as route-dunwall-lead-v1674a on
      1214fs.

- **route-dunwall-lead-v1674a (pass, 1214fs, Route-Mission-v1674):** the partner led all of A Long Day in Dunwall
  (route dunwall-full-v1301c), and every story step ended the same in both worlds (confirmed runtime):
  - her assassination of Ramsey: both worlds to gameflow 237 and `Task_DW_GetRing`;
  - her loot of his ring: both to [237,1,3,6];
  - the vault and lockers: both to [236,1,3,7];
  - the level transition to the streets she started, the balcony and street triggers, and the mission end into the
    Void (`void_01_p` in both worlds).
  - Gameflow counts were equal after every step. The host received 5 advance reports and applied 8 nodes, with none
    refused or expired.
  - **Ramsey himself stayed alive in Corvo's world.** He was bound in both worlds before the fight (NPC stream arm 10,
    host authority and client replica, combat channel ready on both sides). Her assassination was captured on arm 10,
    but her combat session stopped and re-armed between two status samples, so the kill ran only in her world. After
    it, Corvo's Ramsey stood alive at 50/50 where her corpse lay.
    - The objectives and the story are unaffected. The kill sharing is tracked as its own combat issue, [#140](https://github.com/adamkozel/Dishonored-2-coop/issues/140) (stage 4).
    - 1214ft records the session stops with the hit_range geometry; take route-dunwall-lead-v1676a.

- **route-dunwall-autosave-v1674b (fail, 1214fs, partner-led autosave route):** her attack brought Ramsey to 0 health
  in her world at press 4, as in 1674a. This time **neither** world completed `Obj_DW_EliminateRamsey`, and the loot failed.
  - So the partner's kill of Ramsey takes one of two wrong paths:
    - (a) it stays in her world, her script completes the objective, and the advance reaches the host while Corvo's
      Ramsey lives (1674a);
    - (b) no world's story script sees the kill (1674b; hypothesis: the kill reached the host and was applied there in
      a way the story script does not count as a player kill).
  - Both are #140. 1214ft's stop history and authority records (take 1676a) will show the path.
  - For #136, route-dunwall-autosave-v1678a lets the host do Ramsey's two steps, while the partner leads the rest and
    crosses the street balcony autosave alone.

- **route-dunwall-lead-v1676a (pass, 1214ft, steps 1-6 with authority records):** this time the partner's
  assassination killed Ramsey in **both** worlds, at the same spot (-43.8,-119.8): health 0/50 in Corvo's world too.
  - Both worlds then completed the elimination and the ring together at the loot step ([237,1,3,6]).
  - No combat session stop was recorded.
  - Here Ramsey stood still at his post. In 1674a and 1674b the host's AI had walked him 10-15 m, and the kill came
    right after the harness teleported the partner next to him.
  - The probe's NPC position and the range check read the same actor root (+0x300); after 1674a's kill that root and
    the partner were 1.7 m apart. So the trigger of the bad paths is still open (#140).
  - 1214ft's stop history covers the next partner-led kills (route-institute-lead-v1677a clears walking guards).

- **route-institute-lead-v1677a (1214ft; the partner leads The Good Doctor, route institute-partner-v1677):** steps 1-14,
  all by the partner, ended the same in both worlds, with gameflow equal after each (confirmed runtime):
  - the note on Hypatia's door, the guard clear, Hypatia's key, the glass door and the lab trigger;
  - the talk scene with Hypatia, the scroll case on her belt, and the transformation trigger (`ld_trigger_transfo_alone_2`).
  - Step 15, Hypatia's fight given to the host, failed ("still alive after 80 attacks"). The host's lookup teleported
    Corvo to face a wall. In host-led runs Corvo has crossed the transformation trigger himself and stands in the lab.
    This is a gap of the mixed-role route, not of the story path. The fight after a partner-crossed transformation
    trigger is left to the campaign takes.

- **route-dunwall-autosave-v1678a (fail, harness; 1214ft):** the partner led steps 1-4, and the host took Ramsey's two
  steps.
  - Corvo also assassinated Ramsey at his first press (50 to 0 health), natively in his own world. Neither world
    completed `Obj_DW_EliminateRamsey` at the kill.
  - So after an assassination the elimination completes only with the search of the body, as in 1676a, where both
    worlds completed it at the loot step.
  - All five of the host's loot stands missed: no search prompt over the body (frame).
  - **This revises 1674b:** its failure fits the same loot miss. Whether Ramsey died in Corvo's world there was not
    recorded. Path (b) above is therefore unconfirmed; path (a), the kill staying in her world, is confirmed (1674a).
  - [Route-Mission-v1677](../../work/natural-moveset/Route-Mission-v1677.ps1)'s loot step steers by the native use
    focus. At each stand it sweeps pitch and bearing and presses only while the body is focused (`$tag-lootfocus.json`).
  - Repeated for #136 as route-dunwall-autosave-v1681a, after the 1214fu take route-dunwall-lead-v1680a.

- **route-dunwall-lead-v1680a (pass, 1214fu, v1677): the partner leads all of Dunwall again, and the cause of #140
  is measured.**
  - Every story step ended the same in both worlds, through the mission end into the Void.
  - Her assassination of Ramsey (damage kind 3, `Assassination`) was refused by her capture as `hit_range` (1214ft stop
    history): her position was 5.30 m from Ramsey's root against the 3 m melee range. Victim (-43.83,-119.86), partner
    (-47.11,-124.02).
  - Her session stopped, the kill completed in her world only (0 health), and Corvo's Ramsey lived (50, then 47).
  - 1214fu's gap guard never fired (`replicaGapSuppressed` 0). The kill came through the assassination's paired scene,
    not a hooked damage event in the gap.
  - **1214fv:** an assassination or fatality finisher is a paired scene bound to its victim that moves both bodies, so
    the range checks of the partner's capture and the host's apply allow 8 m for them (3 m for swings and kicks), with
    a unit test. Takes route-dunwall-lead-v1683a and route-dunwall-autosave-v1683b (fully partner-led, her own kill of
    Ramsey) follow.

- **route-dunwall-autosave-v1681a (fail, fight; 1214fu):** the host's Ramsey fight (role host) met an aware Ramsey with
  guards and ended at 10 health after the route's 40 presses (host-led runs take about 22). This is fight variance.
  - For #136, route-dunwall-autosave-v1684a has the host lead to the streets, then the partner alone crosses the
    street balcony autosave and dies.

- **route-dunwall-lead-v1683a (pass, 1214fv, v1677):** the partner led all of Dunwall again.
  - Her assassination of Ramsey (second press) killed him in **both** worlds: 0/50 for Corvo and for her.
  - No hit_range stop was recorded, and no replica hit was held back. Every story step ended the same in both worlds,
    through the mission end into the Void.

- **route-dunwall-autosave-v1683b (fail, 1214fv, v1677; the fully partner-led autosave route):** her assassination of
  Ramsey worked (50 to 0 at the second press). The focus-steered loot then never saw the body.
  - The native use focus read none (`0x3fff0010`) at all 75 stands and aims (`step06-loot-lootfocus.json`).
  - In 1683a, on the same candidate and route opening, she searched the body normally.
  - So the partner's search of a body she assassinated is intermittent. Hypothesis: the replica corpse is sometimes not
    interactable after the host-committed kill. Tracked in #140.

- **route-dunwall-autosave-v1684a (fail, harness; 1214fv):** the host led, assassinated Ramsey at his first press, and
  then failed to search the body.
  - The native focus held the body at 3 of 75 aims, and X was pressed each time, but the search never completed.
  - The frame shows the two tower guards (`ld_spawner_guard_escape_1` and `_2`, about 5 m from Ramsey) hitting Corvo over
    the body, so their attacks interrupted the search.
  - In the host-led takes where Corvo fought Ramsey (1669a, 1679a, 1682a) the search worked; both host assassinations
    (1678a, 1684a) failed it.
  - route-dunwall-autosave-v1685a clears the guards around Ramsey (host, health hold) before the search. The partner
    still crosses the street balcony autosave alone and dies.

- **route-dunwall-autosave-v1685a (fail, harness; 1214fv):** the host assassinated Ramsey and cleared the guards, and
  his search of the body still did not register.
  - After this kill the objectives went to [237,2,7,2]; after fought kills they go to [237,1,4,5].
  - The partner-crossed autosave on Dunwall stays behind the Ramsey search, so it moves to a mission without that gate
    (Edge of the World). The mechanism is symmetric: both origins passed with the fixture autosave (1156), and the
    host-crossed real autosave passed in 1669a.

- **route-dunwall-hostkill-v1686a (diagnosis, 1214fv): a host assassination or finisher on Ramsey does not count for
  the story. This is a campaign blocker, found while chasing the #136 takes.**
  - The host fought Ramsey (50 to 0 in 10 presses) and the killing blow was `assassination_damage`, a finisher.
  - Ramsey reached 0 health in both worlds (one death each), but the Dunwall script never counted the kill: gameflow
    [237,2,7,2] with `Task_DW_KillRamsey` and `Task_DW_NonLethal_KnockoutRamsey` active.
  - So his ring could not be searched (1678a, 1684a and 1685a failed the same way after host assassinations).
  - The host's kill counts when the killing blow is a sword hit (1669a, 1679a), and when no combat session runs (1682a,
    the partner on another map).
  - **Cause (confirmed static; the kill evidence is confirmed runtime):**
    - the host captures its own player's damage on bound NPCs, empties the native result and replays it;
    - for an assassination the replay brings health to 0 without the native takedown's story event.
  - **1214fw:** the host player's own assassination or fatality finisher applies natively on the host, as non-replayable
    host damage already does, and HostWorld and Dead share it. The partner's path is unchanged. Unit test for the rule.
  - Takes route-dunwall-hostkill-v1687a, route-dunwall-autosave-v1687b (#136) and route-dunwall-lead-v1687c
    (regression).

- **route-dunwall-hostkill-v1687a (1214fw): the native host takedown did not fix it, and the real cause is the
  partner advance (1214fr).**
  - The host's finisher now applied natively (`hostPairedPassed` 1), and Ramsey died in both worlds. The objectives
    still stuck at [237,2,7,2].
  - Which takes counted the host's kill:
    - counted: 1669a (1214fr, when no advance reached the host), and 1679a and 1682a (partner on another map at the
      kill);
    - not counted: every host-led take with the partner present on 1214fs or later (1678a, 1684a-1687a).
  - **Cause (confirmed static, my 1214fr detection):**
    - the partner took any node that differed from the host's, unless the mirror had set it, for its own advance;
    - when the host completed `Obj_DW_EliminateRamsey` first, the partner world still showed the active state it had
      reached by itself (not set by the mirror);
    - so a false advance from completed back to active went to the host, and the host applied it.
  - **1214fx:** a partner advance is a change of the partner's node away from a host state that stayed the same since
    the previous scan. When the host moves, the host wins: no report, and the mirror applies its state. A held advance
    whose base state the host left is dropped. Unit tests; the status counts `hostMovedFirst`.
  - 1214fw's native host takedown is kept: it kills in both worlds and keeps the game's own takedown events.
  - Takes route-dunwall-hostkill-v1688a, route-dunwall-autosave-v1688b (#136) and route-dunwall-lead-v1688c (#134
    regression).

- **route-dunwall-autosave-v1687b (pass, 1214fw; #136): a real autosave crossed by the partner alone pairs both worlds,
  and both restart from it.** Confirmed runtime:
  - The host led to the streets. The partner alone crossed the street balcony trigger (`ld_trigger_balcony`), 28 m
    from Corvo.
  - The pair: host `local_save_originated` (slot 61), partner `mirrored_save_landed` (slot 62), s2/o1/m1 on both sides,
    0 refused, 0 rejected.
  - The partner then died, and one shared restart brought both worlds back in 7.0 s from the pair, each player at
    their own spot, with gameflow equal.
  - With route-dunwall-autosave-v1669a (the host crossing alone), #136 is covered for either player.
  - How the partner's crossing became a host-side save (hypothesis): the trigger mirror crossed the balcony trigger in
    the host's world, and the host's script autosaved there.
- **route-dunwall-hostkill-v1688a (pass, 1214fx): the Ramsey blocker is fixed.** The host fought Ramsey and finished
  him (0 health in both worlds), and both worlds completed `Obj_DW_EliminateRamsey` at once (gameflow [237,1,4,5],
  `Task_DW_GetRing`).

- **route-dunwall-autosave-v1688b (pass, 1214fx; #136 on the release candidate):** the host led steps 1-12, with
  Ramsey's kill completing the elimination in both worlds, then the search, the vault and the streets.
  - The partner alone crossed the street balcony autosave. The pair landed in both worlds (slot 62).
  - She died, and both restarted from slot 62 in 7.3 s, with gameflow equal.

- **route-dunwall-lead-v1688c (pass, 1214fx; #134 regression on the release candidate):** the partner led all of
  Dunwall. Ramsey died in both worlds (0/50 each), every story step ended the same in both worlds, and both reached the
  Void with gameflow equal.

## The mid-mission join on two Steam accounts (#135, Route-MissionVM-v1703)

[Route-MissionVM-v1703](../../work/natural-moveset/Route-MissionVM-v1703.ps1) adds `-JoinAfterStep`, `-LeaveAfterStep` and `-RejoinAfterStep` to the VM campaign harness. The VM waits unconnected in its main menu while the host alone plays the first steps. It then joins the lobby and follows the host's mission (join-follow), and later leaves as F8 does and joins the same lobby again.

**[campaign-join-vm-v1703a](../../work/natural-moveset/campaign-join-vm-v1703a/result.json) (pass, 1214fz, confirmed runtime):** Dunwall, joining after step 5, leaving after 6 and rejoining after 7.
- **Join:** the host alone had killed Ramsey. The VM followed the host's mission from its main menu (`loads` 1), and the placement moved Emily 53.66 m, to 2.83 m from Corvo. Gameflow was equal (237,1,4,5).
  - Ramsey and Alexi were alive in her fresh world, as in the lab takes 1679a and 1682a. This is the known limitation.
- **Leave:** the session ended. The host kept hosting its lobby.
- **Rejoin:** she reconnected in 4.5 s and converged to the step the host had played alone (237,1,3,6 to 236,1,3,7). Both sides rearmed, with banks active.
  - She stayed 69 m from the host, because the placement ran once per followed map.
  - **1214ga** (`join_warp.h` `joinWarpDue`, unit test) makes it due again on each new connection.
- **To the Void:** both played the rest of Dunwall together.
  - In the Void the joined partner's `Mission_DW` stayed 0 while the host's completed (3).
  - The gameflow mirror leaves mission nodes to each world's own script, because setting one natively would drive the mission flow. Non-blocking, like #94.

**[campaign-join-vm-v1714b](../../work/natural-moveset/campaign-join-vm-v1714b/result.json) (pass, 1214gf, release 0.8.2, Route-MissionVM-v1714):** the same join, leave and rejoin. The harness now requires the placement after the rejoin.
- **Join:** placed 2.38 m from Corvo, with gameflow equal.
- **Rejoin:** she reconnected in 4.1 s and converged to the host's gameflow (236,1,3,7).
  - **The placement ran again on the new connection** (`warps` 2, from 36.03 m) and put her **1.34 m** from Corvo.
  - Both sides rearmed, with banks active.
- **End:** both reached the Void with gameflow equal.
- campaign-join-vm-v1714a did not start, because the previous clean-PC take left the lab host paused (logic 2).

## The partner's kill of a key target (#140, candidates 1214ga-1214ge)

| Take | Candidate | Finding (confirmed runtime) |
|---|---|---|
| route-dunwall-lead-v1702a-c | 1214fz | 1702a: the kill was shared, then her corpse vanished and the search failed. 1702c: her kill was refused as `hit_range` (8.55 m, then 13.35 m), so Ramsey lived on in Corvo's world. In no take was her kill a paired scene. |
| route-dunwall-lead-v1706a-c | 1214ga (scene probe, `player_scene_probe.h`) | Her kill plays **`player/base/keytarget_finish/assassination_keytarget_right`** (subscene `assassination`, 3266 ms) in Assassination13. No takedown census held that folder. |
| route-dunwall-lead-v1708a-c | 1214gc (census 1707: the four key-target finishers, ids 120-123) | Her kill is now paired (variant 123). The host held Ramsey, and he died in both worlds at the same spot, with no `hit_range` refusal. But her corpse vanished 3/3: her world held an `ARK_CUE_NPC_DISAPPEARED` cue at the spot. |
| route-dunwall-lead-v1710a | 1214gd (corpse trace, `npc_corpse_trace.h`) | Vanished again. Her channel ended `native_lifecycle_changed` (entity gone). No trace was recorded, because combat kills set the corpse elsewhere. |
| route-dunwall-lead-v1711a-c | 1214ge | The hold kept: the corpse stayed and the search passed. The hold skipped: once vanished (at 9.95 s), once stayed. The root never moved or fell, and the mod's ash, Dead and roster paths applied nothing. |

- **Ring pickups:**
  - In the host's world, `ld_imperial_ring_pickup_corvo` moves onto the corpse at death; that is the ring the search gives.
  - In her world, after a vanish, `ld_imperial_ring_pickup_emily` is gone and a parentless `imperial_ring` sits at (0,0,0).
- **Hypothesis:** a native removal, such as a level-script despawn of the key-target NPC. The source of `ARK_CUE_NPC_DISAPPEARED` is the next step, in Ghidra.
- **Release 0.8.2** ships the paired key-target finishers: the kill is shared and the worlds no longer diverge. The release notes keep the search limitation, with the host searching as the workaround.
- **Census 1707:** the table generator (`generate_takedown_table_v1707.py`) now also emits the two helpers that 1214eo and 1214da had added to the generated header by hand (`nativeTakedownKnockdownChoke` and the shadow-form sword exception). The first 1214gc build failed without them.

## Multi-state devices and safes (#134)

The shared toggle family (chapter 87) leaves out safes, combination dials, typewriters, the piano and audiographs.
- **Static census** (usable censuses 1123 and 1124-1144, confirmed runtime):
  - Edge of the World has six 10-state combination dials (`script_combinaison_numbers_*`). Their current values are
    scrambled on every load and differ between the two worlds (1144: host 7/3/1, client 1/9/1), so each world has its
    own combination, read from its own notes.
  - The safe doors behind them are 2-state and disabled until dialled. The Dunwall safes (`ld_safe_door_4_1`,
    `esc_env_safe_door_4`) are plain enabled Open/Close toggles, left out only by their name.
  - Typewriters (5 states), the piano (3) and audiographs (flags bit 0, role-specific names) carry no shared state.
- **Take route-dunwall-safe-v1671a (pass, 1214fr, Route-Mission-v1671):**
  - The host opened `ld_safe_door_4_1`: his safe opened (state 0 to 1), and the partner's stayed closed.
  - He took the 150-coin gold ingot inside it (`pickups_ingot_gold_01`). The ingot left both worlds (pickup sync,
    chapter 83).
  - The partner then opened her own safe (state 0 to 1): it was empty.
- **Decision:**
  - Safes and combination dials stay per world. Each player opens the safe in their own world, with that world's
    combination.
  - What one player takes from a safe is gone for both.
  - Typewriters, the piano and audiographs stay per player (no story state; hypothesis for the typewriters and the piano,
    which only play their own animation and sound).
  - The release notes list this.
