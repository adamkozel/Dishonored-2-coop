# 120. Stage 8 area 4.2: items per player (in progress)

Card [#238](https://github.com/adamkozel/Dishonored-2-coop/issues/238), from [the stage 8 brief](115-stage8-plan-brief.md). The user's decision of 2026-10-29: each player can pick up each item once, in their own world. Coins, runes, bonecharms, ammunition and elixirs are each player's own; story items reach both players; chaos stays shared and decided by the host. The starting map of today's sharing is [items-per-player-map.md](stage8-step1-notes/items-per-player-map.md). Labels: confirmed static, confirmed runtime, hypothesis.

## The switch (candidate itemsperplayer-v2033)

- **Lab:** `coop_items_per_player on|off`, off by default, so the 0.9.6 pickup tests keep their behaviour.
- **Product:** on unless the host's `d2coop.cfg` sets `items_per_player=0`. The proxy then sets `D2COOP_ITEMS_PER_PLAYER=0`.
- **The host decides:** it sends its setting as control flag 8 in its D2CT hello. The partner follows it in the product and under the lab product controller, as with host story (flag 4, [chapter 119](119-stage8-host-story.md)).
- **With it on:**
  - the shared pickup catalog (`coop_coin_sync`, request, commit and apply) never arms; a `coop_coin_sync on` from the product controller is refused and counted (`itemsPerPlayerRefusals`);
  - the pickup catch-up no longer gives a world the ordinary pickups its peer took.
- Status: `coin-sync.json` reports `itemsPerPlayer` and `itemsPerPlayerRefusals`.

## Results (confirmed runtime)

`Test-ItemsPerPlayer-v2033.ps1` on Edge of the World, both lab games on the product controller: the role takes the pickup natively; then the other player takes its own copy.

| Take | Result |
|---|---|
| `items-per-player-v2033b` | pass. The partner, then the host, took `pickups_mana_potion_01_13`; the host, then the partner, took `_17`. The other world kept its copy until its own player took it, and each take gave only the taker one mana potion. Both games had items per player on and no shared catalog. |
| `items-per-player-twin-v2033c` (`-Shared`, the 0.9.6 behaviour) | fails as it must: each take gave the other player a potion too, and the shared catalog was armed in both games. |
| `items-per-player-v2033a` | the mana potion case passed. The banana case failed only in the test: food is eaten at once, and the health hold kept health full, so the take showed no gain. Food is not used as a test case. |

## Item families (confirmed runtime)

The probe's `--pickup-census` now reports each pickup's item declaration, family (item+0x60), special index (item+0xc4), carried flag and quantity. On Edge of the World:

| Family | Items | Decision |
|---|---|---|
| 1 | money and valuables (coins, bathing salts, blood amber, statues) | own |
| 4 | ammunition (bolts, bullets, grenades) | own |
| 6 | food | own |
| 7, 8 | runes | own |
| 12 | notes and newspapers (special = note category) | story |
| 14 | elixirs | own |
| 15 | Sokolov's paintings | own |
| 16 | blueprints | own |
| 17 | keys | story |
| 22 | the rewire tool | story |
| 24, 27 | maps | story |
| 25 | books | own |
| 26 | whalebone fragments | own |

Bonecharms did not appear on this map's ground; they are treated as own.

## Story items to both players (candidate itemsperplayer-v2035)

- With items per player on, the pickup catch-up runs for story families only (`storyPickupFamily`, `readNativeStoryPickup`).
- **How it works:**
  - Each world sends the names of the pickups it still has (D2PP presence).
  - A story pickup the peer's world no longer has, in its last two presences, is taken natively in this world too, as if this player had taken it (`applyNativeStoryPickup`: claim, the native reward, the `pickedUp` event, then finish).
  - So the item reaches this player's inventory and leaves this world.
- **Limits:**
  - Character variants (`_corvo`, `_emily`) are never caught up.
  - The host catches up only authored names.
  - As in 0.9.6, a story pickup the peer's world never had would also be given (hypothesis: rare on authored maps).
- **Test:** `Test-ItemsPerPlayer-v2035.ps1` adds `name:role:story` cases. The other world's copy must be gone within 30 s, and the other player must hold the same item.

### Story item results (confirmed runtime)

- **`items-per-player-v2035a` and `v2035b` failed in the route.** The black-market key, the safe contest note and a newspaper never took the native use focus from the ground stands (the note's stands focused a cash machine), so nothing was taken.
- **Candidate itemsperplayer-v2036 adds a lab command,** `coop_items_per_player take <name>`. It takes a story pickup natively in this world through the same steps, and `pickup-catchup.json` reports `storyTake`. `Test-ItemsPerPlayer-v2036.ps1` uses it for story cases; own cases still take through input.
- **`items-per-player-v2036a` passed all four cases:**
  - Each story item (the partner's newspaper `pickups_news_kg_dunwallcoup`, the host's street map `script_eotw_street_map_5`, the partner's key `pickups_key_blackmarket_assistant`) left the other world within 6 to 16 s. The other player received the same item.
  - The host's mana potion `pickups_mana_potion_01_13` reached only the host.
- **The world diff:** the probe's world digest now puts story pickups in their own `storypickups` category, which the diff compares strictly. Other pickups stay per player.
- **Two accounts:** `Route-MissionVM-v2015 -ItemsPerPlayer` (with `-HostStory`) runs all nine missions on v2036 (`campaign-<mission>-vm-ipp-v2036a`).

### Two-account campaign with host story and items per player (candidate itemsperplayer-v2036, confirmed runtime)

Host Corvo on this PC, VM partner Emily on the second Steam account, co-op saves, story flags, host story and items per player on (`Run-Campaign-v2036.sh`). In every mission that reached its end the VM partner had items per player on, the shared catalog off, and 0 World runs of its own.

| Mission | Result | World diff at the end | Partner: blocked / asked / in flow / replayed |
|---|---|---|---|
| A Long Day in Dunwall | pass | equal (286) | 86 / 77 / 36 / 119 |
| Edge of the World | differ | 4 safe-dial values on the next map (area 4.3) | 222 / 207 / 118 / 260 |
| The Good Doctor | differ | the same 2 boat lab glass breakables as in v2031, absent in the VM world | 68 / 59 / 41 / 453 |
| The Clockwork Mansion | stopped | the host died at `step19-heal` (game over), as in v2031 | - |
| The Royal Conservatory | pass | equal (519). The result-screen hold (`resultScreenHeld`) fixes the v2031 VM exit | 148 / 131 / 114 / 205 |
| Dust District | differ | 5 safe-dial values on the next map (area 4.3) | 142 / 139 / 96 / 189 |
| The Dreadful Wale (bunker) | pass | equal (480) | 275 / 235 / 168 / 305 |
| The Grand Palace | stopped | before the mission: the host's co-op saves switch stayed in `profile_saving` for 90 s (its 15 s timeout never reported). Re-run as `campaign-palace-vm-ipp-v2036b` | - |
| Death to the Empress | pass | equal (249) | 586 / 575 / 402 / 679 |

Re-runs on the same candidate:

- **The Clockwork Mansion, `campaign-madinventor-vm-ipp-v2036b`, route v2038.** With host story, Jindosh is already in his lab when the host arrives (`step17-trigger-host.png`; the 0.9.6 take `campaign-madinventor-vm-v1929a` has no Jindosh there yet). The route turned its test health hold on only after that, so the host died. Route v2038 turns it on before the lab trigger.
  - The re-run then stopped at the mansion door. The door opened the native Go/Stay choice as logic 9 itself (caller 0x5369f4, 1 -> 9), not as a pending 9, so the harness counted the open choice as its hit without pressing A (confirmed runtime, from the per-step loading snapshots).
  - Why the choice appeared here and not in v2036a is a hypothesis: the host's Kiscule went to applied 5 / suppressed 2 at the door, against +1 applied in v2036a, so a partner-first request may have changed the host's chain.
  - Harness v2016 presses A on a logic-9 choice too.
- **The Grand Palace, `campaign-palace-vm-ipp-v2036b`.** The co-op saves switch went through this time. The skiff use started the departure scene (1 -> 5 -> 9 -> 5), but the scene ended back in plain gameplay with Meagan offering a talk, and no LevelTransition followed. In `campaign-palace-vm-hoststory-v2031a` the host sat in logic 5 until the transition.
  - The host's Kiscule was not stubbed (applied and suppressed 0).
  - Hypothesis: the host-side story pickup catch-up on the boat. Harness v2016 saves `pickup-catchup.json` and `kiscule.json` at every step to settle it.
- **`campaign-palace-vm-ipp-v2036c` was stopped by hand during its VM join** (a misread stall; it held only the VM sync). The lab was restored with `Deploy-SourceClock`; its `recovery.json` records that the interrupted-take recovery could not run, because the VM take had stopped the local client game.
- **`campaign-palace-vm-ipp-v2036a`** is a hypothesis: the host's engine thread blocked in the native profile save (0xd3bd0) for more than 90 s. The VM lost the host's packets, and the switch's own 15 s timeout never ran.

## Still to do in area 4.2
- The old pickup tests (`pickup_catchup_tests.h`, `coin_protocol_tests.h`, `Test-MissionPickupFamilies-v1855.ps1`, the join audit) run with items per player off. Their per-player versions are the tests above.
- Containers, pickpockets and NPC loot under items per player.
- Review the partner-first locker grants (#183) for doubled ammunition or money.
- The 1.0 `d2coop.cfg` and README document `host_story=0` and `items_per_player=0`.
