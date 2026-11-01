# Safe code map (stage 8 area 4.3, #239)

Read-only research, 2026-10-30. Labels: **CS** confirmed static, **CR** confirmed runtime (take cited), **H** hypothesis. Sources: Ghidra (d2-ghidra MCP), the game's decls read from `work/asset-game/base` with `tools/diag/d2_resource_index.py` and `d2_kiscule_graph.py` (output kept local), and the mod source.

## 1. What the world diff actually shows

- The differing usables are the dials' **start positions**, not the codes. The lock rolls a random `Position` (0-999, never equal to the code) and turns each dial to one of its digits (section 3). CS.
- The final diffs were taken on the *next* map: `campaign-eotw-vm-hoststory-v2031a` ended on `campaign/institute/institute_p_lowchaos` (Vasco's and the office safe: `ld_combinaison_numbers`, `_15` to `_19`), and `campaign-dust-vm-hoststory-v2031a` ended on `campaign/bunker/bunker_p_lowchaos` (`pa_ld_combinaison_numbers_9/10/11` circle lock, `_15/16/17` safe). See `digest-final-host.jsonl`, `map` field. CR.
- The codes themselves are Kiscule int variables. The world diff does not read them, so it cannot show them. They differ too: the notes come out different per world (chapter 106). CR (chapter 106) and CS.
- In the Stilton diff, `pa_ld_combinaison_numbers_10` and `_15` matched by chance (1 in 10 each). CR.

## 2. Where each code is chosen

All are Kiscule script nodes. No native safe or combination code exists: the only "combin" strings are crafting and input classes. CS.

| Map | Safe (Kiscule entity) | Dials | Who rolls the code |
|---|---|---|---|
| EotW | `script_safe_main_01_kiscul_1` (Overseer outpost) | `script_combinaison_numbers_20/21/16` | `script_kiscule_eotw_script`. On LevelLoadedFirstTime, `ExtendedRandomInt` (min 0, max 6, no repeat) fills three Int nodes (#6745/#6750/#6751). `digits_to_combination` turns them into a code, AddInt +111 writes it to `script_safe_main_01_kiscul_1/Combination`, and the script sends `SafeMain.ResetCombination`. The lock's RandomCombination is false. |
| EotW | `script_round_safe_kiscule` (shop contest) | `script_combinaison_numbers_19/7/8` | The lock itself (RandomCombination true): `RandomInt` 0-999. The level script only sends `script_round_safe_kiscule/SafeMain.ResetCombination`. |
| Institute | `ld_safe_vasco_1` | `ld_combinaison_numbers_15/17/16` | `ld_safe_vasco`. `RandomInt` 1-10 → `CombinationID` → DynamicSwitch → one of 10 fixed codes, written to `ld_safe_vasco_1/Combination`, then ResetCombination. |
| Institute | `ld_safe_office` | `ld_combinaison_numbers`, `_18`, `_19` | The lock: `RandomInt` 0-999. `ld_kiscule_master` formats the note from `ld_safe_office/Combination`. |
| Stilton (bunker) | `pa_ld_circle_lock_kiscule` | `pa_ld_combinaison_numbers_9/10/11` | `pa_ld_past_scripting` writes `pa_ld_circle_lock_kiscule/Combination` and sends `CircleLock.ResetCombination` (Stilton's book). |
| Stilton (bunker) | `pa_ld_safe_main_01_kiscule_1` | `pa_ld_combinaison_numbers_15/16/17` | RandomCombination false, and no script writes its code: it is the authored default (H). |
| Dust | `ld_straight_safe_lock_kiscule_1` (`models/campaign/dust/3_digits_lock.kiscule`, same variables) | — | `ld_kiscule` writes `ld_straight_safe_lock_kiscule_1/Combination`. Paolo's note is formatted in `ld_howlerhq_kiscule`. |

All CS from the map `.entities` files. The other campaign maps are not mapped yet. Find them with `grep 3_digits_lock\|SafeMain\.\|CircleLock\.` in each map file.

- Random nodes: `arkKisculeNodeRandomInt` (registration `0x143a74920`, factory `0x1408e9ef0` → vtable `0x141cbe168`, CS). Both they and `ExtendedRandomInt` draw from per-world state, so the rolls diverge per world (CS that they differ, CR chapter 106). That they use the game RNG `game+0x600f8` is H. Matching seeds is no fix: the RNG is shared with AI and loot (state-writers §4).

## 3. The lock (`models/environment/vs_map/props/safe_main_01/3_digits_lock.kiscule`)

Each safe entity owns this decl as a nested DeclInstance (#128). Its inputs are Digit1-3 (dial entities, in **15/17/16** order), RandomCombination, RandomPosition, AutoLock, Debug, Position, and Combination (from the entity's own `Combination` variable). CS.

- **ResetCombination:** if RandomCombination, `RandomInt` 0-999 → `Combination` (reroll while it equals `Position`); else it keeps the input `Combination`. Then the exit "Combination Changed" fires, and the entity sets its `Combination` variable. CS.
- **First ResetCombination only (Gate, auto-close 1):** ResetPosition. `RandomInt` 0-999 → `Position` (reroll while it equals `Combination`), then `combination_to_digits` → `PositionDigit1-3`. Three `ActionUsable` "Set Stage" calls put each dial on its digit (`m_wantedStageIndex`), and the dials unlock after 500 ms. CS.
- **Dial turn:** a `UsableNotification` on each dial ("Stage changed (Start)", `m_stageIndex`) writes `PositionDigitN`. `digits_to_combination` (100·d1 + 10·d2 + d3) gives `Position`, then `CombinationFound = (Position == Combination)`. Exit "Combination Found" → the entity's `CombinationFound` → `safe_door.kiscule` Unlock. CS.
- **Which value is which:** the **code** is the int variable `Combination` on the lock decl instance, mirrored to the entity's `Combination`. The **dial's current digit** is the usable state `component+0x28` (`native_usable.h`), mirrored to `PositionDigitN` and `Position`. CS.
- **Variable layout:** int variable vtable RVA `0x1c9f998` (`story_flag_mirror.h`, CR). Init `0x1408e1ec0` copies the default at `+0x18` into the value at `+0x1c` (CS, same pattern as bool `0x1408f7f10`). How to find a decl instance (entity instance table `+0x88`, decl name `+0x68`→`+8`, graph `+0x70`→vars `+0x40`): `tools/diag/d2_jindosh_lock.py` and `story_flag_mirror.h`. CS/CR.
- Whether the lock reads its own `Combination` or the entity's through the DeclInstance binding is H, so a fix must write both.

## 4. How the note shows the code

- The note decls (`note_eotw_overseersoutpost_safecombination_01`, `note_eotw_safecontest`, ...) only name a strId body. CS.
- `arkKisculeNodeActionStringId` (vtable `0x141b7b368`, factory `0x1408e96b0`, Replace handler **`0x14052a5c0`**) gathers Var0..VarN ints and calls **`arkCpntPlayerJournal::SetFormatedStringInfo` `0x140725a50`(journal, &strId, idList<int>)**. CS.
  - The journal belongs to this world's *local player*: `0x1408a8ba0(game)` = `game+0x3130b0` player 0. It is fetched with `D2_Entity_GetComponent 0x140877720(player, 0x1439c5ac0, 2)`.
  - It finds the strId in `journal+8` (decl) `+0x68` (stride 0x10, id at +0, count `+0x70` & 0xffffff) and copies the ints into `journal+0xd8 + i*0x10` (an idList<int>: ptr +0, num +8 & 0xffffff; copy `0x1409a4240`).
  - An unknown strId hits a **fatal error** (the error call does not return). CS.
- The digits go in as indices. `components/gameplay/safe_combination.stridformating` maps each to `#number_0..9`. CS. That the text is rendered from `+0xd8` when the note is shown, not baked in at pickup, is H.
- So each world's note shows that world's own code, in that player's own journal. The same nodes also format the clue strIds (`Clue_EotW_OverseersSafe_01/02_body`, `Clue_EotW_safeshopcontest_body`, `Clue_IS_03_*`, `Clue_BK_code_body`). CS.

## 5. Dial turns and the existing sync

- A dial is a 10-state `arkCpntUsableWithStateList` (`combinaison_numbers.def`: state i → i+1, 9 → 0). Use goes through `0x14077ae00` → cycle `0x14077aec0` = `0x14077a880(c, (cur+1) % n, transition 1, quiet 0)`, which emits the script events the lock listens to. CS.
- **Not mirrored**, confirmed static in the code:
  - Usable sync (`usable_catalog.h`) needs 2 states and a name without `safe`.
  - The use mirror (`use_mirror_protocol.h`) excludes `safe`/`digit`/`combinaison` and needs 1 or 2 states.
  - This is the chapter 106/87 decision: safes stay per world.
- Under host story, `ActionUsable` is a World class (`kiscule_policy.h`). The partner's "Set Stage" is replayed through its own native handler with its own inputs, so the start positions still differ (H for why; the difference itself is CR in both v2031 diffs).

## 6. Smallest change: the host's codes and notes, written into the partner's world

A safe-code sync modelled on the story flag mirror (D2SF, raw writes by name).

1. **Host, after every map load (new epoch), once both worlds are in plain gameplay and the host's locks are settled** (each lock's `Combination` and `Position` non-zero and stable for 2 s):
   - For every entity whose Kiscule instance owns a nested instance with a decl ending in `3_digits_lock.kiscule`, send {entity name, `Combination`, `Position`, `PositionDigit1-3`}.
   - Send the journal entries whose idList is non-empty, as {strId, ints}.
   - One reliable message, keyed by session, map and epoch. Resend while the partner's acknowledged digest differs.
2. **Partner, on the engine thread:**
   - For each safe found by entity name and decl, raw-write the int at `+0x1c` of the lock's `Combination`, `Position` and `PositionDigit1-3`, and of the entity's `Combination`, `Position` and `Digit1-3`.
   - For each journal strId that exists in its own decl with the **same** idList count, overwrite the ints in place: no allocation, and never `0x140725a50` with a foreign id.
   - Put each dial on the host's digit with the quiet apply `0x14077a880(c, digit, 0, quiet 1)`, so no stage event re-runs the lock.
   - Refuse and count any mismatch.
3. **Result:** the same code, the same note and clue text, and the same dial start positions, so the world diff goes green. Each player still dials and opens their own safe (chapter 106). Contents stay shared pickups.
4. **Later, optional:** mirror dial turns as host-ordered indexed states (`requestNativeUsableIndex`, quiet 0, so the lock re-evaluates). That needs equal start positions first (step 2) and turns one safe into a shared one.

## 7. What could go wrong

- **Order:** if the partner's own LevelLoadedFirstTime chain runs after the write, it rerolls. Write only after its lock has settled, verify again after 2 s, and repeat on every epoch. A save load skips LevelLoadedFirstTime and restores each world's saved values, so the sync must run on loads too (H that Kiscule ints and the journal lists are saved).
- **Raw writes fire no `Updated`.** That is wanted here (no reroll or re-format), but a script that formats on `Combination` Updated (round safe #6913) will not refresh. That is why the journal ints are written directly.
- **Journal fatal error:** never call `SetFormatedStringInfo` with a strId missing from the partner's journal decl. In-place writes only.
- **Coverage:** codes chosen outside `3_digits_lock` (another lock decl, typewriter or Jindosh codes, chapter 110) need their own entry. Build the safe list at run time from the decl name, not from a hardcoded list.
- **Code equals start position:** if the host's code equals the partner's start position, the safe still opens only after a dial change (the check runs on PositionUpdated), and step 2 copies the host's position anyway.
- **Labels to confirm at runtime:** that the int value is at `+0x1c` for the lock variables (read both worlds' `Combination` with `d2_kiscule_vars.py --nested --raw` on the institute map), and that the note text reads `journal+0xd8` when it is shown. That makes a two-account take: both notes show the host's digits, and the partner opens her safe with them.
