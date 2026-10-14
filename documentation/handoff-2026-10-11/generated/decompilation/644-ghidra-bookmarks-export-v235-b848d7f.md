# ghidra-bookmarks-export-v235.json

Original: [work/ghidra-bookmarks-export-v235.json](<../../../../work/ghidra-bookmarks-export-v235.json>)

SHA-256: `CE316973F05255C65C37F607E1A12A10BE420488651A5ABBFA56A8FC7D3FC380`; 92767 bytes.

## Original saved record

````json
[
  {
    "address": "14023fca0",
    "category": "D2 Co-op confirmed runtime",
    "comment": "2026-10-10 Corvo calf/boot preview defect: exact sword_walk_e_add frame17 final publication (isolated CF60, exact executable B671F824) shows helpers196/203 keep bind-local translation under tibia20/24 with small axial rotation. Old offline preview improperly followed ankle21/25 including foot-pivot translation, displacing calf vertices. Same anatomy/weights/camera comparison across walk, crouch, jump and carry release: mean calf skin discrepancy falls 3.65–8.56cm to 1.84–3.37mm when helper bind-local anchoring is restored. Offline approximation still omits native axial twist. No native game code or skin weight edit. Evidence work/natural-moveset/boot-seam-v1/validation.json, comparison-grid.jpg and boot-seam-native-test-1. Do not treat corrected preview as complete movement/state acceptance.",
    "type": "Note"
  },
  {
    "address": "14087e9e0",
    "category": "D2 Co-op static hypothesis",
    "comment": "Movement/VFX integration 2026-10-10: static trace from arkCpntBreakable::SpawnFX diagnostic (string 141c71ba0, caller 1407ee610) reaches this wrapper. It obtains game vtable+0x388 manager, calls 14019d100 with entity handle (+0xa8) and supplied declaration, copies entity transform (+0x300/+0x30c), then dispatches 1401a2ff0. Candidate entity-owned FX allocation/start path. No runtime call or remote power/VFX implementation verified. Ownership, declaration type, end/cleanup and viewer visibility must be resolved before use. Exact resources/evidence in work/movement-integration; no game memory or assets changed by this analysis.",
    "type": "Note"
  },
  {
    "address": "141c85948",
    "category": "D2 Co-op runtime confirmed",
    "comment": "Exact B671F824 build: native arkPlayerStateSprinting vtable, ID1. Independently observed ACTIVE on Corvo host and Emily client during bounded forward/left/right diagonal virtual-controller sprints, 2026-10-10. Evidence work/natural-moveset/native-sprint-observation-v1/states.json (55 sprint samples, 93 running recovery). Separate ground motion vs sprint presentation; not proof of all overlays, upgrades or animation contacts.",
    "type": "Note"
  },
  {
    "address": "14085dcc0",
    "category": "D2 Co-op runtime correlated",
    "comment": "Leaning update vtable for native state18, exact B671F824. Static code writes state+0x40 signed lateral, +0x44 vertical, +0x50 turn scale before camera call1407747e0. Read-only runtime observations both Corvo/Emily: lateral negative under left input, positive under right, upward becomes1. Evidence work/natural-moveset/native-context-observation-v1. Lean persists between some trials; isolated release/collision scaling are not yet confirmed. Never write these local camera fields for remote animation.",
    "type": "Note"
  },
  {
    "address": "14086ef50",
    "category": "D2 Co-op movement research",
    "comment": "Exact B671 executable: Sliding ID7 VT141c86108, observed activation both roles native-context-observation-v1. Update string confirms arkPlayerStateSliding::Update. Static fields: +CC remaining/max travel bound, +D0 accumulated distance, +E0 speed, +F0 next native state, +F4 exit started, +E8 exit time. Runtime field semantics still unverified. Calls native movement, camera/FP and state transitions; remote presentation must not call this function or duplicate gameplay. Full decompilation saved movement-integration/slide-14086ef50.json.",
    "type": "Note"
  },
  {
    "address": "140867b70",
    "category": "D2 Co-op movement research",
    "comment": "Jumping ID2 VT141c85028 entry. Static: +9C elapsed reset to0; +94 impulses available then decremented by 1408680A0; +A0 start Z, +A4 target apex Z updated by impulse helper. Update140868200 increments +9C with game delta and retires to Falling3 or ground Running0/Sneaking6/Sprint1. Read-only live correlation required before packet fields. Full decompilations saved work/movement-integration/jump-*.json.",
    "type": "Note"
  },
  {
    "address": "140867b70",
    "category": "D2 Coop Confirmed",
    "comment": "Exact Steam EXE SHA256 B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180. Jumping state ID2 VT RVA1c85028. Read-only native-jump-timing-v1/states.json confirms both Corvo and Emily standing/held/crouched/moving native jumps: elapsed+9c increments; startZ+a0 matches native floor root66.4036484; targetZ+a4 is apex bound67.9036484; remaining+94=0 in Dunwall fixture. Entry and impulse static analysis explain these fields. Remote authored rise uses delayed network root height relative to native start/target, never calls this FP/physics entry. Double jump/extra remaining impulses are not runtime validated.",
    "type": "Note"
  },
  {
    "address": "1408549c0",
    "category": "D2CoopConfirmed",
    "comment": "Confirmed exact retail B671F824 in native-combat-timing-v3-reader: all 201 active sword-state observations across Corvo/Emily resolve state+0x24 to current Midnight scene; right/left variants, Focus In/Loop/Out, standing/crouched Parry. Strict reader requires local player+right-arm reciprocal bindings (ParryIdle right arm alone). Combat bank v1-test: 16 native-input/Steam/full-body checks passed. Appearance, hit/contact reactions and air combat remain unaccepted. Evidence work/natural-moveset; no FP or damage writes.",
    "type": "Note"
  },
  {
    "address": "141c79cf8",
    "category": "D2CoopConfirmed",
    "comment": "Exact retail B671F824: native-equipment-timing-v2 confirms state53 EquipChange under actual X hold toggles, both Corvo/Emily and standing/crouched. state+24 resolves sword_equip / sword_unequip / _sneak Midnight scene; Scene_2 Corvo, Scene_1 Emily. Bindings are owned right-arm and actual local sword entity, not body+arm like ordinary Slash. Undraw81 (+34 hold timer) and Draw54 trigger equipment change; their +24 observed0. State53 is the timing source. Semantic replication and remote equipment handoff not yet implemented.",
    "type": "Note"
  },
  {
    "address": "140619640",
    "category": "D2CoopConfirmed",
    "comment": "2026-10-10 exact B671F824: native-shadow-equipment-v6 runtime passed 10 checks on isolated Corvo/Emily Doppelganger definitions. Existing inventory entry unequips to holster joint195, Start/Stop lifecycle has a brief transitional value requiring bounded read retry; 614b20 re-equips same sword to joint17. Both player health56 and clone health40 unchanged; local FP equipment unchanged. Duplicate request cannot renew 2.5s lease; expiry/cancel restore. DLL3364AAE2 preserved in movement-integration/verified-equipment-3364. Does not establish network timing or visual contact. Current isolated clones omit registered AIAttack; absence confirmed by component list, not failed snapshot.",
    "type": "Note"
  },
  {
    "address": "14087e9e0",
    "category": "D2Coop static FX research",
    "comment": "Static evidence: caller 1407ee610 emits 'arkCpntBreakable::SpawnFX. Failed to spawn FX!' on null return. This function allocates through game vtable+388 -> 14019d100, copies actor transforms/velocity into effect record, calls 1401a2ff0. Other wrapper 14087eb40 uses actor+398. 14019d100 allocates 0x140-byte records keyed by actor handle and stores definition at record+8. Exact definition validation, lifecycle/stop API and owned power-effect runtime behavior remain UNCONFIRMED; no runtime FX call made. Evidence work/movement-integration/fx-manager-14019d100.json and fx-caller-1407ee610.json.",
    "type": "Note"
  },
  {
    "address": "14085eaf0",
    "category": "D2Coop confirmed low mantle",
    "comment": "Exact Steam executable. Native mantle state8 VT RVA1c82f68 entry; selects midnight scene from kind +D8/next+B8; scene handle+24, goal+BC,start+E8. Runtime Dunwall desk-left: kind1 next0, mantle_low_idle.midnightscene Scene_1 500ms, clock instance+160; owns actual player, optionally separately owned right_arm_2 (both binding orders accepted). Optional arm verified low-mantle-v2-test and mantle-extra-binding-arm.json: reciprocal parent1, correct body/arm defs. Other variants unverified. Candidate presentation intentionally excludes gameplay events, network root remains authoritative; ledge contact not yet visually accepted.",
    "type": "Note"
  },
  {
    "address": "14085fd40",
    "category": "D2Coop mantle equipment recovery",
    "comment": "Deactivated mantle sends request0x46 to equipment state0x35 (53) when flag118&1. Runtime low-mantle-v5-contact-test captured sword_equip_fast.midnightscene Scene_2 500ms on Corvo, binding one reciprocal owned right arm133399. Distinct from ordinary arm+sword equip. Candidate action protocol v10 semantic equipment5 retains observed weapon state with full-body climb recovery, no artificial hip reach. Native original functions remain untouched. Emily exact fast scene duration not yet runtime verified.",
    "type": "Note"
  },
  {
    "address": "14085eaf0",
    "category": "D2Coop confirmed crouched mantle",
    "comment": "Native input observations native-traversal-variants-{corvo,emily}-v1 plus native-crouch-mantle-{corvo,emily}-unarmed-v1 confirm state8 kind+D8=1 next+B8=6: mantle_low_sneak.midnightscene, Corvo Scene_2 / Emily Scene_1, 500ms. Binds actual player and owned right arm, both armed/unarmed. Goals/start differ from standing jump-to-mantle: normal crouch rise~1.3085m, span~1.22m. Candidate v13 full-body bank113/114 ends in crouch; runtime binding not yet validated. Void export movement-native-traversal-v1 preserves exact decls; do not infer other variants from resource presence alone.",
    "type": "Note"
  },
  {
    "address": "14085eaf0",
    "category": "D2CoopConfirmed",
    "comment": "Exact Steam B671F824: observed Dunwall state8 kind+D8=1,next+B8=0 standing mantle_low_idle Scene_1, and next=6 crouched mantle_low_sneak Corvo Scene_2/Emily Scene_1,500ms. Bindings actual player alone OR actual player plus exact owned right arm; player-only crouch captured in crouch-mantle-v13-test. Character from exact player body.def; arm optional. Full runtime acceptance pending fixed reader. No implication of high mantle/vault support.",
    "type": "Note"
  },
  {
    "address": "14085fd40",
    "category": "D2CoopConfirmed",
    "comment": "Mantle deactivation native recovery: standing sword_equip_fast 500ms, crouched sword_equip_sneak_fast 333ms. Both observed Corvo Scene_2 and Emily Scene_1, exact owned right arm only, grounded state0/6 plus EquipChange53. Evidence low-mantle-v5-contact-test; crouch-mantle-v13b-test; native-crouch-recovery-emily-v2. Presentation should retain climb recovery and actual equipped state, not play ordinary hip equip reach.",
    "type": "Note"
  },
  {
    "address": "140872490",
    "category": "D2CoopStatic",
    "comment": "Static-only vault scene start: selector140872080 picks declared normal/high list, optional WINDOW-tag scene. 854be0 main flags7 or5; optional left arm scene flags2. state+24 nonzero =>phase+38=1. 140871950 dispatches native breakable damage to sensor handles; NEVER replay this activation routine merely for remote animation. Use pose-only presentation and retain source gameplay authority. Runtime activation still pending.",
    "type": "Note"
  },
  {
    "address": "140872490",
    "category": "D2CoopConfirmed",
    "comment": "Exact B671F824 runtime native-vault-trigger-corvo/emily-v4 and unarmed-v5 confirm state9 VT1c86e18, phase+38 0/1/2, handle+24, start+58 point+64 goal+70 height+7c. Vault_1/_2 scenes933ms, Corvo Scene_2 / Emily Scene_1, exact player and both owned arms. Runtime scene active~450ms cut at end plane; geometry progress used for remote pose. Native activation invokes breakable damage: never invoke for remote presentation. Reader passed28/28 active samples in corvo-v6. Other vault/window/power variants and contact generalization remain hypotheses.",
    "type": "Note"
  },
  {
    "address": "140618de0",
    "category": "D2CoopStatic",
    "comment": "Native inventory SetQuantity: lookup existing 56-byte entry by declaration at inventory+38/+40, set via63cc00, QuantityChanged6170b0. Called by native PlayerLoadout613ff0. Exact executable static trace. Bounded runtime test pending. Generic Kiscule List AddItem1407abb20 is NOT an inventory grant.",
    "type": "Note"
  },
  {
    "address": "140618de0",
    "category": "D2CoopConfirmed",
    "comment": "B671 exact executable: native SetQuantity with declaration/quantity followed by614b20/619640 confirmed in isolated Dunwall. 5F02 DLL27 checks: Corvo Blink and Emily FarReach0->1 selected->0 unselected; duplicate refused, timeout and pause/resume cleanup; other quantities, sword, health and protected save hashes unchanged. Actual native casts recorded. Does NOT certify remote powers/VFX or restore discovery metadata; no saves written.",
    "type": "Note"
  },
  {
    "address": "14087cb90",
    "category": "D2CoopStaticConfirmed",
    "comment": "Native entity FX creation/start. ABI verified by assembly: entity,outID,unused3,unused4,unused5,FXdecl,optionalOrigin,optionalAxis,u64,u32,u64,u64. Record first fields ownerHandle+0,FXid+4,decl+8; manager Game+1f8430 via VT+388 getter8a8a70. Explicit origin/axis clears flag4 at+138. Native lookup1a04a0 and stop1a31f0 are manager-wide, NOT entity-owner-scoped; require record owner/ID/decl/world validation. Local experimental wrapper native_power_fx.h, runtime verification pending.",
    "type": "Note"
  },
  {
    "address": "140858010",
    "category": "D2CoopConfirmed",
    "comment": "Blink arrival restore: state+a4 bit0 guards completion; native PhysicsSetPosition uses state+54 foot goal. Active travel target+60 is eye-height goal, +6c is initial eye-height origin. Correlated in isolated actual cast integrated-power-corvo-v3. Remote read-only correction and endpoint/VFX stream in native_player_blink.h/power_fx_stream.h. 3EC71 build integrated-blink-fx-v1 passes 8 checks including no eye-height jump, endpoint max error .003051m, two FX, native cancellation none, duplicate rejection. Standing armed only; complete power lifecycle/variants pending.",
    "type": "Note"
  },
  {
    "address": "14087cb90",
    "category": "D2CoopConfirmed",
    "comment": "Runtime verified exact isolated executable build3EC71: native finite Doppelganger Blink departure/arrival FX, owner/id/declaration record validation, automatic expiry and explicit cancellation. 8 integration + 11 stream-stop/pause lifecycle checks pass; records work/natural-moveset/integrated-blink-fx-v1, blink-fx-stream-stop-v1, blink-fx-pause-v1. Not general power replication.",
    "type": "Note"
  },
  {
    "address": "140776490",
    "category": "D2CoopStaticConfirmed",
    "comment": "Updates native Far Reach beam hand from FP joint query; advances e80 endpoint toward e74 target at speed e8c; writes owned FX record+38 and+80. Do not call on a remote avatar; prototype uses source native progress and remote authored left-wrist endpoint. FX record callback is release notification, not endpoint update.",
    "type": "Note"
  },
  {
    "address": "140774d90",
    "category": "D2CoopConfirmed",
    "comment": "Exact EXE B671F824. Native Far Reach tether start, body+e90 owned FX record, record+38 hand and +80 end. Build 8E837631 integrates native effect through scoped Steam semantic protocol v1 while updating start from authored remote left wrist after root presentation every native frame. integrated-grapple-fx-v3 8 checks; grapple-fx-stream-stop-v2 5; pause-v2 6; combat-v15-regression-v1 16. Strict completed-pose/active-registry read snapshots match fast pull at 0m; one recovery boundary remains 0.114m. Standing armed Dunwall scope only; not complete power/movement coverage.",
    "type": "Note"
  },
  {
    "address": "14054f500",
    "category": "D2 co-op movement",
    "comment": "CONFIRMED STATIC exact Steam exe B671F824: evaluates graph then 540a90(component,0) before native feet/IK and final phase3. Candidate place for authored remote body, preserving native foot correction. Runtime effectiveness unverified.",
    "type": "Note"
  },
  {
    "address": "140549340",
    "category": "D2 co-op movement",
    "comment": "CONFIRMED STATIC+LIVE: feetIKRunTime embedded AnimEvolved+2d08, count+2d10, each FootIK stride280. Live isolated Emily has2, weights current/target1. Reflection runtime+28 current,+2c target,+30 velocity. Constructor decl+98 foot config. Native IK candidate only.",
    "type": "Note"
  },
  {
    "address": "14054bc00",
    "category": "D2 co-op movement",
    "comment": "CONFIRMED STATIC: called by54f9f0 before phase1/2 publication; gathers 2 native foot results, prepares pelvis/reference-frame correction on IKobject component+470. Do not call independently with stale casts.",
    "type": "Note"
  },
  {
    "address": "140550a10",
    "category": "D2 co-op movement",
    "comment": "CONFIRMED STATIC: weight selection54be90, weight stepping550e20, foot raycast updater398de0 per2d08 array. Casts are asynchronous. No remote gameplay calls proposed.",
    "type": "Note"
  },
  {
    "address": "140550970",
    "category": "D2 co-op movement",
    "comment": "CONFIRMED STATIC exact Steam: native54f9f0 prepares IK, 53bcd0(...,0xf), publish phase1; 53bcd0(...,0x10), phase2. Candidate now applies authored owned clone bank at phase0 instead of phase3 via experimental toggle, so native feet pipeline may remain. No extra IK calls or AI writes. Runtime not yet accepted.",
    "type": "Note"
  },
  {
    "address": "140397fe0",
    "category": "D2 co-op movement",
    "comment": "CONFIRMED STATIC: reads authored phase0 ankle through53d6a0; ifFootIK+9c==2 OR flags+270 bit4, uses current ankle instead of native graph anticipated planted0/1 and foot+c4. Calls native terrain results/IK task registration. Candidate scoped wrappers on397fe0/398de0 temporarily OR4 for exact owned active authored clone, restorebit onreturn. Not promoted; gameplay test pending.",
    "type": "Note"
  },
  {
    "address": "14054bbc0",
    "category": "D2 co-op movement",
    "comment": "CONFIRMED STATIC 2026-10-11: entity87d200 sends this AnimEvolved VT+b0 callback EVERY network frame, regardless of resetVelocity. Unconditionally54e8c0 zeros feetRuntime+c/+14 pelvis history and397700 resets foot+244 to source pose. Existing network-driven actor is being teleported via notification eachframe, disrupting IK smoothing. Candidate suppresses callback ONLY exact active authored owned remote component, ground-state, <=25cm/frame and facing delta<20deg; actual relocations/other actors/native defaults retained. Runtime quality pending.",
    "type": "Note"
  },
  {
    "address": "140396610",
    "category": "D2 Co-op native feet",
    "comment": "CONFIRMED static: FootIK VT+0 effector reference callback. If foot state+9c==2 or flags+270&4, output source effector Z=0; otherwise uses source joint Z+offset88 minus retained gait c4. Current-pose native solve/cast (397fe0/398de0) requires this callback to use the same branch during later IK stage. Runtime v5 candidate pending. v4 explicit target height was rejected (double-counted authored ankle height).",
    "type": "Note"
  },
  {
    "address": "14016aee0",
    "category": "D2-coop-confirmed",
    "comment": "Exact B671: generic IK solver used by AnimEvolved native feet. 16b270 iterative chain solve;16b750 dispatch(type0:16b940,type1:16b890);16dd40/16dcf0 update chain joints with16d010. Runtime v19 crouched turning shows final knee departures up to31cm although ankle target near unchanged. Internal knee/pole control unresolved; no solver writes/hooks made. Investigating post-solve pole preservation restricted to owned remote presentation.",
    "type": "Note"
  },
  {
    "address": "140541110",
    "category": "D2-coop-confirmed",
    "comment": "Exact B671 helper evaluation precedes final phase3 publish540a90. Native IK can rotate authored crouch knee pole up to92deg while retaining ankle targets. Remote-only correction712205BC (candidate-knee-pole-v19-v4) caches authored primary matrices at owned phase0; rotates final native hip/knee about hip-ankle axis at SAME owned phase3, retains native ankle matrix/local lengths then9cfa0/9dc40/23fca0. Both native rigs have pelvis18(parent2), femur19/23(parent18), tibia20/24, ankle21/25. IMPORTANT540a90 invoked for every actor: filter actor/component BEFORE consuming cached reference. v1-v3 guards refused and made no correction; v4 Corvo/Emily drawn+holstered six-turn tests pass,24completeposes, knee pole errors<.001deg/endpoints<1e-6m. Corvo stance transitions pass. Flat-floor only so far; original IK mm endpoint residual and complete-movement acceptance separate.",
    "type": "Note"
  },
  {
    "address": "14085eaf0",
    "category": "D2 co-op movement",
    "comment": "Exact B671F824 runtime2026-10-11: canopy north[-30.7,-153.6,eye68.1,yaw270] triggers state8 kind2. Corvo high standing mantle_idle Scene_2 duration833ms. IMPORTANT correction: bound arm handle67862 is LEFT arm, verified name left_arm_1, definition corvo/left_arm.def, reciprocal child, in high-mantle-research/host-arm-identities.json and failed recorded v20-v1. Earlier right-arm attribution was a mistaken inference from handle. Low front canopy kind1 has right/body/left three bindings. Runtime-coverage-v1.json pins16 high scene observations; authored remote acceptance pending.",
    "type": "Note"
  },
  {
    "address": "14085f1b0",
    "category": "D2 co-op movement",
    "comment": "Exact B671F824 runtime2026-10-11,16 canopy high-climb cases/158scene samples: kind2 +B8 next0or6 -> mantle_idle/mantle_sneak,833ms, CorvoScene_2/EmilyScene_1. Body plus one bound arm seen. Correction from subsequent identity probe: Corvo bound arm is LEFT support arm, not right (handle is not side evidence). Reader v20-v1 rejected this valid identity and was retained as failed. v20-v2 requires owned LEFT for single-arm high; ownedRIGHT for single-arm low, or both unique validated arms. Stand/crouch/holster repeat and visual playback still required.",
    "type": "Note"
  },
  {
    "address": "1408680a0",
    "category": "D2Coop Movement",
    "comment": "Confirmed static (B671F824): native jump impulse increases absolute target apex state+A4 by state+90 and decrements remaining count+94. Called both on jump entry 140867B70 and jump update140868200. Candidate second-impulse signal is decreasing +94 with increasing+A4; exact installed agility config/runtime second jump NOT yet confirmed. Never infer double-jump availability from this generic implementation.",
    "type": "Note"
  },
  {
    "address": "140867b70",
    "category": "D2Coop Movement",
    "comment": "Confirmed static: jump entry reads attribute index+BC from player's attributes table; unequal effective/base enables decl+168 super-height and +16C successive count, then sets starting/rootZ+A0/A4 and consumes first impulse via8680A0. Exact Void exported base/states/jumping.playerstatejumping has m_superJumpSuccessive=2, m_superJumpHeightIncrease=2.5 and native super-jump sound. Agility inventory modifier is ARK_ATTRIBUTE_SUPER_JUMP_ENABLED +1. Runtime upgrade activation and second impulse still pending; current baseline +94=0 after first ordinary jump.",
    "type": "Note"
  },
  {
    "address": "1408680a0",
    "category": "D2CoopConfirmed",
    "comment": "2026-10-11 exact executable: native jump impulse decrements +94 and adds height+90 to apex+A4. Runtime native-double-jump-v2 observes BOTH Corvo/Emily: Agility TYPE18 super_jump quantity0->1 via native618DE0, first remaining1 then second held-input remaining0, apex rises +2.5m. Quantity restored0; before/after ordinary jumps remaining0; health unchanged. 9 checks. Remote full-body presentation still pending.",
    "type": "Note"
  },
  {
    "address": "1408658c0",
    "category": "D2CoopConfirmed",
    "comment": "2026-10-11 exact Steam: Falling landing decision calls8657C0 damage, then if accumulated fall+88 > decl+158 (live6m), requests native state4 CatFall VT1C82158; stores fall damage into CatFall+50. Live non-damaging Corvo6.5m placement activates CatFall and owned cat_fall_light scene1000ms, Scene_2, playerHandle1. Thresholds live6/8/11/17/20 correspond CatFall/damage/enhancedDamage/death/enhancedDeath. Emily same-class table and thresholds confirmed; positive CatFall runtime pending.",
    "type": "Note"
  },
  {
    "address": "1408658c0",
    "category": "D2Coop-Confirmed",
    "comment": "Exact executable B671F824...: Falling state compares +88 accumulated distance against declaration +158 (observed 6m) to request state4 CatFall VT1c82158. Native-hard-landing-v5: both Corvo/Emily 6.5m isolated gravity fall entered CatFall without damage (health56). Native light body scene cat_fall_light duration1000, one player binding handle1, Corvo Scene_2/Emily Scene_1, handle state+24. 7m room placement failed to produce a fall; do not generalize fixture. Strong/damaging CatFall unverified.",
    "type": "Note"
  },
  {
    "address": "14085b1e0",
    "category": "D2Coop-Confirmed",
    "comment": "Exact exe B671F...16180: CatFall exit chooses crouch via owned component+2f8 input processor, bit index+6c observed16, bits+a0. Both Corvo/Emily runtime latch transitions confirmed. V22 stand-up dip rejected; D2MOVE23/183 with owned delayed endStance blending directly to crouch verified in hard-landing-{corvo,emily}-crouch-v23-v1. Standing regression hard-landing-{corvo,emily}-stand-v23-v5 also passes; 40 checks, 4 actual game videos, DLL16A5A454...73F97C. Scope non-damaging native light scene only; strong roll/travelling/interruption not established.",
    "type": "Note"
  },
  {
    "address": "1408426b0",
    "category": "D2Coop-Confirmed",
    "comment": "Exact exe B671F824...16180, native arkPlayerStatePeeking ID24 VT1c7c4a8. Entry stores logical ArkDoor handle+1f0, initial crouch byte+221 and door side+223. state+24 current scene handle. Both-role normal X-hold/B-exit observation: 1500ms entry,500ms look loop,1000ms exit; left/right and Sneak entry/exit. Scene bound body,right arm,left arm,door_structure child whose+ac parent equalslogicaldoorhandle. ArkDoor VT1cb7c68; +b0 childarray empty unlikeFP arms. Instance+15c cumulative,+160 phase-local,+164 phasebase. Validated read-only reader native-keyhole-reader-{corvo,emily}-v3. No remote visual acceptance yet.",
    "type": "Note"
  },
  {
    "address": "140842d10",
    "category": "D2Coop-Confirmed",
    "comment": "B671F824 native keyhole exit selects ground0/sneak6 from state+221 and reenables equipment53. native-keyhole-holstered-crouch-v4: scene disappears after 1000ms out while exact Peeking24 remains about200ms, phaseEc3 then0. v24-v3 stream250ms stale deadline caught exit. Candidate v24-v4 permits only bounded confirmed endpoint: cached near-complete out scene, nonrenewing end+400ms, same player/state/door/arms, matching scene absent from both native arrays. No event/root/camera writes. Runtime retest pending.",
    "type": "Note"
  },
  {
    "address": "1408310a0",
    "category": "D2Coop-Research",
    "comment": "PE exception metadata confirms function begin8310a0-end8313d3. Native arkPlayerStateClimb update: validates object atstate+220, path list+e8 has>=2 nodes, physics climbing controller is stateMachine+320. Later read-only investigation target; no ladder/chain runtime fixture verified yet.",
    "type": "Note"
  },
  {
    "address": "14043d560",
    "category": "D2Coop-Research",
    "comment": "Exact B671F824 static finding: arkCpntPowerDarkVision initialization calls base61c150 then resolves two components from the current player (8a8ba0); stores +b0 and +b8. +b8 is used by43d3c0 as the start/stop dark vision manager. Runtime grant/cast and remote presentation not yet validated. See work/movement-integration/darkvision-inventory-native-v1.json.",
    "type": "Note"
  },
  {
    "address": "140843540",
    "category": "D2Coop-Research",
    "comment": "Runtime-confirmed on B671F824: early native B cancellation of Peeking entry clears state+24 scene handle to0, leaves +ec3 briefly then original ground/stance; no out-scene exists. Normal out retirement instead preserves scenehandle while absent from both midnight manager lists (+1f5338/+610), phase3->0, ~200ms. Source53571 bounded handoff validates same state/door/owned FP arms, nonrenewing<=400ms. Tests keyhole-corvo-left-early-v24-v9 / keyhole-emily-left-early-crouch-v24-v9. Full functional side/stance matrix + same-world pause recovery recorded, but pause visual continuity and death/load still incomplete. Details work/movement-integration/keyhole-native-findings-v2.json.",
    "type": "Note"
  },
  {
    "address": "14043d3c0",
    "category": "D2Coop-Research",
    "comment": "Runtime confirmed Corvo/Emily: native LT Dark Vision toggle uses power_darkvision_cast.midnightscene Scene_1 933ms, state3, one reciprocal local left-arm binding. Standing Running0 remains active. Native source hand/UI/orange reveal kept local, peer vision inactive. Native unequip does not immediately end reveal; bounded30s native lifetime cleanup passed native-darkvision-corvo-v4. Dark Vision remote full body uses Steam actionv19/variant10, bankD2MOVE25clip193. Four standing takes pass8checks each; v2 closer Corvo/Emily source+observer recordings inspected in selected frames. DLLD01CA730...55B4A. Crouched/moving/unarmed/lifecycle/independent world-VFX determination remain. See work/movement-integration/darkvision-native-findings-v2.json.",
    "type": "Note"
  },
  {
    "address": "14049ed20",
    "category": "D2Coop-Research",
    "comment": "CONFIRMED STATIC (exact Steam exe B671F824...16180): arkCpntDarkVision manager VT141b5eb68+0x28 startup uses declaration+0x98 native FX, stores id at manager+0x300. Manager+0x30 cleanup14049eff0 stops this via87ce20. Exact declaration darkvision_manager.cpntdarkvision names dvision_ply_pp_01.fx: screen particle disabled, screen quad/distortion and native private-vision sound. Cast scene Scene_1 is933ms, one local left-arm group and rumble; no FX track. RUNTIME both roles standing/crouched/drawn/holstered/moving contexts preserve caster-only native reveal; no remote vision manager is started. No independent world-visible Dark Vision effect proven; don't broadcast screen overlay to observer.",
    "type": "Note"
  },
  {
    "address": "140448780",
    "category": "D2Coop-Research",
    "comment": "Windblast static trace on B671F824: arkCpntPowerWindblast VT141b53488 +0x120 ->140448590->this function. Reads upgrade indexes at +B0/B4, selects declaration entityDef +1A0/1A8/1B0/1B8 (basic/greater/shockwave/greater shockwave); derives source position/orientation from native owner helper, spawns via1408AE9B0 and14080CD60. Do NOT call on remote for presentation (damage/AI/physics duplication). Assets identify world effect on native spawned entity; runtime timing/endpoints/cleanup remain unverified. See movement-integration/windblast-native-methods-v1.json.",
    "type": "Note"
  },
  {
    "address": "1401a04a0",
    "category": "D2Coop-ConfirmedLayout",
    "comment": "Read-only FX ID lookup. manager=exact game+0x1f8430; ID hash at +0xeef60 (buckets +0, count +0xc, mask +0x10). Node id+0, uint16 slot+4, next+8. Record pool=*manager, stride0x140, slot<1024. Record owner+0,id+4,definition+8,origin+0x38,axis+0x44. Corroborated 19d100,19d380,1a2970; read-only probe with map/record consistency guards. Windblast FX native lifetime/owner evidence in progress; no damage replication.",
    "type": "Note"
  },
  {
    "address": "1404da500",
    "category": "D2Coop-Windblast",
    "comment": "Native Windblast iterates decl+160/count168 entries(16bytes) and calls87cb90 with blast entity owner. Native FX recording confirms two wblast_blast_01.fx records owned by windblast_object.def, parent local player1, ID changes when record address reused. Manual clone-owner probe ECB2 applies fullscreen distortion to observer: native viewer filtering is insufficient. Reject original declaration for remote presentation. Separate mod FX reuses native particle/sound without fullscreen distortion/rumble; runtime candidate pending.",
    "type": "Note"
  },
  {
    "address": "140448590",
    "category": "D2 co-op confirmed",
    "comment": "Exact B671F824 game: native-windblast-corvo-air-v2 source-only capture confirms cast scene Cast_Ou_Shockwave 1000ms, owned left_arm_1 + player1, persists through Jump2/VT1c85028, Fall3/VT1c845c8 and landing. UsePower63/VT1c80ed8 can coexist with Jump/Fall; two unique native FX IDs at native origins. Jump input must precede LT; simultaneous A/LT is not reliable evidence. This confirms source behavior only; remote air integration/visual quality tracked separately in work/natural-moveset.",
    "type": "Note"
  },
  {
    "address": "141c86448",
    "category": "D2 co-op confirmed",
    "comment": "Exact B671F824 native default-mode state ID5 arkPlayerStateSlipping, VT RVA1c86448 observed in slide-corvo-crouch-v28-v2/states.json at source position(-33.23,-158.38,66.92), stance0. Distinct from sprint-slide ID7/VT1c86108. Sloping/furniture test location invalid for sprint-slide. D662 shadow reader currently rejects this state and retires stream after staleness; requires dedicated supported presentation. Vtable slot+0x68 update1408700a0 manipulates velocity +6c/70/74 and switches toward running/sneak state. Class constructor xrefs1408b99e4/99f3. No animation coverage claim.",
    "type": "Note"
  },
  {
    "address": "140618de0",
    "category": "D2 co-op confirmed",
    "comment": "B671F824 isolated native-ranged-corvo-gun-v2: setting pistol inventory quantity 0->1 via this native API also grants gun_magazine_0_corvo.equipmentupgrade. Removing pistol does not remove that default upgrade. Fixture must snapshot/restore exact role default via same native qty callback; full inventory comparison caught the side effect. Empty pistol input launches gun_ooa Scene_1 500ms bound to owned weapon + left arm; no player state overlay in captured case. Emily behavior pending runtime.",
    "type": "Note"
  },
  {
    "address": "140875e80",
    "category": "D2Coop_ConfirmedStatic_RuntimePending",
    "comment": "D2Coop ranged presentation 2026-10-11: statically traced parent-child native bind wrapper -> 140752900. Parent, child, tag pointer, optional offset/axis, boolean. Creates child reference-frame when absent; guards cycles. Cosmetic-only idEntity gun/crossbow experiment prepared; runtime attachment NOT yet verified. Exact exe SHA B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180.",
    "type": "Note"
  },
  {
    "address": "140879930",
    "category": "D2Coop_ConfirmedStatic_RuntimePending",
    "comment": "D2Coop: static caller trace shows native bind-tag lookup(parent,&tag,child render model,ushort joint,float3 offset,float4 quat). Used before native attachment to avoid unknown-joint fatal fallback. left_hand_wpn from native weapon declarations. Runtime cosmetic prop test pending.",
    "type": "Note"
  },
  {
    "address": "140752900",
    "category": "D2Coop_ConfirmedStatic_RuntimePending",
    "comment": "D2Coop: statically traced native reference-frame bind. +3c0 tag pointer; +3c8 parent entity; +3d0 joint ushort; +3d4 offset; +3e0 quaternion. Updates transform/reparents child, consumes native animation joint pose. Offsets supported by decompilation; pending isolated cosmetic prop runtime validation.",
    "type": "Note"
  },
  {
    "address": "14061aed0",
    "category": "D2Coop_ConfirmedStatic_RuntimePending",
    "comment": "D2Coop: native equipment Start path binds draw-tag at declaration+78 and shows hideWhenNotEquipped equipment. Stop at14061b010 uses holster tag/hides. Pure cosmetic prop avoids inventory/weapon/AI/projectile/pickup components; no assumption that base cpntEquipment can be instantiated.",
    "type": "Note"
  },
  {
    "address": "140619380",
    "category": "D2Coop_ConfirmedStatic",
    "comment": "Ranged cosmetic prop follow-up 2026-10-11: native inventory spawn path explicitly sets new entity+AC owner handle to inventory owner+A8 BEFORE initialization (1408aecc0). Native reference-frame Bind alone does not populate this field: isolated E437 prop runtime has frame+3c8 avatar, joint10, but entity+AC remains3fff0010 and safety validation hid it. Plan mirror only this initialization field on a newly created whitelisted cosmetic idEntity; no source player/inventory writes.",
    "type": "Note"
  },
  {
    "address": "140875e80",
    "category": "D2Coop_ConfirmedRuntime",
    "comment": "43EF isolated Dunwall runtime: custom idEntity with only AnimEvolved+ReferenceFrame, all four Corvo/Emily pistol/crossbow models successfully bind through this API. left_hand_wpn(model-specific) query absent; raw native left_hand_attachment_jnt validated through live skeleton names -> joint10, explicit zero offset/identity axis. Native entity+AC owner handle set before initialize following619380; reciprocal frame parent, child membership and shadow damage ownership pass. 15s native Hide cleanup and unchanged player inventory/health/saves pass. 4 recordings: ranged-prop-{role}-{gun|crossbow}-v3. Wide/dark captures; hand grip, mechanism, networking still unaccepted/unimplemented.",
    "type": "Note"
  },
  {
    "address": "140733330",
    "category": "D2Coop confirmed carry filtering",
    "comment": "Exact retail B671F824. arkCpntPhysicsSystemEvolved virtual +118: sets component+240 bits to (old & BF)|20 and tails to 14076c480 collision filter. Native carry phases invoke this with bool false; release +110 (1407334e0) clears 20/40 and restores declaration filtering. Runtime Mayhew +240 0x30 carrying -> 0x10 after native release. NOT established as general physics/ragdoll suspend. See carry-peer-research-v1/ghidra-carry-lifecycle-v1.json.",
    "type": "Note"
  },
  {
    "address": "140755870",
    "category": "D2Coop carry retargeting",
    "comment": "Confirmed from native Carry45 setup 140829180 and live Emily carry: arkCpntRetargeting target-record setup. Records at +20, count +28 low24, stride100; target handle+90, reference frame+98. Tags from supplied settings. Remote Doppelganger application not verified.",
    "type": "Note"
  },
  {
    "address": "140757830",
    "category": "D2Coop carry retargeting",
    "comment": "Native arkCpntRetargeting activation. With mode+3c nonzero writes component owner+10 into each target reference frame+3f0, invokes per-type control setup140757730. Source Carry45+e8 component observed live. Not an entity+AC spawner reparent.",
    "type": "Note"
  },
  {
    "address": "140757550",
    "category": "D2Coop carry retargeting",
    "comment": "Native arkCpntRetargeting stop from CarryCorpse cleanup140828e80. Per valid target invokes140757c10, clears target reference frame+3f0, releases record list. Remote carry ragdoll handoff remains unverified.",
    "type": "Note"
  },
  {
    "address": "140256000",
    "category": "D2Coop carry joint identity",
    "comment": "Returns skeleton+70 jointIds[index], not the bone index. Native retarget record+6c target / +8c owner are process-local interned name IDs. Live carry target31=anchor_jnt, owner69=center_spine03_jnt. Reconfirm by name in each process; never network raw IDs.",
    "type": "Note"
  },
  {
    "address": "1407d6040",
    "category": "D2Coop victim scene research",
    "comment": "Confirmed native Midnight instance creation via Carry->8549c0. Inputs: manager game+1f5338, out uint handle, arkDeclMidnightScene*, binding list (0x30 records, group intern-name +0, handles idList +10), 8-byte playback options, optional owner/error. Proposed isolated AIGroup1-only carry scene; runtime behavior pending. Exact exe B671F824...16180.",
    "type": "Note"
  },
  {
    "address": "140828845",
    "category": "D2Coop carried NPC state",
    "comment": "Carry45 sets corpse actor+368 bit1 (OR 2); 140829a10 clears bits1/2 on cleanup and 829b85 clears bit1 at drop. Physics tick around 736ccb checks bit1 before forcing a dead/sleeping NPC back to ragdoll. Confirmed disassembly, scoped runtime flag experiment pending. Native victim-only scene alone played with midnightWeight1 but ragdoll visually won (carry-victim-native-corvo-v1).",
    "type": "Note"
  },
  {
    "address": "140736ef0",
    "category": "D2Coop native carry physics",
    "comment": "Exact native PhysicsSystemEvolved callback for message 0xba, registered at732ecc and sent by Carry45 at8288c7. Clears low physics flags, requests mode3 at+23c, queues scheduler via virtual+1c0, invokes+130. Victim Midnight playback and corpse+368 carried bit alone did NOT switch mode0 to3 in isolated v2. Scoped callback experiment pending. Release via native734a10 requests mode0, followed by observed mode0 and pending=-1 before release acceptance.",
    "type": "Note"
  },
  {
    "address": "140197a60",
    "category": "D2Coop carry research",
    "comment": "CONFIRMED runtime corvo-v5: call native model resolver(decl,true) before 7d6040 creates Anim track, midnightData input count becomes1, physics0->3->0. Four source skeleton names at GroupInfo+68 alone were insufficient; resolved MD6 list +18 was empty in v4. v5 body still too high (initial root was shoulder position); not visually accepted. Exact D2 executable B671F824...516180.",
    "type": "Note"
  },
  {
    "address": "14053d3f0",
    "category": "D2Coop carry research",
    "comment": "Confirmed native joint-offset transform: phase cache +2e8, pose +2d8; position from xyz offset, rotation from native idQuat at offset+0c. Native idQuat is conjugate of skeleton QTS. v7 skeleton-derived owner offset corrects standing actor-root placement; crouched owner tilt still raises NPC corpse because root physics retains upright yaw. Continuous rotation compensation pending v8; not accepted release feature.",
    "type": "Note"
  },
  {
    "address": "1407d2160",
    "category": "D2Coop carry",
    "comment": "Exact B671 executable: start wrapper 7d9270 argument3 is int* start offset. This function assigns *param_2 to instance+160; wrapper argument4 is behavior/seek flag, not elapsed milliseconds. Native source drop_low_stand victim ragdoll marker656ms, sneak500ms per exact installed declarations. Victim-only drop derivatives prepared; playback integration pending.",
    "type": "Note"
  },
  {
    "address": "14053d3f0",
    "category": "D2Coop carry",
    "comment": "Exact B671 executable. Joint-cache transform produces native idQuat (conjugate of body/skeleton QTS convention). v8 native victim generic_carrycorpse_idle plays on Mayhew and follows Corvo+Emily standing/walking/crouching; v11 aligns actual victim spine02 to carrier left_arm plus authored model-space clearance(.025,.19,.115), visual review pending alternate angle. No full carry completion; initial attachment pop, release acting and ownership lifecycle remain incomplete.",
    "type": "Note"
  },
  {
    "address": "140832770",
    "category": "D2Coop carry",
    "comment": "Exact B671 executable. Native Drop46 setup. v13 isolated Corvo/Mayhew test now connects victim generic_carrycorpse_droplow through single-NPC derived Midnight scene and authored D2MOVE32 carrier low-drop, using replicated source phase. Native ragdoll request at source marker656ms; scene/retarget/carried flag/physics cleanup verified. Initial attach pop, hand contact, automatic pickup, other victims, throw and broader lifecycle NOT complete. Evidence work/natural-moveset/carry-victim-native-corvo-v13.",
    "type": "Note"
  },
  {
    "address": "14082888f",
    "category": "D2Coop Carry Pickup Research",
    "comment": "CONFIRMED static sequence: Carry45 phase2 sends corpse event0xba, calls PhysicsSystem vfunc118(false) collision filter, starts native pickup scene through829820, configures retarget829180. Offline generic_carrycorpse_in lasts1s but starts with abdomen already1.06m above root; raw clip is NOT a complete floor-to-shoulder trajectory. Existing4.8s authored lift cannot simply be played on1s native timeline. Native ragdoll blend and carrier positioning remain runtime-research gaps. Current v13 scoped low drop works both roles/stances; pickup not integrated.",
    "type": "Note"
  },
  {
    "address": "140829180",
    "category": "D2Coop confirmed / pickup gap",
    "comment": "2026-10-11 native-pickup-geometry-corvo-v1: Carry45 phase1 exposes the exact corpse handle before scene+24 exists; phase2 owns a 1000ms carry_corpse_in scene. Sequential native skeleton snapshots show spine02 model Z ~0.09 before -> ~1.14 near start -> ~1.83 by 0.48s. Retarget count 0 ->1, physics animated during lift. FP capture inspected. Native clip alone does not provide a visually continuous floor lift. recoverFromRagdollSnapshotInput/BlendInput exist in NPC graph, but their use for this transition is a hypothesis, not verified playback.",
    "type": "Note"
  },
  {
    "address": "140596110",
    "category": "D2CoopResearch",
    "comment": "Targeted decompilation confirms native AIPhysicsReaction recovery flow: +88 AnimEvolved, +90 PhysicsSystem; pending +152 bit3 captures with AnimEvolved 54de30, sets +138 weight1, writes graph via 700fc0 and updates graph 6e0ae0. Later decays weight by dt*decl+29c. 54de30 fills preallocated snapshot Anim+2c60 from local matrices+188 using rotation/translation and binds graph via700ff0. Scoped Mayhew carry-snapshot-v18 reuses only capture/setters, not the AI recovery state, health or events. Runtime/visual feasibility pending; no complete pickup claim.",
    "type": "Note"
  },
  {
    "address": "140596110",
    "category": "D2CoopConfirmed",
    "comment": "2026-10-11 isolated carry snapshot experiment: verified exact native snapshot capture 14054de30, binding 140700ff0 and weight 140700fc0 can blend dead Mayhew from floor pose to victim carry loop without invoking AI recovery/health. v18 concurrent retarget setup made root descend ~1.3m; v19 defers setup/activation until after 800ms blend. v20 Corvo video+telemetry confirms restored at812ms, retarget activated at859ms; no floor dive, clean native ragdoll release, first-person and avatar health preserved. Evidence work/natural-moveset/carry-snapshot-corvo-v20/snapshot-validation.json. Snapshot slot Anim+2c60 / wrapper Anim+2728 inputs +4a8 and +4b0. Hypothesis: early retarget used unblended anchor while observed globalPose was blended. Mechanism only: carrier lift acting, automatic native pickup timing and general body authority remain unverified.",
    "type": "Note"
  },
  {
    "address": "140734a10",
    "category": "D2CoopConfirmed",
    "comment": "2026-10-11 native ragdoll request callback: sets PhysicsSystem+23c=0 and invokes vtable+1c0 (140731860 queues scheduler), so wait for mode+238=0 and pending+23c=ffffffff. Isolated v20 snapshot-cancel stopped recovery binding and victim scene before queuing ragdoll: body visibly launched upward despite successful ownership cleanup. v21 holds current recovery weight while requesting ragdoll, then clears snapshot/scene only after mode transition; Corvo cancellation video shows ordinary fall without launch, 13 mechanical checks pass, cleanup279ms. Evidence carry-snapshot-cancel-corvo-v21. Hypothesis for v20: abrupt unblended clip exposure contaminated physics handoff/velocity. Full pickup/automatic authority remain unverified.",
    "type": "Note"
  },
  {
    "address": "140829030",
    "category": "D2CoopConfirmed",
    "comment": "2026-10-11 source preparation: standing native In starts after phase1 with Running0+Carry45+Equip53 and no scene. New crouched Corvo v20 trace confirms exact counterpart Sneaking6(vtable1c84258)+Carry45(1c77748)+Equip53(1c79cf8), stance1, corpseF8 valid, phase110=1, scene24=0. Previously rejected -> native_state_stale at1575ms. v21 strict reader allows only matching stance/base-state combination and rechecks stance/identity. Crouched Corvo v21b passes all10 stream continuity checks: native In and Loop received, sender/receiver stay ready, no rearm. Still source semantics only; remote full-body pickup not connected. Emily counterpart validation in progress. Earlier standing v17 both-role pass remains preserved.",
    "type": "Note"
  },
  {
    "address": "140829030",
    "category": "D2Coop-Confirmed",
    "comment": "v21 exact crouched pickup preparation confirmed in both source roles: stance1, Sneaking6 VT1c84258, Carry45 VT1c77748 phase110=1 scene24=0, Equip53 VT1c79cf8. Strict target generation/stance reread accepted in native_player_carry_scene.h. Both actual source FP recordings + peer telemetry show preparation->In1->Loop2 without stale stream/restart. Does not establish complete remote-body pickup animation.",
    "type": "Note"
  },
  {
    "address": "140734d90",
    "category": "D2Coop-Confirmed",
    "comment": "ExactB671: native carried-body handoff helper copies joint world pose via7697d0 into rigid bodies and zeroes linear/angular velocity via18d020. v22 F687BAA isolated Mayhew pre-attachment snapshot cancellation invokes before734a10 while snapshot remainsbound throughmode0. Both roles +Emilyrepeat visuallyfall/settle without v21Emilyupwardthrow;13checks each. Emily action-stream loss/rearm15checks, noattachmentreplay. Separate causal contributions (pose resync vs velocity reset) remainhypothesis. Fullpickup acting/contact notcomplete. See work/natural-moveset/carry-pose-findings-v22.json.",
    "type": "Note"
  },
  {
    "address": "140829030",
    "category": "D2Coop/CarryPickup-v23",
    "comment": "CONFIRMED by decompilation: Carry45 entry stores corpse handle at +F8, calls phase helper 140828770 with 1; flags at +D0 (bits 0,2,4) can then request 2, bit4 can request 3. Runtime prior v21 both roles: phase +110==1, scene +24==0, Equip53 alongside Running0/Sneaking6. v23 experimental stream variant7 is this exact observed prep, with a locally measured 0..300ms AUTHORING clock; 300ms is NOT a recovered native scene duration. Native In variant1 retains actual scene duration1000ms. Corvo v23 carrier-only game recording verifies packet prep->In->loop and v33 standing pose playback; victim choreography and visual acceptance remain incomplete.",
    "type": "Note"
  },
  {
    "address": "140700fc0",
    "category": "D2Coop/CarryPickup-v24",
    "comment": "CONFIRMED isolated runtime evidence on exact B671F824... executable: native recovery snapshot graph weight setter supports Mayhew floor-pose blend into the victim-only carry loop. v24 D3A792... sources blend from observed Carry45 preparation/1000ms In; reciprocal attachment waits for restored snapshot +50ms. Both-role recordings pass scoped ownership/late cleanup. Emily early cancel (native-paired-pickup-emily-cancel-v24c) restores native ragdoll without attachment; 14cm corpse root displacement. AUTHORING HYPOTHESIS: measured wrist-target offsets/contact curves. Generic corpse geometry, right-hand grasp, crouched hoist, visual acceptance remain incomplete. Evidence: work/natural-moveset/paired-pickup-findings-v24.json; contact probe queries are sequential and status clocks not atomic.",
    "type": "Note"
  },
  {
    "address": "1408784b0",
    "category": "D2Coop/SwordVisibility-v26",
    "comment": "CONFIRMED v26 runtime A3A1542B: exact clone inventory child hide/show for both roles. Four stance/equipment transitions per role, unarmed directional movement, pause/resume restore pass (20 checks); paired-pickup movies both roles pass44 checks each and temporal sheets inspected. Native flags+d0 bit0 and same inventory child independently observed; blade absent at holster and returns on cleanup. These funcs update renderer/descendants via87e040/87e0a0. No physical sword folding or complete carry claim. Evidence work/natural-moveset/sword-visibility-findings-v26.json.",
    "type": "Note"
  },
  {
    "address": "140734d90",
    "category": "D2Coop/DropHandoff-v27",
    "comment": "CONFIRMED exact EXE B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180: copies visible joint pose to rigid bodies and zeros velocities; native carried-body release xref 140736dc5. Owned Mayhew remote drop must call before 140734a10 ragdoll request. v26 Corvo failed continuous standing pickup/walk/drop (5.35 m final separation, upward launch in video). v27 DLL 80DCD988745C5DA1863D13CE1F8FA67B12A4E15FEA6EDE85E17179B50E8197F0 passed four Corvo/Emily standing/crouched-drop cases,36 checks each, native video temporal sheets inspected. Final separations .442/.384/1.157/1.147 m. Scoped physics handoff confirmation only; full hand contacts, arbitrary bodies and throws remain unverified. Evidence: work/natural-moveset/native-pickup-walk[-crouch]-drop-{corvo,emily}-v27.",
    "type": "Note"
  },
  {
    "address": "140828845",
    "category": "D2Coop/CrouchedPickup-v28",
    "comment": "Observed native Carry45 reference on exact EXE B671...: both Corvo and Emily crouched pickup retain variants1/2 with 1000/3000ms durations and stance1; victim actor root is 0.674m lower than standing relative to source player root. Sequential skeleton references in work/natural-moveset/carry-research/native-crouch-lift-heights.json. v28 DLL84112870B0B30DD979F124F91F3075D420F2D140197FAD678AF55302609F80F9 + D2MOVE35 adds separate crouched carrier hoist and matching lowered snapshot frame for bounded unique Mayhew experiment. Both native-input crouched pickup/walk/drop captures pass39 lifecycle checks. Hypothesis: fixed 0.674m offset applies to other body types; NOT verified. Visual contact still rejected for v1; measured loaded wrist gap~0.325m, separate v2 bank revision under test. No first-person pose anchors, no general NPC ownership, no complete carry claim.",
    "type": "Note"
  },
  {
    "address": "140757830",
    "category": "D2Coop/CrouchedRetargetOrientation-v29",
    "comment": "Confirmed bounded runtime finding: when deriving native attachment ownerOffset rotation, DO NOT feed current victim actor root rotation back into the desired anchor rotation. v28 lowered crouched-frame calibration used rootDelta*anchor for position AND rotation; victim yaw drifted63.05deg while player heading stayed fixed during one walk. v29 transforms position into carrier space but retains original native clip anchor rotation, same orientation convention as established standing branch. DLL B700CFBF950E994C77C60A4BBD5B46A99F6C6C4311FC282AECE7E1D0B6F7F100. Both roles pass43 lifecycle/orientation checks: Corvo drift0.044deg, Emily0.497deg; actual close-view D2 recordings inspected. Prior contact bank fitted against drifting geometry rejected; refit under test. Not verified for general NPCs, turning, throws or full co-op ownership. Work/natural-moveset/native-crouched-pickup-{corvo,emily}-orientation-v29.",
    "type": "Note"
  },
  {
    "address": "140757830",
    "category": "D2Coop/CarryStanceAnchor-v30",
    "comment": "CONFIRMED bounded exact-build runtime: source-owned carry anchor must recompute position from current carrier left_arm_joint + clearance(.025,.19,.115) minus victim center_spine02 each frame, retaining native clip rotation. Reusing calibrated position caused about .47m shoulder separation after stand/crouch transitions on v29. v30 DLL891A60B35E162DEE695B7D9C0E7ED67EC5F1CE5C9A05DA4BCD932D4989BD7727 tested Corvo crouched-to-standing and Emily standing-to-crouched native pickup/walk/drop. Final load offsets .01708/.01685m; yaw drifts .269/-.017deg; scoped checks pass (51/48). First Corvo v30 recording missed preparation receive sample: kept as failed validation; improved earlier timestamped sampling in v30 harness, rerun passes. Supporting wrist target residual .1052m Corvo/.039m Emily: full surface contact NOT accepted. Generic NPCs, turning, throws and production carry remain unverified. Evidence work/natural-moveset/native-crouched-to-standing-carry-corvo-v30-sampled and native-standing-to-crouched-carry-emily-v30.",
    "type": "Note"
  },
  {
    "address": "140832770",
    "category": "D2Coop/NativeThrowObservation-v1",
    "comment": "Observed exact B671F824 native standing RT throw in both roles: source first-person clips native-body-throw-{corvo,emily}-v1 use carry_corpse_drop_high.midnightscene Scene_1 duration666ms. 8 high-scene samples each: first sequential state/scene row crosses Carry45->Drop46, later7 show Drop46. Strict all-row validator retained failed; health unchanged and state retires. Video sheets inspected; wall-facing setup does not validate unobstructed trajectory. Static decomp140832770 calls1403a6f10 selector, scene refs+70/+78/+80/+88array/+98, stores index Drop46+54, corpse handle+38; configures native body/physics flags. HYPOTHESIS high-drop is selector2 not yet independently verified. Current native carry reader refuses high variant. No replicated throw implemented. Next bracket native source reads, validate selector/bindings and native release/impulse. Evidence work/natural-moveset/carry-research/native-throw-findings-v1.json and native-drop-high-decompilation-v1.json.",
    "type": "Note"
  },
  {
    "address": "140832290",
    "category": "D2Coop/NativeThrow-v31",
    "comment": "Exact B671F824 build. STATIC CONFIRMED: if Drop46+50 and input history check allow, queues event0x149, payload0x1c, exact corpse handle+38. Linear vector=owned PlayerCamera+25288 * DropCorpseDecl+a0; angular vector at1431af668 is zero; payload bytes24/25=1. LIVE READ: both character declarations have strength4. Native observations and guarded source reader in work/natural-moveset/carry-research and native-body-throw-*-reader-v31. Remote presentation/impulse v31 is isolated experiment, not general corpse authority or accepted quality.",
    "type": "Note"
  },
  {
    "address": "140727100",
    "category": "D2Coop/NativeThrow-v31",
    "comment": "Function newly defined from verified callback thunk14080bc50. STATIC CONFIRMED: physics event149 handler receives payload through event+8; adds or replaces linear/angular vectors at component+98..ac according to byte25; sets pending flags+b0..b2; calls virtual+130, PhysicsSystemEvolved resolves to140769350 wake routine. Registration site140732f32/f49 links handler to149. Not a damage callback. See saved decompilation and native source impulse study. No arbitrary live physics mutation implied.",
    "type": "Note"
  },
  {
    "address": "140734a10",
    "category": "D2Coop/NativeThrow-v33",
    "comment": "Exact B671F824 static: guarded native ragdoll request, skips if PhysicsSystem+241 bit3 set, sets pending+23c=0, virtual+1c0 and native event0xbb. RUNTIME bounded Mayhew paired throw: v31 request before scene stop and v32 stop-only both timed out with zero impulse; v33 stops exact victim scene, requests ragdoll once afterward, then waits mode0/pending-1 before event149. Both standing Corvo/Emily v33 recordings pass23 pickup+21 throw scoped checks and queue exactly one camera-derived velocity. Root cause is ordering; whether the initial request was ignored or superseded is not directly sampled. Hand/body contact, general bodies and authoritative world outcomes remain unaccepted. Evidence work/natural-moveset/native-high-throw-*-v33; DLL48A17FA2.",
    "type": "Note"
  },
  {
    "address": "140829180",
    "category": "D2Coop/NativeThrowAttachment-v34",
    "comment": "STATIC CONFIRMED exact B671F824: called by829e30 before Drop46 scene start832770. Computes reference-animation joint transforms using CarryCorpseDecl+80/+90/+98, invokes755870, sets owner joint/offset, activates757830. Exact Void asset export carry-release-placement-v34 SHA12BB41B5 names spine03 carry, spine02 drop, anchor_jnt, reference ply_body_common_carrycorpse_in. V34 isolated analogue retains remote attachment until280ms and rebases03->02 preserving its model frame; does not invoke player state callbacks or copy FP anchors. It is not the same calculation as native829180 and is not visual acceptance; see native-high-throw-*-v34 recordings and documentation/handoff-2026-10-11/13-native-body-throw.md.",
    "type": "Note"
  },
  {
    "address": "1403a6f10",
    "category": "D2Coop/CompactRelease-v35",
    "comment": "CONFIRMED: selector4 fallback chosen for exact carry_corpse_drop_fetal_pose scene, variant5 in lab, not generic drop_soft. 832770 selects DropCorpse decl+98, stores selector+54, sets corpse+368 bit2 for selector4. Original victim track generic_carrycorpse_dropfoetal lasts166ms, carrier666ms, no ragdoll event. Corvo source-only runtime trace native-fetal-release-corvo-reader-v35 shows physics3->2->0, attachment remains through scene. Experimental166ms remote handoff is a hypothesis, not a proven native callback.",
    "type": "Note"
  },
  {
    "address": "140736c30",
    "category": "D2Coop/CompactRelease-v38",
    "comment": "Static: entry into physical mode with NPC+368 mask0x2 and mask0x4 routes to734d90; Drop46 selector4 sets mask0x4 at832770. Fixture v35 cleared mask0x2 before transition and did not preserve this native branch. Runtime v37 shows foetal pose at fraction0.13 (Mayhew knees148/137deg), then upright pose during handoff. Candidate v38 holds constant foetal clip through666ms and retains masks0x2/0x4 until cleanup. Not yet runtime/visually accepted. No original assets changed.",
    "type": "Note"
  },
  {
    "address": "14061cd60",
    "category": "D2Coop/BendTime-v39",
    "comment": "Confirmed static native toggle: equipment+d0 indicates active; +c0 is owned time-manager request handle. Inactive branch creates request via80eb40(game+1f52e8), starts owner timer778f40, broadcastsE2/E4. Active branch cancels timer778a00, removes owned time request80ede0, zeros+d0/d1 and sets+c0=-1, broadcastsE3/E5. Do not call on remote avatar merely for animation. Exact assets533ms cast-in(action event150ms),300ms loop,433ms out, plus slide variants. New opt-in source acquisition fixture uses normal quantity/equip path only; runtime test pending.",
    "type": "Note"
  },
  {
    "address": "14081ef10",
    "category": "D2Coop/BendTimeViewer-v41",
    "comment": "2026-10-11 exact B671F824 static trace: arkPlayerFXBendtime vtable141c76808, native update separately writes render factor from native time dilation and fade state. 81ec80 selects player mode; 81ee00 creates managed request from declaration+150..170; 81ee80 removes owned request. Void bend_time_01/02.fx contain screen entry/loop particles, not ordinary remote-hand VFX. Manual finite particle feasibility is separate from complete viewer appearance/shared time authority. Saved work/movement-integration/bendtime-research and documentation/handoff-2026-10-11/15-bend-time-integration.md. Do not activate gameplay toggle61cd60 solely for remote animation.",
    "type": "Note"
  },
  {
    "address": "14061cd60",
    "category": "D2Coop/BendTimeAir-v43",
    "comment": "Exact executable runtime observation native-bendtime-corvo-air-v43: normal 533ms cast_in_test retains left-arm67862+player1 during Jump2 and Fall3, optionally UsePower63. 433ms cast_out_test observed during Fall3 with only owned left arm. Two native toggles, inventory cleanup and health pass; source-local time only. v43 permits these exact owned scene overlays and preserves network root/jump metadata. Does not grant generic airborne casts, crouched-air, slide or asset-only hold. Read-only mana candidate component+20 observes100 after load and60 after activation/end. Source evidence work/movement-integration/bendtime-air-evidence-v43.json.",
    "type": "Note"
  },
  {
    "address": "140746310",
    "category": "D2Coop/NativeMana-v45",
    "comment": "Confirmed exact-install read-only runtime: arkCpntMana vtable RVA 1c55ed8, owned component +20 finite float. Native fixture test starts 100; after one Bend Time activation/end reads40; two actual failed requests at6/8s produce power_outofmana.midnightscene Scene_1 333ms, sole owned Corvo left-arm67862, Running0. Later regenerated60 makes11s attempt succeed; initial failed harness preserved. +20 initialization here from declaration+68/attributes; expenditure/cost function not yet identified, do not infer universal cost40/60. See work/natural-moveset/native-bendtime-corvo-no-mana-v44-v2 and outputs/.../asset-exports/native-outofmana-v45.",
    "type": "Note"
  },
  {
    "address": "14087cb90",
    "category": "D2Coop/FailureHandFX-v47",
    "comment": "Exact-executable static/native runtime result: actor-owned FX start writes record world origin+0x38, axis+0x44, optional callback+0x128. v47 isolated manual out_of_mana_01.fx on remote Corvo/Emily left wrist: both six-check takes pass (three starts, tracked owner/ID/definition, moving-hand updates, expiry/cancel, health/protected hashes unchanged). Temporal game frames show blue hand smoke without obvious screen blur. Wrist-centred adaptation, NOT exact FP fx_tatoo tag: source tag parent left_medium_01_jnt, Corvo (-.006,.025,.033), Emily(-.006,.025,.009); remote metacarpal differs. v48 automatic once-per-delayed native failure event remains pending runtime verification. No power activation or damage calls.",
    "type": "Note"
  },
  {
    "address": "140632f70",
    "category": "D2Coop/Grenade-v52",
    "comment": "Exact B671F824: HandleFire identified by debug-string xref141b9da40->1406334ff. Gameplay projectile creation/velocity path, must not replay for remote cosmetic animation. v52 isolated native equip experiment both roles PASS7: regular grenade TYPE_AMMO4 slot1 qty0->1->0 via native inventory callbacks; ordinary X holster/draw required to start FP scenes. gadget_grenade_equip Scene_1 500ms binds own left arm+grenade; unequip Scene_1 233ms arm only. State64 +24 refers grenade scene while active; state53 +24 points sword scene, and grenade can persist after64 ends. Validate manager scene exact arm/equipment ownership rather than state name/selected item. Local FP videos reviewed; remote body/prop and cooking/throw not yet implemented. All protected inventory/health/settings/save and original EXE/SteamDLL hashes unchanged. Evidence work/natural-moveset/native-grenade-equip-corvo-v52-v3 and native-grenade-equip-emily-v52.",
    "type": "Note"
  },
  {
    "address": "141c7f118",
    "category": "D2Coop/Grenade-v54",
    "comment": "Native runtime: arkPlayerStateThrowWeapon id62, both roles high throw and Corvo low throw under normal LT1300ms. Alongside Running0. Scene_1 cooking_in500ms arm+player+grenade; idle1333ms arm+grenade; high/low release400ms arm+player+grenade; high_ooa566ms or low_ooa400ms arm+player. Quantity1->0 at release, equipment identity persists. Component+17c false cooking/high, true low throw under pitch35, confirms scoped low-throw flag. Native high throw WEAPON_FIRE event293ms in asset, never replay for cosmetic remote animation. Source self-blast damage observed; inventories and both isolated checkpoints restored, protected14unchanged. Readonly reader/action candidates under src/native_player_grenade_scene.h, actual-game source videos work/natural-moveset/native-grenade-actions-*-v54. No remote body/prop/projectile authority demonstrated.",
    "type": "Note"
  },
  {
    "address": "140875e80",
    "category": "D2Coop/Grenade-v55",
    "comment": "2026-10-11 exact B671F824: isolated regular-grenade cosmetic idEntity with only native animation and reference-frame components attaches to Doppelganger left_wrist_jnt (joint9) via this native bind/update path. Both Corvo and Emily v55b fixed-pose tests pass reciprocal parent/child, component whitelist, parent damage-source identity, finite Hide and unchanged inventory/health/14 protected-file checks. Native Hide 1408784b0 flag entity+0xd0 bit1 confirms completion; reusable-prop predicate also rejects pendingHide and must not be used to confirm cleanup before clearing that flag. Failed v55 test preserved. v55b DLL AFE5E632E62C54F2A8DAFC4456566F76722D092923C7C726B3D2016DD7C14D84. Exact evidence work/natural-moveset/grenade-prop-{corvo,emily}-close-v55b/result.json, native-sheet.jpg. Dark/occluded fine grip unaccepted. This proves scoped cosmetic attachment/expiry only; network grenade body, mechanism, projectile/world/damage and VFX are not proven by these tests.",
    "type": "Note"
  },
  {
    "address": "141c7f118",
    "category": "D2Coop/Grenade-v56",
    "comment": "CONFIRMED exact executable B671F824...: standing default-mode Running0 (vt+1c85628) plus ThrowWeapon62 (vt+1c7f118), either order, remains stationary motion while independent exact grenade scene reader validates phase. v56i four isolated standing cases PASS11 each: Corvo high/low and Emily high/low, actual source/receiver recordings inspected, rear dark view fine grip not accepted. Source high variants3/4/5/6, low3/4/7/8; cosmetics hide at release. Source alone creates projectile/sparks/explosion/world damage, none synchronized yet. Failed first low stimuli retained: pitch recentered on draw; later Emily fixture expired before recovery. Harness preloads receiver bank before12sec grant and verifies pitch and>=3sec remaining at trigger. Source string batching32bytes preserved limits/rechecks and reduced peak source read~81ms to~18ms. Following rearm preserves peer clock rather than rebasing cached root. Movement/co-op incomplete.",
    "type": "Note"
  },
  {
    "address": "14009cfa0",
    "category": "D2Coop/Grenade-v57",
    "comment": "CONFIRMED isolated v57b: phase3 native local-matrix setter + rebuild14009dc40 drives 6 native grenade joints on exact cosmetic idEntity only. Corvo high and Emily low front-view tests PASS12 each; independent probe localPose idle->cook max delta .98214066, writes stop at release, both checkpoint restores preserve14protected hashes. Exact resources grenade.md6/.md6skl; origin,root_jnt,head_jnt,timer_jnt,pin_jnt,lever_jnt. Pure native offline Edge samples, no animation events or source weapon writes. DLL D35D9138... probe8B523CB6... (earlier stale probe failed, retained). Fine grip/feet not accepted. In-hand VFX next candidate, projectile/gameplay replication unfinished.",
    "type": "Note"
  },
  {
    "address": "14087cb90",
    "category": "D2Coop/Grenade-v58",
    "comment": "CONFIRMED exact EXE B671F824...: v58 DLL129137DA... creates one owned native in-hand cosmetic grenade FX; Corvo high and Emily low close views PASS13 each with paired native sheets. Native-derived particles/light/tick declaration, rumble removed; audio not independently verified. receiver-stop-v58b, source-pause-v58 and receiver-pause-v58b PASS12 cleanup each; mechanism writes stop and owned FX stops. First receiver-pause failed because pause preceded first sampled effect update; failed evidence retained, stimulus now waits for update. These cleanup runs have automatic menu recovery OFF and avatar hidden after pause; NOT recovery proof. Only exact cosmetic six-joint child eligible. Local native gameplay alone creates projectile/damage. World projectile/explosion replication incomplete. See documentation/handoff-2026-10-11/20-grenade-stream-development.md.",
    "type": "Note"
  },
  {
    "address": "140870ce0",
    "category": "D2Coop/NativeStun-v60",
    "comment": "CONFIRMED exact EXE B671F824... Native arkPlayerStateStun state14 VT RVA1c86768 inherits navigation; Update calls1408695c0, decrements countdown +a8, coordinates equipment states64/53 and exits to navigation when complete. 2026-10-12 actual grenade self-damage: standing default mode Stun14 plus optional EquipChange53 exactVT1c79cf8. Corvo owned hit_react_stun_right.midnightscene and Emily hit_react_stun_front.midnightscene, Scene_1,800ms,phase3,one reciprocal binding to player handle1. Evidence work/natural-moveset/grenade-stun-scenes-{corvo,emily}-v60; raw decompilation work/ghidra-stun-v60.json and ghidra-stun-navigation-v61.json. Candidate v61 recognizes exact owned scene and sends cosmetic direction/time over protocol38. Left/back runtime,crouched/airborne stun and remote gameplay damage remain UNVERIFIED; native NPC third-person clip presentation is an unaccepted visual candidate.",
    "type": "Note"
  },
  {
    "address": "141c7f118",
    "category": "D2Coop/GrenadeCrouch-v62",
    "comment": "CONFIRMED exact B671F824 build: ThrowWeapon native state62 vtable. Source observations work/natural-moveset/grenade-crouch-observe-corvo-v60 and grenade-crouch-observe-emily-v62 show default-mode Sneaking6 VT141c84258 + ThrowWeapon62 with physics stance1 during regular-grenade cook/throw. Scene_1 native cooking_in500ms (owned left arm/player/grenade), cooking_idle1333ms (left arm/grenade), throw_high400ms (left arm/player/grenade), throw_high_ooa566ms (left arm/player). Both source videos inspected; no remote binding proved by observation alone. v62b candidate allows exact grounded state pair and exact single Sneaking6 held-item fallback, preserving reciprocal inventory/scene ownership and native-clock guards. D2MOVE44 appends 9 complete crouched body candidates per role; native first-person behavior retained. Low/airborne/multiple overlays and visual acceptance remain unverified; no remote projectile/damage events.",
    "type": "Note"
  },
  {
    "address": "141c7f118",
    "category": "D2Coop/GrenadeAir-v63",
    "comment": "Exact retail SHA B671F824... Native ThrowWeapon62 observed jointly with Jump2 VT1c85028 and Fall3 VT1c845c8, default mode physical stance0. v63b protocol39 delayed-body-clock admission corrected after v63 reason19 failure. Both-role grenade-air-network-{corvo,emily}-v63b PASS16: idle held-grenade jump, cooking jump/high release, remote body+six-joint prop+one owned in-hand FX through Steam, release hide/duplicate rejection/landing; inventories and protected14/checkpoints restored. Source and receiver temporal sheets inspected; fine visual acceptance false. Low air release, double jump while cooking, all directions/interruption matrix unverified. Native source owns projectile/world/damage; receiver projectile/explosion not implemented. VT+10=14084a8f0,+18=14084a7e0,+30=14084a940 use140802c20; exact semantic roles still hypothesis.",
    "type": "Note"
  },
  {
    "address": "1407194a0",
    "category": "D2Coop/NativeElixir-v65",
    "comment": "Exact retail B671F824... PE RIP scan found ConsumeHealthPotion registration1404f6695 inside1404f65b0 despite negative defined-string xrefs. Health callback1404f5fb0 -> native1407194a0: eligibility718ce0, inventory6147f0, health719fe0, message15c, statistic16. Mana callback4f5ff0 ->745b20: eligibility7459f0, inventory, attributes/random branches, gain, statistic17. IMPORTANT CORRECTION: health decl+d8 and mana decl+80 queued via messagea9 outside logic8 are arkDeclSoundEvent m_consumeSoundEvent, confirmed reflection14165daf0/14165e340 and exact asset exports native-elixir-components-v65. NOT body animation/VFX. String m_healthPotionEffect is difficulty factorMultiplier_t (141802090), NOT visual effect. Both-role potion-health-*-v64d wheel tests PASS6; both-role potion-gameplay-*-v65 native callback PASS6, health56->70 gradually, Running0/sword remains. Both-role potion-movement-*-v65 PASS8 native Steam walking/stopping continuity, same tokens, no drink gesture. Silent videos do not prove audio. Remote drink audio/health sync not implemented. Quantity decrease alone includes fixture rollback; v66 read-only stat arrays validation underway. See raw ghidra-potion-consumption-v65 and ghidra-potion-reflection-v65.",
    "type": "Note"
  },
  {
    "address": "14067aea0",
    "category": "D2Coop/PotionStatistics-v66",
    "comment": "Exact retail B671F8...: diagnostic IDs health0x16/mana0x17; manager game+1f77c8, A+44/B+100. v66 Corvo normal callback+walking PASS9 and Emily native wheel PASS7 each A health0->1 and B +1; grant unchanged. Wheel-only Corvo control PASS6 quantity rollback leaves A/B unchanged. v67 Corvo native BendTime spends mana100->40, native wheel X restores40->100 with A mana0->1/B mana+1, health56 unchanged, PASS8, both checkpoints/protected14 restored. A resets on observed checkpoint reload; B persists in process across that reload (health143->144 between v66 tests), not a global contract. 140745b20 attribute-dependent early-return branches can consume without reaching stat increment; counters are insufficient for a complete event protocol. Native audio/remote outcomes unimplemented.",
    "type": "Note"
  },
  {
    "address": "140745b20",
    "category": "D2Coop/NativeMana-v68",
    "comment": "Exact retail B671F8... native wheel handler1404f5ff0 invokes this mana consume routine. Guarded test callback use_mana invokes same engine-thread routine only for finite isolated observed mana grant, live exact player and component1c55ed8/decl1c55d98. Corvo v68 and Emily v69 PASS10 each: controller power spending establishes deficit; native equipment/effects retire before potion use; mana rises40->100 /92.10007->100, both potion counter arrays mana+1 and health unchanged. Steam body walking/stopping and same tokens persist. Source-only resource change. Native +80 consumeSoundEvent message0xa9 suppressed in wheellogic8; remote audio/resource sync unimplemented. Attribute early returns mean statistics do not cover all mana-use branches.",
    "type": "Note"
  },
  {
    "address": "140621660",
    "category": "D2Coop/Mesmerize-v71",
    "comment": "Exact EXE B671F824: equipment VT1b9bea8 decl1b9bd68, +d4 committed world handle; callback resolves world component1c53958 and state3->2. World decl VT1c53a38; parent sentinel3fff0010; native object model mesmerize_v2.md6, not just particles. Preview entity exists BEFORE +d4 commit, ignore entity-name presence. v72 strict owned Emily left-arm enter466/loop1200/out666 PASS7. v74 standing drawn Emily full-body Steam binding PASS10, protocol40/336 and bank45/276: renewed native aim scenes each map to a new remote event (not exactly3). Visual overhead-arm gesture needs refinement; remote world mesh/FX/NPC outcomes pending. v75 strict equipment->world reader PASS8: native states1,3,2 observed, repeat identity/generation/decl/owner/transform guards. Late invalid samples after entity removal are NOT absence events. Finite20s fixture cleanup may cause despawn; natural expiry unproven. Evidence work/natural-moveset/mesmerize-owned-scenes-emily-v72, mesmerize-network-emily-v74, mesmerize-world-emily-v75; detailed MD documentation/handoff-2026-10-11/25-native-mesmerize-investigation.md.",
    "type": "Note"
  },
  {
    "address": "1401a31f0",
    "category": "D2Coop/Mesmerize-FX-stop-v79",
    "comment": "Exact D2 EXE B671F824...; raw decompilation work/ghidra-mesmerize-fx-stop-v79.json. Native FX stop traverses resources and requests particle stop; it does not immediately remove the effect ID map record. Read-only v79 watch retains identical ID1443/pointer0x2199a614450 after model hide and loses it ~3336ms later, before checkpoint. v80 and v81 paired runs pass12 including actual receiver effect-map absence after the native tail; live DLL7E4CF5EB... unchanged. Do not equate fxStopped telemetry to synchronous resource deletion. Target beams, AI effects and natural lifetime unverified.",
    "type": "Note"
  },
  {
    "address": "1407d91c0",
    "category": "D2Coop/Mesmerize-world-v76",
    "comment": "Native Mesmerize Midnight wrapper creates via1407d6040, starts via1407d9270 flags0x100002; original state1 shows entity first. Four-component cosmetic model with animation-only scenes: first immediate/hidden attempts v76/b failed; 200ms initialization and show/refresh before start jointly pass v76c/d. Paired native Steam v80/v81 pass12: committed source world states1/3/2, exact transform, one cosmetic model,3 scene starts,one owned native body FX plus stop/tail retirement, duplicate rejection and protected checkpoint recovery. v81 actual camera proof inspected: forward body gesture and dark feather form visible; purple eye/target beams absent. No AI/gameplay component on proxy; full power NOT complete.",
    "type": "Note"
  },
  {
    "address": "14072a400",
    "category": "D2Coop/Mesmerize-partner-filter-v91",
    "comment": "Verified exact local partner exclusion: v95 PASS13 on v94 DLL A80A734E... All629 eligibility attempts blocked after equipment deselection too;208 valid world snapshots zero targets; no receiver eye. v91 equipped/started gate failed after deselection; v93 repeated full discovery caused stale stream; v94 final validator failed on PowerShell empty-array counting, preserved. v94 cache is discovery only, fresh ownership/declaration/membership/generation checks each attempt; test guards recycling/ownership changes. v99 NPC control PASS13,551 partner attempts blocked,584 ordinary native eligibility accepted. v105 optimized reader PASS13,1031 partner attempts blocked,1041 ordinary accepted; protected14/checkpoints pass. Gameplay outcomes not synchronized. See documentation/handoff-2026-10-11/26-mesmerize-targets-and-partner-exclusion.md",
    "type": "Note"
  },
  {
    "address": "14072e5c0",
    "category": "D2Coop/Mesmerize-target-FX-v83",
    "comment": "Read-only target table component+1a8/+1b0 stride70: handle+0,beam/trail/headIDs+30/+34/+38,anchor+60,flags+6c/+6d. v99 ordinary native tutorial guard confirms beam/trail world-owner and head NPC-owner. Native update writes FX transform+38/+44 and beam/trail other endpoint+80/+8c but ALSO queues NPC events/damage: never invoke for presentation replication. v101 separate native FX-only implementation: world wire3/2056 bytes, <=5 opaque target identities/15 effects, all receiver FX owned by animation-only visual model, no NPC writes. v107 PASS14: one start/stop for each beam/trail/head;197/10/187updates;0refusal;source native effect IDs/target handles match receiver; source scoped eye/link maxima0.3833/0.4515ms over219calls,0fail;full world unknownReads71. Source/receiver videos inspected: purple beam and cleanup visible; target distant, precise alignment/moving-target acceptance pending. Gameplay/NPC positions not authoritative; recordings silent. Protected14/checkpoint cleanup passed. Exact archives work/movement-integration/candidate-mesmerize-target-links-v101; handoff26.",
    "type": "Note"
  },
  {
    "address": "1405a4b00",
    "category": "D2 Coop runtime crash",
    "comment": "Confirmed arkBTTask_Action_CombatGiveUp(vtable141b8aa68). v109 receiver-menu Mesmerize failure: source client exception584660, task+b0=1,+b8=null; actorhandle0c8311e6=209916390 exactly matches registered custom Corvo presentation clone. Caller5a4cbe invokes584660(context) without null check. Old followProbe releases clone decision hold on peer menu, resuming native AI. v111 retains existing pause ref while fresh exact local clone owner lives (no task patch); v113 confirms both AI refs/scheduler through32s, but separate guard damage fails test. v114 keeps finite movement-test guard isolation through menus. v116 receiver and v117 source menu each PASS17: both avatar AI refs exactly1, inactive scheduler; health/inventory protected14/checkpoint cleanup pass; paired videos inspected. Broader death/load/disconnect regressions pending. Evidence handoff27 and work/natural-moveset/mesmerize-links-*; exact archives v111/v114.",
    "type": "Note"
  },
  {
    "address": "14072f3d0",
    "category": "D2Coop/Mesmerize-lifetime-v124",
    "comment": "Exact EXE B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180. Static: 1407290c0 initializes component+1e8 from duration statistic; 14072f3d0 subtracts owner delta time and requests event0x100 at <=0 (or native stat predicate change). Runtime v129 PASS15, DLL C179CFD8..., probe C4AF30EF...: 241 state3 samples from19.95s to near zero; same handle reaches state2 with negative countdown; source and receiver native FX records retire while original power remains selected, quantity1 and40s equipment fixture active. No timer writes. v125 failed incorrect valid-absence expectation; invalid post-retirement owned-world reads are not evidence of absence. v120/v121 active-menu snapshot recovery PASS19 each; no recast. Shared NPC authority and arbitrary lifecycle unverified. See documentation/handoff-2026-10-11/28-mesmerize-cancel-lifetime-and-crouch.md.",
    "type": "Note"
  },
  {
    "address": "141c79cf8",
    "category": "D2Coop/Equipment-overlap-v154",
    "comment": "CONFIRMED exact B671F824 runtime v154 context-setup-samples: Emily native holster has Running0 + EquipChange64 + EquipChange53 concurrently. Both64/53 share this vtable but state+24 references distinct scenes206/207. State64 owns power_unequip.midnightscene Scene_1/333ms with one reciprocal owned left arm67225. State53 owns sword_unequip Scene_1/866ms right arm132762+sword198752. Strict two-state reader rejected overlap for >250ms, retiring stream. v155 targeted two-scene ownership reader under validation. Generic entry/exit methods140853d40/1408541a0 do not independently establish meanings. No native timer/first-person mutations.",
    "type": "Note"
  },
  {
    "address": "141c80ed8",
    "category": "D2Coop/Mesmerize-air-v163",
    "comment": "CONFIRMED exact B671F824 source observations: v151 armed Emily Jump2/Fall3 + UsePower63 holds Mesmerize aim loop (1200ms). v161 early LT release: Jump2-only and Fall3-only overlap exact out666ms; same reciprocal left arm67225, peak1.4026m, same-floor recovery. v163 A+LT entry: two Jump2+UsePower63/in466ms samples then Fall3+UsePower63/loop1200ms; peak1.4415m. Raw Midnight then state reads differ16–31ms, corroborated across adjacent rows and actual-game video. All source cleanup checks pass. This generic UsePower vtable alone does NOT authorize an action: exact owned selected Mesmerize item, scene incarnation/phase and left-arm bindings remain necessary. Empty-hand air and remote air integration are not proven by these observations.",
    "type": "Note"
  },
  {
    "address": "141c80ed8",
    "category": "D2Coop/Mesmerize-empty-air-v177",
    "comment": "CONFIRMED RUNTIME exact EXE B671F824...: source takes mesmerize-empty-air-entry-source-v175 and mesmerize-empty-air-release-source-v177 each pass12 incl native checkpoint/protected14 recovery. Sword holstered before A/LT, zero automatic draws across93/89 equipment samples. v175 Jump+UsePower entry466ms twice, Fall+UsePower loop1200ms once; v177 Jump+UsePower loop twice then Fall-only release666ms twice. All reciprocal owned Emily left arm67225. Raw scene/state reads are sequential, not atomic. Body43 candidate178 admits this observed empty-hand air case; remote integration must be verified independently. This is the generic UsePower vtable, never sufficient by itself to authorize a phase.",
    "type": "Note"
  },
  {
    "address": "141c3eeb8",
    "category": "D2Coop/Domino-static-v174",
    "comment": "CONFIRMED STATIC only, exact executable: RTTI arkPlayerStateUsePhantasmalLink vtable RVA1c3eeb8; Domino assets use internal PhantasmalLink names. Inventory models/powers/phantasmallink/phantasmal_link.inventoryitem, component declaration .../phantasmallink/phantasmal_link.cpntpowerphantasmallink. Component/decl vtables RVA1b52ec8/1b52d88. Scene power_phantasmal_cast.midnightscene has six subscenes plus a distinct slide asset; groups PlayerLeftArm and PlayerBody. 14068ed50 reads state+24 scene handle, +70/+71/+72 flags and input control; dispatches component slot120. 140447170 resolves equipment+0xc8 candidate target and +0xb8 network pointer, invokes network functions; this is gameplay and MUST NOT be called for cosmetic peer effects. 14068ecd0 toggles scene controls via7d9050. Acquisition, state ID, native ownership, actual binding counts, targeting outcomes and remote presentation remain unverified. Sources saved in ghidra-domino-entry/state-v174.json and Void phantasmal-native-v174 export.",
    "type": "Note"
  },
  {
    "address": "141c3eeb8",
    "category": "D2Coop/Domino-native-v187",
    "comment": "Confirmed runtime v187: exact isolated Emily armed standing native state id72 alongside Running0, PhantasmalLink vtable. Owned power_phantasmal_cast scene id206 retains incarnation across Cast_In266ms, Cast_Loop1333ms, Cast_Select400ms, Cast_Select_out266ms; two reciprocal bindings owned Emily left arm67225 and player1. Exit can persist with sole Running0. Native target/validated FX recorded; not proof of multi-target linking. Native inline equipment declaration unnamed has +48 exact named parent, supported by original phantasmal_link.def authored inline m_manaCost25 override. Native inventory/checkpoint/protected14 cleanup passed. Remote body/VFX/gameplay not yet integrated.",
    "type": "Note"
  },
  {
    "address": "1404db360",
    "category": "D2Coop/Domino-target-mark-v198",
    "comment": "Exact installed EXE, read-only v198 runtime: selected network component +28/+30 points to nodes in +68 pool (16 slots, stride0x150). Node +0 guard handle, +8 animation component generation, +10 AnimEvolved pointer. +90 FX ID, +94 temporary idEntityFx owner handle match validated mark. This function resolves the target then creates/attaches an idEntityFx and calls4e2940(node+90,networkDecl+88,fxEntity). Never call for cosmetic replication: it modifies actor flags and creates native ownership. v198 one selected guard only; multi-target connections/gameplay unverified.",
    "type": "Note"
  },
  {
    "address": "140447250",
    "category": "D2Coop/Domino-partner-picker-v211",
    "comment": "Exact18-byte native picker prologue verified independently via Capstone against original EXE. Wrapper calls original once; returns null only for current registered partner and selected owned Emily Domino component. v212 PASS9:1712 partner candidates blocked, no read failures,478 empty valid target snapshots,mana100 unchanged. v214 normal source two-guard native link PASS9. v218 paired guard regression PASS12:four body phases,one mark via native Steam,66 full-weight root samples,zero clock rejects,fade cleanup and14 protected hashes. Shared NPC outcomes and complete power scope remain pending.",
    "type": "Note"
  },
  {
    "address": "1401a31f0",
    "category": "D2Coop/Domino-mark-retirement-v208",
    "comment": "Native FX stop initiates authored particle tail, not immediate record deletion. v208 fixed700ms record test failed but ID stayed owned,frozen,no restarts; gone by4200ms. v209 PASS11:slot stop by700ms,native record absent3272ms afterstop. v218 PASS12 confirms candidate211 mark flow,owner retirement,and bounded nativefade. Keep1a31f0,no forced particle-manager destruction. Saved source decomp and exported authored FX/particles explain lifetime; no completion claim for links/gameplay.",
    "type": "Note"
  },
  {
    "address": "1404dd500",
    "category": "D2Coop/Domino-link-endpoints-v216",
    "comment": "Confirmed exact-executable native link effect endpoint setup via 1404e2de0: connection nodes +8/+10, progress +28, FX ID +2c / owner +30, start transform FX+38, end FX+80, callback +128 null. Strict reader source test domino-link-clear-view-v221 PASS10; candidate222 Domino world v2 size1000 transmits observed endpoints via native Steam. Paired domino-beam-network-v223 PASS12 functional, receiver camera occluded. Corrected domino-beam-view-v225 PASS12 with native beam visible in both recordings, exactly2 owned marks/1 beam, endpoint sampled separation ~6.84mm/0.356mm, native fade cleanup passed. Cosmetic remote avatar-owned FX only; no receiver NPC association, native connection gameplay calls, damage/AI/authority proof. Native cancellation and other body contexts remain pending. Source reader validates selected reciprocal NPC generations, directed pool identity, exact declaration/owner/callback/rigid transforms and endpoint bounds; no foreign handles serialized.",
    "type": "Note"
  },
  {
    "address": "14068eee0",
    "category": "D2Coop/Domino-state-callback-v230",
    "comment": "Static exact B671 confirmed five bytes c6417201 c3 set state+72=1. Registered by14068eb20 via event0x1a9; used by14068ed50 to call447520 target eligibility and select native subscene. Function name SelectionReady is inferred from callers, not a proven event name. No runtime call/hook added. Native X cancellation mechanism remains under isolated controller test.",
    "type": "Note"
  },
  {
    "address": "140447020",
    "category": "D2Coop/Domino-native-cancel-v233",
    "comment": "Static component vtable+110 requests stop of network+b8 via7e7c00 then base61bcc0. Do NOT equate with controller X aiming cancel. Actual candidate222 source/peer diagnosticsv232 prove X whileLT clears state72 but retains2targets/2directedconnections/1beam for11snapshots820..3506ms. Corrected independent testdomino-native-cancel-v233 PASS13: peer keeps same mark/beam incarnations, rootweight1, castweight0 idle stream; independent peer bankoff stops native cosmetics without altering source links.14protected hashes+bothcheckpoints restored. Native deselection of already-linked target is separate test. No gameplay function was invoked directly.",
    "type": "Note"
  }
]

````
