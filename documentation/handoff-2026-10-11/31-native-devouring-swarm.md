# Native Devouring Swarm investigation



The movement phase and playable co-op remain unfinished. This chapter starts Corvo's native Swarm investigation after the paired holstered Domino final-selection tests. Native first-person behavior and the original Steam assets remain protected. Findings below distinguish static evidence, interpretation and runtime validation.



## Exact source and assets



Executable SHA-256: `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`. The [read-only Void export](../../outputs/d2-coop-lab/research/asset-exports/native-swarm-v265/manifest.json) records original index hashes, archive locations, offsets and individual exported file hashes. It uses the existing pinned [Read-D2Assets.ps1](../../outputs/d2-coop-lab/Read-D2Assets.ps1), never an installer.



- Inventory: `models/powers/devouring_swarm/devouring_swarm.inventoryitem`.

- Equipment definition: `models/powers/devouring_swarm/devouring_swarm.def`.

- Component: `components/characters/player/base/powers/devouring_swarm/power.cpntpowerdevouringswarm`.

- Ordinary native scenes: `midnight/gameplay/player/base/powers/power_swarm_cast_in.midnightscene`, `power_swarm_cast_loop.midnightscene`, `power_swarm_cast_out.midnightscene`.



The installed component explicitly specifies Corvo, mana cost40/replenish20, the `DevouringSwarm` equipment state, separate sliding scenes, ordinary and large swarm definitions, the targeting effect and native sound events. Those declarations establish available resources; they do not prove runtime phase coverage, mana accounting or safe multiplayer spawning. Existing authored full-body clips remain candidates until native binding and visual review.



## Static types and functions



The [static RTTI reader](../../work/movement-integration/inspect_static_rtti_v265.py) reads the exact PE, verifies its SHA and matches the MSVC type descriptor, self-relative complete-object locator and vtable pointer. It requires executable method pointers. [Equipment tables](../../work/movement-integration/swarm-equipment-rtti-v265.json) and [declaration tables](../../work/movement-integration/swarm-declaration-rtti-v265.json) preserve all discovered slots.



| Evidence | Address / offset | Interpretation and limit |

|---|---|---|

| Component RTTI | type RVA `0x2304770`, vtable RVA `0x1b51f28` | Exact static `arkCpntPowerDevouringSwarm`; does not establish a live owner |

| Declaration RTTI | type RVA `0x2304738`, vtable RVA `0x1b51de8` | Exact static `arkDeclCpntPowerDevouringSwarm` |

| [Constructor](../../work/movement-integration/swarm-decompile-14043d640-v265.txt) | `0x14043d640` | Calls base constructor, writes vtable, initializes `+0x108=0`, `+0x110=-1`, dword `+0x118=-1` |

| [Begin candidate](../../work/movement-integration/swarm-decompile-14043c750-v265.txt) | slot `0x118`, `0x14043c750` | Activates sensor at component `+0xC8`, calls shared power behavior and optionally stores preview handle `+0xB0`; semantic name inferred |

| [Commit candidate](../../work/movement-integration/swarm-decompile-14043c950-v265.txt) | slot `0x120`, `0x14043c950` | Checks sensor, compacts stale handles at `+0xB8/+0xC0`, enforces count, uses virtual spawn path and stores latest entity handle at `+0xD4`; runtime relationship still unverified |

| [End candidate](../../work/movement-integration/swarm-decompile-14043c860-v265.txt) | slot `0x128`, `0x14043c860` | Stops sensor/audio, retires preview conditionally, calls shared cleanup `0x14061be50`; not permission to invoke directly |



Ghidra bookmark: `D2Coop/Swarm-static-v265` at `0x14043d640`. Decompilation and exact asset content are saved locally. Method names above are interpretations of the code, not exported game symbols. No game executable patch or remote swarm spawn has been added.



## Reader and isolated observation candidate



[Candidate265](../../work/movement-integration/candidate-swarm-inventory-v265/identity.json) preserves candidate264's verified Domino behavior. Its Swarm additions are the [read-only item reader](../../outputs/d2-coop-lab/src/native_player_swarm_item.h), `--player-swarm` probe and a finite [native inventory fixture](../../outputs/d2-coop-lab/src/swarm_fixture.h). The fixture requires Corvo, the exact Dunwall scene, active isolation, sufficient isolation lifetime, no competing movement/power test and an unowned native inventory entry. It uses the previously guarded native inventory acquire/equip/unequip APIs. Controller input alone invokes the power; the fixture does not call spawn, AI or damage functions.



The reader matches exact names, declaration and component RTTI, character, item quantity/slot, entity incarnation, parent, reciprocal component membership and stable rereads. An inline `unnamed` declaration is accepted only through an exact named parent of the same RTTI. This form is a guarded candidate until live evidence confirms it. Unit fixtures cover wrong character, foreign owner, recycled handle, wrong quantity, wrong parent and concurrent retirement.



The first build copied two unrelated Domino-specific test calls into the new reader tests and failed to compile. The [failed build log](../../work/movement-integration/swarm-inventory-build-v265-initial-failure.log) is retained. Those calls were removed, the generator corrected and build/identity/wire/load checks passed before deployment. No failed binary was deployed.



[Observe-NativeSwarm-v265.ps1](../../work/natural-moveset/Observe-NativeSwarm-v265.ps1) runs through the serialized native-operation runner. It records the exact item, source states, sword, raw Midnight scenes, component words, filtered spawned entities, mana observations and actual-game source video. It requires original inventory/health restoration, checkpoint cleanup and the14 protected hashes. AI decisions are held for the controlled fixture; this will not establish general rat behavior or shared enemy outcomes. Completed runtime receipts will be added after cleanup exits.



## Native source proof, v266



The first v265 runtime attempt stopped before acquisition because `Send-D2Command.ps1` did not yet register the finite Swarm command. Its failed receipt and protected-file check are preserved. The wrapper now permits only `20000`, `40000` or `off`; the [v266 run](../../work/natural-moveset/swarm-native-standing-observe-v266/result.json) passed all five checks after full cleanup. Candidate265's [runtime receipt](../../work/movement-integration/candidate-swarm-inventory-v265/runtime-verification.json) distinguishes this from its pre-runtime archive metadata.



Native controller input acquired the exact Corvo item, produced all three ordinary phases and spawned a native rat swarm. The source stayed standing with its sword equipped. Entry/loop used Running0 plus exact UsePower63; the owned release persisted after UsePower retired. Reciprocal bindings were Corvo's left arm and player body1. The [raw scene evidence](../../work/natural-moveset/swarm-native-standing-observe-v266/swarm-scenes.json) records `Scene_1` and durations400/333/1600 ms. Loop clock resets are native behavior. The source video and [contact sheet](../../work/natural-moveset/swarm-native-standing-observe-v266/source-sheet.jpg) show targeting preview, hand release and rats around the native guard; AI was held for this experiment, so this is not general rat AI validation.



The world entity used `models/characters/animals/rat/devouring_swarm.def`, distinct from the selected equipment entity. Its observed class was `arkSwarmEntity`, vtable RVA `0x1c44b98`. Inventory quantities and player/partner health were restored; a mandatory checkpoint reload retired the spawned swarm; all14 protected save/settings hashes passed. Mana100â†’80 is an observation after native replenishment, not proof of a20-point cast cost or synchronized resources.



## Full-body binding candidate, v267



The [bank builder](../../work/natural-moveset/build_swarm_bank_v267.py) appends three full-body clips to bank48, preserving all301 prior payload bytes. Bank49 has304 clips. New indices301â€“303 correspond to semantic phases23â€“25, native entry400 ms, loop333 ms and release1600 ms. Existing authored native-caster motion supplies the body; first-person data supplies timing and finger inspiration, without fixing wrists in space. Current native bone lengths/helpers and the armed sword grip/socket are retained; root transforms and gameplay events are excluded. Emily's equivalent clips are an offline skeleton comparison only; native Swarm remains Corvo-only.



The authoring [entry](../../work/natural-moveset/swarm-videos-v267/in-sheet.jpg), [loop](../../work/natural-moveset/swarm-videos-v267/loop-sheet.jpg) and [release](../../work/natural-moveset/swarm-videos-v267/out-sheet.jpg) sheets were inspected. They show a complete-body casting stance and standing recovery without the earlier disconnected leg deformation. These previews normalize each phase to about2 seconds; they do not establish acceptable native-speed timing. Candidate runtime review remains required.



The [strict scene reader](../../outputs/d2-coop-lab/src/native_player_swarm_scene.h) allows only those exact scenes, durations, reciprocal Corvo arm/body bindings and selected owned equipment in the observed standing/armed context. Release alone may finish on Running0 without UsePower63; non-loop clocks are bounded. Rereads reject retirement/recycling. New semantic protocol46 keeps the336-byte packet layout and refuses unsupported roles, durations, holstered and airborne Swarm. The receiver samples poses only; source rat spawning, AI, effects and damage have not been replicated by this change.



## Native loop observation and test repairs, v267â€“v270



The v267 binary builds and passes exact identity/wire/load checks, all bank formats1â€“49 and both actual bank49 files. Its paired run reached remote phases23/24/25, but the new probe watcher had not been added to the argument parser. That failed result remains preserved. Candidate268 fixes only probe registration; an invalid non-game PID confirms successful parsing followed by the expected game-identity refusal.



