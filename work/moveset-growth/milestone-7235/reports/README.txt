DISHONORED 2: COMPLETE MOVESET EXPANSION, FIRST WALKING BANK

Working candidate: DLL 723540B7, original 092E proof preserved.
The full moveset is NOT finished. COVERAGE.txt / coverage.json track 105 broad
functional work items, plus native-reference-index.json retains 1,942 distinct
aliases (2,221 channel entries) from twelve native base/Corvo/Emily sets. An
arm/camera alias is not a complete-body clip and unused aliases are not proof
of supported gameplay. Conditional actions still need game confirmation.

IMPLEMENTED AND TESTED
- Two exact native skeleton banks: Corvo 215 joints; Emily 219.
- Each bank contains sword-ready idle and eight single-cycle walk directions.
- Original native mesh proportions, anatomical joint translations, role-specific
  closed fingers/socket transforms and native corrective-joint publication.
- D2MOVE01 validates layout, bounded finite rigid poses, identity root/anchor,
  closed cycles and native anatomical lengths before accepting a bank.
- Runtime phase follows distance travelled by the SAME delayed presentation root
  used for native Steam avatar placement. No root animation or generated events.
- Adjacent direction interpolation and idle/walk crossfade; no authored dedicated
  start/stop/pivot motions yet. No gameplay damage, physics or first-person writes.
- A strict movement decoder bug is fixed: known Slash/Block overlays can coexist
  with one known native movement state. Unknown classes/duplicates still fail.
- Existing native action stream must be fresh and idle for walking to apply.
  Slash/Block, crouch, airborne states and unknown actions have native priority.
- The bank is developer-only and opt-in for 60 seconds, with scope/freshness,
  actor generation, world/session, pause and expiry guards. Off by default.

EVIDENCE
Build-D2Lab regression suite passed. d2_movement_bank_tests validates both real
banks, bad layouts, malformed/truncated/root/event data, closed cycles, angle seam,
distance phase and stopping. Both previous authored proof formats still pass.
walk-test-3/result.json: 18 checks, both characters/all eight directions, rest
and cancellation. priority-2/result.json: 13 checks, real standing/crouched
Slash/Block coexistence, native jump/fall priority, pause/rearm refusal and health.
Both players retain health 56, independent avatars 40.

corvo-walking.mp4 and emily-walking.mp4 show equivalent native-controller
forward/left/back/right sequences in D2. The captures are about 12.3/12.5 Hz in
30 fps video containers, with original timing. emily-video-1 was only five frames
and is NOT suitable for judging animation; emily-video-2 replaced that recording.
Recordings and pose captures are evidence of a candidate, not full visual signoff.
Foot planting, moving direction changes, stop poses and grip close-ups still need
polish/coverage. The low-LOD Emily Blender preview lacks normal in-game detail.

GENERATION FINDINGS
Unreal 5.8.3 / Epic AnimGen sample, controller AGC_AnimGenCharacter, seed 1234,
60 fps, 0.25-second pose smoothing; no output animation notifications.
29 original donors plus five NPC-controller comparisons were generated. Walk
cycles are cut between consecutive left contact entries and resampled by travel.
V1 mistakenly allowed double cycles for right/back-right; v2 fixes that. V1 banks
and first test are kept in bank-v1 / walk-test-2 for comparison.

Requests labelled run/sprint are not accepted as those gaits. The engine code
AnimNode_AnimGenController.cpp clamps control vectors to trained min/max. The
Character sample's one-second forward maximum is 210.859543 cm; the NPC sample's
is 193.168121 cm. Increasing requested speed beyond this does not create suitable
sprint data. Both experiments demonstrate this. New source motion or training
coverage is needed; merely accelerating these clips is not the solution.

Turn generation originally used identity ControlRoot after movement. Supplying
the accumulated generated root and world-space trajectory fixes turn convergence:
the four 90/180-degree requests finish within 0.3 degrees. See donors-feedback and
turn-feedback-results.json. These are donor-only, not retargeted/D2 turn coverage.
The same correction improves run_forward_right trajectory alignment to 0.998.
Do not use the original open-loop turn donors as completed turning animations.

REPRODUCE / REVIEW
1. generate_locomotion.py via existing Run-UnrealScript.ps1 generates original
   donors. probe_npc_controller.py and probe_root_feedback.py retain the probes.
2. analyze_donors.py and retarget_walk_bank.py run in Blender 5.2 LTS. The latter
   reads the preserved milestone-F4F native/FP helpers and exact native rigs.
   Emily uses the preserved Corvo FP ready-blade reference as inspiration,
   adjusted to Emily anatomy and her own socket; it is not an Emily FP capture.
3. preview_walk_bank.py builds independent native weighted preview scenes.
4. Build-D2Lab.ps1 builds the candidate. No installation into the Steam directory.
5. Restore-D2Lab.ps1 -Doppelganger restores native clone fixtures; play the native
   Mayhew scene to initialize player swords. Use exact follow/locomotion/combat.
6. Place retargeted/corvo-walk-bank.bin beside CLIENT status as .movement.bin,
   and emily-walk-bank.bin beside HOST status. Test-WalkBank.ps1 and
   Test-MovementPriority.ps1 provide bounded native-controller tests.
7. Replay-WalkBank.ps1 -Source host/client records matching sequences. Sky capture
   must be ready first when -AwaitRecording is used. encode_videos.py preserves
   captured timing. The scripts always retire bank/action leases afterward.

Preserved: original Steam assets, original saves, SLOT81 role fixtures, isolated
save snapshot pre-runtime-saves, prior 092E milestone. No new game UI, storefront,
network provider, first-person moveset or original asset edits were introduced.
Ghidra project saved with confirmed walking-bank notes at 0x140540a90; prior notes
are preserved. Native helper identity is D2_Model_UpdateCorrectiveJoints 0x14023fca0.

NEXT
Retarget/test closed-loop turn donors; obtain actual running/sprinting motion;
build authored start/brake/stance transitions; then extend crouch/lean, traversal,
combat variants, ranged/items, paired actions, powers and interactions. Preserve
native action fallbacks while replacement families are incomplete. Full native UI,
Steam-session, world/combat authority and campaign requirements remain in force.
