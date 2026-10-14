# Airborne root interpolation and body continuity

## Observed defect

[Mesmerize entry v168](../../work/natural-moveset/mesmerize-air-entry-network-emily-v168/result.json) passed its 17 scoped functional checks: native armed Emily entry while jumping drove the remote cast and airborne leg motion. The final native checkpoint cleanup and fourteen protected save/settings hashes also passed. This did **not** establish acceptable presentation. Its [movement states](../../work/natural-moveset/mesmerize-air-entry-network-emily-v168/states.json) contain two vertical `MovementClock` rejections. Body weight briefly dropped to zero at takeoff and after contact.

Source and peer [temporal sheets](../../work/natural-moveset/mesmerize-air-entry-network-emily-v168/peer-sheet.jpg) were inspected. The tight receiver camera also cuts off the head and casting hand at the apex. Both full videos are retained in the review manifest with `visual_accepted=false` and an explicit defect note. A passing functional receipt is not rewritten into a visual pass.

The last retained landing rejection was a descending `-0.19133759 m` step over `0.0646413 s`, with motion ground and prior presentation air state 3 (landing recovery). The native state had reached ground before its final downward settling movement finished. Earlier takeoff interpolation likewise moved vertically while the discrete motion still belonged to the prior grounded source sample.

## Candidate v170

[Pose interpolation](../../outputs/d2-coop-lab/src/pose_interpolation.h) retains discrete native motion at the earlier source sample until the later sample time. This prevents premature ground contact. A separate **local-only** `airTrajectory` flag now describes a bracketed root segment containing confirmed native Jump/Fall samples. It does not alter packet data, native gameplay state, root authority, or animation action selection.

The final ground settlement allowance is limited to coherent descending ground samples within **100 ms and 25 cm of the first confirmed Fall-to-Ground contact**, with at most 100 ms between that fall sample and contact. It rejects an intervening ascent, missing motion data, ordinary ground motion, excessive distance or time, and previous-world evidence. These limits are presentation guards around an observed case, not inferred engine constants or a claim that all landing contexts are covered.

[MovementClock](../../outputs/d2-coop-lab/src/movement_bank.h) uses this interpolation evidence only while eligible, with an air-capable bank and without sliding, traversal or a transport power. Its existing airborne vertical-speed limit remains active; the normal horizontal and ground movement bounds remain intact. The local first-person controller and native root position are untouched.

Regression checks are in [pose interpolation tests](../../outputs/d2-coop-lab/src/pose_interpolation_tests.cpp) and [movement bank tests](../../outputs/d2-coop-lab/src/movement_bank_tests.cpp). The latter uses a synthetic sequence built from the observed boundary heights and settlement delta; it is not represented as a complete captured runtime trace. Earlier isolated landing-boundary negative tests remain unchanged.

The [v170 native harness](../../work/natural-moveset/Test-MesmerizeAirNetwork-v170.ps1) retains the original isolated test and recovery contract, uses the wider v169 camera, and adds an explicit continuity assertion over takeoff, airborne casting, landing and recovery: no new clock rejection, lease loss, or body weight below 0.9. Entry and release require independent actual-game runs. No visual acceptance is implied by those numeric checks.

## Runtime verification and empty-hand extension

[Candidate v170](../../work/movement-integration/candidate-air-trajectory-v170/identity.json) passes interpolation tests, the complete bank-format 1–46 / both-role regression suite, build and proxy smoke. DLL `DDD62F5EB3D7CED26AC45D859C48C72C8CC9A0754A31DF1EEC7C1025FC605805`; unchanged probe `964433461EBAE095F91E13479DE3AF2DA5C38B052DA778394D4AF024C376DCCA`; body42 / world4 / bank46 with285 clips. Its [runtime verification](../../work/movement-integration/candidate-air-trajectory-v170/runtime-verification.json) records the independent armed entry/release runs below.

Native [empty-hand entry v175](../../work/natural-moveset/mesmerize-empty-air-entry-source-v175/result.json) and [empty-hand release v177](../../work/natural-moveset/mesmerize-empty-air-release-source-v177/result.json) each pass12 observation/recovery checks. Native X holsters before casting. All93/89 equipment samples retain the holstered sword. v175 has two Jump+UsePower entry466ms rows and one Fall+UsePower aim1200ms row. v177 has two Jump+UsePower aim rows, then two Fall-only release666ms rows. All bind reciprocal owned Emily left arm67225. Sequential raw scene and state reads are corroborated across rows and inspected source video; they are not described as atomic reads.

[Candidate v178](../../work/movement-integration/candidate-mesmerize-empty-air-v178/identity.json) adds those observed contexts to the strict source reader, body protocol43/336 and existing bank46 context playback. Earlier42 peers are rejected. Scene identity, Emily role, selected native item, reciprocal arm binding, bounded jump metadata and freshness requirements remain. Identity/wire regression tests and proxy smoke pass. DLL `8E3CA92B8589CA623730D40987769A03A6D46B0B64EA3F4F38C2D5F7489A64B0`; probe `E0CF01699B8CDC0511EDA763A8ABEE17CE459080D74C44D5BCA3EA69B4EE6840`.

| Actual D2 take | Sword context / case | Scoped checks | Body continuity samples | Minimum body weight | Clock dropouts |
|---|---|---:|---:|---:|---:|
| [v171](../../work/natural-moveset/mesmerize-air-entry-network-emily-v171/result.json) | Armed entry |18|45|1.0|0|
| [v173](../../work/natural-moveset/mesmerize-air-release-network-emily-v173/result.json) | Armed release |17|44|1.0|0|
| [v179](../../work/natural-moveset/mesmerize-empty-air-entry-network-v179/result.json) | Holstered entry |18|42|1.0|0|
| [v181](../../work/natural-moveset/mesmerize-empty-air-release-network-v181/result.json) | Holstered release |17|45|1.0|0|

All four final receipts include native inventory/health restoration, both isolated checkpoint reloads and protected14 verification. The18-check cases have positive native NPC targeting evidence; the17-check cases observe an empty target set and verify no invented receiver target effects. These are distinct native situations, not missing assertions silently waived. Each source and peer temporal sheet was inspected and both full recordings are in the dashboard. Wider framing contains the complete jumping body. The remote right-hand sword is visible in armed cases and hidden throughout the holstered cases. Native mana/world outcomes remain local; receiver power effects are cosmetic.

Full visual acceptance, additional movement directions, other interrupted/terrain contexts, shared NPC gameplay and playable co-op remain incomplete. These tests concern bounded in-place jumps on the known Dunwall floor, not every campaign landing. Previous builds and failed takes remain preserved.
