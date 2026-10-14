# Full-body crouched grenade candidate v62

## Native evidence

[Corvo standing still in crouch](../../work/natural-moveset/grenade-crouch-observe-corvo-v60/result.json) and [Emily with forward input in crouch](../../work/natural-moveset/grenade-crouch-observe-emily-v62/result.json) establish native Sneaking6 (`1c84258`) plus ThrowWeapon62 (`1c7f118`), default mode and crouched physics stance 1. Both source-only videos were inspected and indexed as observations, not remote integration passes.

The scene/binding pattern matches the standing actions:

| Phase | Native asset suffix | Duration | Reciprocal owned bindings |
| --- | --- | --- | --- |
| Cook entry | `gadget_grenade_cooking_in` | 500 ms | Left arm, local player, grenade |
| Cook loop | `gadget_grenade_cooking_idle` | 1333 ms | Left arm, grenade |
| High throw | `gadget_grenade_throw_high` | 400 ms | Left arm, local player, grenade |
| Empty recovery | `gadget_grenade_throw_high_ooa` | 566 ms | Left arm, local player |

These are exact `midnight/gameplay/player/base/gadgets/*.midnightscene`, `Scene_1`, active phase 3. Subsequent low-throw coverage is recorded below; airborne grenade actions still require runtime coverage. A controller command alone is not proof of world travel; the network harness explicitly requires measured remote speed for a moving case.

## Candidate implementation

[Bank generator](../../work/natural-moveset/build_crouched_grenade_bank_v62.py) runs offline under Blender 5.2 LTS, appending nine full-body crouched grenade phases per role. [Manifest](../../work/natural-moveset/crouched-grenade-bank-manifest-v62.json) pins the output D2MOVE44 banks, donor hashes and prior D2MOVE43 banks. Every previous 264-clip payload is unchanged; the candidate has 273 clips per role.

It rebases authored grenade action deltas onto the existing complete Posture-100 stance, including pelvis and spine. It retains native anatomical translations, shares the animated action through shoulders/neck/head, solves the feet to crouched targets, and corrects offline preview helpers. Native first-person crouched sword wrist orientation is only a local reference; there are no fixed camera-space hand positions. Grenade wrist-relative attachment tracks stay unchanged. The authored output contains pose data only, no gameplay events or actor-root writes. Numerical length/foot checks passed, but do not establish visual quality.

[State reader](../../outputs/d2-coop-lab/src/native_player_states.h) recognizes only the exact observed two-state Running0/ThrowWeapon62 or Sneaking6/ThrowWeapon62 combinations with matching physics stance. [Grenade scene reader](../../outputs/d2-coop-lab/src/native_player_grenade_scene.h) still requires exact scene, arm/item/player ownership and repeated identity reads. Protocol38 already transports stance through the source-clock movement channel; no wire format change is necessary. [Playback](../../outputs/d2-coop-lab/src/movement_playback.h) blends the standing/crouched complete gesture using the existing stance transition, then preserves the distance-driven leg/pelvis gait during travel.

[Native identity tests](../../work/movement-integration/crouched-grenade-identity-v62.log), [both actual banks](../../work/movement-integration/crouched-grenade-banks-v62.log) and [DLL smoke checks](../../work/movement-integration/crouched-grenade-smoke-v62.log) pass. Tests cover state order, wrong class, stance mismatch, pause, duplicates, bank layout and malformed data. The complete runtime/probe/source/harness archive is [candidate v62](../../work/movement-integration/candidate-crouched-grenade-v62/identity.json).

DLL SHA256: `CB1B4100C551A14157C3442DD0FAD0C7EADFA7F130768A522DDE8BB88D27C85A`. Probe: `089D415C7909364B3DD886EE3CA438BDACC67D15D90F87443251134129F3974B`.

## Validation and limits

[Live test harness](../../work/natural-moveset/Test-CrouchedGrenadeNetwork-v62.ps1) grants one finite native grenade while standing, selects it through normal holster/draw input, then crouches. It validates the source scene reader, all remote phases while crouched, exact held-prop mechanism/effect lifecycle and duplicate behavior. A later native stand must recover ordinary posture without replacing stream tokens. Both inventories and both checkpoints are restored and fourteen protected hashes checked. Native operations remain serialized through cleanup; no Blender rendering runs during capture.

The original v62 candidate failed before throwing; subsequent v62b runtime results are recorded below. Projectile flight, explosion/world damage, fine grip/foot contact, all interruptions and airborne context remain separate unfinished work. The original Steam assets and saves are untouched.

