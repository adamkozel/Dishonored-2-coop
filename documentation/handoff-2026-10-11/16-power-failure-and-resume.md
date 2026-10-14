# Native failure animation and interrupted movement recovery

This chapter extends the [Bend Time work](15-bend-time-integration.md). All claims are scoped to the exact Steam executable, isolated Dunwall scene and named take. Movement phase acceptance remains open.

## Confirmed native insufficient-mana branch

[The corrected native test](../../work/natural-moveset/native-bendtime-corvo-no-mana-v44-v2/result.json) passes eight checks. Controller packet consumption is recorded, not inferred from input submission. One normal Bend Time activation/end leaves a measured native mana value of 40 from an initial 100. Two subsequent ordinary requests produce separate `power_outofmana.midnightscene`, `Scene_1`, 333 ms instances. Each binds only the reciprocal Corvo left arm. The native power remains inactive and measured mana does not decrease further. Native Running0 is the sole active state in these standing armed observations.

The [read-only mana probe](../../work/natural-moveset/Read-NativeMana.ps1) validates exact `arkCpntMana` vtable RVA `0x1c55ed8`, component ownership and stable membership before reading the float at component+0x20. Ghidra `140746310` supports initialization from the declaration/attributes. Its precise expenditure function and all upgrade-dependent costs are not identified. Do not treat a single observed difference as a universal power cost.

[The first attempt](../../work/natural-moveset/native-bendtime-corvo-no-mana-v44/result.json) is intentionally retained as failed. It observed two real failed casts, then native regeneration brought mana to 60 and a later cast succeeded. This invalidated the test's third failure precondition. The corrected take requests failure at approximately six and eight seconds, before that regeneration, and independently verifies both scene IDs and every controller acknowledgement. No resource was written by the mana probe.

Exact assets and their SHA-256 identities are in [native-outofmana-v45](../../outputs/d2-coop-lab/research/asset-exports/native-outofmana-v45/manifest.json). The ordinary scene uses the native `ply_leftarm_power_outofmana` alias for both characters. A distinct slide resource exists but has not been observed at runtime. Asset existence alone is not a movement binding.

## v45 candidate implementation

[Archived code and tools](../../work/movement-integration/candidate-outofmana-v45) pin protocol35, D2MOVE40 and 248 clips. [Bank identities](../../work/natural-moveset/outofmana-bank-manifest-v45.json) and [reference identities](../../work/natural-moveset/outofmana-reference-manifest-v45.json) make the conversion reproducible. All prior clip payloads remain unchanged.

New semantic15 requires the exact active native scene, duration, reciprocal left arm, selected owned Bend Time item, inactive native power, native standing Running state and equipped sword. The final native read repeats identity and inactive-power checks. The wire refuses older peers, Emily, holstered and airborne failure packets. The body layer additionally requires standing with its sword drawn and the new bank. Neither input alone nor a low numerical mana value starts the animation.

[Authoring](../../work/natural-moveset/build_outofmana_bank_v45.py) uses the native third-person caster donor for the body and sampled first-person finger movement as reference. It preserves native lengths, foot contacts, corrective joints, sword grip and root identity. First-person wrists are not body anchors. Emily's corresponding authored clip is an offline rig candidate only. The appended clip has no power activation, root displacement, damage or remote screen effect.

Static checks cover exact scene/arm/selected-power identity, active-power rejection, unsupported contexts, packet version and bounds, actual bank payloads and old-bank compatibility. Runtime status must be read from the individual take, not inferred from this design description. [Runtime harness](../../work/natural-moveset/Test-OutOfMana.ps1) records both actual game views, native scene IDs and an independent fast presentation trace.

[The first paired v45 take](../../work/natural-moveset/outofmana-corvo-standing-armed-v45-v1/result.json) passes eleven scoped checks, including two distinct native/remote failure events and recovery without new observer effects or damage. The inspected right-side frames show body response and retained sword, but the acting hand is occluded. This is not visual acceptance of the fingers or complete gesture; a frontal take is next.

## Pause, retirement and recovery

[Source pause with resume](../../work/natural-moveset/bendtime-source-resume-v44/result.json) and [observer pause with resume](../../work/natural-moveset/bendtime-receiver-resume-v44-v2/result.json) each pass eight scoped checks. Both animation banks suspend during native pause, recover using fresh native Steam ownership tokens and retain their pinned checksum and avatar identities. The power presentation returns to standing, equipment and health survive, and the temporary power fixture restores native inventory and selection.

The actual video frames show a temporary disappearance while ownership is suspended, followed by avatar return. These results demonstrate recovery, not seamless visibility. Death, load, disconnect, mission transitions and broader animation interruptions are separate outstanding cases.

Earlier cleanup-only takes deliberately disabled automatic recovery. They correctly stopped body writes and avoided delayed replay, but the avatars stayed hidden. Those takes must not be cited as usable automatic recovery.

[The first observer-resume take](../../work/natural-moveset/bendtime-receiver-resume-v44/result.json) fails its final fixture cleanup check despite recovering both avatars. Loss of the other movement owner starts bounded power-fixture cleanup; waiting for bank recovery before ending native time used that five-second allowance. The revised harness ends time with ordinary controller input just after menu resume, then waits for bank recovery. No native time manager or mana write was introduced to make the test pass. The original failure and its script remain preserved, and the isolated checkpoint was restored before retesting.

