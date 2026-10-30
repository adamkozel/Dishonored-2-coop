# Stage 8 area 4.2: map of today's pickup sharing

Read-only research for [#238](https://github.com/adamkozel/Dishonored-2-coop/issues/238), done on 2026-10-30 by a helper agent and kept as the starting point for area 4.2.

The user decided (2026-10-29) that items are per player:
- each player picks up their own copy of an item in their own world;
- coins, runes, bonecharms, ammunition and elixirs are each player's own;
- story items reach both players' inventories;
- chaos stays shared, decided by the host.

Labels: confirmed static (read in the code), from the docs, or hypothesis. Paths are under `outputs/d2-coop-lab/src/` unless given.

## How release 0.9.6 shares pickups

- **The catalog** (confirmed static). The "coin" sync covers every eligible pickup, despite its name.
  - `bindSharedCoin` (`coin_sync.h:61`) builds the list with `selectNativeCoinCatalog`.
  - The peers swap catalog lists in D2CL packets (`sendCoinCatalog` :174, `receiveCoinCatalog` :185) and keep only the entries both worlds have (:313-323).
  - The product switches it on with `coop_coin_sync on` (`session_product.h:53`, :106).
- **The reward hook** (confirmed static). `observeCoinReward` (`coin_sync.h:227`, hooked at 0x841030) confirms a local reward, pickpocket and Far Reach included (`coinRewardStateOwned` :221).
- **Request, commit and apply** (confirmed static). In `serviceCoinSync` (:340-358) the partner requests, the host commits, and the other world runs `applyNativeCoin` (`coin_native_api.h:68`). That claims the pickup, gives the item and finishes the pickup (:80-93), so today the peer also gets the item and the pickup vanishes there.
- **Presence and catch-up** (confirmed static).
  - The D2PP packet is defined in `pickup_catchup.h:17-83`.
  - Both roles send presence (`sendPickupPresence`, `coin_sync.h:160`).
  - `servicePickupCatchUp` (:372) gives this world each eligible pickup the peer no longer has, through `applyNativeCoin`. It runs only while `coinSync.ready`.
  - Character variants (corvo/emily) are never caught up (`pickup_catchup.h:42`).
- **Checkpoint re-arm** (confirmed static): `checkpoint_sync.h:385` and :413.
- **Containers** (from the docs, chapter 83): they open in both worlds through the usable sync, and their content goes through the catalog.

## Item kinds

- **No catalog file** (confirmed static). The filter is built at run time: `readNativeCoin` (`native_coin.h:29-30`), `pickupFamilyValid` (`coin_protocol.h:38`) and `coinCatalogEligible` (`coin_catalog.h:12`).
  - Accepted item types: 1 (money), 4 (inventory stacks: ammunition), 14 (stored potions) and 6 (food).
  - Accepted only with special index -1 and when not carried.
- **Never shared or caught up** (from the docs, chapters 66 and 83): keys, notes, books, quest items and rings.
- **Runes and bonecharms:** no type found. Probably outside these four types, so not shared (hypothesis).
- **No story-item list exists** (confirmed static).

## Story items today

- **The Kiscule grant mirror** (`kiscule_action_mirror.h:53`, confirmed static):
  - it mirrors the inventory, loadout, mana and tutorial_end classes by count;
  - power grants stay personal (`kisculePlayerScopedItem` :59);
  - `kisculeCharacterGrants` (:76-80) maps three Corvo and Emily weapon grants.
- **So a story item reaches both players only when a script inventory node grants it** (confirmed static).
  - A physical key or ring taken in the world reaches only the taker (hypothesis).
  - Ramsey's ring is placed per world by ActionSpecialLoot, which runs in both worlds since hoststory-v2024 (chapter 119). Nothing copies the taker's ring to the other player.
- **The world diff** treats the whole `pickups` category as per player (`tools/diag/d2_world_diff.py:28`). Story pickups are hidden too (`runtime_probe.cpp:531-536`, confirmed static).

## Changes for area 4.2 (hypotheses until tested)

1. **Stop removing ordinary pickups from the other world.** Turn off the request, commit and apply path and the catch-up (`session_product.h:53`, :106; `coin_sync.h:376`). Keep presence for diagnostics only, or stop it.
2. **Story items reach both players.**
   - A story-item filter: types outside 1, 4, 14 and 6, plus declaration paths such as keys, quest items and the imperial ring.
   - Detect the local story pickup in `observeCoinReward`, which already sees every loot reward (`coin_sync.h:241`), and send a new grant packet.
   - The peer adds the item with the native inventory add +0x618de0 (used in the lab at `observer_camera.h:238-260`), mapping corvo and emily variants like `kisculeCharacterCounterpart`.
   - Decide whether the story pickup also leaves the peer's world.
3. **Kiscule grants:** keep inventory, loadout and mana mirrored. Review the partner-first locker grants (#183), which could double ammunition or money in the host's world.
4. **World diff:** split story pickups out of `pickups` so their differences fail, and add an inventory comparison for story items.
5. **Tests:** update `pickup_catchup_tests.h`, `coin_protocol_tests.h`, `Test-MissionPickupFamilies-v1855.ps1` and the join audit, which expect a pickup to leave both worlds (chapter 106).
6. **Unchanged:** chaos (`serviceChaosSync`) and the container usable sync.
7. **Still open:** NPC loot and random bonecharms that differ per world. The host decides which items exist (chapter 116).
