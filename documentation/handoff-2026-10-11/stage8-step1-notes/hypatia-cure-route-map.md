# Hypatia's cure, partner-led: route map (#220, #237)

Research for a lab route where the partner does the whole non-lethal cure of Hypatia in
`campaign/institute/institute_p_lowchaos` under host story, then both worlds are checked. Labels: confirmed static
(map script or game decls), confirmed runtime (a take file), hypothesis.

**Sources.**
- The map script `maps/campaign/institute/institute_p_lowchaos.entities` was extracted from `work/asset-game/base`
  (`d2_resource_index.py` functions) into the session scratchpad. It was traced with `tools/diag/d2_kiscule_graph.py`.
  The copy is game-derived and stays local.
- No take has run the cure: none of the institute takes in `work/natural-moveset/` contains `ld_campaign_syringe`,
  `blood_sample`, `antidote` or `hypatia_cured` (confirmed runtime, by absence).
- Every existing institute route is lethal: `institute-partner-v1677.json`, `campaign-institute-vm-v1926.json`.

## 1. Steps, entities and coordinates

Positions are `spawnPosition` values from the map (confirmed static) unless marked.

| # | Step | Entity / what fires | Position (x, y, z) |
|---|---|---|---|
| 0 | Reach the lab level, talk to Hypatia (as the existing routes do) | `ld_trigger_45`; Hypatia = spawner `ld_grimalex` | (-48.2, 30.5, 30.3); (-49.05, 33.72, 27.0) |
| 1a | Vasco's scroll case (his special loot) | `ld_npc_belt_scroll_case_01` → activates `Task_IS_NLethal_Take_Vasco_journal_in_his_safe` (#5981) | (-55.1, 21.4, 27.6); Vasco `ld_vasco` (-58.4, 18.6, 27.0) |
| 1b | Journal in Vasco's safe | `ld_belt_note_is_vasco_journal_cur_1` pickup → completes the journal task (#5548) | (36.93, 44.28, 28.11); safe `ld_safe_vasco_1` (36.09, 44.51, 28.68) |
| 1c | Lab key, in the same safe | `ld_key_01_1` (vascolabkey) | (36.88, 44.24, 28.15) |
| 1d | Vasco's lab door, locked, needs vascolabkey | `ld_door_glass_350cm_01_1_30` | (29.5, 39.63, 27.03) |
| 2 | Take the syringe (empty: `syringeneutral.inventoryitem`) | `ld_campaign_syringe` (arkPickup) → `Task_IS_NLethal_Take_the_syringe` (#5546), master var `have_the_syring` | (26.84, 35.59, 28.0) |
| 3 | Blood from an infected body: use the corpse box (enabled only once `have_the_syring` is set) | `ld_usable_dead_body_2` (nearest; `ld_usable_dead_body` and `_1` also work) → corpse cinematic `playeroneliner_nlethal_cure_corpse_01` → `task_blood_sample_complete` → `Task_IS_NLethal_Get_blood_from_an_infected_body` (#5636) | (16.69, 48.47, 27.23); others (-13.71, 53.31, 34.06), (-28.75, 61.42, 34.55) |
| 4a | Preparation: the retort (needs `have_the_infected_blood`) → scene `player_nleth_curestart` | `ld_lab_retort_03_1` (Usable_baseserum) | (27.41, 36.0, 28.26) |
| 4b | The Bunsen burner valve (needs the serum in the glass and the blood) → `player_nleth_cureburner` → `cook_the_serum_finished` | `ld_valve` | (27.42, 35.99, 27.99) |
| 4c | The goblet (needs `cook_the_serum_finished`) → `player_nleth_cureready` → inventory: remove syringeneutral, add `grimalex/syringe.inventoryitem` → `creat_the_antidote_finished` → `Task_IS_NLethal_Make_the_preparation` (#5635) + autosave | `ld_lab_gobelet_01_1` (Usable_cure) | (27.08, 35.69, 28.0) |
| 5 | Cure Hypatia: a **choke-type** player state, not a use (below) → GrimAlex "Cure Started"/"Cure Finished" → `hypatia_cured` → `Task_IS_NLethal_Use_the_seringe_on _Dr_Hypatia` (#5642) | `ld_grimalex` | near (-49.0, 33.7) when routine; the cure-scene target is hypothesis |

**Step 5 is a choke (confirmed static).**
- `components/characters/player/base/states/cure_choke.playerstatecurechoke` has
  `m_cureEquipment = models/characters/inventory/items/grimalex/syringe.inventoryitem`.
- It plays `player_getserum` (front) or `player_getserum_drop` (back).
- The executable has RTTI for `arkPlayerStateCureChoke`, `arkDeclPlayerStateCureChoke` and `arkCpntAIGrimAlexSkills`.
- The NPC side is `aigrimalexskills_grimalex.cpntaigrimalexskills` `m_cureSceneDecl` =
  `grimalex/combat/transfo_cured_pass_out.midnightscene`.
- Hypothesis: the cure runs through the choke input while the cure syringe is carried, probably on Grim Alexandra
  (after `ld_trigger_transfo_alone_2`), from behind and unaware.

## 2. Route draft (lab harness `Route-Mission-v2012 -LeadRole client`)

Only the lab harness can make the partner the actor. `Route-MissionVM-v2014` sends use, trigger, talk and choke
steps to the host only (`C host`, confirmed static). A two-account take needs a role or lead option there first.
F = found in evidence (map or existing route), G = guessed.

```
{"do":"wait","ms":8000,"settle":5000}                                   F (all routes)
... v1677 steps 1-12 (door note, key, glass door, ld_trigger_45)        F (institute-partner-v1677.json)
{"do":"talk","name":"hypat|alexandr|grim","near":[-49.0,33.7],"settle":20000}            F
{"do":"use","name":"ld_npc_belt_scroll_case_01","aim":true,"offsets":[[0.6,0],[-0.6,0],[0,0.6],[0,-0.6]],"settle":20000}  F
{"do":"command","role":"client","text":"coop_grant_named models/interactive/keys/institute/vascolabkey.inventoryitem"}      G: stands in for the safe (no safe/dial step exists; dials are per world, area 4.3); optional journal pickup: {"do":"use","name":"ld_belt_note_is_vasco_journal_cur_1",...} needs the safe open
{"do":"use","name":"ld_door_glass_350cm_01_1_30","hold":1500,"offsets":[[1.3,-1.5],[-1.3,-1.5]],"settle":5000}  name/pos F, offsets G (copied from 01_2_14)
{"do":"use","name":"ld_campaign_syringe","aim":true,"gone":true,"offsets":[[0.6,0],[-0.6,0],[0,0.6],[0,-0.6]],"settle":4000}  F (pos), params G
{"do":"use","name":"ld_usable_dead_body_2","aim":true,"settle":20000}   F (pos), settle G (corpse cinematic)
{"do":"use","name":"ld_lab_retort_03_1","aim":true,"settle":15000}      F (pos), settle G
{"do":"use","name":"ld_valve","aim":true,"settle":5000}                 F (pos)
{"do":"wait","ms":30000,"settle":3000}                                  G (cureburner scene length unknown)
{"do":"use","name":"ld_lab_gobelet_01_1","aim":true,"settle":15000}     F (pos), settle G
{"do":"trigger","name":"ld_trigger_transfo_alone_2","settle":10000}     F (v1677; transformation)
{"do":"wait","ms":60000,"settle":10000}                                 F (v1677)
{"do":"choke","name":"hypat|alexandr|grim","near":[-49.0,33.7],"crouch":true,"dist":1.0,"walkMs":800,"ms":6000,"max":3,"settle":15000}  G (fields from bunker-stilton-partner-v1897)
{"do":"storyflags","expectEqual":true}  {"do":"diff","tag":"cured"}     F (harness vocabulary)
... v1677 exit steps from "clear" near (-70,41) to expect boat_madinventor                 F
```

- Retort, valve and goblet stand within 0.4 m of each other, so `use` lookups can aim at the wrong one (G).
- Use `aim` with tight offsets and `z` near 28.3.

## 3. Kiscule classes per step, under host story

Policy from `outputs/d2-coop-lab/src/kiscule_policy.h` (confirmed static): **W** = World (the partner stubs it and
requests it; the host runs it and sends it back), **B** = runs in both worlds (only in the world whose flow reaches
it). Chains (confirmed static, map trace):

| Step | Partner-only (B) | Reaches the host as a request (W) |
|---|---|---|
| 1 journal / key | PickupNotification, Gate, NamedVariableBool `vasco_journal_reading`, ActionPickupNote, ActionPlayerInventory (key) | ActionGameflowTask #5548/#5981, ActionGameflowMarkerControl (journal markers), ActionDestroy `ld_key_01_1` |
| 2 syringe | PickupNotification ×2, NamedVariableBool `have_the_syring` (which enables the corpse use only in her world) | ActionGameflowTask #5546 |
| 3 blood | UseNotification, ActionTestBool, Cinematic, ActionMidnightControl (corpse scene), SendCustomEvent/GetCustomEvent, NamedVariableBool `have_the_infected_blood` | ActionDestroy (the three dead-body boxes), ActionUsable (dead-body usability), ActionGameflowTask #5636 |
| 4 preparation | UseNotification ×3, Cinematic, ActionMidnightControl ×3, ActionTeleport (`ld_syringe_mdnt_*`), NamedVariableBool `cook_the_serum_finished`, ActionPlayerInventory (syringe swap: the `inventory` class, B, confirmed runtime in `campaign-institute-vm-hoststory-v2031a/end-vm.kiscule.json`), ActionChaosDeedControl | ActionGameflowTask #5635, ActionAutosave #4856 (`ld_kiscule_autosave`) |
| 5 cure | **the native cure (GrimAlex component + CureChoke state)**, GrimAlexNotification ×n, NamedVariableBool `hypatia_cured`, SendCustomEvent `start_dial_hypcured`/`hypatia_cured`, ActionMidnightControl (`dial_nleth_hypcured`), ActionHUDPortraitControl | ActionNPCRelation #1184850189 (Cure Started: neutral), ActionPatrol #1137512508 (Cure Finished: pause), ActionGameflowMarkerControl #1728402907, ActionGameflowTask #5642, StoryFlagBool `hypatia_is_alive` (#5740) and `hypatia_is_cured` (#5821), ActionAutosave |

Class names were confirmed runtime as dispatched in institute (`v2031a` end-*.kiscule.json): ActionGameflowTask,
MarkerControl, Patrol, NPCRelation, Destroy, Usable, `inventory`.

**What follows (hypothesis, to be tested by the take):**
- **The task and story-flag decisions should reach the host.** Every gameflow task and both story flags
  (`hypatia_is_alive`, `hypatia_is_cured`) are World nodes requested from the partner's flow. This is better than
  #220's "does not reach the host".
- **These stay partner-only:**
  - the native cure itself (the host's Hypatia keeps her uncured GrimAlex state and can transform again);
  - every master-local NamedVariableBool;
  - the cure syringe in the partner's inventory.
- **The debrief is at risk.** The host's `mdnt_out_setupmidnight_meagan` reads `ld_kiscule_master/hypatia_cured`
  (a local variable, never set on the host) as `Cond hypatiaiscured` of `debrief_gooddoctor`. The host's debrief
  would then take the uncured branch (confirmed static: the reader; hypothesis: the effect).
- **The chain may stop at ActionPatrol.**
  - "Cure Finished" goes only to ActionPatrol #1137512508 (W), and its `Pause` output alone continues the chain
    (`start_dial_hypcured` → `hypatia_cured`).
  - If the partner's stub does not signal `Pause`, the chain depends on the host's run coming back (in flow within
    750 ms, or as a replay).
  - Watch `in_flow` and `host_story_blocked` for node 1137512508 in end-vm.kiscule.json.
- **The NPC may refuse the cure.** Hypatia is host-owned: the partner's Hypatia is a replica following the host's
  body stream with AI paused.
  - Her CureChoke on the replica may be refused like the old combat choke (#209).
  - The client choke relay (chapter 93) would ask the host for a knockout, not a cure.
  - The host's poses may also overwrite the cure scene `transfo_cured_pass_out`.
  - This is the likeliest failure; record the npc-stream and pairedResult status.
- **The corpse use and the preparation run only in her world.** They depend on her local variables, so the host
  cannot replay them: if the partner disconnects mid-chain, the host has to redo them.

## 4. Native "Cure Finished" (Q3)

- **A script node observes it (confirmed static).** `arkKisculeNodeGrimAlexNotification` has the outputs
  Transformation Started/Finished and Cure Started/Finished. Its `m_listenedEntity` is wired from the `ld_grimalex`
  SpawnerNotification. It is used in `ld_kiscule_master`, `ld_kiscule_objectivs`, `mdnt_lab_brief_vasco`,
  `mdnt_lab_mdnt_hyp_transfo_throwplayer` and `mdnt_lab_barks_hypatia`.
- **It is not in `kiscule_policy.h`, so it runs in both worlds**, but it fires only where the GrimAlex component
  raises the cure.
- **What triggers it (confirmed static, from the decls):** the player's CureChoke state carrying the cure syringe.
- **Not decompiled:** the native body. The d2-ghidra string search finds only the RTTI names above; the port names
  come from the map's node data. A follow-up could decompile `arkCpntAIGrimAlexSkills`' cure handler and
  `arkPlayerStateCureChoke` to see which target checks would refuse a replica.

## 5. Checks the take should record

- **Both games' kiscule status:**
  - `blocked`, `asked` and `in_flow` on the partner for tasks #5546, #5636, #5635 and #5642, Patrol 1137512508, and
    StoryFlagBool #5740 and #5821;
  - `applied_partner` and `sent_back` on the host for the same nodes.
- **Story flags** at the end: `hypatia_is_cured` and `hypatia_is_alive` must be equal and true in both worlds.
- **The gameflow task states** of the five `Task_IS_NLethal_*` tasks in both worlds.
- **Hypatia's state** in both worlds after the cure: health, whether she transforms, the NPC-stream channel.
- **The debrief variant** on both games at the mission end (`event_debrief_gooddoctor_play`).
