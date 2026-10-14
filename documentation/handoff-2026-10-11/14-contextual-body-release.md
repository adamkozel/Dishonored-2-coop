# Compact contextual release — continuation

The original native `carry_corpse_drop_fetal_pose` is **not** `carry_corpse_drop_soft`. Earlier throw notes used “soft drop” informally for variant5; that name is corrected here. The previously authored soft sequence uses a different victim clip and375ms ragdoll marker, so it must not be bound to variant5.

## Confirmed evidence

- Exact scene SHA256 `7B4F08A43A28422F626DF9B82B5CF974BD1A6FB5B9E612B1959590D45D75AB47`; carrier666ms, NPC animation `generic_carrycorpse_dropfoetal`166ms. The NPC event track has no keys.
- Ghidra [selector1403a6f10](../../work/natural-moveset/carry-research/native-soft-selector-1403a6f10-v35.json) returns4 as fallback. Caller140832770 selects declaration+98, stores selector at Drop46+54, and sets corpse+368 bit2 for selector4. Names of all sensor bits are not established.
- [Native Corvo trace](../../work/natural-moveset/native-fetal-release-corvo-reader-v35/native-release.json) uses separate read brackets, not atomic snapshots. Physics changes from3 to2 during the compact release and later0; native retarget count remains1 until source scene completion. The read window cannot establish exact native callback timing.
- [Physics scheduler140736c30](../../work/natural-moveset/carry-research/contextual-release-140736c30-v35.json) distinguishes corpse carry bits and paths734040/734d90 when entering a physical state. [734d90](../../work/natural-moveset/carry-research/contextual-release-140734d90-v35.json) seeds rigid body transforms from visible pose and clears velocities. The experiment does not claim identical native physical behavior.

## Candidate and preservation

[v35 candidate](../../work/movement-integration/candidate-fetal-release-v35) pins DLL `DB4306D57DD72A81ADF7EBD593FC26E8A78BB3BF9AA2907E35CCCCFBB0A91589` and unchanged probe `A0D22CD8EA4203D64AD5F16DB2A55287E45F9D62BAC74F84F16B350727BF8DCD`. v34/v33/v30 rollback archives remain available.

D2MOVE37 adds clip234 to both banks with all234 prior payloads byte-identical. [Bank manifest](../../work/natural-moveset/fetal-bank-manifest-v35.json), [builder](../../work/natural-moveset/build_fetal_bank_v35.py). Full-body high-release preparation supplies the initial acting, then recovery starts after the166ms compact clip. It is a visual candidate, not accepted movement. No first-person pose anchors, bone-length changes or root motion are introduced.

[Victim-only builder](../../work/natural-moveset/build_fetal_release_v35.py) retains the compact NPC animation/look-at tracks; no camera, player arm, gameplay event, spawning or damage tracks. Generated SHA256 `F75A1098E7FE1C3BF7D14CC906D58867EB015726A26211885C35F0AFB58DF4BF`. [Clone-only install report](../../outputs/d2-coop-lab/research/carry-fetal-asset-install.json). Original Steam indexes were verified unchanged by the installer.

The isolated attachment fixture gates variant5 to bank37 and standing source stance, retains the attachment during the166ms animation endpoint, then detaches and restores ragdoll. **166ms is a candidate handoff based on clip duration, not an explicit native event.** Variant5 cannot carry a throw impulse. Older banks retain their previous behavior.

All bank format fixtures, both actual new banks, semantic packet/timing tests and proxy smoke passed. Runtime acceptance is separate.

## Test development

The initial [source reader take](../../work/natural-moveset/native-fetal-release-corvo-reader-v35) uses the v34 DLL. It confirms native variant5, but the full pickup report fails one bank-refusal check after an instantaneous setup turn. It is retained as failed, not promoted.

The first [v35 downward-view attempt](../../work/natural-moveset/native-fetal-release-corvo-v35) passes23 pickup checks but native D2 still chooses high throw. Camera positioning takes long enough to exceed the intended early release window. Compact-release validation fails and correctly records no variant5 coverage. This does not validate compact playback. The footage also clips the carrier at the edge of the observer view.

The next harness uses ordinary controller turning with a measured native yaw rate, avoiding the instantaneous heading change. Its per-take results remain the source of truth: [Record-FetalRelease-Turn-v35.ps1](../../work/natural-moveset/Record-FetalRelease-Turn-v35.ps1). [Validator](../../work/natural-moveset/validate_fetal_release_v35.py) checks the actual selected native variant, owned victim, no throw impulse, cleanup, equipment and health. Never count a completed input sequence as proof of the intended native action.

