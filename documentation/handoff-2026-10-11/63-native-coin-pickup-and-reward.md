# Native coin pickup and reward investigation

Updated 2026-10-13. This continues [continuous recovery](62-continuous-checkpoint-death-recovery.md). Shared pickups remain unfinished. Preserve the verified recovery782 archive while testing this narrow native API path.

## Exact environment and evidence

- Steam executable SHA-256: `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`.
- Original installation: `C:/Program Files (x86)/Steam/steamapps/common/Dishonored2`; test executable: `work/asset-game/Dishonored2.exe`. Neither executable nor original Steam DLL is patched by this work.
- Map: `campaign/dunwall/escape/tower/dunwall_escape_tower_p`.
- [Read-only native capture785](../../work/natural-moveset/native-pickup-research-v785/result.json), [script](../../work/natural-moveset/Read-PickupNative-v785.ps1).
- Ghidra MCP was used for actual function lookup, vtable memory reads, cross-references and targeted decompilation of the exact executable. Transcripts: [initial785](../../work/natural-moveset/native-pickup-research-v785/ghidra-pickup-v785.json), [continuation786](../../work/natural-moveset/native-pickup-research-v785/ghidra-pickup-continuation-v786.json), [API signatures and removal787](../../work/natural-moveset/native-pickup-research-v785/ghidra-native-coin-api-v787.json). Bookmarks at `140841030` and `1406156e0` were saved in the persistent Ghidra project.
- Tools: existing read-only `outputs/d2-coop-lab/bin/d2_runtime_probe.exe`, native command/input bridge, FFmpeg `gfxcapture`/FFprobe, pinned Python under `C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe`, Visual Studio2022 BuildTools CMake via existing `run-clean.cjs`. No new tools, skills, external mod or authoring pipeline were introduced.

## Confirmed native identities

The authored target is `pickups_coin_emily_lowvalue_4`, at `[-42.25,-152.69,67.6]`. Local pointers, generations and handles must be rediscovered after every load; they are not network identities.

| Object | Vtable RVA / details |
|---|---|
| `arkPickup` | `1c5e258` |
| `arkCpntLoot` | `1c55bb8` |
| `arkCpntPickable_Simple` | `1c5a3b8`, owner `+10`, generation `+18`, lifecycle flags `+1c` |
| `arkDeclCpntPickable_Simple` | `1c5a278`, native declaration `map_cpntpickable_simple_decl_26` |
| `arkDeclInventoryItem` | `1b9ea48`, reached from pickable declaration `+f8` |
| Item | `models/interactive/valuables/coins/coin1_lowvalue.inventoryitem`, type `+60 = 1`, quantity at pickable declaration `+100 = 1` |
| Player inventory | `arkCpntInventory`, vtable `1b9a878`; money is signed integer `+16c` |
| Player pickup action | default-mode state47, `arkPlayerStateLootObject`, vtable `1c7bcc8` |

The coin does **not** have a normal counted entry in the inventory's 56-byte item array. The note-restoration helper at `140618de0` must not be reused as a currency setter. Pickable weak reference `+40` resolves to the player's Audio component, not Inventory; `+50` is its rigid body, and `+70` resolves to player OutsiderMark in this fixture.

## Decompiled execution path

Addresses below are Ghidra addresses with image base `140000000`; use runtime module base plus RVA in code.

1. `140840b20`: eligibility of LootObject. Validates the selected component, distance, picked flags and player/equipment conditions.
2. `14083f590`: action entry. Collects eligible target handles into state `+f0`, count at `+f8`; may detach loot from containers; calls `14076e2b0` to begin consumption and sends a player event. It does not itself contain the final coin credit.
3. `14076e2b0`: detaches the pickup, tears down selected interactions, marks component `+d0` bit2 and resumes it; plays its pickup sound through the native Audio reference. This alone is **not a reward**.
4. `140841500`: action update moves the picked object toward the native player presentation, removes completed handles, calls reward `140841030`, emits the native two-argument event using `DAT_1439acd40`, then calls completion `14076ec20`. Action exit `14083fe70` can also finish outstanding ordinary pickups using that reward helper.
5. `140841030`: reward helper reads manager from state `+10`, inventory from manager `+328`, item from pickable declaration `+f8`, quantity from `+100`. For type1 it calls `1406156e0`, then item/mission statistics `14067e200` and native money-HUD helper `1406733c0`.
6. `1406156e0`: types1/15 increment inventory `+16c` by quantity, emit player event `DAT_1439ab8f0` and call statistics `14067aea0`. `140615620` exposes the same currency add/event/stat path without an item declaration.
7. `14076ec20(component,1)`: clears claiming bit2, calls actor virtual `+80`, marks component bit8, and updates owner flags. Virtual `+80` resolves to `1408784b0`, which sets **actor `+d0` bit0** and calls `14087e040` to retire the entity. The actor subsequently leaves the entity registry; old component memory must not authorize another award.