## Air context and remaining work

[The armed airborne v44 take](../../work/natural-moveset/bendtime-corvo-air-v44-v1/result.json) passes nine checks with observed Jump/Fall, exact owned power scenes and remote air/cast overlap. The first v43 attempt failed because the receiver rejected the new combination; v44 updates both the sender and the packet validator. The failed take remains in the dashboard.

Remaining scope includes holstered/travelling airborne combinations, travelling crouch, slide/cancellation, all other failed powers and both characters, native world effects/time authority, close hand contact and continuous visual review. Completing any one row does not complete the full movement phase.

Subsequent [holstered travelling air test](../../work/natural-moveset/bendtime-corvo-air-empty-moving-v45/result.json) passes ten scoped checks. Viewed actual-game frames show airborne empty-hand casting and landing; full continuous quality remains unaccepted.

## v46 context extension

Native-only observations subsequently pass eight checks each for [standing holstered](../../work/natural-moveset/native-bendtime-corvo-no-mana-standing-empty-v45/result.json), [crouched armed](../../work/natural-moveset/native-bendtime-corvo-no-mana-crouched-armed-v45/result.json) and [crouched holstered](../../work/natural-moveset/native-bendtime-corvo-no-mana-crouched-empty-v45/result.json). All use the same exact 333 ms scene and sole owned left arm. Crouched observations have sole native Sneaking6; standing observations have Running0. First-person hands are not equally visible in the holstered crouch capture, so scene ownership and pose visibility are documented separately.

[v46 archive](../../work/movement-integration/candidate-outofmana-contexts-v46) extends the native reader to those observed combinations, while keeping airborne, sliding and Emily failure unbound. Protocol36 allows the observed holstered response. D2MOVE41 adds three stance/equipment clips, with all 248 earlier payloads unchanged. [Builder](../../work/natural-moveset/build_outofmana_contexts_v46.py), [bank pins](../../work/natural-moveset/outofmana-context-bank-manifest-v46.json) and [paired test](../../work/natural-moveset/Test-OutOfManaContexts.ps1) are preserved. Entry and recovery blend the full authored context; root remains network-controlled.

The v46 native-identity, packet, actual-bank and proxy smoke tests pass. Four paired runtime takes each pass twelve checks: [standing armed](../../work/natural-moveset/outofmana-corvo-standing-armed-v46/result.json), [standing empty](../../work/natural-moveset/outofmana-corvo-standing-empty-v46/result.json), [crouched armed](../../work/natural-moveset/outofmana-corvo-crouched-armed-v46/result.json), and [crouched empty](../../work/natural-moveset/outofmana-corvo-crouched-empty-v46/result.json). Every observed frame retains its stance, two independently owned native failures produce two remote events, equipment and inventory recover, and health plus the fourteen protected settings/save hashes remain unchanged. Inspected actual-game temporal sheets show the body response and stance recovery. Hand detail remains dark, and the crouched sword approaches the floor. These are scoped integration results; whole-phase and visual acceptance remain false.

## Manual hand-effect investigation, v47

The native scene starts `models/effects/gameplay/player_generic/tatoo/out_of_mana/out_of_mana_01.fx` on `fx_tatoo`, with position following and a 1000 ms track. [Read-only effect exports](../../outputs/d2-coop-lab/research/asset-exports/native-outofmana-fx-v46/manifest.json) contain the exact FX and particle declarations. Active smoke and ash layers are present; the second smoke layer and trail are disabled in the source declaration. Screen-space behavior still requires a rendered experiment.

[Native first-person mesh exports](../../outputs/d2-coop-lab/research/asset-exports/native-failure-sockets-v47/manifest.json) place `fx_tatoo` on `left_medium_01_jnt`, with identity rotation and offsets Corvo `(-0.006,0.025,0.033)`, Emily `(-0.006,0.025,0.009)`. The remote skeleton lacks that exact metacarpal. Copying these coordinates onto a similarly named remote finger would use the wrong basis. The first probe deliberately uses the remote left wrist centre; precise tattoo placement is not yet established.

[Ghidra start-function evidence](../../work/movement-integration/bendtime-research/outofmana-fx-start-v47.json) confirms native effect world origin at record+0x38, axis at +0x44, and callback at +0x128. [v47 candidate](../../work/movement-integration/candidate-failure-hand-fx-v47) tracks only the native effect record it created, validates owner, effect ID, declaration, epoch and current avatar, and follows the current native left wrist while no callback is present. The finite 1600 ms lease and explicit cancellation retire only owned records. Network kind11 is refused while the [manual test](../../work/natural-moveset/Test-FailureHandFx.ps1) is evaluated. No source first-person attachment, power activation or resource/damage write is added.

