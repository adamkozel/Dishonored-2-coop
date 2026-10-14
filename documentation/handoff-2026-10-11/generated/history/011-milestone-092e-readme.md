# Historical note: work/animgen-feasibility/milestone-092E/README.txt

Original: [work/animgen-feasibility/milestone-092E/README.txt](<../../../../work/animgen-feasibility/milestone-092E/README.txt>)

SHA-256: `2BC93289DC89384460911CF13B31D20948C1EF8CC3162B32F241BB1728011420`; 9763 bytes.

````text
ANIMGEN -> NATIVE CORVO -> DISHONORED 2: FIRST GENERATED MOTION CANDIDATE

Result, 10 October 2026

The Unreal installation and compiler blockers are resolved. An actual pretrained
Epic AnimGen controller produced the locomotion used in this separate candidate.
It was retargeted in Blender to Corvo's native 215-joint rig, combined with captured
D2 first-person wrist/blade references, and played on the owned Corvo Doppelganger
in the isolated Dunwall game. This proves the generation/authoring/game boundary.
It is a reviewable movement candidate, not an accepted production moveset.

Watch d2-generated-sequence.mp4 and blender-generated-sequence.mp4.
The D2 recording includes the old native idle before and after the experiment.
It contains 157 real window captures at about 12 Hz, encoded in a 30 FPS container
at the original timestamps. It is not a 30/60 FPS capture or a speedup. The Blender
render samples the 60 Hz authored sequence at 30 Hz. See video-provenance.json.

Sequence and sources

Eight seconds: ready, forward start, walking, stop, FP-inspired sword guard, and
recovery. AnimGen's requested trajectory is stationary for two seconds, eases to
100 cm/s over 0.5 seconds, stays there until 5.5 seconds, and eases to rest at six
seconds. Inference can anticipate the requested movement. The native body scale
produces about 4 m of travel. Runtime duration follows actual native travel, so it
need not equal the eight-second authoring timeline.

Exact generator: /AnimGenExample/Characters/UEFN_Mannequin/Animations/AGC_AnimGenCharacter
Mesh: /AnimGenExample/Characters/UEFN_Mannequin/Meshes/SKM_UEFN_Mannequin
Epic AnimGen Example 1.0, Fab listing:
https://www.fab.com/listings/df37eb46-09bf-4604-9307-cdc39c769790
Seed 1234, 60 Hz, four-sample one-second TrajectoryFollow + empty tagged behavior,
pose smoothing 0.25 seconds, contact_l/contact_r outputs, no animation notifies.
The sample's learned controller/autoencoder hashes are in sample-pipeline-status.json.
Its additional offset-root, foot-locking Control Rig, aiming and pistol layers
were not evaluated as a complete AnimBP. This is not a model trained on D2 swords.
No D2 assets were uploaded to an external generation service.

The D2 first-person reference is the preserved fullbody-proof/clean-take-2 capture.
It supplies wrist/blade orientation and guard timing. Native arm lengths, closed
right-hand finger reference and weapon socket come from the preserved F4F inputs.
The offhand/torso guard adjustment and native-length IK are authoring constraints.
The first-person animations are reference inputs, not a complete-body training set.

Pipeline and reproducibility

Pinned tools: UE 5.8.3-58210709, bundled AnimGen experimental 0.1, sample 1.0,
Blender 5.2.0 LTS, Unreal Python 3.11.8, MSVC 14.44.35229,
Windows SDK 10.0.26100.0 and .NET Framework 4.8.1 SDK.
See working-versions.json for locations and hashes.

1. Build-CaptureHelper.ps1 builds the isolated editor module. No original Unreal
   project is modified. Run-UnrealScript.ps1 -Script capture_generated.py -Name
   <new-label> performs bounded headless inference with native matching controls.
2. D2ProbeLibrary reads the sample's imported LOD mesh and each evaluated pose.
   Root motion is extracted from AnimGen's native animation attributes, not the
   legacy extraction slot (which was zero). The schema validated on all 480 poses.
3. preview_generated.py verifies the generated donor in Blender, including native
   rest-scale compensation. The headless skeletal-mesh FBX exporter asserted, so
   the successful path uses explicit pose/mesh data. The separate supplied pistol
   animation FBX round trip is a pipeline check, not generated motion evidence.
4. retarget_generated.py reads the preserved native Corvo rig and frozen F4F
   helpers. It maps authored anatomical rotations, preserves local bone lengths,
   compensates rest axes, applies native-length foot contact IK and D2 weapon
   constraints, and saves exports/corvo-generated-candidate.blend and JSON.
   UE +Y/cm is converted to D2 +X/metres; leg scale is 1.09506710455.
5. export_candidate.py writes D2PROOF2: 481 frames, 215 joints, 60 Hz, validated
   root-distance curve. Root and anchor remain identity. Runtime never applies
   generated root displacement to the player. This is a guarded mod pose format,
   not a claim that D2 can load a generic FBX animation.