The native aiming loop creates successive scene incarnations. In [v268](../../work/natural-moveset/swarm-network-standing-v268/result.json), seven native loop scenes produced seven observed remote events, but the test incorrectly required a single scene per phase. In [v269](../../work/natural-moveset/swarm-network-standing-v269/result.json), the final native loop lasted about31 ms across two watcher samples; the slower status samples saw six of seven pose events, with the remote release event advancing to10. That observation alone could not distinguish a missing snapshot from a dropped event. Both failed takes completed checkpoint restoration and protected14 checks.



[Candidate270](../../work/movement-integration/candidate-swarm-history-v270/identity.json) adds a bounded32-entry power-event history for successful sends and fully accepted receiver states, reset with the stream token/world. It does not alter packet semantics or drive animation. The [new harness](../../work/natural-moveset/Test-SwarmNetwork-v270.ps1) checks exact event/phase/duration identities and native scene handles. Runtime completion is recorded separately from archive metadata. The [v268 peer sheet](../../work/natural-moveset/swarm-network-standing-v268/peer-sheet.jpg) shows Corvo entering a complete-body casting stance, holding his sword and returning upright; its [source sheet](../../work/natural-moveset/swarm-network-standing-v268/source-sheet.jpg) shows unchanged first-person targeting/release and native rats. Fine grip quality, native-speed visual acceptance and remote rat/VFX presentation are still open.



## Native world component findings, v267



The complete [v270 run passed ten checks](../../work/natural-moveset/swarm-network-standing-v270/result.json). Its [phase receipt](../../work/natural-moveset/swarm-network-standing-v270/phase-incarnations.json) matches one entry, seven renewed aiming scenes and one release against all9 sent/accepted events. All81 sampled body weights were1 and per-row clock rejection counts were0. The first continuity summary accidentally used PowerShell array `.Count` and wrote81 in the rejection summary; the checked raw rows were already0. The original summary is preserved beside the corrected metadata, and future summary generation now enumerates each row explicitly. Both inventories, player/partner health, checkpoint retirement and protected14 passed. [Paired visual review](../../work/natural-moveset/swarm-network-standing-v270/visual-review.json) is indexed in the dashboard; the full Swarm feature remains incomplete.



The [world definitions](../../outputs/d2-coop-lab/research/asset-exports/native-swarm-world-v267/manifest.json) inherit `ratswarm_base.def`; the [component export](../../outputs/d2-coop-lab/research/asset-exports/native-swarm-components-v267/manifest.json) identifies `arkSwarmEntity`, the rat-swarm component, combat/interaction/vision/attention components and native FX. Ordinary rats are aggressive and declare damage0.75 per agent. The native faction declares the player faction as an ally, which is not yet proof of partner-avatar exclusion. The upgraded swarm changes agent count and attack/eating timings.



[Static RTTI](../../work/movement-integration/swarm-component-rtti-v267.json) locates `arkCpntRatSwarm` primary vtable `0x1c42ab8` and secondary vtable `0x1c42c30` at object offset32. [Allocation](../../work/movement-integration/swarm-decompile-1406a2220-v267.txt) sets component array pointer`+0x68`, stride`+0x70=0x34`, count`+0x78`; each entry begins with invalid handle`0x3fff0010`. [Base initialization](../../work/movement-integration/swarm-decompile-1406a7200-v267.txt) copies owner entity coordinates into entry`+4/+8/+0xC`; the [rat override](../../work/movement-integration/swarm-decompile-1406a2a60-v267.txt) sets per-agent flags, including a candidate white-rat flag. Runtime array and visual entity ownership remain to be checked. The source v266 equipment`+0xD4` matches the committed world-swarm handle after release, consistent with the native commit decompilation.



The initial Ghidra lookup for `0x1408bb280` had no function. Disassembly established an8-byte address-return accessor; a local Ghidra function was created and [decompiled](../../work/movement-integration/swarm-decompile-1408bb280-defined-v267.txt). It returns `0x1439b1e50`; it is not a destructor or spawn call. No executable bytes were modified. Ghidra bookmarks `D2Coop/Swarm-runtime-v266` and `D2Coop/Swarm-agents-static-v267` preserve confirmed observations versus static interpretations.



## Remaining evidence



Grounded armed/holstered body contexts and a measured crouch walk now have paired native receipts, detailed below. Fine native-speed visual acceptance, other walking directions, air/slide contexts, sound and upgrades remain open. Remote world presentation must avoid duplicate AI or damage, respect partner exclusion and retire through cancellation, expiry, pause, checkpoint and world replacement. Full gameplay authority, complete movement and playable co-op remain unfinished.



## Grounded source contexts and body candidate, v271â€“v276



The source-only context matrix now has completed PASS6 receipts for [crouched armed v271](../../work/natural-moveset/swarm-native-crouched-observe-v271/result.json), [standing holstered v273](../../work/natural-moveset/swarm-native-standing-holstered-observe-v273/result.json) and [crouched holstered v275](../../work/natural-moveset/swarm-native-crouched-holstered-observe-v275/result.json). Native controller B and long-X establish the contexts; the holster input faces away from the guard to avoid pickpocket interaction. Every recorded row retains the requested stance and sword selection. Crouched casts use Sneaking6 plus UsePower63 and recover on Sneaking6; the native durations and reciprocal Corvo left-arm/body pair remain the same. Local first-person behavior is untouched. Source videos and temporal sheets are indexed; these observations alone do not establish remote body or world support.



The retained [v274 failure](../../work/natural-moveset/swarm-native-crouched-holstered-observe-v274/result.json) came from one non-atomic watcher sample: loop incarnation231 had eight consecutive reciprocal samples, then a zero clock and empty bindings, then disappeared. The source stayed crouched and holstered, and checkpoint/protected-file restoration passed. The revised [v275 observation harness](../../work/natural-moveset/Observe-NativeSwarmContext-v275.ps1) permits only this bounded terminal pattern: the same incarnation must have two reciprocal bindings immediately before, a zero clock with no bindings, disappear at the next sample, and both gaps must be at most100 ms. Populated foreign/non-reciprocal bindings still fail. The runtime reader never accepts an empty binding list. The repeat passed the entire acquisition, inventory and cleanup sequence.



[Bank50](../../work/natural-moveset/swarm-context-bank-manifest-v272.json) contains313 clips, preserving all304 bank49 payloads. The [builder](../../work/natural-moveset/build_swarm_contexts_v272.py) appends standing holstered, crouched armed and crouched holstered variants for each cast phase at indices304â€“312. It uses full-body donor deltas relative to the common entry pose, Posture100 crouch foundations, native bone lengths/helpers and corresponding hand/socket transforms. Nine paired-role authoring previews were rendered and their temporal sheets inspected. Crouched armed casting raises the right hand toward the upper chest and points the blade downward; fine grip and native-speed visual acceptance remain open. Emily's authored equivalents remain an offline skeleton comparison: native Swarm is Corvo-only.



[Candidate276](../../work/movement-integration/candidate-swarm-context-v276/identity.json) extends the reader to matching Running0/Sneaking6 grounded contexts and observed sword states. Wire47 retains336 bytes and rejects armed-only wire46 peers. The receiver selects or blends the corresponding whole-body context, retaining network root control and never executing native rat-spawn, VFX or damage events. Build, identity, wire, load smoke, bank formats1â€“50 and both actual banks pass. Paired context runtime verification is recorded separately after each test finishes its mandatory cleanup.



One new actual-bank test initially compared quaternion components literally. Corvo's crouched sword socket uses the opposite quaternion sign after shortest-path blending, which represents exactly the same rotation. Nine inspected cases had signed dotâˆ’1 and zero translation error. The [failed log](../../work/movement-integration/swarm-context-bank-actual-v276-initial-failure.log) and [diagnostic](../../work/movement-integration/swarm-context-quaternion-sign-v276.json) are retained. The test now checks rotation equivalence through absolute dot and still requires identical translations. No runtime or bank change was needed; the repaired actual-bank check passes.



## Live rat records and pipeline, v274â€“v277



The source [raw world snapshots](../../work/natural-moveset/swarm-native-crouched-holstered-observe-v275/world-components-0.json) distinguish initialization records from live objects. `arkCpntRatSwarm+0x68` has25 records at0x34 stride in this ordinary swarm. Its initial positions remain fixed between snapshots; they must not be treated as current network coordinates.



`arkCpntRatSwarm+0x80` points to live `arkRat` objects at0x108 stride (`+0x88`). [Static RTTI](../../work/movement-integration/swarm-live-rat-rtti-v276.json) pins the rat vtable to RVA`0x1c442a8`. The first three sampled objects link to `arkCpntReferenceFrame` at`+8`, `arkSwarmAgentEntity` at`+0x10`, their reciprocal swarm component at`+0x18`, `arkRatGroup` at`+0x20` and `arkCpntAnimRat` at`+0x78`. In the early v274 sample, coordinate-looking values at`+0xD0` changed while those at`+0x34` remained fixed. Later v282 snapshots show that`+0x34` also changes, disproving an initial-position interpretation of that rat-object field. Neither field is used as a network transform. [Derived observations](../../work/movement-integration/swarm-live-rat-observations-v276.json) preserve that distinction.