Both manual v47 takes subsequently pass six checks: [Corvo](../../work/natural-moveset/outofmana-hand-fx-corvo-v47/result.json), [Emily](../../work/natural-moveset/outofmana-hand-fx-emily-v47/result.json). Each tests three owned native effects, hand movement, expiry, cancellation, health and protected hashes. Viewed temporal sheets show blue smoke near the moving left hand without obvious observer blur. This does not establish exact tattoo placement or an automatic Emily failure binding.

## Automatic native failure effects, v48

[v48 code archive](../../work/movement-integration/candidate-network-failure-fx-v48) retains protocol36 and the v46 D2MOVE41 banks. [Event policy](../../outputs/d2-coop-lab/src/power_failure_fx_policy.h) consumes each validated event once per stream incarnation, refuses old/duplicate IDs, and consumes late events without replay. Invalid timing or missing current ownership cannot trigger an effect. A native start attempt is not retried when its ownership result is ambiguous. The [service](../../outputs/d2-coop-lab/src/power_failure_fx.h) additionally checks the live movement owner, game/world/session, delayed native power sample and matching event. Only observed Corvo semantic15 is enabled. No second native power activation or animation damage event is introduced.

Four paired recordings pass thirteen checks each: [standing armed](../../work/natural-moveset/outofmana-corvo-standing-armed-fx-v48/result.json), [standing empty](../../work/natural-moveset/outofmana-corvo-standing-empty-fx-v48/result.json), [crouched armed](../../work/natural-moveset/outofmana-corvo-crouched-armed-fx-v48/result.json), [crouched empty](../../work/natural-moveset/outofmana-corvo-crouched-empty-fx-v48/result.json). Each has two distinct native failure scenes, two remote body events and exactly two owned native hand effects. Successful casts generate no failure effect. Both effects retire, stance and equipment recover, inventory cleanup passes, health and protected files remain unchanged. Temporal sheets show hand smoke and recovery. Fine hand contact, exact socket alignment, arbitrary interruption/lifecycle cases and continuous visual acceptance remain open.

## Crouched sword-clearance investigation

The actual crouched recordings show close floor clearance. A separate [read-only screen](../../work/natural-moveset/crouch-power-clearance-screen-v49.json) estimates a conservative blade centreline from the native weapon socket/binding. It finds the successful Bend Time crouched cast/end candidates crossing below the body root plane (Corvo roughly -8 cm); this approximation is not a rendered collision measurement. The failed-cast clip itself stays above that plane in the screen, so the visual impression is not generalized into a confirmed failure-clip penetration.

[v49 data candidate](../../work/movement-integration/candidate-crouch-power-clearance-v49) changes only clips239 and245, retaining all249 other clip payloads and the same v48 code. Local forearm pronation and wrist tilt preserve the moving wrist trajectory, native lengths, finger grip and weapon attachment. Dense geometric screening improves the estimated blade height, but Emily's torso-clearance screen remains unresolved. This is a comparison candidate, not a promoted visual fix. [Builder and candidate measurements](../../work/natural-moveset/crouch-power-clearance-bank-manifest-v49.json) and paired game tests preserve the exact source and conversion.

Matched [baseline](../../work/natural-moveset/bendtime-corvo-crouch-clearance-baseline-v49/result.json) and [v49 candidate](../../work/natural-moveset/bendtime-corvo-crouch-clearance-candidate-v49/result.json) each pass eight runtime checks. The candidate is **rejected for promotion**: viewed actual-game frames show the blade raised awkwardly behind the shoulder despite better estimated floor clearance. Numerical screening did not establish good acting. The original v46 bank remains the working baseline.

[v50 guard candidate](../../work/movement-integration/candidate-crouch-power-guard-v50) instead borrows most right-arm orientation from the native low sword guard, retaining part of the donor movement and the animated shoulder. It uses no wrist position target or first-person body anchor. Its two actual banks pass structural validation; runtime and visual results must be read from the subsequent comparison. Both candidate sets remain independently reproducible and reversible.

## Automatic failure FX interruption tests, v51

[Test-FailureFxLifecycle-v51.ps1](../../work/natural-moveset/Test-FailureFxLifecycle-v51.ps1) observes one exact native Corvo failure scene and its live automatic hand effect before interrupting it. The code is still v48, with standard v46 banks. Each of these takes passes eight checks:

- [Explicit receiver stream stop](../../work/natural-moveset/failure-fx-receiver-stop-v51/result.json): owned effect stops exactly once, body ownership retires and no effect replays. Automatic recovery is intentionally off in this diagnostic.
- [Source pause and automatic recovery](../../work/natural-moveset/failure-fx-source-resume-v51/result.json): native menu suspends both banks/streams; pinned banks resume under fresh Steam tokens; old effect does not replay.
- [Observer pause and automatic recovery](../../work/natural-moveset/failure-fx-receiver-resume-v51/result.json): native effect cleanup defers until the observer resumes gameplay; both owners recover and no duplicate effect starts.

All three verify inventory cleanup, both player/avatar health and fourteen protected-file hashes. Actual-game temporal sheets were viewed. Source pause still produces a temporary invisible remote body, followed by a visible pose change as ownership recovers. These are scoped lifecycle passes, not seamless visual acceptance. Death/load/disconnect, arbitrary cast phases, other roles/powers and exact tattoo placement remain open.
