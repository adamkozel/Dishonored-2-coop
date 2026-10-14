# Grenade full-body and cosmetic prop candidate v55

This remains a movement candidate, not completed grenade co-op. Native source observations are in [the preceding investigation](18-native-grenade-investigation.md). The preserved [v55 candidate](../../work/movement-integration/candidate-grenade-prop-v55/identity.json) adds a finite manual cosmetic prop test. It does not yet stream grenade actions or reproduce projectile gameplay on the other machine.

## Reproducible authoring

[author_grenade_v55.py](../../work/natural-moveset/author_grenade_v55.py) creates nine individual phases and two connected sequences for each character: idle, equip, unequip, cooking entry/idle, high/low throw and their empty recovery. [The manifest](../../work/natural-moveset/grenade-authoring-manifest-v55.json) records binary hashes, anatomical length error, source hashes, frame rates, hand-local attachment tracks and disabled events. The generated clips retain native Corvo/Emily skeletons, models and corrective helpers. Local first-person animation is untouched.

The held posture uses the existing full-body stun-mine carrying study. Cooking and throwing use native NPC grenade body motion. Native player first-person data supplies fingers, equipment placement relative to the moving wrist, and reference durations. No camera-space hand position constrains the body. Replicated player movement continues to own world position; exported demonstration root tracks are zero and do not drive gameplay.

The NPC high launch begins at 1,000 ms and has its native fire reference at 2,167 ms; the low launch begins at 1,000 ms and fires at 2,350 ms. The candidate maps those body phases to the observed player's 293/302 ms release instants within a 400 ms scene. These are timing references only. Neither authored clip nor prop invokes a native fire event.

[Thirty-five hand references](../../work/natural-moveset/fp-grenade-reference-manifest-v53.json) and [three six-joint grenade mechanism clips](../../work/natural-moveset/grenade-mechanism-manifest-v55.json) were sampled using the offline native Edge evaluator. [The native mesh export](../../work/natural-moveset/item-models/grenade.json) has 5,319 vertices, 8,257 triangles and six bones, with source/skeleton/library hashes. The model itself remains the game's native model.

## Visual evidence and limits

[High sequence](../../work/natural-moveset/grenade-videos-v55/grenade_high_sequence.mp4) and [low sequence](../../work/natural-moveset/grenade-videos-v55/grenade_low_sequence.mp4) are paired-role authoring previews. Their temporal contact sheets were inspected: the held prop follows the free hand; the body changes stance during preparation and release. The views are too wide to accept finger contact. The accelerated throw, low-throw transition from the common cooking posture, foot movement and standing recovery require continuous and in-game review. No projectile is generated in the authoring preview; the held prop becomes invisible at the source release reference. These videos must not be described as actual D2 integration.

## Native cosmetic resource and attachment

[The resource](../../outputs/d2-coop-lab/assets/grenade_prop.def) is an `idEntity` using `models/weapons/grenade/grenade.md6` plus native equipment animation. It deliberately has no weapon, grenade gameplay, collision, pickup, attention or damage component. [The installer](../../outputs/d2-coop-lab/Build-D2GrenadePropAssets.ps1) verifies the physical isolated path and exact executable, backs up replaced files, verifies installed resource bytes and rechecks original Steam indices.

[grenade_prop.h](../../outputs/d2-coop-lab/src/grenade_prop.h) uses the already investigated native entity/reference-frame creation and bind calls. It validates the exact definition, component whitelist, reciprocal parent/child relation, joint name and native damage-source parent ownership. A fifteen-second isolated lease hides the prop through native Hide; pause, scope loss and ranged-prop conflict stop it. Changed worlds retire the old identity. Repeated requests cannot extend an active lease. The body remains the existing remote Doppelganger.

The initial experiment uses a fixed native idle hand-local calibration and a held authored idle. Action-dependent attachment tracks and prop mechanism playback are not integrated yet. Native cooking sparks/trails, grenade release/world object replication, detonation, damage authority, moving/crouched variants and interruptions remain separate tasks. The first runtime attachment test is pending in this chapter until linked evidence is added.


## Native runtime attachment v55b

[Corvo close-up](../../work/natural-moveset/grenade-prop-corvo-close-v55b/result.json) and [Emily close-up](../../work/natural-moveset/grenade-prop-emily-close-v55b/result.json) pass attachment, finite native Hide and unchanged inventory/health/protected-file checks. Their actual-game sheets and recordings are indexed in the review dashboard. The grenade remains associated with the authored left hand, then disappears. Lighting and occlusion limit finger-contact judgment; final grip quality is unaccepted.

The initial [v55 take](../../work/natural-moveset/grenade-prop-corvo-idle-v55/result.json) failed its Hide completion assertion. `rangedPropIsHidden` is a reuse predicate that also requires `pendingHide == false`. Calling it before clearing that flag could never complete cleanup. v55b reads native entity flag `+0xd0 & 1` after native Hide, matching the established ranged cleanup path. The failed take and build are preserved; no result was relabeled.

The ten-second review pose lease is re-armed separately from the fifteen-second prop lease for this fixture. A transient native pose between review leases is not a normal gameplay transition acceptance result. The candidate [v56 bank builder](../../work/natural-moveset/build_grenade_bank_v56.py) appends nine grenade phases for each role, preserving all previous payload bytes. [Native Steam test](../../work/natural-moveset/Test-GrenadeNetwork-v56.ps1) records source and remote views and resets both temporary worlds after a real native throw. Network binding is pending until its actual results are linked.
