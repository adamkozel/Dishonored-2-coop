# FINDINGS.txt

Original: [work/natural-moveset/boot-seam-v1/FINDINGS.txt](<../../../../work/natural-moveset/boot-seam-v1/FINDINGS.txt>)

SHA-256: `6CDD319E575CF7DF6C844A547BAA8FCD5F7DC712D8C0939999D2A97ED8FC5438`; 4248 bytes.

## Original saved record

````text
Corvo calf/boot deformation — 2026-10-10

User report: persistent chicken-leg shape where Corvo's boots meet the trousers,
visible in sword_walk_e_add and throughout the authoring review library.

Confirmed cause
The high-detail nocloth Corvo mesh has valid normalized native weights. Much of
the calf is weighted to left_helper_ankle_jnt and right_helper_ankle_jnt (196/203).
Despite the names, these helpers are parented to the tibiae (20/24). The old
offline helper approximation drove their whole transforms from the feet (21/25),
including rotation about the foot pivot. This displaced the trouser/boot-shaft
skin and introduced a false joint in the lower leg.

Native evidence
Captured the exact reported sword_walk_e_add frame 17 in both isolated D2
instances, using the unchanged CF60 milestone. Final pose publication, playback,
cancellation, expiry and peer-pause checks passed. Native D2 retains the helper
bind-local translation with a small axial rotation. Existing crouch, jump and
carry-release captures independently show the same anchoring rule.

Ghidra confirmed the existing D2_Model_UpdateCorrectiveJoints path at 14023fca0;
the native finalizer and our current review playback invoke it after pose edits.
A persistent evidence bookmark was saved in the exact Dishonored2.exe project.
This investigation did not alter the game DLL or native mesh/skeleton assets.

Correction
preview_skin.py restores those two calf helper transforms to the shin-relative
bind transform. It applies to existing clip previews, paired victim stand-ins
with the Corvo rig, and future authored helper previews. All anatomical bones,
first-person rigs, source animation bytes, native skin weights and root tracks
remain unchanged. Previously authored binaries are not rewritten: D2 already
evaluates its exact native helper solution during final publication.

Limits
The offline fix conservatively omits the small native axial helper twist. It is
not a full reimplementation of D2's corrective/cloth solver. This is stated in
the viewer and render metadata. Corrected skin does not establish animation
quality, contextual action coverage, contact, physics or gameplay state binding.

Validation
Same mesh, anatomy, camera and frame; 1,603 calf-dominant vertices compared with
the saved native helper output. Mean skin discrepancy before -> after:
  lateral walk: 36.45 mm -> 1.84 mm
  crouch idle:  79.33 mm -> 3.07 mm
  jump:         85.59 mm -> 1.98 mm
  carry release:55.84 mm -> 3.37 mm
Corrected calf anchor positions match the captured native anchors in all four
cases. See validation.json, comparison-grid.jpg and before-after.jpg. Visual
inspection confirms the displaced lower-leg bend is removed in these poses.
The regenerated body-lift preview was also inspected at 0.5, 1.7, 3.0 and 4.2s
(paired-lift-contact.jpg). Corvo and Corvo-based grey stand-ins use the correction.
The existing actor/target overlap in the gather is still a separate known revision.

Preservation
before/ preserves the previous shared scripts and catalog. Original recordings
remain in their original folders. Corrected recordings use skin-review-v1-*.
rebuild-status.json is authoritative for whether the full rebuild has finished.
The main catalog prefers a completed corrected recording when available; older
recordings are labelled as awaiting the calf correction until then.

Render-load interruption
At 17:01:52 the isolated client stopped publishing status while four headless
render workers were active. At inspection the host remained responsive and the
client did not. The exact cause is unproven; do not attribute this to skinning.
The saved minidump includes a graphics-driver thread and waiting game threads,
but the bounded raw stack-address scan is not a symbolized diagnosis.

Stopped only the identified rendering coordinator, its Blender workers and
encoders. Retained 86 completed corrected videos and all original versions.
Captured client-stall-minidump.dmp, then restarted only isolated client PID42208
as PID24900. Restored both existing SLOT81 fixtures through Restore-D2Lab.
The restoration report passed both roles, fresh Steam packets and avatar follow.
Re-enabled exact follow. Rendering resumes with two workers; no new DLL deployed.

````
