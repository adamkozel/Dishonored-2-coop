# Bend Time: exact native investigation and next movement binding

This work begins after preserving the v38 compact-release correction. It investigates native Corvo Bend Time phases before binding the authored remote motion. It does not establish shared time dilation, remote effects or completed power coverage.

## Source and evidence

- [Read-only Void export](../../outputs/d2-coop-lab/research/asset-exports/native-bendtime-v1/manifest.json) reads the original installation with the pinned Void library and verifies unchanged original indexes.
- [Scene summary](../../work/natural-moveset/bendtime-scene-summary-v1.json): normal and slide variants have533ms cast-in,300ms loop and433ms cast-out. The entry scene has a native action event at150ms. The normal resource names contain `_test`; the installed component explicitly references them, so the suffix does not mean they should be discarded.
- Inventory item: `models/powers/bendtime/bendtime.inventoryitem`, type9, maximum1. [Live initial inventory](../../work/movement-integration/bendtime-inventory-before-v1.json) shows quantity0 and no instantiated equipment. Attribute items in the same list are not the power itself.
- [Equipment RTTI](../../work/movement-integration/bendtime-equipment-rtti.json): `arkCpntPowerBendtime`, vtable141b9b748. [Declaration RTTI](../../work/movement-integration/bendtime-declaration-rtti.json): `arkDeclCpntPowerBendtime`, vtable141b9b608. These derive from the exact hashed PE, not a different release.

## Confirmed static findings

Fifteen fresh decompilations are saved in [bendtime-research](../../work/movement-integration/bendtime-research). Ghidra bookmark `D2Coop/BendTime-v39` at14061cd60 is saved in the persistent project.

- `14061c3c0` constructs the equipment with the identified vtable and initializes its additional state.
- `14061c980` resolves native owner timer/sound components, registers callbacks and resolves attribute handles. Native inventory acquisition must perform this initialization.
- `14061cd60` toggles the native power. Component+d0 is the active byte, +d1 participates in its time mode, and +c0 stores its owned request in the time manager at game+1f52e8. The active branch cancels the owner timer, removes the time request, clears active bytes and sends native end events. The inactive branch installs the request and timer and sends start events.
- `14061d350` is a registered callback that ends an active power under its guard conditions. Do not treat it as a general remote animation API.
- `14061cbd0` handles an active-power release path and can adjust the time request; it is not merely a visual-effect stop.

The implementation must never invoke those gameplay callbacks on a remote avatar solely to animate casting. A future synchronized time authority decision belongs to the shared-world design, while this movement work must identify correct visual phases and cleanup.

## Guarded source fixture

[v39 archive](../../work/movement-integration/candidate-bendtime-fixture-v39) pins proxy `60B99A0397B6C6E2652A132B239A9D9B6622206C170159F3CCC00CDAC5D3DD6D` and probe `273701A2F1666498FCD6A7E4A058757F05CD8F382B0757544402C30092D2D5A4`.

[Native reader](../../outputs/d2-coop-lab/src/native_player_bendtime_item.h) validates inventory, declaration, component, owner, slot, membership and repeated identity/active reads. [Fixture](../../outputs/d2-coop-lab/src/bendtime_fixture.h) is an explicit20-second isolated Dunwall acquisition lease, using the already verified native quantity and equipment paths. It refuses wrong role, occupied selection, existing ownership and overlapping fixture state. Normal controller input alone may toggle the native power. It waits for native power end before removing equipment; cleanup has a finite deadline and reports failure requiring isolated checkpoint recovery.

[Observation harness](../../work/natural-moveset/Observe-BendTime.ps1) captures actual source POV plus native states/scenes/entities, verifies inventory/health/protected files, and preserves all output. The current code compiles and passes proxy smoke. Per-take runtime results remain necessary; build success is not power integration evidence.

Next: verify acquisition and real cast/end behavior, identify actual bound actors and phase/state combinations, carry only validated source semantics over Steam, sample complete-body authored motion against the native clock, and handle viewer-correct native effects. Standing, crouched, airborne, slide, equipment, insufficient mana, cancellation and pause/load cases must retain separate coverage.

## Native phase observations and v40 body integration

The observations in [native-bendtime-corvo-v39](../../work/natural-moveset/native-bendtime-corvo-v39/result.json), its long-press comparison, and [crouched holstered v40](../../work/natural-moveset/native-bendtime-corvo-crouched-empty-v40/result.json) pass native acquisition and cleanup checks. Both presses toggle the native active flag. The `cast_in_test` scene is 533ms with reciprocal left-arm and player bindings; `cast_out_test` is 433ms with only the left arm. Standing uses Running0, crouching uses Sneaking6; UsePower63 may overlap. These are actual runtime findings. A longer 1200ms press still did not expose the asset's 300ms hold, so semantic variant13 remains refused.

