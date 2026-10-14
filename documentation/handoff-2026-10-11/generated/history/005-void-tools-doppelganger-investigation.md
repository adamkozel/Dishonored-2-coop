# Historical note: outputs/d2-coop-lab/analysis/void-tools/doppelganger-investigation.md

Original: [outputs/d2-coop-lab/analysis/void-tools/doppelganger-investigation.md](<../../../../outputs/d2-coop-lab/analysis/void-tools/doppelganger-investigation.md>)

SHA-256: `6C0F42F51F02C5F213B44C2BF1F908BFA3D83318B96B9C87B5CD5AD7DD614ACF`; 25251 bytes.

````markdown
# Native Doppelganger remote-avatar investigation

Updated 2026-10-09. Preserved stable build: **424BE19EA9BF87FD134787D99D1637E2A249CF1A827E51D77D5B28AE333A9B7D**. Remote root interpolation defaults on; native airborne/landing, first-person, sword/action, pause and fixed-checkpoint recovery checks pass. The six-case root A/B report belongs to 8007, before 424B added matching animation delay; strict gait evidence remains attributed to 97D9. Exact Steam executable: B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180.

**Decision (user confirmed 2026-10-09):** native Doppelganger rigs are the primary development approach for both remote players. Earlier story-avatar milestones remain preserved as fallback. Native sword playback and attachments now work without first-person arm/finger retargeting. This remains an opt-in Dunwall experiment; a complete remote player replacement is not validated. Finger-pose tuning remains paused.

## Preservation and rollback

Original Steam assets are read-only. `work/asset-game` is the physical isolated copy; `work/lab-game/base` remains a junction to Steam and must never be an installer target. Normal windowed, side-by-side, low-setting instances and ordinary cutscene speed remain in use.

- `work/native-partner-equipment-6C53`: stable older avatar/equipment milestone and corresponding evidence.
- `work/pre-doppelganger-E6B3`: pre-pivot source, binaries, assets, installed index/patch files and both isolated save profiles.
- `work/doppelganger-native-9193`: native graph clone milestone before action-layer experiments; Corvo locomotion regression passed, Emily failed.
- `work/doppelganger-actions-9D3F`: passing direct-action source, binaries, assets, installed index/patch files and eight-case action report.
- `work/doppelganger-stream-C459`: passing native Steam sword-action stream, protocol tests and pause/reload guard evidence. Assets are unchanged from 9D3F. The later read-only probe fixes/diagnostics are recorded separately.
- `work/doppelganger-locomotion-97D9`: native immediate graph-selection fix, complete source/binaries, current test reports and installed-asset hash manifest. Same seven-resource asset pack as 9D3F.
- `work/doppelganger-recovery-ADB4`: four shared reload/death cases preserve clone identity and exact follow; eight native post-recovery action cases pass. Full source, binaries, scripts, evidence and unchanged installed-asset hash manifest.
- `work/doppelganger-airborne-7D7A`: native airborne/landing playback, matching source/protocol changes, 11-entry action declaration, source/binaries/scripts/evidence and full installed-asset backup.
- `work/doppelganger-smoothing-424B`: buffered root presentation and aligned animation transitions; source, binaries, scripts, evidence and isolated save profiles. Installed assets are byte-identical to the preserved 7D7A backup; manifest records verified hashes and that backup path.
- `work/doppelganger-correct-order-model-override`: intermediate diagnostic pack, superseded by uniquely named models.

Current action models are `models/characters/d2coop/shadow_emily.md6` and `shadow_corvo.md6`. The earlier diagnostic overrides of original head-model resources were removed by restoring the 9193 installed indices before installing the unique variants. Native rigs, meshes, clips and original Steam DLL are reused. No player-facing UI was added.

## Confirmed native resource and lifecycle findings

Decoy/DeadlyShadow definitions inherit full native model/animation data. The thirteen Doppelganger-specific aliases are additions, not a complete player moveset. Emily has 219 joints and Corvo 215. Both native inventories contain the clone Emily sword item and a thrown item; the sword is auto-equipped at joint 17, `right_hand_wpn`.

Base lifecycle Init RVA4a66c0 and Start RVA4a6e40 read local-player attributes and subscribe local-player events. DeadlyShadow Start RVA4a4490 performs target/equipment setup. Its authored combat-expiry timer is eight seconds. Presentation definitions omit DeadlyShadow lifecycle, AICombatAttack, AIReflex, crew and psychic-sensor components before initialization. Ordinary animation, inventory, physics, decision and idle components remain. Scoped native decision/idle holds suppress those latter systems during ownership.

