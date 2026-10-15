# Mission progression: discovery and trigger mirroring (candidates 1163–1171)

Updated 2026-10-14. Audit: [mission-progression-discovery-v1165](../../work/movement-integration/mission-progression-discovery-v1165.json). This is groundwork for two open items: shared mission progression (stage 5) and mission-to-mission transitions (stage 6).

## Why it matters

Each player runs their own world. Mission scripts react to what that world's own player does: crossing trigger volumes, completing objectives, reaching the mission end.

When only one player crosses a scripted trigger, the partner's world doesn't advance. Doors, NPCs, pickups, usables, checkpoints and deaths are already shared ([chapters 83–88](88-mission-shared-death-and-checkpoint.md)), but scripted progression and mission transitions are not yet.

## Findings

**Entity census (1162, read-only).** Edge of the World has 11,155 entities per role, with identical counts on both characters:
- 127 `arkTrigger` volumes (vtable `0x1c5eae8`);
- 205 area volumes;
- 371 usables;
- 472 pickups;
- 141 NPCs.

**No transition command (confirmed static).** The console has 113 commands, and none ends a mission or loads the next one; only `campaign`, `map` and `restartmap` exist. Mission flow is driven by mission-script ("Kiscule") nodes:
- `arkKisculeNodeLevelTransition`
- `arkKisculeNodeActionGameflowMission`
- `arkKisculeNodeActionGameflowObjective`
- `arkKisculeNodeTriggerNotification` (vtable `0x141c68488`)
- `arkKisculeNodeActionAutosave`

**Activation observer (1163, confirmed runtime).**
- 1163 installs a read-only detour on `idEntity::Activate` (`0x140875290`, entity and activator), recording a ring of 256 activations into `coop-status.json.activations.json`. The original call is always forwarded.
- A console `trigger` on a Dunwall light registered exactly one activation, so the observer works.
- But mission load and standing inside three script trigger volumes produced **zero** activations (1164).
- arkTrigger's `OnActivate` slot (vtable+0x128) is a no-op.
- Conclusion: trigger volumes reach the script graph through their touch and physics path (`arkHavokUserData::triggerEnterCount_t`) and `arkKisculeNodeTriggerNotification`.

## Trigger mirroring (candidate 1171, audit 1173)

[Audit 1173](../../work/movement-integration/mission-trigger-mirror-audit-v1173.json). Harness: [`Test-TriggerMirror-v1172.ps1`](../../work/natural-moveset/Test-TriggerMirror-v1172.ps1).

**The native trigger path (1170, confirmed runtime and static):**
- Every `arkTrigger` volume has exactly one `arkCpntTrigger` component (vtable `0x1c5bae8`).
- At spawn, `0x140779980` registers `arkCpntTrigger::OnTriggerEvent` with the physics trigger system.
- The enter/exit handler, `0x140779e00` `(component, entity, enter)`:
  - posts the trigger's script event (enter or exit) to the mission graph;
  - returns early for a once-trigger that has already fired;
  - marks the trigger as fired (`entity+0xd4 |= 4`);
  - sends the 0xd2/0xd3 messages.

**Mirroring (1171):**
- **Observe:** a detour on `0x140779e00` records every enter and exit; the original call always runs.
- **Send:** with `coop_trigger_mirror on`, a `script_trigger*` volume entered or left by the **local player** is sent to the partner on a reliable `D2TM` packet. Hideout, no-swim and audio volumes are excluded, because they describe a player's own position.
- **Replay:** the partner finds the same named trigger and its `arkCpntTrigger`, then calls the native handler for **its own player** during ordinary gameplay.
- **Once-triggers** dedupe natively, and mirrored calls are flagged so they never echo back.

**Runtime (1172, pass):**
- The host walked into three script trigger volumes, crossing others on the way, while the client stayed at the docks spawn about 80 m away.
- All 9 host-local script-trigger enter and exit events were replayed natively in the client's world for Emily's player, with none missing or rejected. Audio volumes were correctly not mirrored.
- The Wall of Light tutorial trigger raised the native tutorial modal (logic 12) on **both** peers.
- Visual (client): Emily stood on the docks while the native "Play Your Way #2" modal appeared, followed by the native "Objective updated" banner and the red "HOSTILE TERRITORY" indicator. Her mission script advanced although she never entered those volumes.

**Runtime (1174, pass, client-originated; [audit 1175](../../work/movement-integration/mission-trigger-mirror-audit-v1175.json)):** the client walked the same route while the host stayed at the docks. All 9 client-local script-trigger events replayed natively in the host's world for Corvo, and the tutorial modal appeared on both peers. The host's HUD showed the objective update and "HOSTILE TERRITORY" without the host moving. Mirroring works in both directions.

**Mission end (hypothesis).** Edge of the World ends at Addermire Station, after the mission objectives. The partner's world is driven by mirrored triggers, shared usables and shared doors. If the mission end is a trigger or usable, the partner's world should run its own native `LevelTransition`. A real end-of-mission take still has to show this.

**How Edge of the World ends (confirmed static, 2026-10-15, read with `tools/diag/d2_resource_index.py`):**
- **Mission tree:** `models/gameflow/gameflow_eotw.arktree` declares `Mission_EotW`:
  - `m_nextMissionMap = "campaign/institute/institute.mapinfo"`, `m_showResultScreen = true`.
  - The required objective `Obj_EotW_A` ("Get to Addermire") runs its tasks in sequence: get past the Wall of Light (always succeeds), get to Addermire Station (`Task_EotW_A_02`), then activate the carriage (`Task_EotW_A_04`).
  - The black market and Mindy objectives are optional side branches.
