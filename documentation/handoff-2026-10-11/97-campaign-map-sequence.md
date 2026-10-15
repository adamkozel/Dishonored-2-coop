# Campaign map sequence (stage 6 plan)

Updated 2026-10-15. Confirmed static from the game's gameflow trees (`models/gameflow/*.arktree`) and each map's `.entities` scripts, read with `tools/diag/d2_resource_index.py`; cross-checked with disrev's `docs/levels.md` ([chapter 96](96-external-research-references.md)). The mission list and order follow the user's list.

## Missions and maps

The goal's nine campaign missions are 2–10. Memories of Training is separate training, launched from the menu with its own map; it gets one coverage take as well.

| # | Mission | Maps, in play order | How it ends (gameflow) |
|---|---|---|---|
| 1 | Memories of Training | `tutorial_p` | Not part of the campaign chain |
| 2 | A Long Day in Dunwall | `dunwall_escape_tower_p` ↔ `dunwall_escape_streets_p` | `Mission_DW`: result screen, then `void_01` |
| 3 | Edge of the World | `void_01_p` (A Strange Visit) → `boat_eotw_p` → `eotw_p_*chaos` | `Mission_EotW`: carriage scene, result screen, then `institute` ([chapter 89](89-mission-progression-discovery.md)) |
| 4 | The Good Doctor | `institute_p_*chaos` | `Mission_IS`: result screen, then `boat_madinventor` |
| 5 | The Clockwork Mansion | `boat_madinventor_p` → `madinv_streets_p_*chaos` ↔ `madinventor_p` | `Mission_MI`: result screen, then `void_02` |
| 6 | The Royal Conservatory | `void_02_p` (Another Strange Visit) → `boat_curator_p` → `curator_p_*chaos` | `Mission_CU`: result screen, then `boat_dust` |
| 7 | The Dust District | `boat_dust_p` → `dust_p_*chaos` → `dust_return_p` (`void_03_p` is reachable from these maps' scripts) | `Mission_DD`: result screen, then `bunker` |
| 8 | A Crack in the Slab | `bunker_p_*chaos` ↔ `bunker_backyard_p` | `Mission_BK`: result screen, then `boat_palace` |
| 9 | The Grand Palace | `boat_palace_p` → `palace_streets_p_*chaos` ↔ `palace_p_*chaos` | `Mission_PA`: result screen, then `boat_dunwall_return` |
| 10 | Death to the Empress | `boat_dunwall_return_p` → `dunwall_return_streets_p` ↔ `_interior_p` ↔ `_tower_p` | `Mission_DTTE` (last mission): result screen, then `end_p` |

`*chaos` maps come in `lowchaos` and `highchaos` variants. Each world picks its variant from its own chaos level, so the partner must load the variant the host loads.

## Boundary types to carry in co-op

1. **Mission end with a result screen (9):** a gameflow mission completes (`m_showResultScreen = true`) and the game loads `m_nextMissionMap`. Edge of the World completes it after a usable and a scene (the carriage), not through a `LevelTransition`.
2. **Hand-offs without a result screen (7):** the Outsider visits (`Mission_Void01`, `Mission_Void02`) and the Dreadful Wale missions (`Mission_*_Boat`, `Mission_DTTE_B`) name their next map without a result screen. Their map change is a `LevelTransition` in the map's scripts, such as `boat_eotw_p` → `eotw_p_lowchaos`/`highchaos`.
3. **Map changes inside a mission (about 9 pairs):** `arkKisculeNodeLevelTransition` nodes. Examples: `madinv_streets_p` → `madinventor_p` (`script_kiscule_change_level`), `palace_streets_p` ↔ `palace_p` (`ld_*kiscule_changelevel`), and the three Dunwall return maps. Several are two-way, so players can go back.
4. **Debug only:** `script_debug` and `ld_kiscule_debug` transitions behind the `Debug.NextMap` and `Debug.PreviousMap` events. Never a product path.

## Route tracing and trigger names

`tools/diag/d2_mission_route.py <base> <map regex>` walks each map's Kiscule scripts back from every gameflow task, objective, mission and `LevelTransition` to the trigger volumes, usables and events that start them, with spawn positions. It is read only, and its output is game-derived, so it stays local.

- **A Long Day in Dunwall, tower (confirmed static):**
  1. *Find a way out:* the trigger `ld_trigger_bedroom`.
  2. *Corvo's sword:* after Mayhew's goodbye scene.
  3. *Ring and vault:* the `Get_Sigil` and `VaultEnter_OnUsed` events.
  4. *Leave the tower:* `Start_Level_Transition` at the vault exit.
  5. *Ramsey:* spawned at `ld_spawner_ramsey_escape`, to be killed or knocked out (incapacitation).
- **Streets:** `ld_trigger_balcony`, the reach-streets triggers and `ld_trigger_around_meagan` (reach the boat), then the debrief with Meagan.
- **Trigger names:** the campaign's scripts listen to many trigger name families (`ld_trigger` 428 times, `script_trigger` 301, `mdnt_trigger` 20, `obj_trigger` and more). Since 1214bf the mirror sends every volume named *trigger* except the ones that describe a player's own position: audio, lighting and environment, hideout, no-swim, death (kill volumes), vista and water.
- **Objective sync check:** `missionend.json` now reports both worlds' gameflow node counts by state.

Co-op therefore needs two native paths, the gameflow mission completion and the script `LevelTransition`, each carried to the partner with the host's map variant and a both-ready barrier. Each player's own character state (inventory, upgrades, chaos) crosses the change in their own world.
