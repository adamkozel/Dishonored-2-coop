# Target-bearing sword playback through the existing pipeline

## Finding and priority

The user asks to speed up the established pipeline and defer visual polish. No third-party co-op code or new authoring tool is needed for this correction. Candidate604 remains archived and its353-clip bank is reused unchanged.

The bounded frontal observations607 did **not** trigger a frontal assassination. Both [Corvo](../../work/natural-moveset/native-front-corvo-v607/result.json) and [Emily](../../work/natural-moveset/native-front-emily-v607/result.json) fail that research assertion, while completing checkpoint inventory/health restoration and all14 protected file hashes. Their original failure receipts are preserved. No front-assassination binding or successful front-takedown claim follows from these tests.

They do reveal a normal-combat gap: ordinary RightTrigger250ms facing the guard runs native `sword_slash_right.midnightscene` then `sword_slash_rightb.midnightscene`,1000ms. Corvo uses `Scene_2`, Emily `Scene_1`. Both have physical standing0 and active Running0 plus Slash39, vtable RVA1c85d28. The scenes bind the native player, its owned right arm and the guard. The old reader permits only one or two bindings and therefore rejects these three-actor samples. Corvo has three recorded active samples with `combatSceneValid=false`; Emily also has three. This is a confirmed source-reader limit, independent of animation quality.

## Candidate610

[Reader](../../outputs/d2-coop-lab/src/native_player_combat_scene.h) accepts an optional third target only for the observed right/rightb, Scene_1/2,1000ms,standing context. It still validates the current native state/scene handle, declaration, active phase, player identity, reciprocal arm ownership and scene lifetime. The target must be an ordinary NPC with a reciprocal, unique native spawner child. Identity is read again after the scene checks. Duplicate arms, foreign players, stale targets, unknown target-bearing scenes and concurrent replacement fail closed.

[Stream](../../outputs/d2-coop-lab/src/shadow_stream.h) excludes the co-op partner's actor and handle before producing presentation semantics. No target identity or damage is sent by this change; no native events are replayed. Existing combat variants, source clock, independent network position and complete-body clips are reused. Protocol65 and bank65/353 remain unchanged. Other targeted sword directions/stances are still unverified.

The source files before editing are in `work/movement-integration/combat-target-pre-v610`. [Preparation](../../work/movement-integration/prepare_combat_target_v610.py), [build](../../work/movement-integration/Build-CombatTarget-v610.ps1), [staging](../../work/movement-integration/stage_combat_target_v610.py) and [deployment](../../work/movement-integration/Deploy-CombatTarget-v610.ps1) reuse the established workflow. New candidate archives reference the already preserved banks by path and hash rather than duplicating large identical files.

Native ownership tests cover the new valid scene and independent corruptions. Existing action protocol tests, proxy smoke and actual353-clip bank checks also pass. [Harness611](../../work/natural-moveset/Test-CombatTarget-v611.ps1) is prepared for independent scene validation, remote body playback, root following, standing recovery and unchanged receiver target health. Runtime and visual verification are pending until their result files are recorded.

## Persistent analysis

The confirmed runtime finding is saved as a Ghidra Note at141c85d28, category `D2Coop targeted sword scene ownership`, in the exact Dishonored2 executable project. The note distinguishes ordinary targeted combat from the unsuccessful front-assassination experiment. It makes no new decompilation hypothesis or native function-call claim.

## Runtime attempts and test efficiency

Corvo611 passes all ten numerical/restoration checks. The observer recording has a foreground guard occluding the body; it is not accepted as complete visual evidence. Emily611 fails the expected sword-scene assertion: the source is crouched and instead runs `midnight/gameplay/player/base/combat/throwhit.midnightscene`, subscene `kick`,800ms, with three bindings consisting of the right arm and the player twice. This new native observation is not an implemented kick or a reason to loosen the target validation. The scene has no NPC binding. Full restoration passes.

Emily612 verifies actual standing posture immediately before input, and the recording shows the remote sword action and recovery with an unobstructed torso. The reader validates one owned target scene, and remote combat weight reaches0.999644. The harness nevertheless fails its minimum of two independently sampled owned scenes: its five unused sensor subprocesses plus other reads are too sparse for the short transition in this take. The original failure is retained, and receiver-health verification was not reached. This is not recorded as a passed runtime test.

[Harness614](../../work/natural-moveset/Test-CombatTarget-v614.ps1) removes those unused sensor subprocesses and the redundant per-sample midnight read, while retaining the independent continuous native scene monitor, the same game input and all existing assertions. It samples at a shorter interval. This reduces test overhead rather than changing animation speed or weakening the checks. [Preparation614](../../work/movement-integration/prepare_combat_fast_capture_v614.py) records that change. New runtime results are pending.

## Completed verification615

[Receipt615](../../work/movement-integration/combat-target-verification-v615.json) records ten passed checks for Corvo611 and ten for Emily614 on the same610 DLL. Emily614 now has seven independently validated source snapshots spanning both attack variants. Source and remote clocks select the existing body clips, the remote root remains independent, standing playback returns, and receiver enemy health is unchanged. Both tests restore checkpoint inventories, player/partner health and14 protected hashes. No new bank payload, protocol field or game event replay is introduced.

The inspected Emily614 observer sheet shows the sword swing, torso/arm movement and return to standing. Contact and visual polish are deferred. Corvo611 numerical evidence is retained with its occluded camera limitation. The attempted Corvo614 camera repeat fails before attack because native standing cannot settle at the chosen position near geometry; the observer capture wait also expires during early cleanup. Its checkpoint restoration succeeds. This fixture failure is not used to erase the passed Corvo611 evidence or claim visual coverage.

The dashboard contains1021 game recordings after this update, including the retained unsuccessful source experiments. This count is not a completion percentage. [Native613 observation](../../work/natural-moveset/Observe-NativeCrouchCombat-v613.ps1) continues with crouched/close-range native actions; no new context binding is claimed in advance. Shared NPC reactions, health, deaths and world authority remain unfinished.
