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

| Take | Candidate | Result |
|---|---|---|
| `animquality-dunwall-v2088a` | nativechoice-v2085 | Harness fail: the fixture guard flattened the five rest positions into one list and compared x with y (a "spread" of 220 m). The raw rests were five identical positions, so the free floor was still. Harness v2089 keeps each position as one array |
| `animquality-dunwall-v2088b` | nativechoice-v2085 | The same harness fail. The raw rests on the furniture moved 12 cm in y, so the guard would have stopped it once fixed |

## The native sequence for the partner's choice (candidate nativechoice-v2088)

**Research follow-up** (d2-ghidra, confirmed static, with the stall explanation as a hypothesis built on confirmed values):
- **The scene's two nodes:** the choice scene has an Emily-focused and a Corvo-focused node.
- **A native right press** (update 0x140681cf0) clears the input byte (state 37 +0x58), sets the selection to 1, sets the scene variable to 1 (`FUN_140854d50` with key `image+0x398a8a0`) and sends signal `image+0x398a8b0` (`FUN_140854d20`). The scene then turns to Corvo.
- **The confirm:** writing +0x114 is exactly what a box "yes" does. The update then sends variable 2 and signal 8a8, which only the Corvo-focused node handles.
- **Why v2087a stalled:** holding the selection alone left the scene Emily-focused, so the Corvo confirm went unheard.
- **Finding state 37:** `FUN_1408a8ba0(game)`, then component `0x39aed60`, then +0x90, then +0x138, with vtable `image+0x1c3aaa8`.

**The service now:**
1. It waits for the scene's Inputs On (+0x58 == 1).
2. For Corvo, it replays the right press, then waits for Inputs On again.
3. It confirms, no earlier than 1.5 s after the screen appeared.
4. If the focus has not landed after 3 s, it resets and presses again (`retries`).
5. The `SwitchCharacter` backstop stays.

**Open:** a native confirm input arrives about 67 frames in, with no harness press. Hypothesis: a held input from the intro skip.

**Takes:**
- `nativechoice-dunwall-v2088a`: a Corvo partner.
- `v2088b`: an Emily partner.
- `v2088c`: the manual twin.
- Then the movement pair on harness v2089.

| Take | Candidate | Result |
|---|---|---|
| `nativechoice-dunwall-v2088a` | nativechoice-v2088 | **Pass:** a partner picking Corvo, opposite an Emily host. One right press was replayed and the confirm answered (`answered 1`, `held 1`, `retries 0`); in gameplay she is Corvo (`answered_verified`, no backstop) |
| `nativechoice-dunwall-v2088b` | nativechoice-v2088 | **Pass:** a partner picking Emily, opposite a Corvo host. The confirm was answered and she is verified as Emily |
| `nativechoice-dunwall-v2088c` | nativechoice-v2088 | **Twin (`coop_native_choice manual`), red as required:** her choice screen was still unanswered after 120 s |
| `animquality-dunwall-v2089a` | nativechoice-v2088 | Both directions `FROZEN`: a wall stood between the bedroom-trigger spot and the partner's spawn, so neither rig was rendered (the host's frame shows the bedroom only). The guard worked as meant |
| `animquality-dunwall-v2089b` | nativechoice-v2088 | **Fixture twin, red as required:** with the partner back on the furniture, the guard stopped the step (`FIXTURE`, a y spread of 0.14 m) |

**The first mission's character choice is done** (candidate nativechoice-v2088). Both characters are answered natively without input, and the twin is red. It remains to update the release notes (RELEASE-NOTES:183, :233).

**Next movement take: `animquality-dunwall-v2090a`.** The partner stands 2.5 m from the host on the in-view line of v2086a, and the moves are mostly sideways.

## First in-view numbers on free floor (Edge of the World docks)

| Take | Candidate | Result |
|---|---|---|
| `animquality-dunwall-v2090a` | nativechoice-v2088 | **Fixture guard stop (`FIXTURE`, a 0.11 m spread):** the spot 2.5 m from the host was furniture again. Dunwall Tower has no free floor in view of both spawns, so the movement takes moved to the Edge of the World docks |
| `animquality-eotw-v2091a` | nativechoice-v2088 | **Both directions sampled in view** (157 and 180 samples at 21 Hz; 7.8 m and 9.7 m of travel). Jitter, snaps and rubber-banding are all 0. Foot skate failed under the path-length metric: p95 0.13 m and 0.22 m |

