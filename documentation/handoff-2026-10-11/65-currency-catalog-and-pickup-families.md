# Currency catalog and reusable pickup families

Updated 2026-10-13. Continuation of [the first shared coin](64-native-steam-shared-coin.md). Work is grouped by native behavior, not one implementation per authored item. Complete movement and playable co-op remain unfinished.

## Scope and preservation

Exact executable SHA-256: `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`. Original Steam installation, isolated saves and prior candidates remain protected. The experiment runs only in the existing isolated Dunwall pair. Original game assets, first-person controls, character banks and native Steam transport are retained.

Verified predecessor: [candidate794](../../work/movement-integration/candidate-coin-sync-v794/identity.json). Catalog implementation: [candidate800](../../work/movement-integration/candidate-coin-sync-v800/identity.json), DLL `3DAF9FB18C51A2235E9012352474ADFEA9C1B6D3677F2B70E9B1D5E36612EB67`. Candidate804 adds a bounded developer batch exercise; see its archive identity for the exact binary and validation status. A successful build alone is not runtime verification.

## One catalog, multiple native currency declarations

The initial [read-only799 scan](../../work/natural-moveset/native-coin-catalog-v799/audit.json) filtered entity names containing `coin`, finding 11 objects worth13 in total. The implementation instead discovers the previously verified native component and declaration types. Both games then agree on **19 eligible currency pickups**, worth258 altogether:

| Native item declaration | Count | Value each |
|---|---:|---:|
| `coins/coin1_lowvalue.inventoryitem` | 9 | 1 |
| `coins/coin2_midvalue.inventoryitem` | 2 | 2 |
| `other/bathingsalts.inventoryitem` | 3 | 15 |
| `coins/pouch_small.inventoryitem` | 3 | 10 |
| `coins/pouch.inventoryitem` | 1 | 20 |
| `coins/goldingot.inventoryitem` | 1 | 150 |

Paths are relative to `models/interactive/valuables/`. This is a binding result, not proof that every object is reachable through ordinary input or that containers are shared.

Sources: [native reader](../../outputs/d2-coop-lab/src/native_coin.h), [catalog selection](../../outputs/d2-coop-lab/src/coin_catalog.h), [wire protocol and receipts](../../outputs/d2-coop-lab/src/coin_protocol.h), [coordinator](../../outputs/d2-coop-lab/src/coin_sync.h), [native application](../../outputs/d2-coop-lab/src/coin_native_api.h).

- The exact `arkPickup` / `arkCpntPickable_Simple` ownership, generation, lifecycle, ordinary non-carried declaration and item type1 checks are retained. Special declarations and unsupported kinds are excluded.
- Up to64 entries are sorted by authored name. Duplicate names refuse the binding. Invalid or retired/claimed entries are excluded. No local pointer or handle is transmitted as identity.
- FNV-1a64 covers each sorted name, item path and little-endian quantity. Both peers must match count/digest and each entry before application. This is a consistency check, not cryptographic authentication.
- Coin wire version2 is496 bytes and includes catalog count/digest. Values1–1000 are bounded; money is checked within0–1,000,000. Version1 is rejected. Native transport still uses reliable Steam messages.
- Every entry has separate arms, sequence tracking, source receipt, host revision and acknowledgement. Different entries add independent deltas; repeated packets cannot reuse a receipt. The original test name only selects flat compatibility diagnostics, never eligibility or gameplay behavior.
- Native worker callbacks copy an immutable lease under a short lock and queue a bounded observation. The queue is128 entries; overflow disables the session. Native reward calls, network I/O and file I/O do not run under the lease lock.
- A recursive per-thread native reward guard serializes source and recipient wallet operations. Recipient application tries the guard and defers instead of blocking. The engine drains queued source observations under that guard immediately before application, preventing a completed ordinary pickup from being mistaken for a missing recipient.
- Pauses retain receipts. Coordinated checkpoint recovery retires all native bindings and rebuilds the catalog with new world epochs/arms. Previous-world packets cannot apply to the new catalog.

