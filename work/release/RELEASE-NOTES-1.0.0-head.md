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
- **Reinforcements:** guards the host's game sends later (Edge of the World's checkpoint reinforcements) appear for both players, and the partner's game no longer keeps reinforcements of its own.
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

