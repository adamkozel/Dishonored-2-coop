# Mutual partner exclusion

## Product requirement, 12 October 2026

The user requires complete mutual exclusion from direct gameplay interactions: partners play together but never against or upon each other. This supersedes any earlier proposal for partner interaction features. It is an acceptance requirement, not a claim that every path is already covered.

Exclude both local players and their current remote representations from partner damage, harmful or beneficial powers, target selection and highlighting, takedowns, assassination, choking, looting, carrying, throwing, swapping, healing, trading, interaction prompts. Normal native movement collision between partners is explicitly allowed by the user. Do not introduce cooperative partner actions or a revive mechanic. Preserve ordinary interactions with enemies and the world. Shared deaths, checkpoints, cutscenes and progression remain coordinated session behavior.

## Coverage and gaps

| System | Evidence | Remaining work |
|---|---|---|
| Direct damage | Existing exact-identity friendly-fire checks cover scoped sword/health cases | Audit projectiles, explosions, redirected damage, traps, powers and attributed indirect hazards; exclude partner targets before reactions as well as suppressing outcomes |
| Mesmerize | Existing native partner filter and isolated tests; see chapter 26 | Recheck on current build and all relevant lifecycle contexts |
| Domino | Existing native partner exclusion and isolated tests; see chapter 30 | Recheck current build, link propagation and reload/reconnect |
| Possession | Partner test374 and ordinary guard control375 pass; details below | Broader context/lifecycle regression coverage remains |
| Blink / Far Reach / Doppelganger swap | No blanket exclusion claim | Audit target acquisition, pulling, swapping, collision and native effects |
| Bend Time / Swarm / other area effects | Presentation and power investigations exist | Ensure partner control, damage, AI reactions and world effects are excluded without excluding enemies |
| Contextual takedowns and body handling | Rear choke rejected in both directions388/389; ordinary guard control390 passes | Lethal/crouched/drop/slide/window cases, focus, loot and carrying still require coverage |
| Movement collision | User clarification: normal movement collision is fine | Preserve native collision; walk-through behavior is not a requirement. Candidate380 is withdrawn. |
| Death/load/reconnect | Scoped shared recovery exists | Remove stale target references and re-establish exclusion for fresh generation/epoch identities |

Each row requires reciprocal role coverage where applicable, genuine native input, reload/identity replacement tests, and a normal enemy/world positive control. Prefer exact current-session identity checks over excluding all friendly NPCs or all Doppelgangers. A stale pointer or reused entity handle is not sufficient identity. Cosmetic presentation must never apply damage or interaction outcomes.

## Possession investigation and candidate 374

Evidence: `work/natural-moveset/possession-partner-aim-v373`. The native selected target handle matched the registered partner (`0x0c8311fb`), and the target component matched the partner's NPC Possession component. Native states reached transfer 26 and occupied physics 18; mana fell from 100 to 62.20001. The test remains failed: its supposed B cancellation actually crouched, and releasing LT performed a real transfer. Both isolated checkpoints and original inventory/health were restored; all 14 protected save/settings hashes remained unchanged.

Exact executable: SHA-256 `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`. Ghidra analysis traced selection updater RVA `0x448200` through sensor lookup `0x777b90` to ranked selector `0x3c22f0`. The returned borrowed 0x30-byte record has entity handle at +0x18 and optional proxy at +0x20. Native selection subsequently resolves the component and publishes target effects. Main sensor vtable RVA `0x1c5b108`, Possession sensor mode vtable `0x1b41918`, and reciprocal component membership were read in the exact running image. These are scoped findings, not an exhaustive call graph.

Candidate 374 calls the original ranked selector once and suppresses only a returned current partner record after selected-power, sensor ownership/generation, component membership, record bounds and repeated entity identity checks. It does not modify ranked arrays, native inventory, assets or normal target ranking. Source: `outputs/d2-coop-lab/src/possession_partner_identity.h`, `possession_partner_filter.h`, and `possession_partner_identity_tests.h`. Diagnostics report blocked targets, identity refusals and maximum filter read time. At this documentation revision, build, identity, action, bank and proxy tests passed; deployment and runtime verification remain pending.

