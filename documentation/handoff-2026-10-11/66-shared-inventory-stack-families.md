# Shared inventory stacks and native capacity

Latest follow-on: [820–826 food and grouped pickup validation](67-food-and-grouped-pickup-validation.md) adds27 health food objects and resolves the representative ordinary ammunition/potion input gap. The following chapter remains the historical810–819 record; its then-current exclusions are superseded only where chapter67 provides evidence.

Continuation 810–818, 2026-10-13. See [currency catalog](65-currency-catalog-and-pickup-families.md) for the shared transport, reward observer and predecessor. This advances pickup families; it does not finish movement, world authority or playable co-op.

## Implementation and scope

The user asked whether every consumable would require individual development. The implementation now reuses the catalog, native reward observer, Steam host commit, per-object receipt, menu suspension and checkpoint renewal for **native type1 currency, type4 inventory stacks and type14 stored potions**. There are no per-ammunition-object gameplay handlers. Food, keys, notes, books and scripted/quest items remain excluded until their different outcomes are handled.

The existing exact executable is unchanged: SHA-256 `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`. This remains an isolated Dunwall experiment using the native Steam socket connection between two local test processes. It does not establish separate-account online sessions, campaign-wide binding, or shared containers. The selector currently refuses catalogs above64 members and restricts execution to the established Dunwall map.

The read-only [810 census](../../work/natural-moveset/simple-pickup-catalog-v810/result.json) found69 Simple pickups in both restored worlds:19 currency,4 ammunition,1 stored health potion,27 food,8 notes/newspapers,6 books,2 character-specific imperial rings,1 office key and1 carved boat. Every admitted member requires the exact component/declaration types, reciprocal owner, live generation, supported flags, non-carried state, ordinary special index and positive bounded quantity. Negative/sentinel note quantities are not treated as ordinary stacks.

The mixed catalog contains24 members and has digest `1df4dd38b1c62203`. Packet version3 adds the native family to the496-byte envelope and authored-identity digest. Old version2 packets fail validation. Both peers must agree on the catalog before applying rewards. Local pointers and handles do not enter the digest.

## Native outcome, not a raw inventory replacement

[native_stack_item.h](../../outputs/d2-coop-lab/src/native_stack_item.h) reads one exact existing inventory entry for type4/14. It verifies inventory vtable/owner/generation/lifecycle, bounded item count, unique declaration, quantity/capacity and a stable second read. Entries are56 bytes: declaration at+0, quantity at+0x20, capacity at+0x24. Unsupported types, duplicate declarations, changed arrays and quantities above capacity are rejected.

[coin_native_api.h](../../outputs/d2-coop-lab/src/coin_native_api.h) still uses the native claim, LootObject reward, pickedUp event, event-result destruction and finish path. Before and after values now refer to money or the exact native stack, according to family. Stack rewards must leave money unchanged, preserve the owned inventory entry, and produce a bounded positive grant, or zero if that recipient was already full. A receipt is reserved before mutation; unconfirmed partial applications cannot be blindly retried.

This preserves native modifiers. In the actual812 batch, the bullet box declares2 but awarded3 to each player; each of the other three bullet pickups awarded1. Each inventory ended with6 bullets,1 stored health potion and258 money. Health remained56. Merely adding the declared quantity would have been incorrect under these settings.

The present sharing policy is: the ordinary world object is consumed for both players; each player receives the amount accepted by their own native inventory. A full recipient accepts zero and still retires its world object. Existing inventory differences are preserved. First-person use/consumption remains native and local. This is not a claim that general inventory replication, shared consumable use, every gadget or all potion declarations are validated.

Relevant source: [native_coin.h](../../outputs/d2-coop-lab/src/native_coin.h), [coin_catalog.h](../../outputs/d2-coop-lab/src/coin_catalog.h), [coin_protocol.h](../../outputs/d2-coop-lab/src/coin_protocol.h), [coin_sync.h](../../outputs/d2-coop-lab/src/coin_sync.h). The old single-coin reader defaults to currency-only; stack support is an explicit option in the shared catalog path.

