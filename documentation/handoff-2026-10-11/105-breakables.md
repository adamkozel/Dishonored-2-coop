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

- Multi-step barricades are not on the lab map, and Edge of the World has none either: its planks have one step
  (#179, below). The step rule and the health lowering cover them (hypothesis until a map with one).
- Grenades and other damage sources take the same path, because the stream polls steps whatever broke them. On a
  mission map, a grenade's and a Windblast's breaks were shared (confirmed runtime, #179 below). The 1273b grenade door
  is not retaken.

## Movable props and pickups (#178)

Breakables covered fixed glass and barriers. Loose props and pickups were still per world. Probe-PhysicsProps-v1766 to
v1773 measure a host Windblast (or a partner sword swing) on the Dunwall tower lab map, reading every entity in both worlds
before, 5 s and 15 s after. The lab is restored at the end.

**Found (confirmed runtime):**
- At rest, the 29 `arkMovable` props sit at identical positions in both worlds.
  - The first takes moved nothing: v1766a never cast; v1767a/v1768a hit only a chair, the globe and debris.
  - v1768a: the Windblast threw broken-door debris 10-25 m in the host world. Debris pieces are runtime entities that the two
    worlds name differently, so they are not matched (cosmetic, not shared).
- physics-props-v1769a, the table 3 m in front:
  - the Windblast broke four bales and two bottles in the host world only; the partner's world kept them all;
  - the fruit flew up to 18 m off the table in the host world only.
- Bottles, glasses and bales are `arkMovable` entities (vtable RVA `1c5de18`) with the same `arkCpntBreakable` and
  `arkCpntHealth` as breakables. Pickups are `arkPickup` (`1c5e258`) with a rigid body.
- `ld_key_01` moves with its guard: it is bound to his belt.

**Breaking (1214io):**
- The breakable reader accepts `arkMovable` entities, and hits on them are noted.
- An entry that vanishes within 5 s of a noted hit is sent as broken. A smashed bottle leaves the world at once.
- physics-props-v1770a: the host sent 6 breaks, and the partner applied all 6 through the native step advance.

**Resting places (1214ip-1214ir, [prop_rest_sync.h](../../outputs/d2-coop-lab/src/prop_rest_sync.h)):**
- **What is sent:** each world catalogs its movables and pickups and polls them every 250 ms. A prop that moved and has
  been still for 1 s is sent (`D2PR`, 312 bytes) when:
  - it is more than 5 cm from its last rest;
  - no player holds it (the held object stream carries held ones);
  - it is not quiet after an applied rest;
  - it is not bound to another entity.
- **Applying:** the peer places its copy through the native entity transform setter (`0x87d200`), unless its own copy is
  held or bound. That prop then stays quiet for 2.5 s, so it is not sent back.
- **Switch:** lab command `coop_prop_rest on|off`; product step 22, also in the maintenance list.
- **Bound** (confirmed runtime):
  - The native reference frame (`arkCpntReferenceFrame`, `1c573e8`) names what a prop stands on or is bound to at +0x84
    (-1 is none).
  - Only a prop bound to a joint also has a local offset at +0x70/+0x78. Of 98 props on the map, only the guard's key had
    one; fruit on a plate and blasted fruit read a parent with zero offsets.
  - 1214ip used the distance to a character instead, and sent Alexi's belt scroll case (v1771a). That item had fallen when
    the blast knocked him over, so syncing it was right, but the rule was fragile.
  - 1214iq used the +0x84 parent and sent nothing (v1771b: blasted fruit stands on something).
  - 1214ir uses the local offset.

**Takes:**
- physics-props-v1772b (host Windblast, 1214ir):
  - the six table props broke in both worlds;
  - the four fruits rest within 1 mm of each other;
  - Alexi's fallen scroll case rests in the same place in both worlds;
  - the guard's key stays on its guard: 7 bound skips, 1.3 cm apart.
- physics-props-v1773a (partner sword swing, 1214ir): the partner sent the apple's rest and the host placed it 2 cm from hers.

**Open:**
- Breakable debris chunks and dropped guard swords stay per world (cosmetic).
- The mission take is #179, below.

## Mission take: Edge of the World (#179)

Survey-MissionBreakables-v1774, Frame-MissionSpots-v1775 and Test-MissionBreakables-v1776 to v1779 load Edge of the
World in both lab games under the product controller. The source stands in front of the boarded window near the start
(stand b3-px, mission-spots-v1775a) and breaks its planks, `env_wooden_plank_03_11` to `_14`. Every target plank that
breaks in the source world must break in the other world too. Both streams' status and frames of both windows are
kept, and the lab is restored at the end.

**Found (confirmed runtime):**
- With the movables, the map has 1269 breakables (mission-breakables-v1774a), all with one step. The planks are
  `arkBreakable`s with health 10.
- The stream's catalog stopped at 1024 entries, so later breakables were never polled (1778a). 1214is raises the cap to
  4096 and counts a full catalog (`catalogFull` in the status). In the 1214is takes the catalog holds 1265-1269 and
  `catalogFull` stays 0.
- The lab grenade fixture refuses mission maps (1776a, `unsupported_scope_or_item_owned`), so the takes grant a grenade
  with `coop_grant_item grenade`. 1777a stopped on the grant's result name, `granted_item_equipped`.
- The prop rest stream runs on the mission too: 959-963 movables and pickups, and the rests the blast caused were
  applied in the other world.
- On this map one catalog scan takes 15-32 ms per stream, every 15 s. The 100 ms and 250 ms polls take under 1 ms.

**Takes:**
- mission-breakables-v1778a (host grenade, 1214ir): the grenade landed at the foot of the barricade. It broke a bottle
  and two glasses but no plank, and the partner applied all three breaks.
- mission-breakables-v1779a (host sword, 1214is): Corvo's swing broke `env_wooden_plank_03_12`, and the partner's world
  broke it too (sent 1, applied 1).
- mission-breakables-v1779b (partner sword): Emily's swing broke the same plank, and the host applied it.
- mission-breakables-v1779c (host Windblast): the blast broke all four planks and a glass in the host world. The host
  sent 8 breaks and the partner applied all 8, so the window is open in both worlds.
- Visual review: the 1779c host frame shows the native splinters flying from the window.

**Open:**
- A grenade that breaks the planks themselves (its breaks take the same stream: 1778a).
- The 15 s catalog scans may show as a short hitch on large maps.
