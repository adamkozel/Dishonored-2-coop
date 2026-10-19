<!-- Chapter 100: static research (Ghidra MCP, read-only) into the native conditions behind the window, Shadow Walk wall, seated and knocked-down takedowns, written 2026-10-19 for #11 wall_and_window_variants and enemy_body_size_variants. Lab takes that applied it: mission-coverage-v1518 (knocked down), v1519 (window); see chapter 99. -->

# Native takedown trigger research (static, Ghidra MCP, read-only)

Exe: retail Dishonored2.exe (image base 0x140000000; repo notes give SHA B671F824...). Nothing was renamed,
retyped, commented or saved in Ghidra. `disassemble_bytes` was only used with `dry_run=true`.
Labels: **[CS]** = confirmed static (seen in code/data), **[H]** = hypothesis, **[RT-repo]** = runtime fact
already recorded in the repo (not re-verified here).

## 0. Common machinery (applies to all four kinds)

- Player sensors live in a per-player table: `FUN_140777b90(table, slot)` = `*(table + 0x20 + slot*8)`;
  the table is `player+0x338` (also cached at `sensor+0x30`). Each sensor decl sets its slot at decl+0x68. [CS]
  - slot 12 = arkCpntPlayerSensorWindowAssassination (decl ctor 0x1403d5870 sets +0x68 = 0xC). [CS]
  - slot 13 = arkCpntPlayerSensorWall (decl ctor 0x1403d1990, vtable 0x141b432d8, RTTI
    `.?AVarkDeclCpntPlayerSensorWall@@`). [CS]
  - slot 33 (0x21) = arkCpntPlayerSensorInteraction (decl ctor 0x1403b7a40, RTTI
    `.?AVarkDeclCpntPlayerSensorInteraction@@`; component vtable 0x141b403b8). [CS]
  - sensor vtable slot 26 (+0xD0) = IsAvailable, slot 27 (+0xD8) = SetEnabled(bool). [CS]
- Takedown conditions are data-driven. NPC `arkCpntAssassination` (decl assassination_npc) maps
  ASSASSINATION_TYPE_{SHADOW_WALK,SLIDE,BASE,WINDOW,DROP} to rule handlers
  `components/characters/npc/base/rules/assassination_{shadow_walk,slide,base,window,drop}.rulehandler`;
  the player's `arkCpntPlayerSensorGlobalAssassination` (decl globalassassination) lists the same handlers
  (repo export outputs/d2-coop-lab/research/asset-exports/partner-targeting-v384). [CS data]
  The **scene names (choke_window_*, choke_sit_*, choke_knockdown_*, choke_sw_wall_*) are not in the exe**
  (string search: 0 hits) - they live in those .rulehandler decls, which are NOT exported in the repo yet.
- Conditions those rule handlers can use on the player side are the `arkPlayerAssassinationTargetInfo`
  TI providers (table 0x1423a38c0..0x1423a3a00, entries = {name, wrapper, type, rettype}); the context's
  `+0x10` = player entity, `+0xb8` = candidate NPC handle. [CS]

  | TI name | wrapper -> impl | what it checks [CS] |
  |---|---|---|
  | TargetIsVulnerable | 1409e9e70 -> 14082d2f0 | NPC `+0xd1` bit 6 |
  | TargetIsUnaware | 1409e9ed0 -> 14082d250 | FUN_1405bf560 (AI awareness) or player-invisible special case |
  | TargetIsAlwaysChokable | 1409e9f30 -> 14082d160 | NPC `+0xd1` bit 4 |
  | TargetIsGrappled | 1409e9f90 -> 14082d180 | NPC == player's grapple target |
  | TargetIsSitting | 1409e9ff0 -> 14082d200 | see section 3 |
  | IsBehindWindow | 1409ea050 -> 14082cb80 | see section 1 |
  | IsLayingOnGround | 1409ea0b0 -> 14082cc20 | see section 4 |
  | IsRecoveringFromRagdollLyingDown | 1409ea110 -> 14082cd10 -> 1405bf4c0 | see section 4 |
  | IsRecoveringFromRagdollDoggyStyle | 1409ea170 | (AIPhysicsReaction +0x13c == 1) |
  | IsOnGround | 1409ea1d0 -> 14082cc80 | see section 4 |
  | TargetDirection | 1409ea230 -> 14082cd40 | back/front/left/right; for an on-ground NPC it is computed from the ragdoll bone orientation, else cached `+0xbc` |