Existing world identity, action, checkpoint, resume, proxy smoke and actual357-clip bank checks pass for800 and804. New tests cover catalog ordering, pointer independence, quantity/item differences, duplicate names, collected/retiring entries, size bounds, mixed-value receipts and wire rejection. No new animation acceptance is implied.

## Runtime evidence and unsuccessful approaches

- [801](../../work/natural-moveset/shared-coin-catalog-v801/result.json): stopped before source input because the harness expected the name-filtered11 count; both native catalogs actually matched19. Corrected the expectation, not the implementation. Cleanup passed.
- [801b](../../work/natural-moveset/shared-coin-catalog-v801b/result.json): original ordinary coin pickup passed with the full catalog. The next camera approach reached a closed cupboard, and the finite NPC fixture had released on native state change before the next source input. Stopped and restored the baseline.
- [801c](../../work/natural-moveset/shared-currency-catalog-v801c/result.json) and [801d](../../work/natural-moveset/shared-currency-catalog-v801d/result.json): the ordinary coin still converged, but the pouch was behind room geometry from the chosen camera approaches. No pouch reward was observed and neither channel reported an application failure. These are unsuccessful ordinary mixed-value pickup tests, not successful concurrency evidence. Both restored complete inventory/currency/health and protected files.
- [802](../../work/natural-moveset/shared-currency-recovery-v802/result.json): full catalog active; ordinary Corvo pickup, duplicate replay, exactly one coordinated fixed-SLOT82 load per game, automatic fresh catalog binding, three old-world packet rejections, then a fresh ordinary Emily pickup without rearming the sessions. Each new-world wallet increases once; original roles and baseline are restored. This verifies lifecycle for the catalog with a representative ordinary coin, not every entry's reward.

The data-driven [ordinary-input harness](../../work/natural-moveset/Test-CoinCatalog-v801.ps1) varies targets as data. The [recovery harness](../../work/natural-moveset/Test-CurrencyCatalogRecovery-v802.ps1) preserves the earlier ordinary-input regression. Failed recordings remain evidence; do not relabel them as successful mixed-value tests.

## Bounded native batch exercise

Candidate804 provides `coop_coin_sync exercise <authored-name>` only for a ready current catalog, a finite active movement/NPC fixture, idle native player and inactive animation experiments. It consumes one request, verifies the member and uses the same native claim/reward/event/result-destruction/removal API. The source deliberately traverses the installed reward observer; the normal network recipient continues to bypass it to prevent an echo.

Each entry retains a separate exercise receipt before mutation. Duplicate commands cannot repeat the native sequence. Status reports `exerciseCalls` separately from recipient `nativeCalls`; this prevents confusing a developer-triggered native award with normal player input. The [batch harness805](../../work/natural-moveset/Test-CurrencyBatch-v805.ps1) enumerates the catalog and alternates roles, then verifies all per-entry receipts, wallet sums, actor removal, repeated exercise requests, replayed Steam packets and complete cleanup.

## Completed validation805–808

Live candidate804 DLL: `21C2413FFFD447D2CE6395712AD7571980AD9E64A25CC03A9658634A8CE08688`. Probe: `1ECB428D77B93915EA0417F3531E9103BB8E24DEC2336E874A809BA827810B22`. Current sources match its preserved archive.

- [Batch805](../../work/natural-moveset/shared-currency-batch-v805/result.json): all19 entries pass, alternating source roles. Every entry produces exactly its declared currency delta, one source observation, five source exercise calls, five recipient calls and registry removal in each game. Both wallets go0→258. Repeating all19 exercise requests and three old Steam actions makes no additional native application or credit. This is a native API exercise, not ordinary player reachability coverage.
- [Simultaneous806](../../work/natural-moveset/shared-currency-simultaneous-v806/result.json): ordinary controller input in both games collects the original coin. Both native source hooks observe one award; neither peer needs an additional application. Each wallet reaches1, duplicate packets do nothing and cleanup passes. Following is temporarily disabled to position the two views equally, then restored. This tests the same coin from both players; ordinary simultaneous collection of different values remains unverified.
- [Restoration807](../../work/natural-moveset/currency-catalog-restoration-v807/result.json): both games rebuild all19 original members with the same catalog digest `5fd266cc323a86fa`, empty receipts, zero application counters and money0. All inventory name/quantity pairs match the baseline; Corvo/Emily roles, health56/40, following and released NPC fixture are checked. A developer exercise request on each role outside the finite fixture produces no call or award. All14 protected files match; review dashboard returns HTTP200.
- [Independent audit808](../../work/movement-integration/currency-catalog-audit-v808.json) cross-checks saved evidence, full restoration for every retained failed/successful take, source/archive identity, all38 per-role entry receipts and prior recovery802. The dashboard now indexes1272 actual game videos, including16 new recordings. This count is evidence inventory, not animation completion.

