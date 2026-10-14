# Airborne grenade presentation v63

## Exact native evidence

Both [Corvo](../../work/natural-moveset/grenade-air-observe-corvo-v63/result.json) and [Emily](../../work/natural-moveset/grenade-air-observe-emily-v63/result.json) pass seven source-observation and cleanup checks. The source recordings were inspected and indexed. These are observations, not receiver integration passes.

Native state rows show physics stance 0, default mode, Jump2 (`1c85028`) with ThrowWeapon62 (`1c7f118`) while the grenade cooking loop runs. Fall3 (`1c845c8`) with ThrowWeapon62 overlaps the high-release scene. Cooking loop uses `Scene_1`, 1333 ms and reciprocal left-arm/item bindings. High release uses `Scene_1`, 400 ms and reciprocal left-arm/player-handle-1/item bindings. The scene paths remain `midnight/gameplay/player/base/gadgets/gadget_grenade_*.midnightscene`. Native first-person video corroborates the ascent, release above the initial view height and landing. Neither the input command alone nor view height alone authorizes a replicated action.

## Implementation

[Exact native state recognition](../../outputs/d2-coop-lab/src/native_player_states.h) separates idle Jump/Fall from an active ThrowWeapon overlay. [The grenade scene reader](../../outputs/d2-coop-lab/src/native_player_grenade_scene.h) admits that exact two-state combination for cook/release/recovery only, retaining the existing scene, arm, item, generation, duration and repeated-read checks. Unobserved airborne equip/unequip remain refused.

[Native air metadata](../../outputs/d2-coop-lab/src/native_player_action.h) requires a separately validated grenade scene before reading jump apex/remaining-jump fields through a throwing overlay. Held-idle observation uses exact single-state Jump/Fall and the existing equipped/started/native inventory checks. The source sends original native air timing alongside the grenade gesture. [Protocol39](../../outputs/d2-coop-lab/src/shadow_action_protocol.h) retains the 336-byte layout but explicitly versions the new semantic combinations; older protocol38 is refused. It bounds apex and stage, rejects stale fall apex, mixed hand ownership, orphan phases and unobserved airborne equipment phases.

[Body playback](../../outputs/d2-coop-lab/src/movement_playback.h) retains existing airborne pelvis/leg motion while the complete authored grenade gesture supplies upper-body motion. It does not move the network root, execute animation events or affect local first-person controls. D2MOVE44/273 banks are unchanged from v62. The cosmetic prop, six-joint native mechanism and owned spark/light/audio effect retain their existing source-clock lifecycle.

[Identity tests](../../work/movement-integration/grenade-air-identity-v63.log), [protocol tests](../../work/movement-integration/grenade-air-protocol-v63.log) and [DLL smoke tests](../../work/movement-integration/grenade-air-smoke-v63.log) pass. The preserved [candidate](../../work/movement-integration/candidate-grenade-air-v63/identity.json) contains source, binaries, bank manifest and test harness. DLL `C4AA318837D220E821F9FBF00FF3D744F39ED5AA922297914EEB57F0FC3CEAC7`; probe `35DE9C4E568AC47FAA6F9E0975972090712CC5302F9E3B25DE36263D169BDEC7`.

## Ghidra

[Raw decompilation and xrefs](../../work/ghidra-grenade-air-v63.json) are from the exact pinned executable. ThrowWeapon's vtable references `14084a8f0` at +0x10 and `14084a7e0` at +0x18. They query `140802c20` and inspect returned +0x160/+0x170. At +0x30, `14084a940` queries the same object, invokes its virtual +0x110 and conditionally calls `140854890`. Their precise gameplay roles remain unconfirmed; this inspection does not establish projectile spawn or authority. Bookmark `D2Coop/GrenadeAir-v63` at `141c7f118` records the independently confirmed runtime combinations and these limits. The Ghidra project was saved.

## Runtime scope

[The isolated network harness](../../work/natural-moveset/Test-GrenadeAirNetwork-v63.ps1) records an idle held-grenade jump, native cooking followed by a second jump/release, remote body/prop/effect lifecycle and landing. It checks native Jump and Fall independently, same stream tokens through the sequence, item rollback and both checkpoints plus protected hashes. Receiver runtime results are pending. Shared projectile flight, explosion/world damage, other movement contexts and final visual acceptance remain incomplete.

### First network failure and v63b correction

[Corvo v63](../../work/natural-moveset/grenade-air-network-corvo-v63/result.json) fails because remote release phase 5 is absent. The source reader validates all phases, and native release occurs. Receiver telemetry records rejection reason 19 at the airborne cooking overlap: `MovementSemantics::push` retained a separate grounded-only restriction for state15 even though the packet validator admitted the new air metadata. Both recordings were inspected, the failure was indexed, and both checkpoints/protected files restored. The old receiver framing also crops the top of the idle jump; the retry camera is farther away and level.

[Candidate v63b](../../work/movement-integration/candidate-grenade-air-v63b/identity.json) adds state15 to the delayed air timeline, retaining all bounds, freshness and ordering rules. [Regression tests](../../work/movement-integration/grenade-air-protocol-v63b.log) now pass a validated grenade packet into the movement timeline, verify that a future fall cannot change an older jumping pose, and confirm the fall appears at its own source time. [Smoke tests](../../work/movement-integration/grenade-air-smoke-v63b.log) also pass. Banks and wire layout remain unchanged.

Initial v63 deployment also encountered a transient native status-file sharing violation during restoration. [Restore-D2Lab.ps1](../../outputs/d2-coop-lab/Restore-D2Lab.ps1) now retries locked, truncated or empty reads eight times at 15 ms intervals. It does not cache old snapshots or weaken process, startup freshness, world-epoch or ready-state checks. The separate [prepare retry](../../work/movement-integration/prepare-grenade-air-v63.log) completed successfully. The failed deployment is retained in its log and is not described as a completed restore.

### Verified v63b runtime

[Corvo](../../work/natural-moveset/grenade-air-network-corvo-v63b/result.json) and [Emily](../../work/natural-moveset/grenade-air-network-emily-v63b/result.json) each pass sixteen checks. Both actual-game source/receiver temporal sheets were inspected and indexed. The wider level camera contains the complete jumps. Native idle held-grenade jump, cooking through the next jump, high release during descent, empty recovery and ordinary standing are visible. Exact native Jump/Fall observations agree with receiver airborne body motion. The grenade mechanism, one owned effect start/stop, release disappearance and duplicate-packet refusal pass. Both streams remain armed/ready with their original tokens; both inventories/checkpoints and protected14 restore.

The measured v63b DLL is `28F1DB269BD455B85D6CA86C34F13DBD007C3EFDF7DCFB6F468911C10E6FC0AF`, probe remains `35DE9C4E568AC47FAA6F9E0975972090712CC5302F9E3B25DE36263D169BDEC7`. Protocol39 and bank44/273 remain in use. These takes use forward input and a high throw. Low airborne release, all direction/speed combinations, upgraded double jump, airborne equipment changes and broader interruptions are not established. Fine contacts and final visual acceptance remain separate; the receiver still has no shared projectile or explosion damage.
