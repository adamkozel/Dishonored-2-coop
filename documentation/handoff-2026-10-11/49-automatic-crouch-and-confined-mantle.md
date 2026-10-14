# Automatic crouch and confined low mantle

Candidate651 adds functional bindings to the existing pipeline. It preserves candidate635 and all357 bank67 clips; semantic protocol69 and400-byte packets are unchanged. It does not change the local capsule, camera, first-person animations, native gameplay or Steam API. [Exact candidate pins and source snapshot](../../work/movement-integration/candidate-auto-crouch-v651/identity.json), [pre-edit source](../../work/movement-integration/auto-crouch-pre-v651), [patch generator](../../work/movement-integration/prepare_auto_crouch_v651.py), [build checks](../../work/movement-integration/Build-AutoCrouch-v651.ps1), [isolated deployment](../../work/movement-integration/Deploy-AutoCrouch-v651.ps1).

## Native evidence

[Corvo649](../../work/natural-moveset/native-window-corvo-v649/result.json) and [Emily650](../../work/natural-moveset/native-window-emily-v650/result.json) attempted to open and cross the office window. The window stayed closed. Jump plus forward instead caused a low mantle onto the sill and confined crouching. These are useful native observations, not successful window traversal. Their remote streams never became ready, so neither establishes remote presentation. This is distinct from the later candidate651 tests.

Both characters were observed with these exact identities:

| Stage | Native identity | Scene |
| --- | --- | --- |
| Low mantle into confined crouch | Default mode, ID8, VT RVA1c82f68, kind+0xd8=1, next+0xb8=10, physical stance2 | `midnight/gameplay/player/base/mobility/mantle_low_crouch.midnightscene`,500ms; Corvo `Scene_2`, Emily `Scene_1` |
| Confined idle | ID10, VT RVA1c812b8, RTTI `arkPlayerStateAutoCrouch`, physical stance2 | No independent mantle scene after recovery |
| Sword recovery | AutoCrouch10 plus EquipChange53, VT RVA1c79cf8 | `midnight/gameplay/player/base/combat/sword_equip_sneak_fast.midnightscene`,333ms; Corvo `Scene_2`, Emily `Scene_1`; reciprocal right-arm binding |
| Backward exit | Manual Sneaking6, physical stance1 | Native game remains crouched after leaving the obstruction |
| B after backing away | Running0, physical stance0 | Native standing resumes |

The mantle owns the actual player and native arm actors, with both arms present in Emily650. Goal displacement is about0.716m horizontally and0.56m upward. No player state, scene or arm is fabricated to make the reader pass.

## Implementation and verification

`native_player_states.h` recognizes the exact default-mode AutoCrouch10/raw2 state with at most the observed EquipChange53 overlay. `native_player_action.h` classifies its idle and equipment action. `native_player_traversal_scene.h` admits only the observed low mantle into next10 and requires the exact confined-crouch scene,500ms duration, role subscene, geometry, reciprocal actor/arm owners and stable lifetime. High automatic mantle remains rejected. `native_player_equipment_scene.h` allows only the observed333ms fast right-arm draw in this context.

`engine_bridge.cpp` maps the validated native state2 to existing wire presentation stance1. Actual native state IDs and physics stay intact. A low mantle with state2 also needs its owned scene before gaining a presentation stance. The receiver reuses the existing low-crouch mantle variant2 and crouch locomotion. Fast recovery variants5/6 intentionally preserve full-body traversal recovery while following native sword contact; they do not play an unrelated hip-draw clip.

Ownership, wrong class/stance, unknown overlay, foreign arm, wrong duration/asset, high mantle rejection and concurrent scene/next-state replacement tests were added to `world_identity_tests.cpp`. The first build found a test-only component-variable naming error; its log is retained. The corrected build passes native ownership, action protocol, proxy loading and actual unchanged357-clip bank tests.

[Audit656](../../work/movement-integration/auto-crouch-audit-v656.json) passes36 functional checks across Corvo653, Emily654 and Corvo655. Both Steam connections and action streams remain ready; native mantle/recovery are recognized; held automatic crouch retains full body weight and wire stance1; native sword presentation and final standing recover. The final Corvo655 attachment read confirms a started/equipped sword attached to `right_hand_wpn`. Root-error maxima are0.594m,0.540m and0.480m; means are about0.026–0.030m. The1m bound detects wrong/abandoned following, not ideal interpolation quality.

All runs restore both checkpoint inventories, players/partners' health and14 protected save/settings hashes. [Test653](../../work/natural-moveset/Test-AutoCrouch-v653.ps1), [clearer camera654](../../work/natural-moveset/Test-AutoCrouch-v654.ps1), [Corvo655](../../work/natural-moveset/Test-AutoCrouch-v655.ps1) record ordinary native inputs and both actual game views. Use a fresh take name; do not overwrite evidence.

## Retained failures and visual limits

[Run652](../../work/natural-moveset/native-auto-crouch-corvo-v652/result.json) successfully recorded and restored the fixture, but [its binding audit failed](../../work/movement-integration/auto-crouch-audit-v652.json): the stream retired near5.24s after a266ms root freshness gap. Its remote camera was obstructed by furniture.653 removes the expensive broad Midnight watcher, records both streams and adds B after backward exit. No production freshness threshold was relaxed. Later tests pass stream continuity, but that does not prove all hitches have been solved.

The clear [Emily654 sheet](../../work/natural-moveset/native-auto-crouch-emily-v654/observer-sheet.jpg) and [Corvo655 sheet](../../work/natural-moveset/native-auto-crouch-corvo-v655/observer-sheet.jpg) were inspected. Both show entry, whole-body crouch and recovery. Foot/sill height, authored idle leg placement and contact remain visibly imperfect and need animator/retarget polish. Views are backlit and do not validate precise finger grip. The older652 observer sheet is obstructed and rejected for visual coverage.

Brief body-blend dips remain at entry or stepping backward off the sill: recorded whole-run minimum weights are0.0556,0 and0.0573. Held automatic crouch and final standing stay at full weight. These dips are retained evidence, not hidden by the passing scoped checks. Complete visual acceptance, arbitrary-obstacle coverage, AutoCrouch combat/power overlays, high confined mantle and actual open-window crossing are not established. Movement and playable co-op remain incomplete.

## Persistent Ghidra notes

Three bookmarks under `D2Coop Native AutoCrouch` were saved in the persistent exact `Dishonored2.exe` project. [Decompilation records](../../work/movement-integration/auto-crouch-research-v651) contain `140869370`, `140869400`, `140868d70`. VT141c812b8+0x30 points to140869370; +0x38 to140869400; +0x60 to140868d70. Static gates and cleanup-like accesses are confirmed, while semantic callback names remain hypotheses. No direct native callback invocation was used.

Continue functional coverage in the current pipeline. Keep editable clips and recordings for later hired-animator polish, as the user requested. Do not return to external co-op mod research.
