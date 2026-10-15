# Native rear-choke binding,581 development

## Purpose and current scope

Continue broad functional movement coverage through the established authored-bank/native-state/Steam pipeline. No third-party co-op code, new authoring package or finger-pose detour is involved. The latest preserved trap candidate is575, with passing Emily576/Corvo578 tests and the unresolved Corvo576 freshness dip retained.

Candidate581 adds **remote player body presentation only** for rear choke entry, struggle and completion. It does not animate or attach the receiver's victim, run native choke events remotely, change enemy health, synchronize knockout outcomes or claim completed paired interaction gameplay. First-person camera, arms, input and the native source victim sequence remain native. The network root continues to place the remote actor.

## Exact-game source evidence

[Native Corvo579](../../work/natural-moveset/native-guard-choke-corvo-v579/result.json) passed4 source-observation/restoration checks. Standard simulated RightShoulder input was held behind the unique authored guard `ld_spawner_guard_tutorial_2`, definition `models/characters/def/generic/dunwall_guard_male_small_01.def`. The experiment restores both checkpoint inventories, player/partner health and14 protected files. It does not force a native player state or directly mutate the victim.

Confirmed in this exact executable (`B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`):

| Field or phase | Observed contract |
|---|---|
| Player state | Exclusive41, `arkPlayerStateChoke`, vtable RVA1c77c18 |
| Player physical context | Default mode, standing0 at entry, **native choke stance12** during struggle/finish, logic1; independent root position continues |
| Entry | `midnight/gameplay/player/base/choke/choke_back.midnightscene`, subscene `Npc`,600ms |
| Struggle | `choke_loop.midnightscene`, `Npc`,1400ms, successive scene instances |
| Finish | `choke_win.midnightscene`, `Npc`,1800ms |
| Bindings | Exact authored NPC, player1, reciprocal owned left and right arms; four bindings |
| Clock | Total+15c, local+160, start+164; captured phases have start0 |

The bindings and clocks are saved in [choke-scenes.json](../../work/natural-moveset/native-guard-choke-corvo-v579/choke-scenes.json). Corvo's observed arms were67862 and133399; implementation resolves ownership dynamically rather than hardcoding these local handles.

[Emily580](../../work/natural-moveset/native-guard-choke-emily-v580/result.json) **failed** to start choke. Its video shows the ordinary guard continuing native walking and leaving reach before input; the player performed a sword parry. The failed take restores all protected state. It is not Emily animation coverage. Harness583 refreshes the player's position behind the actual same guard immediately before ordinary input. It does not teleport or force the guard. Whether that correction succeeds requires its own completed receipt.

## Implementation

- [Read-only source reader](../../outputs/d2-coop-lab/src/native_player_choke_scene.h): exact state, resource/subscene/duration, unique reciprocal NPC, both locally owned arms, clock range and repeated identity checks. Ambiguous scenes, unsupported phases and unsupported actor ownership are refused.
- [Native movement state helper](../../outputs/d2-coop-lab/src/native_player_states.h): exclusive standing Choke41 can publish its independently observed root. This does not admit an unknown phase as a completed gesture or ordinary-ground fallback.
- [Protocol](../../outputs/d2-coop-lab/src/shadow_action_protocol.h):63,400 bytes, state21; phase/time/duration and stable spawner/definition hashes. Sword must be holstered, held gadgets and conflicting actions excluded. Local pointer values and victim commands never cross the wire.
- [Timeline](../../outputs/d2-coop-lab/src/choke_presentation.h): native phase clocks sampled on the existing delayed root clock; target changes and time rewinds require new semantic events;250ms gap handling and200ms sampling expiry remain bounded.
- [Steam path](../../outputs/d2-coop-lab/src/shadow_stream.h): validates unique source ownership and resolves the same authored target on the receiver. Explicit partner exclusion remains. Resolving a target does not itself mutate it.
- [Playback](../../outputs/d2-coop-lab/src/movement_playback.h): existing full-body authored actor clips, short phase blends, native equipment and standing recovery. Root displacement remains network-owned. No native animation gameplay event is replayed.
- [Bank builder](../../work/natural-moveset/build_choke_bank_v581.py): bank63/351; all previous348 payloads are byte-identical. Entry/loop/finish are cropped from `choke-sequences-v1/nonlethal_back_finish_sequence-{corvo,emily}.bin` using saved phase marks and native600/1400/1800ms durations. Existing bone layout, lengths and helper data remain.
- [Build](../../work/movement-integration/Build-Choke-v581.ps1) and [deployment](../../work/movement-integration/Deploy-Choke-v581.ps1): existing native-operation mutex, isolated game only, archived sources/binaries/banks, ownership/protocol/smoke and actual bank tests. [Candidate identity](../../work/movement-integration/candidate-choke-network-v581/identity.json) is unverified until a separate completed runtime receipt.
- [Runtime harness583](../../work/natural-moveset/Test-ChokeNetwork-v583.ps1): independent native reader observations plus remote phase/body-weight checks and source/observer videos. Both checkpoints and14 protected files restored. File existence is not proof of test success.