Component`+0x138` points to `arkRatFxPipeline`; [static RTTI](../../work/movement-integration/swarm-fx-pipeline-rtti-v277.json) pins its vtable RVA`0x1c44518`. [Rat serialization candidate `0x1406bab70`](../../work/movement-integration/swarm-decompile-1406bab70-v276.txt) reads transform-like records from pipeline`+0x138`, using index `(rat - *(pipeline+0x1A8)) / *(int*)(pipeline+0x1B0)` and0x20-byte record stride, plus secondary0x18-byte records from pipeline`+0x170`. This is a stronger route for tracing actual presented transforms than guessing from object fields; reciprocal runtime validation remains required.



[Rat initialization `0x1406baf20`](../../work/movement-integration/swarm-decompile-1406baf20-v276.txt) stores the rat object at its entity`+0x360` and invokes animation/component setup. [Rat cleanup `0x1406b33e0`](../../work/movement-integration/swarm-decompile-1406b33e0-v276.txt) retires effects and children, detaches the entity, increments rat`+0x44` and clears owner/entity/group pointers. These lifecycle details matter for rejecting recycled remote identities. Neither function has been invoked by the mod. Ghidra bookmark `D2Coop/Swarm-live-agents-v276` records the confirmed offsets and unresolved semantics; the analysis project was saved.



The [pipeline initialization path `0x1406b4a20`](../../work/movement-integration/swarm-decompile-1406b4a20-v279.txt) uses an explicit returned index list to select rat slots. It resolves each rat at `*(component+0x80) + *(int*)(component+0x88)*index` before calling its initialization virtual method. Therefore the firstN physical records cannot be assumed to be the active swarm. An exploratory [v279 observation](../../work/natural-moveset/swarm-native-world-holster-observe-v279/result.json) stopped when a physical slot did not have the expected active entity/frame links. Cleanup passed. The next observer retains raw prefixes before traversal and examines the component`+0x90` candidate index list; its exact runtime relation remains to be established. It never writes rat or pipeline memory.



The same pipeline function populates position-like XYZ at record0 and direction-like XYZ at record`+0x10` of the0x20-byte array, with scale at`+0x1C`, from the0x34-byte initialization records. The [generic allocation/index helper](../../work/movement-integration/swarm-decompile-1406be910-v280.txt) calls `0x140ff2200`, updates a bounded output list and calls `0x140ff2730`. The [pipeline destructor](../../work/movement-integration/swarm-decompile-1406b3c80-v277.txt) has a0x5B0 allocation size; [job setup](../../work/movement-integration/swarm-decompile-1406b3d20-v279.txt) registers embedded pipeline jobs. These names are interpretations of decompiled behavior, not game symbols. Bookmark `D2Coop/Swarm-pipeline-v279` was saved. No direct pipeline calls have been made.



## Paired crouch and Corvo holster correction, v276â€“v280



[Crouched armed v276](../../work/natural-moveset/swarm-network-crouched-armed-v276/result.json) passed11 checks: all three phases and all9 sent/accepted events matched,40 sampled phase poses kept crouch/equipment context, and60 continuity samples retained body weight1 with zero clock rejections. The source remained native; the remote body returned to crouch and then stood after controller B input. Original inventory, player/partner health, checkpoint retirement and protected14 passed. [Paired visual evidence](../../work/natural-moveset/swarm-network-crouched-armed-v276/visual-review.json) is indexed; dark side lighting limits fine sword-grip inspection.



[Standing holstered v277](../../work/natural-moveset/swarm-network-standing-holstered-v277/result.json) stopped before casting. The source holstered correctly, but both streams retired with `native_state_stale`; source read failure stage5 identified the equipment-scene reader. The native source [v279 holster trace](../../work/natural-moveset/swarm-native-world-holster-observe-v279/holster-midnight.ndjson) showed Corvo's separate power-unequip `Scene_1`/333ms bound to left arm67862 and sword-unequip `Scene_2`/1266ms bound to right arm133399 and sword198936. Native states64 and53 point to distinct scene incarnations225 and226. Eleven power samples and forty sword samples were recorded. The earlier dual-scene reader explicitly allowed Emily only, causing a real support gap. v279's later exploratory rat traversal failure does not invalidate these preceding raw holster observations; its failed overall receipt remains unchanged.



[Candidate280](../../work/movement-integration/candidate-swarm-holster-v280/identity.json) requires the power arm's character to match the validated sword arm/player, supporting both observed native roles. It retains separate reciprocal scene bindings, exact power duration, owner/child membership, generation checks and stable rereads. It does not relax the250ms stale-state timeout. New fixtures exercise Corvo's standing/crouched equipment cases and refuse foreign character arms, aliased scenes, expired clocks and concurrent retirement; existing Emily tests remain. Build, identity and load checks pass. Wire47, bank50 and all animation data are unchanged.



The corrected [standing holstered v280 run](../../work/natural-moveset/swarm-network-standing-holstered-v280/result.json) passed11 checks, including the preceding native holster, remote complete-body cast and holstered recovery. [Source and peer sheets](../../work/natural-moveset/swarm-network-standing-holstered-v280/visual-review.json) show native casting/rat arrival and the remote empty-hand body respectively. Remote rats, power FX/audio and authoritative shared enemy outcomes remain unfinished; this is scoped body/context support.





## Grounded context completion and measured walking, v281–v285



[Crouched holstered v281](../../work/natural-moveset/swarm-network-crouched-holstered-v281/result.json) passes all11 checks. Its9 native scene incarnations match9 sent/accepted events;61 continuity samples retain body weight1 and zero clock rejections. Both sheets were inspected: empty-hand crouched casting, return to crouch, then native B standing recovery. This supplements candidate280 without changing its DLL.



The two axis8000 tests [crouched armed v283](../../work/natural-moveset/swarm-network-crouched-moving-v283/walking-evidence.json) and [standing holstered v284](../../work/natural-moveset/swarm-network-standing-holstered-moving-v284/walking-evidence.json) each pass the11 context/network checks, but travelled only0.045898 and0.069244 world units respectively. Their maximum remote gait speeds were0.015692 and0.02344. They establish minimal-input context continuity, **not walking coverage**. The raw successful context receipts remain unchanged; separate walking-evidence files explicitly fail eligibility.



[Crouched armed axis12000 v285](../../work/natural-moveset/swarm-network-crouched-walking-v285/walking-evidence.json) travels1.257553 world units and records35 casting samples above0.1 gait speed, max0.426686,0.882694 gait cycles while moving and zero phase range after stopping. All8 native/sent/accepted events match,59 continuity samples retain weight1 and no clock rejections. [Visual review](../../work/natural-moveset/swarm-network-crouched-walking-v285/visual-review.json) records the viewed source/peer sheets. The remote body changes location and leg poses while retaining the cast, then returns to crouch and standing. Feet become partially cropped near the end; precise sliding and grip remain unaccepted. These recordings are in the dashboard.



The reproducible [walking analysis](../../work/movement-integration/analyze_swarm_walking.py) uses recorded source path length and remote speed/phase, independently of requested input. Its numerical gate does not establish visual quality.



## Indexed native rats and reader development, v282–v289



[v282 source observation](../../work/natural-moveset/swarm-native-world-indexed-observe-v282/result.json) passes6 checks, including crouched dual holster, native cast and checkpoint/protected14 restoration. [Decoded raw observations](../../work/movement-integration/swarm-indexed-pool-evidence-v283.json) and [decoder](../../work/movement-integration/analyze_swarm_pool_v283.py) retain both sequential snapshots. Component and pipeline pool pointers/0x108 strides agree; the bounded index list contains0..24. Three sampled rats have reciprocal owner component, entity+0x360 backlink, entity parent equal to the world handle, and owned/member frame and animation components. Sequential entity+0x300 and frame+0x30 reads can differ by0.072 because the rats move between reads. Rat+0x34 also moves; earlier initial-position language was a hypothesis, now corrected. Pipeline transforms sampled much earlier can differ by0.9. The new reader uses complete entity presentation transforms, never mixes those separate reads.



New exact static identities: agent entity vtable RVA0x1c44ce8, reference-frame0x1c573e8, AnimRat0x1b5b938, and [RatSwarm declaration](../../work/movement-integration/swarm-declaration-rtti-v283.json)0x1d25df8. [Agent creation/selection0x1406b9070](../../work/movement-integration/swarm-decompile-1406b9070-v286.txt) uses rat-swarm declaration+0xE0, obtains the entity, stores rat+0x10 and queries AnimRat into+0x78. [Initialization0x1406bc4f0](../../work/movement-integration/swarm-decompile-1406bc4f0-v287.txt) copies spawn-record position/orientation to entity+0x300 and velocity to+0x33C. These functions were decompiled, not invoked by the mod.



The [read-only world reader](../../outputs/d2-coop-lab/src/native_swarm_world.h) follows exact owned Corvo equipment+0xD4 into the committed Swarm entity; no name scan can authorize a world. Component membership, declaration, indexed pool capacity, rat/entity reciprocity, entity-table handles, component generations, finite rigid transforms and stable identity rereads are enforced. [Tests](../../outputs/d2-coop-lab/src/native_swarm_world_tests.h) cover nonzero pool slots, malformed/duplicate indices, foreign owners, wrong frame/animation membership, invalid matrices and retirement races.



Preserved failures and corrections:



- Candidate286 incorrectly required an unparented world. Its live run rejected244 samples at stage2. The raw world+0xAC value is1, identifying the native local caster. [Failed receipt](../../work/natural-moveset/swarm-native-world-reader-v286/result.json) and cleanup remain preserved. Candidate287 requires parent1 and tests refusal of an unparented substitute.

