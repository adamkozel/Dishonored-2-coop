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
