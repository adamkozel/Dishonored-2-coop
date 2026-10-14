# Partner equipment on missions and item-wheel robustness (candidates 1071–1074j)

Updated 2026-10-14. Continues [shared doors](84-mission-shared-doors.md). Audit: [mission-equipment-audit-v1089](../../work/movement-integration/mission-equipment-audit-v1089.json).

| | Value |
|---|---|
| Live candidate | [candidate-npc-combat-v1074j](../../work/movement-integration/candidate-npc-combat-v1074j/identity.json) |
| Action protocol | 73 |
| Bank | 70 (unchanged) |
| Intermediate builds | 1071–1074i, archived as superseded steps |

## 1071: presentation gates lifted

The following no longer require the Dunwall tower; they need the same native map and world pair on both peers:
- ranged, grenade, heart and trap props;
- the Doppelganger visual;
- partner (network) native power effects.

The Dunwall observer-isolation fixture is local test scaffolding (its own comment says it is "never a world-authority protocol"). It is now required only for manual fixture requests and local fixture casts.

## The item wheel retired partner presentation (1072–1084)

**Confirmed runtime: the native item wheel is `arkGameLogicStatePowerwheel`.**
- The logic stack while it is open is `InGame(1) + Powerwheel(8)`.
- While opening, it samples as logic 1 with pending 8.
- Inside it, cinematic metadata is unavailable and the player state machine is unreadable; the stance reader still works.

Opening the wheel used to retire the action stream permanently on both roles. Each fix removed one blocker:

1. **1074, 1074b, 1074f:** a new predicate, `coopPresentationReady`, accepts the wheel shapes: logic 8 with pending 0, or logic 1 with pending 8, with no cinematic nodes. It is used only on ongoing presentation paths: pose receive, follow invalidation, bank lease, shadow stream and arm-probe pair. Starting presentation, world authority and checkpoints still require gameplay.
2. **1074c, 1074g, 1074j:** during its own wheel, the source sends payload-free protocol-73 holds.
   - The proof is two stable stance reads of the same live player and physics component (stance and state readers take an explicit `allowWheel` flag).
   - The hold carries the last validated equipment state, because the native health and sword readers refuse the wheel.
3. **1074i:** the action-stream wire validator (`validShadowAction`) still required `coopWorldPairReady` and rejected packets in both directions. It now uses `coopPresentationPairReady`.
4. **Diagnostics used to locate the blockers:**
   - follow-stop log lines carrying the exact condition bits;
   - `pairBits` in the stream status;
   - a read-only probe mode, `--presentation-hold`.

## Runtime (1086–1088)

- **Wheel survival:** the stream stays `ready` on both roles through an open/close; the source sends about 48 holds, and the observer shows a neutral body while the wheel is open.
- **Blink:** the partner's Blink aim is presented after the wheel (power variants 2/3).
- **Pistol:** virtual sticks don't drive wheel selection (no highlight), but the native D-pad quick slots do (HUD: Left pistol, Right crossbow, Up sleep darts, Down Blink). D-pad Left equips the pistol and LT enters the native `ReloadWeapon` state.
- **Corvo source (1088):** the client presents ranged kind 1 for 135 samples with the native pistol prop. Video: sword, neutral during the wheel, pistol drawn, reload with an ejected casing, aim.
- **Emily source (1088):** the host presents 153 samples with the prop on remote Emily. Same video sequence.
- **Checklist:** `item_switch_interruptions` → `partial_native_steam_wheel_interruption_and_quickslot_switch_both_roles_mission_map`, with bounded visual acceptance. `pistol_draw_aim_fire_reload_holster` notes the mission-map verification.

## Open

- Wheel stick selection under virtual input.
- Crossbow, grenade, heart, trap and powers via quick slots on missions.
- Pause and journal menus (logic 2/4) still depend on movement resume, which needs a live bank on the arming side.
- The stance-based hold also applies in gameplay when the full proof fails. It only ever proves the same live player.
