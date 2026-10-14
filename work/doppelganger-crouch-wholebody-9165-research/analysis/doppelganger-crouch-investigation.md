# Complete crouch investigation

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