The stable 371 archive and bank 54 with 319 clips remain preserved. Original Steam assets and isolated saves are protected. Future findings and scoped verification results should be appended here; do not replace pending rows with blanket completion claims.

## Native collision ownership, static and read-only runtime evidence 377-380

`Read-PartnerPhysics-v377.ps1` and `Read-PartnerPhysicsDetails-v378.ps1` inspect components through the read-only probe, under the same serialized operation mutex. Additional379 reads locate the world's collision filter and partner body-ID array. Files live under `work/movement-integration/partner-physics-{read-v377,details-v378,details-v379}`. Exact PE RTTI tools identify `arkCollisionFilter`, `arkHavokUserData`, and player callbacks; Ghidra transcripts are saved as `partner-collision-140*-decompile-v380.txt`.

Confirmed static: RVA093b00 canonicalizes two complete Havok body IDs and queries the collision filter's excluded-pair hash.093a40 rejects a pair when this returns true;099650 returns its inverse.095730/18cf60 maintain native per-body references, but the candidate does not call these or mutate their lists. Confirmed read-only runtime: game+60410 points to `arkNpWorld` (vtable1ad4f38); world+470 points to `arkCollisionFilter` (vtable1ad4c08); world+28/+30 hold the body table/capacity. Body stride is0xb0, generation-bearing ID at+70, active flags at+44, user data at+90. Player user data (vtable1b02768) +8 points to its exact PlayerPhysics component; component+48 owns the inner physics object, whose+148 proxy has body ID at+38. Partner user data (vtable1c542a8) +8 points to its exact PhysicsSystemEvolved component (vtable1c54008); component+100 owns its Havok system, whose+28/+30 hold the bounded body-ID list and+20 identifies the same physics world.

Candidate380 intercepts093b00 after one original call. A cached local capsule ID is only a cheap candidate check; every actual exclusion revalidates the current world/filter, both body generations, user data, component ownership/membership, partner body-list membership and registered partner identity. All other pairs retain the original result. No world masks, native ignored-pair references or body data are written. Candidate tests cover reversed pair order, both avatar declarations, ordinary NPC rejection, recycled IDs, retired bodies, foreign ownership/world/filter, invalid lists and pending reload. The first test build had an MSVC lambda default-parameter error; the saved failed log remains. Corrected build and all required identity/action/bank/proxy checks passed.

Archive: `work/movement-integration/candidate-partner-collision-v380`; DLL `CA19D04C7035C14873F3272F8BFF2739C9F9D9541F8B50A45DA90B466544932B`; probe unchanged `31EDE6888F7C05363128BA4FC047E5B59DEE9F737FFFC1135DE623C0A6C2D6A4`; bank54/319 unchanged. Runtime collision behavior remains to be tested at this entry. Do not treat this candidate as a verified correction.


## Verified Possession exclusion and collision defect, 374-376

The archived374 DLL `AD06C1A322AD45C7A511A1D74AB1058156F87DC527041F36CB82C894D6500BF2` now passes the [partner test](../../work/natural-moveset/possession-partner-exclusion-v374/result.json):228 partner-ranked selections rejected,0 identity refusals,no target pointer/halo,no transfer/occupation,mana100 unchanged,health/inventory retained. Maximum uncached validation sample2165 microseconds; performance optimization is a separate concern. Source recording was viewed. The [ordinary crouched guard positive control375](../../work/natural-moveset/possession-partner-guard-control-v375/result.json) passes20, preserving native transfer/exit and Steam body/target particles. Both checkpoints restore complete inventory/health and14 protected hashes. Stair post obstructs most remote gesture in375; no complete animation visual approval.

[Collision audit376](../../work/natural-moveset/partner-collision-audit-v376/collision-analysis.json) finds a real defect in both directions. With the same1800ms forward input, clear distance is approximately5.3m; partner-present distance approximately2.4m. The other player is nudged0.118m (host mover) or0.151m (client mover), and separation stays approximately0.7m. Both partner-present videos were viewed and show blocking. Evidence collection passes4 and restores checkpoints; collision exclusion itself FAILS. Native pair suppression is under investigation; no collision patch has been applied.