**What the samples show** (confirmed runtime, `animquality-eotw-v2091a/step05-animsample-anim.jsonl`):
- **A planted toe wobbles ±1–2 cm around its spot** while the root moves. Example: the left toe from 1375 ms to 1938 ms stays between y 176.952 and 176.980, a net move of 7 mm, while the path-length sum came to 0.13 m. When idle, the toes hold to 1 mm.
- **The wobble is about one frame of root travel** (1.2 m/s × 16 ms). Hypothesis: the probe reads the root and the pose a frame apart.
- **Idle pops:** in about 1 of 10 idle samples, both toes and ankles rise exactly 10 cm, the knees bend 4 cm and the hips stay put, then everything returns on the next sample. The pose is coherent (not a torn read). It is either a one-frame foot-IK dropout or a read between the animation pass and the IK pass. Removing the pops does not lower the skate numbers, so they are a separate item, kept for the native baseline.

**The analyser now measures foot skate as each contact's farthest excursion from where the toe landed** (`d2_anim_quality.py` 2092). Summing the path counted the wobble as skate. A new test (`test_planted_wobble_is_not_skate`) and twin (`skate_path_length`) cover it: 8 tests, 6 twins caught.

| Take, direction | Contacts | Skate p95 | Skate max | Status (proposed limits 0.05 / 0.12 m) |
|---|---|---|---|---|
| v2091a, host observes the partner | 30 | 0.083 m | 0.145 m | FAIL |
| v2091a, partner observes the host | 20 | 0.073 m | 0.074 m | FAIL (p95) |
| v2086a step09 (Dunwall, re-scored) | 42 | 0.041 m | 0.091 m | PASS |

**The contacts over 5 cm are at starts and stops:** the foot slides 7–15 cm into the idle stance while the root is already still, and running contacts drift 6–7 cm. Native locomotion blends in the same way, so the limits need a native baseline before these count as defects.

**Native baseline, first take:**

| Take | Candidate | Result |
|---|---|---|
| `animquality-baseline-eotw-v2093a` | nativechoice-v2088 | Five native civilians within 4–17 m of the docks walkway, sampled in view for 10 s each. **None walked** (travel 0 for four). Idle contacts scored 0 to 0.033 m. The bloodfly-meat worker turned in place (0.2 m of travel) and scored **0.161 m**, so the native game scores high on in-place pivots and settles too |

The partner's high contacts are at starts and stops, where the native worker scores just as high.

**The native walking baseline and the calibration:**

| Take | Candidate | Result |
|---|---|---|
| `animquality-baseline-eotw-v2094a` | nativechoice-v2088 | `docks_docker_10` was sampled three times from 10 m away. It **walked 12.6 m in view once** (step09), over stairs and flat floor. Old detector: 3.08 m. `docks_docker_19` worked in place |

- **What broke the old detector** (confirmed runtime): the docker shuffles with his swinging toe only 2–5 cm up. That toe passed the height and vertical-speed tests and counted as planted while it moved forward.
- **The detector now** (`d2_anim_quality.py` 2097): a low toe moving faster than 0.3 m/s, or faster than half the root's speed, is swinging. The exception is when the other toe is 6 cm or more higher: then the low toe carries the body and any travel is skate. New test `test_low_lift_swing_is_not_skate`, twin `no_speed_gate`: 9 tests, 7 twins caught.
- **Calibrated limits:** p95 at most **0.08 m**, max at most **0.12 m**, from the native walker (p95 0.073 m, max 0.073 m).

| Sample (re-scored) | Contacts | Skate p95 | Skate max | Status |
|---|---|---|---|---|
| Native docker walking 12.6 m (v2094a step09) | 14 | 0.073 m | 0.073 m | PASS |
| Native workers and pivots (v2094a step06, step08; v2093a step08) | 3–7 | 0.011–0.043 m | 0.011–0.043 m | PASS |
| **Partner seen from her own client** (v2091a step09) | 19 | 0.074 m | 0.074 m | **PASS** |
| **Partner seen from the host** (v2091a step05) | 29 | 0.091 m | 0.145 m | **FAIL** |
| Partner, Dunwall (v2086a step09) | 41 | 0.032 m | 0.091 m | PASS |