- An initial synthetic test incorrectly expected a stable, valid newly generated agent handle to fail discovery. A stable new handle is a new identity; mismatched table slots and changes during a read must fail. The initial failure log and corrected positive/negative tests remain preserved.

- [v287](../../work/natural-moveset/swarm-native-world-reader-v287/result.json) passes the original7 checks and validates23 complete25-rat snapshots, but219 later rows fail because rat+8 becomes zero during native state3. Its full-case pass did **not** establish sustained world coverage.

- Static [0x1406b8fc0](../../work/movement-integration/swarm-decompile-1406b8fc0-v288.txt) restores frame+0xD8=rat and rat+8=frame after a contextual attachment. Therefore rat+8 is the currently driven frame, not a permanent entity-frame pointer and not a death predicate. The native entity frame accessor uses entity+0x358. Candidate288 obtains that registered component and permits only direct reciprocal driving or state3 with the frame's own+0x20 driver. Foreign drivers still fail.

- [v288](../../work/natural-moveset/swarm-native-world-reader-v288/result.json) validates221/235 rows, but its old requirement for20 full25-rat samples fails because the native live index count shrinks. Six full25 snapshots were recorded. Candidate289 requires an initial complete25 snapshot plus sustained valid coverage of the currently indexed agents, rather than assuming that all rats must survive.

- Candidate289 adds [scoped equipment revalidation](../../outputs/d2-coop-lab/src/native_swarm_equipment_owner.h), derived from the already-tested Domino ownership pattern and restricted to exact Corvo Swarm identities. It rechecks current player/inventory membership, item range, type, generation, inline declaration parent and gameplay readiness every time, while avoiding unrelated first-person sword-pose scans. Tests include deselection, pending transitions, foreign inventory owners and invalid parents. Runtime result is recorded after cleanup below.



[Native agent assets](../../outputs/d2-coop-lab/research/asset-exports/native-swarm-agent-assets-v287/manifest.json) declare agent model definition`models/characters/animals/rat/rat.def`, max pool100 and normal count25. [Rat render/animation resources](../../outputs/d2-coop-lab/research/asset-exports/native-rat-render-v287/manifest.json) confirm rat.md6, rat skeleton/animset, native AnimRat tree with speed/turn/contextual inputs, audio and possession components. Asset presence does not authorize spawning autonomous gameplay rats on the receiver.



Ghidra bookmarks `D2Coop/Swarm-indexed-world-v286` and `D2Coop/Swarm-frame-driver-v288` are saved. [Filtered export v288](../../work/ghidra-bookmarks-export-v288.json) includes the new findings; confirmed observations and static interpretations remain separate. The working gameplay DLL remains candidate280 throughout these probe-only changes; bank50/313 and semantic wire47 remain unchanged.



## Sustained reader result and Steam transport candidate, v289–v290



[v289 native result](../../work/natural-moveset/swarm-native-world-reader-v289/result.json) passes all eight checks. It records 470 valid reads of 475, including 362 complete 25-agent snapshots. After spawning, 353 of 356 reads are valid (99.16%). The five refused reads comprise one component transition, three changing-pose reads and one identity-reread race; unknown data does not become confirmed absence. [Continuity evidence](../../work/natural-moveset/swarm-native-world-reader-v289/world-reader-continuity.json) and [source contact sheet](../../work/natural-moveset/swarm-native-world-reader-v289-sheet.jpg) are retained. The sheet was visually inspected: native first-person entry/aim/release, preview effects, spawned rats and subsequent moving rats are visible. This establishes source observation, not remote presentation quality. All 14 protected hashes and both isolated checkpoint restorations pass. The saved Ghidra frame-driver bookmark now records this result.



Candidate290 adds [Swarm world wire1](../../outputs/d2-coop-lab/src/swarm_world_protocol.h) and a [native Steam stream](../../outputs/d2-coop-lab/src/swarm_world_stream.h). The packed header is 124 bytes and each rat adds 72 bytes, with a maximum of 100 agents. Normal 25-rat snapshots therefore use 1,924 bytes. Unused capacity is not transmitted. The existing Steam SDK allows unreliable messages larger than MTU; missing fragments can drop a whole snapshot, so lease expiry remains necessary.



The sender revalidates exact native ownership and indexed identities, carries source handles only as opaque incarnation keys, and retains position/orientation, scale, native state and confirmed frame-driver mode. Session, map, both world epochs, role, stream tokens, root sequence, mapped clock and monotonic event/order checks protect the receiver. A new world handle requires a new world event. Unknown reads send nothing; confirmed absence can send a header-only clear. Presentation expires after 300 ms without accepted samples and is retired on stream or gameplay readiness loss. No receiver-side foreign handle lookup, AI, attack, VFX or rat spawn is performed.



[Packet tests](../../outputs/d2-coop-lab/src/swarm_world_protocol_tests.h), native identity tests and proxy load checks pass. Tests include variable message truncation/trailing bytes, maximum capacity, duplicate indices/entities, reflections, nonfinite scale, contextual driver restrictions and stale-checkpoint/replay rejection. The native reader now also rejects reflected matrices. Bounded histories retain a digest of the exact sent and accepted wire bytes for the paired live comparison. These digests are diagnostic evidence, not authentication.



[Candidate archive](../../work/movement-integration/candidate-swarm-world-v290/identity.json) preserves source, binaries and checks. DLL SHA256 is `1E95D3F9F2E94C5C2977D701183FE22F36E5643F0026F3C9A71BF9089C66B33D`; probe SHA256 is `363440D2ECB34C6B6A42643B307D260633F610EDE0CACCCB5986B96670BF903E`. Semantic wire47/336, bank50/313, Mesmerize wire4 and Domino wire2 remain unchanged. The [serialized live test](../../work/natural-moveset/Test-SwarmWorldNetwork-v290.ps1) adds exact source/accepted packet comparisons and stream/checkpoint retirement checks to the existing three-phase body proof. Its live result is pending at this documentation point; remote rat visuals, VFX/audio, native gameplay authority, upgrades and multi-swarm behavior remain unimplemented.



[v290 live paired proof](../../work/natural-moveset/swarm-world-network-v290/result.json) now passes all 14 checks: 117 sampled active peer states and 126 overlapping exact sent/accepted wire digests, zero rejected/unknown snapshots, and a 1.638 ms maximum scoped native read. Actual indexed counts were [10, 25]. The initial summary used PowerShell collection `.count`; the [corrected derivation](../../work/natural-moveset/swarm-world-network-v290/world-summary-corrected.json) preserves this reporting correction without altering the raw receipt. Source stream stop retires the peer lease; checkpoint reload clears presented agents and restores inventory/health/protected14. Source and peer contact sheets were inspected: source preview/rats are present, peer full-body cast/recovery remain visible, and peer rat models are intentionally absent from this transport-only candidate. This is not a remote VFX or gameplay completion claim.



## One-rat visual feasibility and unresolved animation output, v291–v294



[Candidate291](../../work/movement-integration/candidate-swarm-rat-visual-v291/identity.json) reuses native `rat.md6` in a dedicated generic `idEntity`, with only AnimEvolved, MidnightProxy, Audio and ReferenceFrame components. The definition has no SwarmAgent, attack, AI or possession component. It caches at most one model per world and follows one source rat's opaque handle/incarnation. Its owned Midnight scene is stopped and its model hidden on lease retirement; world changes discard cached pointers. It does not yet reproduce the entire group, source animation state/phase, individual scale, interpolation, effects or gameplay.



The [asset generator](../../work/natural-moveset/build_swarm_rat_visual_v291.py) builds animation-only idle/walk/run Midnight resources from the previously validated Mesmerize scene structure, bound to the rat model. Native Edge headers give idle01 1,267 ms and walk/run 333 ms. The original model/animset remain unchanged; animation-embedded audio is possible through the generic Audio component, but no gameplay component or Midnight event track is present. [Asset manifest](../../work/natural-moveset/swarm-rat-visual-assets-v291.json), [read-only native exports](../../outputs/d2-coop-lab/research/asset-exports/native-rat-visual-v291/manifest.json), isolated installer and backup manifests retain provenance.



[v291](../../work/natural-moveset/swarm-rat-visual-network-v291/result.json) passes 16 lifecycle/transport checks: one model, one native scene, 366 transform updates, no refusal, then scene stop/model hide and checkpoint retirement. The native scene has the exact owned model binding and its clock advances. The component probe confirms exactly four whitelisted native components. Source/peer wire overlap is128 packets; indexed count changes25 to9, zero rejected packets, one unknown source read, scoped reader maximum1.1033 ms. **These checks do not prove visible or animated rat output.** The inspected peer sheet does not clearly show the model, so visual acceptance remains false.



[v292](../../work/natural-moveset/swarm-rat-visibility-v292/result.json) preserves a harness failure: cumulative model-creation counters were incorrectly compared with1 after an earlier test in the same process. The code still limits the current world to one model. The corrected harness compares counter differences against its own baseline. [v293](../../work/natural-moveset/swarm-rat-visibility-v293/result.json) then exits on `native_state_stale`; checkpoint/protected14 cleanup passes. Its earlier read-only diagnostics remain useful independently of that failed full run.



