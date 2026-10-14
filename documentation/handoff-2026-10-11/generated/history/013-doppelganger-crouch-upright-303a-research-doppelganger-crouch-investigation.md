# Historical note: work/doppelganger-crouch-upright-303A-research/doppelganger-crouch-investigation.md

Original: [work/doppelganger-crouch-upright-303A-research/doppelganger-crouch-investigation.md](<../../../../work/doppelganger-crouch-upright-303A-research/doppelganger-crouch-investigation.md>)

SHA-256: `004E2E5AEB3874471CF023E7DB408FD920688468E772007ED405C5B74E6B6753`; 19146 bytes.

````markdown
# Complete crouch investigation

## Upright comparison: current result

303A passes independent Corvo and Emily flat-floor crouch/walk/stand cases. Head lowering is about 0.25m with the pelvis; the head is 0.37m (Corvo) / 0.31m (Emily) higher than the forward-search reference. The spine/head remain within 0.03 radians of native standing. Calibrated leg motion, ground clearance, anatomical bone lengths, sword attachment and standing return pass. Both native pause directions and checkpoint reference invalidation also pass. The independent-case aggregate explicitly links Corvo's clean repeat and Emily's valid case within the earlier partially failed run; it does not relabel failed runs. Evidence: `doppelganger-crouch-upright-303A-cases.json`.

All twelve upright directional cases also pass, with at least thirteen coherent samples per case, 0.81–1.10m root travel, and knees/toes above the floor. Evidence: `doppelganger-crouch-directions-303A.json`, `doppelganger-crouch-guards-303A.json`. Rendered inspection confirms upright torsos and connected bent legs on both roles; the stance is relatively shallow and asymmetrical and is a comparison prototype, not final animation acceptance. A later attempted same-camera A/B was interrupted by uncommanded player movement, so its screenshots are not a controlled angle comparison. Both clones remain at health 40. Upright is enabled in the current isolated session; after restart/reload use `Enable-D2ShadowCrouch.ps1 -Style upright` to recapture, or `-Style native` for the complete forward-search pose. No native game UI was replaced.

Original Steam executable/DLL and the eight protected baseline save/configuration files passed integrity verification on 303A. The current isolated settings remain windowed, side by side and without focus pause. Friendly-fire suppression remains a requested follow-up; no immunity was enabled to manufacture a passing crouch test.

User steering, 2026-10-09: compare bent legs with an upright torso and native standing arms/sword. This explicitly authorizes revisiting a lower-body-driven appearance; the complete native 6FDE alternative is preserved. The new comparison keeps the calibrated native legs (not the rejected absolute FPS resting angles), counter-rotates the first spine joint against pelvis tilt, preserves its authored connector length, and restores the native standing upper-body subtree. Native helper evaluation remains active. Working candidate `303AF7E16B85CF061195163FA557D35FDFEEBEC65CB2C1EF2620D0224BF9B2C1`; runtime and visual comparison pending.

The user also requires friendly fire to be fully disabled in both directions. This is a separate follow-up covering weapons, powers, owned projectiles and presentation events. The existing outgoing clone-event firewall is not complete partner immunity; the incoming/authoritative routes still need investigation and tests. Current crouch work continues first.

First upright trial is preserved as `doppelganger-crouch-upright-303A-first.json`. It showed torso orientation within 0.027 radians of native standing and head height 0.31/0.37 above the native forward-search reference, but failed the test. Corvo's source switched back to crouch and moved during the supposedly standing-return observations, invalidating that fixture; the repeat now rejects unexpected settled-stage stance changes immediately. The old length loop also treated hand attachment locators 10/17 as anatomical bones. Those offsets intentionally differ between native search and standing clips. The upright check now excludes those two locators from anatomical length comparison, retains the same 1mm limit for real bones, and continues separately checking actual sword attachment position/render alignment at 3cm. No failed report is rewritten as a pass.

## Latest status: 6FDE calibrated native pose passes directional checks

Build `6FDE1885980C35A48E3EDB2CDC1FFD905BE95DB501637FB718E3CEDC209B265F` now passes a fresh crouch/walk/stand run for both rigs and twelve flat-floor directional cases (six per character). Directional samples show knees at least 0.53 and toes at least 0.10 above the model origin, angular leg excursions about 0.25 radians, and no observer displacement beyond the 0.02 tolerance. Native bone lengths, full standing return and sword attachments pass in the separate relative-pose report. Both pause directions and checkpoint invalidation pass. These are bounded mechanical checks, not foot-planting, terrain or full-moveset acceptance.

