# 112. Final stage-exit audit (2026-10-24)

This audit checks each exit criterion of the [roadmap](02-roadmap.md) and its added scope against the chapters and
evidence takes. A read-only survey of all chapters mapped each criterion to its evidence; the gaps below were then
checked in the source and the takes. Labels: confirmed runtime (CR), confirmed static (CS), hypothesis.

**Result:** stages 1 and 3 are met. In stages 2, 4, 5, 6 and 7 the criteria are met except for the open cards below.

| Card | Stage | Gap |
|---|---|---|
| [#221](https://github.com/adamkozel/Dishonored-2-coop/issues/221) | 2 | An abrupt peer loss mid-mission (a crash or a dropped network) is untested. Only the graceful leave and rejoin are proven. |
| [#219](https://github.com/adamkozel/Dishonored-2-coop/issues/219) | 4 | Enemy hits other than the guard sword never reach the partner (CS). |
| [#220](https://github.com/adamkozel/Dishonored-2-coop/issues/220) | 5 | The partner's non-lethal branches past Ramsey and Stilton are unproven, and there is no story-flag diff at each mission's end. |
| [#206](https://github.com/adamkozel/Dishonored-2-coop/issues/206) | 6 | The tutorial's combat lessons stall when both players start it together. |
| [#222](https://github.com/adamkozel/Dishonored-2-coop/issues/222) | 7 | Hosting starts with the F7 key, not from the native Co-op menu. |

## Stage 1: protected development lab

| Criterion | Status | Evidence |
|---|---|---|
| Exact executable guards | met (CR) | The deploy guard refuses a changed baseline ([runbooks](06-runbooks.md), "Resume safely"). The installer checks `Dishonored2.exe` SHA-256 `B671F824...`. |
| Original-file integrity | met (CR) | Protected save hashes are checked before and after each take ([runbooks](06-runbooks.md), step 4). The baseline was re-recorded once, after the user's own launches, with the reason documented. |
| Reproducible restoration | met (CR) | Each take's `restoration.json`. From v1761 to v1887, 212 restores passed and none failed (the #173 closing note). Earlier restore timeouts are kept: coop-leave-v1770d, coop-hostsave-v1776a. |

## Stage 2: native transport and remote identity

| Criterion | Status | Evidence |
|---|---|---|
| Real two-account host and join | met (CR) | [Chapter 90](90-release-packaging-and-steam-sessions.md): two-account-session-v1221, product-two-account-v1225. The clean-PC proofs from the GitHub download, the newest being [product-two-account-v1891a](../../work/release/product-two-account-v1891a) (0.9.3). |
| Invite | met, with a caveat | F7 opens Steam's native invite overlay (v1225). Lab joins use the lobby id, as Steam's own join does. The human playtest (#99) covers a clicked invite. |
| Disconnect and reconnect with no stale ownership | partial | Graceful leave, rejoin and a mid-mission join on two accounts: [chapter 106](106-partner-led-story-and-mid-mission-join.md) (campaign-join-vm-v1703a, v1714b; #135). An abrupt loss is #221. |

## Stage 3: complete movement phase

| Criterion | Status | Evidence |
|---|---|---|
| Every applicable motion bound to native state, natural full-body quality, transitions, contacts, equipment and VFX, both roles, reviewed real-game videos | met (CR) | [Chapter 99](99-movement-completion-coverage.md): 49 completion audits. Each item has passing coverage cases and reviewed observer videos with verdicts. The [checklist](generated/movement-requirements.md) reads 103 of 105 complete, runtime-bound and visually accepted. `sit_stand` is not applicable (CS, audit 1240) and incapacitation is excluded by policy. The final reconciliation (chapter 99) corrected 23 stale visual columns and 8 stale binding columns after re-checking every video and case. |

Scoped limits are recorded in the audits: window front and right variants are not reachable, and Corvo's plain crouched
kick is not reproducible.

## Stage 4: shared combat and world authority

| Criterion | Status | Evidence |
|---|---|---|
| Enemies perceive and fight both players | partial | Guards keep the partner's clone as their combat target and their sword hits reach the partner (audit 1122; the attack relay, [chapter 95](95-shared-combat-authority.md), 1214ah; counterattack-v1258l). Every other enemy hit on the clone is dropped (`suppressCloneWorldDamage`, CS): guns, grenades, clockwork soldiers, hounds, witches (#219). |
| One authoritative damage and outcome path | met (CR) | Chapter 95: player hits and kills commit once and apply once (mission-multicombat-v1270e/f, clientnative-damage-v1275c, kill-unconscious-v1271m). |
| Synchronized health | met (CR) | NPC health follows the host (hostworld-damage-v1272b). Each player's own health lives in their own world by design. |
| Deaths | met (CR), with documented limitations | Chapter 95 and the life roster; [chapter 108](108-character-choice-and-pairings.md) #182. Rat and fish possession happen only in the partner's world, and corpse possession is unverified (release notes). |
| Spawn | met (CR) | The spawn census on every mission (#181, mission-spawn-census-v1825a-m, v1829b). |
| Bodies | met (CR) | Carried bodies (carry-body-v1277e/f), corpse authority, the ragdoll audit (completion-audit-v1368-ragdoll). |
| Physics | met for gameplay state (CR) | Movable props and pickups settle to the same resting places, and breakables break in both worlds ([chapter 105](105-breakables.md), physics-props-v1772b, mission-breakables-v1779c). Debris and dropped NPC swords stay per world, a decided limit. |

## Stage 5: interactions and progression

| Criterion | Status | Evidence |
|---|---|---|
| Doors | met (CR) | [Chapter 84](84-mission-shared-doors.md); door locks reach a joining partner (route-resumed-join-v1879a, 1214js). |
| Pickups | met (CR) | [Chapter 83](83-mission-shared-pickups.md) (#186), #203 (route-resumed-join-v1876a). |
| Inventory | met (CR), per player by design | Story grants in both worlds ([chapter 97](97-campaign-map-sequence.md)); the Dunwall loadout (audit 1311). |
| Objectives | met (CR) | Gameflow is equal at every step of the nine two-account campaign takes ([chapter 109](109-pairing-validation.md)). |
| Dialogue | met (CR) | The campaign routes run the story conversations through the native talk use in eight missions: Meagan, Sokolov, Hypatia, Delilah and Emily. The gameflow after each is equal in both worlds (for example campaign-eotw-vm-v1889a step 10, campaign-institute-vm-v1889a step 9, campaign-bunker-vm-v1889a step 25). |
| Mission decisions | partial | [Chapter 110](110-mission-decisions.md): equal decision flags at every map load (story-flags-v1836a). Kills by either player set the host's flags (route-dunwall-lead-v1835a). Ramsey and Stilton are partner-made in both worlds (route-dunwall-nonlethal-v1896a, route-backyard-stilton-v1908a). The rest is #220. |
| Scripted interactions | met (CR) | Shared usables ([chapter 87](87-mission-shared-usables.md), #187) and multi-state devices (chapter 106). Safes and the Jindosh lock are per world by design. |

## Stage 6: campaign and recovery

| Criterion | Status | Evidence |
|---|---|---|
| Checkpoint and save transfer | met (CR) | Paired autosaves and shared death and restart ([chapter 88](88-mission-shared-death-and-checkpoint.md)); real installs in 0.9.1 (#171). Saves are not copied: each player keeps their own co-op save, and a resumed host's world reaches a joining partner through the mirrors (#189). |
| Role restoration | met (CR) | Every campaign take checks both characters; the character choice is per save and per join ([chapter 108](108-character-choice-and-pairings.md)). |
| Simultaneous request deduplication | met (CR) | Chapter 88 #188: mission-simultaneous-v1869a, v1870a, v1870b. |
| Both-ready barriers | met (CR) | The transition hold (audit 1293) and every campaign take's mission starts and ends. |
| Every map, load and scene boundary | met (CR), except #206 | The whole campaign on two Steam accounts, all nine missions on 0.9.3 ([chapter 109](109-pairing-validation.md)). The map sequence between missions is in [chapter 97](97-campaign-map-sequence.md). |

## Stage 7: native UI and release

| Criterion | Status | Evidence |
|---|---|---|
| Native host, join, character-choice and recovery flows | partial | The native Co-op menu, co-op saves and Leave co-op ([chapter 107](107-coop-menu-and-saves.md)), the Character dialog ([chapter 108](108-character-choice-and-pairings.md)), and Steam's own invite and join. Hosting still starts with F7 (#222). |
| Packaging and rollback | met (CR) | The one-step installer and the byte-identical uninstaller (release-install-v1889b, 8/8; chapter 90). |
| Sustained stability | met (CR) | 2-hour soaks (mission-soak-v1316a, v1698a). Frame pacing on both PCs ([chapter 111](111-frame-pacing.md), frame-timing-vm-v1887b). Nine-mission two-account runs of about 5 h on each release candidate. |
| Online validation | met (CR) | The two-account campaign and the clean-PC proof from the GitHub download (product-two-account-v1891a). |

## Added scope (2026-10-21)

| Item | Status | Evidence |
|---|---|---|
| Co-op menu and saves (#156) | met (CR) | Chapter 107: coop-hostsave-v1755a (two accounts), coop-saveflow-v1772a. |
| Leave co-op (#157) | met (CR) | Chapter 107: coop-leave-v1755a (a later take, v1770d, failed on a restore timeout and is kept). |
| Character choice (#158) | met (CR) | Chapter 108: character-choice-v1779a. In the first mission the partner uses the game's own choice screen (release notes). |
| All four pairings and Corvo's partner powers (#159, #160) | met (CR), with documented limitations | Chapter 108: pairing-v1793a-d, v1781a; mission-partner-powers-v1819b, v1814a, v1820a, v1821a-c. |
| Validation (#161) | met (CR) | Chapter 109: the whole campaign in a new pairing on two accounts, and mission samples in the other pairings. |