- **The end is a usable plus a scene, not a `LevelTransition`.** In `maps/campaign/edge_of_the_world/eotw_p_lowchaos.entities`:
  1. `script_kiscule_carriage` listens to the usable `script_kar_cable_car_inv_use` (and `_1`/`_2`), enters a cinematic and plays the `cablecar_enter_lever` scene.
  2. A leave-or-stay prompt follows. `Carriage.Leave` plays `carriage_movement_toinstitute`.
  3. In `script_kiscule_eotw_ui`, `Midnights.carriage_movement_toinstitute.OnEnded` fades to black over 2 s. Then `arkKisculeNodeActionGameflowTask` completes `Task_EotW_A_04`, and the gameflow mission completes: result screen, then the next mission map.
- **Debug only:** the map's three `arkKisculeNodeLevelTransition` nodes are all in `script_debug`, behind the `Debug.PreviousMap` and `Debug.NextMap` events. They target `boat_eotw_p` and `institute_p_lowchaos` or `institute_p_highchaos` by chaos.
- **Map list:** every mission opens on a Dreadful Wale map (`boat_<mission>_p`), and five missions span several maps ([chapter 96](96-external-research-references.md), disrev `docs/levels.md`). Stage 6 must therefore carry both in-mission and mission-to-mission map changes.

So in co-op, the partner reaches the end through the shared carriage usable or a mirrored gameflow completion. Which one, and how both worlds then load the next map together, is the next runtime test.

**Runtime (take mission-end-carriage-v1291a, candidate 1214aw, lab mode, follow off; [audit 1291](../../work/movement-integration/mission-end-audit-v1291.json)):**
- **Setup:** the host was placed in `script_trigger_addermirestation_inside_area` with the trigger mirror and the lab health hold on. It used the carriage from its east door.
- **Host (confirmed runtime):** the native end ran in full. Logic states, in order:
  1. 1 → 5: the carriage-entry scene.
  2. 9: the leave-or-stay prompt; A leaves.
  3. 5: the ride.
  4. 11: the native result screen, "Edge of the World — Mission Complete"; A continues.
  5. 0 → 15/5: the map transition.
  6. 5 → 1: the Good Doctor intro, then gameplay on `institute_p_lowchaos`, about 135 s after the use.
- **Partner:** Emily's world stayed in gameplay on `eotw_p_lowchaos` throughout. Neither the shared usable nor the mirrored triggers carry the mission end, so the partner needs its own native completion.
- **Transport:** the Steam connection stayed fresh with the peers on different maps.
- **Restore:** the lab's SLOT81 checkpoints were unchanged, and the lab and protected files were restored.
- **Side effect:** the host instance unlocked a Steam achievement ("Alternative Approach") on the shared lab account.

**Mission end mirror (candidate 1214ax, take mission-end-mirror-v1291b, [audit 1292](../../work/movement-integration/mission-end-mirror-audit-v1292.json)):**
- **Static (confirmed):** the gameflow set-state routine `0x1404e9bd0` takes the manager (game+0x1f7780), a node id and a state.
  - The manager's hash table is at +0x18 (mask at +0x28). A node keeps its id at +8 and its state at +0xc.
  - Mission requests (`0x14040f760`) allow 1→2 (active), 2→3 (complete), 2→4 and 2→5.
  - A mission that reaches 3 with its result screen enabled switches the game to logic 11 (`0x1404ea020` → `SwitchState` `0x1407885b0`).
- **Mechanism:**
  1. When a world enters its own result screen with its mission complete, it sends `D2ME` with its mapinfo.
  2. A partner on the same mapinfo, in ordinary gameplay, finds its **own** active mission node (state 2) and sets it to 3 through the native routine. Ids never cross processes, since they are localization string ids.
  3. Scenes, prompts and menus wait, because `SwitchState` can refuse from them.
  4. Lab command: `coop_mission_end_mirror on|off`. Always on in product mode.
- **Runtime (pass):**
  - The host sent once. Emily's world completed its own mission (1 of 18 mission nodes) and opened its own result screen in the same 400 ms sample as Corvo's.
  - Both loaded `institute_p_lowchaos` and reached gameplay 1.6 s apart, with the connection fresh. The lab was restored.
  - Visual (accepted): each player saw a native "Edge of the World — Mission 02 Complete" screen with their own portrait, then The Good Doctor.
- **Open:**
  - The partner must load the host's chaos variant.
  - A both-ready hold at the next mission start.
  - A client-originated end, and product mode across the VM.
  - The boat and in-mission `LevelTransition` changes ([chapter 97](97-campaign-map-sequence.md)).

**Next (superseded by the mirror above):** mirror gameflow state changes, so the partner's world completes its own mission: its own result screen, then the next map with its own character state. The game posts them as `arkMsgGameflowTaskChangeState`, `arkMsgGameflowObjectiveChangeState` and `arkMsgGameflowMissionChangeState` (confirmed static, string names). Shared objective progress (stage 5) uses the same path. The partner must load the host's chaos variant, behind a both-ready barrier.

## Next for this line

1. Walk a whole mission with the mirror on.
2. Review triggers whose scripts depend on the entering player's position.
3. Mission end and next-mission load: one world's `LevelTransition` should make the partner's world perform its own native transition, with its own character state, behind a both-ready barrier.