**Finding (#266):** seen from the host, the partner's stance foot glides 14.5 cm at a walk start from idle. Her root starts moving (3313 → 3360 ms) while her right toe is planted and the left is 12 cm up, and the toe moves with the root. Hypothesis: the root follows the position stream before her gait animation starts. Stop contacts settle 7–8 cm.

**#266, a second reading of the code (confirmed static) and candidate progress-v2114:**
- **The cycle advances by distance.** The host's rig advances its walk cycle by the distance travelled (`MovementClock::advance`: phase += distance / stride), so a walking stance foot does not slide.
- **The blend is short.** The idle-to-walk blend is `moving = smoothstep((speed − 0.025) / 0.30)` on a speed filtered with τ = 0.1 s (`movement_playback.h`). At walking speed it is complete in about 30 to 50 ms, too short for a 14.5 cm slide while the root moved 7 cm.
- **The phase resumes where the last walk ended.** On a start from rest, the cycle picks up there. The pose then jumps in a few frames from the idle feet to that phase's feet, and the analyser reads the jump as a planted toe moving (hypothesis).
- **Candidate progress-v2114.** A start from rest (filtered speed under the blend's 0.025 m/s floor) begins at the clip's phase nearest the idle pose.
  - `MovementBank::startPhase` picks it by the summed joint distance to idle over 60 phases, cached per clip, with a unit test.
  - Lab switch `coop_movement_start_phase on|off`; the movement status gains `startPhase`.
  - Stops are unchanged.
- **Takes.** `Run-V2122.sh` runs the v2091 walkway route in all four pairings, plus the twin `v2122e` with the switch off.

**Result, `Run-V2122.sh` (progress-v2123, which carries the v2114 start phase): not confirmed.** Step 6 has the host sample the partner, and step 11 has the partner sample the host. The route has two parts: walk, start and stop, then a run.

| Take | Pairing (host / partner) | Start phase | Step 6 skate p95 / max | Step 11 skate p95 / max |
|---|---|---|---|---|
| `animquality-eotw-v2122a` | Corvo / Emily | on | not run: the route file was missing (see below) | |
| `animquality-eotw-v2122b` | Emily / Corvo | on | 0.076 / 0.080 m PASS | 0.305 / 0.305 m FAIL |
| `animquality-eotw-v2122c` | Corvo / Corvo | on | 0.107 / 0.184 m FAIL | 0.159 / 0.204 m FAIL |
| `animquality-eotw-v2122d` | Emily / Emily | on | 0.051 / 0.062 m PASS | 0.131 / 0.160 m FAIL |
| `animquality-eotw-v2122e` (twin) | Corvo / Emily | off | 0.039 / 0.054 m PASS | 0.169 / 0.169 m FAIL |

- **The twin passed step 6** with the switch off, and c failed it with the switch on. The take-to-take spread (p95 0.04 to 0.11 m) is larger than any effect of the start phase. The start-phase finding stays a hypothesis.
- **Step 11 failed in all four takes.** Per contact (scratch analysis of the samples), the large drifts are not at walk starts:
  - They fall in steady walking at 1 to 1.4 m/s, at the walk-to-run change, and once with the root at rest (c step 6, 0.107 m).
  - Most last two samples (94 ms). In b, a "planted" toe moved 0.305 m in 94 ms, faster than the root (2.1 m/s at most). That is a swinging foot read as planted, or a root and a pose read at different times.
- **The measurement is the next step (hypothesis).** The probe samples from outside the process at about 21 Hz, with irregular gaps (31, 47 ms), and reads the root and the pose separately. At that rate a contact is two or three samples, so one misread sample decides the p95. Planned: sample the root and the feet in the same engine frame inside the DLL, at the frame rate, then judge #266 again on the same route.
- **Take a** started during the squash merge of PR #262 (`git switch main` removed the branch's route file for about a minute) and failed on the missing file. It is rerun with the next round.

## The version notice (candidates notice-v2092, progress-v2097, progress-v2100)

When the friend's mod is a different version (control hello or lobby tag), each game shows a one-button engine message box (`native_notice.h`). Lab command `coop_notice_test` raises one.

| Take | Candidate | Result |
|---|---|---|
| `notice-eotw-v2092a` | notice-v2092 | Opened in gameplay, the box closed at once, unseen. The check read the appended logs whole, so twin b counted a's lines |
| `notice-eotw-v2097a` | progress-v2097 | **Log pass:** both games logged "notice shown (version mismatch) in logic 2" and "notice answered (result 0)". **Visual fail:** the box opens in the pause menu on both machines (frames `step04-press-box1-host`, `step11-press-box2-vm`) but reads "Character" with a Confirm button |
| `notice-eotw-v2097b` | nativechoice-v2088 (twin, no notice) | **Red as required:** no notice lines in either log after the take began |
| `notice-eotw-v2100a` | progress-v2100 | **Pass, visual acceptance on both machines:** the box reads "Your friend's co-op mod is a different version. Both players need the same version to play together." with Confirm (frames `step04-press-box1-host`, `step11-press-box2-vm`), and both logs show it shown and answered. The harness failed the take on an unrelated end difference: Edge of the World bottle `env_bottle_04_19` is absent on the host and whole on the partner (open issue [#260](https://github.com/adamkozel/Dishonored-2-coop/issues/260), seen since `deadlyshade-eotw-v2074b`) |

**Why the text was wrong (confirmed static):** the box sends its question to the movie as text it resolves itself. A nonzero question id at `+0x14` goes through `FUN_1400288e0` and the native localization lookup `FUN_1401653e0`, so the Iggy translation hook never sees the borrowed `ui_mplus_character_label` key. With id 0, `FUN_140462210` sends the box's own idStr at `+0x18` (built empty by the constructor `0x460e20`) through `FUN_140028870`. Candidate progress-v2100 sets the id to 0, rebuilds that idStr with the notice text, and drops the unreachable translation swap.