The clones survive beyond eight seconds. This is bounded lifecycle evidence, not a campaign-wide lifecycle guarantee. Immediate AI hold inside spawn is too early for deferred component initialization; later follow acquisition succeeds. This startup gap and AI behavior when follow is released remain open.

## Animation playback and parser correction

Ghidra-confirmed APIs:
- 486d40 / 486bc0: requested velocity / facing.
- 486c50 / 486b00: stance / equipment requests.
- 53c5b0: AnimBasic alias lookup through model+148.
- 53eef0: native AnimBasic playback; 540a00 stop; 53e770 playback state.
- Reflected 24-byte AnimBasicInputData: index, weight, play mode, rate, start time and initialization fields.
- 1c9630: MD6 fixed-order parser; modern locomotion and AnimBasic references must come after required `props` and optional joint metadata.

An incorrectly ordered model declaration produced null model+140/+148 and a null modern locomotion controller. Native foot IK then faulted at RVA396029. A temporary clone-only foot-IK disable allowed inspection; it was removed after the parser-order fix. **Current models retain foot IK and valid controllers.** Failed diagnostics remain under `research/doppelganger-action-null-controller-*.json`; corrected references are recorded in `doppelganger-action-correct-order-*.json` and persistent Ghidra notes.

The isolated graph reuses the native NPC graph and its existing AnimBasic channel. It replaces that channel's additive blend with a gated full-body blend while a bounded clone action is active. No new clips or rigs are authored. The original nine-entry AnimBasic declaration inherited five entries and added sword attack, parry-in, parry-out and idle aliases. The airborne milestone used eleven entries, adding the native airborne and landing aliases described below. The 9165 crouch research pack adds a twelfth native reference alias.

`coop_shadow_action attack|parry|recover|idle|off` is a developer-only command. Parry uses the native hold-last-frame mode with a 3.5-second maximum. Other clips finish normally or expire. Playback requires exact owned clone/model/weapon identities, active AI hold, locomotion/follow, fresh peer, matching sessions/epochs and native gameplay. The resolved named input must match the component's actual channel binding. Local camera, arms and controls are not modified.

## Current runtime evidence

The original 9D3F report, preserved as `research/doppelganger-action-verification-9D3F.json`, passes **8/8** cases: attack, held parry, recovery and idle on both rigs. It verifies observed native command indices, actual right-arm pose changes, stable sword attachment origins, matching weapon native/render roots, network-root ownership, explicit native stop and unchanged local player health. Short clips are sampled immediately after command publication so mailbox receipt timing does not conceal them. Earlier failed sampling evidence is retained in `doppelganger-action-first-pass.json`.

Those eight cases were direct playback on 9D3F. C459 adds real native player-state replication through the existing SteamNetworkingSockets012 connection. The read-only source mapper requires native Slash state 39 (VT RVA1c85d28) or Block state 44 (VT RVA1c81a68), known movement-state combinations, and the exact locally owned drawn sword. Both classes were observed on both characters with ordinary inputs; raw input buttons do not select remote actions.

The 104-byte semantic action protocol exchanges two opt-in tokens and binds both sessions, map/world epochs, root freshness and monotonic packet/event identities. The nonrenewable 15-second experiment applies one native attack per observed slash episode, sustains parry only while fresh block evidence arrives, and plays recovery on release. It stops on stale evidence, lost peer, pause, load, incarnation change or expiry. It sends no damage calls and does not retarget first-person arms.

`research/doppelganger-steam-action-verification-97D9.json` passes six checks with 70 snapshots: both real slash/block sources produce corresponding native remote inputs and changing arm poses; exactly three action applications per receiver (attack/parry/recovery); duplicate packets do not replay actions; one-sided opt-in, withdrawal and expiry stop or refuse playback. `doppelganger-steam-action-guards-97D9.json` passes three additional checks: either native pause cancels held parry, paused rearming is refused, resume does not replay, and restoring both isolated checkpoints retires old-world streams. Both reports were rerun successfully on 97D9 after the graph-switch fix (six stream checks / 70 snapshots and three pause/reload checks). This is bounded same-PC Steam evidence, not internet/two-account validation, directional combat parity, combos or authoritative outcomes.

`research/doppelganger-action-first-person-verification.json` passes on 9D3F: both independently owned 69-joint first-person arm rigs, actual local sword swings, normal Mayhew cutscene and player health 56. The same three checks / 32 snapshots pass on 97D9 in `research/doppelganger-first-person-97D9.json`. During the scene, existing follow ownership correctly retires; automatic clone rearming afterward is not implemented.

