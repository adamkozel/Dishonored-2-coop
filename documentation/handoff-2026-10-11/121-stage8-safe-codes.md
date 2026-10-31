# 121. Stage 8 area 4.3: one safe code for both players (in progress)

Card #239. Before this, every safe in the two worlds had its own code, its own note text and its own dial start positions. That gave the safe-dial differences in every two-account world diff ([chapter 119](119-stage8-host-story.md), [chapter 120](120-stage8-items-per-player.md)). The research is in [the safe code map](stage8-step1-notes/safe-code-map.md).

## How it works (candidate safecodes-v2039, `safe_code_mirror.h`)

Every code is a Kiscule int, rolled by a script in each world. It is either the `Combination` of the `3_digits_lock.kiscule` decl nested in the safe's entity instance, or a level script that writes that `Combination`. No native safe code exists (confirmed static).

- **Host** (in plain gameplay, 3 s after a map load, read every 2 s): it sends a D2SC message when anything changed, and at least every 6 s. The message holds:
  - each lock's `Combination`, `Position` and `PositionDigit1-3`, read as ints at +0x1c of the int variable (vtable RVA 0x1c9f998);
  - each safe dial's state (a 10-state usable whose name holds `combinaison_numbers`);
  - the journal's three-digit formatted lists (the note and clue texts of safe codes).
- **Partner** (same map, plain gameplay):
  - it writes the host's `Combination` into its own lock and into the safe entity's `Combination` whenever they differ. This is a raw write, so no Updated event fires and nothing rerolls;
  - once per map load, it writes the host's `Position` and digits, and puts each dial on the host's state with the quiet state apply `0x14077a880(component, state, 0, 1)`, which fires no script event;
  - it overwrites in place each journal digit list whose text id and length match its own. It never calls the formatter with a foreign id, because the formatter stops the game on an unknown one.
- **After the start**, each player turns their own dials and opens their own safe. The contents stay pickups, per player ([chapter 120](120-stage8-items-per-player.md)).
- **Switches:**
  - Lab: `coop_safe_codes on|off`, on by default.
  - Product: `d2coop.cfg` `safe_codes=0` sets `D2COOP_SAFE_CODES=0`.
  - A host with it off sends nothing, so each world keeps its own codes, as in 0.9.6.