[Probe292](../../work/movement-integration/candidate-swarm-rat-probe-v292/identity.json) adds a narrowly scoped `--rat-skeleton` mode: dedicated cosmetic definition, unparented generic entity, four reciprocal component types, exact rat MD6/skeleton and bounded cache reads. [Early v293 skeleton](../../work/natural-moveset/swarm-rat-visibility-v293/visual-early-skeleton-0.json) is stable with31 joints. Two samples120 ms apart have identical matrices in all four cache phases. The tree is present/active but reports no output. The entity is unhidden (`entity+0xD0` low bit0) and has unit scale. **Correction after v294 inspection: the entity/render transform remains at the initial spawn position; the changing sourceTransform telemetry is only the requested target, not a measured body position.** Active scene clock and update counters were insufficient evidence of actual pose evaluation or moving presentation.



[Candidate294](../../work/movement-integration/candidate-swarm-rat-mainclip-v294/identity.json) is a reversible asset-only experiment setting the clip's main-track flag and moving the observer closer to the floor; DLL291/probe292 remain unchanged. The original false-main assets are preserved in candidate291. [The live result](../../work/natural-moveset/swarm-rat-mainclip-v294/result.json) passes16 lifecycle/transport checks, but the main-track change does not fix animation: the two early31-joint samples remain identical across all four phases, with tree output0. Visual inspection of the peer sheet shows a visible brown rat in a fixed pose and location. This confirms visible native model compatibility only. Do not expand the rat pool or claim swarm visuals complete until changing pose and visible playback are established.



## Live transform correction, v295



[Candidate295](../../work/movement-integration/candidate-swarm-rat-transform-v295/identity.json) replaces spawn-local setters0x87c320/0x87c2c0 in the update loop with the already established live dispatcher0x87d200. Its [decompilation](../../work/movement-integration/swarm-decompile-14087d200-v295.txt) shows component notification, entity+0x300 transform update and renderer update. The new path verifies exact API prefix, owned generic entity, reciprocal ReferenceFrame component and its expected transform vtable entry before invoking the native function on the engine thread. Actual actor transform is then read back and compared with the requested position within1mm. The test requires actual travel of at least10cm, separately from animation output. Build, identity, packet and DLL-load checks pass. Assets294 and all first-person/AI/gameplay paths are unchanged.



[Live295 passes17 checks](../../work/natural-moveset/swarm-rat-transform-v295/result.json), including checkpoint/protected14 restoration. The [transform receipt](../../work/natural-moveset/swarm-rat-transform-v295/live-transform-summary.json) has104 temporal samples,423 verified dispatches, zero measured position error and axis extents0.986m/0.770m. Source and peer sheets were inspected at1.5,2.5,3.5,4.5,5.5,7.5,10.5s and both videos indexed. The cosmetic rat visibly moves/turns with the source identity. Walking remains unverified: joint-local changes are at most6e-8, consistent with numerical noise, and the graph output flag remains0. The source native gameplay swarm is not duplicated. Dashboard library now725 videos; total clip count is not completion percentage.



## Direct native walk graph experiment, v296



[Candidate296](../../work/movement-integration/candidate-swarm-rat-leaf-v296/identity.json) uses a dedicated AnimEvolved declaration pointing to a minimal native `animleaf` graph, with `rat_walk`, full skeleton weight and repeated playback. Native model, skeleton and animset remain intact. Graph syntax comes from the already installed native-style Doppelganger graph. The cosmetic path no longer invokes Midnight scenes, retaining the verified live transform/lease/hide behavior and exact four-component whitelist. This is a feasibility test, not native-rat AI attachment. The harness now requires at least4 significant joint-local matrix changes above0.01 over120ms, so root motion and numerical noise cannot count as walking. Build/load checks pass; isolated install/runtime/visual result is pending.



[Initial296](../../work/natural-moveset/swarm-rat-leaf-v296/result.json) fails before cosmetic creation: native action stream expires with `native_state_stale`. The peer rejects samples as213ms in the future under the unchanged80ms freshness gate. The last state ages265ms; root traffic stays fresh. Source reader misses are0. This remains a clock-mapping reliability issue to investigate, not evidence against the animation graph. Checkpoint/protected14 restoration passes. The same archived build/assets were repeated without changing freshness rules.



[Repeat297 passes18 checks](../../work/natural-moveset/swarm-rat-leaf-v297/result.json). [Joint-local evidence](../../work/natural-moveset/swarm-rat-leaf-v297/animation-output-summary.json) has21 joints changing above0.01, including arms, wrists, toes and femur, with native tree active/output both1. [Transform evidence](../../work/natural-moveset/swarm-rat-leaf-v297/live-transform-summary.json) records442 verified dispatches, zero measured position error,104 samples and1.323m/0.904m axis extents. Both temporal sheets were inspected: one cosmetic rat visibly moves and walks; full-body Corvo cast/recovery remains intact.112 active world samples/127 exact wire overlaps, zero rejected/unknown world packets, max scoped read1.0042ms. Both recordings are indexed, bringing the library to727; no full visual or gameplay completion claim. This confirms direct native `rat_walk` playback on the generic cosmetic entity and favors it over the unresolved Midnight route.



## Cosmetic pool, v298



[Candidate298](../../work/movement-integration/candidate-swarm-rat-pool-v298/identity.json) expands the verified native graph/live-dispatch path into100 bounded slots, keyed by source pool index and guarded by source handle/incarnation, event, stream tokens and local world epoch. At mostone new model is allocated per engine frame; each slot caches one entity until world retirement. Model identity uses the same exact dedicated definition/four component whitelist. Stream/identity disappearance hides its owned model; no native Rat/SwarmAgent/AI/attack component is added. The normal25-rat test requires25 unique local entities, matching source identities across20+ temporal samples, per-model live transform readbacks and full lease/checkpoint retirement. Build/load checks pass; runtime is pending.100-rat upgrade capacity, packet interpolation, source speed/phase and scale, VFX/audio and shared outcomes remain pending.



[Live298 passes19 checks](../../work/natural-moveset/swarm-rat-pool-v298/result.json):25 unique cosmetic models match all25 source identities across100 full-pool samples. [Pool receipt](../../work/natural-moveset/swarm-rat-pool-v298/pool-summary.json) reports max service time1.6848ms at capture; telemetry later reaches1.7192ms during retirement.9,609 actual transform readbacks have zero measured position error; sampled rat has21 changing joints/tree output1. The peer recording visibly contains the full animated normal swarm. All25 models hide after source stream stop, and checkpoint reload retires caches/restores health, inventory and14 protected files. Both temporal sheets were inspected and videos indexed, library729. Native source rats have scale/color variation absent from this unit-scale brown cosmetic candidate. Constant walk rate and packet-rate positions remain visible-quality limitations. No remote gameplay-outcome claim.



## Source-time interpolation, v299



[Candidate299](../../work/movement-integration/candidate-swarm-rat-interpolation-v299/identity.json) preserves the validated normal pool and native walk graph but samples between the currently presented rat snapshot and the next accepted packet at the existing80ms delayed presentation clock. XYZ is linear; orientation uses shortest-arc quaternion interpolation. It holds without extrapolation on missing samples and across source handle/incarnation/event/session/world/map/token changes, world absence, gaps above250ms or teleport distances above3m. Every native live dispatch still has actor readback. Pure tests cover diagonal/vertical motion, rotation and retirement boundaries; existing pose interpolation/protocol/DLL-load tests pass. Native paired proof is pending. This changes presentation only, without new wire, native game events or first-person controls.



[Live299 passes20 checks](../../work/natural-moveset/swarm-rat-interpolation-v299/result.json). [Interpolation counters](../../work/natural-moveset/swarm-rat-interpolation-v299/interpolation-summary.json) show6,447 of9,584 actual native updates interpolated between samples(67.27%); the remainder held a current position. No extrapolation.25 models remain matched across99 full-pool samples; maximum captured pool service2.1194ms and scoped source read1.1459ms.113 active world states/127 exact wire overlaps, zero rejected world snapshots, one unknown source observation. Both sheets inspected and videos indexed;731 recordings. Protected14 and checkpoint restoration pass. The67% interpolation coverage shows the80ms source cadence often leaves no following packet in the80ms presentation buffer; this is a measured limitation, not full smoothness acceptance.



[Candidate300](../../work/movement-integration/candidate-swarm-rat-cadence-v300/identity.json) reduces the known-owner sample interval from80ms to33ms(maximum about30Hz; actual engine cadence may be lower), retaining500ms undiscovered-owner probes, the same strict scoped native reader, payload cap and80ms presentation delay. Normal25-agent payload is1,924 bytes, about58KB/s at30Hz before transport overhead;100-agent upper bound is7,324 bytes/about220KB/s. The paired test requires85%+ of native model updates to have bracketing samples, and records source/pool cost. Build/load checks pass; live result pending. No freshness gate is relaxed, no game or damage authority changes.



Additional static AnimRat findings: initialization0x14048e860 stores a native manager reference at+0x460 and resolves inputs into+0x468/+0x470; generic animal initialization0x14048f7b0 resolves contextual inputs at+0x428/+0x430. [Decompilation files](../../work/movement-integration/swarm-decompile-14048e860-v291.txt) preserve the raw evidence. Precise input meanings and a safe cosmetic equivalent still require confirmation. Native animation binding stores a rat object at component+0x410; this reinforces why the initial cosmetic experiment avoids attaching native AnimRat to an arbitrary generic entity. No new AnimRat native function was invoked.