Fresh rendered checks show connected complete bodies on both rigs without the earlier collapsed knees. The native search reference still leans forward and holds the sword low; combat posture and crouched action blending remain unfinished. Source first-person arms remain native. Reports: `doppelganger-crouch-relative-6FDE.json`, `doppelganger-crouch-directions-6FDE.json`, `doppelganger-crouch-guards-6FDE.json`, `doppelganger-firstperson-6FDE.json`, `doppelganger-actions-6FDE.json`.

The fresh 6FDE Steam-action regression now passes all six checks with 70 observations; both players retain health 56 and clones 40. Passive native damage capture recorded no events or rejected observations during that successful test. Its earlier failures remain preserved and were followed by reads showing both clones at zero health. A longer startup delay failed because the clones were unavailable, so that attempt cannot establish a startup defect. The first-person passive repeat also retained clone health 40, with one rejected native damage observation per role. The earlier deaths remain unexplained; do not infer that healthy repeats fix them. Evidence: `doppelganger-steam-actions-6FDE.json`, both `doppelganger-steam-damage-trace-6FDE-*.json`, and `doppelganger-firstperson-damage-observation-6FDE.json`. Older 9165 airborne evidence remains attributed to its own build.

## Historical rejected experiments

The user reported deformation in the displayed whole-body 9165 pose. Its numerical tests are retained as mechanical evidence only; they did not establish acceptable skinning or posture. That experiment is disabled outside explicit trials. Stable 424B is unchanged.

Exact-executable analysis found that the late pose injection runs after native corrective/helper-bone evaluation. The old composition copied helper joints from the captured pose even after changing their driving legs and pelvis. Calling the ordinary forward-kinematics rebuild at RVA `0x9dc40` does not recalculate these correctives. This is a confirmed missing evaluation step; its contribution to the visible defect still needs runtime and visual comparison.

The native finalizer at RVA `0x541110` calls `0x23fca0(model, animation+0x40, animation+0x2b0)` after `0x53bcd0(...,0x40)` and before final phase-3 publication. The helper routine reads the model's helper array at `+0xa0`, dispatches its native solvers, writes local helper joints and rebuilds dirty transforms. Decompilation and the verified call instructions are preserved in `analysis/ghidra/crouch-helpers/` and annotated in the persistent Ghidra project.

Research build `5E41A801077E4206D07029ABFCB1FD5F8EE6AA8A522656DDFC667836807C50E5` adds this call after composing only the owned remote clone. It validates the entry bytes, model helper count and native joint mask. No local first-person bones or animation events are replayed. Its first fixture restoration stalled before any body/helper writes; both processes subsequently resumed and a second fixture restoration passed. That stall is not evidence of a helper-call failure.

**5E41 runtime:** both roles pass the existing mechanical crouch/walk/stand checks; native helper applications match all 1696 Corvo and 1580 Emily body applications. A subsequent visual check still shows compressed, unsuitable poses and a low sword. This is not an accepted crouch. The rendered right knee is below model-space ground throughout coherent static observations: approximately -0.041 Corvo and -0.072 Emily. Left toe height is approximately -0.020/-0.046. The earlier source-direction and hip-height checks did not test ground clearance. Do not treat matching the first-person body pose as a valid third-person pose criterion by itself. Evidence: `research/doppelganger-crouch-5E41.json`; preserved source: `work/doppelganger-crouch-helpers-5E41-research`.

Next experiment samples complete native poses with all retargeted body writes disabled. The isolated pack adds `sword_dodge_left_in` and `sword_dodge_right_in` as entries 12/13. Their inspected native event blocks contain footsteps; attack variants are excluded. The developer command `coop_shadow_pose <idle|dodgeleft|dodgeright|crouchpose> <0..3000 milliseconds>` uses native playback at rate zero with a ten-second lease, existing actor/world/session guards, and network root control. This is an evidence-gathering tool, not a new player UI or crouch feature. The original Steam assets remain untouched.

