# Native traps: existing-pipeline body and held-item presentation

## Scope and current evidence

This continuation follows the user's explicit priority: use the established pipeline, stop third-party co-op investigation, broaden functional animation bindings, and defer fine polish. It reuses existing authored body/attachment clips and the current native Steam semantic channel. No new authoring application or external dependency was introduced.

The [candidate571 source and identity](../../work/movement-integration/candidate-trap-network-v571/identity.json) preserve bank61/347. The previous340 clip payloads are unchanged. Seven appended clips represent idle, equip, holster, ready-in, ready-loop, place and empty-stock. Both gadget types use the same observed native animation scenes; their held visual models remain distinct.

- [Corvo spring-razor runtime572](../../work/natural-moveset/trap-network-corvo-razor-v572/result.json) and [viewed temporal sheet](../../work/natural-moveset/trap-network-corvo-razor-v572/observer-sheet.jpg).
- [Emily stun-mine runtime572](../../work/natural-moveset/trap-network-emily-stun-v572/result.json) and [temporal sheet](../../work/natural-moveset/trap-network-emily-stun-v572/observer-sheet.jpg). Consult the completed result, not file existence, for its verdict.
- [Exact test harness](../../work/natural-moveset/Test-TrapNetwork-v570.ps1); every take retains the script it actually ran.
- [Both-role bank provenance](../../work/natural-moveset/trap-bank-manifest-v569.json).

These are scoped presentation tests. The original player performs the real native placement. Remote animation and held props execute no placement, inventory-consumption, damage or trap events. Shared placed traps, damage outcomes and the complete moveset remain unfinished.

## Exact native findings

Executable SHA-256 remains `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`.

[Corvo native568](../../work/natural-moveset/native-trap-corvo-razor-v568/result.json) and [Emily native568](../../work/natural-moveset/native-trap-emily-stun-v568/result.json) each pass four native acquisition/observation/rollback checks. Ordinary controller placement consumed quantity1 to0. Native equipment identities, scene bindings and state samples are retained alongside each recording.

| Finding | Confirmed value |
|---|---|
| Inventory names | `components/equipment/gadgets/stun_mine.inventoryitem`, `components/equipment/gadgets/springrazor.inventoryitem` |
| Inventory constraints | Type4, maximum3, native left slot1 |
| Placeable component | `arkCpntWeaponPlaceable`, VT RVA `1b54538` |
| Component declaration | VT `1b543f8`, exact `components/characters/player/base/weapons/{stun_mine,springrazor}.cpntweaponplaceable` |
| Equipment entity | `idEntity` VT `1c87d58`, reciprocal parent1 and component membership |
| Ready state | ID61 / VT `1c3db88`, `arkPlayerStatePlaceWeaponReady` |
| Placement state | ID60 / VT `1c7c098`, `arkPlayerStatePlaceWeapon` |
| Ground context | Observed Running0 or Sneaking6; known equip/undraw overlays are separately checked |
| Native held models | `models/weapons/grenade/stun_mine.lwo`, `models/weapons/grenade/spring_razor.lwo` |

Both inventory entries begin with zero quantity and absent equipment in the isolated checkpoint. The bounded fixture uses native quantity callback RVA618de0; existing Ghidra analysis of6170b0 shows the zero-to-positive transition calls619380 to ensure the native equipment entity. Selection uses existing614b20. Teardown unequips and restores zero quantity, then both checkpoints reload. No fabricated placeable component is inserted into the actual player's inventory.

[Inventory/component exports](../../outputs/d2-coop-lab/research/asset-exports/native-traps-v567/manifest.json), [scene exports](../../outputs/d2-coop-lab/research/asset-exports/native-trap-scenes-v567/manifest.json), and [native held-model definitions](../../outputs/d2-coop-lab/research/asset-exports/native-trap-models-v569/manifest.json) retain exact resource hashes and unchanged original Steam index checks.

All paths in the following table share prefix `midnight/gameplay/player/base/gadgets/`:

