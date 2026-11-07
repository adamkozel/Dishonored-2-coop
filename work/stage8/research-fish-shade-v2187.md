# Research v2187: fish possession presence and Deadly Shade on a Corvo host

Static research only (no game, lab, VM, build or deploy). Labels: **confirmed static**, **confirmed runtime** (from
earlier recorded takes), **hypothesis**. Game decls were read read-only from `work/asset-game/base` with
`tools/diag/d2_resource_index.py`; code from `outputs/d2-coop-lab/src`; RE through the d2-ghidra MCP.

---

## A. Fish possession presence

### A1. How the game defines the possessable fish (confirmed static, game decls)

| Decl | Content |
|---|---|
| `models/characters/animals/fish/fish.def` (game1voidmod) | `class = "arkSwarmAgentEntity"`, model `models/characters/animals/fish/w_fsh_trout01/w_fsh_trout01.md6`; components `audio_default.cpntaudio`, `fish/possession.cpntswarmagentpossession`, `fish/anim.cpntanimfish` |
| `.../w_fsh_trout01/trout.def` | `inherit = fish.def`, `renderModelInfo.scale` 0.3 |
| `.../w_fsh_hagfish01/hagfish.def` (game3voidmod) | `inherit = fish.def`, model `w_fsh_hagfish01.md6` |
| `fishswarm_possess.def` / `fishswarm_possess_hagfish.def` | `arkFishSwarmEntity` (via `fishswarm_base.def`) with `swarm_possess*.cpntfishswarm` (`m_isAlreadyForPossession = true`) |
| `components/characters/animals/fish/swarm_base.cpntfishswarm` | `m_agentEntityDef = trout.def`, `m_scaleVariation` 25..35 (rat swarm: 80..120, so a percent; fish render at about 0.3) |
| `fish/possession.cpntswarmagentpossession` | inherits the bloodfly one; `PLAYER_MODE_POSSESS_FISH`, `ARK_PLAYER_POSTURE_POSSESS_FISH` |
| `w_fsh_trout01.md6` | skeleton `w_fsh_trout01.md6skl`, mesh `w_fsh_trout01.md6mesh`, animset `models/characters/animals/fish/trout.animset`, loco `fish/loco.ratlocomotion` |
| `trout.animset` | aliases `fish_idle`, `fish_swim`, `fish_swim_fast` (all `forceLoad`), `fish_turnleft_add`, `fish_turnright_add`, `fish_snap_a/b/c`, `fish_eating`, `fish_death`, `fish_attack`, `fish_flip` |
| `fish/loco.ratlocomotion` | move anims `fish_idle`, `fish_swim`, `fish_swim_fast` (the rat's: `rat_walk_slow`, `rat_walk`, `rat_run`) |
| `fish/anim_tree.animtree` | uses the swarm `rat` node with pause leaves (driven by the swarm agent): not usable on a plain cosmetic entity, as for the rat |

The fish is built exactly like the rat (`rat.def`: `arkSwarmAgentEntity`, `rat.md6`, `rat.animset`), so the rat visual's recipe carries over one to one.

### A2. RE: kind and target (d2-ghidra)

- `FUN_140823300` (only caller `FUN_140822a70`, the swarm possession start; confirmed static): resolves the target component's entity (`FUN_1408c3e30(target[2])`), reads the agent's swarm `*(*(agent+0x360)+0x18)` and classifies by type: 8 (bloodfly class), **0x10 fish**, **4 rat**, else 1/2. `FUN_140822a70` stores it at controller+0x260 = `internal+0x670`, matching `readNativeAnimalPossession`.
- `FUN_140822a70` (confirmed static): `target+0x10` is the possessed agent entity; it spawns at `agent+0x300` (position) / `agent+0x30c` (orientation), and saves the **player's own position** (`player+0x300..0x308`) at controller+0x330 (`internal+0x740`).
- **Hypothesis, risk for rat and fish alike:** that saved position suggests the player body is parked and restored, so the partner's streamed root may not follow the animal during a real possession. The rat presence was proven only with the `coop_lab_animal` override (labanimal-eotw-v2180a), never with a real possession (possessrat-eotw-v2178a missed). If the root does not follow, the fix is the same for both: while the bit is set, the partner stamps `outgoing.position/orientation` from `*(target+0x10)+0x300` (48 bytes, position + 3x3, the layout `swarmRatSetLiveTransform` already reads). Check it in the first real-possession take (log both in `swarm-rat-visual.json`).

### A3. How the rat visual was built (confirmed static)

- The four rat decls are **our text**, not copies: `outputs/d2-coop-lab/assets/swarm_rat_visual.def` (`idEntity`, model `models/characters/animals/rat/rat.md6`, 4 components: `components/d2coop/swarm_rat_visual.cpntanimevolved`, `midnightproxy_default`, `audio_default`, `referenceframe_default`), `swarm_rat_visual.cpntanimevolved` (`m_animTree = animtree/d2coop_swarm_rat_visual.animtree`), `swarm_rat_visual.animtree` (inputs `ratRate`, `ratMoving`; leaves `rat_idle01` and `rat_walk` resolved through the model's own animset; `blend2`). The three `swarm_rat_{idle,walk,run}.midnightscene` are also installed but **unused**: `startSwarmRatVisualScene` (`swarm_rat_visual_scene.h`) has no caller; the animleaf replaced them (candidate 296).
- `Build-D2SwarmRatVisualAssets.ps1` writes these files into a Void mod pack (`ResourceCollectionWriter`, `game1`, keys `generated/decls/<kind>/<name>.decl`) and installs it into `work/asset-game` only (Steam index hashes checked unchanged).
- Release: `tools/release/build_decl_recipes.py` takes **every** entry of the lab's `game1voidmod.index` whose source contains `/d2coop` or `d2coop_` and rebuilds it as lines copied from the player's own decls plus our text (format `D2RCP004`). `decl-recipes-v2/report.json` lists the rat's 7 decls among the 48 (e.g. `swarm_rat_visual.def` from `mesmerize_object.def`, 119 bytes our text). `d2coop_setup.exe packages` (parser `src/decl_recipe.h:39-40`: magic, count <= 4096) is generic; `Install-D2Coop.ps1:56` just calls it with `data\decl-recipes.bin`. `build_release_v1929.py:43-49` proves the generated packages from the clean Steam install and requires them byte-identical to the lab's files.

### A4. Minimal fish visual (plan)

1. **Assets** (new, `outputs/d2-coop-lab/assets/`):
   - `swarm_fish_visual.def` = the rat def with `model = "models/characters/animals/fish/w_fsh_trout01/w_fsh_trout01.md6"` and anim component `components/d2coop/swarm_fish_visual.cpntanimevolved` (keep the other 3 components, so the identity check's vtables are unchanged: actor +0x1c87d58, components +0x1b80408/+0x1c59ac8/+0x1c4e588/+0x1c573e8).
   - `swarm_fish_visual.cpntanimevolved`: `m_animTree = "animtree/d2coop_swarm_fish_visual.animtree"`.
   - `swarm_fish_visual.animtree`: same as the rat's with `aliasname "fish_idle"` and `"fish_swim"` (input names may stay `ratRate`/`ratMoving` so `shadowNamedInput` is reused). No midnight scenes.
   - Hagfish: use the trout for both (the bit has no variant; adding one needs a protocol bit: `validCoopPose` mask `0x7f1f01`, `coop_protocol.h:99`). Optional later.
2. **Lab builder:** `Build-D2SwarmFishVisualAssets.ps1`, a copy of the rat script with the 3 resources, package `assets/swarm-fish-visual-v<take>-v1`, its own stage/backup/research names.
3. **Code** (`swarm_rat_visual_fixture.h`):
   - Parametrise `swarmRatVisualIdentity` (definition name, anim component name: lines 39-41) and `swarmRatDriveGait` (animtree name line 128, reference speed `1.11f` line 142).
   - Add `possessedFish` (a second `SwarmRatVisualFixture`) serviced like `possessedRat` (lines 238-278) on `remote.flags&0x1000`, with `kSwarmFishVisualDefinition`, `sourceScale = 0.3f` (trout scale; inside the 0.1..4 bound of `swarmRatSetNativeScale`), swim reference speed about 1.5-2 m/s at that scale (**hypothesis**, tune by eye; swarm `m_maxHorizontalSpeed` 4).
   - Hiding the avatar already covers the fish (`movement_playback.h:234`). Add fish counters to the `possessedAnimal` status block (line 303).
   - `native_skeleton.h:56` names the rat def for the swarm skeleton scope only; no change needed.
4. **Release:** after installing in the lab, `python -I tools/release/build_decl_recipes.py work/asset-game/base work/release/decl-recipes-v3/decl-recipes.bin --report work/release/decl-recipes-v3/report.json` (51 decls, or 53 with B below; the `/d2coop` filter picks them up), and a new builder from `build_release_v1929.py` pointing `data/decl-recipes.bin` at `decl-recipes-v3` (update the docstring count). **No change** to `d2coop_setup.exe`, `decl_recipe.h` or `Install-D2Coop.ps1`.
5. **Take:** `coop_lab_animal fish` (labanimal route of v2180) on Edge of the World, then a twin with `coop_possess_animal off`; plus one real fish possession for A2.

### A5. Alternative: spawn the native fish def (not recommended)

- Spawning `trout.def` (`arkSwarmAgentEntity`) through `0x8a78f0` gives a fish with no swarm: `FUN_140823300` and other agent code dereference `*(agent+0x360)+0x18` without a null check (confirmed static), and its `cpntswarmagentpossession` would make the partner's fish a possession target for the host (breaks "partners excluded from each other's direct interactions"). `npcSpawnFromSpawner` (`npc_knockout.h:46`) does not apply: fish come from `arkFishSwarmEntity`, not `arkCpntSpawner`. Spawning `fishswarm_possess.def` would add a real swarm with physics/AI/faction to the host's world. The cosmetic decl is smaller and proven by the rat.

---

## B. Deadly Shade on a Corvo host

### B1. How her shade is presented today (confirmed static, code)

- Her world: `native_doppelganger_world.h:56-62` reads her committed clone; `deadlyshadow_entity.def` gives `kind 2`, `decoy_entity.def` kind 1. So her upgrade state **is hers** and reaches the host (kind 2 present: confirmed runtime, deadlyshade-eotw-v2074a). The hypothesis "the upgrade is read from the host's inventory" is refuted.
- Host, cosmetic: `doppelganger_visual.h` shows `models/powers/d2coop/doppelganger_visual.def` posed from her streamed joints for both kinds. Its FX follow her native scene only for `doppelganger_spawn`/`doppelganger_dispawn` (`native_doppelganger_fx_scene.h:11-12`); the deadly shadow plays `doppelganger_ds_spawn`/`ds_dispawn`/`ds_death` (from `deadlyshadow.cpntdeadlyshadow`), so kind 2 gets variant 0, no remote FX.
- Host, gameplay puppet: `shadeHostDeadlyParent` (`native_shade_host.h:130-151`) takes the clone definition from the **host player's** component `0x39ae070` +0x6d8 (`cloneOf(observedPlayer)`). Corvo has none (28 refusals, deadlyshade-eotw-v2076c, confirmed runtime), so line 150 falls back to `decoy_entity.def`, free-standing. That puppet is a full decoy: `decoy.cpntdecoy` (decoy spawn/despawn/death scenes and FX `decoy_spawn_01.fx`, `decoy_spawn_body_01.fx`), decoy AI. **That is why the Corvo host sees the decoy.**
- The assets are loaded in a Corvo game: `deadlyshadow_entity.def` sits in the shared `game1voidmod` container, and v2077 created it on a Corvo host (confirmed runtime). The second hypothesis is refuted too.

### B2. Why the native deadly shadow cannot be used (confirmed static + runtime)

`D2_DeadlyShadow_Start` (0x1404a4547 call) -> `FUN_1404a3f90`: if the handle at DeadlyShadow+0x60 resolves (`FUN_1403da910`), it calls `GetComponent(shadowEntity, 0x1439e8250, 2)` (0x1404a3ff5) and reads `[R14+0x40]` at **0x1404a404e** with no null check: the crash site of v1729c, v2077a and v2078a. The component type behind descriptor `0x1439e8250` (getter `0x14092cc90`) is not named (hypothesis: a crew/target-list component the native caster path provides).
**Risk found:** on an **Emily host**, `cloneOf(host player)` most likely returns `deadlyshadow_entity.def` (hypothesis, ch.125 line 88, never run) and the code then spawns it parented to the host player, the exact case that crashed in 2077/2078.

### B3. Smallest fix (plan)

1. Two new decls (our text, recipe-built like the others):
   - `components/d2coop/deadlyshade_puppet.cpntdecoy`: `inherit = "components/characters/player/base/powers/decoy/decoy.cpntdecoy"`, overriding `m_spawnMidnight`, `m_despawnMidnight`, `m_deathMidnight`, `m_deathFxMidnight` with the game's `doppelganger_ds_spawn`, `doppelganger_ds_dispawn`, `doppelganger_ds_death`, `doppelganger_ds_death_fx` (all exist; deadly_shade FX `deadly_spawn_01.fx`, `deadly_spawn_body_01.fx`, `deadly_despawn_01.fx`, `deadly_e_death_01.fx`; same material effect value 4 as the decoy).
   - `models/powers/d2coop/deadlyshade_puppet.def`: `inherit = "models/powers/doppelganger/decoy_entity.def"`, `item_override["2Vs8_0"]` -> the cpntdecoy above. Same component classes as the proven decoy puppet; no `arkCpntDeadlyShadow`, so 0x1404a404e is never reached. (`inherit` is the game's own decl mechanism, e.g. `trout.def`.)
2. `native_shade_host.h:150`: for kind 2 use `lookupEntityDefinition("models/powers/d2coop/deadlyshade_puppet.def")` (fall back to the decoy def if missing). Better, and still small: for kind 2 **always** use this puppet, skipping `cloneOf` (removes the untested Emily-host crash path). Kills stay forwarded (1214gx) and the host's guards fight it (v2079a).
3. Optional polish: map `doppelganger_ds_spawn`/`ds_dispawn` to new variants in `native_doppelganger_fx_scene.h` so her cosmetic visual also plays the deadly-shade FX; and the shade's sword (`inventory_deadlyshadow.cpntinventory`, `sword_idle` in ds_spawn) is not shown on the host puppet.
4. **Hypotheses for the take:** the host puppet's decoy component plays `m_spawnMidnight` on a direct `0x8ae9b0` spawn; the ds scenes hold two scenes named `Emily` and `Corvo` (the decoy's has one, `Scene_1`), so if the decoy plays the first scene by index it shows the Emily one (correct for an Emily partner); if it looks up `Scene_1`, derive a one-scene d2coop copy instead. Test with the v2079 route (`Test-PartnerShade` / deadlyshade-eotw) plus the `deadly_off` twin and a host film for visual acceptance.

Files: `outputs/d2-coop-lab/src/native_shade_host.h`, `src/native_doppelganger_fx_scene.h`, `src/swarm_rat_visual_fixture.h`, `src/native_player_possession_target.h`, `Build-D2SwarmRatVisualAssets.ps1`, `tools/release/build_decl_recipes.py`, `work/release/build_release_v1929.py`, `work/release/decl-recipes-v2/report.json`.
