# Continuous shared checkpoint and death recovery

This follows [native-menu door recovery](61-door-native-menu-resume.md). The experiment keeps one checkpoint arm per player and one persistent ordinary-door session across repeated reloads and deaths. The preserved baseline is candidate769; candidates779 and782 are separate immutable source/binary archives. First-person behavior, protocol69, bank67/357 clips and native Steam transport remain unchanged.

## Reproduced failures and corrections

[Run778](../../work/natural-moveset/native-continuous-recovery-v778/result.json) completed its first shared reload but failed the harness's retired-native-call assertion. Its independent native reads had observed both doors Moving, while the asynchronous door status still reported the previous call count. The new harness requires the receiver's corresponding native-call count before storing the retirement baseline. Original778 run-script, readings and paired recordings are retained.

Cleanup also exposed one item quantity difference: Emily's initial `models/interactive/notes/note_dw_frommeagan.inventoryitem` was1, while the standard restored fixture had0. Every other inventory name/quantity matched. Attempts to obtain this through Mayhew interaction did not restore it; the longer input picked up the corpse instead. The subsequent isolated checkpoint restore removed that carry state. The origin of the initial extra note, and any relationship to scene skipping, are **not established**.

[Candidate779](../../work/movement-integration/candidate-recovery-v779/identity.json) adds a deliberately narrow developer cleanup command for that exact scalar note. [The helper](../../work/natural-moveset/Restore-LabNoteInventory-v779.ps1) first refuses any difference in other items, validates the isolated process incarnation, and requires the existing finite NPC fixture. It invokes the normal engine inventory quantity API, then independently compares every item name/quantity to the captured baseline. This does not restore arbitrary journal read flags or implement shared pickup progression. The first779 helper attempt used an unsupported fixture duration and was refused before any native inventory call. The corrected60000ms fixture succeeded; [receipt](../../work/natural-moveset/native-continuous-recovery-v778/note-restored-v779b/result.json). Temporary NPC references are released afterward.

The same inspection corrected an old state label: **logic13 is GameOver**, with native vtable RVA `1b6aab8`, Flash and show-function fields. It is not the weapon wheel. Saved potion-wheel66 evidence correlates wheel input with state8, but8 is not added to any retention whitelist without dedicated runtime verification. Candidate779 allows ordinary menu retention only for pause2 and ReadNote4; shared death uses the checkpoint barrier.

[Run780](../../work/natural-moveset/native-continuous-recovery-v780/result.json) passed both consecutive manual reload cycles on779. Corvo's subsequent natural fall showed both native death screens but retired the source door session as `unsupported_native_phase`; the peer retained its session. Loads did not start early and the test stopped before retry. Its sampled status already had committed death and `deathActive`; it did not capture the exact sub-frame retirement. Source inspection identifies an ordering gap: pending native13 is excluded by ordinary-menu policy before `checkpointService` sees committed13 and starts the reliable death exchange. This timing explanation is an inference from the source and failure, rather than a sampled instruction trace.

[Candidate782](../../work/movement-integration/candidate-death-barrier-v782/identity.json) adds [checkpoint_barrier_policy.h](../../outputs/d2-coop-lab/src/checkpoint_barrier_policy.h). An enabled, nonfailed checkpoint session blocks world actions on either peer's pending or committed13, as well as during the established recovery phases. Door bindings are retired immediately; only session identity/intent remains. This does not authorize a load or reuse old component pointers. Disabled/failed checkpoint sessions do not retain intent through this helper. Existing pair identities and connection checks precede the barrier. The ordinary menu whitelist still excludes13.

## Exact native analysis

[Ghidra evidence779](../../work/movement-integration/ghidra-recovery-v779.json) targets the exact executable SHA-256 `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`.

- `140618de0`: finds the declared item in the inventory's56-byte entries, calls `14063cc00`, then dispatches `1406170b0` with old/new quantities. The existing potion fixture already uses this native entry.
- `14063cc00`: clamps the desired quantity to `[0, maximum]`, updates entry+20 and the new-item byte+28. It is a setter, not an additive grant.
- `141b6aab8`: native GameOver vtable used by the independently verified game-over reader. Persistent Ghidra bookmarks record the correction to the old wheel label.

The cleanup command requires the exact declared note name, declaration vtable `1b9ea48`, type12, maximum1, scalar handle `3fff0010`, absent equipment/component, stable current local player inventory and the exact32-byte API prefix. It executes on the native engine thread, reads back immediately, and reports refusal or confirmed result. The quantity restore is an explicit test-only operation.

## Build and continuous runtime course

Build scripts: [779](../../work/movement-integration/Build-Recovery-v779.ps1), [782](../../work/movement-integration/Build-DeathBarrier-v782.ps1). Both pass native ownership/door protocol, action, resume, checkpoint, proxy smoke and actual357-clip bank checks. New checkpoint policy tests cover pending/committed death from both sides, ordinary states, disabled sessions and all checkpoint phases. Archives preserve predecessors and refuse duplicate staging.

[Test-ContinuousRecovery-v778.ps1](../../work/natural-moveset/Test-ContinuousRecovery-v778.ps1) accepts a unique take and pinned candidate. Run783 uses candidate782. Its course is:

1. Corvo requests reload while both native doors are Moving; the other player submits a duplicate restart.
2. Emily requests the next reload under the same checkpoint arms and door-session intent.
3. Corvo dies from a real native fall. Both native death screens must become ready; survivor input must not move Emily; a survivor retry and duplicate request must cause one shared load.
4. Emily dies while Corvo is already in the native pause menu, followed by the same checks with roles reversed.

Each cycle requires one actual native load-entry/finish pair, one restored opposite-role avatar, new world epochs and six newly bound door channels, no old action replay, zero movement during the both-ready hold, and a fresh native door interaction after recovery. Native Ramsey decision/scheduler reads must show all test isolation released after the load. A finite NPC fixture is reacquired only between stimuli. This does not establish shared AI authority.

Paired35-second recordings are saved per cycle. Contact sheets establish visible menus/loading/return/door use; ownership, quantities and event counts are separately checked against native observations. The harness now persists each role's death-screen read before asserting, so a later failure retains the evidence.

[Run783](../../work/natural-moveset/native-continuous-recovery-v783/result.json) passes all four cycles and five grouped checks. [Independent audit784](../../work/movement-integration/continuous-recovery-audit-v784.json) rechecks the underlying native observations. Both survivors retain health56 and input displacement0; four native loads per role complete with four fresh catalog renewals and one avatar per role per cycle. Complete inventory names/quantities and player/counterpart health56/40 match the initial baseline after cleanup. All14 protected save/settings hashes remain unchanged.

All eight successful recordings were inspected through contact sheets; the Corvo-death pair also has dense7–11second sheets showing the brief native GameOver UI. Eight earlier recordings from retained778/780 evidence were also reviewed and indexed, for16 additions to the dashboard. The initial broad Corvo-death sheets skip over the brief menu; use the detail sheets or video for that visual claim.

Live DLL SHA-256: `1DD50096D91436684A592C77937CF762DF81B57F08A4636A859BDE73DD65C285`; probe: `1ECB428D77B93915EA0417F3531E9103BB8E24DEC2336E874A809BA827810B22`. All archived source files match the live source tree. Prior769/779 binaries remain preserved.

## Remaining scope

This is one continuous same-map fixed-SLOT82 fixture. General save selection, campaign/map boundaries, disconnect/rejoin, shared pickup and enemy authority, complete movement/VFX coverage, native session UI and separate-account online Steam validation remain. The current recovery recreates the native partner presentation; complete authored movement-bank restoration across arbitrary campaign saves is not proven by this test.
