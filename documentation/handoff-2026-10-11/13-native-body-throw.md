# Native body throw: v31 investigation and isolated implementation

This is an unfinished experiment on the exact executable with SHA-256 `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`. The preserved v30 carry build is the rollback milestone. Read the per-take results before treating v31 as validated.

## Confirmed native observations

Normal standing X pickup followed by RT enters `arkPlayerStateDropCorpse`, state46, vtable RVA `1c790c8`. The active scene is `midnight/gameplay/player/base/carrycorpse/carry_corpse_drop_high.midnightscene`, `Scene_1`, 666ms. Drop46 stores the scene handle at+24, exact corpse handle at+38 and selector2 at+54. The observed bytes+50/+51/+52 are `[1,0,1]`.

Fresh [Corvo](../../work/natural-moveset/native-body-throw-corvo-reader-v31) and [Emily](../../work/natural-moveset/native-body-throw-emily-reader-v31) recordings each contain eight accepted high-throw samples from a reader that brackets the player state, scene generation, scene bindings, exact corpse, reciprocal native right arm and source clock. This addresses the earlier sequential state/scene boundary problem without reclassifying the failed v1 observations. Their actual temporal sheets were inspected: native body release and sword recovery are visible, but a nearby wall constrains flight. These are source references, not remote-animation acceptance.

The [vector observation](../../work/natural-moveset/native-body-throw-corvo-vector-v31) adds an owned camera/configuration read. The source vector was approximately `[0,-4,0]` in the level-camera fixture. [Both live declarations](../../work/natural-moveset/carry-research/native-throw-parameters-v31.json) give strength4 at+a0. [Camera ownership](../../work/natural-moveset/carry-research/native-throw-camera-owner-v31.json) confirms `arkCpntPlayerCamera`, vtable RVA `1c72218`, owner+10 matching the native player; the DropCorpse declaration vtable is RVA `1c78f98`.

## Decompilation and disassembly

All addresses use Ghidra image base `140000000`.

| Address | Confirmed static behavior | Limits |
|---|---|---|
| `1403a6f10` | Selects native drop scene kind from state/sensor flags | Sensor meanings are not all named |
| `140831fe0` | Drop46 entry copies the carried corpse, reads throw input and registers callbacks | Registration alone is not callback timing proof |
| `140832770` | Starts selector-dependent scene; selector2 uses declaration+80; saves selector at+54 | Exact runtime ownership still required |
| `140832b10` | Matching corpse/event callback invokes `140832290` for payload kind0 | Early ragdoll may precede the nominal scene marker |
| `140832290` | Queues event149, payload28 bytes, for exact corpse. Linear vector is camera+25288 times declaration+a0; angular vector is zero; bytes24/25=1 | Existing input-history check is part of native behavior; remote experiment only accepts observed RT flags |
| `140732f32` / `140732f49` | Registers thunk `14080bc50` for event149 | Code-site registration is distinct from a named function |
| `14080bc50` | Jumps to `140727100` | Verified disassembly |
| `140727100` | Adds/replaces physics vectors+98..ac, sets flags+b0..b2, calls virtual+130 | Function explicitly defined in Ghidra from the verified thunk; prior no-function result retained |
| `140769350` | PhysicsSystemEvolved virtual+130 wakes its rigid bodies | Not general authority to modify arbitrary physics |
| `1408112c0` | Allocates/enqueues typed event payload and target handle | Called only on the engine thread in the isolated fixture |

Raw results live in [carry-research](../../work/natural-moveset/carry-research). Ghidra bookmarks `D2Coop/NativeThrow-v31` at832290 and727100 were saved. Native asset data has early-ragdoll at200ms and start-ragdoll at280ms; early collision behavior remains a coverage gap.

## Experimental implementation

- [Native reader](../../outputs/d2-coop-lab/src/native_player_carry_scene.h): high throw is variant8; variant7 remains preparation. Explicit opt-in, standing stance, exact666ms duration, selector/flags, owned configuration/camera, finite unit direction and end-of-read rechecks. Default callers refuse high throw. The isolated source stream opts in.
- [Semantic packet](../../outputs/d2-coop-lab/src/shadow_action_protocol.h): version31,308bytes, carries source throw velocity. Other actions require that field to be zero. Throw vectors must have magnitude approximately4, preserve target identity and obey existing session/world/replay checks.
- [Carrier bank builder](../../work/natural-moveset/build_high_throw_bank_v31.py): D2MOVE36,234clips. Appends233 to the v35 banks; all233 prior clip payloads remain byte-identical. Trims the400ms review lead-in from the native-victim-inspired full-body high-release sequence and maps666ms without speeding the whole donor study.
- [Victim scene builder](../../work/natural-moveset/build_high_throw_experiment_v31.py): adds only `midnight/d2coop/carry_victim_drop_high.midnightscene`. Only NPC animation/look-at tracks remain. No player camera, first-person arm, event, damage, inventory or spawn tracks are retained.
- [Attachment fixture](../../outputs/d2-coop-lab/src/carry_attachment_fixture.h): exact known dead Mayhew only, same source incarnation, v36 carrier bank, early source window. Detaches, plays the victim clip at the source clock, seeds rigid bodies from visible pose at release, requests native ragdoll, then queues one event149. Delayed, stale or changed-source impulses are refused. Native damage leases and existing cleanup remain in place.

