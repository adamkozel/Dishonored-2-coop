# Native damage families and ordered contacts, 902 onward

## Purpose and scope

This continues [generic NPC combat](73-npc-combat-development.md) toward normal shared gameplay. The native melee adapter is still opt-in. A catalog entry or successful local shot is not evidence that the corresponding family replicates. Native UI, Steam transport, original first-person control and existing movement banks are unchanged.

Preserved candidate896 is `44437162D7AA92E60BDF30249336BC3B089611D778C496366C05BE3DB875C56A`. Candidate905 is `CD704EE79E92928B349F159523D9481BCEB5935E82726433E6C274FD62C8B87B`; candidate907 is `F794D6974553FF253A7A8561749BBB429BD3ED66BCF2C883F3505B7251528515`. Each has full source and build/deploy scripts under `work/movement-integration/candidate-npc-combat-vNNN`.

## Exact asset catalog

The existing pinned Void reader exported 49 player/equipment damage declarations in [batch902](../../outputs/d2-coop-lab/research/asset-exports/npc-damage-families-v902/manifest.json) and 36 base damage declarations in [batch904](../../outputs/d2-coop-lab/research/asset-exports/npc-world-damage-v904/manifest.json). Both manifests record exact executable B671F824, export hashes and unchanged original index hashes. These are 85 selected declarations, not every damage declaration in the game.

Reproduction:

```powershell
& outputs/d2-coop-lab/Read-D2Assets.ps1 -NamePattern '^components/(equipment|characters/player)/.*\.damage$' -Export -Batch npc-damage-families-v902 -Limit 1000
& outputs/d2-coop-lab/Read-D2Assets.ps1 -NamePattern '^components/base/damage/.*\.damage$' -Export -Batch npc-world-damage-v904 -Limit 1000
```

The paths distinguish contact/projectile damage, gun spread, explosions, status effects, physics/corpses, fall/environment and scripted kill/sleep behavior. Do not turn these into 85 independent implementations. The source class, geometry form, native outcome and lifetime determine reusable behavior; declarations supply data. Inheritance and modifiers must be resolved before treating a textual maximum as an actual inflicted amount.

## Native pistol observation903

[Harness](../../work/natural-moveset/Observe-NpcRangedDamage-v903.ps1), [native callback data](../../work/natural-moveset/npc-ranged-observe-v903/damage.json), [source recording](../../work/natural-moveset/npc-ranged-observe-v903/host.mp4), [restoration](../../work/natural-moveset/npc-ranged-observe-v903/restoration.json).

On candidate896, native controller LT with the existing two-round gun fixture produced three related callbacks. Combat replication was disabled for this observation:

| Native declaration | Attribution | Native data and outcome |
| --- | --- | --- |
| `components/base/damage/gun_spread_damage.damage` | Player1, inflictor330010, matching the equipped gun | Flags `0x100000000`, amount15.1337957, reactions2/4, region1, region mask255, zero normal, unit direction. Victim40 to24.8662033. |
| `components/equipment/damage/bullet_regular.damage` | Player1, different inflictor210440868 | Flags `0x200`, amount20, reactions2/2, region2/mask4, nonzero normal/direction. Same victim24.8662033 to0; native inflicted24.8662033. |
| `components/base/damage/corpse_damage.damage` | Invalid/native absent attacker `0x3fff0010`, dead victim handle190777211 as inflictor | Flags `0x12`, amount1. A different living NPC50 to49. |

The first two happened at the same recorded millisecond, one on the engine thread and one on a worker. The corpse contact followed562ms later. Declaration pointers were resolved locally with the read-only object probe and stored in `declaration-0/1/2.json`; no pointer is a proposed network identity. Projectile ownership beyond the explicit native attacker remains to be traced.

Visual inspection of the source contact sheet confirms the pistol shot, victim reaction/death and nearby NPCs. The intended below-chest aim still yielded native region2; the code must honor observed native hit data rather than infer regions from camera intent. This recording does not establish a synchronized projectile, paired victim animation or matching corpse physics.

