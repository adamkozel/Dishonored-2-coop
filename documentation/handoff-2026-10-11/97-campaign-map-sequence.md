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

## Progress and map changes follow the host (1214bg–1214bh, [audit 1299](../../work/movement-integration/campaign-dunwall-audit-v1299.json))

- **Gameflow mirror (1214bg):**
  - Problem: story progress from an NPC death, a body search or a scene ran only in the host's world (Ramsey and the ring in take 1300d).
  - The host publishes every non-mission gameflow node's (id, state) twice a second (`D2GF`). The partner sets each differing node through the native set-state routine, whose change messages its own scripts hear.
  - Both worlds report the same node-id hash, and objectives match after every route step.
- **Level transition mirror (1214bh):**
  - The `LevelTransition` run function (`0x1407a1340`) switches logic to 15 and stores the map name and player start in the MapTransition state (manager+0x90, idStr +0x48/+0x70) through `0x1404f1d40`. Kiscule node run functions are called through their type info by `0x1407c0e30` (all confirmed static).
  - A world entering 15 that way sends the target (`D2LT`). The partner performs the same native calls with idStrs built by the game's own constructor, and both expect the transition, so the both-ready hold applies.
  - Checkpoint sync now lets `RestartFromMemoryCheckpoint` through at logic 15 as well as 11 (take 1300m hung on black without it).
- **A Long Day in Dunwall (take campaign-dunwall-v1300t, lab product mode, pass):** one continuous run from the coup and both character choices through Mayhew, Ramsey, the ring, the vault, the tower exit (both worlds to the streets), the boat and the Meagan debrief. It ends with both result screens and both worlds in the Outsider visit, with objectives equal after all 17 steps.
  - The route lives in `work/natural-moveset/routes/dunwall-full-v1300t.json`, run by `Route-Mission-v1300.ps1`.
  - **Two Steam accounts (take campaign-dunwall-vm-v1301c, pass):** the same route with the host on this PC as Corvo and the lab VM's second account as Emily, joined through the friends-only lobby in product mode. Objectives were equal after all 17 steps, the VM applied the tower exit transition, and both worlds ended in `void_01_p`. `Route-MissionVM-v1301.ps1` runs it, with `routes/dunwall-full-v1301c.json` (the Meagan talk step is optional because stepping onto the boat can start her scene first).
  - Known gap: items a script grants to the player who uses something (the safe-room loadout) reach only that player.

## Story grants in both worlds (1214bi–1214bk, [audit 1302](../../work/movement-integration/campaign-eotw-audit-v1302.json))

- **Problem (take route-eotw-v1302c, confirmed runtime, visual):** after the Outsider visit, Corvo had the Mark and mana and Emily had neither. Scripts give items, loadouts, mana and the Mark to the player of the world that runs them. Dunwall's safe-room loadout is the same gap.
- **Native path (confirmed static):** every queued Kiscule signal goes through the dispatcher `0x1407c0e30`. The signal holds the node id (+0xc), the input id (+0x10) and the node instance (+0x18), whose vtable names its class. From there:
  - The node's `arkKisculeInstance` (node+0x20) keeps the owning entity's handle at +0x60, and the Kiscule manager (`game+0x1f6e48`) maps entity handles to instances.
  - The node decl (node+0x28) lists the inputs, and each input's descriptor holds the handler.
  - Class vtables (RVA): inventory `0x1b7ddf8`, loadout `0x1b7df48`, mana `0x1c65ef8`, Outsider's Mark control `0x1cd4238`.
- **Script action mirror (`D2KA`, `coop_kiscule_mirror`, product step 20):**
  1. Each world counts its handler runs per (entity, node, input) for those classes and sends the count.
  2. The peer runs the same node's native handler in its own world until its count matches.
  3. A later local request the mirror already covered runs the dispatch with the handler stubbed, so the node's outputs still fire.
  4. Mirrored runs never echo.
- **Personal choices (1214bk):** the Outsider's offer (`ld_trigger_19`, `brief_outsider_powers`, `power_yes` → Give Mark) runs in both worlds through the trigger mirror, so each player accepts or refuses for themselves. The Mark control, power items (declaration type 9) and the `no_power` refusal item are therefore recorded but never mirrored. Each character keeps its own power: Corvo gets Blink and Emily gets Far Reach.
- **Runtime:**
  - Take 1302d: eight actions crossed, including the mission keys the carriage removes.
  - Take 1302e: both players have the Mark and mana from the void to Addermire, and both reach the Good Doctor with equal objectives.

**Deaths on campaign maps:** from Edge of the World on, the mission routes include a host fall (`die` step). A death after a level transition used to leave both players on GameOver; chapter 88 has the fixes (1214bl–1214bo).

Co-op therefore needs two native paths, the gameflow mission completion and the script `LevelTransition`, each carried to the partner with the host's map variant and a both-ready barrier. Each player's own character state (inventory, upgrades, chaos) crosses the change in their own world.