The full-body authoring is a candidate. Current numerical tests cannot establish natural contact, good silhouette, correct body flight or visual acceptance.

## Preservation and tests

Candidate archive: [candidate-high-throw-v31](../../work/movement-integration/candidate-high-throw-v31). Rollback: [candidate-carry-stance-anchor-v30](../../work/movement-integration/candidate-carry-stance-anchor-v30).

DLL `59BB03FE23AB8BDE5B363A0CF58658749EBA6DD928BE582C98B7131743963426`; probe `A0D22CD8EA4203D64AD5F16DB2A55287E45F9D62BAC74F84F16B350727BF8DCD`. [Bank hashes](../../work/natural-moveset/high-throw-bank-manifest-v31.json), [asset hashes](../../work/natural-moveset/carry-high-throw-scenes-v31.json), [installation log](../../outputs/d2-coop-lab/research/carry-high-throw-asset-install.json), [clone asset rollback](../../work/carry-high-throw-backup-20261011-192820/manifest.json).

Native ownership/read-race tests, shadow packet/timing tests, proxy smoke and both actual v36 bank tests passed. Native source reader recordings are separate from the paired fixture. The first [Corvo paired take](../../work/natural-moveset/native-high-throw-corvo-v31) stopped during pickup with `unsupported_or_claimed_target`, refusalStage5. It never reached throw and is preserved as failed. Later read-only diagnostics show unclaimed body and valid named joints; that does not prove the same conditions held at the rejected tick. No guard was weakened.

The [recording harness](../../work/natural-moveset/Record-HighThrow-v31.ps1) and [throw validator](../../work/natural-moveset/validate_high_throw_v31.py) retain source/observer telemetry, one-impulse counters, physics state, native first-person continuity, health, sword restoration, capture hash and visual-acceptance separation. Review fresh per-take reports for subsequent results.

## v31 runtime failures and v32 follow-up

The [Emily v31 take](../../work/natural-moveset/native-high-throw-emily-v31) passes the 23 pickup checks, but the throw passes only 18 of 21 checks. It queues zero impulses and records one refusal. The native source throw, replicated carrier clip, and victim scene are observed; after the 280 ms handoff the fixture waits for ragdoll while retaining the victim scene, then expires after 200 ms. Cleanup restores ragdoll, equipment and health. The actual video sheet shows the victim dropping beside Emily instead of being thrown forward. This take is rejected and is not visual acceptance.

The [Corvo v31 repeat](../../work/natural-moveset/native-high-throw-corvo-v31-repeat) again refuses pickup at stage5 before any throw. That disproves the assumption that the first failure was merely a one-off. v32 splits the existing stage5 checks into named-joint lookup (51/52), frame ownership (53), carried flags (54), and interned-name read (55). Conditions remain identical.

The v32 candidate stops and waits for retirement of the exact victim scene before checking native ragdoll and attempting the one-shot impulse. The existing 200 ms deadline, source-event/identity guards and physics ownership checks remain. This is a hypothesis being tested, not a proven correction. [v32 archive](../../work/movement-integration/candidate-high-throw-v32) pins DLL `46DCE62AB9B8FEDA9457C6760F3096E494307ED9DBBAD5FD398D4EDB1FFF3217`. Source/protocol/ownership tests and proxy smoke passed; runtime results must be read separately.

Fresh [734a10 decompilation](../../work/natural-moveset/carry-research/native-throw-140734a10-decompilation-v32.json) confirms the native ragdoll request is conditional on PhysicsSystem+241 bit3 being clear. It requests pending mode0 and queues native event0xbb; calling it does not prove that the scheduler has already completed the transition.

## v33: verified impulse handoff

v32 stops the scene but still queues zero impulses for both characters. v33 requests ragdoll once **after** the exact scene handle has retired, waits for mode0/pending−1, then makes one event149 attempt. It retains the 200 ms deadline and source incarnation/event/identity checks. Both [Corvo](../../work/natural-moveset/native-high-throw-corvo-v33) and [Emily](../../work/natural-moveset/native-high-throw-emily-v33) pass all23 pickup and21 throw checks. Each queues exactly one approximately `[0,-4,0]` impulse, preserves source/observer/avatar health, restores the original sword and removes native retargeting. Actual capture rates are26.93 and27.33Hz respectively. The video sheets show forward body travel instead of the previous stationary drop.

This is a scoped physics milestone, not visual acceptance. The victim unfolds abruptly above the carrier, and the hand/body alignment is poor. The tested lane faces a nearby wall. Whether the pre-stop request was ignored by the bit3 guard or superseded by scene teardown is not directly established by the sampled telemetry; the successful ordering is the confirmed result. [v33 archive](../../work/movement-integration/candidate-high-throw-v33) pins DLL `48A17FA29C9BF0DE418C06F1F5A9BCEBF73E9B30D3F000101C3D4C98A114C520`.