6. The opt-in Corvo experiment runs on the existing native Doppelganger through
   native skin correctives and equipment publication. Actual movement over the
   existing Steam transform stream selects walking phase. No damage, hit, or
   animation-notify gameplay events are generated by this pose player.

Source and binary pins

DLL: 092ED98F46921D73A46354CA31F69DE7AEFDFDF30FD9B14DAC767AE49ED9A23B
Clip: AA3942F70C5494949E00B18925E9377425A2B88F3E8C8B7C460D8E4FC3F3526C
Native joint layout: 00db32d37719931f
The candidate retains D2PROOF1 compatibility. The previous F4F milestone and
runtime-before-v2 remain intact. The new milestone-092E stores sources, pins,
editable authoring output, clip, reports and videos with a SHA-256 manifest.

Runtime evidence

runtime-take-5 is the complete synchronized recording: 475 coherent published
pose observations; full phase 0..8; no playback refusal; source height constant.
Actual controller travel was 4.083 m against the 3.9825 m authored curve. This
overshoot is retained in the evidence. The test harness reads coarse status files
to brake; animation root placement remains governed by the native game/network.
Take 4 completed with 4.016 m travel. Take 1 hit furniture and correctly stopped
on vertical deviation; take 2 hit a wall short of the endpoint. Those are not
successful full-sequence tests. Take 3 completed but had inferior camera framing.

Fixed guard frame 405 isolates transfer accuracy from asynchronous timing:
77 stable pose observations, anatomical/socket translation error zero, rotation
error at most 0.0396 degrees. A separate stable native sword attachment snapshot
matched parent x socket x native binding, including native/render position.
This verifies attachment transforms; it does not replace a visual hilt-fit check.

Walking contact measurements still show visible-quality work remaining:
high-contact segments displaced by roughly 2-9 cm; median ankle speeds were
0.027-0.037 m/s with larger transition spikes. Approximate moving phase matching
has large guard-transition outliers, whereas the exact held pose agrees. A future
atomic phase/pose record is needed to separate timing from transfer there.
Numerical round-trip success is not acceptance of final animation quality.

Build and safeguards

Build-D2Lab.ps1 and the legacy/new clip tests passed. New format rejects malformed
headers, invalid distance curves, bad hierarchy/layout, nonrigid data, changed
anatomical lengths and root/anchor animation. Runtime checks passed for explicit
cancel, wrong character/role refusal, invalid clip, either native pause, refusal
to rearm while paused, and no automatic replay on resume. The new 12-second lease
expired and stopped pose writes. Moving tests additionally exercised off-plane
refusal and bounded no-progress expiry. Reports include exact DLL/clip hashes.

Original Steam executable/DLL and all eight baseline save/config files passed the
integrity check. No dinput8 was installed in Steam. Isolated saves were copied and
hash-verified before restarting two memory-heavy overnight game instances. Their
combined ~69 GB private commit is an unresolved soak-test issue, not a diagnosed
leak. The original Unreal project was also preserved. Test instances are left
connected, windowed, side by side, with neutral input and the proof disabled.

Review assessment and remaining work

The generated walk has coherent full-body stride and weight changes and retains
the native character silhouette in D2. The closed sword hand and guard survive
transfer. This makes the pipeline useful enough to retain for further work.
The guard remains restrained; stop/entry/exit blending and foot contact need
polish. The old bent-leg idle/open hand returns when the bounded clip releases.
Do not promote this experiment to the stable automatic movement path yet.

Still unproved here: Emily, turns, side/backward movement, crouch, jumps, slopes,
climbing, powers, attacks/parries as authoritative gameplay, and action interruption
arbitration. Full local first-person combat regression was not repeated for this
candidate; the new code targets the owned remote Corvo only, and the original
first-person implementation/assets remain unchanged. Prior tests retain their
original DLL attribution. Native UI, complete co-op world/campaign synchronization,
all friendly-fire routes, and separate-account Steam online sessions remain goals.

Safe replay in the existing isolated fixture

Use outputs/d2-coop-lab/Restore-D2Lab.ps1 -Doppelganger when restoration is needed.
Do NOT use the older Load-D2AnimationCheckpoint.ps1 here: it spawns story NPCs
instead of the expected native shadow actors. Enable exact follow, locomotion and
shadow combat style as used in the current fixture. The candidate is installed
only at work/lab-instances/client/coop-status.json.authoring.bin.

Replay-Generated.ps1 -Take <new-take-name> drives the bounded forward corridor.
The observer recording used setviewpos -32.3 -148.5 68.1 270 12. The script places
the source once at -34.3 -153 68.1, facing +X, then uses native controller input.
For synchronized capture pass -AwaitRecording and create <take>/record.ready only
after the recorder starts. It times out without driving input if no recorder
arrives. coop_authoring_proof off cancels immediately. No continuous replay loop
or automatic enablement is installed.

````
