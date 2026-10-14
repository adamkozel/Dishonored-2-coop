Dishonored 2 - Corvo/Emily co-op (build 1193)

Install (PowerShell 7, game closed):
  pwsh -File Install-D2Coop.ps1 -GameDirectory "<your Dishonored2 folder>"
The installer refuses unknown game builds, backs up base\master.index and any
existing dinput8.dll, and writes d2coop-install-receipt.json.
Dishonored2.exe and steam_api64.dll are never replaced (native Steam).

Uninstall (restores the original files byte-identically):
  pwsh -File Uninstall-D2Coop.ps1 -GameDirectory "<your Dishonored2 folder>"

Playing:
  Both players install the same build and start the game from Steam.
  Host (plays Corvo): in game, press F7. A friends-only Steam lobby is
    created and Steam's invite overlay opens.
  Partner (plays Emily): accept the invite, or use "Join Game" on the host in
    the Steam friends list. The partner's game follows the host's mission.
  F8 leaves the session.
  Once both players are in the same mission, every co-op system arms itself.

Status:
  Lab-verified on one PC with two instances. Online play between two Steam
  accounts has not been validated yet.
