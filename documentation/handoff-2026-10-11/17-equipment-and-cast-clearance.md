# Equipment recordings and crouched cast clearance

## Scope and protected baseline

Continuation on 2026-10-11 uses the archived v48 DLL and the D2MOVE41 v46 banks (251 clips). The native first-person camera, hands and input remain the source of action. Only the remote presentation actor uses the authored bank. The working build is preserved in [candidate-network-failure-fx-v48](../../work/movement-integration/candidate-network-failure-fx-v48).

## Equipment recording failures retained

The current harness is [Test-EquipmentReview-v48.ps1](../../work/natural-moveset/Test-EquipmentReview-v48.ps1). It records native standing and crouched sword draw/holster, plus forward, sideways and backward empty-hand travel, for both roles. Directional sections use explicit fixture position resets; they are not a continuous locomotion-quality acceptance test.

| Take | Result and diagnosis |
|---|---|
| `equipment-review-both-v48` | Seven Corvo checks passed, but a 20-second recorder wait could not finish the requested 48-second movie. The movie finalized during cleanup. Emily was not tested. |
| `equipment-review-both-v48-v2` | Four Corvo checks passed before an unconfirmed command receipt. Both processes remained responsive afterward. GPU contention from concurrent Blender work is a hypothesis, not a proven cause. |
| `equipment-review-both-v48-v3` | Failed at the first sample with `awaiting_peer`. Later inspection showed source native avatar following had stopped in the earlier timeout. The harness had not restored it and checked bank activity without checking mutual stream readiness. |

The revised harness explicitly restores exact native following and locomotion before arming both streams. It requires both tokens to be ready, both follows enabled, the expected bank version active and movement weight above 0.9 before recording. On failure it captures both endpoint statuses before cleanup. This does not relax the stream's identity, freshness or retirement checks.

`equipment-review-both-v48-v4` passes all fifteen checks for both roles. Both actual-game temporal sheets were inspected at 1.6, 2.3, 10.4, 11, 15.8 and 24.1 seconds. Standing and crouched equipment transitions are visible. Fine hand contact is too dark/distant for acceptance. The harness check wording “authored contact” establishes selection/playback of the authored equipment scene, not visually acceptable contact. The index keeps this requirement incomplete.

[Send-D2Command.ps1](../../outputs/d2-coop-lab/Send-D2Command.ps1) now accepts an optional `ReceiptTimeoutMs` bounded from 100 to 9000 milliseconds. Its default remains 2500 milliseconds. Waiting cannot exceed the original packet expiry. The equipment harness requests 8000 milliseconds; it does not resend unconfirmed mutations automatically. The longer wait alone did not solve the v3 stream-start problem.

## Crouched Bend Time sword arm

The original native-clocked crouched cast has a low trailing sword. A conservative offline blade-centreline screen suggested floor proximity, but this is not native collision evidence.

The v49 candidate rotated only the wrist/forearm to increase blade clearance. Both baseline and candidate passed eight scoped native integration checks. Inspected game frames nevertheless showed the candidate blade rising awkwardly behind the shoulder/head. It is rejected for promotion. Its files and actual-game recordings remain available for audit.

The v50 alternative blends the right arm's actor-space orientations toward the native crouched guard, while keeping the animated shoulder/body, native bone lengths, existing grip and attachment. The wrist follows the arm chain; it is not anchored to first-person coordinates. Only bank clips 239 and 245 change; the other 249 payloads remain byte-identical. The maximum wrist displacement from the previous cast is approximately 0.486 m for Corvo and 0.418 m for Emily. This is a material visual change requiring game review, not proof of improvement.

- [v49 rejected source](../../work/natural-moveset/build_crouch_power_clearance_v49.py)
- [v50 guard source](../../work/natural-moveset/build_crouch_power_guard_v50.py)
- [v50 bank manifest](../../work/natural-moveset/crouch-power-guard-bank-manifest-v50.json)
- [matched camera runtime harness](../../work/natural-moveset/Test-BendTimeClearance.ps1)

Both v50 bank files pass the actual native bank parser tests. `bendtime-corvo-crouch-guard-v50` passes eight native checks; `bendtime-corvo-crouch-guard-moving-v50` passes nine including confirmed travelling gait. Both actual-game temporal sheets were inspected. The blade stays below the shoulder in the sampled cast frames, unlike v49. This is the preferred Corvo casting candidate for further work; it is not full visual acceptance or an Emily gameplay binding. The standard v46 banks remain the baseline used by the failure-FX regressions. Emily's adapted comparison clip does not grant Emily the Corvo-only power.