- Generic per-NPC eligibility (FUN_1403b00a0, already bookmarked by the project) [CS]: NPC must be an AI
  entity; `arkCpntAssassination`-ish flag `npc+0x368` bit0 set; NPC `+0xd1` bit3 clear; not the partner;
  not currently on a navigation link whose object (`nav+0xa8 -> +0xd8`) vfunc +0x20 returns false; Midnight
  layer priority (component DAT_1439ba280, FUN_1407659c0(c,0,2)) must be <= 23; not in the DAT_1439e27c0 /
  FUN_1405bfbb0 state; FUN_1403ee810 false. [H] the nav-link and priority gates are good candidates for why
  freshly teleported lab targets refused takedowns (repo 1476b note).

## 1. Window takedowns (assassination_window_*, choke_window_*)

### Player sensor (arkCpntPlayerSensorWindowAssassination, vtable 0x141b43a28)
Functions (from .pdata, several not defined as functions in Ghidra):
- ctor 0x1403d5700; shapes built in 0x1403d5970; pre-physics query submit **0x1403d64c0**;
  post-physics evaluate **0x1403d6050** (the "UpdateQueryPhysics" profiler string 0x141b43b08 is the
  query tag passed to FUN_140095280); obstacle collector 0x1403d5b20; NPC-beyond-window test 0x1403d5d90;
  IsAvailable (slot 26) 0x1403d5f70; SetEnabled (slot 27) 0x1403d5fc0. [CS]

Decl defaults (decl ctor 0x1403d5870) [CS]:
`+0x70 0.7 (windowMinWidth)`, `+0x74 0.9 (windowMinHeight)`, `+0x78 1.0`, `+0x7c 1.0`, `+0x80 0.75`,
`+0x84 15.0 (deg)`, `+0x88 0.1`, cylinder `+0x8c 0.3, +0x90 0.3, +0x94 0.2`, `+0x98 0x2b` (collision filter),
base `+0x68 = 12`, `+0x6c enabledAtStart = 1`.
The shipped decl overrides only windowMinWidth 0.7, windowMinHeight 0.9 (= defaults) and npcMaxDistance 10.
Field names are not in the exe, so which offset npcMaxDistance is is **[H]**: most likely `+0x7c` (forward
sweep length, default 1.0 -> 10 m); if it were `+0x80` the opening box would be 10.3 m long and almost no
real window could pass query C, so `+0x80` is unlikely.

Shapes (0x1403d5970, FUN_1400efc60 / FUN_1400ef880) [CS]:
- `sensor+0x40` "cylinder": 8-sided prism, radius decl+0x90 = **0.3 m**, z from decl+0x94 = **0.2 m** to
  decl+0x8c+decl+0x94 = **0.5 m** above the player origin (shin height).
- `sensor+0x48` box (player-local, x = player forward): x **-0.3 .. +1.05 m**, y **+-0.35 m**,
  z **1.1 .. 1.9 m** (min = (-d90, -d70/2, d88+d78), max = (d90+d80, d70/2, d74+d78)).

Queries, all from the player origin (`entity+0x300`) with the player body rotation (`entity+0x30c..`) [CS]:
- **B** (result word +0x140): sweep the shin cylinder along the player's forward axis by decl+0x7c
  (1 m default, 10 m if npcMaxDistance is +0x7c), filter 0x2b.
- **C** (result word +0x1c0): sweep the opening box straight down by 2*decl+0x88 = **0.2 m**, filter 0x2b.
- **A** (result word +0xc0): overlap of the opening box (no sweep), filter 0x28, collected by 0x1403d5b20.