**Native reference survey 9B9E:** 22 fixed-time samples pass clip identity, network-root and health checks across both rigs. Dodge-in clips maintain high hips (about 0.81–1.05), so they are poor deep-crouch references. The search transition at 600ms gives connected complete-body shapes on both visual checks, with head heights 1.063 Emily / 1.153 Corvo and sword-hand heights 0.633 / 0.607. Both toes remain near 0.11 / 0.13. This is a better whole-rig base than the 1200ms deep kneeling frame, but it is not a finished combat crouch. Report: `research/doppelganger-native-pose-survey.json`; source and assets preserved under `work/doppelganger-native-pose-survey-9B9E`.

**Relative-pose experiment 6FDE:** retains the complete 600ms native skeleton and transfers only changes from a settled source crouch, using a per-incarnation native/source reference pair. This removes the first-person body's absolute resting leg angles and hip depth from the target posture. Stance transitions blend the entire authored rig; helper joints are recalculated before publication. Source calibration requires grounded crouch (native motion 1), a fresh body pose and less than 0.02 movement since sampling started. First attempted build 11CD incorrectly required motion 0 and refused all captures without writing poses; its failure report is retained. Unit checks cover zero-motion preservation of the complete native pose, rejection of upright source calibration, relative stepping, invalid transforms and standing return. Current runtime and visual limits are recorded above.

The first completed 6FDE trial passed Corvo and failed Emily's old element-wise gait-amplitude criterion. Both rigs passed reference height, knee/toe clearance, native bone-length preservation, standing return, root travel and sword attachment. Minimum sampled knees were 0.534/0.570 and toes 0.133/0.104; maximum limb-length error was below 0.000001. The former amplitude metric compared each matrix entry with one first coherent sample, which depends on joint basis and initial gait phase. An independent reanalysis measured maximum relative joint rotations of 0.248/0.152 radians (14.2/8.7 degrees). The new test uses this basis-independent angular excursion with a 0.1-radian minimum and keeps the original component metric for transparency. The original failure remains in `research/doppelganger-crouch-relative-6FDE-component-metric.json`, with separate angular reanalysis evidence. The subsequent fresh run passed on both rigs; its results appear at the top of this note.

The required result is a connected full-body crouch, including pelvis, spine, head, arms and sword posture. The user explicitly rejected a lower-body-only result. Stable root smoothing milestone 424B remains preserved. None of the crouch experiments below establishes a finished player crouch moveset.

## Evidence and rejected approaches

- **570B:** lower-body-only retargeting rejected. It also exposed native foot IK changing the transferred knees after the early pose callback. Preserved in `work/doppelganger-crouch-lowerbody-570B-rejected`.
- **18A4:** all 17 shared body joints and coherent 80 ms body buffering pass both roles' mechanical crouch/walk/stand checks. Applying before final phase-3 cache publication fixes reversed knees. The first-person body has an upright torso and no arms; retaining idle NPC arms gives unsuitable visual posture. Research backup: `work/doppelganger-fullbody-crouch-18A4-research`.
- **DCD0:** native `sword_patrol_distraction_search_undertable` exists for both female and tall skeletons. Playing it on each exact clone changes pelvis, spine and sword arm together. The inspected alias event blocks contain footsteps only. Other inherited event paths are not exhaustively covered.
- **C5B5:** attempting to hold the clip using integer start time 1000 failed. The native duration wrapper stores a float, not milliseconds as an integer. Failed evidence is preserved.
- **888A:** corrected float start time 1.2 seconds with playback rate zero holds the native crouched reference on both rigs. Both tests pass hip, torso/arm response, root ownership, unchanged local health and sword attachment checks. Visual inspection shows a full-body search/kneeling pose, particularly for Emily; it is a reference, not an acceptable finished crouch walk.

## Current moving experiment

CDD8 introduces an explicitly captured native reference tied to the exact clone, skeleton, handle, both sessions and world epochs. The entire native torso/head/arm/hand posture is combined with player-driven pelvis and stepping. Its spine is placed relative to the changed pelvis, preserving the native crouched upper-body orientation. All joints blend with the standing graph during stance transitions. Native frame/root control and the 80 ms presentation timeline remain separate.

This is an opt-in experiment. `coop_shadow_action crouchpose` prepares the bounded native pose; after settling, `coop_shadow_crouch capture` records it. Stop the action before enabling `coop_probe_body on`. The action and body experiments are mutually exclusive until crouched combat has been implemented. `coop_shadow_crouch off` clears the reference and disables the experiment. No first-person pose is written.