- **Status:** `<status>.safe-codes.json`. It has the counters plus `locksDetail` (entity hash, `Combination` and `Position` of this world's locks).

## Results

All on The Good Doctor (`campaign/institute/institute.mapinfo`), two lab games, `Test-SafeCodes-v2037.ps1` / `v2039.ps1`.

| Take | Candidate | Result |
|---|---|---|
| `safe-codes-v2037a` | safecodes-v2037 | Failed in the harness: no `.safe-codes.json`. The writer was only in the product status rotation, and the lab runs the diagnostics rotation. v2038 also writes it there |
| `safe-codes-v2038a` | safecodes-v2038 | **Pass.** The host found 2 locks, 6 dials and 4 texts. The partner holds the host's codes (183 and 15) and positions: 4 code writes, 8 position writes, 6 dial writes, 10 journal writes, nothing unknown. World diff equal (2158). Flaw: the host sent once, and the partner fell back to `waiting_host` after 15 s, because one clock served both the 2 s read and the 6 s resend |
| `safe-codes-v2038b` | safecodes-v2038 | **Mutation twin, red as required.** With `coop_safe_codes off` on the host, all 6 safe dials differ (`ld_combinaison_numbers`, `_15` to `_19`) |
| `safe-codes-v2039a` | safecodes-v2039 | **Pass** with the stricter test (the host keeps publishing, and the partner ends in `applied`). Host sent 8; partner applied, with nothing unknown; world diff equal (2103) |

The int value at +0x1c and the journal layout (`+0xd8` lists parallel to the decl's text ids) are now confirmed runtime by these takes: the partner's reads and writes agree with the host's values, and the world diff is equal.

## The campaign on two accounts (v2040a)

`Run-Campaign-v2040.sh a`: all nine missions, harness `Route-MissionVM-v2016`, candidate safecodes-v2039 (host story, items per player and safe codes on), host Corvo on this PC, VM partner Emily. Results in `work/natural-moveset/campaign-<mission>-vm-ipp-v2040a/`.

| Mission | Result |
|---|---|
| Dunwall | **Equal** (286). The take sat 1 h 45 min at one screenshot: the host game window was minimized, and gfxcapture gets no frame from a minimized window, so the single-frame ffmpeg grab never ended (confirmed runtime: grabs resumed once the window was restored). Harness v2017 kills a grab after 15 s and restores a minimized window first |
| Edge of the World | **Equal** (2103). In v2036a the safe dials differed; the safe codes fix it (confirmed runtime) |
| The Good Doctor | 2 values differ: `b_hyp_arrived_lab_graduated_cylinder_01` and `b_hyp_arrived_lab_retort_01` (host 0, partner absent), the same lab glass as v2036a |
| The Clockwork Mansion | Partner GameOver "You have lost a critical ally: Anton Sokolov" on the streets after the host carried him out; the host showed the copied plain GameOver. The streets map has its own Sokolov-loss script (`script_kiscule_gameover`, confirmed static), and the partner ran its `ActionGameOver` itself although it is a World class. The route fixes of v2038 and v2016 worked: the run passed Jindosh, the carry and the exterior door. Candidate gameover-v2041 blocks the partner's own `ActionGameOver` under host story (below) |
| The Royal Conservatory | The host died ("You Have Perished") at the last audiograph: the route switches the health hold off before the exit use while an enemy was still near. Route v2041 keeps the hold on until `boat_dust` has loaded. Not a sync fault |
| Dust District | **Equal** (2325). Differed in v2036a; the safe codes fix it (confirmed runtime) |
| A Crack in the Slab | The host's co-op saves switch stayed in `profile_saving` (phase 2, no switch), the second time after palace v2036a. Under study |
| Death to the Duke | **Equal** (608). Failed in v2036a and v2036b (skiff scene) |
| Death to the Empress | The host loaded `campaign/end/end_p` alone: its transition hold timed out after 90 s, and the partner stayed on `dunwall_return_tower_p`. Under study |

Four missions are now equal that the 0.9.6 rules also passed, plus Edge of the World and Dust District, which needed the safe codes.

**Candidate gameover-v2041** (`kiscule_action_mirror.h`): under host story the partner never runs its own `ActionGameOver` node (vtable RVA 0x1b79cc8). It reaches GameOver only from the host's `Death` message (`checkpoint_sync.h`), so the host decides. Counted as `hostStoryGameOverBlocked` in `<status>.kiscule.json`. What harmed the partner's Sokolov is still a hypothesis (his spawn is a replay of the host's run, out of the partner's own flow order). `Run-Campaign-v2041.sh a` reruns The Clockwork Mansion, Dunwall, The Royal Conservatory and A Crack in the Slab on it with harness v2017.

### Reruns on gameover-v2041 (v2041a, harness v2017)

| Mission | Result |
|---|---|
| The Clockwork Mansion | The host died on arrival in the streets ("Your Death Is Upon You"); the partner showed a plain copied GameOver. The last per-step status (step 23, before the door) has `hostStoryGameOverBlocked` 0 in both games, and nothing later was saved, so whether the partner blocked its own GameOver is not shown. A GameOver with no reason picks a random line from `ui_gameover_01`-`09`, so the host's text does not tell which game ended first (confirmed static). Hypothesis (a helper's reading of the streets script): the partner fell into `script_trigger_falldeath`, whose `ActionDamage` "Kill" (node 244843386, target Player, `DAMAGETYPE_INSTANTKILL`) is a World class; under host story the partner asked the host for it, and the host ran it on its own player. In v2040a and hoststory-v2031a the partner's own Sokolov GameOver came first. Harness v2018 saves both games' `.kiscule.json`, `.missionend.json` and `.checkpoint.json` at a failure to settle it |
| Dunwall | `mortimer_ramsey still alive after 40 attacks`. Under study |
| The Royal Conservatory | **Equal** (519) with route v2041 (health hold kept until `boat_dust`) |
| A Crack in the Slab | The co-op saves switch worked this time. At the end the partner loaded `boat_palace` after its 90 s hold, while the host stayed on `dust_p_lowchaos` in logic 11. Under study |

No screenshot grab timed out in harness v2017.

**Candidate gameover-v2041b** (from a helper's reading of `campaign-empress-vm-ipp-v2040a`):
- **Death to the Empress:** `Mission_DTTE` was 2 on the host and 0 on the partner from step 9 to the end (confirmed runtime). The partner's own `ActionGameflowMission` (node 1281) was blocked by host story during its load, and the host's run of it expired (`expired` 0 to 58 in that step). At the end the mission-end message found no active mission on the partner (`no_active_mission`), and the host loaded `end_p` alone. Why the host's load-time runs expired is a hypothesis (the partner's long arrival scene). v2041b: the host's gameflow mirror also sends missions in state 2, and the partner activates a mission that is 0 there (`missionActivated`). Completion stays with the mission-end message.
- **Co-op saves:** a host stuck in `profile_saving` with no timeout means `serviceCoopSaves` stopped running for at least 15 s (its 15 s deadline never fired; confirmed static). v2041b writes `serviceAgeMs` and `serviceGapMaxMs` in the saves status, so the next occurrence shows the gap.

`Run-Campaign-v2041b.sh a` reruns Death to the Empress on it: `campaign-empress-vm-ipp-v2041ba` is **equal** (249). The partner's `Mission_DTTE` activated on its own this time (`missionActivated` 0), so the new path was not exercised. (Its `vm-sync.log` was committed while the take ran; the full log is `vm-sync.complete.log`.)

**Candidate playerdamage-v2041d** (from a Ghidra reading of ActionDamage): an `ActionDamage` Hurt (input 2) or Kill (input 4) whose target is this world's own player runs in that world only. It is never blocked, mirrored or asked of the host (`playerDamageLocal`), and the host refuses a partner request for one (`playerDamageRefused`). The check reads the target input's link to a Player node (`arkKisculeNodePlayer`, vtable RVA 0x1cda848), or resolves the target as the handlers do (0x528040 with key 0x3984148, against the player entity's handle at +0xa8). A Player node resolves to the player of the world that runs it, so a Player-targeted Kill run on the host's behalf hits the host (confirmed static). `Run-Campaign-v2041c.sh a` (harness v2018) runs Dunwall (route v2041), A Crack in the Slab and The Clockwork Mansion on it.

### Reruns on playerdamage-v2041d (v2041ca, harness v2018)

| Mission | Result |
|---|---|
| Dunwall | **Equal** (286) with route v2041 (Ramsey attacked from behind) |
| A Crack in the Slab | **Equal** (480). The co-op saves switch worked, and no Replay Mission prompt opened |
| The Clockwork Mansion | The host died again on arrival in the streets ("You Have Perished"); the partner shows "You have lost a critical ally: Anton Sokolov". Details below |

The Clockwork Mansion, from the failure status that harness v2018 now saves (confirmed runtime):
- **Who ended first:** the host. Its logic transition is `sent`, the partner's is `applied` (3 transitions from the host), and the partner blocked its own `ActionGameOver` once (`hostStoryGameOverBlocked` 1, `script_kiscule_gameover` node 1399851223). Neither game ran a `playerDamageLocal` or `playerDamageRefused`, so the fall-death hypothesis of v2041a does not explain this death.
- **What ran first:** about 12.6 s after the streets loaded, the host's `script_kiscule_cablecar` ran keyframes 250 and 258, a `Collision` (node 1457201474), and 2.8 s later `ActionDamage` node 609, Hurt. The host died after it. The partner applied the host's run of the same nodes, then its own run of them was blocked by host story; its own GameOver came 0.4 s after its blocked Damage.
- **Where:** the host died on the streets spawn point (243.05, 184.58, 0.81), as in hoststory-v2031a, ipp-v2040a and ipp-v2041a. Before host story, v1929a stood at the same point and passed. The partner stood on exactly the same point.
- **Also seen:** the partner's world chose two different `ActionSpawner` nodes on the streets load (791556406 and 1248112637, where the host ran 1200362106 and 391263537). It asked the host for them, and the host ran them (`applied_partner`), so the host world gets the partner's choices as well as its own. This is a separate host-story fault (hypothesis: these spawners pick at random).

What node 609 hurts is not recorded. **Candidate damagetarget-v2041e** (diagnostic only) records, for every `ActionDamage` Hurt or Kill that runs or is applied, the entity its target resolves to in that world (`damage_target` and `apply_damage_target` events; `count` is the handle, and the local player shows as `<local player>`). `Run-Campaign-v2041e.sh a` reruns The Clockwork Mansion on it.

**The streets scripts** (a helper's reading of `madinv_streets_p_lowchaos.entities`; confirmed static):
- `ActionDamage` 609 is `sleep_no_reaction_damage` (amount 1) on the Sokolov that `script_spawner_sokolov` spawns in the cable car carriage. It runs from the `player_pull_sok_out_of_carriage` scene ("Player Leave With Sokolov"), after the ride from the mansion reaches the lower station, where both players stand.
- `Collision` 1457201474 is a fake collision under the carriage at the lower station, with no link to 609.
- The witch ambush dead bodies come from `test_player.kiscule`: Corvo runs spawners 1200362106 and 391263537 (dead bodies 8 and 10), Emily runs 791556406 and 1248112637 (7 and 9). Nothing is random. Each world takes its own player's branch.

**Clockwork Mansion on damagetarget-v2041e (v2041ea):** the same death on arrival. The diagnostic shows both damage nodes hit Sokolov in both worlds, never a player (confirmed runtime): 746122563 in the house (`anton_sokolov.def_11756` on the host, `_11731` on the partner) and 609 in the carriage (`_14696` and `_14683`). The partner applied each of the host's runs once; the repeated `apply_damage_target` lines are the 750 ms wait for the partner's own flow, logged on every retry. The partner's own `ActionGameOver` was blocked 3.4 s after it applied 609. Its screen still shows the Sokolov reason, while the host shows "You Have Perished". A GameOver without a reason picks a random line, and "host died" only names the first game the harness found in logic 13. Hypothesis: Sokolov dies in the partner's world, the partner reaches GameOver with that reason by a path other than the blocked node, and the host copies it. Harness v2019 also saves both games' `.reset.json` (`deathCount`, `deathReceived`) at a failure to settle the order.

**Candidate charbranch-v2041f:** the host refuses a partner request for the other character's branch of a World node behind a character test (`characterBranchRefused`, a table in `kiscule_action_mirror.h`; first entries: the streets dead body spawners above). The partner gets the host's own spawners as they are. `Run-Campaign-v2041f.sh a` reruns The Clockwork Mansion on it with harness v2019.

**Clockwork Mansion on charbranch-v2041f (v2041fa, harness v2019):** both players got through the arrival for the first time under host story. The host refused the partner's two Emily-branch spawners (`characterBranchRefused` 2), and the partner applied the host's Corvo ones. The host then carried Sokolov to Meagan (steps 25 to 29). The partner's game ended at step 30, 3.3 s after it applied the host's Meagan talk. Its screen shows the Sokolov reason. The death counters settle the order (confirmed runtime): the partner has `deathCount` 1 and `deathReceived` 0, and the host has `deathReceived` 1 and `deathApplied` 1. So in every Clockwork Mansion failure since v2040a the partner ended first, and the host's "You Have Perished" or "Your Death Is Upon You" is the random line of a copied GameOver.

The streets GameOver (a helper's second reading; confirmed static): `ActionGameOver` 1399851223 (`ui_gameover_10`) runs from a gate fed by the carriage Sokolov's `IncapacitationNotification.Died`, by `DamageNotification` `DAMAGETYPE_PRE_SLEEP` on him, or by `DAMAGETYPE_GRAPPLE` while `sokolov_is_inside_carriage`. 609's sleep damage is not one of those types.

**gameoverstack-v2041g (v2041ga, harness v2020):** the GameOver switch now records its stack, and harness v2020 saves `.loading.json` at a failure. The partner ended first again (death counters as above). Its switch to 13 came from `0x525df2`, called from `0x7c11c0`. That is the Kiscule run loop's update call (`call [vtable+0x40]`, kind 1), not the input dispatcher `0x7c0e30` that host story hooks (confirmed runtime and static). `ActionGameOver` is a latent node:
- its input only arms it;
- its update, vtable entry 8 (`0x525d30`), counts down the delay at `+0x30`, sets the reason and switches to 13 at `0x525ded`;
- it then returns status 1, and 4 once it has fired.

Blocking the input therefore never stopped it.

**Candidate gameoverupdate-v2041h:** under host story the partner's `ActionGameOver` update returns finished (status 4, with the result fields the native update writes) without firing (`hostStoryGameOverUpdateBlocked`, a vtable slot wrapper installed with the dispatch observer). The partner's GameOver comes only from the host's Death message. Why the partner's Sokolov is lost in its own world is still open; if he dies there, the end diff will show it. `Run-Campaign-v2041h.sh a` reruns The Clockwork Mansion on it.

**Clockwork Mansion on gameoverupdate-v2041h (v2041ha):**
- **The partner fix works** (confirmed runtime). The partner blocked both its `ActionGameOver` input and its update at step 25 (`hostStoryGameOverUpdateBlocked` 1), 141 ms after its own blocked run of 609, and did not end.
- **The host lost its own Sokolov this time.** The host's own `script_kiscule_gameover` node 1399851223 ran 16.7 s after its 609, at step 26, with no other script action between. Its switch stack is the same latent update.
- **So the carriage Sokolov is lost in either world,** the host's in v2041ha and the partner's in v2041fa and v2041ga.
- **Overlap ruled out:** both players stood on the carriage exit in v1929a too, before host story, and that take passed. So the overlap alone does not explain the loss.

Harness v2021 saves the Sokolov, Meagan, player and avatar entities of both games at a failure. `Run-Campaign-v2041i.sh a` reruns The Clockwork Mansion on v2041h with it.

## Still to do in area 4.3

- **The campaign on two accounts:** The Clockwork Mansion streets death (cable car damage) and the partner's own spawner choices reaching the host; The Good Doctor glass; then one full run of all nine missions on one candidate.
- **Note text on screen:** check that the partner's note shows the host's digits. This is visual acceptance, recorded separately.
- **A partner opens a safe with the host's code** (a route step on Edge of the World).
- **Codes outside `3_digits_lock`:** the Jindosh lock (chapter 110) and other lock decls, if any.
- **Safe-room doors** (result-level door sync) and the door mutation twin.
- The 1.0 `d2coop.cfg` and README document `safe_codes=0`.
