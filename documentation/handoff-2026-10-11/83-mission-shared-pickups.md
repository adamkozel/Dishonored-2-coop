# Shared pickups on mission maps (candidates 1059–1059c, 1214jg)

Updated 2026-10-14. Continues the Dunwall-only pickup work in [chapters 64–67](64-native-steam-shared-coin.md) (one coin, then a 51-object catalog). This is the first of the remaining Dunwall-only gates to be lifted (gate list in [audit 1040](../../work/movement-integration/mission-swim-audit-v1040.json)). Audit: [mission-pickup-audit-v1065](../../work/movement-integration/mission-pickup-audit-v1065.json).

## Identity

| | Value |
|---|---|
| Live candidate | [candidate-npc-combat-v1059c](../../work/movement-integration/candidate-npc-combat-v1059c/identity.json), DLL `3B8BBEE7…` |
| Coin wire | 7 |
| Catalog-list wire | 1 (magic `D2CL`) |
| Action protocol | 73 (unchanged) |
| Bank | 70 (unchanged) |

1059 and 1059b are archived as failed intermediate steps.

## Steps

1. **1059: map gate removed.**
   - `bindSharedCoin` no longer requires the Dunwall tower; both peers must still be on the same native campaign map and world.
   - The per-map catalog bound rises from 64 to 512 (`coin_limits.h`).
   - A read-only probe census, `--coin-catalog`, runs the DLL's exact reader and selection. In Dunwall it reproduces the 51-object catalog from chapter 67.
2. **1060 (failed, confirmed runtime): mission worlds are character-specific.**
   - Edge of the World has 292 eligible pickups on each side: 124 currency, 75 ammunition, 61 food and 32 other.
   - The catalogs still differ. Corvo's load has script-placed tranquilizer bolts and springrazor ammunition; Emily's load has a coin and a banana instead.
   - The census also varies slightly between loads. A whole-catalog digest can therefore never match.
3. **1061 (1059b, failed): per-entry Hello pairing does not scale.** About 290 Hellos per 250 ms produced asymmetric settles (host 251, client 244), so neither side became ready.
4. **1059c (wire 7): one identity-list exchange.**
   - **Identity:** FNV-1a over name, item declaration, quantity and family.
   - **List exchange:** each peer sends its sorted list of identity and arm pairs in one reliable `CoopCoinCatalogPacket`. The packet holds at most 512 entries in 8,264 bytes and is scoped to session, epoch and map. It repeats every second until the peer confirms.
   - **Intersection:** both peers compute the same intersection. Unpaired entries lose their hook lease and stay ordinary local native pickups.
   - **Heartbeat:** Hello becomes a 10/s heartbeat carrying the shared digest and count.
   - **Ready:** requires equal digests and counts plus a fresh peer.
   - **Guard:** a changed peer list without a checkpoint renewal stops sync.
   - **Tests** cover malformed or out-of-scope lists, identity conflicts and symmetric digests.

## Runtime

| Take | Result |
|---|---|
| 1062 | Both peers 287 shared, same digest, ready after one list each; host pickup converged. The client's chosen item lay on the canal bed (harness aim error). |
| 1063 | 285 shared; the client's item was an NPC-carried pickpocket pouch, so X hit the worker's Talk prompt (harness item choice). |
| **1064 (pass)** | Ordinary native pickups by alternating roles, each converging with receipts on both peers and removal from both worlds. Shared currency rose 0 → 30 on both. Dunwall fixture restored. |

The 1064 pickups:

| Picker | Item | Family | Amount |
|---|---|---|---|
| Host (Corvo) | copper wire | currency | 10 |
| Client (Emily) | `pickups_pouch_01_2` | currency | 20 |
| Host (Corvo) | pistol bullet | ammunition | 1 |

Each pickup was made by placing the player at the item, aiming with native look input, and pressing X.

**Harness:** [`Test-MissionPickup-v1064.ps1`](../../work/natural-moveset/Test-MissionPickup-v1064.ps1). It uses world items above the waterline, excludes NPC-carried `pouch_*` items, and picks the nearest items.

## Pickup families on missions (candidate 1214jg, #186)

Updated 2026-10-24. Harness: [`Test-MissionPickupFamilies-v1855.ps1`](../../work/natural-moveset/Test-MissionPickupFamilies-v1855.ps1), Edge of the World in product mode. Each case takes one shared pickup natively:
- **pickup:** native aim, with use (X) pressed only while the pickup has the use focus;
- **pickpocket:** crouched behind the NPC carrying it, pressing only while that NPC has the use focus;
- **Far Reach:** LT on the aimed pickup;
- **container:** open the container usable natively, then take the item inside.

Each case must commit once in both worlds, leave both worlds and reach each player's own inventory. Lab setup only: the health hold, and Far Reach with its pull upgrade granted for the Far Reach cases.

**Found (confirmed runtime):** a theft's reward arrives with the game's `fromGrapple` flag set, from the LootObject state (47). The reward observer refused every `fromGrapple` reward, and one refused reward stopped all pickup sharing on the map (`ordinary_reward_identity_or_delta_unconfirmed`). In [mission-pickup-families-v1850a](../../work/natural-moveset/mission-pickup-families-v1850a), her pickpocket presses left the catalog re-armed by product mode with no receipts. A Far Reach pull of a pickup is an ordinary reward (`fromGrapple` 0), so it was never affected.