## Measured cadence, native gait inputs and buffering, v300–v302



[v300 passed21 checks](../../work/natural-moveset/swarm-rat-cadence-v300/result.json):8,286/9,549 actual native updates were interpolated(86.77%),25 models stayed matched across101 full-pool observations, source scoped read peaked at0.8009ms and captured pool service at2.4587ms.112 live world observations and127 exact source/receiver wire overlaps were retained, with no rejected or unknown world snapshots. Both video sheets were inspected;733 recordings were indexed through this run. Checkpoint restoration and protected14 hashes passed. This supersedes the preceding pending status for300.



[Candidate301](../../work/movement-integration/candidate-swarm-rat-gait-v301/identity.json) adds a dedicated native idle/walk blend tree. Exact owned float inputs `ratRate` and `ratMoving` are located through the declaration input table, with graph/data/owner identity rechecked before each write. Initial bindings commit only after all checks pass. Horizontal presentation displacement divided by elapsed real time drives walk rate, normalized by the native rat_walk average speed1.11m/s and capped at3. The idle/walk blend changes over125ms. Source local first-person, rat AI and damage remain native/source-owned; cosmetic models contain no RatSwarm, RatAgent or possession component.



[v301 failed its overall test](../../work/natural-moveset/swarm-rat-gait-v301/result.json) because bracketing fell to84.36%(7,215/8,552 updates), below the unchanged85% threshold. Preserve this failure. Its preceding [gait observations](../../work/natural-moveset/swarm-rat-gait-v301/gait-summary.json) nevertheless verified2,388 named-input readbacks:2,106 moving and202 held samples, rates0–1.944737. The native graph produced output and21 joints changed locally over120ms. Source and peer [contact sheets](../../work/natural-moveset/swarm-rat-gait-v301/peer-sheet.jpg) were inspected: native rat models move across the peer view, but all remain unit scale/brown while the source varies size/color and has cast effects. Static sheets do not certify fine foot-slip or temporal phase acceptance. Failure occurred before source-stop assertions; final cleanup independently restored both checkpoints and all14 protected hashes. Failed footage is retained outside the pass-only dashboard index.



[Candidate302](../../work/movement-integration/candidate-swarm-rat-buffer-v302/identity.json) increases only cosmetic rat presentation delay from80ms to120ms. The33ms known-owner source cadence, packet validation,250ms stale/80ms future admission checks,300ms lease, source ownership and85% test gate remain unchanged. This adds40ms rat visual latency to allow another engine frame of jitter; it does not change the local player's controls or the remote avatar delay. Native runtime result is pending.



Static scaling investigation: [new decompilation](../../work/movement-integration/swarm-decompile-scale-driver-v302.txt) identifies0x140753630 as a driver scale setter. It follows driver+8 to the reference-frame component, writes frame+60/+64/+68, compares entity+330/+334/+338 and queues dirty-frame processing via0x1407521f0. Rat initialization0x1406baf20 also writes render+1890/+1894/+1898 before invoking this setter. Reference-frame driver binding0x140751050 establishes reciprocal frame+D8/driver+8; generic self-driver is frame+20. Native render flush0x14087c5d0 copies entity scale to render. These are static facts; no cosmetic scale mutation has yet been tested. Source301 transmitted25 distinct scales spanning0.805127–1.199097. Any implementation must validate the owned generic driver and confirm native frame/entity/render scale, not only a written input.



## Clock regression correction and measured gait result, v303



[v302 failed](../../work/natural-moveset/swarm-rat-buffer-v302/result.json) at5.814s with `native_state_stale`; its final semantic rejection11 reported a mapped sample183.88886ms in the future. Both checkpoints and protected14 were restored. Source clock diagnostics were absent in302, so the exact cause cannot be claimed from its receipt alone.



[Candidate303](../../work/movement-integration/candidate-swarm-rat-clock-v303/identity.json) fixes a reproduced mapper defect. Previously a same-session receive gap over250ms rebased the source clock to the first queued packet's arrival. A270ms delayed packet could therefore shift the mapping forward and make the next fresh action appear270ms in the future. The corrected mapper retains its established offset through receive gaps, permits bounded drift(up to1ms/s and5ms per update), applies existing freshness/monotonic gates after gaps, and commits clock fields only on acceptance. New session/map/world identity can still establish a new mapping. [Regression tests](../../outputs/d2-coop-lab/src/pose_interpolation_tests.cpp) cover queued stale/fresh recovery, delayed valid samples, unchanged offsets on rejection, discontinuities, scope replacement and the prior cached-root rearm regression. Existing semantic/world protocol tests and load checks pass.



A bounded128-entry diagnostic ring records root clock initialization, clear, long-gap acceptance and rejection. Its [client evidence](../../work/natural-moveset/swarm-rat-clock-v303/clock-before-cleanup-client.json) exposes a separate startup limitation: an initial loaded-world clock estimate around151ms could reject fresh packets until the native readiness boundary cleared it. The following gameplay scope established a much smaller offset and passed.303 therefore fixes outage rebasing, **not every clock synchronization problem**. A queued first packet still needs robust initialization/synchronization work. The diagnostic ring may overwrite the earliest events during prolonged rejection; improve retention if investigating startup.



[v303 passes22 checks](../../work/natural-moveset/swarm-rat-clock-v303/result.json).9,369/9,735 native updates interpolate(96.24%) with120ms rat delay;25 models remain matched across102 full-pool samples. Native gait input samples2,524 include2,440 moving and17 held, rates0–1.752749;21 significant local-joint changes confirm native animation output. World evidence retains110 active rows/126 exact wire matches, no unknown/rejected world packets, scoped source read maximum1.7641ms and captured pool service2.4489ms. Both video sheets were inspected and indexed, bringing the dashboard manifest to735 recordings. Source-stop, checkpoint, inventory/health and all14 protected hashes pass. Color/size variation and source effects remain visibly absent on peer models; gait speed/phase remains subject to full video acceptance.



[Candidate304](../../work/movement-integration/candidate-swarm-rat-scale-v304/identity.json) attempted native size replication using0x140753630 on the owned generic self-driver. Constructor0x14074f5e0 confirms self-driver vtable0x141c574c0. The exact driver backlink guard refused the operation before invoking scale: the generic self-driver backlink is initially null. [The failed receipt](../../work/natural-moveset/swarm-rat-scale-v304/result.json) and checkpoint/protected14 cleanup are preserved. This build was never a verified scale implementation.



## Generic model scale ownership, v305–v307



[Read-only305](../../work/natural-moveset/swarm-rat-scale-observe-v305/result.json) restores303's unit-scale behavior and records the frame layout. Frame+D8 points to its own +20 driver with the expected native vtable, but driver+8 is zero. Its25 cosmetic models receive9,693 updates with no refusal. The test fails its fixed25-rat sample threshold: the source naturally retires a rat, leaving18 complete25-rat snapshots and80 complete24-rat snapshots. The corrected test matches every currently indexed source identity rather than assuming survival; unique handles, complete source matching,25 cached creations and cleanup remain required.



[Candidate306](../../work/movement-integration/candidate-swarm-rat-scale-bind-v306/identity.json) uses native0x140751050 to bind only the existing owned self-driver and verifies reciprocal links before scale dispatch. [Runtime306](../../work/natural-moveset/swarm-rat-scale-bind-v306/result.json) confirms that binding succeeds, but all scale readbacks return to1, and the guard hides the refused models. Its25 cached creations are not duplicated despite repeated refusals. Checkpoint/protected14 restoration passes. Native binding alone is therefore insufficient.



The [reference-frame decompilation](../../work/movement-integration/rat307update-decompile-v307.txt) explains the distinction:0x140753c10 has a self-driver branch that copies authoritative entity+330/+334/+338 back to frame+60/+64/+68. By contrast, [native rat initialization](../../work/movement-integration/rat307init-decompile-v307.txt) binds the external RatAgent as driver before calling0x140753630. Earlier descriptions of that scale setter as generally usable on the self-driver were hypotheses, disproved by306. The [transform dispatcher](../../work/movement-integration/rat307transform-decompile-v307.txt) and render flush remain separately established.



[Candidate307](../../work/movement-integration/candidate-swarm-rat-scale-owner-v307/identity.json) writes only the three authoritative scale floats on the exact owned cosmetic entity, then calls native0x14087c5d0 to update the render object. Normal native self-frame processing must settle the frame scale within500ms. Guards require the dedicated definition, four component identities, entity-table ownership, exact unparented self-driver and reciprocal render/animation ownership, finite previous scale and exact native API prefix. There is no external RatAgent, AI, source-entity or player mutation. This is explicitly a guarded field write with native propagation, **not** an inferred general native scale setter. Runtime and visual result are pending.



Ghidra analysis hygiene: the temporary search boundary0x14087c000 began inside an instruction. The saved `CorrectScaleSearchBoundary.java` script verifies original bytes and restores analysis disassembly at0x14087bffc (`MOVSS XMM0,[0x141ab3810]`). Only database code units were repaired; the executable bytes were never modified. The persistent Ghidra project was saved after the repair.



