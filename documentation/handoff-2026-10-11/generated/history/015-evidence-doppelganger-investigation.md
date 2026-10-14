# Historical note: work/doppelganger-locomotion-97D9/evidence/doppelganger-investigation.md

Original: [work/doppelganger-locomotion-97D9/evidence/doppelganger-investigation.md](<../../../../work/doppelganger-locomotion-97D9/evidence/doppelganger-investigation.md>)

SHA-256: `17A526622B9E87E962052978080A98C25F9A93D4FEAF14CB4279720EEF901DE9`; 15409 bytes.

````markdown
# Native Doppelganger remote-avatar investigation

Updated 2026-10-09. Current tested Steam action / locomotion build: **97D9EC98AA3882B26FE04F0FDF4F2039487CCAC17B1DD73971CC63DAEEAD0BE6**. Exact Steam executable: B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180.

**Decision:** continue with native Doppelganger rigs as the preferred animation candidate. Native sword playback and attachments now work without first-person arm/finger retargeting. This remains an opt-in Dunwall experiment; a complete remote player replacement is not validated. Finger-pose tuning remains paused.

## Preservation and rollback

Original Steam assets are read-only. `work/asset-game` is the physical isolated copy; `work/lab-game/base` remains a junction to Steam and must never be an installer target. Normal windowed, side-by-side, low-setting instances and ordinary cutscene speed remain in use.

- `work/native-partner-equipment-6C53`: stable older avatar/equipment milestone and corresponding evidence.
- `work/pre-doppelganger-E6B3`: pre-pivot source, binaries, assets, installed index/patch files and both isolated save profiles.
- `work/doppelganger-native-9193`: native graph clone milestone before action-layer experiments; Corvo locomotion regression passed, Emily failed.
- `work/doppelganger-actions-9D3F`: passing direct-action source, binaries, assets, installed index/patch files and eight-case action report.
- `work/doppelganger-stream-C459`: passing native Steam sword-action stream, protocol tests and pause/reload guard evidence. Assets are unchanged from 9D3F. The later read-only probe fixes/diagnostics are recorded separately.
- `work/doppelganger-locomotion-97D9`: native immediate graph-selection fix, complete source/binaries, current test reports and installed-asset hash manifest. Same seven-resource asset pack as 9D3F.
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

The isolated graph reuses the native NPC graph and its existing AnimBasic channel. It replaces that channel's additive blend with a gated full-body blend while a bounded clone action is active. No new clips or rigs are authored. The nine-entry AnimBasic declaration inherits the five existing entries and adds sword attack, parry-in, parry-out and idle aliases.

`coop_shadow_action attack|parry|recover|idle|off` is a developer-only command. Parry uses the native hold-last-frame mode with a 3.5-second maximum. Other clips finish normally or expire. Playback requires exact owned clone/model/weapon identities, active AI hold, locomotion/follow, fresh peer, matching sessions/epochs and native gameplay. The resolved named input must match the component's actual channel binding. Local camera, arms and controls are not modified.

## Current runtime evidence

`research/doppelganger-action-verification.json` passes **8/8** cases: attack, held parry, recovery and idle on both rigs. It verifies observed native command indices, actual right-arm pose changes, stable sword attachment origins, matching weapon native/render roots, network-root ownership, explicit native stop and unchanged local player health. Short clips are sampled immediately after command publication so mailbox receipt timing does not conceal them. Earlier failed sampling evidence is retained in `doppelganger-action-first-pass.json`.

Those eight cases were direct playback on 9D3F. C459 adds real native player-state replication through the existing SteamNetworkingSockets012 connection. The read-only source mapper requires native Slash state 39 (VT RVA1c85d28) or Block state 44 (VT RVA1c81a68), known movement-state combinations, and the exact locally owned drawn sword. Both classes were observed on both characters with ordinary inputs; raw input buttons do not select remote actions.

The 104-byte semantic action protocol exchanges two opt-in tokens and binds both sessions, map/world epochs, root freshness and monotonic packet/event identities. The nonrenewable 15-second experiment applies one native attack per observed slash episode, sustains parry only while fresh block evidence arrives, and plays recovery on release. It stops on stale evidence, lost peer, pause, load, incarnation change or expiry. It sends no damage calls and does not retarget first-person arms.

`research/doppelganger-steam-action-verification.json` passes six checks with 70 snapshots: both real slash/block sources produce corresponding native remote inputs and changing arm poses; exactly three action applications per receiver (attack/parry/recovery); duplicate packets do not replay actions; one-sided opt-in, withdrawal and expiry stop or refuse playback. `doppelganger-steam-action-guards.json` passes three additional checks: either native pause cancels held parry, paused rearming is refused, resume does not replay, and restoring both isolated checkpoints retires old-world streams. Both reports were rerun successfully on 97D9 after the graph-switch fix (six stream checks / 70 snapshots and three pause/reload checks). This is bounded same-PC Steam evidence, not internet/two-account validation, directional combat parity, combos or authoritative outcomes.

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

| Action/system | Current evidence | Remaining |
|---|---|---|
| Movement speed/direction | Steam velocity/facing drives native gait and network root; current Corvo five-stage regression passes, Emily movement/body checks pass | Emily idle assessment, overlap collision, broader speeds/directions and stance-transition histories |
| Sword grip/equipment | Native auto-equip, hand attachment and native action poses; eight-case attachment test passes | Draw/holster replication for clone scope; full visual A/B |
| Attack/parry | Real native Slash/Block state drives both rigs through Steam; attack/parry/recovery and duplicate/expiry/pause/load checks pass | Directional variants, combos, parry success, actual contact and authority integration |
| Crouching | Actual crouch inputs and native stance/motion received in the 148-observation source-action capture; clone graph stances are different | Clone-specific crouch representation and collision |
| Jumping | Actual player jump/fall states 2/3 are observed and transmitted; native graph has jumpData and inherited aliases | Real airborne/landing clip mapping; do not equate root height with correct jump animation |
| Sliding/climbing/leaning | No verified native clone mapping | Identify native equivalents or scoped supplementation |
| Powers | Doppelganger cast/teleport aliases are present | Each real player power and its outcomes remain unverified |
| Expiry/effects | Lifecycle removed; opaque normal presentation and survival beyond native timer observed | Long-run transitions, remaining sword material effects and startup AI gap |
| Damage | Owned clone/equipment firewall self-tests pass | Actual animation-event hit tests and duplicate-outcome integration |
| Recovery/scenes | Preserved isolated restore works; first-person/cutscene regression passes | Automatic clone respawn/rearm across shared resets and scene boundaries |

Native game UI and native Steam networking remain requirements. Real two-account online sessions, full world/AI/progression consistency and the wider campaign remain unfinished. This work does not claim playable complete co-op.

## Optional REA tooling

REA was reviewed, not installed. Its documented Windows boundary duplicates static Ghidra reads and does not add the Windows runtime capture needed here. Existing Ghidra MCP supplied the decisive parser decompilation and persistent annotations. See `../ghidra/rea-assessment.md`.

````
