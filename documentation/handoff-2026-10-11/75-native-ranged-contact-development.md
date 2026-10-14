# Native ranged damage: callback identity and ordered contacts

Updated 2026-10-14. This is active development; complete combat, movement and co-op acceptance remain open.

## Preserved baseline and candidate chain

Candidate926 remains the last accepted scoped melee/world milestone (audit928, chapter74). Its source, binaries, movement banks and evidence were preserved. Candidate931 adds bounded read-only attacker/inflictor snapshots before native damage can remove a projectile. Candidate932 adds the same snapshots to the existing NPC combat capture queue and lets the ranged equipment fixture use the combat fixture's native AI ownership. Candidate934 adds experimental pistol spread and regular-bullet transport, wire5/276 bytes. Runtime acceptance must be read from the subsequent receipts, not inferred from the build.

The original and isolated game executable remain pinned to SHA256 B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180. Source and builds are under `outputs/d2-coop-lab/src` and `work/movement-integration/candidate-npc-combat-v{926,931,932,934}`. Original Steam executable, Steam DLL, protected saves and movement banks were not modified by this work.

## Confirmed findings

- Existing native shot903 produces spread and bullet callbacks, then corpse contact. Spread has a variable amount, flag0x100000000, reactions2/4, region1/mask255, zero normal and a unit direction. Regular bullet has flag0x200, reactions2/2 and an individual hit region. The damage amount after native modifiers can exceed the bullet's base20 and is clamped by remaining health.
- Observation933 confirms the spread inflictor is `arkWeapon`, vtable RVA1ce1e98, selected gun handle330010 and parent1. Bullet inflictor is `arkProjectile`, vtable RVA1c5e758, parent1, a generation-validated transient entity. Both callbacks identify attacker1, exact `arkPlayer`, vtable RVA1c5e428. Ownership is captured while the callback still owns valid entity handles. Do not infer that arbitrary projectile families share these exact predicates.
- Native damage params reflection1415a1b40 names offset48 `m_hitDistance`. Candidate934 transports that finite field as well as the hit location relative to the victim, direction, normal and body-region fields.
- **Correction to an earlier hypothesis:** the class range at1439ce350..354 identifies `arkPlayer`, not `arkNpc`. A read-only live descriptor at module+39ce2f0 explicitly names arkPlayer. Function1405bf1f0 tests that range. Function140719d20 conditionally applies hit-region multipliers based on this source predicate and victim predicate1405bf560. The remote Doppelganger is arkNpc; equal native evaluations therefore require evidence. Merely using the same declaration and base amount is insufficient proof for all combat states.
- Function14039eec0 returns damage attacker+54, rather than falling back to the inflictor. Function140717dc0 filters by direction and health declaration policy. Native717950 evaluates via health vtableD0, reports an available inflictor to the impact manager, then subtracts through vtableF8 and invokes native reactions/death. The existing final-subtraction ownership retirement is retained; the evaluator is not called a second time to predict lethality.

Raw decompilations, field-name bytes and runtime descriptor proof: `work/movement-integration/npc-ranged-research-v931`. The source-class finding was saved as a Ghidra bookmark at1405bf1f0 and the persistent program was saved. These observations supplement, rather than replace, runtime tests.

## Candidate934 design and bounded scope

`NpcCombatRangedDamage` contains a declaration key, amount, hit distance and weapon family. No process pointers or native handles cross the network. Supported declarations initially are the base pistol spread and regular player bullet. Exact declaration RTTI, manager, name, default amount, flags and reactions are checked locally. Spread and single-region contact have distinct geometry validation. The current experimental range bound is32m, not a validated claim about every native weapon range.

At callback time the selected native gun, attacker, inflictor and parent chain must agree. A bullet may be removed before the queued event is serviced, so servicing does not attempt to recover ownership from an expired projectile handle. Source snapshots are diagnostic local data with bounded storage.

Host orders outcomes. Client requests use the existing bounded32 FIFO; `pendingHighWater` records whether multiple contacts were actually outstanding. Constructors create local damage parameters, and native damage runs once. Local spread uses the equipped native gun as inflictor; candidate934 required a validated attached cosmetic gun for remote spread (made optional by candidate936 below). Regular bullet uses a native absent-inflictor default because a gameplay projectile is not spawned during damage replay. This does **not** establish complete projectile or impact-VFX replication; native impact behavior that depends on an inflictor remains a gap. Attacker attribution is retained as the relevant local player/remote avatar.

Native damage, death, duplicate handling and checkpoint tests remain mandatory. The existing source-class distinction and independently evaluated victim AI state can cause divergence; do not hide divergence by writing health directly or calling a test pass based only on a host result. Limb severing randomness, paired victim animation and corpse physics are separate visual and gameplay coverage gaps.

## Diagnostic failure retained

`work/natural-moveset/npc-ranged-inspect-v933/result.json` is a failed test, not a replication pass. Its early status read preceded the combat status writer and incorrectly reported zero callback captures. The later `failure-state.json` contains the spread capture and native observer's bullet identity. Candidate932 intentionally lacked a ranged profile, rejected spread and released the combat fixture (10 references released, zero lost); subsequent native bullet damage was local only. The test independently restored both checkpoints, player health and protected files. Useful identity evidence is retained without reclassifying the failed result as success.

