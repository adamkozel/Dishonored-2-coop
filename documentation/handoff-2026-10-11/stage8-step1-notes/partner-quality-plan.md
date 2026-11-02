# Partner quality (stage 8 step 5, #243): research and plan

Read-only research, 2026-11-02. Labels: confirmed static, confirmed runtime, hypothesis.

## What measures movement today

- **Pose cadence (root only):**
  - The lab command `coop_pose_trace 3000` (engine_bridge.cpp:2138) writes `.pose-cadence.json`. The analyser is `outputs/d2-coop-lab/Test-D2PoseSmoothing.ps1`: moving Hz, Gap90, travel, render and native target error, final remote error.
  - Its limits: smoothed Hz at least 2× unsmoothed, Gap90 at most 0.65×, target errors at most 0.001, final remote error at most 0.01.
  - Last result, 2026-10-09: smoothed 153-547 Hz, errors 0 (confirmed runtime, old).
- **Observer presentation stream:** `.movement.json` (movement_playback.h:906) carries speed, mixes, the clock trace, `kneeOffsetError`, the foot counters and stale counts. After 1213m, the presented-to-native fraction is 1 % (ch91).
- **Feet:** the ankle joints are 21 and 25. Foot cadence is solved from the stride and has never been measured.
- **Frame pacing:** `Measure-FrameTiming-v1886.ps1` (ch111).
- **The movement requirements:** 103 of 105 items, accepted visually. No item is finished on a numeric test alone, so numbers come in addition to visual acceptance.
- **coop-anything animcheck:** in `work/external-references/coop-anything/tools/animcheck`, not yet vendored.
  - Its defaults assume 70 units per metre; D2 uses metres, so they scale by 1/70.
  - Its "sliding" means moving without a locomotion animation, not foot skate.

## The first mission's character choice

- **Where the partner's choice happens:** the partner's co-op box (`coop_character.h`) is skipped for `dunwall_escape_tower.mapinfo` (`session_product.h:283-284`, `nativeChoice`). She then meets the game's own Kiscule PlayerCharacter node, then logic 14, then "Play as Corvo Attano?" (ch108).
- **The 2026-10-21 rule:** the host picks once per save, which is native already. The partner picks on every join, so the change is partner-only.
- **Change:**
  - Drop the exclusion, so her box decides and the campaign command carries her character.
  - Answer her native PlayerCharacter node or logic 14 automatically with that choice; it needs Ghidra on the node and on `arkGameLogicStateCharacterChoice`.
  - Update the harness to expect no logic 14 on the partner.

## Polish list (known items)

- The version mismatch has no on-screen message (ch118:51).
- The partner's result-screen buttons give no hint while she waits for the host (ch122:62).
- RELEASE-NOTES:183 and :233 change after the dialog fix.
- Hypotheses to check:
  - partner wait states without on-screen text (session_product.h:269-286);
  - host or partner loss without a message;
  - the localisation of the "Co-op" label.

## Plan

1. **Probe:** `coop_anim_probe <ms>` (read-only, 30 Hz) writes `.anim-probe.jsonl` in animcheck's format: root (render, target, remote), yaw, about 14 world-space joints (ankles 21 and 25, knees, hips, hands, head), and the native or authored state, weights, grounded flag and speed.
2. **Analyser:** vendor animcheck under `third_party/coop-anything/` with its notice, add a D2 profile (1 unit per metre), and our own foot-skate, snap and position-error detectors. Each detector gets synthetic tests and a mutation twin.
3. **Proposed limits**, to calibrate on a clean lab baseline first:
   - foot skate: planted-ankle drift p95 at most 5 cm, and mean planted speed at most 3 cm/s;
   - jitter: no episodes at 0.036 m;
   - snapping: no `presentedToNativeDrops` outside Blink, teleports and loads, and the native-pose fraction at most 1 %;
   - rubber-banding: no episodes (2.1 m within 600 ms);
   - render against the delayed remote: p95 at most 0.10 m while moving;
   - facing: p95 at most 10°.
4. **Takes:**
   - First, local lab cases: walk, run, sprint, turn, start and stop, crouch walk, jump and land, stairs and slope, mantle.
   - The twin: `coop_probe_smoothing off`.
   - Then VM takes for the four pairings (CC, CE, EC, EE), each judged in both directions.
5. **Dialog:** the session change plus a VM take for each partner character on Dunwall. Then the polish list and the release notes.
