# Stage 8 step 1: the activation range (static findings and the lab plan)

This is read-only research for [#234](https://github.com/adamkozel/Dishonored-2-coop/issues/234), done on 2026-10-29 with d2-ghidra, a disassembly of the lab's exe copy and `d2_resource_index.py`. Labels: **CS** confirmed static, **KNOWN** already in the docs or source, **H** hypothesis. The main session re-checked `FUN_1407e7560`: it pauses or resumes a component only when the decl's +0x64 auto-pause level is below 8.

**Short answer:** Void Engine has a per-entity LOD system measured from one local view (the camera). By default it does **not** pause NPC AI. So host authority over far guards looks viable on paper. The runtime checks below decide it.

## What the mod already knows

- **55 m / 75 m are design numbers, not engine numbers** (KNOWN; chapter 86, candidates 1166–1166b, audit 1169).
  - They are `kNpcRelevanceAdd`/`kNpcRelevanceDrop` in `src/npc_authority_stream.h`, with at most 24 bound and a rescan every 2 s.
  - Civilians use 15/20 m (1214ey).
  - Bandwidth and relevance chose these numbers. No chapter measured how far the engine itself keeps NPCs active.
- **Scheduler membership is readable** (KNOWN: `native_ai_state.h`, chapters 70 and 102). The native pause and resume (`1407e75b0`/`1407e7610`) move a component out of or into the AI-manager registry's active prefix; `schedulerActive` reads it.
- **Most AI was inactive in one lab take** (KNOWN: `work/natural-moveset/ai-registry-research-v1595a/registries-host.json`). On Dunwall, the decision registry had 30 live and 5 active; the reflex registry had 38 live and 5 active. The cause was never worked out.

## Engine findings

1. **The LOD manager runs every frame from one view** (CS).
   - `FUN_14064cff0` (arkLODManager) drives the per-entity job `FUN_14064c1a0`.
   - The reference is the top of the view stack (`*(game+0x1f52b8)`, last element, +0x10; `FUN_1407e7110`): position +0xa8..+0xb0, plus +0xc0..+0xc8 when it differs (the smaller distance wins). FOV is at +0x9c.
   - Distance is measured from the render-bounds centre, minus the half extent.
   - It does not loop over players: the partner's rig in the host's world is not a view.
2. **Levels and default distances** (CS; cvars registered at `0x1413875a0`):
   - `lod_dist_level0..3` = 3/6/12/20 m, `lod_dist_far` = **50 m**, `lod_forcevisibledistance` = 10 m, warmup 3 frames.
   - The levels are 0–4, 4_FAR (5), HIDDEN (6), HIDDEN_FAR (7) and MAX (8). A visible entity beyond 50 m gets 5; a not-rendered one beyond 10 m gets 6, and beyond 50 m gets 7.
   - Guards also inherit `loddriver_grd.cpntloddriver` 3/8/20/100, probably model LOD (H).
3. **Entity fields** (CS, `FUN_14087bb70`):
   - +0x298 is the computed level, +0x29c the clamp (lock requests through `arkRequestManager<arkLODLevel_t>`), and +0x294 the effective level, the lower of the two.
   - When the effective level changes, every started component gets vtable +0x68 `OnLODChanged(old, new)`.
4. **The mechanism is off for AI by default** (CS, re-checked).
   - The decision, civilian decision, reflex and attack components use the base handler `1407e7560`. It pauses at `decl->m_autoPauseLODLevel` (decl +0x64) and resumes below it.
   - Component start (`1407e7670`) starts a component paused when the entity's level is already at or above that level.
   - The base decl constructor `FUN_14000cc90` sets +0x64 = 8 (MAX), which means never.
   - No NPC decl or mission `.entities` file sets `m_autoPauseLODLevel`. Only doors do, and only to MAX. Swarms use `m_lodPauseLevel = LOD_LEVEL_4`.
   - A runtime read must confirm +0x64 = 8.
5. **Throttled updates** (CS; the owner is H):
   - `FUN_1405dba00` skips its update at levels 5 and 7 unless decl byte +200 is set.
   - `FUN_14054be90` (a component of about 0x2d40 bytes) changes behaviour when the level is above decl +0x164.
   - So far guards may keep thinking but move or animate at a lower rate.
6. **PVS dormancy, possibly left over from Doom 3** (H). `g_showActiveEntities` mentions "dormant entities (outside of pvs)", alongside `CHECKDORMANT` and `g_pvsLocationMemory`. No live link to the AI scheduler was found.
7. **The observer camera** probably moves the top view, and with it the LOD reference (H). Every take must record the view position.

## Open questions

1. Is decl +0x64 = 8 at runtime on every guard and civilian AI component?
2. Do guards at level 7 relative to the host's camera stay scheduler-active and patrol at full rate, or does the throttle make them drift or snap?
3. Is there a PVS-dormancy path that freezes thinking? (It could explain "active 5 of 30".)
4. What is the view's second position at +0xc0?

## Lab plan

1. **Probe:** a read-only runtime_probe mode, `--npc-activity`, built on the `--npc-authority` catalog and `readNativeAiComponent` (1214ew `--npc-ai-registries`). Every 0.5 s, for each living NPC, it records:
   - handle, name, position, health;
   - entity +0x294/+0x298/+0x29c;
   - per AI kind: the component +0x1c pause flags, `schedulerActive`, the live and active counts, and decl +0x64;
   - the view's +0xa8/+0xc0 positions and FOV;
   - the cvars `lod_dist_far` and `lod_forcevisibledistance`.
2. **Map:** Edge of the World (141 NPCs; the roughly 205 m route from take 1168, `Test-MissionNpcRelevance-v1168.ps1`). A new take name, and the lab restored afterwards.
3. **Cases:**
   - **A, baseline:** both players at the spawn, 30 s.
   - **B:** the partner far into the interior, the host at the spawn, 60 s. Compare the guards near the partner in the two worlds.
   - **C:** the host's camera at 10/30/60/120/200 m from a patrolling guard, first by moving the player, then with the observer camera.
   - **D:** B with NPC authority off.
4. **Decision:**
   - **Host runs far guards** if, in B and C, far guards keep pause refcount 0, stay scheduler-active, have +0x64 = 8, keep their 60 s patrol length within about 10 % of the near run, and perceive the partner. Then step 4.5 needs only relevance and bandwidth.
   - **Regional or closest-player ownership** is needed otherwise.
   - A cheaper fix to try first: the host locks those guards' LOD clamp (+0x29c) to 4 or less through the native request manager (H).
