# 110. Mission decisions in both worlds (#184)

Decisions carry across missions through two things: the gameflow tasks (each target has a Kill task and a NonLethal
task) and the **story flags**. Labels follow the handoff rules: confirmed static, confirmed runtime or hypothesis.

## Story flags (confirmed runtime)

- **What they are:** bool variables of the `storyflags/*.kiscule` script instances on the worldspawn
  (`tools/diag/d2_kiscule_vars.py <pid> storyflags`).
- **Where they come from:** each map loads `global.kiscule`, its own declaration and the earlier missions' declarations
  (Death to the Empress's tower: 12 declarations, 110 flags). They travel with the save, and the next mission reads
  them, for example to choose `dust_return_p` when Stilton was saved.
- **Kinds** (124 flags across the campaign, `story-flags-v1836a`):
  - **Alive flags** for every target and side character (`ramsey_is_alive`, `hypatia_is_alive`, `jindosh_is_alive`,
    `breanna_is_alive`, `stilton_is_alive`, `duke_is_alive`, `body_double_is_alive`, `delilah_is_alive`, Meagan,
    Sokolov, Mindy, Paolo, Byrne, Lucia, the printer, the shopkeepers).
  - **Non-lethal outcomes:** `ramsey_in_vault`, `hypatia_is_cured` and `hypatia_is_transfomed`, `jindosh_neutralized`,
    `breanna_is_powerless`, `stilton_is_crazy` and `stilton_is_incapacited`, `delilah_is_nl`.
  - **Robberies:** the black markets (`blackmarket_0N_robbed`, `bm_shops_robbed`) and `galvany_is_robbed`.
  - **Other:** `player_got_dukeplans`, `sokolov_carried`, `sokolov_with_meagan`, Meagan's lines.
- **Tool:** `tools/diag/d2_storyflags_diff.py` compares the two lab worlds' flags and writes only the differences.
  Bytes 8 and 9 of a bool are its default and value; the rest of the variable is padding.

## Who decides

| Decision | Who decides | Carried by |
|---|---|---|
| A target's or a character's death (alive flags) | whoever kills | NPC outcome sync (chapter 95): the host's own scripts then set the flag |
| A non-lethal outcome | whoever completes it | the body (carried-body sync), the place (trigger mirror), the device (shared usables, #187) and the story grants (chapter 97, 1214jc) |
| Chaos | the host | 1214ba |
| The Outsider's offer | each player | 1214bk (chapter 97) |
| Map variants (`dust_return_p` and others) | the host's world | map changes follow the host (chapter 97); they read the host's flags |
| Combination dials and the Jindosh lock riddle | each world | per world, decided (chapter 106); `EnigmaExpectedStage*` differ by design |

## Runtime

- **Every mission map loads with equal decision flags in both worlds** (story-flags-v1836a). Both lab games loaded
  the nine missions' maps with the native `campaign` command. The only differences are the Jindosh lock riddle's two
  `EnigmaExpectedStage*` values from Dust District on, which the game scrambles per world (non-bool, pointer-like bytes).
- **Her kill of Ramsey sets the host's flag** (route-dunwall-lead-v1835a). In a partner-led Dunwall (steps 1-15 of the
  v1674 route, held at the end), she kills and loots Ramsey, opens the vault and her locker. Both worlds then hold
  `ramsey_is_alive` 0, and the 22 flags show no difference (`storyflags-diff.json`, `storyflags-values.json`).

**Open (#197):** the non-lethal outcomes end to end, with the partner completing them. Each needs its mission's
non-lethal sequence (the Ramsey vault, the Hypatia cure, the Jindosh shock, the Breanna shrine, the Duke's double,
Stilton, Delilah). The campaign routes take the lethal branches. The final two-account campaign validation will take
non-lethal branches with the partner acting, and diff the story flags at each mission's end.

### Ramsey in the vault, partner-led (#197, in progress)

- **The sequence (confirmed runtime, gameflow names):** `Task_DW_NonLethal_KnockoutRamsey`, then `_GetRing`, then
  `_DropRamseyInVault`, then `_CloseTheSafeRoom`. The safe room is behind `ld_ring_locker_01_2_open`/`_lock`
  (-36.2, -171.1), and `ld_target_vaultroom_1` (-47.3, -177.4) is inside it.
- **Harness:**
  - [Route-Mission-v1882.ps1](../../work/natural-moveset/Route-Mission-v1882.ps1) adds `choke` (hold RB behind the NPC,
    optionally crouched and creeping in) and `drop` (put down a carried body), for a route led by the partner
    (`-LeadRole client`).
  - The route actions ([d2_route_actions.py](../../tools/diag/d2_route_actions.py)) take `"role": "client"`. `face` and
    the walking heading pick the nearest matching NPC (the map has a second, audience Ramsey).
- **Takes (1214jv, lab):**

  | Take | Approach | Result |
  |---|---|---|
  | [route-dunwall-nonlethal-v1881a](../../work/natural-moveset/route-dunwall-nonlethal-v1881a) | choke from 0.8 m behind | His confrontation started at the first approach; RB only blocked. |
  | [v1881b](../../work/natural-moveset/route-dunwall-nonlethal-v1881b) | block, then grab him off-balance | 235 presses in 4 minutes, no grab (#209). |
  | [v1881c](../../work/natural-moveset/route-dunwall-nonlethal-v1881c) | `chase_hold` behind him while he walks | He never walks: he waits at his dialogue spot (-44.1, -120.1). |
  | [v1882a](../../work/natural-moveset/route-dunwall-nonlethal-v1882a) | crouched, 2.5 m back, creeping in | The same confrontation, with his guards. |

- **Finding:**
  - Ramsey's encounter is a scripted fight, so his non-lethal outcome needs the combat choke: block him, then grab him
    while he is off-balance.
  - A partner cannot grab an off-balance host-owned NPC (#209): the parry is relayed, but her native grab never
    starts on the replica.
  - Until #209 is fixed, the host has to take Ramsey's non-lethal outcome.
- **The lab AI pause (1214jw, the user's suggestion):**
  - `coop_lab_ai_pause <spawner> <seconds>` holds up to 8 named NPCs' AI decision with the native pause request
    ([observer_camera.h](../../outputs/d2-coop-lab/src/observer_camera.h)); `off` releases them all. Status is in
    `observer-camera.json` `labAiPause`.
  - Route-Mission-v1883 adds a `command` step.
  - [v1883a](../../work/natural-moveset/route-dunwall-nonlethal-v1883a): with Ramsey and both guards held, they stood
    passive while the partner sneaked behind Ramsey. Her native choke still never started on his replica
    (`pairedKnockouts` 0). So the partner-made path stays blocked (#209, and perhaps his scripted state).
- **The host-made outcome reaches both worlds (confirmed runtime, [route-dunwall-nonlethal-v1883d](../../work/natural-moveset/route-dunwall-nonlethal-v1883d), 1214jw):**
  - The fixture holds the two escape guards' AI.
  - The host blocked Ramsey and grabbed him off-balance (`grab`), took his ring, opened the safe room, carried him in
    (X held 1200 ms; 400 ms never picked him up in [v1883b](../../work/natural-moveset/route-dunwall-nonlethal-v1883b)
    and [v1883c](../../work/natural-moveset/route-dunwall-nonlethal-v1883c)), dropped him and locked it.
  - Every gameflow task advanced identically in both worlds.
  - `ramsey_in_vault` is 1 in both worlds.

### Ramsey in the vault, partner-made, on 1214kd (#197, #209)

With the combat choke fixed (#209, below), the partner can take Ramsey's whole non-lethal outcome. The lab routes
`dunwall-nonlethal-partner-v1892/v1893/v1894` are partner-led (`-LeadRole client`) with the `grab_react` action.

| Take | Change | Result |
|---|---|---|
| [route-dunwall-nonlethal-v1892a](../../work/natural-moveset/route-dunwall-nonlethal-v1892a) | the escape guards' AI held from the start (as v1883d) | Ramsey was never off-balance during the fight: no knockout. |
| [v1893a](../../work/natural-moveset/route-dunwall-nonlethal-v1893a) | the guards held only after the knockout | She knocked him out (5 windows, 2 grabs, 158 s), took the ring, opened the safe room and carried him in. Arriving there completed the outcome in both worlds ("Place Mortimer Ramsey in the Safe Room" ticked, the objective updated, `Task_DW_LeaveTower` next). The route's drop and lock steps expected the host-made order, so the take failed. |
| [v1894a](../../work/natural-moveset/route-dunwall-nonlethal-v1894a) | ends at the safe room with the story-flag checks | The host accepted her hold (`host_paired_released`), but the choke ended before the knockout (`client_paired_ended`): fight variance. |
| [v1894b](../../work/natural-moveset/route-dunwall-nonlethal-v1894b) | | 28 of 30 windows belonged to another NPC, and her one grab caught a guard. `grab_react` now follows only its target's windows. |
| [v1894c](../../work/natural-moveset/route-dunwall-nonlethal-v1894c) | target-only windows | Ramsey was not off-balance during the fight. |
| v1894d, f, g (`Run-RamseyPartner-v1894.sh`) | | No knockout: fight variance. |
| [v1894e](../../work/natural-moveset/route-dunwall-nonlethal-v1894e) | | Knockout in 33 s, ring, carry into the safe room, and the objective updated in both worlds. But `ramsey_in_vault` was still 0 at the check: it comes with the drop and the lock, which this route skipped. |
| [v1896a](../../work/natural-moveset/route-dunwall-nonlethal-v1896a) | the drop (no task condition) and the lock before the flag checks | **Pass.** Knockout in 55 s (3 windows, 3 grabs), then the ring, the safe room opened, the carry, the drop and the lock. `ramsey_in_vault` is 1 in both worlds, the gameflow is equal (`Task_DW_LeaveTower` next), and restoration passed. |

- **What the fight decides:** whether Ramsey ends up off-balance near her after a timed parry, with the guards out of
  the way. Every mechanism the outcome needs worked in at least one take: the vulnerable bit, the forced hold, the
  knockout in both worlds, the shared ring and safe-room door, and the carried body completing the task.
- `Run-RamseyPartner-v1894.sh` repeats the take with new letters until one passes.
- **Confirmed runtime: the partner-made Ramsey outcome reaches both worlds** (#197, Ramsey done for both roles: host-made
  v1883d, partner-made v1896a). `Run-RamseyPartner-v1896.sh` is the repeat runner for this route.

### The other non-lethal branches (#197): static plan

Read with `tools/diag/d2_mission_route.py`, `d2_kiscule_graph.py` and `d2_resource_index.py` on the game copy (read-only;
the game-derived outputs stay local). All of this is confirmed static; the routes are still to be taken.

- **Stilton (A Crack in the Slab), the simplest:**
  - `storyflags/bunker.kiscule` declares only variables (`stilton_is_incapacited` defaults to false), and no map
    script node writes the flag.
  - `pa_ld_past_scripting` only reads it, to choose the ritual with or without him ("stilton dead or chocked").
  - Stilton in the past is `pa_ld_spawner_stilton` (-26.2, 3.6, 307.0, patrol `pa_ld_patrolroute_62`). His
    definition inherits `components/characters/npc/base/assassination_key_target.cpntassassination`.
  - Hypothesis: the native key-target component sets the flag when he is knocked out.
  - Route: the partner travels to the past and chokes him before the ritual. A partner's choke already asks the host
    for the native knockout (chapter 93). Then diff the flags and load *The Dust District* return variant.
- **Stilton, partner-made: done (confirmed runtime, [route-backyard-stilton-v1908a](../../work/natural-moveset/route-backyard-stilton-v1908a), 1214ke).**
  - The flag's only writer is in the backyard map (`bunker_backyard_p`, `backyard_ld_kiscule_node_template`):
    `stilton_is_incapacited` becomes true when `backyard_ld_spawner_stilton_1` dies or falls asleep (confirmed static).
  - That Stilton uses the story definition `aramis_stilton.def`, a civilian with AI. He is streamed and stood at the same
    place in both worlds.
  - Route-Mission-v1907 (the choke step's task condition is optional, since the backyard has no active gameflow): the
    partner choked him from behind. He was knocked out in both worlds (h10/10), and the flag is 1 in both.
  - Earlier takes, in the past of the main map: route-bunker-stilton-v1895a/v1897a/v1898a. The past ritual characters
    (`*_ritual.def`) are scripted actors that the NPC stream never binds, and each world runs its own copy, but they
    are not the target of any decision. #216 recorded them as a gap and was closed with this correction.
  - The next mission's variant follows the host's flag (map changes follow the host, chapter 97), and the flag is
    equal in both worlds.
- **The Duke's double, partner-made: the double now agrees between worlds (1214ke); his confrontation is not yet driven.**
  - Reconnaissance (route-palace-duke-v1899a/v1900a, confirmed runtime):
    - the Duke (`ld_spawner_duke`) is an AI NPC, unique and streamed;
    - his double (`luca_abele_double`) is a civilian, bound only within 15 m of a player, so each world ran his routine on
      its own: (242.4, 17.9) for the host and (341.9, 1.9) for the partner.
  - **1214ke:** story characters (definitions under `def/story/`) rank first in NPC relevance and use the combat radii
    even as civilians. In route-palace-duke-v1903a and v1904a the double stood at the same place in both worlds
    (confirmed runtime).
  - Confirmed static: the confrontation (`brief_double_confront`) plays when the player is in the trigger of the
    double's room (`ld_trigger_dd_quarters/throneroom/office/garden/delilahroom`, `Entity_Double_TriggerListened`), the
    Duke is not there (`Entity_Duke_TriggerListened`), and no alarm has rung (`DEV_NLETH_ALARM_RINGED` closes it).
  - Takes v1901a (the talk step teleported next to him; the identify task closed without the scene), v1902a (her double
    was elsewhere) and v1903a/v1904a (`enter_nearest`, the double alone in the quarters, six 30 s tries): no confrontation.
    The partner was undetected in the bedroom, so a remaining scene condition is not met (hypothesis: the standard
    confront location within the trigger).
- **The Duke's double (The Grand Palace):** the tasks are `Task_PA_NL_Identify_Talk_Double_title`,
  `Task_PA_NL_Neutralise_The_Real_Duke` and `Task_PA_NL_Set_up_the_scene` (`palace_p_lowchaos`).
  1. The double's confrontation scene (`brief_double_confront`) sends `EVENT_DEV_MissionEvent_DoubleDealtWithPlayer`.
  2. `duke_is_choked` is the incapacitation of `ld_spawner_duke` (280.1, -15.1, 20.0).
  3. The success dialogue (`dial_nleth_success_01`, or `_nodrop`) sends `EVENT_DEV_NlethEvent_NLCompleted`.
  The shape is the same as Ramsey's: talk, choke, carry. What starts the success dialogue is still to be read
  (`ld_kiscule_script`).
- **Jindosh (The Clockwork Mansion), a puzzle:**
  1. `Task_MI_NonLethal` starts on `non_lethal_known`: the electroshock notes, or using `env_lab_electrohelm` or
     `env_lab_electrochair`.
  2. `Task_MI_Find_Electro` needs `found_electro_therapy` (looking at `script_target_electrolookat_1`).
  3. `Task_MI_Electro_Tweak` needs `electroshock_tuned`: `script_nonlethal_kiscule` #363, behind the dial variable
     `dial_setting` and the power routing (`total_power`, `available_power`).
  4. `Task_MI_KO_Jindosh`: knock out `script_spawner_jindosh`.
  5. `Task_MI_Electro_Place`: the body in the chair spawns `script_spawner_jindosh_electroshock`.
  6. `Task_MI_Electro_Jindosh`: the handle `script_handle_electroshock` sends `nonlethal_complete` through
     `nleth_shocked_success`.
  7. `Task_MI_NonLethalEnd`.
- **Hypatia (The Good Doctor), five steps** (`institute_p_lowchaos`, `ld_kiscule_objectivs`):
  1. Take Vasco's journal: `ld_belt_note_is_vasco_journal_cur_1` in his safe, or his scroll case
     `ld_npc_belt_scroll_case_01`.
  2. Take the syringe, `ld_campaign_syringe` (26.8, 35.6, 28.0).
  3. Get blood from an infected body (`task_blood_sample_complete`).
  4. Make the preparation (`creat_the_antidote_finished`).
  5. Use the syringe on Hypatia (`hypatia_cured`).
  Pickups and usables are shared (chapters 95 and 97, #187), so the partner can do every step.
- **Ramsey:** the host-made outcome passes (above); the partner-made one waits for #209 (1214kc).
- **Breanna (The Royal Conservatory), a device chain** (`curator_p_lowchaos`, `ld_script_main`):
  1. `Task_CU_FindOutHow` (`NonlethalLearned`).
  2. `Task_CU_InvertLens`, gated by `Ashworth_Knocked_Out`.
  3. `Task_CU_movebreanna`.
  4. `Task_CU_OverloadPower` (`NonlethalCompleted`).
  Next, read the lens and power usables with `--event`.
- **Delilah (Death to the Empress), the longest** (`dunwall_return_tower_p`, `obj_tr_kiscule`):
  1. Use the Heart.
  2. Craft the corrupted runes (`EVENT_DEV_NLStep_Rune_Crafted`).
  3. `Task_DTTE_NL_Cor_Runes_Throne`: place them on the throne (`EVENT_DEV_NLStep_Rune_PlacedOnThrone`).
  4. Follow her painting.
  5. `Task_DTTE_NL_Knockout_Delilah`: knock her out (`EVENT_DEV_MissionStep_Delilah_IsUnconscious`).
  6. `Task_DTTE_NL_Delilah_Throne` (`EVENT_DEV_NLStep_Delilah_NLed`).
- **Order once the lab is free:** Stilton, the Duke's double, Ramsey partner-made (after 1214kc) and Hypatia, the
  simplest first. Jindosh, Breanna and Delilah are puzzle chains and follow.

### The partner's combat choke (#209)

**Why her grab never starts (d2-ghidra, confirmed static):**
- The player's choke rule handler (`components/characters/player/base/rules/choke.rulehandler`, chapter 100) ends with
  the unaware, vulnerable or always-chokable targets (`choke_front/back/left/right`). An alerted NPC is chokable only
  while it is vulnerable.
- `TargetIsVulnerable` (`TIProvide_ProvideTargetIsVulnerable`, provider `0x1409e9e70` → `FUN_14082d2f0`) and
  `IsVulnerable` (`0x1409e9ef0` → `FUN_1403b09d0`) both read bit 6 (`0x40`) of the target entity's flag byte `+0xd1`.
- The only writer of that bit is a component handler for entity message `0xc4` (`0x14071bc20`, registered through
  `FUN_140810810` at `0x14071ad05`). It flips the component's `+0x150` bit 0 and copies it into the entity's bit.
- The only sender of `0xc4` is `0x14084e530`, handler 9 of the table at `0x1423263c0`, which runs a midnight event track's
  events by type (`0x140850d70`). It sends the message to the entity playing the scene.
- A timed parry that the attacker loses starts `arkCpntAICombatAttack::StartParryLose`
  (`D2_AICombatAttack_StartParryLose`), which plays the slash type's parry-lose midnight on the NPC.
- Hypothesis: that midnight's event track sends `0xc4` at the start and end of the window, which makes the NPC
  vulnerable while it is off-balance.
- In co-op the parry is resolved in the host's world (chapter 80), so the midnight plays only there. The partner's
  replica follows the host's body stream and never plays it, so it is never vulnerable in her world, and her choke rule
  finds no scene.

**1214kc:**
- A host NPC Pose carries `reserved` bit 1 while the NPC's `+0xd1` bit 6 is set; bit 0 is already "down" (1214en).
  Pose validation accepts 0-3, and the unit tests cover 2, 3 and the refused 4.
- The client keeps her replica's bit 6 set while host poses carry it (within 400 ms) and clears it afterwards. Host
  poses get priority (every 50 ms) while the NPC is vulnerable, as they do while it is down.
- Her grab then plays her native combat choke on the replica. The client-sourced choke relay (chapter 93) holds the
  host's NPC and asks for the native knockout.
- Status: `vulnerableSent`, `vulnerableReceived` and `vulnerableApplies` per NPC channel in the NPC stream status.

**1214kd:** the host takes the forced AI lease (as Possession does, 1214ij) for her combat choke (a takedown of kind
entry, loop or knockout) on an NPC it saw vulnerable within 2.5 s. An alerted NPC refuses the ordinary lease, and a
non-lethal choke had no fallback (`npc_paired.h`, status `pairedResult` `host_vulnerable_choke_hold`).

**Takes** (lab, partner-led Dunwall, Ramsey; routes `dunwall-partner-grab-v1890/v1891`):

| Take | Candidate | Result |
|---|---|---|
| [route-dunwall-partnergrab-v1890a](../../work/natural-moveset/route-dunwall-partnergrab-v1890a) | 1214kc | Her grab started (2 grabs; 0 in 235 presses on 1214jv). The host sent the vulnerable bit for Ramsey and both guards (18, 58 and 24 poses), and she applied it (1, 2 and 1 times). The host refused the hold: `host_paired_hold_refused`. |
| [v1890b](../../work/natural-moveset/route-dunwall-partnergrab-v1890b) | 1214kd | Three off-balance windows reached her replica, but the fixed press rhythm (700 ms holds) landed in none: 0 grabs. |
| [v1891a](../../work/natural-moveset/route-dunwall-partnergrab-v1891a) | 1214kd | Pass. The `grab_react` action taps grab as soon as a new window reaches her replica, as a player reacts to the stagger: one window, two taps, one grab, in 23.5 s. Ramsey reads h10/10 in both worlds, `Task_DW_NonLethal_KnockoutRamsey` completed in both, and the gameflow is equal. |

- `tools/diag/d2_route_actions.py` `grab_react`: blocks in presses like `grab`, but on a new off-balance window
  (`vulnerableApplies` grows in its own npc-stream status) it releases, faces the target and taps grab for up to 1.6 s.
- **Confirmed runtime: #209 is fixed on 1214kd.** A partner can now knock out an alerted host-owned enemy with the
  combat choke after a parry.
