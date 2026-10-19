# 105. Breakables shared between the worlds (#132)

Before this chapter, a breakable broken in one world stayed whole in the other. In grenade-real-v1273b-host a grenade blew
the office door's glass apart in one world only (chapter 95). Live candidate: 1214fq.

## Native breakables

- **Entity and component** (vtables confirmed runtime with `--components`):
  - a breakable is an `arkBreakable` entity (vtable RVA `1b67bc8`) with one `arkCpntBreakable` (`1c71a88`) and its own
    `arkCpntHealth` (`1c51268`);
  - the Dunwall lab map has 51: door, display-case and cupboard glass, and a chamber pot. All have one step, with
    health 1 or 10.
- **Layout** (confirmed static, `1407ed920` and `1407ed000`):
  - component +8 is the declaration: steps at +0x68 (stride 0x60, health threshold at +0), count at +0x70;
  - component +0x20 is the health component (current at +0x20), and +0x38 is the current step.
- **Break path** (confirmed static):
  - `1407ed920` advances a step when the health falls to the step's threshold.
  - `1407ed000(component, damage params)` advances one step: it detaches the step's pieces, spawns its FX and chunks
    with the hit impulse (params +0x24 hit point, +0x3c direction, +0x54 attacker), and notifies scripts.
- **Final step** (confirmed runtime): a breakable on its final step leaves the reader's catalog (51 to 50 in 1660a), so a
  broken single-step pane reads as gone.
- **Reader:** [native_breakable.h](../../outputs/d2-coop-lab/src/native_breakable.h). The probe's `--breakables` lists
  each breakable with its step, health and position.

## The breakable stream (1214fq)

[breakable_sync.h](../../outputs/d2-coop-lab/src/breakable_sync.h),
[breakable_protocol.h](../../outputs/d2-coop-lab/src/breakable_protocol.h) and
[breakable_hits.h](../../outputs/d2-coop-lab/src/breakable_hits.h). Enabled with `coop_breakable_sync on|off`; it is product
step 21 and in the maintenance list.
- Each world catalogs its breakables (per world, and every 15 s) and polls their steps every 100 ms.
- **Sending:** a step that advanced locally is sent once, as `CoopBreakablePacket` (`D2BK`, 296 bytes: handle, name,
  step, count, session, epoch and map). The packet carries the hit point and direction of the native damage that broke
  it; the damage observer notes them without changing the damage.
- **Applying:** the peer advances its copy to that step through `1407ed000`, one step at a time, with default
  `player_sword` damage params carrying the hit, so the pieces, chunks and FX are the game's own.
  - The peer then lowers the breakable's health to the applied step's threshold, so later hits break the next step as
    in the other world.
- Breaks only go forward. An applied step counts as sent, so nothing echoes. Either role sends.
- Unit tests cover the packet, the step rule and the reader over a synthetic entity.

## Takes (Test-Breakables v1659-v1662)

- Each role attacks a display-case glass with the sword under the product controller.
  - Both worlds' breakables are sampled. A world's broken set is every breakable whose step rose or that disappeared.
  - Both worlds must break the same set. Films come from the source and from a camera held in the peer world.
- **breakables-v1662b, Corvo** (confirmed runtime): his sword broke `esc_env_display_case_glass_01b_5` at 5.3 s.
  Emily's world broke the same pane in the same 150 ms sample, so the broken sets are equal.
- **breakables-v1663b, Emily:** her first press broke `esc_env_display_case_glass_01a_5`, and Corvo's world broke the
  same pane in the same sample.
- **Stream counters:** breakables-v1660a shows a step sent with its hit in each direction, applied natively (1 step,
  health lowered).
- **Visual review:**
  - in Emily's world (1662b), native glass shards spill from the case onto the rug at Corvo's break;
  - in Corvo's world (1663b), shards burst from the case as Emily's avatar swings.
- **Fixture failures** (kept): 1659a (the stand was inside the cupboard), the harness watching only the named pane
  (1660a), and swords that missed (1661a, 1662a door glass, 1662b and 1663a Emily).

## Open

- Multi-step barricades (wooden planks) are not on the lab map. The step rule and the health lowering cover them
  (hypothesis until a mission take).
- Grenades and other damage sources take the same path, because the stream polls steps whatever broke them
  (hypothesis; the 1273b grenade door is not retaken).