[v40 archived source and binaries](../../work/movement-integration/candidate-bendtime-body-v40) pins proxy `CD3FB5F65FEB36951BFDCE98A7D51BC4F1914E01AA9C31287B4BDEC5BC92F337`, probe `37148C32956582AAEFE6EF1351243C84A878EC3CD8792ED71DBDCB64F056008C`. D2MOVE38 contains238 clips, with the entire235-clip prior payload unchanged. [Bank builder](../../work/natural-moveset/build_bendtime_bank_v40.py) preserves native lengths, corrected helpers, sword fingers/attachment and network root ownership. Emily is an offline rig comparison only for this Corvo power.

The first [paired take](../../work/natural-moveset/bendtime-corvo-body-v40-v1/result.json) fails because its roughly3Hz external observations missed one short start phase. Its actual video shows both gestures, but that does not turn the test into a pass. The revised [v2 take](../../work/natural-moveset/bendtime-corvo-body-v40-v2/result.json) observes all four phases, passes eight checks, restores native equipment and retains both players/avatar health. Both remote body contact sheets were viewed. Recovery and attached sword are visible; precise finger/contact quality, contexts and viewer effects are not fully accepted.

Offline validation passes exact item/scene ownership tests, malformed and incompatible network packets, proxy smoke, both actual D2MOVE38 banks and synthetic bank formats1–38. Hold remains asset-only, not a replicated invented gameplay state.

## Native viewer FX investigation

Read-only Void export [native-bendtime-fx-v1](../../outputs/d2-coop-lab/research/asset-exports/native-bendtime-fx-v1/manifest.json) preserves original index hashes. `bend_time_01.fx` and `bend_time_02.fx` contain screen-space entry and looping particle layers, not ordinary world-space effects to attach visually to a remote hand.

`arkPlayerFXBendtime` vtable is `141c76808`, exact-PE RTTI [record](../../work/movement-integration/bendtime-playerfx-rtti.json). Fresh saved decompilations in [bendtime-research](../../work/movement-integration/bendtime-research) establish: `14081ec80` chooses a native effect mode from player systems and active Bend Time; `14081ee00` creates a managed player-effect request selected from declaration offsets150–170; `14081ee80` removes its owned request. `14081ef10` separately updates the rendering factor using native time dilation and fade-out state. This explains why merely starting the particle declaration does not establish the complete native appearance.

v41 work therefore treats the screen particles as a bounded manual viewer feasibility test only. It must not invoke `14061cd60` on the observer or claim shared world-time authority. Full-screen effect ownership, source state, lease expiry, pause/load/disconnect and overlap with a local power still need independent evidence before network integration.

## v41 contexts and v42 crouched arm correction

[v41 source/binaries](../../work/movement-integration/candidate-bendtime-contexts-v41) pin proxy `459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7` and probe `4F275F0D33134E47D25CEA7A1E6A8D2643AF5314DE2450506BBA35A0BF7A1A57`. D2MOVE39 adds nine context clips, for247 total. The new variants cover standing empty hands and crouched armed/empty hands. Emily's generated files remain offline rig comparisons for this Corvo power.

The armed and empty-handed v41 crouch takes both pass their eight scoped checks, but viewed footage reveals an awkward raised trailing elbow. Applying a crouched local torso offset and a shoulder offset compounded the arm rotation. Those takes remain visible as rejected pose comparisons.

[v42 authoring script](../../work/natural-moveset/build_bendtime_balance_v42.py) keeps the donor's changing actor-space arm rotations relative to the moving crouched shoulder. It preserves native bone lengths, helpers, sword attachment and foot targets; it does not pin hands to first-person positions. The [balanced bank manifest](../../work/natural-moveset/bendtime-balanced-bank-manifest-v42.json) records the exact files and hashes. Corvo's bank SHA256 is `F72E01750AC8EFA2A4F4B37FAD0BBC14BA3C7C35FCE5AC7652C82E8EA95700E3`.

[Armed crouch v42-v3](../../work/natural-moveset/bendtime-corvo-crouched-armed-v42-v3/result.json) passes all eight scoped checks, with four independently observed native scene identities and four remote presentation events. Its viewed temporal frames show a lower trailing elbow, left-arm casting, attached sword and recovery to crouch. This is a limited visual improvement, not full continuous movement acceptance. Earlier v42 takes remain failed: one missed a phase in sparse observations, and one failed to start the helper because PowerShell split an unparenthesized concatenation into separate arguments.

[Bounded presentation recorder](../../work/natural-moveset/Trace-BendTimePresentation.ps1) independently reads existing movement/Steam status files at a requested20ms interval for10.8seconds. It generates no game input and records timestamps; requested interval is not a measured capture rate. [Context harness](../../work/natural-moveset/Test-BendTimeContexts.ps1) still requires four native phases, both semantic variants, four remote phases and an active/inactive power cycle. The quoting fix passed a separate1second recorder preflight.

The initial v41 standing-empty take produced no native casts after consecutive power tests. Mana depletion is a hypothesis; it was not numerically established in that take. Positive tests now reload the preserved Dunwall checkpoint first. No direct mana writes were added.