## Ghidra findings and boundaries

[Static RTTI scan](../../work/natural-moveset/native-choke-research-v574/arkPlayerStateChoke.json) identifies the exact vtable. [14082adb0 pseudocode](../../work/natural-moveset/native-choke-research-v574/ghidra-14082adb0.txt) initializes the choke rule handler from declaration+1a0, registers player events a2/c7/189/18a and fails with `Choke rule handler not set` if absent. Candidate field meanings at state+a8(rule handler),+b0(sensor result),+98(helper entity),+a0(helper component) remain **static hypotheses**, not runtime contracts. [14082cab0](../../work/natural-moveset/native-choke-research-v574/ghidra-14082cab0.txt) samples input flags. No call to either function was added.

Findings were saved as Ghidra Note bookmarks at141c77c18 and14082adb0; the persistent project was saved. [Export582](../../work/handoff-project-bookmarks-v582.json) retains197 analyst notes across all existing categories.

## Remaining work

### Verified breakout592 and regression601

[Receipt601](../../work/movement-integration/choke-breakout-verification-v601.json) records60 passing checks across Corvo593/597 and Emily595 breakout tests and Corvo/Emily598 finish regressions. All tests use the archived592 DLL and independently check native source scene ownership, Steam full-body playback, delayed native-root following, recovery, unchanged player/partner health and complete checkpoint/protected-file restoration. Bank64/352 preserves prior351 clip payloads.

Actual Corvo597 and Emily595 observer sheets show release, recoil and standing recovery; neither is visually accepted. Corvo593's guard obscures the actor and its view is not sufficient. Corvo finish598's camera clips balcony geometry and remains numerical regression evidence; Emily598's finish/recovery is visible with some occlusion. Earlier Corvo585/Emily589 recordings still show the unchanged entry/loop/finish clips. Receiver victim behavior and shared outcomes remain unimplemented. No further cosmetic tuning was added.

[Coverage reconciliation600](../../work/movement-integration/coverage-reconciliation-v600.json) repairs six stale dashboard labels for pistol, crossbow, Windblast, standing/crouched turns and stairs from exact saved passing receipts. It does not claim new tests, current-build retesting, broad terrain/angle coverage or visual acceptance. This avoids duplicating work in the current pipeline.

### Verified actor binding and recording correction

[Corvo585](../../work/natural-moveset/choke-network-corvo-v585/result.json) and [Emily586](../../work/natural-moveset/choke-network-emily-v586/result.json) each pass12 checks: independent source-owned entry/loop/finish, native Steam body presentation, standing recovery, source-root following, ordinary NPC eligibility and complete checkpoint/protected-file restoration. [Receipt587](../../work/movement-integration/choke-verification-v587.json) preserves the exact584 DLL/probe and limited actor-only scope.

**Visual evidence is separate.** Corvo585's actual observer sheet shows the actor sequence. Emily586's numerical checks passed, but the observer fell into the atrium and its video does not show her body. Its initial automated review description was corrected by [588](../../work/movement-integration/fix_choke_review_v588.py); it remains retained as an unusable observer view. Failed583 also had a low observer view, in addition to its real stance-reader failure.

[Emily589](../../work/natural-moveset/choke-network-emily-v589/result.json) repeats all12 checks with a same-floor camera probe, and its [observer sheet](../../work/natural-moveset/choke-network-emily-v589/observer-sheet.jpg) visibly shows entry, struggle, finish and standing recovery. [Supplement589](../../work/movement-integration/choke-camera-verification-v589.json) records this correction. Both characters' actor sequences have now been visually observed. Neither is marked visually accepted: the receiver's guard remains standing and independent, with contact and victim/world behavior still unfinished. The dashboard contains994 actual game videos after retaining all these passes and failures.

The cancellation observation590 stopped before input because every proposed observer position fell to the lower floor. That is a fixture failure, not evidence about cancellation. Source-only observation591 removes that irrelevant observer gate while retaining native source recording, ordinary input and automatic restoration. It **failed the expected cancel400 assertion**: releasing RightShoulder after1200ms leaves the native hold running, followed by `choke_combat_lose.midnightscene / Npc /1500ms`. The source sheet shows the guard breaking free. It is a distinct branch, with exact state41, physical stance0, four reciprocal NPC/player/arm bindings and the same three scene clock fields. Cancellation400 remains unverified. Both tests fully restored checkpoint inventories/health and14 protected hashes.

### Combat-lose candidate592

[Builder592](../../work/natural-moveset/build_choke_breakout_v592.py) extends the same bank with one actor-only clip per role. Bank64/352 preserves all previous351 payloads byte-for-byte. The existing authored release segment supplies the initial0.28s and final hands-free recovery; the existing native `sword_stinghit_front` full-body retarget supplies the recoil. Blending and resampling are recorded in [the manifest](../../work/natural-moveset/choke-breakout-bank-manifest-v592.json), including exact donor hashes. This is a composited candidate, not a native third-person player breakout clip. No first-person constraints, root displacement, damage or animation events are imported.

