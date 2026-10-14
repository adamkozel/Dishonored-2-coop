# Shared pickups on mission maps (candidates 1059–1059c)

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

## Open

- **Families not yet exercised on missions:** food, stored potions, pickpocket pouches, containers and Far Reach pickups (the ones Dunwall verified).
- **Mission catalog renewal** after a checkpoint reload.
- **Remaining Dunwall-only gates:** usable/door sync, checkpoint/shared death, Doppelganger visual, props, power effects, and the legacy guard/damage experiments.