The damage firewall ignores hits whose attacker/inflictor resolves to the exact owned clone or its parented equipment, preserving ordinary sources and local-player exemption. Current self-tests again report two blocked native parameter reconstructions, stable health and zero inflicted damage for both roles. The final 97D9 fixture again passes both guard self-tests with local player health 56 and no active action stream. The inspected `sword_attack_fasta` event blocks contain footsteps and a sword sound, with no direct damage event; this does not cover every inherited event or weapon path. **Actual animation-event contact coverage is still pending.**

Close views show native closed-hand sword poses and normal character presentation, with no finger stream. Saved frames: `screens/doppelganger-emily-native-pose-9D3F.jpg` and `screens/doppelganger-corvo-native-pose-9D3F.jpg`. Framing varies; Corvo's held parry raises the sword outside that close frame. Compare cautiously with historical `screens/avatar-hand-grip-DBA7.png`; this is not a matched, exhaustive visual A/B benchmark. Numerical attachment consistency is also present in older 6C53, so alignment alone does not prove complete animation superiority.

## Locomotion findings and coverage

The 9193 strict movement test passed all five stages for Corvo. Emily still failed settled Relaxed/Sword idle: native output speed 0.42112 persists even with exactly zero requested/actual velocity. Sub-5mm/s quantization did **not** fix it. The same test also exposed local-player collision when both representations overlap. More samples improved Corvo's external snapshot coverage without weakening thresholds. These reports do not constitute a full locomotion pass for the current action graph.

Native stance names are Relaxed, Combat, Search and Mesmerize. They are not physical standing/crouch states. The C459 Combat/Sword regression also fails. In the preserved both-role report, Corvo continues producing approximately 1.39-1.44 native output speed at exact zero request, while Emily reports zero root output but about 0.11 foot excursion, above the unchanged 0.08 idle threshold. A later Corvo start/stop sample does return to native Idle, so state history matters; investigation is ongoing. Observer displacement also needs a settled-floor fixture before it can be attributed solely to overlap.

The read-only bone probe previously rejected Emily because it validated unused native offset-record lane 7 as a coordinate. Only lanes 12-14 are consumed as offset XYZ by the reconstruction. Validation now checks those consumed finite values while retaining transform, cache, body and coherence checks; gait/body/root assertions are unchanged. A read-only graph trace exposes active native Idle/Strafe/transition classes and stance/equipment state. The C459 loader was unchanged by those probe-only corrections.

### Native graph selection correction (97D9)

Ghidra and read-only runtime graph snapshots identified a stale pending graph: Combat/Sword was active while Relaxed/Empty remained selected for completion. The scheduled transition could therefore replay movement even at zero requested speed. The adapter now uses the existing native immediate stance/equipment request flags only for the explicitly scoped clone. It does not edit native controller memory directly, global NPC graphs, player controls or Steam files.

`research/doppelganger-combat-locomotion-verification.json` on 97D9: Corvo passes all five unchanged stages. Idle output is zero; foot excursion stays below 0.049, movement gait exceeds 0.82, and disabling the adapter stops all adapter applications while network root following continues. Emily passes movement and body alignment but still fails the unchanged idle-foot limit (about 0.11-0.13 versus 0.08). Both observer positions remain stable in the separated, settled exact-follow fixture; this does not resolve overlapping-player collision.

`research/doppelganger-idle-reference.json` compares graph idle against direct native `sword_idle` playback (confirmed AnimBasic index 8 at weight 1). Emily's direct clip shows approximately 0.11 foot excursion too; authored weight shifting is a supported hypothesis, not a waived assertion. The shorter graph sample spans about 0.06. No claim of universally correct idle or full movement coverage follows from this comparison.

Persistent decompilation and notes: `analysis/ghidra/doppelganger-graph-selection.json`; source-player class evidence: `analysis/ghidra/native-player-sword-actions.json`. Original-save/executable/Steam-DLL integrity passes on the final 97D9 fixture. Final connected roles, inactive action streams and damage self-tests: `research/doppelganger-final-fixture-97D9.json`.

## Shared clone recovery (ADB4)

The checkpoint transaction now captures the explicitly owned presentation and exact-follow mode when the developer session is armed. After both native loads and role restoration complete, it recreates the same clone profile and checks native sword initialization before restoring root-follow ownership. It retains booleans across the boundary rather than actor pointers and refuses an unavailable clone profile instead of substituting the older story avatar. The existing 800ms native initialization interval is unchanged; startup AI behavior in that interval remains a gap.