[v307 passes23 checks](../../work/natural-moveset/swarm-rat-scale-owner-v307/result.json). There are2,358 settled scale observations,25 distinct source sizes from0.802210 to1.186279, and95 complete-pool snapshots. Frame/entity/render XYZ readbacks all match. Gait samples include2,288 moving and33 held;20 joint-local changes exceed0.01.7,867/8,610 updates interpolate(91.37%);107 active world rows and125 exact wire matches have no rejected/unknown snapshots. Scoped source reads peak1.2809ms and captured cosmetic service peaks3.8244ms. Stream-stop, native checkpoint inventory/health restoration and protected14 checks pass.



Both307 contact sheets were viewed and paired recordings indexed, bringing the library to737. Rat size variation now appears on the receiver; mixed skins and source summon smoke/circle remain absent. The peer camera crops much of the remote caster. On-sheet HUD rates drop from roughly96 before the pool to27–38 with it; source shows114–213 early and55–66 later. This is an observed performance concern, not an isolated timing attribution: both processes, capture, rendering and native animation can contribute. Profile before expanding toward100 cosmetic rats. Passing transform/input counters do not establish frame pacing or final visual quality.



## Startup clock calibration candidate308



[Candidate308](../../work/movement-integration/candidate-swarm-pose-bootstrap-v308/identity.json) quarantines a newly observed root scope before publishing its clock to action/world streams. It requires8 distinct monotonic samples, at least250ms of both source and arrival span, and at least100ms of near-minimum observations within20ms. The minimum observed arrival-minus-source offset removes the specific first-packet queued bias recorded in303. During calibration, published source session remains zero; no side stream can treat the provisional offset as established. New scopes clear the old root buffer. After initialization,303's bounded-drift mapper and unchanged80ms future/250ms stale rules apply; ordinary receive gaps never restart calibration.



Regression tests cover the151ms startup bias, compressed arrival bursts, duplicate/invalid observations, changed scopes, startup interruption, and post-calibration stale/fresh outage recovery. A separate16-entry scope-boundary history preserves initialization/clear records even if the128-entry rejection ring wraps. This is a one-way presentation-clock estimate, not a proof of accurate clock synchronization over arbitrary Internet latency. Build, clock/protocol tests and load smoke pass; native result is pending.



[v308 passes23 native checks](../../work/natural-moveset/swarm-pose-bootstrap-v308/result.json). Two startup boundaries establish offsets1.3914ms and1.8431ms after358.84ms and486.41ms, respectively. The second required10 samples; the steady mapper later drifts to20.65ms under its existing1ms/sec bound. The preserved clock log contains no steady rejection records. All25 sizes settle,2,388 gait/scale samples pass and8,210/8,772 rat updates interpolate(93.59%). Both temporal sheets were inspected and indexed(739 recordings); color/effects/performance remain open. This run and deterministic151ms-bias test support the specific calibration repair; they do not replace network-latency testing on separate machines.



## Performance instrumentation candidate309



[Candidate309](../../work/movement-integration/candidate-swarm-rat-profile-v309/identity.json) adds fixed-size per-world counts, totals and maxima for cosmetic identity validation, gait inputs, scale propagation, live transforms and overall pool service. Transform includes an identity check; total includes all stages, so stage times must not be added naively. Separate source-present and source-absent categories record callback intervals. **Correction to earlier per-frame wording:** rat service currently runs inside the10ms transport-pump gate. Actual callback frequency is capped at100Hz and may be lower; it is neither the render frame rate nor proof of display smoothness.



The309 harness keeps all existing gameplay/ownership checks and captures11 quarter-second observations with the cosmetic pool active, then11 after stream retirement hides it. Both video recorders and the native scene watcher must have exited successfully before these quiet windows. Source native rats remain source-owned. This comparison reduces observation load but still includes two games and background tooling. No performance optimization is included in309; measured attribution is pending.



[Initial309 failed](../../work/natural-moveset/swarm-rat-profile-v309/result.json) after successfully collecting all11 active25-model observations. A simplified PowerShell `Where-Object activeModels -gt0` was parsed as a nonexistent parameter. The [310 harness correction](../../work/movement-integration/rat-profile-harness-correction-v310.json) uses a scriptblock comparison, validated against the actual309 observations; the309 DLL is unchanged. Cleanup/checkpoint/protected14 checks pass in both attempts.



[Repeat310 passes24 checks](../../work/natural-moveset/swarm-rat-profile-v310/result.json). The [quiet comparison](../../work/natural-moveset/swarm-rat-profile-v310/performance-summary.json), computed by the [counter-delta analyzer](../../work/movement-integration/analyze_rat_profile_v309.py), measures81 active service calls averaging1.44594ms within31.74596ms observed intervals. Identity checks account for1.15957ms per service, gait0.07463ms, scale0.10940ms and live transform0.43632ms; transform includes one of the identity checks.140 hidden calls average0.02073ms within17.88212ms intervals. Instrumented pool service occupies about4.55% of the active interval. There is substantial remaining time outside this routine; these counters do not attribute it to native animation, GPU, synchronization or another mod subsystem. Stream retirement also stops other semantic/world work, so the comparison is **not a pure rendering A/B**. No performance improvement is claimed. Both310 sheets were viewed and paired recordings indexed(741 total); mixed skins, summon VFX and full gameplay remain incomplete.





## Native effect ownership investigation and continuity diagnostics, v311–313



[311](../../work/natural-moveset/swarm-fx-observe-v311/result.json) failed because the new read-only probe mode was missing from the argument allowlist. It exited before reading the game; its zero-byte FX file is not evidence that effects were absent. Existing23 gameplay checks and protected cleanup passed, but the whole run remains failed. [312](../../work/movement-integration/candidate-swarm-fx-observe-v312/identity.json) corrects admission. An invalid-PID CLI smoke reached the explicit “not Dishonored2.exe” guard(exit4), confirming command admission without touching the game. The first smoke assertion incorrectly expected exit3; that assertion was corrected after reading the code and output.



[Native312](../../work/natural-moveset/swarm-fx-observe-v312/result.json) collected476 observations,458 stable and18 rejected as changing. [Its FX summary](../../work/natural-moveset/swarm-fx-observe-v312/fx-observation-summary.json) contains13 IDs: one aiming marker, one summon burst, seven rat trails and four death effects. The burst record remains present for at least11,922ms, despite its authored emitter duration of1s. Record lifetime is not emission duration. Trails last approximately1.8–2.2s in the observed map and use temporary owners with no entity definition; name/proximity alone cannot prove the rat attachment. Death effects use `fx/worldfx.def`. No receiver FX is created in312.



312 fails body continuity: four sampled weights at3.984–4.296s fall below.95, minimum.254896. Clock rejection counters stay0. Action telemetry records one packet253.26ms old exceeding the250ms admission limit. The old telemetry does not identify the precise eligibility predicate responsible; attributing the drop to a specific buffer reset would be a hypothesis. Do not relax freshness gates based on this run. Both checkpoint restores and protected14 hashes pass. The two contact sheets were viewed but this failed run is not indexed as passing.



[Candidate313](../../work/movement-integration/candidate-swarm-diagnostic-v313/identity.json) adds a bounded16-record diagnostic for movement eligibility. Mask bits:1 missing semantic sample;2 unsupported physical context;4 expired stream lease;8 peer unready;16 state absent/older than250ms;32 unsupported action context;64 native shadow action owns actor. It records pose/semantic times, state age and physical context on failure. Existing predicates and freshness limits are unchanged. The external research watcher now polls every100ms and reads the validated equipment's current world handle. Its checks remain read-only. Camera pitch20 improves the combined caster/floor view, though a pillar still partly occludes Corvo.



[313 passes23 checks](../../work/natural-moveset/swarm-diagnostic-v313/result.json): minimum body weight1.0 across53 continuity samples, no clock rejections and no eligibility failures. [137 FX observations](../../work/natural-moveset/swarm-diagnostic-v313/fx-observation-summary.json) include135 valid equipment-bound reads. All108 summon observations have an owner equal to the exact power component+D4 world handle. This supports source attribution. It does not establish that reducing observer load fixes the312 reliability issue; the failure did not reproduce. Both sheets were inspected and paired videos indexed(743 total). Visual acceptance remains false; rat color, native action states, VFX and shared outcomes still need work.



Read-only exports are in [native-swarm-fx-v311](../../outputs/d2-coop-lab/research/asset-exports/native-swarm-fx-v311/manifest.json). Exact static evidence: [world-spawn function1406a7a30](../../work/movement-integration/swarm-spawn-function-decompile-v313.txt), [allocation call sites](../../work/movement-integration/swarm-fx-allocation-calls-v312.txt), [rat FX helpers](../../work/movement-integration/swarm-fx-helpers-decompile-v312.txt). Correction: the summon axis is a random Z rotation produced through1408ac000/14010e6a0, not the world entity's orientation. Origin comes from entity+300. Component declaration+A0 supplies the FX;19d100 allocates it with the world owner;1a2ff0 starts it. Rat+80 is a generic effect ID used for more than spawn, including burning; rat+104 ownership remains under investigation. The Ghidra project was saved with confirmed/static distinctions.



## Source-owned summon burst candidate314



[Candidate314](../../work/movement-integration/candidate-swarm-spawn-fx-v314/identity.json) adds a separate176-byte native Steam cosmetic packet and a native-derived effect. Source discovery requires a newly observed world after stream baseline, the current strict equipment/world relationship, exact declaration+A0, unique FX owner identity, origin within10cm of the world actor, a current cast25 and a750ms discovery window(max8 scans). An already-present world is baselined and cannot replay on late join. Sender and receiver consume the event once, even if native playback fails.