| Scene | Subscene | Duration | Reciprocal native actors |
|---|---|---:|---|
| `gadget_stunmine_equip.midnightscene` | Scene_1 |500ms| Owned left arm |
| `gadget_stunmine_unequip.midnightscene` | Scene_1 |500ms| Owned left arm |
| `gadget_stunmine_ready.midnightscene` | In |200ms| Owned left arm |
| `gadget_stunmine_ready.midnightscene` | Looping |666ms| Owned left arm |
| `gadget_stunmine_place.midnightscene` | Scene_1 |300ms| Owned left arm, exact selected item, player1 |
| `gadget_stunmine_ooa.midnightscene` | Scene_1 |333ms| Owned left arm, player1 |

The placement resource's236ms fire marker controls cosmetic held-prop disappearance only. It never invokes a native event remotely. Native reload, ready-out and slide resources exist but are not enabled by guessing their runtime bindings.

## Failed570 and clock correction571

[The failed Corvo570 run](../../work/natural-moveset/trap-network-corvo-razor-v570/result.json) demonstrated held-idle attachment but no action-phase binding. Its native source camera also lacked the floor-facing angle used in568, so that take did not exercise placement. Both checkpoints and14 protected files restored successfully. The failure and both videos remain retained and explicitly excluded from completed animation coverage.

The first candidate misinterpreted where `runtime_probe.cpp` starts `clockWords`. **Correct offsets are total at+15c, subscene-local time at+160, and subscene start at+164.** They are not +160/+164/+168. Recorded ready-loop tuples `{217,17,200}` and `{910,44,866}` establish total=local+start, with loop starts200+n*666. Candidate571 reads the correct tuple and adds regression checks using these actual observations. Loop wraps produce a new semantic event; stale samples and same-event clock rewinds remain rejected.

The corrected evidence was saved in Ghidra bookmarks at141b54538 and141c3db88, and the persistent project was saved. [The refreshed analyst-note export](../../work/handoff-project-bookmarks-v571.json) includes194 Note bookmarks, including earlier categories whose names differ from `D2Coop`.

## Implementation and reproduction

### Recovery extension575, verification576

Emily572 did **not** pass the full test: it presented equip, holster, ready and place, but the actual native source then played `gadget_stunmine_reload.midnightscene`, Scene_1,866ms instead of the expected empty333 scene. Native state60 stayed active and the selected gadget's stock had already reached0. The recovery bound the exact left arm, player and gadget. This is an observed alternate recovery branch, not a reason to synthesize an empty gesture from stock alone.

[Candidate575](../../work/movement-integration/candidate-trap-network-v575/identity.json) adds that existing authored reload clip and its wrist track: protocol62, bank62/348, action20, unchanged372-byte packet. Both the prior340 baseline and the prior347 payloads are checked unchanged by [the new builder](../../work/natural-moveset/build_trap_bank_v575.py). [Build575](../../work/movement-integration/Build-Traps-v575.ps1) passed the native ownership, protocol, proxy and actual-bank checks. [Test576](../../work/natural-moveset/Test-TrapNetwork-v576.ps1) requires every recovery branch that the source actually played, including either empty333 or reload866. It does not claim a native phase occurred when it did not. Consult the completed576 receipts for the final verdict;575's initial identity is deliberately marked unverified until a separate runtime receipt exists.

The finding is saved at Ghidra bookmark141c7c098. No new inventory quantity was introduced to force this branch. Source native input remains responsible for the real placement and stock change.