All16 new contact sheets were inspected. Batch805 has a fixed first-person view and native money HUD notifications; it does not show individual distant object removals or a remote pickup animation.802 includes actual native loading screens and fresh pickup HUD.806 shows ordinary source HUD in both views.801b–d show obstructed cupboard/pouch approaches and remain marked unsuccessful. Remote avatars on furniture, first-person holstering and nearby native NPC activity are visible in some clips; no new movement-quality or shared-AI acceptance is claimed. Per-video `*-review-v808.json` records those limits.

## Decompilation: why families differ

Actual Ghidra MCP decompilation on the exact executable is saved in [reward branches803](../../work/natural-moveset/native-pickup-research-v785/ghidra-pickup-families-v803.json) and [inventory/capacity803](../../work/natural-moveset/native-pickup-research-v785/ghidra-inventory-capacity-v803.json).

Confirmed in decompiled control flow:

- `140841030` dispatches by native declaration type, performs a full/eligibility check at `140615d40`, calls the native inventory add function, emits mission statistics, and chooses native HUD/event behavior. Types7/8 also have a conversion branch. These are reasons to preserve native behavior rather than writing item quantities directly.
- `1406156e0` handles types1/15 as currency. Other kinds use inventory entries of size`0x38` at inventory`+0x38`; a quantity change reaches `1406170b0`. Type6 has additional player restorative behavior. Type4 with quantity greater than1 includes modifier/randomization branches. Exact semantic names for all type IDs still require matching declarations and runtime evidence.
- `140615d40` explicitly exempts types selected by mask`0x38022` from its usual entry-capacity lookup. Other types resolve an entry and delegate to `140615d90`. The decompiler's inferred return type for `140614f30` is incomplete; do not adopt its displayed `void` as a verified signature.
- `1406170b0` emits a quantity-change message, performs zero-to-positive equipment setup and positive-to-zero depletion handling, sets an item notification flag for the native local player, and branches into further category-specific work. Calling only a counter setter would omit these effects.
- Initial803 lookup of `14063c9f0` found no defined function; that failure is retained. Follow-up [disassembly/decompilation809](../../work/natural-moveset/native-pickup-research-v785/ghidra-inventory-clamp-v809.json) defines `D2_InventoryEntry_AddClamped` in the persistent project. For bounded nonnegative inputs it writes `min(old + requested, capacity)` using entry quantity`+20` and capacity`+24`, returns the accepted delta, and sets byte`+28` precisely for a zero-to-positive addition. The53-byte function has two native return paths. This is confirmed machine-code behavior; replicated stack runtime coverage is still pending.
- `140615d90` compares entry quantity`+20` against maximum`+24`, with additional type6 current/max restorative checks. Together with the modifier branches above, this confirms that stack receipts must verify accepted outcomes instead of assuming every requested unit was granted. No direct counter-write implementation was added.

Next: reuse the catalog/session/receipt machinery with family-specific outcomes for inventory stacks/ammunition, health/mana items, and keys/notes/quest objects. Distinguish pickup from consumption; capacity, owner-only effects, actual granted quantities and scripts need explicit behavior. Do not expand the currency allowlist to these families merely because they share the outer LootObject helper. Continue shared world/AI, progression, remaining movement/VFX, native session UI and separate-account Steam testing after representative family coverage.

Tools: existing Ghidra MCP, native probe/command/input bridge, FFmpeg/FFprobe, pinned Python and Visual Studio2022 CMake. No new external mod or authoring pipeline, skill, or subagent was introduced.