`research/doppelganger-shared-recovery-ADB4.json` passes four cases on ADB4: host reload with duplicate requests and delayed client, client reload with delayed host, host death/client retry, and client death/host retry. Underlying native reports verify shared death bindings, one coordinated load, canonical SLOT82, role preservation and the both-ready barrier. Independent post-load reads find exactly one intended Doppelganger on each side, the expected unique model and initialized controller/sword, AI and idle holds, new world epochs and exact follow. These are fixed-Dunwall tests, not arbitrary campaign checkpoint or save-transfer coverage.

`research/doppelganger-post-recovery-actions-ADB4.json` then passes all eight direct native attack, parry, recovery and idle cases on the recovered actors. Stable sword attachment errors stay below 0.000009, native/render weapon roots agree, network roots remain controlled and local player health is unchanged. This rechecks native playback after recovery; the separate Steam action lease still needs explicit rearming.

`research/doppelganger-final-fixture-ADB4.json` records the final connected, separated pair with native locomotion and exact follow enabled, health 56 on both local players, and temporary action/body/finger streams and shared-reset arming off. The original executable, Steam DLL and all eight protected save/config files still pass the baseline check. The full source/binary/scripts/evidence milestone is `work/doppelganger-recovery-ADB4`; all nine installed index/patch hashes match 97D9 and the preserved 9D3F asset pack.

Automatic follow recovery after ordinary cinematic boundaries, broader movement coverage and general campaign save/map handling remain open.

## Airborne presentation and smooth root movement (7D7A / 8007 / 424B)

Actual Jump/Fall native classes now select `sword_fall_loop` (AnimBasic index 9, native repeat mode 2). Returning to grounded idle selects one `sword_land_walk_out` (index 10, once). The semantic Steam action protocol is version 2 and retains its role, session, world, token, freshness and replay guards. Airborne presentation takes priority over sword actions; aerial combat parity is not implemented.

On 7D7A, four real standing jump episodes (two per role) passed lower-body pose, native clip identity, sword attachment, network-root and unchanged-observer checks. Both native-pause guards and the six-check sword-stream regression passed. The inherited landing clips contain bark/footstep events; no direct damage event appears in the inspected blocks. Dedicated takeoff, directional/moving landing, crouch landing and complete traversal remain gaps. Initial arming refusals came from pre-Mayhew saves with unusable swords; the normal Mayhew scene is required. An initial client route stood on furniture and produced another real fall; the preserved flat-floor route corrected the fixture without relaxing assertions.

The user's report of low-Hz body translation was confirmed in the root path: accepted network snapshots directly replaced the owned native transform. The transport sends on a coarse tick-based 33ms threshold, and follow previously ran inside the 10ms service gate. Interpolation now buffers 80ms of receiver-time snapshots, evaluates XYZ and shortest-arc quaternion orientation using QPC on every native command-engine callback, and uses the same sampled frame for the owned native actor, rendering and locomotion orientation. It does not change local controls, the player camera, game time or native Steam transport.

`research/pose-smoothing-verification.json` on 8007 passes all six off/on comparisons: horizontal, diagonal and vertical input for both roles. Raw movement updates measured 17.9-22.0/sec; smoothed native/render transforms measured 153-547 updates/sec depending on the engine callback rate. These are transform-write measurements, not a claim about monitor refresh or rendered FPS. Native/render targets match (zero recorded error), settling reaches the latest network position, and the local observing player stays still. Full per-callback traces are the twelve `pose-cadence-*.json` reports.

The presentation buffer never extrapolates. World/session/map replacement, >250ms gaps and >3m position discontinuities reset history; pause/load/follow guards still retire ownership. Unit checks cover XYZ continuity, shortest rotation, rigid transforms, loss, replay, teleport and scope resets. `coop_probe_smoothing off` retains raw playback for developer A/B tests; smoothing defaults on. Very short teleports, internet jitter and long-fall/high-speed discontinuity tuning are not yet covered.

424B keeps the identical root interpolation and delays whitelisted remote animation transitions by the same 80ms, preventing immediate landing playback ahead of buffered root motion. Four airborne cases, the three-check/32-snapshot local first-person regression, six sword-stream checks/70 snapshots and both airborne pause-cancellation guards pass on 424B. See `research/doppelganger-airborne-424B.json`, `doppelganger-first-person-424B.json`, `doppelganger-sword-stream-424B.json` and `doppelganger-airborne-guards-424B.json`. More external skeleton samples were collected at the higher transform rate; the unchanged coherence and pose thresholds remain enforced. Initial insufficient-coherent-sample reports are preserved; they are not counted as passes.

