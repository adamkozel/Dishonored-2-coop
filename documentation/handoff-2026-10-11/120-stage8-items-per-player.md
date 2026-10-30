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

## Still to do in area 4.2

- The story-item take on v2035, with a key and a note.
- The world diff: split story pickups out of the per-player `pickups` category so their differences fail.
- The old pickup tests (`pickup_catchup_tests.h`, `coin_protocol_tests.h`, `Test-MissionPickupFamilies-v1855.ps1`, the join audit) run with items per player off. Their per-player versions are the tests above.
- Containers, pickpockets and NPC loot under items per player.
- Review the partner-first locker grants (#183) for doubled ammunition or money.
- The 1.0 `d2coop.cfg` and README document `host_story=0` and `items_per_player=0`.