- [Native item reader](../../outputs/d2-coop-lab/src/native_player_placeable.h): exact inventory names, class/owner/declaration, quantity limits, selected-slot/started agreement, generation and repeated identity validation.
- [Native scene reader](../../outputs/d2-coop-lab/src/native_player_trap_scene.h): exact observed resource/subscene/duration, actual selected gadget, reciprocal player/item/arm bindings, corrected local clock, and final ownership recheck.
- [Presentation timeline](../../outputs/d2-coop-lab/src/trap_presentation.h): source-driven phases, loop boundaries and stale-data expiry; no gameplay events.
- [Protocol](../../outputs/d2-coop-lab/src/shadow_action_protocol.h): version61,372 bytes, action20. Appends gadget kind, stock, variant, elapsed and duration. Rejects conflicting held items, unsupported airborne actions, unknown types, impossible stock and older packet versions.
- [Playback](../../outputs/d2-coop-lab/src/movement_playback.h): existing whole-body authored clips, delayed source timeline, existing grounded locomotion/crouch composition. Root position remains network-controlled.
- [Bank builder](../../work/natural-moveset/build_trap_bank_v569.py): appends existing `items-authored-v2/stunmine_{idle,equip,unequip,ready_in,ready_loop,place,ooa}-{corvo,emily}.bin`; verifies donor SHA/layout and old payload equality. Existing authored wrist-relative tracks are reused for both native static models.
- [Prop service](../../outputs/d2-coop-lab/src/trap_prop.h): independently parked static native render entities for the two gadgets, remote `left_wrist_jnt` joint9 attachment, bounded visibility and world/session retirement. No placeable, damage, physics, pickup or inventory component. Static trap identity requires zero animation components; other existing animated prop identities retain their prior constraints.
- [Isolated asset installer](../../outputs/d2-coop-lab/Build-D2TrapPropAssets.ps1) and [receipt](../../outputs/d2-coop-lab/research/trap-prop-presentation-asset-install.json): physical `work/asset-game` only, preserved isolated replacements and verified untouched original Steam indices.
- [Build571](../../work/movement-integration/Build-Traps-v571.ps1): ownership, action/protocol, proxy-load smoke and both actual347-clip banks passed. [Deploy571](../../work/movement-integration/Deploy-Traps-v571.ps1) preserves candidate569 and uses existing restart/checkpoint recovery. Candidate567 and verified Heart564 remain archived.

To reproduce source precisely, use the candidate571 source snapshot. Preparation scripts are historical, single-run transformations with exclusive backup directories; rerunning them blindly is not a supported restoration method. Native tests, builds and deployment remain serialized with `Local\D2CoopLab_NativeOperation_01a11a65`; wait for cleanup before the next native operation.

## Remaining coverage and next work

### Completed scoped verification and unresolved continuity

Candidate575 passed15 checks for Emily in [576](../../work/natural-moveset/trap-network-emily-stun-v576/result.json) and15 for Corvo in [578](../../work/natural-moveset/trap-network-corvo-razor-v578/result.json). [Receipt577](../../work/movement-integration/trap-verification-v577.json) records both exact binaries, the scope and outstanding work. Actual observer contact sheets were viewed for both roles. Fourteen new source/observer videos, including failed attempts, brought the runtime index to984.

**Corvo576 remains a failed test.** Its ready-in phase3 did reach the receiver, but overall body weight briefly faded to about0.48 after a250ms semantic freshness gap. Native movement remained valid; there was no recorded body-clock rejection. [The failed receipt](../../work/natural-moveset/trap-network-corvo-razor-v576/result.json), state trace and video remain retained. An unchanged repeat578 passed. The cause is not established, the intermittent continuity issue is not fixed, and neither freshness guards nor test thresholds were relaxed. Successful repeat coverage does not erase the failure.

The user requested faster broad coverage in the existing pipeline, with polish deferred. Continue using candidate575's preserved source snapshot to reproduce this stage; newer development proceeds from it. The earlier reference to candidate571 above describes that historical clock correction, not the current recovery-capable source.

Fine fingers/grip, exact floor or wall reach, blade clearance and continuous visual acceptance remain polish work. Reload with remaining stock, ready-out cancellation, slide/airborne contexts, upgraded items, both gadget kinds on each character, wider interruptions and multiple-acquired-trap holster ambiguity remain functional coverage gaps. Ambiguous unselected inventories are refused rather than assigned a guessed gadget.

Remote placed-trap persistence, shared trap activation/damage and world authority are separate unfinished co-op work. This phase does not claim them complete. The next broad existing-pipeline area is contextual takedowns, beginning with the already established ordinary-guard rear-choke fixture and authored body library. [Observation harness574](../../work/natural-moveset/Observe-NativeChoke-v574.ps1) is prepared against preserved575; file existence is not evidence that it was executed.