**Fix (1214jg, [coin_sync.h](../../outputs/d2-coop-lab/src/coin_sync.h)):**
- A reward from any of the local player's own mode state slots can be confirmed: the state id must resolve back to the same object, and its owner must be the player component. The item, wallet, claim-flag and outcome checks are unchanged.
- An unconfirmed reward is refused for that item only and no longer stops the sync.
- `coin-hook.json` keeps the last refused or `fromGrapple` reward's checks (bit mask), state id and vtable.

| Take | Result |
|---|---|
| 1850a (1214jf) | Food: the host's banana committed in both worlds and left both. Stored potion: her mana potion committed in both, and both inventories went from 1 to 2. Her pickpocket stopped the pickup sync (above). |
| 1851a (fixture) | The jellied eels were not in that load's shared catalog: catalogs vary slightly between loads. |
| **1852a** | Her jellied eels and banana (food) and the host's mana potion committed in both worlds: **food and stored potions pass in both roles.** Fixture: the Far Reach grant left both worlds on a native page, so the pull and the crouch never ran. |
| **1853a** | Her pickpocket of a dock worker's pouch committed on the first press: both players got +10. The reward had `fromGrapple` 1, state 47 and all eight checks. Her Far Reach pull of a mana potion committed in both worlds (both 1 to 2). |
| **1854a** | The host's pickpocket of a walking civilian: both +20. Her container: the metal crate opened in both worlds (603 ms), then the mana potion inside committed in both. The host's cash register opened in both (570 ms), but the coin inside never had the use focus (fixture: it was aimed at the closed position). |
| **1855a** | The host's cash register opened in both worlds (554 ms); its coin, aimed at its position after opening, committed in both (+1 each). **Containers pass in both roles.** A trash bin opened in both (565 ms), but native look stopped responding afterwards (fixture). |
| **1855b (host Emily)** | Run with `-HostCharacter emily -ClientCharacter corvo`; this harness version does not save the pairing in the take folder. The host's Far Reach pull of a mana potion committed in both worlds (both 1 to 2). It was an ordinary reward (`fromGrapple` 0). **Far Reach pickups pass in both roles.** |

- **Food** restores each player's own health. Both players were at full health here, so 0 was restored; Dunwall measured the healing ([chapter 67](67-food-and-grouped-pickup-validation.md)).
- **Stored items** go to each player's own inventory, within that player's native capacity.
- **Currency** is credited to both players.

## Regression: shared pickups off from 1214jl to 1214jp (fixed in 1214jq, #189)

- **Found (confirmed runtime):** in [route-resumed-join-v1873a](../../work/natural-moveset/route-resumed-join-v1873a), neither world's pickup session ever bound (`initial_binding_refused`). 1214jp records why a binding is refused (`bindRefusal`, `bindRefusalItem` and `bindRefusals` in `coin-sync.json`). [v1874a](../../work/natural-moveset/route-resumed-join-v1874a) gave `native_api` in both worlds.
- **Cause (confirmed static):** 1214jl's lab entity event trace (#187, [kiscule_action_mirror.h](../../outputs/d2-coop-lab/src/kiscule_action_mirror.h)) hooked three event posts, including the `pickedUp` post `0x1408848a0`. `nativeCoinApiAvailable()` requires the first 32 bytes of that function verbatim, so every binding was refused from 1214jl on. Pickups were silently unshared in the #187 and #188 candidates; #186's takes ran on 1214jg, before the hook.
- **Fix (1214jq):**
  - The trace hooks only `0x140883f80` and `0x140884920`; the use mirror needs only the latter.
  - `kCoinVerbatimRvas` ([coin_native_api.h](../../outputs/d2-coop-lab/src/coin_native_api.h)) lists the code the pickup API requires verbatim. A `static_assert` keeps every entity event hook site off it.
- **Verified:**
  - [route-resumed-join-v1875a](../../work/natural-moveset/route-resumed-join-v1875a): bound in both worlds (`bindRefusal` none, 287 shared).
  - [mission-pickup-families-v1855c](../../work/natural-moveset/mission-pickup-families-v1855c) (1214jr, the 1855 harness's default plan): the host's banana and her mana potion each committed once in both worlds.
- **Dormant pickups (confirmed runtime, the probe's `--pickup-census`, 1214jq):**
  - Both worlds hold the same pickup actors (about 458 on Edge of the World).
  - A pickup whose component flag byte (`+0xd0`) has any of its high bits set (`0xf0`) is not read, so it is not cataloged. In a world fresh from the mission start, about 165 are in that state. In a world loaded from a save, about 90 are.
  - After a join to a resumed host, 73 to 77 pickups readable in the host's world were dormant in the partner's, so the shared catalog left them out (`localOnly`).
  - Since 1214jr, the host's taking of such a pickup still reaches the partner, through the pickup presence ([chapter 106](106-partner-led-story-and-mid-mission-join.md#a-partner-joining-a-resumed-host-189)).
  - Since 1214jv (#203), the partner's taking of one reaches the host too, for authored names. [route-resumed-join-v1880a](../../work/natural-moveset/route-resumed-join-v1880a): she took `pickups_dried_lezard_01`, which was outside the catalog, and the host's world caught it up.
  - Pickups that wake later still never join the catalog, so they have no shared one-transaction commit; the presence is eventually consistent instead.

## Open

- **Pickup families on missions:** food, stored potions, pickpocket pouches, containers and Far Reach pickups are covered above (#186).
- **Mission catalog renewal** after a checkpoint reload: covered by the shared restart takes in [chapter 88](88-mission-shared-death-and-checkpoint.md) (coin sync re-armed and settled again after each restart).
- **Remaining Dunwall-only gates:** usable/door sync, checkpoint/shared death, Doppelganger visual, props, power effects, and the legacy guard/damage experiments.
