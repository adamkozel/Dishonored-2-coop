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
| 7 | The Dust District | `boat_dust_p` → `dust_p_*chaos` | `Mission_DD`: Stilton's door, result screen, then `bunker` |
| 8 | A Crack in the Slab | `bunker_p_*chaos` (present and past layers) → `void_03_p` → `dust_p_*chaos` (Stilton mad or dead) or `dust_return_p` (Stilton saved) | `Mission_BK`: Sokolov at the skiff, result screen, then `boat_palace` |
| 9 | The Grand Palace | `boat_palace_p` → `palace_streets_p_*chaos` ↔ `palace_p_*chaos` | `Mission_PA`: Meagan at her boat, result screen, then `boat_dunwall_return` |
| 10 | Death to the Empress | `boat_dunwall_return_p` → `dunwall_return_streets_p` → `_tower_p` ↔ `_interior_p` (and the painted world inside `_tower_p`) | `Mission_DTTE` (last mission): the rescue scene, result screen, then `end_p` (credits) |

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
  - A node only the partner advanced is reset to 0 (1214bt, #87): it must be absent from 10 consecutive complete host packets (about 5 s), since the host sends only non-zero nodes. The native set-state routine stores any state, and its HUD notification covers only states 2 to 4 and flagged 5, so the reset is silent (confirmed static).
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
  - Class vtables (RVA): inventory `0x1b7ddf8`, loadout `0x1b7df48`, mana `0x1c65ef8`, Outsider's Mark control `0x1cd4238`, and the tutorial end `0x1b7b628` (1214bz).
- **Script action mirror (`D2KA`, `coop_kiscule_mirror`, product step 20):**
  1. Each world counts its handler runs per (entity, node, input) for those classes and sends the count.
  2. The peer runs the same node's native handler in its own world until its count matches.
  3. A later local request the mirror already covered runs the dispatch with the handler stubbed, so the node's outputs still fire.
  4. Mirrored runs never echo.
  5. 1214ca: only the host sends its counts, the same host authority as the gameflow mirror. A partner's own runs stay local and are only counted.
     - The reason (held take route-tut-v1310g, confirmed runtime): the partner's world looped its own stealth restart in Memories of Training.
     - Each restart's empty loadout was replayed in the host's world, emptying the host's hands.
- **Personal choices (1214bk):** the Outsider's offer (`ld_trigger_19`, `brief_outsider_powers`, `power_yes` → Give Mark) runs in both worlds through the trigger mirror, so each player accepts or refuses for themselves. The Mark control, power items (declaration type 9) and the `no_power` refusal item are therefore recorded but never mirrored. Each character keeps its own power: Corvo gets Blink and Emily gets Far Reach.
- **Runtime:**
  - Take 1302d: eight actions crossed, including the mission keys the carriage removes.
  - Take 1302e: both players have the Mark and mana from the void to Addermire, and both reach the Good Doctor with equal objectives.

**Deaths on campaign maps:** from Edge of the World on, the mission routes include a host fall (`die` step). A death after a level transition used to leave both players on GameOver; chapter 88 has the fixes (1214bl–1214bo).

## Named gameflow states (1214bp, [audit 1303](../../work/movement-integration/campaign-institute-audit-v1303.json))

- **Node id (confirmed runtime):** a gameflow node's id is the 32-bit FNV-1a hash of the lowercased task, objective or mission name. All 29 names of `gameflow_institute.arktree` match the live ids.
- **Tools:**
  - `tools/diag/d2_gameflow_dump.py <pid> --base work/asset-game/base` names every live node from the game's own gameflow trees. It is read-only.
  - 1214bp lists every non-inactive node as `[id, state]` in `coop-status.json.missionend.json` (`gameflowNodes`).
  - The route harnesses record the active node names per step (`active`, from the local cache `work/natural-moveset/gameflow-names.local.json`). The cache is game-derived and not committed.
- **Script gates:** `tools/diag/d2_kiscule_graph.py <base> <map> <entity>` prints one script entity's node graph, which shows the gate in front of an event.

## The Good Doctor (takes 1303a–1303ap, two accounts campaign-institute-vm-v1303a)

- **Required path (confirmed static and runtime):**
  1. The office note (it opens a reading screen, which B closes), then the master key.
  2. The recuperation door: a held unlock of 1.5 s.
  3. `ld_trigger_45`, then talking with Hypatia (`grim_alex.def`). This completes `Obj_IS_Investigate_Sokolov_whereabouts`.
  4. Vasco: a talk or the scroll case on his belt. This sets `Task_Find_the_crownkiller_complet`.
  5. A `ld_trigger_transfo_alone*` volume, then the transformation and the fight. This completes `Obj_IS_Eliminate_the_Crown_Killer`.
  6. The watchtower's whale-oil tank. Its removal completes `Task_IS_Turn_OFF_the_watchtower`, the only event that evaluates the escape gate, which then activates `Task_IS_Talk_to_Meagan`.
  7. Meagan sails to the lower dock. The talk there plays the departure scene, the leave prompt and the debrief, then the result screen loads `boat_madinventor_p`.
- **Route notes:**
  - The watchtower's body stays put; its top turns back and forth about the body's axis (−75.34, 42.92) and carries the tank about 1.45 m out (confirmed runtime, held take 1303am). The route stands 2.1 m out from the axis through the tank's live position, facing it, 20 degrees down. The deck carries the player along: one press opens the cover, the next carries the tank. Stands about 1 m from the axis are inside the turning top and fatal (takes 1303ai–1303al).
  - Guards on the walkway beside the tower attack a player at the tank, so the route clears them first and keeps the lab health hold on for that one stand.
  - Guards block plain sword strikes, so the fights use focused strikes.
  - Teleports beside wall-mounted objects at height can be fatal falls.
- **Mission end in a cinematic (1214bq–1214bs):** this mission completes inside its debrief cinematic. The native switch to Results (caller `0x4ea07c`) and back to the cinematic happen in one tick, on the cinematic's thread, so the mission end mirror never sampled logic 11 and the partner stayed behind (takes 1303ac/1303ae/1303af). The logic-switch trace now records that switch from any thread (1214br).
  - The debrief is a movie, and movie playback and the following load bypass command execution, where the engine-thread services run (take 1303ah: 49 s from Results to gameplay on the next map, and the observation expired unsent). Since 1214bs the sender runs from the transport pump, which the native screen callback keeps running during movies and loads.
  - The current mapinfo (`idCommonLocal+0x2b8`) is null during a mission started with the native `campaign` command, in both worlds (confirmed runtime, 1303am), so the sender could not name the mission. Since 1214bu it is derived from the loaded map's name when the declaration is absent (directory plus the name without `_p`, `_p_lowchaos` or `_p_highchaos`; an existing mapinfo for all 35 campaign maps, confirmed static). The same read names the host's mission for join-follow (`D2MF`).
- **Lab pass (take route-institute-v1303ap, 1214bu):** one continuous run from the cable car to Meagan. The partner followed the host's mission end, both worlds loaded `boat_madinventor_p` through the both-ready hold, and gameflow counts were equal. 1214bv drops a Results observation after the world changes, so a partner that applied the mirrored end does not resend it on the next map.
- **Two accounts (take campaign-institute-vm-v1303a, 1214bv, pass):** the same route with the host (Corvo) on this PC and the client (Emily) in the lab VM. The client applied the mission end, both worlds loaded `boat_madinventor_p` together, gameflow was equal, and the client dropped its stale Results observation. The host video was reviewed.

## The Clockwork Mansion (takes 1304a–1304s, two accounts campaign-madinventor-vm-v1304a, [audit 1304](../../work/movement-integration/campaign-madinventor-audit-v1304.json))

- **Required path (confirmed static and runtime):**
  1. The Dreadful Wale: talk to Meagan, then the skiff. Its `LevelTransition` to the streets is mirrored.
  2. The streets: the station trigger, then the cable car (`script_kar_cable_car_inv_use_5`: entry scene, the prompt "Go to Upper Aventa District", the ride). This completes `Task_MI_Carriage`. The mansion door (prompt, transition) needs no script gate.
  3. The mansion: Jindosh (the walking `kirin_jindosh.def`; `kirin_jindosh_noweapon` is the hidden electroshock stand-in). Then Sokolov: his first use plays his scene, after which "Anton Sokolov (unconscious)" is carried with a held use (900 ms or more). Out through the exterior door from inside (`p_start_2` faces into the house).
  4. The streets again: the return script rides the car down with Sokolov in it. Carry him to Meagan's skiff; the talk press there sets him down in the boat. The next talk plays the debrief with Sokolov and the prompt "Go to the Dreadful Wale". Both worlds then load `void_02`.
- **The ending needs Sokolov (confirmed static):** Meagan's talk reaches the leave prompt only when `sokolov_with_meagan` is set, which the debrief with Sokolov does. With Jindosh dead and no Sokolov, the talk does nothing (takes 1304e–1304h). Without the car having gone up, the return scene never ends (take 1304l).
- **Hazards:** the station's wall of light (its tank is in the lower office), guards on the car platform, and a clockwork soldier beside Sokolov that sword strikes don't hurt. Sokolov dropped off the dock drowns, which is a shared GameOver ("You have lost a critical ally"). The route keeps the lab health hold on for these stretches.
- **Passes:** lab take route-mi-v1304s and two-account take campaign-madinventor-vm-v1304a (host on this PC, client in the lab VM). Both worlds reached `void_02` with equal gameflow. In the VM take the host's both-ready hold reached its 90 s cap while the slower VM client loaded the Void (hypothesis); both still ended equal.
- **Tools:** `tools/diag/d2_kiscule_vars.py` reads live script and story-flag variables (bool value at +0x19, confirmed static). `work/natural-moveset/Focus-Use-v1304.ps1` and the route's `focus` option press only while the native use focus is on the target.

## The Royal Conservatory (takes 1305b–1305f, two accounts campaign-curator-vm-v1305c, [audit 1305](../../work/movement-integration/campaign-curator-audit-v1305.json))

- **Required path (confirmed static and runtime):**
  1. The Void (`void_02`): the brief and memories triggers, then the exit portal `script_usable_portal_exit`. Its use plays `player_leavingvoid` and the transition to the Dreadful Wale.
  2. The Dreadful Wale: Meagan's briefing chair (`mdnt_briefing_chair`, about 90 s), then the skiff.
  3. The Conservatory: an entrance trigger (`ld_trigger_main_entrance_door`), Ashworth's intro and her kill (`breanna_ashworth.def`; she sometimes fights back, so the route allows 200 presses). Then the mandatory `Obj_CU_DiscoverAshworthRole`: her office note (`ld_note_archived_journals`) and the audiograph (`ld_audiograph_01_1_2`). `Obj_CU_Escape` activates after it (confirmed static, `gameflow_curator.arktree`).
  4. The escape: a talk with Meagan at the start dock plays her scene, the leave prompt and the result screen. Both worlds load `boat_dust`, and each world also ends the mission itself (both mirrors read `local_already_ending`), which is benign.
- **A paused partner (1214bw–1214bx):** in the first two-account takes the VM client sat in the pause menu or a scene when the host left the boat, and the transition expired. The pause menu also stops the engine services. Received transitions and mission ends now stay pending while the partner is outside gameplay on the source map, time without a service call doesn't count toward their caps, and they apply when the partner resumes.
- **Passes:** lab take route-cu-v1305f and two-account take campaign-curator-vm-v1305c (host on this PC, client in the lab VM), both ending in `boat_dust` with equal gameflow.

## The Dust District (takes 1306a–1306d, two accounts campaign-dust-vm-v1306a, [audit 1306](../../work/movement-integration/campaign-dust-audit-v1306.json))

- **Required path (confirmed static and runtime):**
  1. The Dreadful Wale: the briefing trigger, Sokolov's talk, then the skiff.
  2. Meagan's briefing in the Dust District activates the gang tasks. These are routes to the solution, and none is required once the lock is dialled.
  3. The Jindosh lock at Stilton's door, dialled natively (below). Its door scene, the door, the prompt and the result screen follow. Both worlds load `bunker_p` (A Crack in the Slab).
- **The Jindosh lock (confirmed static and runtime):**
  - The lock entity `ld_enigma_v2_kiscule` owns a `jindosh_enigma.kiscule` decl instance, which owns one `jindosh_enigma_wheel.kiscule` instance per seat.
  - In the wheel decl, a wheel's family stage maps through `TranslatorFamily` to a family id; that id's position in the randomized `RandomFamily` list is `WheelFamilyIndex`. Heirlooms work the same way.
  - A seat is right when both indices are equal. The lock records 2^(index−1) per seat and opens at index sum 31.
  - Each use advances a wheel one stage. Seat *i* is dialled to index *i* with focus-guided presses (the route's `jindosh` step). The lists change per load, so the step reads them live with `tools/diag/d2_jindosh_lock.py`.
- **Tool notes:** int variables keep their value at +0x1c and lists at +0x28. The lock's nested decl table has 256 buckets.
- **Passes:** lab take route-dd-v1306d and two-account take campaign-dust-vm-v1306a, both ending in `bunker_p` with equal gameflow.

## A Crack in the Slab (takes 1307a–1307d, two accounts campaign-bunker-vm-v1307a, [audit 1307](../../work/movement-integration/campaign-bunker-audit-v1307.json))

- **Required path (confirmed static and runtime):**
  1. The present (z ≈ −300): the Outsider trigger (`pr_a_ld_trigger_outsider`) plays his scene and activates `Task_BK_Pick_TimeDevice`.
  2. The Timepiece (`pr_a_ld_timeshift_device_pickup_1`) is picked up through the native use focus. Its tutorial page closes with B, and LT shifts to the past (z ≈ +300).
  3. The past:
     - `pa_ld_trigger_61`;
     - then trigger 38 with the look-at on `pa_ld_target_43`, which starts the guests' ritual (`ritual_has_started`);
     - then the ritual hall floor (below).
  4. Back in the present, the exit door `pr_a_ld_door_tr_bunker_todust_01` and its prompt lead to `void_03`. This needs `update_last_obj`, which is set once the ritual is seen; without it the door leads straight to the Dust District.
  5. `void_03`: the door (`script_usable_door`: fall and first brief), the portal to the ritual memory, then the exit portal.
  6. Stilton mad or dead: `dust_p_*chaos` from the bunker. The sewer trigger `ld_trigger_karnaca_discovery` spawns the return Meagan and Sokolov. Talking to Sokolov at the skiff plays his scene, then the leave prompt and the result screen. Both worlds load `boat_palace`.
- **The ritual (confirmed static and runtime):**
  - It counts as seen (`player_has_seen_ritual`, and `Task_BK_WatcheRitual` completes) only while the player is inside `pa_ld_trigger_1` when the second ritual scene emits Delilah's spirit.
  - That trigger's volume is the ritual hall (world x −40.7…−21.8, y 16.3…37.2, z 300…313.5), 6 m north of its entity origin, which lies outside it.
  - The volume comes from the trigger's Havok shape `<map>/<trigger>.cvd._100_100_100.0`, a tagfile 5 bytes into the blob. Its first float4 pair (min, max, min.w = 0.5) is the AABB relative to the origin. New tool: `tools/diag/d2_trigger_box.py`.
  - The upper gallery and the origin did not register. The hall floor (−30, 24, 300) did, and the scene played there.
  - The route's `flag` step waits there for the story flag (26 s in take 1307d).
- **Timepiece notes:**
  - LT is refused where the other time is obstructed (the ritual hall). The route shifts back from the stand where it entered the past.
  - A pickup's effects can arrive after the use step's 1.5 s check, so pickups use `gone` (take 1307c failed there).
- **Passes:** lab take route-bk-v1307d and two-account take campaign-bunker-vm-v1307a, both ending in `boat_palace_p` with equal gameflow.

## The Grand Palace (takes 1308a–1308f, two accounts campaign-palace-vm-v1308a, [audit 1308](../../work/movement-integration/campaign-palace-audit-v1308.json))

- **Required path (confirmed static and runtime):**
  1. The Dreadful Wale (`boat_palace`): the meeting-room trigger, Meagan's briefing (a talk), then the skiff and its prompt. Both worlds load `palace_streets`.
  2. The streets: the palace door `ld_streets_door_tr_palace_ext_topalace` and its prompt load `palace_p`.
  3. The palace:
     - the arrival triggers `ld_trigger_player_reach_palace` and `ld_trigger_task_entering_palace`;
     - the real Duke (`luca_abele.def`, not the double), killed: `Obj_PA_Eliminate_The_Duke`;
     - the vault: its entry trigger, then the button `ld_button_01_5`. The button steps the locked door `ld_breakout_saferoom_01_1_2` (static), and `Task_PA_Open_The_Vault` completes after an autosave delay.
  4. The Delilah golem `ld_delilah_golem_01` bends time, spawns Jessamine's ghost and plays `dial_jessamine`. The Heart becomes `heart_delilah_palace` and `Obj_PA_Capture_Delilah_Spirit` completes. The script then kills the vault's NPCs, including a clockwork soldier that sword strikes do not hurt.
  5. `Task_PA_Meet_Meagan`:
     - `ld_trigger_dukesdock` enables `ld_trigger_extract_skiff` at Meagan's boat. With Stilton mad, the boat carries the one-armed Meagan.
     - A talk with her there plays the debrief, the ready-to-leave prompt and the result screen. Both worlds load `boat_dunwall_return`.
- **Route notes:**
  - The boat's skiff becomes unusable when Meagan reaches `script_waypoint_5`. It turns usable again on the player's next fit-state change (a landing), so the route drops the player beside it.
  - The vault button takes the focus only near pitch 30.
  - The Duke fights back. One take died in that fight: a single hit above current health kills even with the lab health hold. The attack step's `behind` option stands at the NPC's back, using rotation row 0 at entity +0x84 (confirmed runtime).
- **Passes:** lab take route-pa-v1308f and two-account take campaign-palace-vm-v1308a, both ending in `boat_dunwall_return_p` with equal gameflow.

## Death to the Empress (takes 1309a–1309e, two accounts campaign-empress-vm-v1309a, [audit 1309](../../work/movement-integration/campaign-empress-audit-v1309.json))

- **1214by (confirmed static and runtime):**
  - The boat's LevelTransition names `campaign\dunwall\return\streets\dunwall_return_streets_p.map` with backslashes. The level transition mirror required `campaign/`, so the host read `target_unread` and the partner stayed on the boat.
  - `map_path.h` now normalizes the name, and a unit test covers it.
- **Required path (confirmed static and runtime):**
  1. The Dreadful Wale: a talk with Meagan (her briefing and a dialogue choice), then the skiff, after a short drop beside it.
  2. The streets: the door to the Tower.
  3. The Tower courtyard: the door into the interior.
  4. The interior: the note `script_note_missionstep_dtlocked`. It completes Find_Delilah and Find_DT_TR and activates the elevator power task (three whale oil holders).
  5. Back up the interior's top door to the Tower's top level. The safe-room trigger `script_tr_trigger_saferoom` with the power off cancels the power task and activates Task_DTTE_ReachDelilah. This is the game's own no-power path; placing a whale oil tank in its holder through the cover's focus did not work.
  6. The throne room: its trigger registers only at the doorway slabs (`script_tr_trigger_reachthroneroom`, a compound of thin boxes), so the route walks through the south door.
  7. The Heart: raised (LT) at Delilah, it sends `Heart_Used`, plays her painting teleport and opens the portal (`Portal_IsActive`).
  8. Walking through `script_tr_trigger_portal` enters the painted world (`UDW_Reached`). Delilah is killed on her pedestal (`Delilah_IsDead`), and `script_udw_trigger_portal` leads back.
  9. The rescue: a talk with the stone Emily (as Corvo), from the north of the statue. It plays the rescue scene, then the result screen, and both worlds load `campaign/end/end_p` (the credits).
- **Route notes:**
  - The flag step reads `sf_temp_kiscule`'s mission-step bools at each stage.
  - Restoring from the credits failed natively once; Deploy-SourceClock relaunched the lab.
- **Passes:** lab take route-dtte-v1309e and two-account take campaign-empress-vm-v1309a, both ending in `end_p` (the credits).
- **Known limitation ([#94](https://github.com/adamkozel/Dishonored-2-coop/issues/94), confirmed static):**
  - In `end_p` the partner keeps `Obj_DTTE_EndGame_asCorvo` and `Task_DTTE_ReclaimThrone_Corvo` active. Gameflow counts: host [226,0,0,14,0,8,0], client [226,2,2,12,0,6,0].
  - Cause: the host completes them during the rescue scene, and the gameflow mirror runs only in gameplay, which `end_p` never returns to.
  - The credits play the same in both worlds.

## Memories of Training (takes 1310a–1310c, [audit 1310](../../work/movement-integration/campaign-training-audit-v1310.json))

- **Start:** the menu starts `tutorial_p` as Emily with no character choice; Corvo is the trainer NPC. The route harnesses take `-HostCharacter emily`. Take 1310a asked the lab `campaign` command for Corvo, and the host game exited as the map loaded (no Windows error event).
- **Required path (confirmed static and runtime, held take 1310b):**
  1. Mobility: `ld_trigger_beforecrouch` completes Task_Intro_FollowCorvo. `ld_trigger_roof_3` ends the course, plays Corvo's success scene and moves both players to the stealth yard. The client follows through its own scripts.
  2. Stealth, which restarts whenever the patrolling Corvo sees a player (`Player_Busted`, then `Stealth_Failed` and `Restart_Stealth`). Standing at the start in his view is enough; the partner's avatar in the host's world did not trigger it.
     - The partner's world runs the same lesson with its own Corvo. An idle partner at the start is seen there, so that world restarts in a loop and keeps resetting the partner (confirmed runtime, take 1310g). The route hides the partner, crouched, beside the gate for this lesson (`partner` step), as a human partner would hide.
     - Choke: crouched, behind the walking Corvo, RB held.
     - Key: X from 0.7 m behind as he walks into his north stand. Pressed on the standing Corvo, the theft is noticed and fails the task. The key never takes the use focus itself.
     - Gate: `ld_door_grid_metal_350_01_6` from its south-west side. This completes the stealth objective and moves both players to the crossbow yard.
  3. Crossbow: `ld_invisible_usable_1` beside the crossbow (its use sends `Crossbow_PickedUp`).
     - Then the crossbow (LT) breaks the three `Crossbow_Targets` bottles (`ld_bottle_01_2`, `ld_mulberry_mead_01_4`, `ld_rivera_fig_wine_01`), then the marked `ld_turnbuckle_01_small`, whose fall breaks `ld_door_interior_350cm_02` (`Crossbow_Door_Broken`).
     - `ld_trigger_endcrossbow` ends the section.
  4. Combat with Corvo:
     - RT attacks;
     - RB held blocks;
     - a block cycle (RB 1.2 s on, 0.32 s off) produces the timed parries;
     - a parry followed by a fresh RB held chokes him off balance;
     - a parry followed by RT performs the fatality.
  5. End: `Combat_Fatality_Success` plays Corvo's success scene, then `Start_Level_Transition` and `arkKisculeNodeActionTutorialEnd`, which returns the world to the main menu.
- **1214bz (confirmed static):** only the world that played the fatality ran the tutorial end, so in take 1310b the host went to the menu and the partner stayed in the combat room. The Kiscule action mirror now carries this node like a story grant: the vtable is `0x141b7b628`, confirmed by its RTTI type descriptor `.?AVarkKisculeNodeActionTutorialEnd@@`.
- **1214ca (confirmed runtime cause):** each of those restarts ran the empty-loadout node, and the Kiscule action mirror replayed it in the host's world. The host's hands were emptied after the crossbow pickup (take 1310f broke one bottle of three). The mirror is now host-authoritative.
- **Route notes:**
  - The harness's `action` step runs the timed host actions in `tools/diag/d2_route_actions.py`.
  - `setviewpos` ignores its pitch argument; pitch comes from `Set-ObserverPitch.ps1`.
  - The tutorial grants no powers.

Co-op therefore needs two native paths, the gameflow mission completion and the script `LevelTransition`, each carried to the partner with the host's map variant and a both-ready barrier. Each player's own character state (inventory, upgrades, chaos) crosses the change in their own world.
