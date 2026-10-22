# 107. The Co-op menu entry and isolated co-op saves (#156)

The user's added scope (2026-10-21, [roadmap](02-roadmap.md) "Added scope"):
- a **Co-op** entry directly under Campaign in the native main menu;
- it opens the same save selection and creation screen, with co-op saves isolated from the campaign slots;
- a co-op session never reads or writes a campaign save, and the campaign never sees a co-op save.

## The main menu movie (confirmed static)

The main menu is the Iggy movie `ui/main_menu.iggy`, from `ui/mainmenu.iggyfile` in `game1voidmod`.
- It is AS3: one ABC block at file offset 0x20e14, ABC 46.16, 429 classes, 4467 methods.
- It was read with a small ABC parser written for this work, from the public AVM2 format (no third-party code). The
  movie itself is game-derived and stays out of the repository.

**The Home entries:**
- They are data. `com.bsp.main.data:MainMenuData._initData` builds a `com.ark.data:UIDataProvider`
  (`_mainMenuEntries`), in this order:
  - Campaign, Tutorial, Options and Credits;
  - the DLC entry, only when `DLCEntryEnabled`;
  - Quit, on PC.
- `MainHomeScreen` shows `mainMenuData.mainMenuEntries` through its `MainMenu`. `MainMenuScene` (the root) exposes
  `mainMenuData`, which is the same object the engine binds as `mainMenuData.campaignData...`.
- Labels come from localization keys in `updateEntriesLabel`, for example `ui_main_home_campaign` and
  `ui_main_tutorialMode`.

**What a Home entry does:**
- An entry is a `MenuItemDefinition` (label, locked, button class, `screenNameLinkage`, `kisculeEvent`, `gameData`,
  `gameCallback`).
- `MainHomeScreen._onEntrySelected` handles it in two steps:
  - It navigates to `screenNameLinkage` (`NavigationManager.gotoScreenByLinkage`), passing `gameData`.
  - It sends `kisculeEvent` to the engine (`GameService.kisculeHandler.invokeKisculeNode`).
- Campaign uses screen `com.bsp.main.campaign.CampaignScreen` and event `GotoCampaignScreen`. Leaving that screen sends
  `GofromCampaignScreen`.
- `ExtraItemDefinition` entries send a game callback instead (Credits: `ShowCredits`), and the Tutorial entry has its
  own handler.

**Spare objects:** `MainMenuData._DLCEntry` is always constructed as a `MenuItemDefinition`. On PC it is never added to
the list, so it is a spare entry object.

**The data provider:**
- `UIDataProvider` holds a `Vector` (`_data`). It has `addItem`, `getItemAt`, `removeItem(At)` and `replaceItem(At)`,
  with update events that rebuild the menu.
- `Def_InteractiveElement` has `label`, `labelID`, `locked` and `instanceIndex`.

## The Campaign screen data (confirmed static)

The engine section `arkMainMenuSectionCampaign` fills these:
- `mainMenuData.campaignSlots`, an array of campaign slots, each with `savesData` and `missionsData`;
- `campaignSlotIndex`, `characterIndex` and the continue, load and missions entries.

On disk, saves are `base/savegame/SLOTxx` folders, with `game.details`, `checkpoint.dat` and `screenshot.png`.

## Design (in progress)

1. **The entry:** at runtime, the mod turns the spare `_DLCEntry` into the Co-op entry and inserts it directly under
   Campaign. It is labelled Co-op, uses the Campaign screen and its data, and carries the event `GotoCoopScreen`. No
   game file changes.
2. **The event:** the mod's hook on the engine side of `invokeKisculeNode` turns `GotoCoopScreen` into co-op mode and
   passes the native `GotoCampaignScreen` on. `GotoCampaignScreen` turns co-op mode off.
3. **Isolation:** while co-op mode is on (the host after the Co-op entry, or the partner in a session), the game's save
   root is redirected to a co-op subfolder. Every native save screen, autosave, checkpoint and load then sees only co-op
   saves. The player's own saves and the protected files are never written.

## The pause menu (for #157; confirmed static)

