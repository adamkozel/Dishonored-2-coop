# Animation authoring and D2 integration

The first-person animation provides intent, timing, blade orientation and native state evidence. It must not freeze the third-person wrists in space or become a permanent full-body anchor. The user specifically rejected that appearance. Local first-person camera, controls, hands and animations remain native.

## Sources and representations

Native assets come from exact catalogs via the pinned Void reader. Native rig agreement established Corvo 215 joints, Emily 219 and the observed first-person arm 69; a generic 212-joint civilian rig is not interchangeable. Preserve rest axes, anatomical lengths, corrective/helper bones and socket hierarchy. Native deformations require their procedural helper phases as well as pose data.

Mixamo FBX is imported in Blender and retargeted, not loaded directly by D2. The selected crouch donor settings and subsequent authoring steps remain in the Mixamo/source manifests. In-place playback leaves position to networking. The user-approved Posture 100, Corvo Arm-Space 70 and Emily 50 refer to that crouch experiment, not every later animation.

AnimGen runs locally inside Unreal. The actual proof uses `/AnimGenExample/Characters/UEFN_Mannequin/Animations/AGC_AnimGenCharacter`, seed 1234, 60 Hz evaluated poses and contact outputs. It exports evaluated native data through the helper; it does not claim a successful generated FBX path or a network service trained on D2 assets. See [original AnimGen notes](../../work/animgen-feasibility/README.txt).

## Pipeline

1. **Inspect source.** Preserve exact native resource name/key, bank/clip identity, donor settings, duration, coordinate conventions and source hash. Separate locomotion from contextual/paired clips.
2. **Import and normalize.** Decode native bind hierarchy or import donor motion. Convert source coordinate axes and units explicitly. The initial AnimGen proof used UE +Y/cm to D2 +X/metres and a recorded leg-scale factor; never apply that factor blindly to other donors.
3. **Retarget anatomical motion.** Map semantic joints and rest-axis differences; preserve native lengths and unmapped helpers. Author full torso, pelvis, arms and head motion that supports balance and contact. Root/anchor channels stay identity for in-place mod playback.
4. **Author contacts and transitions.** Match feet to planted intervals/terrain, preserve knee direction, integrate equipment and hand poses, and build stance/action entry/exit. Contact constraints must not erase natural body mechanics.
5. **Export a guarded mod bank.** D2PROOF1/2 preceded D2MOVE formats. The current D2MOVE35 bank has 233 clips; separate standing and crouched carry hoists use indices 231 and 232. Versioned loaders validate hierarchy, layout, lengths, rigidity and forbidden root animation.
6. **Bind real semantics.** Use actual native states or scene clocks over Steam. Locomotion uses distance/timestamps; contextual actions use their native source duration/phase. A triggered preview is not gameplay binding.
7. **Apply at the correct native phase.** Preserve animation, IK, helpers and child-equipment publication order. Sample final published poses for verification. Corrective stages can otherwise make a preview accurate but the game rig visibly deformed.
8. **Validate and record.** Numeric transfer and lifecycle checks plus actual D2 recordings, useful views, both characters and transition/recovery cases. Keep rejected variants with exact hashes.

## Corvo legs and native IK

The user identified persistent distortion where boots meet trousers, described as chicken legs. Preview correction and native post-IK correction are separate investigations. Recent state records identify native IK knee movement up to 31 cm despite nearly preserved ankle endpoints in the rejected v19 case. The later owned phase-0-to-phase-3 knee-plane correction preserves native foot endpoints, ankle rotations and lengths and reruns helpers; selected pose matrices and sheets were inspected. This does not grant blanket acceptance to all terrain, turns and animation combinations.

## Carry example and exact authoring assumptions

`author_crouched_pickup_v1.py` added a complete crouched gather/hoist, blending unarmed crouch to loaded pose with pelvis/torso movement, native-length foot IK and measured victim contact references. v2 deepened the reach and added a small approach step. v3 retained that motion but refit supporting contact against the corrected v29 victim orientation.

The contact target uses a point 0.72 along the victim thigh plus explicit palm offsets. The load target uses the carrier shoulder plus clearance `(0.025, 0.19, 0.115)`. These are authoring choices, not recovered universal anatomy. The native crouched Mayhew reference has a 0.674 m root-height difference; generalizing it to other victim types is a hypothesis.

v3 changes bank clips 227, 228, 230 and 232 relative to the retained v1 base, preserving unrelated motions. The v30 runtime change corrects shoulder-relative placement independently of those bank edits. Keep binary and bank revisions separately attributable.

## Powers and effects

A cast pose is insufficient for powers whose identity depends on effects. Each power needs native source detection, its full-body presentation, target/endpoint context, native effect lifetime and gameplay outcome ownership. Record abort, expiry, holster/stance, pause, load and world replacement behavior. A local/private overlay or bounded native effect test must not be called synchronized authoritative power gameplay.

The complete catalog retains power-specific visual requirements. See its per-item notes and the exported Ghidra bookmarks for Dark Vision, Windblast, Blink endpoints and other investigated contexts. Never let an animation notify independently apply a second hit or power outcome.
