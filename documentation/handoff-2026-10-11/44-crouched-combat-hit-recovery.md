# Crouched combat and native hit recovery

## Exact native source observations613

Both [Corvo613](../../work/natural-moveset/native-crouch-combat-corvo-v613/result.json) and [Emily613](../../work/natural-moveset/native-crouch-combat-emily-v613/result.json) pass four source-observation/restoration checks on the verified610 build. Ordinary crouched RightTrigger250ms facing the authored Dunwall guard produces real native combat. These receipts prove the source observations and restoration, not a new remote binding.

Corvo runs `sword_slash_sneak_right` and `sword_slash_sneak_left`, both `Scene_1`,933ms, then `sword_sneak_attack_left_hit`, `Scene_2`,666ms. Emily runs `sword_slash_sneak_right`, `Scene_1`,933ms, then `sword_sneak_attack_hit`, `Scene_1`,866ms. All asset names are under `midnight/gameplay/player/base/combat/` with the `.midnightscene` suffix. The active Slash39 state retains the current scene handle at+24 across replacements.

Initial crouched slashes bind three reciprocal actors: owned right arm, player1 and the unique native guard. Hit recovery binds only the owned right arm and player1. Corvo's observed left hit changes from Sneaking6/physical1 to Running0/physical0 while Slash39 stays active. Emily remains Sneaking6/physical1 during the right hit. These native posture changes must not be mistaken for a broken crouch input or imposed first-person animation anchor.

The earlier Emily611 `throwhit/kick/800ms` observation is a different action with duplicate player bindings; it remains outside this change. No general three-binding relaxation is introduced.

## Candidate616 implementation and reuse

The source before editing is preserved in `work/movement-integration/crouch-combat-pre-v616`. Candidate610 and its runtime615 receipt stay available. [Preparation616](../../work/movement-integration/prepare_crouch_combat_v616.py) reuses the build/deploy/test workflow. No new character rig, authoring tool or bank payload is created; bank65/353 is referenced by hash.

[Native combat reader](../../outputs/d2-coop-lab/src/native_player_combat_scene.h) accepts the exact observed crouched target-bearing slash contexts and the two owned hit-recovery resources. Target and arm reciprocity, scene/state identity, duration/subscene, ground stance and repeated lifetime validation remain required. Partner exclusion remains in the stream. Protocol66 adds combat variants15/16 for right/left hit recovery and rejects wrong durations, stale recovery time or an airborne recovery claim. It remains400 bytes.

[Combat presentation](../../outputs/d2-coop-lab/src/combat_presentation.h) continues the matching existing crouched attack clip from its last authored progress into follow-through, using the native hit-recovery clock. A late entry without a prior attack uses the middle of the authored clip as a conservative presentation fallback; that heuristic needs visual validation. It never replays the whole attack from frame0. [Playback](../../outputs/d2-coop-lab/src/movement_playback.h) blends the complete crouched and standing counterparts according to native stance recovery. The native local player retains its own first-person animations, control, hit detection and damage. The remote pose path issues no victim gameplay events.

Synthetic tests cover both native target-bearing crouch directions, both exact recovery resources, ownership/target refusal, source stance transition, wire timing/scope, and monotonic authored continuation. [Harness617](../../work/natural-moveset/Test-CrouchCombat-v617.ps1) is prepared to verify the source phases, native Steam actor playback, follow-through interval, return to movement, independent root and unchanged receiver victim health, with actual source/observer recordings and complete checkpoint restoration.

At the initial616 preparation, build and runtime verification were pending; later results are recorded below. The change does not implement synchronized victims or claim a complete sword moveset. Shared enemy health, reactions, deaths and authority remain unfinished; fine animation contact and polish are deferred under the user's priority.

## Retained617/619 failures and continuation620

Candidate616 passes build, ownership, wire, smoke and actual-bank tests. Corvo617 validates the native target-bearing slash but fails remote playback: its stream retired during capture/camera setup (`scope_or_peer_changed`, root age250ms). Checkpoint inventory/health and all14 protected hashes restore successfully. [Harness619](../../work/natural-moveset/Test-CrouchCombat-v619.ps1) starts a fresh verified stream after camera setup; it does not relax the runtime freshness checks.

Corvo619 then records the crouched strike on the remote actor at full weight, with a visible full-body attack and rise to standing in the observer sheet. Its assertion still fails because no separate hit-recovery resource occurs in this take. The native source instead retains `sword_slash_sneak_right/Scene_1/933` while changing physical stance from1 to0. This exposes another exact reader limit: requiring physical1 for the entire target-bearing crouched resource drops its final standing portion. The actual source recording shows blood at contact but does not establish the causal rule for selecting the separate recovery resource. Treat that selection rule as unknown.

Candidate620 accepts this observed ground stance continuation and blends the existing complete crouched/standing clip counterparts according to native stance. Unknown airborne target-bearing contexts remain refused. It preserves616 and changes no bank payload. [Preparation620](../../work/movement-integration/prepare_crouch_stance_v620.py) generates [harness621](../../work/natural-moveset/Test-CrouchCombat-v621.ps1), which requires the targeted crouched strike, root and stance recovery; if a separate native recovery is observed, its remote playback is also required. `context-coverage.json` records whether that optional branch occurred. Absence of the branch must never be represented as verified branch coverage. No failed receipt is overwritten.

## Verified620/622 scope

[Receipt622](../../work/movement-integration/crouch-stance-verification-v622.json) records Corvo621 and Emily621, each PASS10. The native target-bearing crouched strike drives the existing complete-body clip through Steam, retains independent root movement, returns to native stance locomotion and leaves the receiver enemy health unchanged. Both checkpoints, inventories, player/partner health and all14 protected save/settings hashes restore. The two observer sheets show crouched strikes and return to standing; foreground guards limit contact/grip assessment. Fine polish is deferred, with no visual acceptance.

Neither621 take selects separate recovery15/16. That optional branch is explicitly **not runtime verified**, despite source613 observations and synthetic tests. Corvo621 records one validated same-scene standing sample; Emily621 stands after the owned scene ends. Failed617/619 receipts stay intact. Six source/observer videos are registered by [record622](../../work/movement-integration/record_crouch_stance_v622.py), including retained619 failure.

## Right hit recovery verified on625/628

[Regression receipt628](../../work/movement-integration/kick-regression-verification-v628.json) records Corvo627 PASS10 and Emily627 PASS13. Emily selects native right recovery15 after a crouched strike and proves owned source clock, remote follow-through, bounded authored phase, stance recovery and unchanged receiver victim health. Left recovery16 remains runtime-unverified. Both observer sheets were inspected; fine visual acceptance is still pending. Earlier failed626 also contains this branch and has a separate [read-only audit](../../work/movement-integration/hit-recovery-audit-v628.json), without changing its failed kick assertion.