## Runtime evidence

| Evidence | What it establishes |
|---|---|
| [812 mixed batch](../../work/natural-moveset/shared-pickup-stack-batch-v812/result.json) | All24 objects, alternating source roles, installed reward observer, native Steam recipient, per-entry native grants, native registry removal, repeated developer requests and packet replay do not grant twice; complete inventory/currency/health restoration. Source is native API exercise, not ordinary input. |
| [813 recovery](../../work/natural-moveset/shared-pickup-recovery-v813/result.json) | Ordinary coin source on the new mixed catalog, one coordinated load per game,24 fresh members, new world/arms and empty receipts,3 old-action rejections, and ordinary Emily pickup after reload without manually rearming. |
| [815 capacity matrix](../../work/natural-moveset/shared-pickup-capacity-v815/result.json) | Recipient bullets9→10 while source receives its own native grant; recipient bullets10→10; recipient stored health potions10→10. All three acknowledge once and remove the native object in both worlds. Duplicates, unrelated inventory, health and money checked. |
| [817 final restoration](../../work/natural-moveset/pickup-catalog-restoration-v817/result.json) | Final paired native identities, roles, health, complete inventories,24 rebuilt members, empty receipts, released isolation, and rejected developer requests outside finite test scope. Consult the receipt for execution status. |

The batch and capacity recordings show the actual first-person game views and native HUD. Their fixed framing does not visually prove every distant object's disappearance or remote pickup animation. Native entity snapshots establish retirement; these clips are functional evidence, not animation approval. All video entries retain `complete=false` and `visual_accepted=false`.

Ordinary stack-input attempts are retained separately. [816 ammo](../../work/natural-moveset/shared-pickup-ammo-input-v816/result.json) failed before input because the chosen approach fell away from the pickup and the aim guard rejected the steep angle. [816b potion](../../work/natural-moveset/shared-pickup-potion-input-v816b/result.json) and [816c adjusted potion facing](../../work/natural-moveset/shared-pickup-potion-input-v816c/result.json) reached a stable aim but lacked a valid finite isolation window before input. All restored the complete baseline. These are test-setup failures, not successful ordinary pickups. Ordinary ammo/potion input remains unverified; batch API success is not substituted for it.

## Capacity fixture, archives and reproduction

[Candidate811](../../work/movement-integration/candidate-coin-sync-v811/identity.json) preserves the mixed-family implementation: DLL `EDAB91AA6B7CBF96BC442C917E4E4B14DDFB77ED03BB724C683EEDD29A186104`. [Candidate814](../../work/movement-integration/candidate-coin-sync-v814/identity.json) adds the finite capacity test fixture: DLL `1FAD933ED423A1DA4B80FFC80F043D6C464398D8ED75FE6FBAE38E76FFE42EB2`. The read-only probe remains `1ECB428D77B93915EA0417F3531E9103BB8E24DEC2336E874A809BA827810B22`. Both archives contain exact sources, build/deploy scripts and unchanged357-clip bank pins. Candidate804 remains available.

[pickup_capacity_fixture.h](../../outputs/d2-coop-lab/src/pickup_capacity_fixture.h) is development-only. `coop_coin_sync capacity <catalog-name> <quantity>` requires the active matched catalog, an unclaimed type4/14 entry, native idle gameplay, a finite isolated NPC fixture and no conflicting animation/equipment experiment. It uses the already proven native inventory setter at`140618de0`; it never writes raw quantity memory. It retains the original entry and quantity, expires after30 seconds, and restores through that native API on expiry/off. A world change retires the old identity. An unconfirmed restore demands isolated checkpoint recovery. The capacity setter is never used for a network reward.