Other functions investigated and excluded: `1407456f0`, `14076ecf0` and `14076ed80` are serialization paths; `140744210` and `14076e670` register managers; `14076e4a0` runs at component startup and accounts for available world loot; `14080cd60` establishes native time-space attachment between entities, not a reward. These exclusions prevent apparently plausible but incomplete replication hooks.

## Ordinary native pickup baseline786

[Harness](../../work/natural-moveset/Test-NativeCoinReward-v786.ps1), [successful take786c](../../work/natural-moveset/native-coin-reward-v786c/result.json).

Each role uses normal controller X from the previously verified crouched vantage (`setviewpos -41.65 -152.9 69.5 180 0`, settle, crouch, then measured native camera aim). The other player is placed clear of the approach. Both Corvo and Emily independently change money `0 -> 1`, retain `1` after another X, and leave the other world's money unchanged. Read-only state watches observe LootObject47 with Sneaking6; the entity is absent from the registry afterward. A diagnostic read of the old component storage showed flags `0x0d` but generation `ffffffff` and stopped lifecycle flags; this is retired storage, not a valid live identity or an authorization to replay the reward.

Repeated X can pick up the nearby whisky glass after the coin is gone. This is visible in Corvo's recording; it is not a second coin reward. Checkpoint cleanup discards the held glass and restores both complete inventory name/quantity lists, money0, health56, counterpart health40 and the coin. Emily's previously recorded Meagan-note quantity1 is restored through the existing bounded helper. All14 protected save/settings hashes were checked. This does not establish arbitrary journal-state restoration.

Retained failed attempts:

- [786](../../work/natural-moveset/native-coin-reward-v786/result.json): standing approach selects the whisky glass in front of the coin; no currency award. The recorder requested20seconds where its supported maximum is15, so both state watches refused. Actual game videos exist and show the wrong interaction. Cleanup passed.
- [786b](../../work/natural-moveset/native-coin-reward-v786b/result.json): close standing approach exceeds the measured aim helper's70-degree limit; abort before pickup, cleanup passed. Crouching resolves the geometry in786c.

The successful and failed recordings were visually inspected. They document actual native interaction and HUD context, not acceptance of a new remote animation.

## Bounded direct native API experiment787/788

Files: [coin reader](../../outputs/d2-coop-lab/src/native_coin.h), [fixture](../../outputs/d2-coop-lab/src/coin_fixture.h), [reader tests](../../outputs/d2-coop-lab/src/native_coin_tests.h), [runtime harness788](../../work/natural-moveset/Test-NativeCoinAPI-v788.ps1).

Command: `coop_coin_fixture collect`. It is a developer experiment restricted to this exact named Dunwall coin, quantity1, known declarations, correct current player/default state, native code signatures, engine thread, ready local/remote gameplay and a finite active NPC test fixture. Existing first-person controls, animation bank and original assets are not replaced. The fixture makes three native calls: claim `76e2b0`, reward `841030`, finish `76ec20`; it does not write currency or picked flags directly. It checks the money delta before finishing and never retries a partial award. A world-epoch receipt also handles duplicate commands after native entity retirement has removed the coin from the registry.

787 safely refused with zero native calls because the initial freshness check used actor `+d7` bit0, already set in fresh native coin captures. Ghidra's actual removal virtual confirmed the correct retirement flag at actor `+d0` bit0.788 corrects this and retains the failed787 archive and recording. Reader tests cover stale handles, owner mismatch, wrong pickup classes, special/carried item exclusions, duplicate component membership, invalid quantities, unknown flags and a generation change during a read. Existing ownership, action, movement-bank, resume, checkpoint and proxy checks pass.

[Successful take788b](../../work/natural-moveset/native-coin-api-v788b/result.json) and [independent audit789](../../work/movement-integration/native-coin-audit-v789.json) pass both roles: three native calls, money0->1, entity retirement, duplicate with no new calls/credit, outside-fixture zero-call refusal, and full baseline restoration. The epoch receipt permits the restored coin after a later checkpoint. Earlier take788 passed Corvo but stopped before Emily application when the peer full-inventory reader refused; that reader also requires a stable sword attachment. The replacement currency observer validates current player/Inventory ownership directly. The exact cause of that transient sword-dependent refusal was not captured, and is not asserted as a game bug. Both successful source recordings and all retained failed recordings were inspected; nine videos are added to the review dashboard.