## User clarification and withdrawal of collision candidate, 383

The user explicitly clarified: **Movement collision is fine.** Normal movement collision is allowed; the exclusion requirement continues to cover combat, powers, targeting, contextual actions and other direct interactions. Do not resume pass-through implementation without a new request. Earlier descriptions of collision as a defect refer to the superseded interpretation.

Candidate380 failed behavioral test381 despite suppressing some ragdoll body pairs. The separate Navigation body found in read-only census382 was a research lead, not a verified correction. The experiment, failed result and exact-image findings remain archived. Active source and staged binaries have been restored byte-for-byte to verified374, preserving all existing damage and power exclusions and bank54/319. Deployment verification is recorded separately in `work/movement-integration/native-collision-restoration-v383.json`.

Restoration383 is now verified: both live processes contain the original14-byte native collision prologue at RVA093b00, the native Steam connection is healthy, the damage and three power filters remain installed, health remains56/40 and all14 protected save/settings files are unchanged. This read-only verification does not establish every remaining interaction exclusion.

## Contextual targeting and confirmed partner choke, 384-387

The read-only Void export `outputs/d2-coop-lab/research/asset-exports/partner-targeting-v384/manifest.json` pins nine resources and verifies all original index hashes unchanged. Both custom partner declarations inherit `assassination_npc.cpntassassination`; its native rules include base, drop, slide, window and Shadow Walk, with drop kill/choke enabled. This is a reason to investigate, not proof that every rule can execute. The Far Reach sensor also includes NPC snapping; its exact partner behavior remains untested.

Neutral observation385 covers both roles in front, rear and crouched rear positions. All60 sampled global assassination sensor records contain the current partner handle in two of five records. Records start at component+0x40, stride0x38, with target handle at record+0x20; current rule result is at component+0x1d0. The source changes stance, and the peer's actual stance is retained in observations; these are not six identical victim contexts. The two rear camera sheets were viewed and show no contextual prompt. Absence of a prompt is not proof that the action is unavailable.

Native input386 proves the gap: Corvo behind Emily enters state41 and `choke_back.midnightscene`, followed by `choke_loop.midnightscene`, after a2000ms RightShoulder input. The viewed recording shows the native choke/throw HUD. All23 post-input samples remain in choke state through the recording. Health stays protected, but interaction exclusion fails. The test's collection/restoration result is successful; the gameplay requirement is not. Evidence: `work/natural-moveset/partner-rear-choke-host-v386`. Both checkpoint inventories and health were restored and all14 protected save/settings hashes retained.

Ghidra exact-image analysis identifies global assassination sensor vtableRVA1b3f998 and constructor3af060. Eligibility function3b00a0 calls helper3b0000, which obtains source PlayerStates through context+0x20. Candidate387 rejects only the current registered partner after checking its generation, declaration, current game, the local PlayerStates type/owner/component membership, and repeated identities. Other targets call the native function unchanged. No movement collision, native assets, player animation or state fields are altered. The16-byte prologue contains six complete non-relative instructions, including TEST whose flags are preserved by the trampoline before the native conditional branch. Exact PE exception-table and Ghidra disassembly evidence are retained.

Archive: `work/movement-integration/candidate-partner-assassination-v387`; DLL `440B0F9FD8BE7D24D22B62DE0BD7FB43C2E9C85FE6AB3551DA9A988AA273C6CD`; probe unchanged from374; bank54/319 unchanged. Build, ownership/refusal fixtures, action protocol, bank and proxy smoke tests pass. Runtime verification and ordinary guard positive control are pending at this entry. Full contextual interaction exclusion is not claimed.


## Scoped verified candidate387, tests388-390

Both native standing rear partner choke tests pass4 checks. Corvo rejects1668 eligibility calls and Emily1644, with zero ownership refusals. All27 samples in each recording contain no partner target in the five global sensor records and no choke state41; native running/block states0/44 remain. The viewed1/2/3/4/5/6s sheets show normal first-person sword blocking, no partner choke and no choke/throw HUD. Maximum validation samples were328/335 microseconds. Each test includes complete checkpoint inventory/health restoration and14 protected hashes.