**9165 runtime result:** both roles pass the whole-body crouch/walk/stand test. Host-source Corvo has 13 coherent static and 24 walking observations; client-source Emily has 17 static and 16 walking observations. Hip drops are 0.680/0.673; standing return errors are below 0.001; root travel is 1.74/1.68. Spine and right-arm rotation deltas exceed 0.65/0.80 respectively. Native sword attachment errors remain below 0.00001 game units. These measurements establish mechanical continuity, not finished animation quality.

`research/doppelganger-crouch-verification.json` contains the current result. `doppelganger-crouch-guards.json` passes both native pause directions and checkpoint invalidation: a reference from the prior incarnation cannot apply to the replacement clone. The original Steam executable/DLL and eight protected save/config files passed integrity checks on 9165.

Visual inspection of both composed rigs confirms connected crouched torso/arms and forward knees, but the inherited search pose holds the sword too close to the floor, and Emily remains too bent. It stays an opt-in research candidate, **not the default or a completed crouch feature**. Crouched attacks/parries, full directional gait quality, collision capsule agreement, uneven terrain, slide/climb/lean/powers and general campaign use remain open. Research snapshot including all nine installed asset files: `work/doppelganger-crouch-wholebody-9165-research`.

### Regressions and remaining pose work

On 9165, native first-person arm ownership and real local sword swings pass (3 checks, 32 observations); direct clone actions pass (8 cases); the native Steam action stream passes (6 checks, 70 observations); and both roles pass two jump/landing cycles (4 cases). The direct-action test initially encountered overlapping players after the first-person fixture. That failed report is retained in `doppelganger-actions-9165-overlap-fixture.json`. The action test now restores the separated flat-floor fixture before running, without changing its quality thresholds. This does not resolve general partner-collision handling.

The final lab state has both players standing, health 56, clones health 40, fresh native Steam traffic, exact follow, 80 ms interpolation and locomotion enabled. Body-reference and action experiments are off. Stable milestone 424B and original Steam files remain protected.

The previously recorded native reference playback also explains why simply holding another deep-search frame is unlikely to finish the pose: coherent Emily samples around 1.2–5.6 seconds have sword-hand heights 0.22–0.37, and Corvo's deep-search samples are similarly low. These are observed model-space heights, not a visual acceptance metric. The asset reader found native sword dodge and recovery clips for both rigs; they are candidate references only and have not been played or accepted for crouch. Next work should evaluate a coherent native combat-ready upper-body posture within the complete crouch blend, then integrate crouched attacks/parries. Do not return to a lower-body-only solution or resume finger tuning.

### Reading completed animation frames

CDD8 passed Emily but recorded two apparent standing Corvo samples. Ghidra shows that phase 0 resets the shared cache and copies its mappings forward into later phase slots. A snapshot whose bytes are stable can therefore still be taken between evaluation stages. This is a separate condition from a torn memory read.

9165 adds a monotonically changing publication sequence for the exact owned animation component. Phase-0 entry marks it odd; final phase-3 return marks it even. The read-only `--skeleton-final HANDLE PUBLICATION_ADDRESS` mode requires an unchanged nonzero even sequence and the matching animation component across the entire skeleton read. It validates that the counter resides within the loaded lab DLL. Test stability requires both these conditions and the pre-existing byte/owner checks. Hip, knee and attachment quality thresholds were not relaxed. The unbracketed failure and the first bounded-read retry failure are preserved separately.

## Exact executable analysis

Executable SHA256: `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`.

- `0x9cfa0`: native local-joint setter; `0x9dc40`: global pose rebuild.
- `0x540a90`: native phase cache publication. Clone changes happen before phase 3, after native IK.
- `0x53eef0`: AnimBasic playback. Start time is **float seconds**.
- `0x6e4af0`: copies the input start-time bits into the native clip property.
- `0x6e4850`: advances float clip time by float native delta multiplied by playback rate.

Decompilation and annotations are saved in the persistent exact-game Ghidra project and `analysis/ghidra/doppelganger-crouch-playback-timing.json`. The installed isolated action pack has 12 entries and adds each role's native distraction animset. Original Steam assets are not modified.

````