The corrected [crouched-empty v42](../../work/natural-moveset/bendtime-corvo-crouched-empty-v42/result.json) passes eight scoped checks. [Standing-empty moving v42](../../work/natural-moveset/bendtime-corvo-standing-empty-moving-v42/result.json) passes nine, including changing gait phase during confirmed network travel. Temporal contact sheets for both were viewed. The standing retest establishes that the holstered context works after a checkpoint recovery; it does not retroactively prove why the earlier no-cast test failed.

**Integrity wording correction:** early context result strings say “original executable, Steam API and save hashes unchanged,” but that per-take loop actually checks the14 entries in `protected-save-hashes-before-v4.json`: original settings/saves and isolated checkpoint files. The original executable is checked separately by the handoff integrity report. The context harness wording was corrected; old evidence files are retained unchanged. Do not infer a Steam DLL hash measurement from that old string.

## Viewer policy and additional static findings

[Manual native FX take](../../work/natural-moveset/bendtime-viewer-fx-v41-v1/result.json) passes five checks for fixture refusal, finite ownership, expiry/cancellation, health and protected files. Viewed frames show full-screen radial blur and clearing. Network kind10 is explicitly refused. These viewer-space particles must not be attached to a remote avatar or replayed on the peer merely because a casting animation occurs; the movement contract preserves native viewer-specific effects. Shared time presentation requires an actual shared-world time decision, outside this diagnostic.

The base FX manager at `components/characters/player/base/fx_manager.cpntplayerfxmanager` contains the inline `m_bendtime` declaration. `m_bendTimeScreenFX` is `bend_time_01.fx`; `m_stopTimeScreenFX` is `bend_time_02.fx`. Daud Blink, grapple and slide occupy the other selectors seen in decompilation. These are not standalone `playerfxbendtime` resource entries. [Class-filtered asset export](../../outputs/d2-coop-lab/research/asset-exports/native-playerfx-classes-v42/manifest.json) was obtained by a read-only class query `(?i)bendtime|playerfx`. The asset reader now exposes `-ClassPattern`, in addition to resource-name filtering. All exported indexes retain their hashes.

Native `arkCpntMana` vtable `141c55ed8` was found in a player component listing. Saved decompilations `140746310`, `140746660` and `140745e20` suggest current mana at component+20 and initialization/max-related storage at+2c. This is a static finding pending repeated runtime identity/value validation, not a supported mutation API. Raw addresses captured before checkpoint reset are stale and must not be reused.

## v43 airborne observations and implementation

[Native airborne source take](../../work/natural-moveset/native-bendtime-corvo-air-v43/result.json) passes five checks. Its recorded source frames were viewed. [Reduced exact scene evidence](../../work/movement-integration/bendtime-air-evidence-v43.json) shows533ms `cast_in_test` during Jump2 and Fall3, with and without UsePower63, and reciprocal left-arm67862/player1 bindings. The433ms `cast_out_test` also occurs during Fall3, with only the left arm. No new asset or guessed airborne event is needed. Ghidra bookmark `D2Coop/BendTimeAir-v43` at14061cd60 was saved.

[v43 archived implementation](../../work/movement-integration/candidate-bendtime-air-v43) pins proxy `B4845ABC910D51DB2C1C79F47BA6D39E3A00AC6427EF0E90EC42CA96291B3699` and probe `8A7580023490DE27DDA2131AA2C0AB2032950526EF9E87AB5E3275A5AE5C555E`. It admits only these owned power scenes into the existing Jump/Fall path, transmits the real native jump metadata through Steam, and retains airborne leg presentation under the casting body. It still refuses crouched-air sources, slide resources and unobserved hold. World identity, action protocol and rebuilt proxy smoke tests pass. The paired runtime take is the separate proof of implementation; compilation is not visual acceptance.

[Read-only mana observer](../../work/natural-moveset/Read-NativeMana.ps1) checks the exact isolated process, native component RTTI/RVA, owner, repeated membership and a finite value at+20. Fresh checkpoint value was100; after one activation/end pair it was60. The same value was60 after the previous ground test and100 after reload. This supports interpreting+20 as current mana, while maximum, costs/upgrades and all failure behavior remain separate. No resource values are written.

The [v43 paired air attempt](../../work/natural-moveset/bendtime-corvo-air-v43-v1/result.json) **fails** with `native_state_stale`. Viewed frames show a jump but no casting gesture. The exact native reader had succeeded; `validShadowAction` still only allowed Dark Vision/Windblast to carry air metadata and rejected Bend Time before transmission. Its prior test even asserted the old restriction. This was an implementation omission, not a native-game limitation or sampling failure.

[v44 archive](../../work/movement-integration/candidate-bendtime-air-v44) pins corrected proxy `DB166E88A45DC94FAD0C6CE851B354E33E707B3BA0E524A79D6E52CD720A4024`, with the same `8A758...55E` probe. Wire protocol34 admits Bend Time start/end with validated jump/fall fields; it rejects protocol33 peers, invalid heights, stale fall apex and the unobserved hold. Native identity, expanded protocol and rebuilt proxy smoke tests pass. Bank format remains D2MOVE39/247 clips; no unrelated payload was reauthored.
