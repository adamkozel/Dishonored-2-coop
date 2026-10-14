# Architecture and invariants

Primary code is [src](../../outputs/d2-coop-lab/src); the generated [source map](generated/source-map.md) lists every current source module and [native address occurrences](generated/source-addresses.md) links constants and nearby source context. These are navigation aids, not automatic proof that every constant is a function address.

## Process and data boundaries

The original Steam installation is preserved. The early `work/lab-game/base` directory is a junction to Steam assets, so it must never receive asset edits. Resource experiments run from the physically separate `work/asset-game`. `work/lab-instances/host` and `client` contain isolated launch records, saves, config, input and status files. The local host role is Corvo and client role Emily in the established fixture.

The proxy forwards the real DirectInput export, scopes only the expected retail mutex and validates exact executable bytes before process patches. Commands requiring native mutation run on the game's established engine thread. The external runtime probe reads state independently; it is not a general arbitrary-memory writer.

Controller packets are finite, atomic and role-specific. Expiry returns neutral input even if a sender stops. The developer command mailbox is single-slot; use `-WaitConsumed` when ordering commands. Consumption only means the engine read a command, not that its requested operation succeeded.

## Networking

The game keeps its original Steam initialization and shutdown lifecycle. The mod uses the existing user and pipe with `SteamClient017` generic-interface access to obtain `SteamNetworkingSockets012` from the running Steam client. Same-PC transport is established. This must not be presented as verified internet sessions, lobby/invite support or two-account play.

Packets bind the applicable process session, world epoch, map, character role, recent root sequence, opt-in tokens and actor/body identities. Wire payloads use semantic values rather than native pointers or process-local interned names. Freshness and generation checks retire stale presentation. Specific stream protocols and lengths are versioned in the source and per-build tests; do not reuse an old length from a historical report.

Root presentation buffers snapshots and interpolates translation and shortest-arc rotation. The early smoothing implementation used 80 ms and reset on gaps over 250 ms or discontinuities over 3 m. Later timing changes must be checked against current source and exact reports. Authored locomotion uses travel/native timestamps; it must not translate the local player or inject a second source of root motion.

## Animation and equipment

Owned native Doppelgangers retain native meshes, rigs, inventory, skin helpers, correctives and child attachments. Custom definitions strip selected autonomous lifecycle/combat components before initialization; bounded native decision/idle holds control remaining autonomous behavior. Spawn initialization timing and unowned behavior require care.

The authored movement bank is a guarded mod format, not a native FBX loader. Native animation evaluation, procedural/IK phases, authored overrides and final pose publication have distinct timing. A status-file snapshot may lag the rendered pose. The compact final contact probe brackets the carrier publication and reports whether it is complete; the victim read remains sequential rather than atomic with it.

Sword transforms depend on native inventory ownership, the correct slot/socket, actor parent transforms and attached-child updates. A mathematically correct socket does not prove the fingers wrap the hilt. Holstered clone swords can require hiding the actual owned child and restoring its prior state on cleanup.

## Gameplay and damage

Pose playback is presentation and must not independently create damage through animation events. The outgoing clone damage firewall prevents duplicate presentation damage. The friendly-fire filter is separate: it rejects attributed player/owned-clone damage at native health entry, following generation-checked attacker/inflictor parent chains.

Authority experiments for guards and damage are explicit bounded adapters. They do not make the entire receiving world authoritative. General combat must reconcile damage attribution, reactions, deaths, corpse state and quests without double application or blanket suppression of legitimate authoritative outcomes.

## Carry lifecycle

Current source modules include [carry_attachment_fixture.h](../../outputs/d2-coop-lab/src/carry_attachment_fixture.h), [carry_presentation.h](../../outputs/d2-coop-lab/src/carry_presentation.h), [carry_victim_scene.h](../../outputs/d2-coop-lab/src/carry_victim_scene.h), [native_player_carry_scene.h](../../outputs/d2-coop-lab/src/native_player_carry_scene.h), [movement_playback.h](../../outputs/d2-coop-lab/src/movement_playback.h) and [movement_bank.h](../../outputs/d2-coop-lab/src/movement_bank.h).

For the known Mayhew experiment, native Carry45 preparation and hoist timing control the remote carrier and victim blend. Identity is bound before use. A snapshot bridges the floor body into the victim-only native scene. Retargeting/attachment is established at the appropriate late phase. The v30 anchor preserves native clip orientation and continuously derives body position from the current shoulder, avoiding rotation feedback and stance drift. Native drop relinquishes animation ownership only after copying visible pose into rigid bodies and zeroing their velocities. Exact native functions, offsets and confidence are in the decompilation export.

Cleanup must be explicit on expiry, cancel, pause, map/world/session replacement and peer loss. Never reuse an actor pointer across these boundaries. Do not turn an experimental fixture into automatic global ownership without proving selection, generation and cleanup invariants.
