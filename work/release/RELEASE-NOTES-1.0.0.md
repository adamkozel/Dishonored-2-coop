# Dishonored 2 Corvo/Emily co-op 1.0.0

Two friends play the Dishonored 2 campaign together. Each plays Corvo or Emily, and each sees the other in the world.

**This package contains no Dishonored 2 files.** The installer builds every file it needs from your own copy of the game.

## New in 1.0: the host decides the shared world

Both players still run the whole game on their own PC. What changes in 1.0 is who decides: the host's game decides the shared world, and the partner's game sends requests to it.

- **Story, objectives and mission decisions** come from the host's game. The partner's game no longer runs them on its own, so the two games can no longer drift apart in the story.
- **Items are per player.** Each player can pick up each item once, in their own world. Story items (keys, notes, maps, the rewire tool) reach both. Coins, runes, bonecharms, ammunition and elixirs are your own. The chaos level is shared, and the host decides it.
- **One safe combination for both players.** A partner who turns a safe's dials or the Jindosh lock's wheels opens the host's lock as well.
- **Joining mid-mission:** you join the host's world as it is, with the characters the host has already dealt with gone.
- **Loading:** any load, by the host or the partner, rewinds both games together. The host can also load one of their own co-op saves from the pause menu; the partner follows.
- **Guards near a partner who is far from the host keep moving.** Before, they stood still in the partner's game.
- **The partner's powers act on the host's world:**
  - The partner's Deadly Shade is fought by the host's guards, and on a Corvo host it appears as Emily's own shadow.
  - A Blink Assault kick knocks the host's guards down, as in single player.
  - A partner who possesses a rat or a fish is shown as that animal in the host's game. A partner who possesses a corpse is hidden in the host's game while inside it.
- **Reinforcements:** guards the host's game sends later (Edge of the World's checkpoint reinforcements) appear for both players.
- **First mission:** the partner chooses Corvo or Emily in the co-op *Character* dialog here too.
- **The partner keeps their progression between joins, per character.** Powers, upgrades, extra health, runes, coins, ammunition, elixirs and bonecharms (found and crafted, and which are equipped) come back after the partner leaves and joins again, exactly as they were. Each character keeps its own set, saved on the partner's own PC beside the co-op saves.
- **Mission ends:** the partner's game follows the host into the next mission straight away.
- **A different co-op version is explained on screen:** if your friend runs another version of the mod, a message says that both players need the same version.
- **Smoother partner movement:** the other player's feet no longer jump when they reverse direction.

## Fixed in 1.0

- After joining a mission in progress, characters the host had already dealt with could still be present in your world.
- Safe combinations could differ between the two worlds.
- In the first mission the partner had to use the game's own choice screen.
- **First mission:**
  - The tower safe-room doors could open for the host and stay closed for the partner.
  - At the end of the mission the partner went back to the streets and reached the next mission about two minutes late.
  - The finished first mission was not marked complete in the partner's game.
  - When the partner fought Ramsey, his finisher could wait and never play, and the partner could not take his ring.
  - The other player's pistol arm pointed out to the side.
- **The Good Doctor:** an elevator's buttons could stay disabled in the partner's world.
- **The Clockwork Mansion and A Crack in the Slab:** the partner's mission end could lose a story step or reload the wrong mission.
- A bottle that broke only in one world now breaks in both.
- **Rejoining:** a partner who left and joined again could miss the deaths that had happened in the host's world while they were away.
- In the end credits the partner no longer keeps two end-game script nodes active.

## Updating from an earlier version

`Install.cmd` updates an earlier version in one step: it removes the earlier version through that version's own receipt, restoring the original files, and then installs 1.0. You do not need to run the old `Uninstall.cmd` first.

## Requirements

- Windows 10 or 11, 64-bit.
- Dishonored 2 on Steam, current version: `Dishonored2.exe` SHA-256 `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`. The installer checks this for you.
- **Both players own the game** and install the **same mod version**.
- The two Steam accounts are **Steam friends**, and the Steam overlay is enabled (it shows the invite).
- No other Dishonored 2 mods: `base\master.index` must be the original.
- About 1 GB of free disk space in the game folder and 2 GB of free memory while installing.

## Install (both players)

