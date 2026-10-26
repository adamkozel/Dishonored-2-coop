# 113. Power effects on every install (#230) and powers in every mission (#228)

Release 0.9.5 showed the partner none of your power effects on a player's install
([#230](https://github.com/adamkozel/Dishonored-2-coop/issues/230)). This chapter covers the cause, the fix in 1214ko
to 1214kq (release 0.9.6), and the two-account matrix that shows every power of both characters, in both roles, in the
other player's game. It also covers Emily's powers in the host role and the powers cast in every mission
([#228](https://github.com/adamkozel/Dishonored-2-coop/issues/228)). Labels: confirmed runtime (CR), confirmed static
(CS), hypothesis.

## The bug

- **Found:** [powerfx-corvohost-vm-v1919a](../../work/natural-moveset/powerfx-corvohost-vm-v1919a) (1214kn, two
  accounts, Corvo hosting on this PC, Emily joining from the lab VM). Every Corvo power effect was refused in the
  partner's game with `scope_native_api`. Emily's Far Reach and Domino were presented in the host's game. (CR)
- **Cause (CS):** `powerFixtureApiMatches()` checked the native signatures and also required the game executable's
  path to be the lab copy, `work/asset-game/Dishonored2.exe`.
  - `nativePowerFxApi()` and the partner presentation paths used it: the Doppelganger clone, the Mesmerize spirit and
    its host object, the swarm rats, and the held grenade, Heart and trap props.
  - On any other install, the check failed, so the partner saw none of these.
  - Every lab take on this PC runs the lab copy, which is why the lab never saw it. The host's game in the VM takes is
    also the lab copy, which is why only the partner's game refused.
- **Diagnosis aid (1214kn):** each scope condition of the power effect path names its own refusal: `scope_native_api`,
  `scope_local_cast_off_fixture_map`, `scope_isolation`, `scope_local_not_gameplay`, `scope_peer_not_gameplay`,
  `scope_avatar_not_doppelganger`, `scope_avatar_ai_not_held`, `scope_avatar_follow_not_exact` and
  `scope_avatar_ai_target_stale`.

## The fix

| Candidate | Change |
|---|---|
| 1214ko | `powerApiMatches()` (in `power_fixture.h`) keeps the native signature checks without the path. The power effect path and every partner presentation path use it: `doppelganger_visual.h`, `grenade_prop.h`, `heart_prop.h`, `trap_prop.h`, `mesmerize_visual_fixture.h`, `swarm_rat_visual_fixture.h` and `native_mesmerize_host.h`. The lab-only fixtures keep the path check. |
| 1214kp | `time_bend.h`: the Bend Time status writes finite numbers only, since a non-finite value makes the status file invalid JSON. |
| 1214kq | 1214kp with the version resources 0.9.6: the release candidate of 0.9.6. |

## The power effect matrix

[Route-MissionVM-v1919](../../work/natural-moveset/Route-MissionVM-v1919.ps1) to
[v1925](../../work/natural-moveset/Route-MissionVM-v1925.ps1) add a `powerfx` step, played on two Steam accounts in
both pairings.
- **The step:** the caster is granted the power (and its upgrades) and mana, stands at the anchor near the Addermire
  station in Karnaca, and casts. The step then reads the other world's status files for the power's native effect.
- **Retries:** a cast that neither fired nor showed is retried at the next of four facings.
- **Targeted powers:** these stand the caster 6 m from the nearest live NPC, facing it.
- **Difficulty:** the matrix plays on Easy.

| Power | Judged in the other world by |
|---|---|
| Blink, Blink Assault | `power-fx.started`, `power-fx-stream.applied` |
| Windblast, Greater Windblast | `windblast-fx.applied` |
| Devouring Swarm, Double Swarm | `swarm-world.presented`, `swarm-rat-visual.created`, `swarm-spawn-fx.applied` |
| The host's Bend Time and Stop Time | the caster's `time-bend.requests` while active (the host's world is the one that bends) |
| The partner's Bend Time and Stop Time | the host's `time-bend.added` |
| Possession (with `possession_parasite` for a human) | `possession-fx.presented` |
| Far Reach | `grapple-fx.beams` |
| Domino | `domino-world.presented` |
| Doppelganger, Twin Shade | `doppelganger-world.presented`, `doppelganger-visual.created` |
| Mesmerize | `mesmerize-world-stream.presented`, `mesmerize-visual.created` |
| Shadow Walk | `shadowwalk-material.starts`, `shadowwalk-fx.started` |
| Dark Vision | viewer-only by design, as in the single-player game; the gesture is on the frames |

### Results (CR)

Every power of both characters, in both roles, shows its native effect in the other player's game.

| Take | Pairing | Presented in the other world |
|---|---|---|
| [powerfx-corvohost-vm-v1923a](../../work/natural-moveset/powerfx-corvohost-vm-v1923a) (1214kp) | Corvo host, Emily partner | The host's Blink and Blink Assault, Windblast and Greater Windblast, Swarm (25 rats) and Double Swarm (15 rats), Bend Time and Stop Time. The partner's Far Reach (beam and impact), Domino (68), Doppelganger (clone created) and Twin Shade, Mesmerize (spirit created) and Shadow Walk (material started). |
| [powerfx-corvohost-vm-v1924a](../../work/natural-moveset/powerfx-corvohost-vm-v1924a) (1214kp) | Corvo host | The host's Possession of a guard, with the parasite upgrade (16 presented). |
| [powerfx-emilyhost-vm-v1923a](../../work/natural-moveset/powerfx-emilyhost-vm-v1923a) (1214kp) | Emily host, Corvo partner | The host's Far Reach, Domino (66), Doppelganger and Twin Shade, Mesmerize and Shadow Walk. The partner's Blink and Blink Assault, Windblast and Greater Windblast, and Swarm and Double Swarm (123 presented). |
| [powerfx-emilyhost-vm-v1929a](../../work/natural-moveset/powerfx-emilyhost-vm-v1929a) (1214kq) | Emily host | The partner's Bend Time and Stop Time (the host's world bent: 1 and 3 added) and Possession (15 presented). |

| [farreachpull-emilyhost-vm-v1931a](../../work/natural-moveset/farreachpull-emilyhost-vm-v1931a) (1214kq) | Emily host | The host's Far Reach with `grapple_pull_npcs` pulling a guard: beam and impact. |
| [farreachpull-corvohost-vm-v1931a](../../work/natural-moveset/farreachpull-corvohost-vm-v1931a) (1214kq) | Corvo host | The partner's Far Reach pulling a guard: beam and impact. The take then failed: the host died at the closing difficulty step, after the pull. |

- 1214kq differs from 1214kp only in its version resources, so the 1214kp takes stand for 0.9.6.
- In the Emily-host takes the swarm's rats are the host's own native rats, so no rat visual is created there (the
  world shows the swarm 123 times).

### Failed takes, kept

| Take | Why |
|---|---|
| powerfx-emilyhost-vm-v1919a | A PC shutdown at step 12; recovered with Recover-InterruptedVMTake. |
| powerfx-corvohost-vm-v1920a, powerfx-emilyhost-vm-v1920a (1214ko) | The mobility casts left the caster swimming in the harbour, so the later powers never fired. The host's Bend Time was judged on a partner-only counter. |
| powerfx-corvohost-vm-v1921a, powerfx-emilyhost-vm-v1921a | The host was not in gameplay at the start (object-carry-v1921a had left it out), and a status read failed. |
| powerfx-corvohost-vm-v1921b | Presented everything it cast. Swarm, Double Swarm, Possession and Twin Shade were not cast after earlier casts had moved the caster. |
| powerfx-corvohost-vm-v1922a, powerfx-emilyhost-vm-v1922a | Both ended in GameOver to the dock guards. Emily's anchor was Meagan's skiff, with no floor in reach. |
| powerfx-corvohost-vm-v1923a, powerfx-emilyhost-vm-v1923a | Possession needs the parasite upgrade for a human. The partner's Bend Time file was read through a flattened list (`$(...)` unrolls a nested list). Fixed in v1924 and v1925. |
| powerfx-emilyhost-vm-v1924a | The VM's `--world-npcs` probe failed once (exit 45); v1925 retries it. |

## Held items

The grenade, the Heart and the stun mine a player holds are presented in the other world as props (`grenade-prop`,
`heart-prop`, `trap-prop`), which 0.9.5 also refused on a player's install. The `itemprop` step
([Route-MissionVM-v1930](../../work/natural-moveset/Route-MissionVM-v1930.ps1)) equips the item with
`coop_grant_item` at the anchor. It then samples the other world's prop status for 6 s; the prop must be created or
attached there.

| Take (1214kq) | Shown in the other world | Not equipped |
|---|---|---|
| [itemprops-corvohost-vm-v1930a](../../work/natural-moveset/itemprops-corvohost-vm-v1930a) | Corvo's grenade (host), Emily's grenade (partner) | both Hearts and both stun mines |
| [itemprops-emilyhost-vm-v1930a](../../work/natural-moveset/itemprops-emilyhost-vm-v1930a) | Emily's grenade (host), Corvo's grenade and Heart (partner) | the host's Heart and both stun mines |

The lab item grant equips only into a free left hand (`serviceLabItemGrant`, `grant_slot_occupied`), and the grenade
still held it. [Route-MissionVM-v1933](../../work/natural-moveset/Route-MissionVM-v1933.ps1) gives each player one
item per take and records the grant status. Every case passed (CR): each item was equipped (`granted_item_equipped`),
and its prop was created and attached in the other world.

| Take (1214kq) | Shown in the other world |
|---|---|
| [itemprops-heart-corvohost-vm-v1933a](../../work/natural-moveset/itemprops-heart-corvohost-vm-v1933a) | Corvo's Heart (host), Emily's Heart (partner) |
| [itemprops-trap-corvohost-vm-v1933a](../../work/natural-moveset/itemprops-trap-corvohost-vm-v1933a) | Corvo's stun mine (host), Emily's stun mine (partner) |
| [itemprops-heart-emilyhost-vm-v1933a](../../work/natural-moveset/itemprops-heart-emilyhost-vm-v1933a) | Emily's Heart (host), Corvo's Heart (partner) |
| [itemprops-trap-emilyhost-vm-v1933a](../../work/natural-moveset/itemprops-trap-emilyhost-vm-v1933a) | Emily's stun mine (host), Corvo's stun mine (partner) |

So the grenade, the Heart and the stun mine each player holds show in the other game, for both characters in both
roles.

## Emily's powers in the host role (#228)

The movement recheck ([chapter 99](99-movement-completion-coverage.md), movement-completion-recheck-v1917) found five
Emily power items with completion evidence only in the partner role. Each is now recorded with Emily hosting (her
SLOT80 pairing fixture) and filmed from the partner's game, one case per take after a fresh relaunch:

| Item | Host-role take |
|---|---|
| `emily_far_reach_aim_cancel_release_arrive` | [coverage-v1921-farreachcancel](../../work/natural-moveset/coverage-v1921-farreachcancel), [coverage-v1921-farreachfar](../../work/natural-moveset/coverage-v1921-farreachfar), and the Far Reach case of [coverage-v1917a](../../work/natural-moveset/coverage-v1917a) |
| `emily_dark_vision_cast_end` | [coverage-v1921-darkvision](../../work/natural-moveset/coverage-v1921-darkvision) |
| `emily_doppelganger_cast` | [coverage-v1921-doppelganger](../../work/natural-moveset/coverage-v1921-doppelganger) |
| `emily_mesmerize_cast` | [coverage-v1921-mesmerize](../../work/natural-moveset/coverage-v1921-mesmerize) |
| `emily_far_reach_pull_variants` | pending: coverage-v1934-farreachpull (Record-Coverage-v1934, a guard pulled with `grapple_pull_npcs`). [coverage-v1932-farreachpull](../../work/natural-moveset/coverage-v1932-farreachpull) pulled the guard natively (state 84; the partner presented variants 4-7 and 9), but its camera stood inside the gallery wall. On two accounts, the pull's beam shows in the other world in both roles (farreachpull-*-vm-v1931a, above). |

The host-role bottle pull ([Probe-ObjectCarry-v1917](../../work/natural-moveset/Probe-ObjectCarry-v1917.ps1)) failed
three times on a lost host command receipt after the SLOT80 load: object-carry-v1917a, v1921a and v1929a.

## Powers in every mission

From 0.9.5 on, every campaign take casts both players' powers near the start of each mission. From harness v1917 on,
the cast must move the caster in its own world, and its avatar by the same distance in the other world. From v1918 on,
it must also show its native effect in the other world. [Chapter 109](109-pairing-validation.md) lists the takes on
1214kq. A Long Day in Dunwall comes before the Outsider's Mark, so no one has a power there.