Live DLL: `62B32D189FA999D62A8C25D393087DED1E752C4E5EB908BBF7EC612963E31132`; probe: `1ECB428D77B93915EA0417F3531E9103BB8E24DEC2336E874A809BA827810B22`. Sources match archived788. No transport packet or Steam pickup relay is added by this candidate.

The [event-wrapper transcript788](../../work/natural-moveset/native-pickup-research-v785/ghidra-pickup-event-v788.json) narrows the missing step: `1408848a0` supplies two native variants to variadic `140884a90`; `140884980` invokes entity virtual `+10` and optional descriptor `+40` scripted dispatch. `140882fc0` releases owned variant data. Thus this is a real native entity/script event, not just UI cleanup; read-only descriptor captures789 in both isolated games identify `pickedUp`, format `bb`, types `bool;bool;`, arguments `_isSuccesful;_fromGrapple;`, argument count2 and a non-null script-dispatch member. [Host descriptor](../../work/natural-moveset/native-pickup-research-v785/pickup-event-descriptor-host-v789.json), [client descriptor](../../work/natural-moveset/native-pickup-research-v785/pickup-event-descriptor-client-v789.json).

## Remaining before shared pickups

This experiment is not yet a Steam pickup channel. In particular, the direct three-call experiment does **not** reproduce the additional two-argument native entity event emitted by LootObject update/exit. The descriptor is now identified as `pickedUp(successful, fromGrapple)`; its behavior and scripted consumers still need runtime coverage before generalizing to quest items or container loot. Currency credit and visual removal alone do not establish progression correctness.

Native application, pickedUp dispatch and cleanup now pass in both roles as recorded in the790 section below. Next: add reliable host ordering keyed by authored identity plus map/session/world epochs, with exactly-once receipts, simultaneous-claim handling, duplicate and delayed-packet tests, and checkpoint retirement. Shared-wallet semantics, initial unequal balances, reconnect/reload persistence, multiple coins, notes, food, ammo, keys, quest objects and containers remain separate coverage. Preserve continuous shared recovery783 as a regression, and continue world/AI, remaining movement/VFX and native UI/Steam-session work.


## Native event completion790

Candidate790 adds `pickedUp(true,false)` through native wrapper `1408848a0` between reward and retirement, then native variant cleanup `140882fc0`, matching LootObject update/exit. Both input variants use24 bytes, type byte `0x69` and integer value at`+8`; the wrapper destroys its input copies. Exact runtime descriptor strings/count and wrapper bytes are checked before any mutation. This produces five direct calls per successful application. Candidate788 remains archived as the verified three-call reward/removal baseline. [Runtime790](../../work/natural-moveset/native-coin-api-v790b/result.json) and [audit791](../../work/movement-integration/native-coin-audit-v791.json) pass both roles: money0->1, five direct native calls including pickedUp and return cleanup, one native removal, duplicate without new calls/credit, and outside-fixture refusal. Both first-person recordings were inspected and show the native coin HUD. Corvo shows removal; Emily points at the ceiling, so her clip does not visually establish removal. Her currency/removal checks are native-reader evidence, and788b remains the earlier Emily visual baseline. Complete inventory quantities, money0, health56/40 and the exact coin restore after cleanup;14 protected files match. This validates return through the native event call for this coin, not all quest-script effects. No new Steam packet is implemented. Current DLL `6FF3C09E64DAADE87C1133294D50773CCF27968C7B9EA6DA7F28DDDE8A461BDE`; prior788 remains preserved.


Initial [take790](../../work/natural-moveset/native-coin-api-v790/result.json) aborted before either application: the outside-fixture guard correctly reported zero native calls, but the harness compared this new process against the previous process's diagnostic file counters. Money and inventories were restored. The harness now permits the fresh zero counter and still checks unchanged money plus `native_scope_required`; follow-up take790b is distinct evidence.


The790b Emily visual limitation predates the award: `client-start.jpg` at0.1seconds already shows the ceiling, and `aim-client.json` ended with native eye`[-40.7375,-149.524,63.2936]` looking upward toward the coin at67.6. The state monitor begins in Running0. The camera helper aimed at the target from the floor below rather than verifying an unobstructed view. This clip therefore cannot establish on-screen coin removal and must not be labeled as such; it does show the native money HUD after the event. The coin API does not require proximity. A future visual retake should check height and line of sight after aiming.