Build/checks: [Build-CoinSync-v814.ps1](../../work/movement-integration/Build-CoinSync-v814.ps1), native world/ownership tests including stack lifetime/capacity cases, action protocol, checkpoint/resume tests, DLL load smoke and both actual357-clip banks. [Test-PickupStackBatch-v812.ps1](../../work/natural-moveset/Test-PickupStackBatch-v812.ps1), [Test-PickupCatalogRecovery-v813.ps1](../../work/natural-moveset/Test-PickupCatalogRecovery-v813.ps1), [Test-PickupCapacity-v815.ps1](../../work/natural-moveset/Test-PickupCapacity-v815.ps1) and [Test-OrdinaryStack-v816.ps1](../../work/natural-moveset/Test-OrdinaryStack-v816.ps1) preserve reproducible inputs. Each take copies its actual script before execution; later script adjustments do not rewrite past evidence.

Run native scripts only against validated isolated PID/path/start identities, holding the existing named operation mutex. Consume terminal completion before the next native operation. Each mutation take restores the isolated checkpoint, note baseline, full inventory, money and health and checks the14 protected files. The integrity publisher also hashes original/isolated executables and Steam DLLs. No original Steam asset or save modifications are required.

## Decompilation record and next family

Ghidra remains the persistent exact-executable analysis project; [inventory capacity809 transcript](../../work/natural-moveset/native-pickup-research-v785/ghidra-inventory-clamp-v809.json) and [setter814 transcript](../../work/natural-moveset/native-pickup-research-v785/ghidra-inventory-setter-v814.json) preserve the targeted analysis. The project bookmark at`140618de0` was saved after this continuation. Tool setup and pinned Ghidra integration remain in the existing handoff tool documentation; no new external tool or skill was installed.

- `14063c9f0`: native stack add clamps against entry capacity and returns accepted delta; analyzed machine code and saved function name `D2_InventoryEntry_AddClamped`.
- `140615d90`: capacity/full check. Type6 also considers health/mana, so food cannot be validated as a stored-potion quantity alone.
- `1406156e0`: native inventory add, including type4 modifier logic and type6 immediate restorative behavior. Retain native semantics.
- `1406170b0`: downstream quantity/category notification. Native API calls preserve this behavior.
- `140618de0`: locates the entry through`140618af0`, saves old quantity, calls`14063cc00`, then`1406170b0`. Used only by bounded test setup/restoration here.

Next work is a reusable immediate-restoration outcome adapter for the27 food objects, including full health/mana cases; retain ordinary input gaps as explicit coverage work. Containers and quest/scripted items require their own progression semantics. Then continue shared NPC/world authority, remaining movement/VFX, native session UI and separate-account Steam testing. Do not resume one custom implementation per authored consumable or animator polish as a prerequisite for this functional pass.

## 819 follow-on food analysis, static only

The [saved819 decompilation transcript](../../work/natural-moveset/native-pickup-research-v785/ghidra-food-restorative-v819.json) traces the food branch without modifying the live814 candidate. Native type6 health amount is item+0x78 (signed int) plus the inventory food modifier at+0x174; mana amount is item+0x7c. Positive health routes through`14071a140`→`140719fe0`; negative amounts enter native damage construction and must not be treated as ordinary healing.

For players, positive healing queues a target at health+0xf4, based on max(current+0x20, previous target)+amount and capped to native maximum. Owner+0xd1 bit8 suppresses it. `1407467d0` queues player mana at mana+0x2c; mana maximum is declaration+0x68 plus the bounded attribute at component+0x48/index+0x68. Native options may convert overflow between health and mana. `140745f60` may report maximum instead of raw current when the global at`14398dbd8` is zero. These findings are saved as Ghidra research bookmarks, not new runtime certification.

Consequently, the next adapter must validate owned health/mana components, current values and queued targets; an immediate current-health poll alone can miss the native award. It must retain native overflow/full behavior and avoid classifying an ordinary damage change as a food reward. Food still remains excluded from packet/catalog eligibility in814. No code, native food call or health/mana write was added in819.