Blender MCP was unavailable at localhost9876 during this continuation. Existing Blender5.2.0 LTS ran the authoring script in background mode successfully without modifying the interactive scene. No reinstall or version changes were needed.

Whole movement and co-op remain incomplete. Completion requires remaining gameplay bindings, transitions, both-role coverage, convincing contact and equipment, power VFX and actual video review across the105-item movement checklist.

## Framed comparison and rejected snapshot experiment

The Corvo and Emily `*-turn-v35` takes pass23 pickup and19 compact-release checks, but the observer framing loses most of the carrier. Ordinary controller turn input did not produce the intended180-degree turn: the stored native camera reports about-100.6 degrees. Those recordings establish only their measured lifecycle coverage.

The subsequent `*-framed-v35` takes change observer heading to195 and pitch to5. Both again pass23+19 checks, but inspection clearly shows the victim straightening above the carrier before falling. Hands do not convincingly support the release. These are **visually rejected** despite the successful lifecycle checks.

[Native pose screening](../../work/natural-moveset/carry-research/native-release-pose-screening-v36.json) evaluates the exact extracted clips in Blender. `generic_carrycorpse_dropfoetal` has6 frames at30Hz, with a strongly folded pose throughout: male knee flexion about140/129 degrees and female149/138. These are offline reference measurements, not proof of native runtime playback.

[v36 snapshot candidate](../../work/movement-integration/candidate-release-blend-v36) DLL `A726151D2416C6A0B83F0206C07FC6FDEC925544DF20425856BC229081591177` attempts a100ms native recovery-snapshot blend from the held victim. [Corvo take](../../work/natural-moveset/native-fetal-release-corvo-blend-v36) passes pickup checks but **fails release-scene activation and bounded-trajectory checks**. The sampled status never records a captured release snapshot, and the victim exits above the visible frame. This candidate is rejected; do not promote or interpret it as a smoother release. Existing ownership checks were retained.

[v37 candidate](../../work/movement-integration/candidate-release-order-v37) restores the v35 implementation and changes request ordering: after old-scene retirement, request native animated physics and wait for mode3/pending-1 before binding the release scene. [Exact736ef0 decompilation](../../work/natural-moveset/carry-research/release-animated-request-140736ef0-v37.json) confirms the mode3 pending request; whether the previous order caused the visual defect is still a hypothesis. DLL `0D16B29C6757132107320EF10706E0592992B6726394245CAA39A9144399190F`. D2MOVE37 payloads remain unchanged.

The [contact trace screening tool](../../work/natural-moveset/screen_release_contacts.py) reports victim root, anchor, head and knee bend from separately bracketed live skeleton reads. It explicitly does not treat those reads as atomic or mark visual acceptance. All take folders preserve their exact input harness, build identities, videos and validation results.

## v38: compact pose survives the physical handoff

The order-only v37 take passes lifecycle validation but still exposes the upright pop. Its live skeleton proves the compact clip itself works: knee flexion reaches about148/137 degrees, then falls to22/5 degrees during handoff. This rules out a missing compact animation as the whole explanation.

[v38](../../work/movement-integration/candidate-compact-hold-v38) DLL `A46597540535879DE144EE173C18D7F995195B16FC43EC35F35BCD86F3BBB09D` adds a separate held-pose derivative and preserves the native compact-release flags through the physical transition. [Builder](../../work/natural-moveset/build_compact_hold_v38.py), [asset manifest](../../work/natural-moveset/compact-hold-v38.json), [clone install report](../../outputs/d2-coop-lab/research/carry-compact-hold-v38-asset-install.json). The derivative stretches the constant6-frame pose from166 to666ms so it remains available while physics blends; the source166ms handoff hypothesis is unchanged. No animation events are added.

`140832770` sets NPC+368 mask0x4 for selector4. `140736c30` uses masks0x2 and0x4 to select visible-pose seeding through734d90. The fixture now retains those masks until cleanup instead of clearing mask0x2 before the transition. It waits for ragdoll mode0/pending-1 before retiring the held scene, with a finite600ms fallback. The original compact resource remains installed separately, so earlier binaries can still select it.

[Corvo](../../work/natural-moveset/native-fetal-release-corvo-hold-v38) and [Emily](../../work/natural-moveset/native-fetal-release-emily-hold-v38) each pass23 pickup and19 compact-release checks at about27Hz. Their inspected in-game frame sheets show the upright pop removed and a folded body falling into ragdoll. Sampled knee flexion stays roughly109–148 degrees during the release instead of approaching straight. This establishes a useful improvement in these exact takes, **not complete carry quality**: carrier hand/body contact and recovery acting still need refinement, and arbitrary victims, interruptions and other contexts remain unaccepted. High/low release regressions are tracked separately.