### Idle held-item correction v62b

The [first Corvo run](../../work/natural-moveset/grenade-crouch-network-corvo-v62/result.json) failed readiness before firing. Both streams were ready and native crouch motion was valid, but `sourceGrenadeHeld` stayed zero: the ordinary held-item fallback in `shadowSourceAction` still required standing Running0. No recording or successful throw is claimed for that run; its finite item fixture was retired and protected hashes checked.

[Candidate v62b](../../work/movement-integration/candidate-crouched-grenade-v62b/identity.json) recognizes exact single-state standing Running0 or crouched Sneaking6 for that fallback. It still requires the native sword state plus reciprocal live grenade inventory/component identity, equipped/started flags and positive quantity. The action scene reader remains separate. New tests reject mismatched stance/class, throwing as idle and nondefault state. [Identity checks](../../work/movement-integration/crouched-grenade-identity-v62b.log) and [DLL smoke checks](../../work/movement-integration/crouched-grenade-smoke-v62b.log) pass. The D2MOVE44 bank is unchanged. Live retry is pending.

[Corvo stationary crouched high throw v62b](../../work/natural-moveset/grenade-crouch-network-corvo-v62b/result.json) passes fifteen checks. The native source and receiver sheets were inspected: complete crouched body gesture, held prop/mechanism/sparks, release disappearance, empty recovery, and standing after native input are visible. Both streams retain tokens throughout, both inventories/checkpoints restore, and protected hashes match. This stationary take requests no travel (`leftY=0`); the older combined check wording must not be interpreted as a moving gait pass. The harness now records movement as a separate conditional check. Fine sword grip, feet and posture quality remain unaccepted. The failed v62 readiness attempt remains preserved.

v62b DLL: `CB1685DD316E9099AD551BF349987ACEADFBD1218EC8F8D89941D440CBEEAC4F`; probe: `CBF8DDA86965437A9245B5288A58ABD76832BBD530D141AF5A3D1D6F39E0D512`. Ghidra bookmark `D2Coop/GrenadeCrouch-v62` at `141c7f118` records the native state and scene evidence, with unverified contexts explicitly identified.

[Emily moving crouched high throw](../../work/natural-moveset/grenade-crouch-moving-emily-v62b/result.json) passes sixteen checks on the same build. Both source/receiver temporal sheets were inspected: travel occurs during cooking (measured remote speed up to 0.415 m/s), the complete body transitions through throw/recovery, the held prop/effect clear, and the native stand restores ordinary posture. Both streams retain tokens; both inventories/checkpoints and protected hashes restore. Fine contact quality remains unaccepted.

[The first Corvo moving low-throw attempt](../../work/natural-moveset/grenade-crouch-low-moving-corvo-v62b/result.json) stops before input because less than three seconds of the twelve-second grenade fixture remain. It is not a low-throw pass. Camera setup took thirteen native-feedback samples to approach the requested pitch. The optional `-Quick` path in [Set-ObserverPitch.ps1](../../work/natural-moveset/Set-ObserverPitch.ps1) uses bounded 100–600 ms controller pulses based on observed native look rates, then repeats the same native-matrix feedback and angle-at-trigger checks. The fixture lease and refusal threshold are unchanged. Retry evidence is tracked separately.

[Corvo moving low throw v62c](../../work/natural-moveset/grenade-crouch-low-moving-corvo-v62c/result.json) passes sixteen checks at measured pitch 34.173 degrees, including movement and the distinct low release/recovery variants 7/8. The camera retained a near-target pitch before this run; that run alone does not establish quick camera convergence from zero. Both source/receiver temporal sheets were inspected and indexed, with fine contact quality still unaccepted.

[Emily stationary low throw v62c](../../work/natural-moveset/grenade-crouch-low-emily-v62c/result.json) passes fifteen checks. Her camera converged from zero to 34.911 degrees in five feedback samples. The crouched body, native source low release, receiver held effect and release disappearance, empty recovery and standing transition were inspected in both recordings. Both streams retained their original tokens throughout; inventory and both checkpoints restored, and all protected save/settings hashes matched. This stationary take establishes no travelling low-throw gait. Native projectile flight and blast remain source-local.

The four passed v62b/v62c takes cover Corvo stationary high/moving low and Emily moving high/stationary low in this Dunwall fixture. They do not establish every speed, direction, interruption or stance transition. Contact-sheet inspection is recorded separately from overall visual acceptance. The unchanged candidate DLL/probe and D2MOVE44 banks remain pinned above.
