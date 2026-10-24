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

## The partner's kill of a key target (#140, candidates 1214ga-1214hl)

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

### The removal trace (1214hb, #140)

- **The removal event** (confirmed static): entity removal is the event `1439a6f30`. Two functions post it:
  - `FUN_140884980` (entity, buffer, event, args) posts it at once; it hands the event to the entity's own handler.
  - The event handler `FUN_1408759c0` (entity, ..., float delay) posts it after a delay through the variadic
    `FUN_140884030`, which is not hooked.
- **1214hb hooks both functions** in a lab build only (never in product). For an `arkNPC`, it keeps the handle, the path,
  the thread and the caller stack (image RVAs) of the last 24 removals, written with the corpse traces into the NPC
  stream status.
- **route-dunwall-lead-v1745a-c** (Route-Mission-v1710, partner-led, the corpse hold skipped; all pass): her copy of
  Ramsey (190318452) stayed in all three. The search passed, and the trace saw no removal of it.
  - The trace recorded 253, 510 and 767 NPC removals. They come in batches with one stack (`884b3a`, `884809`, `75b137`,
    `7ea9e4`, `8a4962`, ...), at one tick each, which looks like the world teardown of the restore.
  - The vanish rate on 1214gc-ge was 5 of 7 takes. More takes follow until one vanishes.
- **route-dunwall-lead-v1745d** (fail, the search found no Ramsey; confirmed runtime): her copy vanished 10.2 s after
  his death, and the trace caught its removal.
  - The removal came from the event queue's dispatch at `1408853e8`, with stack `8ad463`, `8ad211`, then the main loop
    (`45a61`, `451c9`, `e3c21`, `166d9c`).
  - The dispatch (confirmed static) takes a queued record, posts its event (`record+8`) to its entity
    (`record+0xe8`) through `FUN_140884980`, and frees it. Other NPCs were removed through the same stack within the
    next 3 s.
  - So the removal was queued earlier, about 10 s before, around the death. The queued removals had been overwritten in
    the 24-entry ring by the restore's teardown.
  - **Next (1214hc):** also hook the queue insert `FUN_1408851f0` (queue, record, entity, delay; it sets the due time at
    `record+0xe0`). A queued removal of an `arkNPC` keeps its delay and caller stack in its own ring.

### The physics removal and the corpse poses (1214hc-hj, #140)

Partner-led Dunwall routes (Route-Mission-v1710, from 1749 on v1749, which also saves both worlds' NPC-stream status at the
search and on failure). "Hold skipped" is the lab switch `coop_corpse_hold own_paired_skip`; the others use the product
configuration.

