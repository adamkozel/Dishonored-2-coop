# Dishonored 2 co-op 1.0.0 on Linux (Steam with Proton)

For Bazzite, SteamOS and other Linux systems that run Dishonored 2 through Steam's Proton.

**Status:** Proton support is new and has not been tested on a Linux machine yet. The installer's logic was tested on Windows: the built files match the release, and uninstalling restores every original file byte for byte. If anything fails, send the text it prints.

## Install

1. Install Dishonored 2 in Steam, start it once, then quit it. This lets Proton set up the game.
2. Unpack `d2coop-v1.0.0-linux.zip`, open a terminal in the `d2coop-v1.0.0` folder, and run:

   ```sh
   ./install-linux.sh
   ```

   It finds Dishonored 2 in your Steam libraries. If your game is somewhere else, give its folder: `./install-linux.sh "/path/to/Dishonored2"`.
3. In Steam, open Dishonored 2 > Properties > General > Launch Options, and paste:

   ```
   WINEDLLOVERRIDES="dinput8=n,b" %command%
   ```

   Without this, Proton loads its own `dinput8.dll` and the mod stays off.

The installer works like `Install.cmd` on Windows:
- It builds the mod's game-derived files from your own game files, using the Wine inside Steam's Proton.
- It refuses to install if your `Dishonored2.exe`, `steam_api64.dll` or `base/master.index` aren't the supported Steam versions.
- It backs up every file it replaces.

You need `python3`, which Bazzite and SteamOS already include.

## Uninstall

```sh
./uninstall-linux.sh
```

This restores the original files. Then clear the launch option in Steam.

## If the installer can't run the setup tool

The message names each Wine it tried. You can choose one yourself, for example a Proton build:

```sh
D2COOP_WINE="$HOME/.local/share/Steam/steamapps/common/Proton - Experimental/files/bin/wine64" ./install-linux.sh
```
