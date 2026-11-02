# 126. Stage 8: partner quality (step 5, #243)

Step 5 measures partner movement in numbers, gives the first mission's character choice to our own dialog, and polishes the messages and menus. The plan is in [partner-quality-plan.md](stage8-step1-notes/partner-quality-plan.md). Labels: confirmed static, confirmed runtime, hypothesis.

## Movement measured in numbers

**Probe:** `d2_runtime_probe <pid> --anim-sample <handle> <ms>` (candidate animsample-v2082).
- It samples a rig in actor scope about every 45 ms (20 to 22 Hz with a full skeleton read).
- Each sample is one JSON line: time, root position and yaw, and per foot the world points of the toe, ankle (joints 21 and 25), knee and hip.
- A world point is `root[0..2] + sum_c root[3+c*3+r] * globalPose[j][12+c]`.
- The partner's rig is the observer's `probeActorHandle`, the d2coop shadow rig (for example `shadow_emily`). Native NPC skeletons use the same ankle and toe indices (confirmed runtime).

**Analyser:** `tools/diag/d2_anim_quality.py`, with 6 tests and 4 mutation twins (`test_d2_anim_quality.py`, ALL OK).
- Foot skate: a toe's horizontal drift while planted, per contact; p95 and max.
- Jitter: animcheck's reversal rule at 0.036 m.
- Snaps: a root step over 0.6 m.
- Rubber-banding: 2.1 m out and back within 600 ms.
- The limits are proposals, to be calibrated on a native baseline.

**Local lab test:** `work/stage8/Test-PartnerMotion-v2082.ps1`. Seven cases per direction (walk, run, sprint, turn, stop and start, crouch walk, jump); the observer samples the mover's rig.

| Take | Result |
|---|---|
| `partner-motion-v2082a` | Measured, 8 of 14 cases over the proposed foot-skate limit (5 cm p95). Walk drifted 0.13 to 0.14 m per contact; turning on the spot 0.15 m (client) and 0.69 m (host), since the planted toes sweep round as the rig turns; run, sprint and crouch walk 0.06 to 0.09 m. No jitter or rubber-banding anywhere, one snap (client run). Jump and stop-start passed. Lab restored |
| `native-baseline-v2082a` | The 20 sampled native NPCs of the lab fixture all stood still (travel 0), so this is no baseline. The baseline moves to Edge of the World, where keep-awake makes `addermire_roof_03` walk |

**Two-account take:** `animquality-eotw-v2083a` (harness v2083, `animsample` step).
- Native baseline: `addermire_roof_03`.
- The host samples the partner's rig while she walks, runs and strafes.
- The partner samples the host's rig while he does the same.