The old movement observation fixture lost two AI-reference release confirmations when its victim died, reporting `native_state_changed`, released8/lost2. This is retained in `isolation-after.json`; it is not a clean lethal-ownership test. The independent isolated checkpoint restoration completed, both players returned to56HP and protected hashes passed. New combat death tests use the dedicated native pre-death release path instead.

## Decompilation and exact death boundary905

Actual Ghidra calls and saved output:

- [Parameter constructor140780260](../../work/movement-integration/npc-damage-research-v902/params-140780260.txt), with [callers](../../work/movement-integration/npc-damage-research-v902/params-xrefs.txt): declaration+5C supplies the initial amount, +60 flags, +68/+6C reactions; geometry and handles receive native absent defaults. Dynamic input values are not necessarily equal to those defaults.
- [Health vtable slot141c51680](../../work/movement-integration/npc-damage-research-v902/health-subtract-vtable.txt) is `1c51588+F8`, pointing to718C80.
- [718C80](../../work/movement-integration/npc-combat-research-v869/health-subtract-140718c80.txt) calls [7177E0](../../work/movement-integration/npc-damage-research-v902/health-subtract-base-1407177e0.txt) to subtract the amount before actor-specific accounting. The decompiler's apparent setter arguments are imperfect; the established damage-entry call and virtual slot establish the component/float ABI.
- The already saved717950 native entry evaluates damage at+D0, invokes subtraction at+F8, and then runs its native zero-health/death path below0.5HP. Calling the evaluator a second time is not used as a prediction technique; its filters/modifiers may have effects.

Candidate905 adds a checked virtual-slot observer. Outside the exact engine-thread authoritative application it passes directly to the original function. Inside that scope, it validates current native component/actor/world identity and the pre-hit health, reads the **final native subtraction amount**, and releases that actor's owned pose/frame/AI references immediately before a lethal subtraction. Native damage evaluation, subtraction, reactions and death still execute through the original functions. The old heuristic based on weapon name and nominal10 damage no longer chooses retirement.

The same change removes the blanket thread-local bypass for nested damage callbacks. The primary application already calls the original entry directly; a nested callback for another bound victim now returns through ordinary capture and ordering instead of escaping synchronization.

Fractional damage conservation uses a bounded float comparison (approximately two rounding units). Observation903 demonstrates why `before-after == inflicted` can reject a valid native result. Genuine differences and nonfinite values remain refused. Cross-game result equality is still checked separately.

[Client test906](../../work/natural-moveset/npc-combat-client-v906/result.json) passes on905: native30/20/10/0, four subtractions and exactly one lethal subtraction in each game, four host commits/applications, duplicate rejection, shared checkpoint recovery and fresh combat negotiation. [Restoration906](../../work/natural-moveset/npc-combat-client-v906/restoration.json) passes. Source recording shows the native strikes and death; the observer recording was largely blocked by a door frame, so it does not establish paired visual quality. Harness908 repositions that observer.

## Compound contact ordering907

Previously the client discarded another captured event whenever the same actor had one outstanding request. Observation903 proves that a single weapon action can produce multiple callbacks before a reply, although ranged profiles are not enabled yet.

Candidate907 replaces that implicit one-contact limit with a per-actor FIFO of32 request IDs. The existing `pending` status field is the head ID. Acknowledgments must consume the exact head; duplicate and out-of-order outcomes cannot consume another contact. Terminal outcomes clear remaining contacts for that actor, and a new world rebuilds the channel. Capacity exhaustion stops the experiment explicitly rather than silently dropping damage. No packet-layout change is needed: combat remains version2/224 bytes.

Tests cover two queued contacts before any reply, wrong-order and duplicate acknowledgment, full capacity, wraparound, overflow preservation, gaps and terminal/new-world clearing. These prove queue policy, not actual replicated gunfire. Native908 is the integration regression; its final audit is linked from the combat chapter when published. A real multi-contact ranged request burst remains unverified.

## Next implementation requirements