The final 424B direct-action check passes all eight cases with 21-32 coherent pose/attachment samples per case (`research/doppelganger-native-actions-424B.json`). Both shared reload directions and both natural-death/native-retry directions pass with new world epochs and restored native clones (`doppelganger-recovery-424B.json`). Original executable, Steam DLL and eight protected save/config files match the baseline. The final fixture has one correct clone per role, healthy local players, exact native follow, smoothing on and experimental action/reset streams off (`doppelganger-final-fixture-424B.json`). This is the isolated Dunwall fixture, not a complete campaign or online release validation.

Crouch asset search found native player-body `ply_body_generic_sneak_*` / `crouch_*` clips, while the inspected female/tall NPC locomotion sets lack dedicated crouch locomotion. The subsequent complete-body investigation is recorded below; lower-body-only presentation was explicitly rejected. No first-person retargeting or finger tuning was added.

Persistent native notes: `analysis/ghidra/doppelganger-airborne-and-presentation.json`, comments at AnimBasic playback 0x53eef0 and native transform dispatch 0x87d200, and the saved exact-executable Ghidra project. Airborne asset/event evidence: `analysis/void-tools/doppelganger-airborne-assets.json`.

| Action/system | Current evidence | Remaining |
|---|---|---|
| Movement speed/direction | 80ms QPC root interpolation passes all six horizontal/diagonal/vertical A/B cases; Corvo five-stage gait regression passes on 97D9, Emily movement/body checks pass | Emily idle assessment, overlap collision, broader speeds/directions and stance-transition histories |
| Sword grip/equipment | Native auto-equip, hand attachment and native action poses; eight-case attachment test passes | Draw/holster replication for clone scope; full visual A/B |
| Attack/parry | Real native Slash/Block state drives both rigs through Steam; attack/parry/recovery and duplicate/expiry/pause/load checks pass | Directional variants, combos, parry success, actual contact and authority integration |
| Crouching | 9165 opt-in whole-body reference plus player stepping passes both roles crouch/walk/stand, pause and old-reference rejection after reload | Low sword-hand and Emily torso posture; crouched combat, directional/terrain quality, collision and automatic setup |
| Jumping | Native Jump/Fall state drives inherited airborne loop and one landing clip over Steam; four standing episodes pass for both rigs | Dedicated takeoff, directional/moving and crouch landing, aerial combat and traversal parity |
| Sliding/climbing/leaning | No verified native clone mapping | Identify native equivalents or scoped supplementation |
| Powers | Doppelganger cast/teleport aliases are present | Each real player power and its outcomes remain unverified |
| Expiry/effects | Lifecycle removed; opaque normal presentation and survival beyond native timer observed | Long-run transitions, remaining sword material effects and startup AI gap |
| Damage | Owned clone/equipment firewall self-tests pass | Actual animation-event hit tests and duplicate-outcome integration |
| Recovery/scenes | 424B: both reload directions and both death/retry directions preserve native clone identity, exact follow and swords at canonical Dunwall SLOT82; earlier ADB4 post-recovery action evidence remains preserved | Arbitrary checkpoints/save transfer, automatic action-lease rearming and clone follow after cinematic boundaries |

Native game UI and native Steam networking remain requirements. Real two-account online sessions, full world/AI/progression consistency and the wider campaign remain unfinished. This work does not claim playable complete co-op.

## Complete crouch follow-up (9165 research)

The user rejected lower-body-only crouching. The current experiment captures a native whole-body crouched reference, including torso/head and arms/hands, and combines it with replicated player pelvis/stepping. Both roles pass completed-frame crouch/walk/stand checks, plus both pause directions and stale-reference rejection after checkpoint restoration. It remains opt-in: the search-derived sword hand is too low and Emily bends too far. Full crouched combat and directional/terrain quality are unverified. Stable424B remains preserved. See [the complete crouch investigation](doppelganger-crouch-investigation.md) for exact builds, failed trials and the native cache-stage observation correction. The isolated research asset pack now has twelve AnimBasic entries; the eleven-entry rollback is preserved.

## Optional REA tooling (unchanged)

REA was reviewed, not installed. Its documented Windows boundary duplicates static Ghidra reads and does not add the Windows runtime capture needed here. Existing Ghidra MCP supplied the decisive parser decompilation and persistent annotations. See `../ghidra/rea-assessment.md`.

````
