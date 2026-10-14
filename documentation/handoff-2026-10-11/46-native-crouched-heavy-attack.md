# Native crouched heavy attack: next existing-pipeline binding

The next missing context is the charged attack while crouched. Existing standing heavy clips50,51,52 provide anticipation, hold and release. The native source uses the same focus-in scene for the first two phases but a separate crouched release resource, currently absent from the combat reader. Complete crouched counterparts for those three authored body phases are still needed; do not hold first-person arms as rigid body anchors or replay native damage events.

## Exact observations

Both isolated characters receive ordinary RightTrigger1500ms while crouched at the established central-room fixture, `setviewpos -33.5 -153 68.1 0 0`. The observer uses `-30.5 -149 68.1 233.13 0`. No native animation scene is forced. Each source monitor records:

| Phase | Corvo629 | Emily630 |
|---|---|---|
| Focus entry | `sword_attack_focus_in`, `In`,266ms | Same |
| Focus hold | `sword_attack_focus_in`, `Loop`,2000ms | Same |
| Crouched release | `sword_attack_focus_sneak_out`, `Scene_2`,1000ms | `sword_attack_focus_sneak_out`, `Scene_1`,833ms |

All resources use the prefix `midnight/gameplay/player/base/combat/` and suffix `.midnightscene`. The crouched release binds the reciprocal owned right arm and player1, with Sneaking6+Slash39/physical1. The reader validates the focus phases but currently rejects the unmapped release name. These are source observations, not a new remote-body playback proof.

Evidence: [Corvo629 scenes](../../work/natural-moveset/native-heavy-corvo-v629/scenes.ndjson), [source states](../../work/natural-moveset/native-heavy-corvo-v629/host-crouched-heavy-observations.json), [Emily630 scenes](../../work/natural-moveset/native-heavy-emily-v630/combat-scenes.json), [source states](../../work/natural-moveset/native-heavy-emily-v630/client-crouched-heavy-observations.json). Source and observer videos remain beside the receipts.

## Retained test failures and recovery

[Corvo629](../../work/natural-moveset/native-heavy-corvo-v629/result.json) fails an inherited nearby-guard eligibility assertion that does not apply to this empty-area heavy test. It restores both full inventories, player/partner health and all14 protected hashes. [Corrected harness630](../../work/natural-moveset/Observe-NativeHeavy-v630.ps1) retains the partner-filter installation check while removing the irrelevant requirement that guard eligibility counters increase. It does not relax native scene/state ownership or any restoration check.

[Emily630](../../work/natural-moveset/native-heavy-emily-v630/result.json) passes the source-observation checks but fails final cleanup: host's avatar is absent after checkpoint reload. The14 protected hashes still match. Its original result and failure are preserved. The receipt's fixed `scope` sentence describes the intended restoration and must not override its actual failure/check list.

[Recovery631](../../work/movement-integration/Restore-HeavyFixture-v631.ps1) reruns the established native fixture restoration under the shared operation mutex, then independently compares both complete inventories against630, native player/partner health56/40, fresh live partner handles and14 protected hashes. Its [separate receipt](../../work/movement-integration/heavy-fixture-recovery-v631/result.json) is authoritative for whether recovery finished. Do not alter the original630 failure.

## Continue directly

Add a narrowly scoped native reader variant for the exact two release contracts and role-specific timings. Reuse the existing body library and crouch adaptation workflow for full-body charge/hold/release counterparts, preserving bone lengths, helpers, sword attachment, first-person control and network root. Validate both roles through ordinary input, real game recordings and native stance recovery. Defer fine contact/pose polish; retain kick eligibility and left hit-recovery16 as explicit unverified follow-ups. No third-party co-op mod or new authoring tool is needed.

Recovery631 **PASS**: both fresh partners, full inventories, player/partner health56/40 and all14 protected hashes match. [Source audit632](../../work/movement-integration/crouched-heavy-source-v632.json) correlates the release scene IDs to8 Corvo and7 Emily native Slash/Sneaking samples without changing either original failed receipt. Both source videos are indexed as unbound native observations.

## Correction635: variants, not character-specific scenes

Corvo634 selected `Scene_1/833`, contradicting the earlier role-specific inference. The retained [failed634 receipt](../../work/natural-moveset/crouched-heavy-corvo-v634/result.json) records successful inventory/health/protected-file restoration. Its independent raw scene/state trace shows native Sneaking6/Slash39 and two owned player/arm bindings; charge and hold reached the remote body, while the release was rejected by the mistaken role check.

The [exact Void export635 manifest](../../outputs/d2-coop-lab/research/asset-exports/crouched-heavy-v635/manifest.json) confirms shared Corvo/Emily groups and attack variants `out_a` (`Scene_1/833`) and `out_b` (`Scene_2/1000`). The export SHA256 is `070B199FBD070824C7B0A3B7A4DEFA69F9E65F716CBA578D32D7BC6FBFC03B06`. Candidate635 accepts only those exact pairs, retaining owned-player/right-arm identity, crouched native state and scene lifetime checks. The persistent Ghidra bookmark was corrected and saved. No first-person position or event replay is introduced.

[Bank633](../../work/natural-moveset/heavy-bank-manifest-v633.json) reuses existing heavy clips50/51/52 and the original complete-body crouch adaptation/foot IK as clips354/355/356. Prior354 payloads are byte-identical, anatomical translation error is below1e-5 and network roots remain neutral. Both native release variants currently share this body donor, with playback duration taken from the source scene. Separate directional acting, hold breathing, contact polish and moving/cancel contexts remain open. Runtime correction verification636 is pending.


## Corrected runtime verification637

[Receipt637](../../work/movement-integration/crouched-heavy-verification-v637.json) records both-role636 PASS10 on candidate635. Ordinary held input drives native charge entry/hold and owned release18 through Steam, preserving network root, sword and stance recovery. Both inventories, player/partner health and14 protected files are restored. Source/observer recordings are indexed in the dashboard, and both observer sheets were inspected. Prior354 bank payloads remain unchanged.

Stationary crouched charge/hold/release actor presentation only. Moving/cancel/interruption contexts and shared victim outcomes are not covered.

Both636 tests selected Scene_1/833. Scene_2/1000 has native source629, asset and unit-test evidence; corrected remote playback of that alternative was not exercised.

Both native release subscenes use the same existing adapted body donor. Direction-specific acting, static-hold breathing, contact and fine stance-transition polish are deferred.

Both observer sheets were inspected: full body windup/release/recovery visible. Coarse pose transitions remain; no final visual acceptance or complete movement claim.

Corvo634 failed release validation due to incorrect role-specific timing inference. Exact asset635 and corrected636 tests establish scene/duration pairs independent of role. Original failed receipt retained, with restoration verified.