Evaluation (0x1403d6050) [CS]:
1. Target NPC = `interactionSensor(slot 33)->+0x88` via FUN_1403b8080(sensor, 1) (the `1` ignores the
   interaction sensor's "blocked" bit `+0xb8&1`), stored as handle at **+0x1c4**. This is the answer to "what
   writes slot-12 +0x1c4": the window sensor itself, every frame, copying the interaction sensor's
   assassination/sword target (+0x88). The interaction sensor (update 0x1403b8430) sets +0x88 for AI hits of
   its first four view queries and sets +0xb8 bit0 from an obstruction test (FUN_1403b7b90). [CS]
2. Query B: hits sorted; first hit that is **not** a character (FUN_14061e1a0 = layer flag 4 + component
   DAT_1439cb320) gives the window point +0x1d8 (the wall face under the sill). `+0x1e8` = 1 iff the angle
   between the 2D player->NPC vector and the reversed hit normal is <= decl+0x84 = **15 deg**. [CS]
3. Query C: first non-character hit; d = fraction*0.2 m. `+0x1e9` = d > 0, `+0x1e4` (sill height above player
   origin) = 1.0 + 0.1 - d. So the box must **start free** (fraction 0 = fail) and must **hit a surface within
   0.2 m going down**: sill top between **0.9 and 1.1 m** above the floor the player stands on, opening clear
   from the sill to **>= 1.9 m** above that floor, **>= 0.7 m** wide, and clear from 0.3 m behind the player
   to 1.05 m in front of the player (frames, glass, bars, curtains, shutters or a low lintel inside that box
   all kill it). Characters (the NPC) inside the box are ignored. [CS]
4. Query A: characters overlapping the opening box that are above the sill or closer than the target are
   collected into list +0x1c8 (consumer not found; probably "someone in the way"). [CS collection, H use]
5. `+0x1ea` = FUN_1407fcb90(movement component sensor+0x28, ...) - player grounded (not +0x2cc/+0x2cd,
   finite ground distance) and not ceiling-blocked. [CS code, H naming]
6. IsAvailable (0x1403d5f70) = `+0x1ea && NPC beyond window (0x1403d5d90) && +0x1e8 && +0x1e9 &&
   playerState(sensor+0x20) != 2`. State 2 is Shadow Walk (TIProvide_ProvidePlayerShadowWalk 0x140575640
   returns state == 2), so **no window takedown while in Shadow Walk**. [CS]
   0x1403d5d90: the NPC's 2D projection on the player->window-point direction must be >= the distance to the
   window point, i.e. the NPC is past the near face of the wall. [CS]

### NPC/AI side
- `TIProvide_ProvideIsBehindWindow` (0x14082cb80) is a player-side TargetInfo condition: player's slot-12
  sensor IsAvailable and its +0x1c4 == candidate NPC handle (`npc+0xa8`). [CS]
- No code path requires a window distraction/lookout behaviour; `distraction_lookaround_window.midnightscene`
  is only data. Whether assassination_window.rulehandler additionally requires TargetIsUnaware etc. is in
  the unexported rule handler. [H: it requires unaware, like the base rule]
- Direction variant (back/front/left/right) comes from TargetDirection (cached `+0xbc`, player relative to the
  NPC's facing). [CS that the TI exists; H that window rules use it]

### Why the 0.55 m / 0.55 m lab attempts probably failed [H, ranked]
1. Sill height: query C only succeeds if the sill top is 0.9-1.1 m above the player's floor and the opening is
   clear to 1.9 m. A low "crossable" window (mantle/short-jump sill) or a window whose lintel is below 1.9 m
   above the player's feet fails. 1482 found the Edge of the World wall ~0.6 m thick; with the player 0.55 m
   from the centre line, the 1.05 m box reaches only 0.5 m past the centre line, fine, but any frame/shutter
   geometry inside x -0.3..1.05 m at z 1.1-1.9 kills it.
2. The NPC must be the interaction sensor's +0x88 target, i.e. the player must be looking at it (view
   queries, interaction decl range +0x70 = 3.0 m default).
3. Facing: the player body yaw must point at the wall under the sill (query B) and the NPC must be within 15
   deg of the wall normal as seen from the player.
4. Earlier lab notes: teleported NPCs were ineligible for a while (see section 0 gates).

### Fixture recipe (window)
- Pick a window with: solid wall under the sill; **sill top 1.0 m (0.9-1.1) above the player's floor**;
  opening >= 0.8 m wide and clear from the sill up to >= 2.0 m above the player's floor; no bars/glass/frame
  members inside the opening; NPC side floor at about the same level.
- Player: standing (not shadow walk), on the ground, body yaw perpendicular to the wall, origin 0.4-0.6 m
  from the near wall face (closer than ~0.7 m so the 1.05 m box reaches over the sill; farther than 0.3 m so
  the shin cylinder starts outside the wall). Look at the NPC's torso.
- NPC: on the far side, directly across (within 15 deg of the wall normal through the player), 0.3-1.0 m past
  the far face; unaware (idle/patrol, back to the window for the _back variant); let it settle >= 3 s after any
  placement.
- Pass checks to read at runtime on the slot-12 sensor: +0x1c4 == NPC handle, +0x1e8 == 1, +0x1e9 == 1,
  +0x1ea == 1, +0x1e4 in 0.9..1.1, window point +0x1d8 not the sentinel DAT_1431af620.

## 2. Shadow Walk wall chokes (choke_sw_wall_*)

- Shadow Walk player state = arkPlayerStateShadowWalk (vtable 0x141c7ed28; RTTI found by byte search). Right
  after the vtable sit the strings "Enables hiding to walls when shadowwalking." and
  `ark_shadow_walk_hiding_enabled` (0x141c7ee20). [CS]
- Enter (slot 10, 0x1408492f0): `if (DAT_143994648 != 0)` enable player sensor slot 13 (arkCpntPlayerSensorWall),
  else nothing; error "Cannot find any wall sensor." if missing. Exit (0x1408498a0) disables it. [CS]
- Update (code around 0x14084a0a6): only when DAT_143994648 != 0 and the wall sensor IsAvailable
  (0x1403d2d30 = enabled && +0x40 wall found): dot(movement input direction, wall sensor normal at
  sensor+0x68..0x70) >= threshold (+0xa4, from an angle) accumulates a timer +0xa0; when the timer >= decl
  +0x15c the state calls FUN_1408022e0(player, **0x14**) - a transition to player state 20. [CS]
- Wall sensor (decl ctor 0x1403d1990; shapes 0x1403d20c0): grid of boxes 3x3 (decl +0x9c/+0xa0) over a
  **2.0 m wide x 2.0 m tall** patch (decl +0x90/+0x94), depth ~0.05-1.0 m in front (decl +0x84/+0x88/+0x8c),
  plus edge strips (margin decl +0xa4 0.1). So it wants a large flat wall. [CS shapes, H interpretation]
- DAT_143994648 is (by adjacency and use) the value of cvar `ark_shadow_walk_hiding_enabled`; the repo's
  runtime cvar dump (documentation/.../generated/history/039-dishonored2-debug-cvars.md) lists it with
  **Value 0**. [H strong: the cvar init code is not disassembled, so the value address is inferred]
- Conclusion [H strong]: "wall" = Emily's shadow form hidden on a wall (player state 20), a feature disabled in
  retail by the cvar default 0. That explains 1469a (pushing into the lab wall never climbed or ran a wall
  scene). The choke_sw_wall_* scenes are presumably selected by assassination_shadow_walk.rulehandler when the
  attacker is in that wall-hiding state; not verifiable without the rule handler.
- NPC-side TIs `TargetIsShadowWalking` (0x1405cf3b0, AI target is the player in state 2) and
  `HasBackAgainstWall` (0x1405ce680: point decl+0x1a4 behind the NPC is off the navmesh) are AI-context
  conditions (combat reasoning), not player takedown conditions. [CS code, H role]

Fixture recipe (shadow-walk wall): lab-only, set `ark_shadow_walk_hiding_enabled 1` at runtime (no file
change); Emily with Shadow Walk; enter Shadow Walk, walk up to a flat wall >= 2x2 m and hold movement into it
(facing the wall) until the hide timer (decl +0x15c) elapses -> confirm player state 20; have an unaware NPC
pass or stand within choke reach in front of that wall; press the choke/attack. Read the state id and the
slot-13 sensor (+0x38 enabled, +0x40 found, +0x68 normal) to confirm each step. If the cvar cannot be set,
these scenes are unreachable in retail play.

## 3. Seated takedowns (choke_sit_back/left/right)

- `TIProvide_ProvideTargetIsSitting` (0x14082d200) and self `IsSitting` (0x1403b0960): NPC component
  DAT_1439e2a00 = **arkCpntNavigation** (ctor 0x1405f3420, RTTI `.?AVarkCpntNavigation@@`), then
  FUN_1405f8c70(nav, 1): its **arkNavigationReqMgr** at nav+0xab8 (list at +0xac0, count +0xac8, 28-byte
  entries) contains an entry whose first dword == **1**. [CS]
- Entry dword 0 = low dword of `request+0x14` (FUN_1405f45d0). All four direct add callers
  (FUN_1403ee660, FUN_1404014c0, FUN_1404ce480, FUN_140691550 via FUN_1405f42f0) add type 0. The only other
  add path is the command relay FUN_1405f78d0 (add/update/remove commands), fed by an entity-event listener
  (event 9) installed by FUN_1405f7c40 / FUN_1405fc480 on the entity stored at the req-mgr +0x130. [CS]
- So "sitting" is a navigation request of type 1 pushed from a linked entity, not an AI enum. [H] It is
  raised by the seated activity (chair smart object / seated Midnight scene) for as long as the NPC is
  actually seated; it is absent while the NPC stands up/turns (repo 1512a/b: office_guard_07 gave choke_back,
  not choke_sit, because he was not seated at the press).
- No front scene exists (choke_sit_back/left/right only); the side is TargetDirection (player position
  relative to the NPC's facing). Approach from behind or the sides. [CS scene list from repo table, H rule]
- The Midnight priority gate (<= 23) in the eligibility check may matter for scripted seated scenes. [H]

Fixture recipe (seated): use an NPC that the map authors to sit (seated activity, not a placed standing
NPC), wait until the sit loop is running (nav req list at nav+0xac0 contains a type-1 entry - readable at
runtime), creep up from directly behind (or 90 deg left/right) without alerting him, and press choke while he
is still seated. Do not teleport the NPC into the chair.

## 4. Knocked-down takedowns (choke_knockdown_*; also fatality_knockdown_fast_*, fatality_generic_knockdown)

- `IsOnGround` (0x14082cc80) = any of [CS]:
  - FUN_1405bf440: physics component DAT_1439b4630 (project-known "PhysicsSystem", mode at +0x238) mode is
    **0 or 2** (repo bookmarks: mode 0 = ragdoll after a ragdoll request);
  - FUN_1405bf480: AIPhysicsReaction (DAT_1439e2340, ctor 0x140590990, RTTI `.?AVarkCpntAIPhysicsReaction@@`)
    recovery pose +0x13c == **1** ("doggy style" get-up);
  - FUN_1405bf4c0: AIPhysicsReaction laying flags (+0x150 bit1, +0x152 bit3, or timer +0x138 > 0) or recovery
    pose +0x13c == **0** ("lying down" get-up). Default +0x13c = 3 (none).
- `IsLayingOnGround` (0x14082cc20) = +0x150 bit1 || +0x152 bit3 || +0x138 > 0. [CS]
- `TargetDirection` (0x14082cd40): when the NPC is on the ground, direction comes from the ragdoll bone frame
  (component DAT_1439e8010 + bone lookup) relative to the player - so back/front/left/right refers to how the
  body lies, not the NPC's pre-fall facing. [CS]
- Damage path: arkCpntAIPhysicsReaction::HandleDamaged (0x140591df0) -> FUN_140593bd0: if the damage decl
  field +0x58 (impulse/knockback kind, 1..6, jump table 0x1405948b0) != 0 and NPC decl +0x288 allows it, a
  directional big reaction is played (direction via FUN_1406dd170, entry via FUN_140591920, sets +0x150 bit2);
  otherwise PlayHitReact (0x140593860). [CS] Which damages carry +0x58 is data. Exported decls in the repo:
  windblast_npc, explosion_grenade and shove_damage are NPC_HITREACT_BIG; player_kick is SMALL. [CS data]
- Window length [H]: from the ragdoll start through the get-up animation, ending when the physics mode returns
  to animated and +0x13c returns to 3. No fixed timer was found.

Fixture recipe (knockdown): unaware or alerted guard on flat ground with 3-4 m of free space; Corvo casts
Windblast (windblast_npc, BIG) from 2-4 m so the guard ragdolls (alternatives: grenade/explosive bolt in
range, a fall of a couple of metres, Emily's Far Reach pull); close to <= 1.5 m while he is down or getting up
(sprint immediately) and press choke (choke_knockdown_*) or attack (fatality_knockdown_*). Approach side picks
the variant relative to the lying body. Runtime checks: target physics +0x238 in {0,2} or AIPhysicsReaction
+0x13c in {0,1}.

## 5. Recommended next static/data step
Export (read-only, existing Void tooling) the five rule handlers named in assassination_npc.decl
(assassination_{base,window,shadow_walk,slide,drop}.rulehandler) and the player decls
`sensors/windowassassination` defaults, `sensors/wall`, `sensors/interaction`, and the Shadow Walk state
decl. They hold the per-scene conditions (which TI each variant needs) and would settle npcMaxDistance's offset.


## 5. The rule handlers (exported 2026-10-19, read-only; confirmed static)

Exports `npc-rulehandlers-v1518` and `choke-rulehandlers-v1518` (research, gitignored) hold the conditions that pick each scene. The first matching rule wins, in file order.

**Player chokes** (`components/characters/player/base/rules/choke.rulehandler`), in order: sliding (`choke_slide_*`); target grappled by Far Reach (`choke_grapple_back`); target `IsOnGround` (`choke_knockdown_*`, the direction mapping rotated by one quarter); target `isBehindWindow` (`choke_window_*`); target `isLayingOnGround` (`choke_knockdown_front`); drop attack (`choke_drop_*`); target `isSitting` (`choke_sit_back/left/right`, no front scene); then unaware, vulnerable or always-chokable targets (`choke_front/back/left/right`).

**Shadow Walk chokes** (`shadow_walk_choke.rulehandler`): `player.isShadowWalking` gives `choke_sw_*`; `player.isShadowHiding` gives `choke_sw_wall_*`. Shadow hiding is the hide-on-wall mode gated by `ark_shadow_walk_hiding_enabled` (global 0x143994648 read by the Shadow Walk state before it enables the wall sensor, slot 13; confirmed static). The cvar is 0 in the build's cvar list (`work/external-references/fakuivan-cvars-1.77.9.0`), no game or user config sets it, and 1469a found no wall state when Emily pressed into a wall in Shadow Walk for 3 s. So `choke_sw_wall_*` cannot occur in retail play and is not applicable to the partner presentation.

**Lethal** (`assassination_base`): grappled targets within 5 m (`assassination_grapple_*`); target `IsOnGround` within 2 m and 0.5 m height (`fatality_knockdown_fast_*`); Bend Time or stopped time within 3 m (`assassination_front/back/...`); unaware within 3 m (slow when alone, else fast); near-death or vulnerable fatalities. `assassination_window`: grappled within 5 m or unaware within 0.45 m height gives `assassination_window_*` when the window sensor selects this handler. `assassination_shadow_walk`: `isShadowWalking` gives `assassination_sw_*`; `isShadowHiding` rules carry no scene. `assassination_slide`: sliding within 4 m.