1. Add a shared validated damage descriptor for contact and spread geometry, dynamic input amount and current source ownership. Use locally resolved declaration identities, not transmitted pointers or raw native144-byte blocks.
2. Validate the source at callback time: the gun's spread inflictor is the weapon, while the bullet has a different short-lived inflictor. Remote application must not pretend that every event is a sword.
3. Add explicit native blocked/zero-damage outcomes and protected-region results. Current positive-melee outcome assumptions do not cover those cases.
4. Separate host-owned world/NPC damage from player-origin requests. Corpse/environment events with no player attacker must not be attributed to the local player, and locally simulated replica side effects must not duplicate a host outcome.
5. Test representative ranged contact/spread and secondary-world effects, then broaden active AI, both-player targeting and campaign coverage. Keep visual corpse/limb agreement separate from numeric health acceptance.

The85 declarations and observation903 are analysis inputs. They do not count as85 supported weapons, complete ranged combat or complete playable co-op.

## Host regression909 exposed a real secondary world event

[Take909](../../work/natural-moveset/npc-combat-host-v909/result.json) failed on907 after four native subtractions and matching target health0. The exact refusal was `hit_damage_parameters` for `components/base/damage/corpse_damage.damage`, amount1, flags0x12, absent attacker and the newly dead guard as inflictor. The new nested-callback handling exposes this secondary contact instead of letting it escape unnoticed. [Restoration909](../../work/natural-moveset/npc-combat-host-v909/restoration.json) passed. It is not a complete host regression pass. The proposed publisher910 was never executed and must not be treated as evidence.

Candidate911 extends the same protocol with an explicit host-owned world source. Combat wire3/232 bytes appends a source NPC arm; the local corpse handle is resolved from a current, generation-checked NPC death binding in each process. The victim keeps its own normal actor binding and ordered event counter. Player requests cannot prescribe world damage or inject a source arm.

Only the observed corpse-contact profile is enabled initially: locally resolved declaration, amount1, flags0x12, reactions2/2, region1/mask0 and validated contact geometry. Source death completion and its ordered commit precede a host corpse outcome; the capture wait is bounded. The receiver accepts the authoritative outcome against its matching dead source actor, and its independent native corpse contacts on bound victims are suppressed instead of producing duplicate requests. Replica corpse position is not asserted to match host physics.

The native original entry still computes the result, including a possible harmless zero result for this profile. Positive/zero result agreement, world identity, source generation, exactly-once ordering and bounded lifetime remain required. The native health-subtraction boundary from905 handles lethal ownership. This is not generic acceptance of every environmental hazard or unbound corpse.

Harness912 adds a post-death settling check for active combat, equal world outcome counters, and health equality for every bound NPC. Positive world counters are needed to establish that a corpse collision was exercised; zero counters only establish a regression pass. Runtime results are pending until a final audit verifies them.

Take912 on911 failed before attacks: a client-local `bolt_incendiary.damage` callback (amount1, flags0x400020, absent inflictor and nonplayer attacker) stopped the narrow adapter. Both target guards remained40 and no authoritative subtraction occurred. The finite fixture was not ready; no combat recording was started. Restoration passed. This reproduces the architectural problem seen in earlier886/880, rather than establishing a corpse-implementation failure.

Candidate915 moves replica authority filtering ahead of damage-profile extraction. After validating the bound victim and current world, it suppresses a client-native world event unless an exact current local-player source is attributable through attacker or the validated native parent chain. Such a callback neither changes replica health nor generates a player request. The host still validates and orders its own world events; recognized player-owned callbacks still undergo full profile/geometry/range checks. Indirect player attacks whose owner cannot be resolved remain a coverage gap and require callback-time attribution before they can be supported. This changes a shared authority policy instead of adding an incendiary exception to the client.

## Authored map-trigger source917 and descriptor918

[Take916](../../work/natural-moveset/npc-combat-world-v916/result.json), candidate915, again failed on `bolt_incendiary.damage`, this time on the host after three synchronized sword hits (target10HP in each game). It did not establish corpse collision coverage. [Restoration916](../../work/natural-moveset/npc-combat-world-v916/restoration.json) passed.

