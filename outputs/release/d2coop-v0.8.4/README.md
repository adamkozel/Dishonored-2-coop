# Dishonored 2 Corvo/Emily co-op 0.8.4

Two friends play the Dishonored 2 campaign together. The host plays Corvo, the partner plays Emily, and each sees the other in the world.

**This package contains no Dishonored 2 files.** The installer builds every file it needs from your own copy of the game.

## Fixed in 0.8.4

- When the partner assassinates a story target (for example Ramsey), the body now stays in her world and lies where it fell in both worlds, so either player can search it ([#140](https://github.com/adamkozel/Dishonored-2-coop/issues/140)).
- A partner who accepts the invite at the title screen ("Press any key") now joins only after pressing a key there, so saving works ([#146](https://github.com/adamkozel/Dishonored-2-coop/issues/146)).

## New in 0.8.3

Emily's powers now act on the host's characters as they do in single player ([#131](https://github.com/adamkozel/Dishonored-2-coop/issues/131)):

- **Mesmerize:** the characters it takes walk to her spirit and stand entranced, in both games.
- **Doppelganger:** the host's guards see her shade and fight it. When they kill it, it dies in her game too.
- **Deadly Shade:** the shade's kills happen in both games.
- **Baffling Shade:** the confusion when the shade dies affects the host's guards.

## Fixed in 0.8.2

- On the first map of *A Long Day in Dunwall*, a death is now shared: both games show the game-over screen and restart together ([#148](https://github.com/adamkozel/Dishonored-2-coop/issues/148)).
- When the partner assassinates a story target (for example Ramsey), the kill now happens in both worlds: the host's game holds the target still during her finishing move ([#140](https://github.com/adamkozel/Dishonored-2-coop/issues/140)).
- A partner who leaves (F8) and joins the same mission again is placed beside the host ([#135](https://github.com/adamkozel/Dishonored-2-coop/issues/135)).

## Fixed in 0.8.1

- When the host finished a mission, the partner's game could stay in the finished mission while the host's moved on. Both games now move on to the next mission together.

## New in 0.8.0

- **Shared combat.** The host's game decides what happens to every NPC, and both worlds show the same result:
  - damage, knockouts, deaths and takedowns by either player;
  - guards, hounds, clockwork soldiers and scripted civilians;
  - the partner now takes damage from guards ([#108](https://github.com/adamkozel/Dishonored-2-coop/issues/108)).
- Counterattacks and knockdowns are shown on the partner ([#9](https://github.com/adamkozel/Dishonored-2-coop/issues/9)).
- Spyglass, Timepiece, stumbles and recoils are shown on the partner ([#102](https://github.com/adamkozel/Dishonored-2-coop/issues/102), [#122](https://github.com/adamkozel/Dishonored-2-coop/issues/122)).
- The Doppelganger clone and the Mesmerize spirit are shown to the partner ([#104](https://github.com/adamkozel/Dishonored-2-coop/issues/104)).
- Far Reach pulls of objects and bodies happen in both worlds ([#115](https://github.com/adamkozel/Dishonored-2-coop/issues/115)).
- Power upgrades with world outcomes are shared, for example time stops, kills and possessed bodies ([#117](https://github.com/adamkozel/Dishonored-2-coop/issues/117)).
- Breakable doors, barricades and glass break in both worlds ([#132](https://github.com/adamkozel/Dishonored-2-coop/issues/132)).

## Requirements

- Windows 10 or 11, 64-bit.
- Dishonored 2 on Steam, current version: `Dishonored2.exe` SHA-256 `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`. The installer checks this for you.
- **Both players own the game** and install the **same mod version**.
- The two Steam accounts are **Steam friends**, and the Steam overlay is enabled (it shows the invite).
- No other Dishonored 2 mods: `base\master.index` must be the original.
- About 1 GB of free disk space in the game folder and 2 GB of free memory while installing.

## Install (both players)

1. Close Dishonored 2.
2. Download `d2coop-v0.8.4.zip` from the GitHub release and extract it anywhere.
3. Double-click **`Install.cmd`**.
   - It finds Dishonored 2 through Steam. If your game is somewhere else, run `Install.cmd "D:\Games\Steam\steamapps\common\Dishonored2"` from a command prompt.
   - It checks the game version and the package files.
   - It then builds the co-op files from your game: resource packages, the partner movement banks and prop animation tables. This takes one to three minutes.
   - It changes nothing in the game folder until every file is built and checked.
   - It backs up `base\master.index` (and any existing `dinput8.dll`) into `d2coop-backup\` and writes `d2coop-install-receipt.json`.

`Dishonored2.exe` and `steam_api64.dll` are never replaced.

## Play

1. Both players start Dishonored 2 from Steam as usual.
2. **Host (Corvo):** start a new game as Corvo, or load a Corvo save. In game, press **F7**. A friends-only Steam lobby is created and Steam's invite window opens. Invite your friend.
3. **Partner (Emily):** start the game, press a key at the title screen, and wait in the main menu (or anywhere in a mission). If you accept the invite at the title screen, your game waits there until you press a key. Accept the invite, or use **Join Game** on the host in your Steam friends list. Your game loads the host's current mission by itself; press any key at the mission card when it has loaded. If the host is already further into the mission, you are then placed beside the host.
4. If the game asks who to play (the start of *A Long Day in Dunwall*), the host chooses **Corvo** and the partner **Emily**.
5. Once both players are in the same mission, every co-op system turns on by itself.
6. **F8** leaves the session.

## Saves, deaths and missions

- Each game keeps its own saves in its usual profile: the host's are Corvo saves, the partner's are Emily saves.
- To continue later, the host loads their save and presses F7, and the partner joins again. The partner's game follows the host's mission.
- When either player dies, both see the game-over screen. Restarting from the last checkpoint restarts both games together.
- Cutscenes play for both players. Mission ends take both players to the next mission together.
- Either player can do the story: objectives, story items and level changes completed by the partner count for both worlds.

## Uninstall

Double-click **`Uninstall.cmd`** (or `Uninstall.cmd "<your Dishonored2 folder>"`).
- It removes everything the installer added.
- It restores the original files byte for byte and checks them.

To be completely sure afterwards, let Steam verify the game files: *Properties > Installed Files > Verify integrity of game files*.

## Troubleshooting

| Problem | What to do |
|---|---|
| "Unsupported Dishonored2.exe build" | Update the game in Steam. The mod supports only the version above. |
| "base/master.index is not the original" | Another mod or an old install changed it. Verify the game files in Steam, then install again. |
| "Dishonored 2 was not found" | Pass your game folder to `Install.cmd` (see Install). |
| "Co-op is already installed here" | Run `Uninstall.cmd` first. |
| Windows Defender reports `dinput8.dll` (for example `Trojan:Win32/Bearfoos.A!ml`) | This is a false positive. The mod hooks the game's own functions, which is what this heuristic looks for. Restore the file from *Protection history*, or add the game folder as an exclusion, then install again. |
| F7 does nothing | Check that `d2coop\d2coop.cfg` exists in the game folder (re-install). The game window must have focus. |
| The invite window does not open | Enable the Steam overlay for Dishonored 2. |
| The partner cannot join | Both players need the same mod version and must be Steam friends. The lobby is friends-only. |
| The partner's game stays at the title screen after accepting the invite | Press a key: it joins after that ([#146](https://github.com/adamkozel/Dishonored-2-coop/issues/146)). |
| Something went wrong during install | Nothing in the game folder changed unless the installer said "Installed". Run `Uninstall.cmd` if a receipt exists. |

## Known limitations

- Two players only. The host is always Corvo and the partner is always Emily.
- After joining a mission already in progress, a few characters the host has already dealt with can still be present in your world.
- Safes belong to each player's own world: each player opens their own, and combinations can differ between the two worlds (read them from the notes in your own game). Whatever one player takes from a safe is gone for both.
- Partners cannot affect each other directly: no damage, powers, takedowns or prompts on each other. Normal collision is kept.
- In the end credits the partner keeps two end-game script nodes active ([#94](https://github.com/adamkozel/Dishonored-2-coop/issues/94)). Both players still reach the main menu.
- With **Deadly Shade**, the host's guards do not fight Emily's deadly shadow itself, although its kills happen in both games ([#131](https://github.com/adamkozel/Dishonored-2-coop/issues/131)).
- The partner's movement banks are rebuilt from your game's animations. On a different CPU the values can differ from ours in their last digits, which is invisible.
- Some partner movement clips are retargeted from Adobe Mixamo motion.
- The game uses a little more memory over long sessions, with or without the mod. Restart the game after several hours.

## What the installer builds

| File | Built from |
|---|---|
| `base\master.index`, `base\game1voidmod.*` to `game4voidmod.*` | your resource containers and `data\decl-recipes.bin` (our text and line copies from your own decl files) |
| `d2coop\corvo-choke-bank.bin`, `d2coop\emily-choke-bank.bin` | your animations, sampled through your own `Dishonored2.exe`, and `data\*-bank.patch` (which animations, times and blends, plus our corrections) |
| `d2coop\native-tables.bin` | your prop and first-person animations and `data\native-tables.recipe` |

The release ships only our own files: `dinput8.dll` (the mod), `bin\d2coop_setup.exe` (the builder), the data above and these scripts.
