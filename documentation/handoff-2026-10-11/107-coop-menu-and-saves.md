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