The read-only exact-image lookup [host917](../../work/movement-integration/npc-damage-research-v917/source-host.json) and [client917](../../work/movement-integration/npc-damage-research-v917/source-client.json) resolved native attacker handle `0x0933054d` to the authored `arkTrigger` named **esc_env_trigger_4** in both restored instances. Its actor vtable is `base+1C5EAE8`; its sole `arkCpntTrigger` component uses `base+1C5BAE8`, with reciprocal component ownership and absent parent. These are post-restoration observations; the runtime adapter independently revalidates source identity when handling a new callback. The old local handle alone is not a cross-game identity.

Actual Ghidra [xrefs141C5BAE8](../../work/movement-integration/npc-damage-research-v917/trigger-xrefs-141c5bae8.txt) and [constructor1407792C0](../../work/movement-integration/npc-damage-research-v917/trigger-constructor-1407792c0.txt) confirm the component table and initialization. No new trigger function is called by the mod. The previously observed fire event is an authored-world source, not evidence of a player firing an incendiary bolt. This explains why NPC decision/reflex holds do not exclude it.

Candidate918 adds a common descriptor for **native map-trigger damage with absent hit geometry and no inflictor**. The descriptor contains an authored trigger-name key, a declaration-name key and the actual bounded damage input. It does not contain native pointers, raw damage memory, actor handles or prescribed health. The host alone creates such outcomes; player requests cannot carry the descriptor. The combat wire is4/256 bytes and rejects earlier layouts.

Each instance builds its own trigger catalog for the current world and rejects duplicate name keys. Before applying an outcome it rechecks actor type, exact local handle, component owner, component generation and name against that catalog. Local native declaration resolution uses the85 names exported in902/904, with key collision checks at generation. Exact declaration type, manager, name, maximum amount, flags and reaction defaults are validated. The callback amount may be less than its declaration maximum: the observed trigger uses1 although the incendiary declaration maximum is10. Input flags/reactions must match the local declaration. This is a lookup catalog, **not85 runtime-supported damage cases**.

Application uses the original native constructor and native damage entry. Damage evaluation and visual effects remain native. The checked subtraction hook releases ownership before any lethal result. A world event may yield a verified zero result with unchanged health, including native paths that skip subtraction entirely; positive damage still requires the subtraction observer. Replica-local world callbacks remain suppressed, so they cannot duplicate host outcomes. Duplicate world packets use the existing ordered victim event counter.

New policy tests cover trigger name ambiguity, component ownership, slot reuse, incarnation changes, unsupported declarations, malformed amounts, mixed player/world identities and old wire layouts. [Implementation script918](../../work/movement-integration/implement_script_damage_v918.py) records the declaration-catalog conversion and source changes; the archived candidate contains the final stricter zero-result check and duplicate-key fix. Build918 passes ownership/world, action, checkpoint, resume, proxy and both actual357-clip bank checks. Runtime verification is recorded separately; no broad environmental, active-AI or campaign acceptance follows from the build.

## Runtime distinction: natural activation, reconstructed event and continuation

Host919 passes native assassination/death, duplicates, all bound NPC health checks, checkpoint and fresh negotiation on918. Host920 adds a12-second natural map observation before native sword contacts; host921 adds one native-console `trigger esc_env_trigger_4` activation. Both also pass melee/death/recovery, but **all three have zero world outcomes**. Command consumption is not damage evidence. Their paired recordings are retained with visual reviews; full remote paired attack choreography remains visibly incomplete.

Candidate922 adds the explicit local command `coop_npc_combat trigger_fixture_once`, restricted to one use per active combat nonce, the host, a ready finite NPC test lease and the exact existing target/trigger identities. It reconstructs the previously observed native fire parameters with the native constructor and enters the actual hooked native damage entry. It does not write health. A pending request is discarded on combat shutdown; issuing it with combat disabled does not queue future damage. This is a **reconstructed-event fixture**, not evidence of natural trigger activation.