## Alternate direction and contextual release

The first alternate observer position did not establish the stream and was preserved as a failed setup. [Corvo away-facing near furniture](../../work/natural-moveset/native-high-throw-corvo-open-v33-b) then completed the native input sequence but selected native **variant5 soft drop**, not high throw. No high-throw impulse was queued. The high-throw validator remains failed; it is evidence of a missing contextual release variant, not a reason to force the high-throw behavior over the native decision.

[Emily clear-lane geometry study](../../work/natural-moveset/native-high-throw-emily-contact-v34) uses the **v33 DLL** despite the harness suffix. It explicitly repositions the loaded fixture before the test action; that reposition is not natural-traversal evidence. Native high throw is observed with approximately `[0,+4,0]` and exactly one impulse. All20 throw checks independent of the pickup report pass, but the all-sequence pickup report fails its unchanged-bank-refusal check after setup reposition. The whole take remains failed. Actual video and sampled final carrier/victim joint frames are retained; hand contact remains visibly unaccepted. The mixed read brackets are recorded and do not claim atomic source/render timing.

## v34: retained attachment candidate

Fresh static tracing found a missing native step. [829e30](../../work/natural-moveset/carry-research/native-throw-140829e30-decompilation-v34.json), called before Drop46 starts its scene, invokes [829180](../../work/natural-moveset/carry-research/native-throw-140829180-decompilation-v34.json). The latter computes a reference-animation joint transform and sets up/activates native retargeting. The [exact exported CarryCorpse declaration](../../outputs/d2-coop-lab/research/asset-exports/carry-release-placement-v34/game1_patch/generated.decls.playerstatecarrycorpse.components.characters.player.base.states.carry_corpse..playerstatecarrycorpse.decl) names `center_spine03_jnt` for carry, `center_spine02_jnt` for drop, `anchor_jnt` as the attachment, and `ply_body_common_carrycorpse_in` as the reference animation. Export SHA-256: `12BB41B51BE8CF61101874FC7CFF658642D698AB68FF2C2E1F1E0317D17560E5`. Original asset indexes were checked unchanged by the read-only exporter.

Our v34 analogue retains the existing owned attachment until the high-throw ragdoll boundary. It changes the owner reference to spine02 while preserving the existing model-space attachment frame, then detaches before physics handoff. It deliberately does not invoke a real player's Carry45 callbacks on the remote NPC. This is an experiment and is not identical to the native reference-animation calculation. Low-drop code and all bank payloads remain unchanged.

[v34 archive](../../work/movement-integration/candidate-high-throw-v34) pins DLL `67010EF9F82D61BB6D0B415A399D209AFF7E5F4A208906F0876395223EB55D85`. Both [Emily](../../work/natural-moveset/native-high-throw-emily-v34) and [Corvo](../../work/natural-moveset/native-high-throw-corvo-v34) pass23+21 checks at27.27 and26.8Hz capture respectively. Both actual sheets still show abrupt unfolding and weak supporting contact. Passing lifecycle checks does not establish a convincing body sequence. The candidate remains isolated and visually unaccepted; the v33 and v30 rollback archives remain intact.

## Reproduction and next work

The current v34 build also passes ordinary standing low-drop regression for [Corvo](../../work/natural-moveset/native-low-drop-corvo-regression-v34) and [Emily](../../work/natural-moveset/native-low-drop-emily-regression-v34):23 pickup checks plus14 low-drop checks per character. Final corpse distance from the source is1.083m and0.442m respectively; neither body launches away. Both actual temporal sheets were inspected. Equipment returns, native ragdoll settles, and both instances finish in gameplay state1 with native Steam connection state3 and no active carry lease. These are regression cases, not new visual acceptance.

The final read-only integrity check after these tests passes all14 protected save/config files and the original/isolated executable and candidate/probe identities. The review index now includes the scoped successes and explicit rejected comparisons; [index_carry_regressions_v34.py](../../work/natural-moveset/index_carry_regressions_v34.py) adds the low-drop evidence without rewriting the high-throw findings.

- [Standard native-input harness](../../work/natural-moveset/Record-HighThrow-v31.ps1): used across v31–v34; per-take identity hashes distinguish the actual DLL and bank.
- [Alternate-direction harness](../../work/natural-moveset/Record-HighThrow-Open-v33.ps1) and [clear-lane/contact study](../../work/natural-moveset/Record-HighThrow-Contact-v34.ps1): explicit fixture variants with their limitations above.
- [Indexer](../../work/natural-moveset/index_high_throw.py) preserves rejected comparisons and separates checks from visual acceptance.
- [Progress recorder](../../work/natural-moveset/record_high_throw_progress.py) updates the current state and archive scope without overwriting older findings.

Remaining: accurate native release attachment/reference placement, full-body acting and supporting contact, native soft and other drop variants, arbitrary owned victims, pitched throws, interrupted/cancelled/pause/disconnect cases, and consistent physical/gameplay outcomes in both worlds. A presentation-only throw impulse is not complete world authority. Do not replace these remaining requirements with a count of clips or passed fixture checks.