1. Close Dishonored 2. An earlier version of the mod is updated in one step (see *Updating* above).
2. Download `d2coop-v1.0.0.zip` from the GitHub release and extract it anywhere.
3. Double-click **`Install.cmd`**.
   - It finds Dishonored 2 through Steam. If your game is somewhere else, run `Install.cmd "D:\Games\Steam\steamapps\common\Dishonored2"` from a command prompt.
   - It checks the game version and the package files.
   - It then builds the co-op files from your game: resource packages, the partner movement banks and prop animation tables. This takes one to three minutes.
   - It changes nothing in the game folder until every file is built and checked.
   - It backs up `base\master.index` (and any existing `dinput8.dll`) into `d2coop-backup\` and writes `d2coop-install-receipt.json`.

`Dishonored2.exe` and `steam_api64.dll` are never replaced.

## Play

1. Both players start Dishonored 2 from Steam as usual.
2. **Host:** on the main menu choose **Co-op** (under *Campaign*), then **New Game** or a co-op save. A new co-op game asks
   once who you play, Corvo or Emily. In the mission your game hosts a friends-only Steam lobby by itself. Invite your
   friend from Steam's overlay (Shift+Tab, *Friends*, *Invite to Game*), or press **F7** to open Steam's invite window.
3. **Partner:** start the game, press a key at the title screen, and wait in the main menu (or anywhere in a mission).
   If you accept the invite at the title screen, your game waits there until you press a key. Accept the invite, or use
   **Join Game** on the host in your Steam friends list.
4. **Partner:** the game's *Character* dialog asks who you play this time: **Play as Corvo Attano** or **Play as Emily
   Kaldwin**. You choose again every time you join. Your game then loads the host's current mission by itself; press any
   key at the mission card when it has loaded. If the host is already further into the mission, you are placed beside
   the host.
5. Once both players are in the same mission, every co-op system turns on by itself.
6. To leave, open the pause menu and choose **Leave Co-op** (or press **F8**).

How the two games share the world:

- Partners cannot affect each other directly: no damage, powers, takedowns or prompts on each other. Normal collision is kept.
- *Memories of Training* (the tutorial) is a stealth lesson for two: its trainer notices either player, and a noticed lesson restarts for both. Let one player take the stealth lessons while the other stays out of sight.

## Saves, deaths and missions

- Co-op games have their own saves, apart from your campaign saves. *Co-op* on the main menu shows only co-op saves;
  *Campaign* shows only your own.
- To continue later, the host chooses *Co-op* and loads the co-op save. The game hosts again by itself, and the
  partner joins again.
  The partner's game follows the host's mission.
- When either player dies, both see the game-over screen. Restarting from the last checkpoint restarts both games together.
- Cutscenes play for both players.
- At a mission's end the host chooses for both players: *Continue* takes both games to the next mission, and *Replay
  Mission* restarts the mission in both. The partner's own buttons on the result screen wait for the host's choice.
- Either player can do the story. The host's game records it for both: objectives, story items, level changes and the outcomes of characters (spared, killed, knocked out) reach the host's world whoever caused them.

## Uninstall

Double-click **`Uninstall.cmd`** (or `Uninstall.cmd "<your Dishonored2 folder>"`).
- It removes everything the installer added.
- It restores the original files byte for byte and checks them.

To be completely sure afterwards, let Steam verify the game files: *Properties > Installed Files > Verify integrity of game files*.

## Troubleshooting

| Problem | What to do |
|---|---|
| The game says **Free Trial** | Start Dishonored 2 from your Steam library (or the Steam shortcut), not by opening `Dishonored2.exe` in the game folder: started directly, the game cannot confirm you own it. |
| "Unsupported Dishonored2.exe build" | Update the game in Steam. The mod supports only the version above. |
| "base/master.index is not the original" | Another mod or an old install changed it. Verify the game files in Steam, then install again. |
| "Dishonored 2 was not found" | Pass your game folder to `Install.cmd` (see Install). |
| "Co-op 1.0.0 is already installed here" | This version is already installed. To reinstall it, run `Uninstall.cmd` first. |
| Windows Defender reports `dinput8.dll` (for example `Trojan:Win32/Bearfoos.A!ml`) | This is a false positive. The mod hooks the game's own functions, which is what this heuristic looks for. Restore the file from *Protection history*, or add the game folder as an exclusion, then install again. |
| F7 does nothing | Check that `d2coop\d2coop.cfg` exists in the game folder (re-install). The game window must have focus. |
| The invite window does not open | Enable the Steam overlay for Dishonored 2. |
| The partner cannot join | Both players need the same mod version and must be Steam friends. The lobby is friends-only. |
| The partner's game stays at the title screen after accepting the invite | Press a key: it joins after that ([#146](https://github.com/adamkozel/Dishonored-2-coop/issues/146)). |
| Something went wrong during install | Nothing in the game folder changed unless the installer said "Installed". Run `Uninstall.cmd` if a receipt exists. |
| Something in the shared world misbehaves: the story, an item, a safe or a mission end | The host's game decides these for both players. To go back to the 0.9.6 behaviour for one area, the host adds a line under `[d2coop]` in `d2coop\d2coop.cfg`: `host_story=0` (each game runs its own story), `items_per_player=0` (pickups are shared again), `safe_codes=0` (each world keeps its own safe codes) or `mission_decision=0` (each player chooses *Continue* or *Replay Mission* for their own game). The host's setting holds for both players. Please report what happened in a [GitHub issue](https://github.com/adamkozel/Dishonored-2-coop/issues). |
| Stutters or hitches in a session | Open `d2coop\coop-status.json.frame-timing.json` in the game folder after a few minutes of play (the mod rewrites it every second). Report its `window` (frame-time percentiles and spikes per minute) and its `spikes` list (each hitch with the mod's own time and its most expensive part) in a [GitHub issue](https://github.com/adamkozel/Dishonored-2-coop/issues), with both players' files if you can. For a deeper report, add `diagnostics=1` under `[d2coop]` in `d2coop\d2coop.cfg`, play the moment again, and attach the `coop-status.json.*` files. Remove the line afterwards, because the full diagnostics cost frame time. |

## Known limitations

- Two players only.
- The game uses a little more memory over long sessions, with or without the mod. Restart the game after several hours.

## About the partner animation

- The partner's movement banks are rebuilt from your game's animations. On a different CPU the values can differ from ours in their last digits, which is invisible.
- Some partner movement clips are retargeted from Adobe Mixamo motion.

## What the installer builds

| File | Built from |
|---|---|
| `base\master.index`, `base\game1voidmod.*` to `game4voidmod.*` | your resource containers and `data\decl-recipes.bin` (our text and line copies from your own decl files) |
| `d2coop\corvo-choke-bank.bin`, `d2coop\emily-choke-bank.bin` | your animations, sampled through your own `Dishonored2.exe`, and `data\*-bank.patch` (which animations, times and blends, plus our corrections) |
| `d2coop\native-tables.bin` | your prop and first-person animations and `data\native-tables.recipe` |

The release ships only our own files: `dinput8.dll` (the mod), `bin\d2coop_setup.exe` (the builder), the data above and these scripts.