[Take923](../../work/natural-moveset/npc-combat-fixture-v923/result.json) on922 demonstrates one host world commit and one client native application, both yielding0 inflicted damage and unchanged40HP, with one observed native subtraction in each process. Fire effects subsequently appear in both recordings (source by~1s, partially occluded client by~3s in the inspected contact sheets). The full take fails and is not accepted: native ignition emits a subsequent `components/base/damage/incendiary_death.damage` event, which918/922 rejected because it has actual contact geometry. [Restoration923](../../work/natural-moveset/npc-combat-fixture-v923/restoration.json) passes.

The follow-up event retains the same native trigger attacker and absent inflictor. Its native amount is0, flags`0x80000c`, reactions0/0, region1/mask0, finite hit point and unit upward normal/direction. The exact Void declaration specifies INSTANTKILL, NO_REACTION and FIRE; zero input amount therefore does not imply a zero final native result. The game, not a nominal-damage heuristic, must choose the actual subtraction and death.

Candidate926 extends the shared map-trigger geometry validator to accept either canonical absent geometry or bounded contact geometry (within3m of the victim, finite unit vectors, observed region1/mask0). Native construction restores absent defaults only for absent geometry; contacts are reconstructed relative to the local victim. No declaration-specific incendiary handler is introduced. Tests cover zero-input terminal events and malformed/unbounded contact geometry. The explicit927 harness awaits the native effect continuation and death without issuing a sword action. Positive continuation evidence is recorded separately when available.

## Accepted scoped result927 and client regression929

[Audit928](../../work/movement-integration/npc-combat-audit-v928.json) verifies11 actual take receipts, retains the failed investigations, and adds21 recordings to the review catalog. Current [candidate926](../../work/movement-integration/candidate-npc-combat-v926/identity.json) has DLL`4CE3947C8453C56EE55AF2117B62EAD6E38D1E8ED4CF8DB68E7AB3A88CB2A2F1`, unchanged probe`EA5444F1…`, combat4/256, movement69, bank67/357. All source files match the preserved archive. This is scoped acceptance, not completion of combat or co-op.

[Test927](../../work/natural-moveset/npc-combat-fixture-v927/result.json) passes on926 with no sword input. The explicit ignition event produces a native0-damage outcome at40HP. Its native burning continuation then kills the guard40→0. A native collision from that corpse damages a second NPC50→49. [Settled receipts](../../work/natural-moveset/npc-combat-fixture-v927/settled-death.json) show3 host world commits,3 client applications and3 native subtractions in each game, exactly1 lethal subtraction and one client-local world continuation suppressed. Every bound NPC's health matches. Repeating the latest host outcome does not apply damage twice. Both native AI schedulers stop on the dead target and both owned references release before death.

Shared checkpoint recovery retires remaining old references, discards the old combat nonce and creates fresh bindings without stale outcomes. Independent restoration passes, both players return to56HP and protected hashes pass. [Test929](../../work/natural-moveset/npc-combat-client-v929/result.json) then passes the ordinary client-request path: three sword contacts plus assassination, native30/20/10/0 in both games, four commits/applications, one lethal release each, duplicate request/outcome rejection and shared checkpoint/rearm. All world counters in929 are zero; it is a melee regression.

The927 source recording visibly shows ignition, native flame effects, burning reaction and collapse. The client recording shows the burning victim behind remote Corvo, collapse and the burning body near his feet. Corpse placement and visual timing differ; shared corpse physics and matched choreography are **not** accepted. The929 observer similarly shows incomplete remote paired attack behavior despite matching native health. See the per-take `visual-review-v924.json` files and contact sheets.

Ghidra bookmark receipts [907](../../work/handoff-npc-combat-bookmark-v907.json) and [930](../../work/handoff-npc-combat-bookmark-v930.json) are saved to the exact executable project and merged into the handoff indexes.930 updates the trigger bookmark initially saved by918 with the actual927 outcome. Earlier failed909/912/916/923 remain in the audit. The unexecuted publisher drafts910/924 are not acceptance receipts.

Next work is a shared ranged contact/spread descriptor and reliable callback-time projectile attribution, then active-AI and both-player perception. Real compound ranged bursts, general protected/blocked outcomes, naturally occurring trigger activation, broader hazards/corpses, full paired animation/physics and campaign coverage remain open. Preserve the functional native world chain already proven instead of restarting it per inventory item.