[Candidate592](../../work/movement-integration/candidate-choke-breakout-v592/identity.json) adds exact `combat_lose/Npc/1500` variant4, protocol64/400 bytes, and guards older banks against the new clip index. It retains all target/arm/state ownership checks. Build, protocol, proxy and actual-bank tests pass; runtime remains pending until a separate receipt. [Harness593](../../work/natural-moveset/Test-ChokeNetwork-v593.ps1) tests the new branch and the previous finish branch using ordinary input, same-floor observer trials on both sides, independent source and Steam presentation checks, recovery after the actual terminal phase, and restoration. Receiver victim alignment/animation and shared outcomes remain unimplemented by this binding.

### Failed583 and correction584

[Corvo583](../../work/natural-moveset/choke-network-corvo-v583/result.json) independently validated and presented phase1, then failed because the reader required physical stance0. Full579/583 evidence shows the player changes to stance12 during struggle and completion. This was present in the earlier579 record and was missed when only the first state sample was inspected. The source never stopped playing its native loop/finish.

[Candidate584](../../work/movement-integration/candidate-choke-network-v584/identity.json) accepts only stance0 or12 with exclusive exact Choke41. The native reader repeats the stance equality check at exit. Exact owned scenes authorize the remote full-body gesture alongside the independent root channel, as the existing traversal path already does. The legacy stance protocol still accepts only0/1. New tests refuse neighboring stance13 and unsupported crouch1. Packet/bank version63 and351 clips remain unchanged. No gameplay event or victim mutation is added.

[Harness585](../../work/natural-moveset/Test-ChokeNetwork-v585.ps1) retains the original phase/body-weight criteria. It also refreshes source position after observer camera setup, because camera preparation itself lets a walking guard move. Guard positions are observed, never written. Completed585 receipts determine runtime success;584's identity remains marked unverified until a separate receipt.

Complete the recorded581/583 checks and retain failures. Confirm Emily's native phase contracts before counting her binding. Remote victim placement/animation, struggle matching and shared unconscious/death/corpse outcomes remain separate required co-op work. Cancellation, interruption, front/air/slide takedowns, moving a grabbed target and character-size/contact variants are not covered here. Fine contact, hand shaping and footwork are deferred polish; do not confuse that with the unimplemented victim gameplay.

## Crouched takedowns and the choke-hold packet rejection (candidates 1213p–1213r, audit 1246)

Updated 2026-10-15 (#11, `crouched_takedown`). [Audit 1246](../../work/movement-integration/crouched-takedown-audit-v1246.json).

- **Native (probe [1244b](../../work/natural-moveset/crouched-takedown-v1244b), both roles, confirmed runtime):**
  - A rear choke started from crouch stands the player into exclusive Choke41 and plays the standing scenes: `choke_back` 600 ms, `choke_loop` 1400 ms and `choke_win` 1800 ms.
  - A fast-back lethal from crouch plays `assassination_fast_back` (1333 ms) while the player stays crouched: exclusive Assassination13 at physical stance 1. 1213p admits it ([chapter 42](42-native-fast-back-assassination.md)).
- **Lab fixture:** `PrepareShadowSword` alone drops the lab-restored note `note_dw_frommeagan` from the client, so every per-case restore re-runs `Restore-LabNoteInventory-v779` (probe 1244 stopped on this).
- **Choke-hold packet rejection (confirmed static and runtime, takes [1245](../../work/natural-moveset/crouched-takedown-network-v1245)–[1245c](../../work/natural-moveset/crouched-takedown-network-v1245c)):**
  - During the hold (Choke41 at physical stance 12) the wire stance was invalid, but the paired-state root motion stayed valid.
  - `validCoopPose` rejects motion without stance, so the peer dropped every pose packet of the hold (8 in 1245b). The lab's 250 ms lease then retired the partner.
  - 1213q's `engine_call_gap` timing ruled out an engine stall, and both windows kept rendering.
  - This affected the standing choke too, so the earlier choke bindings had regressed.
- **1213r:** the choke hold is presented as stance 0 on the wire, and motion is only sent with a wire stance (motion refusal 5).
- **Take [1245d](../../work/natural-moveset/crouched-takedown-network-v1245d)** passed every case in both roles, and restoration passed: crouched choke (phases 1/2/3), crouched lethal (phase 5), and a standing choke as the regression check. Locomotion returns after each.
- **Visual (held observer camera; accepted for all four crouched cases, both characters):** Corvo's choke is in [1247b](../../work/natural-moveset/crouched-takedown-observed-v1247b) (side view). Corvo's lethal and Emily's choke and lethal are in [1247d](../../work/natural-moveset/crouched-takedown-observed-v1247d).
  - The 1247d camera looks along the corridor from behind and above the attacker.
  - From in front of the guard, the guard hid the partner (1247). Side spots in the bookshelf corridor landed in walls or off the balcony (1247b/c).
  - The observer's guard is not animated in these lab takes (NPC stream off), so the release lowers into empty air. Victim alignment is its own item.
  - The partner rises into the standing paired clips.