Build934 passed the native world/ownership, action, checkpoint/resume, proxy smoke and actual357-clip bank checks. Build and deployment scripts are versioned alongside the candidate. Use fresh isolated PIDs, exact physical executable paths, start-time checks and the named native-operation mutex for all further tests. `Test-NpcRanged-v935.ps1` records both games and checks native damage/death, duplicate rejection, shared checkpoint, renewed combat nonce, old ownership release and restoration.

## First replay failure and candidate936

Take935 on candidate934 is a retained failed host test. Native source callbacks were captured twice, and the host applied spread40→28.4413147 and a native lethal bullet28.4413147→0. The receiver rejected spread at preconditions because its cosmetic gun attachment was inactive. The test fixture did not enable remote ranged presentation. Host released all10 temporary AI references, with zero lost; both isolated checkpoints and protected files were restored. This is not synchronized damage acceptance.

Candidate936 makes the remote spread inflictor optional. A validated, active cosmetic gun supplies additional impact context when present; otherwise the native constructor's absent-inflictor value is retained. Source ownership still comes from the authenticated event and callback-time native ownership proof. Requiring a render attachment to be ready is inappropriate as a prerequisite for a reliable gameplay event. The receiver retains remote-avatar attacker attribution. Full source-sensitive impact VFX remains unverified; this change does not fabricate a gameplay projectile.

## Scoped gameplay proof and presentation integration

Candidate936 passes both host937 and client938 native pistol tests. One controller press generated spread plus regular bullet contacts, two ordered host commits and two receiver applications, matching native death, duplicate rejection and shared checkpoint/rearm. Client938 reached `pendingHighWater=2`: the bounded FIFO was exercised by two real outstanding native contacts, not just a synthetic queue unit test. Source937 evaluated spread11.5352631 followed by lethal28.4647369; client938 evaluated spread15.9795208 followed by lethal24.0204792. Both instances agreed within each take. Different source/aim contexts must not be interpreted as a role-independent damage calibration.

World regression940 on candidate936 produced two synchronized outcomes: reconstructed native ignition and native burning death, with the replica's autonomous continuation suppressed. No secondary corpse contact occurred in that take. The earlier three-outcome corpse-contact proof remains927; do not relabel940 as that same three-event test.

Both plain pistol recordings show native first-person firing and victim death. Remote shooters remain neutral because those tests intentionally did not turn on the movement bank. Full corpse poses and impact particles differ. Viewed contact sheets and visual limitations accompany937/938.

Combined presentation941 enables the existing role-correct67/357 bank and automatic gun attachment. Its gameplay/death/reload checks pass, but its presentation does not: the equipment fixture treated a victim's terminal tombstone as loss of scope and unequipped too early; the native muzzle helper only recognized the older movement-isolation fixture. Enabling the movement bank before the test-position teleport also correctly caused one motion rejection. These three observations are retained separately from the passing gameplay receipt.

Candidate944 retains an already acquired test weapon while the same combat session contains ordered death/tombstone states; initial acquisition still requires living ready targets. Its native effect helper accepts only network muzzle kind9 under the checked combat isolation scope, retaining the existing owner, declaration, world and stream-token guards and native cleanup. Other effect families are unchanged. Updated test945 settles camera/test positions before starting the existing bank and uses an oblique view containing shooter and victim. It asserts body continuity, attached prop and successful native muzzle counters in addition to damage/death/reload; video inspection is still required.

## Spread-region subset correction948

Combined test945 on candidate944 remains a failed test with clean independent restoration. The real native shot produced three callbacks involving two bound guards; one spread callback used region mask0xFE rather than earlier observations0xFF. The fixed-mask validation rejected that legitimate subset and stopped the combat session. The movement bank and gun remained active with zero refusals at the sampled point, but the cancelled combat scope means that take cannot establish successful muzzle playback.

Candidate948 accepts any nonempty subset of the low eight native spread-region bits. It retains region1, zero normal, normalized direction, source ownership, declaration and finite amount/range checks. The regression covers accepted0xFE and rejected out-of-domain0x100; the build and both actual357-clip banks passed. This generalizes the contact geometry rather than adding one specific mask. Actual combined runtime acceptance is pending takes949/951 and audit952. Never run the superseded draft publisher947, which anticipated successful945/946 before945 exposed this issue.

## Combined native ranged milestone948 / audit952

Both host949 and client951 passed with candidate948 (SHA2565433FCD5CE5E191BEE5741DEC9F2A1AA86E635FB99CBF2D53238862552BEA4F9). The existing67/357 movement bank, native attached pistol and one network muzzle effect remained active across the native damage/death, with zero presentation refusals. Duplicate requests/outcomes caused no extra damage/death/effect. Both passed shared checkpoint retirement, fresh NPC/channel negotiation, player health56 restoration and protected-file checks. Native shot data still travels through combat wire5/276 bytes.

Manual contact-sheet inspection shows each remote character extending the gun arm, flash illumination/particles at firing, then recovery with the gun retained. The oblique camera is obstructed by throne furniture, so victim/feet visibility is insufficient for full paired visual acceptance. Native first-person shot, smoke and victim death are visible in the source recordings; corpse/limb state remains an independent gap. Preserve this functional milestone and continue coverage rather than polish-gating it.

Evidence: `work/movement-integration/npc-ranged-audit-v952.json`, each take's result/restoration/death/duplicate/reload records and `visual-review-v952.json`. Sixteen new videos (including retained negative and partial cases) were registered; dashboard catalog1363. Draft publisher947 was superseded by952 and must not be executed. Regular crossbow contact inspection953 is the next investigation using the same callback snapshot and native constructor pipeline.