The ordinary authored guard2 positive control390 passes4 checks, preserving native state41 and choke_back/choke_loop. Its exact generation-bearing handle appears reciprocally in native scene bindings. The viewed recording retains the native guard interaction prompts and choke/throw HUD. All checkpoints/inventories/health and14 protected hashes restore. This verifies a scoped eligibility change, not every takedown rule or full multiplayer combat authority.

[Verification receipt](../../work/movement-integration/candidate-partner-assassination-v387/runtime-verification-v391.json) records build pins, measurements and six newly indexed source recordings, including the failing baseline. Other four neutral observation385 videos are retained but not claimed visually reviewed. Normal movement collision remains allowed.


## Confirmed sword fatality gap392 and native interaction-sensor trace395

The first front-standing native RT300ms test392 fails: `fatality_generic_right.midnightscene` reciprocally binds player1 and the current registered Emily partner. State39 is therefore insufficient to distinguish a normal slash from a paired finisher. Viewed footage shows cinematic sword movement and blood/impact effects. Rear-choke eligibility still rejects1038 calls with zero refusals. Remaining three contexts and the reciprocal role were not run. Both checkpoint inventories/health restore and all14 protected hashes remain unchanged. The failing video is indexed separately; no full takedown exclusion claim.

Exact-image Ghidra trace: Slash state vtable1c85d28;86b050 falls back from global assassination to interaction sensor type0x21 through3b8080. Native3b8080 resolves the generation-bearing handle at sensor+88;3b8020 resolves+8c. Both respect flag+ b8 bit0 unless explicitly overridden. Native86d320 stores the latter handle in Slash state+e0;86cbf0 uses it as a paired scene target. Static RTTI identifies `arkCpntPlayerSensorInteraction`, vtable1b403b8. Read-only392 samples confirm the exact partner handle at both+88 and+8c, local player ownership at+10 and local PlayerStates at+20. These are separate from the five global assassination records already filtered. Target-specific getter exclusion is a candidate, not yet runtime verified. Normal movement collision stays enabled.


## Scoped sword targeting correction396, verified398/400 and ordinary guard399

[Candidate396](../../work/movement-integration/candidate-partner-interaction-v396/identity.json) adds two result filters at3b8020/3b8080. Each calls the original getter once, then excludes only the current registered partner after reciprocal player/sensor membership, exact type and generation, selected handle, local PlayerStates and repeated lifetime checks. Raw native target records are not overwritten. Both16-byte copied prologues have internal conditional jumps ending at+16; these land on the trampoline resume jump. Native collision and bank54/319 remain unchanged. Ownership/refusal, bank, action and proxy checks pass.

All eight native RT cases pass in398/400: front/rear, standing/crouched, both characters. Both getters reject partners without ownership refusals; no global partner target or paired partner scene remains. Source39 performs ordinary swings, health remains protected, full inventory/health/checkpoint cleanup and14 protected hashes pass. Viewed temporal sheets show no previous finisher grab or blood effects. Test397 is retained as failed: its old assertion demanded downstream assassination-counter growth even when the earlier filter already removed the partner.398 checks real target records instead.

Ordinary guard399 passes: native target getters still return the exact authored guard2; source sword scenes reciprocally bind that NPC, viewed impact/blood remain, final guard health30. It is normal sword-contact evidence, not a verified guard fatality. All cleanup passes. [Verification receipt](../../work/movement-integration/candidate-partner-interaction-v396/runtime-verification-v402.json) contains case counts, hashes and recordings.

User priority correction: keep the basic safeguards and move on. AI ignore flags alone do not cover player targeting paths, but exhaustive interaction edge-case work must not block movement/world development. Remaining powers, contextual edge cases and lifecycle breadth stay in the acceptance backlog. Normal movement collision remains allowed. Next development returns to native Doppelganger cast/presentation.