| Take | Candidate | Finding |
|---|---|---|
| route-dunwall-lead-v1746a/b | 1214hc (queue insert `FUN_1408851f0` hooked) | 1746b vanished again. It queued 43 NPC removals, all with delay 0 on one job-thread stack, and the 24-entry ring kept other NPCs. |
| route-dunwall-lead-v1747a/b | 1214hd (a never-overwritten watched list for traced or dead NPCs) | 1747b vanished at 9.9 s (confirmed runtime). Ramsey's removal was queued with delay 0 from the physics step `FUN_140c942d0` (TtPostSolve, TtDispatchCommands). It went through its listener dispatch (`cebd30`, `ce57a0`, the delegate list `c96120`) to a delegate (`883f60`) that posts the removal. Two other dead NPCs went the same way within 1 s. |
| route-dunwall-lead-v1748a-e | 1214he (her game drops that removal of a dead replica) | a-c: no removal was queued. d failed: her search found the entity at the host's spot, but no body (bare carpet, no use focus). e: the removal was queued 10 s after the death and dropped, and her search passed. Her game then queued it again several times per frame (94441 marks); the host queued none. |
| route-dunwall-lead-v1749a/b | 1214hf (the trace also samples her copy's skeleton) | 1749a (hold skipped): at 750 ms her game began presenting the host corpse pose on her unheld copy, and its root moved to the host root, 1 m above hers. At 1.9 s her ragdoll left the animated mode (physics mode 3 to 2) under that presentation, and the skeleton exploded: joints from z 52.8 to 117.6 for 300 ms, around a floor at 61. 1749b: the host root stayed at 61.0, and nothing exploded. |
| route-dunwall-lead-v1750a-f | 1214hg (her game presents a corpse only once its hold holds it) | No explosion and no removal in all six takes. b, d, f (hold skipped): her copy lay flat, native, and the search passed. a, c (the hold) passed. e failed: her held copy showed the host corpse pose, upright and sunk to the waist in the floor, and her press picked the body up. |
| route-dunwall-lead-v1751a/b | 1214hh (her own paired kill's native corpse streams to the host for 6 s) | Both failed. The host followed her settling corpse to within 1 cm. Then its hold ended, and the 1214fj freeze kept her bones on the host's native root, which sank 0.7 m; her held copy took that pose. |
| route-dunwall-lead-v1752a/b | 1214hi (her settle stream ends the host's freeze) | Released to its ragdoll from her settled pose, the host body came to rest 0.45 m below her root, sunk into the carpet to the head and one arm (step10-at-host.png). 1752a passed; 1752b failed. |
| route-dunwall-lead-v1753a-e | 1214hj (the host keeps her settled corpse held and presented) | Her search passed in all five. The host and partner roots coincide at the death (z 61.15-61.29), with no removal and no vanish. The host view (1753a, step10-at-host.png) shows "Mortimer Ramsey (dead)" lying on the carpet. 1753e then failed at the optional end-of-mission talk to Meagan (below). |
| mission-coverage-v1754a/b | 1214hk (her Far Reach pull of a settled corpse returns it to the freeze) | The #127 regression (Record-MissionCoverage-v1632, Emily's lethal takedown of the Edge of the World docks photographer). b: the pull moved the body 7.33 m in both worlds. a: in Corvo's window the photographer stood up in her native idle again. That host corpse stays in physics mode 3, so the hold cannot take it, and since 1214hi the settle stream had ended the 1214fj freeze. |
| mission-coverage-v1755a/b, route-dunwall-lead-v1755a-c | 1214hl (the settle stream no longer ends the freeze) | 1755a film: the photographer lies on the deck in both windows. 1755b: the pull moved the body 7.33 m in both worlds, dead in both. Routes b and c passed. Route a passed Ramsey's search, then failed at the Meagan talk. |

- **The explosion** (confirmed runtime): her game presented the host corpse pose on a corpse whose native ragdoll was
  not held, and the ragdoll started under that pose, as the 1214ak launches did. A body thrown out of the world is
  then removed by the physics step (hypothesis), or leaves an entity with no visible body (1748d).
- **The host's frozen pose** (confirmed static, `npc_paired.h`): the host corpse follows her victim pose only until
  1.6 s after her last PairedHold. Her ragdoll falls at 1.9 s and settles by about 4 s, so the 1214fj freeze held a
  mid-scene pose on a native root that then moved.
- **The fix (1214hg-hj):**
  - Her game presents a corpse only once the corpse hold (`npcCorpseHoldAnimated`) holds it. Until then the corpse is
    native, as in single player.
  - After her own paired kill, her game streams her native corpse to the host (PairedHold and PairedPose) for 6 s after
    the death, without the hold or any presentation. Then her copy takes the normal hold.
  - The host recognises that settle stream (a reverse pose that is not a carry, 3 s or more after the death). It keeps
    the corpse held (keyframed) and presents her settled pose with its root, until a carry takes it or the channel
    stops. Her held copy then shows the same pose.
  - A host corpse the hold cannot take (physics mode 3, as in #127) keeps the 1214fj freeze, now on her settled
    bones (1214hl). Her Far Reach pull of a held one returns it to that freeze, whose native root the pull moves
    (1214hk).
  - The 1214he drop of a physics removal of a dead replica stays as a backstop.
  - Lab switches: `coop_corpse_present unheld` (the old presentation), `coop_corpse_hold own_paired_skip` (never hold
    her own paired kill).
- **Two observations from these takes:**
  - The end-of-mission talk to Meagan: route-dunwall-lead-v1753e and v1755a failed it ("Talk to meagan not registered").
    - On the v1750 steps, which add a host view of the corpse, Emily's arrival on the boat sometimes left the end
      scene waiting for the talk, and the harness's talk then did not register (1753c passed it).
    - On the original v1674 steps the scene started on her arrival in about 20 takes, including 1756a and 1756b on
      1214hl.
    - So the failures follow the harness step that teleports the host, not the corpse code (hypothesis for why the
      scene waits).
  - After her search, the NPC stream session stops on a world or native phase change (`world_or_native_phase_changed`,
    existing behaviour). That releases both bodies to their ragdolls, and the two copies then lie 0.1-2.5 m apart
    (1756b: 2.5 m). This is cosmetic, after the search.

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

## Joining from the title screen (#146, 1214ha)

- **The problem:** a partner who accepted the invite at the title screen ("Press any key") had their game follow the
  host's mission with no signed-in user. Its first save failed with "No connected user. You won't be able to save."
  (product-two-account-v1696a/v1699a).
- **Research** (Probe-TitleSignIn-v1741/v1742, read-only, the VM's game):
  - title-signin-v1741a: the main menu state object (`arkGameLogicStateMainMenu`, vtable RVA 1c60e68) is identical at
    the title and in the menu.
  - title-signin-v1742a (confirmed runtime): after the key, the Home section (`arkMainMenuSectionHome`, state+0x98,
    vtable RVA 1b6e698) sets +0x38 and +0x68 to 1, and the New Game and Difficulty sections fill their lists.
  - Confirmed static: the Home section's activate `FUN_1404fea30` binds its screen elements (+0x28, +0x58), and its
    deactivate `FUN_1404fefc0` unbinds them. +0x38 is therefore "Home shown", which first happens when a key is pressed
    at the title.
- **1214ha:**
  - The logic sampler latches `frontEndTitlePassed` once the Home section has been bound, for the life of the process:
    the title only shows at boot, and the user stays signed in.
  - Join-follow from the front end waits (`waiting_title_key`) until then.
- **campaign-titlejoin-vm-v1743a** (Route-MissionVM-v1743 `-TitleJoin`, two Steam accounts):
  - The VM joined while at its title. It stayed there for 20 s with `waiting_title_key` and no load.
  - After Enter, it followed and loaded the host's mission (same gameflow, 48 NPCs each, both armed).
  - It failed only on the harness's 3 m placement check. `join_warp.h` warps only from 25 m, and she was 12.6 m away.
- **campaign-titlejoin-vm-v1744a** (v1744: a partner within 25 m with no warp is accepted): the acceptance take.

## A partner joining a resumed host (#189)

Join-follow loads the host's mission from its start ([chapter 108](108-character-choice-and-pairings.md)). A host who
resumed from a mid-mission co-op save ([chapter 107](107-coop-menu-and-saves.md)) therefore has progress that the
partner's fresh world lacks.

**Harness:** [Route-Mission-v1879.ps1](../../work/natural-moveset/Route-Mission-v1879.ps1) (v1871-v1879) with
`-JoinAfterStep`. Only the host plays the steps, then the partner joins.
- [route-steps-resumed-join-v1871c.json](../../work/natural-moveset/route-steps-resumed-join-v1871c.json):
  1. take a banana;
  2. kill a dock worker;
  3. save through the pause menu (since v1876 a new file in the save slots is required);
  4. load that save.
- [route-steps-resumed-join-v1880.json](../../work/natural-moveset/route-steps-resumed-join-v1880.json): unlock and
  open the Overseer outpost's interrogation room with its key (granted, setup), go back to the docks, take the banana,
  save and load.
- `JoinAudit` compares both worlds at the join and 60 s later: shared pickups, doors (current/locked), usables, story
  flags, both pickup syncs and the pickup census.

**What transfers:**

| State | How | Evidence |
|---|---|---|
| NPC lives | The life roster (existing) | v1871c, v1876a: the worker killed before the save is dead in the partner's world too. |
| Objectives and story flags | The gameflow mirror (existing) | Equal gameflow, 0 flag differences. |
| Door open or closed | The door session (existing; confirmed static): the host commits each door's current state when its channel arms | 0 door differences. |
| Door locks | 1214js (below) | v1879a: the interrogation room the host unlocked and opened before the save is unlocked and open in the partner's world, with 0 door differences at the join and 60 s later. |
| Pickups | 1214jo-jr (below) | v1876a |
| Safe dials | Per world by design ([chapter 87](87-mission-shared-usables.md)) | 6 dial differences. |
| Script spawners | The host's load re-creates 8 of them with new handles: the same spawners under other handles | 8 pairs. |

### Pickups (1214jo-1214jr)

- **The gap (confirmed runtime, [v1871c](../../work/natural-moveset/route-resumed-join-v1871c)):** the banana the host
  took before saving was still in the partner's world.
- **1214jo:** the host sent the names of its readable pickups with each catalog list, and the partner awarded itself
  the eligible ones absent from them. It never ran ([v1872a](../../work/natural-moveset/route-resumed-join-v1872a)):
  shared pickups were off in every world, a regression from 1214jl fixed in 1214jq
  ([chapter 83](83-mission-shared-pickups.md)).
- **What "absent" means (confirmed runtime, [v1875a](../../work/natural-moveset/route-resumed-join-v1875a), probe
  `--pickup-census`):**
  - Both worlds hold the same pickup actors, about 458. A pickup with a high bit of its component flag byte set
    (`+0xd0 & 0xf0`) is dormant and is not read: about 165 in a fresh world and about 90 in a loaded one.
  - Readable names are therefore no presence. 1214jo awarded `amber_01_12151`, which was dormant in the host's world
    (flags 240), not taken.
  - Pickups spawned at load are named `<declaration>_<entity slot>` (the slot is the handle's high 16 bits). Most get
    the same names in both worlds; some differ (`ammo_bolt_pack_regular_12140` / `_12358`, `outsider_rune_12363` /
    `_12364`, each pair at one position).
  - Character variants differ by name at one position (`pickups_news_kg_ontherun_corvo` / `_emily`).
  - The take's save step had failed silently (the load list had no newer manual save), so the host's world was the
    mission-start autosave. Route v1876 requires a new file in the save slots.
- **1214jr ([pickup_catchup.h](../../outputs/d2-coop-lab/src/pickup_catchup.h), [coin_sync.h](../../outputs/d2-coop-lab/src/coin_sync.h)):**
  - **The census:** every pickup actor, active or dormant. Each 5 s cycle reads the entity slot table
    (`game+0x3130b0`, 128 KB) in one call and each occupied slot's handle. Only a slot whose entity or handle changed
    is classified again, at most 256 reads per tick.
  - **The host's presence (D2PP version 2):**
    - It holds each census name's hash; runtime-named entries also carry their base name and position.
    - It is sent after a census cycle when it changed, at least every 15 s, and at once for a new pair of worlds.
    - It is scoped to both sessions, both world epochs and the map, and only the entries in use travel (64 + 32 per
      entry bytes, at most 2048 entries).
  - **The partner's catch-up:**
    - After each local census cycle, the partner awards itself, through the shared pickup's native application
      (`applyNativeCoin`), each eligible readable pickup that is absent from the host's presence.
    - The pickup must not be a shared catalog entry, and must have been in this world 10 s before the presence
      arrived.
    - A runtime-named pickup counts as present if the host has its base name within 3 m. Character variants are never
      caught up.
    - Dormant pickups wait for a later cycle.
  - **Unit tests:** the packet (scope, size, sorted names, positions only on runtime entries), the runtime-name rule,
    and the target rules.
- **Result (confirmed runtime, [v1876a](../../work/natural-moveset/route-resumed-join-v1876a)):**
  - At the join, the banana was the only readable pickup in the partner's world that was missing from the host's.
  - 60 s later the partner had caught up exactly the banana (planned 1, awarded 1, failed 0), and the two worlds'
    readable pickups no longer differed on the partner's side.
  - Both pickup sessions were bound (291 shared).
- **Cost:** the first census cycle classified about 11,200 slots in 2.1-2.5 s at no more than 267 reads per tick; later
  cycles reclassify only changed slots.
- **The award:** a caught-up pickup rewards the partner as a shared pickup would have (currency to both, items to each
  player's inventory).
- **Not covered:** the partner's taking of a pickup outside the shared catalog does not reach the host (#203).

### Door locks (1214js)

- **The gap (confirmed static):**
  - The door session shares an unlock only as a transition seen during the session
    ([chapter 84](84-mission-shared-doors.md): "a lock that already differs when a channel arms" was not reconciled).
  - A door the host unlocked with its key before saving is therefore still locked in the partner's fresh world.
  - The host's first `State` (open) would then wait for an unlock that never comes and retire after its 5 s deadline.
- **Fix (1214js, [door_sync.h](../../outputs/d2-coop-lab/src/door_sync.h)):**
  - Until the partner acknowledges a first `State`, the host sends `Unlock` for its unlocked door just before each
    `State` resend (every 250 ms). The partner accepts the `State` only once paired, and `Unlock` needs the same pairing.
  - A partner whose door is unlocked ignores it; one whose door is locked calls the native lock routine, as for a
    shared key unlock.
  - `armUnlocksSent` counts these sends in `door-sync.json`.
- **Result ([v1879a](../../work/natural-moveset/route-resumed-join-v1879a), candidate 1214js):**
  - The host was granted the key (setup), unlocked and opened `script_door_overseersoutpost_3rdfloor_interrogationroom_01`,
    returned to the docks, took the banana, saved (SLOT68) and loaded.
  - The partner joined. At the join and 60 s later both worlds had 0 door differences, so the partner's copy was
    unlocked and open too.
  - The banana was caught up (only it). The host's outpost visit had advanced the gameflow, and the partner's gameflow
    equaled the host's by the end.
- **Saving in combat (hypothesis):**
  - The pause menu's Save Game did nothing while the host was fighting ([v1878a](../../work/natural-moveset/route-resumed-join-v1878a)
    in the outpost; [v1878b](../../work/natural-moveset/route-resumed-join-v1878b) after the dock worker turned and
    fought): the item was dimmed and no slot file was written.
  - Route v1879 tries three times, 15 s apart, and the door route has no fight.

### Takes

| Take | Candidate | Result |
|---|---|---|
| 1871a | 1214jn | Fixture: the host died in the worker fight. |
| 1871b | 1214jn | Fixture: an interior door's use never registered (dropped from the route). |
| **1871c** | 1214jn | The measurement: the banana stayed in the partner's world. The worker's death transferred. No door or flag differences. |
| 1872a | 1214jo | Route passed. The catch-up never ran (`presenceSent` 0). |
| 1873a | 1214jo | Both pickup sessions `initial_binding_refused`. |
| 1874a | 1214jp | `bindRefusal` `native_api` in both worlds (the regression). |
| 1875a | 1214jq | Pickups shared again (287). The census. The catch-up awarded a dormant `amber_01_12151` (wrong). The save had failed silently. |
| **1876a** | 1214jr | The banana caught up, and nothing else. |
| 1877a | 1214js | Fixture: the door unlocked from one side and never opened from it. |
| 1878a | 1214js | The door unlocked and opened (every stand). Fixture: no save in combat in the outpost. |
| 1878b | 1214js | Door again. Fixture: no save after the worker fought back. |
| **1879a** | 1214js | The door reached the partner unlocked and open, and the banana was caught up. Saved in SLOT68. |