`ui/corvo_pause_menu.iggy` (and Emily's) is AS3 too: ABC at 0x1813c, 391 classes.
- `com.bsp.pause.data:PauseMenuData._initMenuEntries` adds Resume, Save, Load, Restart, Options and Tutorials to a
  `UIDataProvider` (`_entries`), then Statistics when available, then Quit.
- Quit is labelled `UI_quitgame`.
- The engine side of quitting is the pause callbacks (`BackToMainMenu`, `QuitGame` and `ReloadLastCheckp...`,
  strings at 141b6ac60).
- Plan: during a session the mod relabels `_quitEntry` "Leave co-op", and its hook on the engine-side quit callback
  acts as F8.

### Leave co-op (1214ht; take coop-leave-v1770a, two accounts)

- **The label (confirmed runtime):** during an online session the mod sets `pauseData.quitEntry.label` through the
  Iggy API whenever the pause screen (logic 2, its movie at state+0x20) shows. Both pause movies read LEAVE CO-OP:
  Corvo's on the host and Emily's in the VM. After the session a new pause movie shows the native Quit Game.
- **The leave (confirmed runtime):** the quit dialog's second button sends `BackToMain` through the Iggy hub. During a
  session the hub hook renames it `ResumeGame` (the same length), so the pause closes as Resume does, and the session
  is left as F8 leaves it. Both sides were tested: the host leaving (the VM loses its partner and stays in its world)
  and the partner leaving (the host keeps hosting its lobby and loses its partner). Each player stays on the same map
  and world epoch.
- **The dialog text (not yet working):** the confirmation still read the stock question ("Do you want to quit the
  game?", "Back To Main Menu").
  - `Localization.translate` in the movie caches each key in a static `_locals` dictionary and calls the native
    `IggyFunctions.translate` only on a miss (confirmed static). `_C_ClearLocalization` cleared it (clears 1).
  - The engine registers the **UTF-8** translation callback (table 0x143ec0920 +8), not the UTF-16 one (+0)
    (`translationHooked` stayed false, confirmed runtime).
  - FUN_14102a270 (confirmed static): when +0 is empty it converts the key to UTF-8 and calls `+8(data +0x20, {key,
    length}, {out, length})` (or +0x18 when the flag at +0x38 is set), converts the result to UTF-16 and frees it with
    `+0x28(data +0x30, out, length)`.
  - 1214hu wraps the UTF-8 callback the same way. Take coop-leave-v1770b passed all its checks: the question reads
    "Leave the co-op session?" on both sides (confirmed runtime).
  - The middle button still read "Back To Main Menu". `PauseMenuScreen._onQuitGameClicked` passes the raw keys
    `ui_common_cancel` and `ui_gameover_button_back_main_menu` as button labels (confirmed static). Their text fields
    resolve them through Iggy's second translation table (0x143ec08e0, keys prefixed `@`; FUN_1400358c0 →
    FUN_140034e40), not `IggyFunctions.translate`.
  - Both tables end in the engine lookup `FUN_140165420(&0x1432c7e98, key)`, which returns the key itself when there
    is no entry (confirmed static). 1214hv hooks that lookup for the two keys. It applies only during a session while
    the pause menu (logic 2) or its quit dialog (logic 3 entered from 2) is open. The BackToMain rename now needs the
    same pause context, so a GameOver screen keeps its native Back to main menu.
  - Take coop-leave-v1770c (1214hv) passed again, but the hook never swapped. The message box is built by the engine
    (arkMessageBoxManagerLocal at game+0x30f260; its AS3 request handler 0x1404630f0). It turns each button key into
    a string id with `FUN_140165c80(&id, key)` and the movie shows the label by id (confirmed static). Raw label text
    is impossible, so 1214hw maps the key `ui_gameover_button_back_main_menu` to the game's own `ui_common_confirm`
    in that function, in the same pause context. The dialog then reads "Leave the co-op session?" with Cancel,
    Confirm and Back To Desktop.
- **The host's co-op save on two accounts (take coop-hostsave-v1776a, 1214hv; every behaviour check passes):**
  - The host on this PC uses the simulated pad: pause, Quit Game, Back To Main Menu, the title, CO-OP, New Game. It
    gets the native character choice (Corvo) and starts the mission. It hosts a friends-only lobby, and the VM
    (second account) joins from its menu and follows the mission as Emily.
  - The host saves through the pause menu (Save Game, "Create a new Savegame": SLOT04 in `savegame_coop`). It then
    loads that save (Load Game; the question "Do you want to load this savegame?" has the focus on No, so Right then
    A). The world epoch changes and the partner is connected again.
  - The campaign slot files are unchanged. The campaign `profile.bin` is saved once at the switch to co-op, which
    persists the player's own pending profile; after that nothing in the co-op game touches it.
  - There is a host window video.
  - The restore after the take failed: the relaunched host game hung at startup (logic 0, Not Responding). It was
    relaunched and restored by hand (coop-hostsave-v1776a-recovery.log).
  - Earlier takes failed in the probe:
    - v1773a: on the host's menu the pad focus started on QUIT GAME. Down reached the Also Available banner and A
      opened the Steam overlay store page.
    - v1774a: Up reaches that banner above CAMPAIGN, so two Downs are needed to reach CO-OP.
    - v1775a: A answered No to the load question.
  - In v1775a the intro skip passed the character choice and the host played Emily; in v1776a the choice appeared
    (chapter 108).
- **The save flow (take coop-saveflow-v1772a, 1214hv, pass):** CO-OP, New Game, quicksave, the title, CO-OP,
  CONTINUE loads the co-op quicksave, the title again, then CAMPAIGN lists the player's own saves. The campaign slots
  are unchanged and `profile.bin` keeps every key. The file itself is rewritten at each switch, because its dirty
  byte was set at every switch (`lastDirty` 1), so 1214hu's skip never applies.

## Save isolation research (confirmed static unless marked)

- **The save root:**
  - The pointer `0x142286f40` holds `"savegame"` (0x141ac7378). It is not cached: eight paths read it on every call
    (save 1400e5f90, load 1400e5ac0, enumerate 1400e5150, delete 1400e49e0, delete-all 1400e4ba0, delete files
    1400e4db0, enumerate files 1400e56d0, screenshot loader 1400d55a0).
  - The folder is `SLOT%02d` (format pointer 0x142318180).
- **The dispatcher:** all seven file operations go through `0x1400e5e40` (vtable entry 0x141add460). Its flags are at
  `*(job+0x68)+0xb0`: slot load 1, slot save 2, `profile.bin` 0x201 and 0x202 (bit 0x200).
- **Why a sibling folder:** the save listing is recursive. It finds `game.details` under the root, so a subfolder of
  `savegame` would show in the normal campaign as a slot. Co-op saves therefore go in the sibling `savegame_coop`
  (hypothesis: it resolves under `<fs_savepath>/base/` like the original), and the profile stays shared.
- **Slots:** a slot is campaign x 30 + index (3 campaigns).

  | Index | Save |
  |---|---|
  | 0 | mission transition |
  | 1-2 | autosaves |
  | 3 | quicksave |
  | 4-20 | manual saves |
  | 21-29 | mission-start saves (20 + chapter) |

  The lab folders match (SLOT00, 21, 60-62, 81-89).
- **The save list:**
  - It belongs to the session (`*(void**)0x142287c78`). It is enumerated by session vtable +0xa0 (1400dae40), which
    frees `sgm+0xd0` (sgm = `*(session+0x20)`) and clears the ready flag `sgm+0x3920`. Completion (1400db550) sets the
    flag again.
  - The save manager (`*(u8**)0x1439a5290 + 0x2e75d8`) rebuilds its per-campaign lists with `140660210(mgr,-1)`.
  - The Campaign screen builder is 1404fb3b0.
- **New Game** (140500750, 140466ca0): it resets that campaign's profile keys (140465aa0) and queues the deletion of
  the campaign's saves (14065cb20).
- **Switching** (main thread, while idle):
  1. Check that nothing is saving or loading.
  2. Clear the screenshot cache (1400d69a0 per entry, then 1400d5ea0; 0x142318198 = -1).
  3. Swap the pointer.
  4. Re-enumerate and wait for the ready flag.
  5. Rebuild the save manager lists.
  Reverse the same steps when leaving co-op.
- **The design risk:** campaign progress lives in `profile.bin`: `campaignIndex`, `campaignSaveType_%02d`,
  `HasFinishedCampaign_%d`, `PlayTime_%d(_%d)` and `DeathCount_%d(_%d)`. A co-op New Game would reset those keys for
  its campaign index in the shared profile. Either stash and swap them across a co-op switch (with a stash file for
  crash recovery), or give co-op its own profile (a profile reload at run time).

## The profile (confirmed static unless marked)

**The objects:**
- `idProfileMgr` is `lu+0x10`: the session (`*(void**)0x142287c78`), then the sign-in manager (vtable +0x38), then
  the local user `lu` (+0x28(0)). It has the save processor at +0, the load processor at +8 (busy byte +0x3a11 on
  each) and the profile at +0x18.
- The profile (`idPlayerProfile`, vtable 0x141ac9e70) has a dirty byte at +8, an achievement bitset at +0x10/+0x18 and
  an `idDict` of string keys at +0x28.
- Dict access: get `140657ce0`, read-only `140657c80`, find `140104190`, set `140105040`, set-int `140069b50`,
  reset `14005a8c0`. Writers hold the lock `1401548e0()` vtable +0x48/+0x50.

**Loading and saving:**
- `LoadProfile` `1400d37b0(pm)` runs at sign-in. It is synchronous, it is refused when a processor is busy, and it
  merges into the existing in-memory profile.
- When the file is found it re-runs `dishonored2Config.cfg` and the bindings (hypothesis: harmless). When it is
  missing, it saves the in-memory profile.
- `SaveProfile` `1400d3bd0(pm)` is asynchronous. It runs after each slot save, on `lu+0x3c & 1`, and after a missing
  or corrupt load.
- Both read the save root when the job runs, so moving the root also moves `profile.bin`.

**Keys:**
- Per campaign: `campaignSaveType_%02d`, `HasFinishedCampaign_%d`, `TutorialShouldLaunchCampaign_%d`, `PlayTime_%d`,
  `DeathCount_%d`, per-chapter `PlayTime_%d_%d` and `DeathCount_%d_%d`, and 26 `difficulty*_%02d`.
- Global: `campaignIndex`, `HasFinishedNewGamePlus`, the `missionPlus*` keys, `HasSetGamma`, `HasPreorderDLC`,
  `HasLinkedBnet`, 46 `ARK_PLAYER_STATISTIC_*` stats and the achievement bitset.

**Decision:** co-op has its own `savegame_coop/profile.bin`, seeded once from the player's profile with every
per-campaign key cleared (global keys, stats and achievements kept). The switch runs on the main menu with both
processors idle:
1. Save the profile and wait.
2. Keep a copy of the dict for rollback.
3. Swap the root.
4. Reset the in-memory profile (or, the first time, clear only the campaign keys).
5. `LoadProfile`; if it is refused, roll back.
6. Re-list the saves, call `140660210(cm, GetCampaignIndex 140657380())` and the stats reload `14067d3f0(game+0x1f77c8)`,
   and rebuild the Campaign screen.

Leaving co-op runs the same steps in reverse.

**The switch in practice (takes coop-saveflow-v1768a and v1769a):**
- Co-op New Game, the quicksave and the mission-start save are written to `savegame_coop` (SLOT03, SLOT21); the
  campaign slots are untouched (path, size, time and SHA-256 equal).
- Each re-visit of the Co-op or Campaign screen re-lists the saves (1214ht; v1768a listed none on the second visit).
- Leaving a game returns to the title screen (Press Any Key), not straight to the menu.
- The switch rewrote the player's `profile.bin` with identical keys: only bytes 0-3 and 9-14 of its header differed
  (hypothesis: a checksum and a time). The writable profile accessor `140657ce0` sets the dirty byte at profile+8
  (confirmed static), so 1214hu saves the profile before a switch only while that byte is set.
