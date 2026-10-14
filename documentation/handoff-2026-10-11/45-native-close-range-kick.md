# Native close-range kick through the existing pipeline

## Confirmed source and asset contract

The preserved Emily611 targeted-combat test did not perform the expected slash. Its independent native scene monitor instead records `midnight/gameplay/player/base/combat/throwhit.midnightscene`, subscene `kick`, duration800ms, with one reciprocal owned right arm followed by two references to the same player1. Active state is Sneaking6 plus Slash39, physical stance1. The source recording was inspected during the preceding continuation and shows a native kick. The failed targeted-slash assertion remains intact.

[Read-only Void export624](../../outputs/d2-coop-lab/research/asset-exports/native-kick-v624/manifest.json) corroborates why the player appears twice: the native declaration contains PlayerRightArm, PlayerCamera and PlayerBody groups. Both camera/body groups list the native Corvo and Emily player body resources. It names `ply_rightarm_throw_hit_kick` and `ply_body_throw_hit_kick`. The native close-combat active-frame event spans163–667ms; this is native source behavior, **not an event to replay on the remote representation**.

The exported `components/characters/player/base/rules/slash.rulehandler` selects throwhit when `slash.isThrowHitAvailable` is true, with separate rules for the previous left/right slash. The upstream world condition that makes availability true is still unknown. The ordinary-input Emily623 replay selected standing right/rightb scenes, not kick. It passes four source-observation/restoration checks and is not kick verification.

## Ghidra investigation

[Saved transcripts624](../../work/movement-integration/native-kick-research-v624) preserve the targeted analysis, including two failed initial function lookups and their subsequently created function definitions. No game executable was modified. Exact program remains B671F824.

- Vtable141c85d28 is the native Slash state. Its attached callback14086b900 obtains a rule handler through1408947f0, stores it at state+A0 and registers callbacks for its rules. This agrees with the slash declaration's rule-handler reference.
- Entry14086aa80 resets several state flags and initializes weapon context. Update14086d060 tracks input-release flags and calls14086cf10/14086d200. These findings do not identify the throwhit eligibility predicate.
- 1408549c0, already used by the reader, stores the active native scene handle at state+24 and starts the scene through the game scene manager. No new invocation of this function is introduced.
- 14086bff0 and14086ca20 are small list lookup/removal helpers after targeted function creation; their existence does not establish kick selection logic.

## Candidate625

Verified620/622 is archived. Source before625 is preserved in [kick-pre-v625](../../work/movement-integration/kick-pre-v625). [Preparation625](../../work/movement-integration/prepare_kick_v625.py) extends the existing reader, timeline, bank and playback; [deployment preparation](../../work/movement-integration/prepare_kick_deploy_v625.py) retains the existing isolated build/deploy/test workflow.

Combat variant17 requires the exact throwhit/kick/800 contract, native Sneaking6/physical1, one owned right arm and exactly two references to the owned player. The duplicate-player allowance applies only to this resource and admits no NPC or partner. Reciprocal entities, arm ownership, scene generation, active state and repeated lifetime validation remain required. Standing and airborne kick contexts remain unobserved and refused.

Protocol67 rejects incorrect durations, loop phases, expired clocks and airborne kick claims. Bank66/354 appends the existing retargeted `sword_attack_kick` donor at clip353. The previous353 clip payloads remain byte-identical. Older banks cannot address the added clip. The kick retains its complete authored legs even during root movement; the locomotion lower-body override is disabled for this action. Local first-person control, network root and all gameplay outcomes remain native/source-owned.

[Bank manifest625](../../work/natural-moveset/kick-bank-manifest-v625.json) records the exact existing donor hashes and conversion. The1.5s donor is normalized to the observed800ms source clock for this first candidate. [Donor sheet](../../work/movement-integration/native-kick-research-v624/donor-sheet.jpg) was inspected: both native-model previews show anticipation, leg extension and recovery. This authoring preview is not D2 runtime evidence; timing/contact quality needs actual-game review. Roots, events and first-person constraints are excluded.

Ownership, protocol/timeline, proxy smoke and actual354-clip bank tests pass. [Harness626](../../work/natural-moveset/Test-NativeKick-v626.ps1) requires the real native scene selection and independent source ownership, Steam body playback, independent root, stance recovery, unchanged receiver victim health and full restoration. Runtime verification is pending. If ordinary input continues to select another action, retain that limitation and proceed to other missing contexts rather than repeatedly forcing this fixture.

## Deployed625 and retained626 result

Candidate625 is live. Emily626 fails the required native kick selection; actual source scenes are crouched slash933 and right hit recovery866. The kick remains runtime-unverified. Do not repeat random guard setups. The existing crouched path regresses successfully on625: Corvo627 PASS10, Emily627 PASS13 including right hit recovery15, unchanged receiver victim health and complete restoration. [Receipt628](../../work/movement-integration/kick-regression-verification-v628.json) separates this verified scope from the unproven kick branch. Next work is targeted native crouched heavy-attack observation629 using the existing central-room fixture.

## Kick eligibility source (static, 14 October 2026)

`throwhit` is the native context kick. Asset export [native-throwhit-v1032](../../outputs/d2-coop-lab/research/asset-exports/native-throwhit-v1032) shows `components/characters/player/base/sensors/throwhit.cpntplayersensorthrowhit` with `m_maxNpcBreakableAngle 30` and `m_ledge.m_ledgeMaxHeight 3`.

- The player sensor `arkCpntPlayerSensorThrowHit` (vtable RVA `1b42b68`; slot 27 `1403cd6d0` toggles enabled `+0x38`) collects NPCs, breakable targets and potential ledges. Its type strings include `arkBreakableTarget`, `arkPotentialLedge`, `arkNpc` and `arkResult`.
- The kick is therefore available when an enemy stands between the player and a breakable object or ledge within the angle limit. Emily 611 kicked the patrolling upper-floor guard at (-44.14, -142.99, 66.4), facing +Y. In 626 the guard had patrolled elsewhere, so ordinary attacks selected slashes.
- A deterministic fixture needs that guard/ledge geometry, or a reader of the sensor's result list. The native `teleport` command is unverified for NPC placement and has not been used.
- Per this chapter's earlier guidance, the fixture is not forced further now. `kick_or_shove_if_present` remains runtime-unverified.
