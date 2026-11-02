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

| Take | Candidate | Result |
|---|---|---|
| `animquality-eotw-v2083b` | nativechoice-v2084 | Harness fail: the NPC-baseline step has no inputs, and `@($null)` sent one 0 ms input. Harness v2085 skips empty inputs |
| `nativechoice-dunwall-v2084a` | nativechoice-v2084 | **Fail:** the partner was meant to play Corvo and ended as Emily. Her service saw logic 14 but never answered (`answered 0`): the screen left in under 2.5 s. **Cause (hypothesis, consistent with ch108):** the harness's cutscene skip confirms the current selection, which is Emily by default |
| `nativechoice-dunwall-v2084b` | nativechoice-v2084 | "Pass", but not evidence: Emily is the default selection. Logic 14 was seen, not answered |
| `nativechoice-dunwall-v2084c` | nativechoice-v2084 | **Twin (`coop_native_choice manual`), red as required:** the partner was meant to play Corvo and ended as Emily |

**Candidate nativechoice-v2085:**
- From the first frame of logic 14, the selection is held at her pick, so any confirmation, a skip included, takes her character (`held` counter).
- The explicit confirm still follows after 2.5 s.
- The backstop (check the character in gameplay, else `SwitchCharacter`) runs after any screen she saw.

**Harness v2085:**
- No input for an empty `inputs`.
- The co-op saves wait is 180 s, with one more `coop_saves on` at 90 s. animquality-eotw-v2083a and bottle-eotw-v2078a stopped when the host's profile save stalled in `profile_saving`. The host's status file stopped updating; hypothesis: disk contention right after the VM workspace sync.

| Take | Candidate | Result |
|---|---|---|
| `nativechoice-dunwall-v2085a` | nativechoice-v2085 | Setup fail: co-op saves did not switch on, even with the 180 s wait and a retry (issue #263) |
| `nativechoice-dunwall-v2085b` | nativechoice-v2085 | **Pass:** the partner plays Emily, opposite a Corvo host. Her selection was held for 67 frames, and the character was verified in gameplay (`answered_verified`) |
| `nativechoice-dunwall-v2085c` | nativechoice-v2085 | **Twin (`coop_native_choice manual`), red as required:** her choice screen was still unanswered after 25 s |
| `animquality-eotw-v2085a` | nativechoice-v2085 | Harness fail at the partner-observer step: the `animsample` loop variable `$i` overwrote the step counter (fixed in harness v2086). **Finding (confirmed runtime):** a rig far from the observer's camera is not animated. The partner rig 200 m from the host kept its toe at a fixed offset from the root while travelling 13.6 m, and the NPC baseline moved its toe only 3 cm. Neither can be measured |

**Analyser changes:**
- **Foot skate** uses a local floor (the lowest toe within 0.5 s) and only the lower toe.
- **A frozen pose** (the toe's offset from the root never changes while the rig travels) is reported as `FROZEN`, not scored. 7 tests and 5 mutation twins.
- **Re-analysis of v2082a with the new skate:** walk 0.05 to 0.07 m p95 per contact (0.14 m max); turning on the spot 0.15 m.

**Next take: `animquality-dunwall-v2086a`**, in the tower bedroom.
- The two players stand 4.7 m apart, facing each other.
- The partner walks, runs and strafes while the host samples her rig, then the reverse.
- Then `nativechoice-dunwall-v2086a` again, a Corvo partner opposite an Emily host.

| Take | Candidate | Result |
|---|---|---|
| `animquality-dunwall-v2086a` | nativechoice-v2085 | **First in-view numbers on two Steam accounts.** Tower bedroom, players 4.7 m apart. **Host watching the partner** (VM): 208 samples at 19.9 Hz, travel 10.1 m, foot skate p95 0.22 m (max 0.25 m), **3 jitter episodes**. From 1.5 s, the root's y swings 4 to 7 cm in alternating directions every sample. **Partner watching the host:** 244 samples at 21.4 Hz, travel 11.7 m, foot skate p95 0.052 m (max 0.091 m), no jitter, snaps or rubber-banding |
| `nativechoice-dunwall-v2086a` | nativechoice-v2085 | Harness timeout: the Corvo partner's screen was still logic 14 at 25 s. Her VM frame shows the choice scene with both characters, likely the chosen ending. Harness v2087 waits 120 s and keeps her `.native-choice.json` |

**Open, being researched:** the jitter of the partner's rig in the host's view. Alternating ±4-7 cm per sample suggests two positioning paths taking turns (hypothesis). Only the host-watches-partner direction shows it.

| Take | Candidate | Result |
|---|---|---|
| `nativechoice-dunwall-v2087a` | nativechoice-v2085 | **Fail:** a partner picking Corvo, opposite an Emily host. Her selection was held for 67 frames; then something other than our write confirmed it (`answered 0`), and her choice scene stayed in logic 14 for the full 120 s. The Kiscule mirror was not active yet, so host story is not involved |
| `nativechoice-dunwall-v2087b` | nativechoice-v2085 | **Twin, manual:** her screen stayed unanswered for 120 s, red as required |

**Analysis:**
- The Emily pick (v2085b) completed after the same 67 frames. The Corvo pick then stalls.
- Hypothesis: the direct writes skip what the native focus (left/right) and confirm paths also signal to the choice scene. Research into the native confirm sequence is under way.
- Input injection is not an option for the release: the lab's virtual pad (`lab_proxy.cpp`) exists in lab builds only.

## The partner rig jitter was the fixture (research, confirmed runtime)

In v2086a the route placed the partner about 0.8 m up on furniture in the bedroom, wedged in a slot.
- **The source:** her own sent position wobbled 4 to 7 cm in y while idle (`route.json` `localPosition`).
- **The host:** it follows a single interpolated target and never extrapolates (`pose_interpolation.h`), so it showed the wobble faithfully.
- **The gait clock:** it took the wobble for walking (`movement_bank.h:291`), so the legs stepped in place, which gives the foot skate.
- **Controls:** the reverse direction, and the partner on a free roof (eotw-v2085a), were still at rest.

**Harness v2088 adds a fixture guard:** the mover must rest within 5 mm before sampling, or the step stops as `FIXTURE`.

**Take `animquality-dunwall-v2088a`:** both players on free floor, 13 m apart.

**Its twin, `v2088b`:** the partner back on the furniture, where the guard must stop the step.

**Noted for later (a decision, not done):** smoothing the gait clock's velocity vector instead of its magnitude would keep a real player wedged in geometry from stepping in place. It changes every gait start and stop, so it needs the full movement regression.