The receiver validates sessions, worlds, map, tokens, source clock and ordering; then requires the matching presented Swarm world and power event before starting the finite effect. It never resolves source handles in the receiving game or invokes Swarm/AI/damage APIs. The effect is owned by the remote character, anchored to the observed source transform, stopped after1600ms or ownership retirement. A dedicated isolated resource keeps the native summon particle and sound event and removes only rumble. [Asset provenance](../../outputs/d2-coop-lab/assets/swarm_spawn_remote.provenance.json), [installer](../../outputs/d2-coop-lab/Build-D2SwarmSpawnFxAssets.ps1), original-index hashes and clone backup receipts preserve reproducibility. Native sound-event configuration is not an auditory quality check; current videos have no audio track.



Protocol tests cover malformed packets, wrong worlds/roles, stale cast, reflected/nonfinite matrices, duplicate source effects/casts/worlds, late-join baselining and bounded discovery. Build/load/protocol tests pass. Runtime/visual result pending. This first burst path does not cover the aiming marker, per-rat spawn trails, death/burning/despawn, attack sounds or shared damage.



## Candidate315: native summon effect verified in both instances

Candidate314 started the remote effect, but the duplicate-test command was absent from both command allowlists; the test failed and cleanup passed. It is preserved and is not indexed as successful. Candidate315 admits only the exact `coop_swarm_fx duplicate` command and preflights it before casting.

`work/natural-moveset/swarm-spawn-fx-v315/result.json` passes24 checks. One176-byte source packet was sent; the receiver accepted/applied one and rejected the deliberate duplicate. Source/peer native FX ID, event identities, world identity, origin and orientation match. One owned native effect was observed and retired. Discovery took0.2631ms maximum in this take.53 body samples retained weight1.0 with no presentation-clock rejection. Both isolated checkpoint recoveries and all14 protected save/settings hashes pass. The operation exited0 and was consumed before further native work.

Both temporal sheets were viewed at1.5,2.5,3.5,4,4.5,5.5,7.5s: the main rotating dark effect appears near arriving peer rats at4-4.5s; the source also shows additional rat trails that the peer does not yet replicate. A pillar partially occludes the body. These contact sheets do not prove native-speed timing, grip or final visual quality. Recordings were captured without audio; retained native sound configuration is not auditory verification.745 videos are indexed after this take.

Candidate directory: `work/movement-integration/candidate-swarm-spawn-fx-v315`. DLL SHA256 `E39FE945183C10FA6E4365812EAD7408D821D9372E5B12561C14BF4D33F07B54`; probe `EC60730DCCE141C73F9C81E82AFDD4082F86AE3226C0BF5EF2B954C9241023D6`. Asset/provenance and source are pinned there. Candidate314 identity inherited the earlier rat-only asset map; its summon asset is independently pinned by copied provenance and installation receipt.315 recomputes the full archived asset map.

### Separate rat-trail attachment investigation

Static native6baf20 creates temporary FX owners and attaches them through875e80→752900. The child reference frame is entity+358; frame+10 reciprocates child, frame+3c8 points to attached parent, frame+3c0 stores binding text, frame+3d0 is joint index. This is distinct from entity+AC, which is not a valid trail-to-rat ownership link. Native attachment stores the parent pointer near the end of752900 and registers it through7511f0/751c80. Full decompilation is `work/movement-integration/swarm-frame-attachment-decompile-v315.txt`; these offsets are static findings pending strict runtime rat-pool attribution. No generic effect presence is treated as a new gameplay event.


## Native rat-trail ownership and version2 transport, v316–318

Read-only316 linked128 samples to7 source rats through their temporary FX owners. Full316 failed the absolute summon-counter assertion: existing process totals were sent2/applied2/received4/rejected2, including the previous cast. Both checkpoint restores and protected14 passed.317 fixes the harness to capture per-run baselines; no gameplay DLL or probe changes. Its complete24-check native test passes in the reused instances, with bodyweight1.0 and no clock rejection.317 observes7 trails on indices0–6, all joint2 named `center_spine_jnt`; attachment summaries are saved beside both takes. Observed record spans in317 are1640–2297ms, longer than the declared0.7s particle emitter; record presence is not repeated emission. Both317 temporal sheets were viewed and747 videos are now indexed. Source trail ribbons remain absent from the317 receiving implementation.

Ghidra bookmark `140752900 / D2Coop/Swarm-trail-attachment-confirmed-v316` is saved in the persistent project. Full child registration helpers7511f0/751c80 are saved in `work/movement-integration/swarm-frame-children-decompile-v316.txt`. They route parent-child registration through878fd0. The read-only reader validates current equipment→world, indexed native RatAgent pool, RatAgent+18→world component, RatAgent+10→actor, actor+360→RatAgent, FX owner→ReferenceFrame and its rat parent/binding/joint with rereads.

Candidate318 adds optional trail fields to each world agent: native FX ID, age since bounded first discovery, and12-float native world transform. Agent wire size is128bytes (was72), world protocol version2, header124bytes;25 agents require3324bytes and capacity100 requires12924bytes. Previous wire versions fail validation. Max7 unique trail IDs, finite rigid transforms within2m of their rat and age<=2500ms are required; absent fields must be zero. Source discovery is baseline-aware, limited to16 scans over750ms, stops after7 identified trails and reads cached validated records for at most2500ms after birth. This observed-duration bound covers the current initial-spawn fixture, not all upgrades or future trail classes.

Receiver uses the exact existing native `rat_spawn_trail_01.fx` particle declaration (no sound/rumble in this declaration). It creates at most one effect for each source trail ID first admitted within200ms of discovery, carries tombstones across lease gaps, interpolates only matching rat/trail incarnations on the existing120ms source timeline and updates only its own callback-free FX record. Identity, source retirement, stream/world changes and a3s fallback lease control cleanup. No RatAgent, native power, damage or first-person animation event is invoked. The effect is owned by the remote avatar while explicit source world transforms place it near the corresponding cosmetic rat; this does not attach gameplay components to that rat.

Build, proxy-load checks and protocol tests pass, including malformed/expired/duplicate trail fields, capacity bounds, exact decode/digest preservation and interpolation boundaries. Candidate318 runtime and visual verification are pending; its archive pins source, DLL, probe and assets. Do not infer final visual acceptance, shared Swarm AI/damage or complete movement from these changes.


## Candidates318–320: trail lifecycle and idle reader performance

Candidate318 started7 peer trails and verified411 guarded transform updates, but classified3 native end-of-life records as ownership refusals. The failed take and both viewed sheets are preserved, not indexed as a successful case.319 distinguishes native completion from explicit source retirement and retains the last actual refusal reason. It completed7 effects (4 stopped,3 finished), refused0, updated416 and interpolated389 samples with no readback error. Its full test nevertheless failed body continuity.

319 eligibility evidence isolates the failure to mask1 (no semantic sample), with no native movement-clock rejection. At time361479803.1765, poseTime361479798.084459 was235.505105ms after the final semantic sample361479562.579354; the200ms hold correctly refused it. State receipt age was94ms. The root pose was only5ms behind current time despite the normal80ms delay; root-buffer restart was a hypothesis, not proven by that take. Additional root-buffer fields were added in320. Both checkpoints and14 protected files passed. Raw status files were preserved under `swarm-trails-v319/preserved-*` before further testing.

Code inspection found that idle frames searched all816 inventory entries twice for a grenade scene that usually did not exist, then repeated the full search for held-grenade presentation.320 first finds a native grenade scene before resolving the full item. Idle held presentation now checks only slot1, requiring in-range aligned entry, unique equipment membership, exact declaration/component identity, equipped/started agreement and matching double reads. Actual equip/unequip/cook/release scenes retain the full reader. Freshness,200ms semantic hold,250ms leases, replay checks and root interpolation behavior remain unchanged. Per-reader timing is exposed as `sourceReadTimings`.

[320 result](../../work/natural-moveset/swarm-trails-v320/result.json) passes25 checks. Source maximum action read falls from100.8959ms to10.931ms; maximum send gap from281ms to78ms. All53 measured body samples retain weight1.0 and zero clock rejection.7 trails start and complete, with533 guarded transforms and487 interpolated updates, no refusal or readback mismatch. Source `sourceReadFailures=7` still includes normal expired records; this counter is not a classified error metric. Viewed source/peer sheets show summon ribbons and smoke around the rats, then clearing by late recovery. A pillar occludes part of Corvo. No auditory or fine-grip acceptance follows from these images.

[Reader regression receipt](../../work/movement-integration/candidate-swarm-trails-v320/reader-regression-verification.json) records Corvo high throw and Emily low throw,13 checks each, plus native source equip/unequip and crouching checks. Both action recordings were visually inspected and indexed. Native first-person animation, selected held prop, cook mechanism, spark effect, release hide and empty recovery remain functioning in these fixtures. Source projectile/explosion/damage remains local. All regression cleanup and protected-file checks pass. Failed takes remain preserved separately.

Candidate320 DLL `B088BCBA3302E5A8D1A78CE282C816A638022995BB69BFCC1626A0109EC72A50`; probe `527A8D81167204270CB424790C4F852FD708EF6CB7E2E4E23AE0F1A9B6CCAED4`. The archive pins source before subsequent Possession edits. The current review library has753 recordings; all completion/visual acceptance flags remain false. This improvement does not complete Swarm gameplay, the full moveset, or playable co-op.
