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

**1214kc (built after the campaign run):**
- A host NPC Pose carries `reserved` bit 1 while the NPC's `+0xd1` bit 6 is set; bit 0 is already "down" (1214en).
  Pose validation accepts 0-3, and the unit tests cover 2, 3 and the refused 4.
- The client keeps her replica's bit 6 set while host poses carry it (within 400 ms) and clears it afterwards. Host
  poses get priority (every 50 ms) while the NPC is vulnerable, as they do while it is down.
- Her grab then plays her native combat choke on the replica. The client-sourced choke relay (chapter 93) holds the
  host's NPC and asks for the native knockout.
- Status: `vulnerableSent`, `vulnerableReceived` and `vulnerableApplies` per NPC channel in the NPC stream status.
