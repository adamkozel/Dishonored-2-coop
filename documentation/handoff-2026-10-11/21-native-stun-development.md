# Native stun presentation and stream continuity

## Observed blocker

The [v59 grenade impact take](../../work/natural-moveset/grenade-impact-state-corvo-v59/result.json) passes its standing grenade presentation checks, but its [native samples](../../work/natural-moveset/grenade-impact-state-corvo-v59/owned-scenes.ndjson) show an additional unsupported state after detonation. Corvo transitions from Running0 through Stun14 with EquipChange53, then Stun14 alone, before returning to navigation. The old motion reader refuses that combination, semantic updates expire, and subsequent ordinary movement loses its stream. This is a separate failure, not covered by the grenade pass.

Source-only [Corvo](../../work/natural-moveset/grenade-stun-scenes-corvo-v60/result.json) and [Emily](../../work/natural-moveset/grenade-stun-scenes-emily-v60/result.json) observations establish exact native scenes: `midnight/gameplay/player/base/hitreact/hit_react_stun_right.midnightscene` and `hit_react_stun_front.midnightscene`, respectively. Both use `Scene_1`, 800 ms, native active phase 3, and exactly one actor binding to local player handle 1. A concurrent 333 ms sword fast-unequip scene binds the player's right arm. The [asset export](../../outputs/d2-coop-lab/research/asset-exports/native-stun-scenes-v60/manifest.json) also contains left/back declarations, but those directions are not yet runtime-verified.

The isolated source grenade damages only the source test world; every run restores both checkpoints and verifies original player/avatar health and fourteen protected save/settings hashes. No test creates a permanent save.

## Exact executable findings

Original/clone executable SHA256: `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`.

- `arkPlayerStateStun` vtable: VA `141c86768`, state ID 14. Optional concurrent `arkPlayerStateEquipChange` ID 53, vtable `141c79cf8`.
- `140870ce0` updates the native countdown at state +0xa8 and coordinates equipment states 64/53 before returning to navigation. `140870470` accepts its native event parameters.
- Common navigation update `1408695c0` is called from stun update. This supports treating this exact standing combination as root-motion-readable; it does not establish crouched/airborne stun coverage.
- [Raw decompilation and references](../../work/ghidra-stun-v60.json) preserve successful and failed queries. [Common navigation decompilation](../../work/ghidra-stun-navigation-v61.json) is also saved. The persistent Ghidra bookmark `D2Coop/NativeStun-v60` at `140870ce0` records evidence and limits.

## Candidate v61

[Native scene reader](../../outputs/d2-coop-lab/src/native_player_stun_scene.h) requires default mode, standing, the exact Stun14 class plus at most one exact EquipChange53, active instance/scene identities, one reciprocal player binding, an exact direction asset, exact duration and bounded native time. It repeats identity/list/binding reads and re-reads player states before returning. Unknown states are still refused.

[Protocol 38](../../outputs/d2-coop-lab/src/shadow_action_protocol.h) adds distinct action 16 and bounded direction/time/duration fields. [StunPresentation](../../outputs/d2-coop-lab/src/stun_presentation.h) interpolates on the established source clock and rejects stale, reversed or changing same-event data. It never calls native damage, camera or gameplay events. Existing state freshness bounds remain unchanged.

[D2MOVE43 bank generator](../../work/natural-moveset/build_stun_bank_v61.py) appends four native third-person `sword_stinghit_*` body clips to each existing grenade bank, preserving all previous 260 clip payloads byte-for-byte. The [manifest](../../work/natural-moveset/stun-bank-manifest-v61.json) pins donor paths/hashes and resulting banks. Local skeletons retain their 215/219 joint layouts and identity root tracks. Playback maps the native 800 ms scene time onto the authored body clip. This is a visual candidate, not accepted animation quality or remote gameplay stun.

Offline checks pass: [native ownership/state fixtures](../../work/movement-integration/stun-identity-v61.log), [protocol/timeline integrity](../../work/movement-integration/stun-protocol-v61.log), [both actual banks](../../work/movement-integration/stun-banks-v61.log), and [DLL smoke checks](../../work/movement-integration/stun-smoke-v61.log).

[Candidate archive](../../work/movement-integration/candidate-stun-v61/manifest.json) preserves the DLL, probe, complete source and native test harness. DLL SHA256 `A699D40BC86A483C85F6256AECE5EACD51C78A44C0C71C41DE3C242DE074BFDC`; probe `EC3F445C3DE17A32060DC463E4F63371E148CFE72928343026C57C36057B89E1`.

The [live harness](../../work/natural-moveset/Test-StunNetwork-v61.ps1) requires the native source scene, a matching remote body phase, both streams continuously ready with unchanged tokens, cleared reaction, subsequent ordinary input advancing the remote gait, unchanged observer/avatar health, inventory rollback and both checkpoint restores. It runs only through the serialized native-operation runner. Live results and visual inspection must be appended before claiming verification.

## Still outside scope

Left/back native stimuli, crouched/airborne/other damage reactions, equipment gesture quality, fine foot/grip quality and reaction interrupt blending need evidence. This candidate does not replicate grenade flight/explosion/world damage or apply damage/stun to the remote local player. It repairs presentation ownership and continuity without changing the authoritative local first-person behavior.

## Live continuity verified for both roles

[Corvo v61b](../../work/natural-moveset/stun-network-corvo-v61b/result.json) and [Emily v61b](../../work/natural-moveset/stun-network-emily-v61b/result.json) each pass sixteen checks on the pinned v61 DLL. Native grenade phases, mechanism and one owned in-hand effect continue to work; the exact native stun advances the remote reaction; both streams remain ready with unchanged tokens throughout; the body recovers and native forward input advances its gait. Observer player/avatar health is unchanged. Inventories, both checkpoint worlds and fourteen protected hashes are restored. These are cosmetic reaction/continuity passes, not shared damage passes.

Both source/receiver temporal sheets were inspected and indexed in the dashboard. The wider view shows flinch, blast-driven root displacement, return to ordinary pose, and later walking. Fine contact and overall animation quality remain unaccepted. Videos are silent; declared effect audio is not independently verified by these recordings.

The [initial Corvo run](../../work/natural-moveset/stun-network-corvo-v61/result.json) is retained as failed: its final backward-input stimulus produced no world travel, although both streams survived and the reaction ran. The actor also left part of the close camera frame after knockback. The retry uses a wider observer camera and records native input plus before/after player positions; Corvo travels approximately 1.76 m with forward input. This establishes successful forward movement, without asserting a proven cause for the earlier blocked backward stimulus.
