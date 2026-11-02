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

| Take | Result |
|---|---|
| `animquality-eotw-v2083a` | Setup fail: co-op saves did not switch on (the same flake as bottle-eotw-v2078a). Rerun as v2083b on nativechoice-v2084 |

## The first mission's character choice in our own dialog (candidate nativechoice-v2084)

**Research:** [native-character-choice.md](stage8-step1-notes/native-character-choice.md).
- The native screen is logic state 14 (`arkGameLogicStateCharacterChoice` at `game+0x1f6f50`).
- Writing its selection (+0x48: 0 Emily, 1 Corvo) and its confirmed flag (+0x114) makes the game play the chosen ending, and its Kiscule node calls `SwitchCharacter` itself. The "Play as …?" box never opens (confirmed static).

**The change:**
- `session_product.h`: our Character box asks on the first mission too, where it was skipped before.
- The `campaign` command records the partner's pick (`coopNativeChoicePick`).
- `native_choice_answer.h`: a partner-only service answers logic 14 about 2.5 s after it appears, when no box is open. Back in gameplay it checks the native character, and `SwitchCharacter` is the backstop.
- The host keeps its own native screen.
- Lab switch: `coop_native_choice auto|manual`. Status file: `.native-choice.json`.

**Harness v2084:** it presses nothing on the partner's logic 14 and fails if that screen is still up after 25 s; `-NativeChoiceManual` is the twin. The takes (`Run-NativeChoice-v2084.sh`) are a partner Corvo with an Emily host, a partner Emily with a Corvo host, and the manual twin.
