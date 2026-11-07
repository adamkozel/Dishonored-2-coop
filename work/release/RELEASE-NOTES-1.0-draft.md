# Dishonored 2 Corvo/Emily co-op 1.0 (draft)

The draft is collected during stage 8 and finished in step 6 (#244), after the proof runs. Each entry names its card.

## New in 1.0: the host decides the shared world

Both players still run the whole game on their own PC. What changes in 1.0 is who decides: the host's game decides the shared world, and the partner's game sends requests.

- **Story, objectives and mission decisions** come from the host's game. The partner's game does not run them on its own (#236, #253).
- **Items are per player.** Each player can pick up each item once, in their own world. Story items reach both. Coins, runes, bonecharms, ammunition and elixirs are your own. The chaos level is shared, and the host decides it (#238).
- **Doors, levers, lifts, switches and safes:** one safe combination for both players. A partner who turns a safe's dials or the Jindosh lock's wheels opens the host's lock as well (#239).
- **Joining mid-mission:** you join the host's world as it is, with the characters the host has already dealt with gone. Any load, by the host or the partner, rewinds both games together (#240).
- **Guards near a partner who is far from the host keep moving.** Before, they stood still in the partner's game (#241).
- **The partner's powers act on the host's world:**
  - The partner's Deadly Shade shadow is fought by the host's guards.
  - A Blink Assault kick lands on the host's guards with its full push (#242).
- **First mission:** the partner chooses Corvo or Emily in the co-op *Character* dialog here too; the game's own choice screen is answered for them (#243).
- **The partner keeps their progression between joins, per character (#265).** Powers, upgrades, extra health, runes, coins, ammunition, elixirs and bonecharms, found and crafted (and which of them are equipped) come back after the partner leaves and joins again. Ammunition and the like come back exactly as they were, so rejoining does not refill them. Each character keeps its own set: a partner who switches from Emily to Corvo starts Corvo with Corvo's progression, and Emily's is waiting when they play Emily again. The progression is saved on the partner's own PC, beside the co-op saves.
- **A different co-op version is explained on screen:** if your friend runs another version of the mod, a message says that both players need the same version (#243).

## Fixed

- After joining a mission in progress, characters the host had already dealt with could still be present in your world (#240).
- Safe combinations could differ between the two worlds (#239).
- In the first mission the partner had to use the game's own choice screen (#243).
- **First mission (#264):**
  - The tower safe-room doors could open for the host and stay closed for the partner.
  - At the end of the mission the partner went back to the streets and reached the next mission about two minutes late. Now the partner follows the host into it straight away.
  - The finished first mission was not marked complete in the partner's game.
  - When the partner fought Ramsey, his finisher could wait and never play, and the partner could not take his ring.
  - The other player's pistol arm pointed out to the side (#271).
- **Partner powers (#261):** a partner's Blink Assault kick now knocks the host's guards down, as it does in single player. A partner who possesses a rat is shown as a rat in the host's game.
- **Characters the host's game adds later** (Edge of the World's checkpoint reinforcements) now appear in the partner's world too, and a bottle that broke only in one world breaks in both (#260).
- **Rejoining:** a partner who left and joined again could miss the deaths that had happened in the host's world while they were away. The host now sends them again until the new game accepts them (#265).

## Still to come before 1.0

- **Step 5:** partner movement measured in numbers, and the polish pass over messages and menus (#243).
- A bonecharm made at a crafting table in a later mission is still to be checked (#265).
- **Step 6:** the proof runs and the release (#244).
- **Updating:** 1.0's Install.cmd updates an earlier version in one step. It removes the earlier version through that version's own receipt, then installs 1.0 (to be proven from 0.9.6, #244).

## Known limitations (so far)

- Two players only.
- **Partner powers (#261):**
  - While the partner possesses a fish, the host's game hides them but shows no fish.
  - Possessing a corpse is unverified.
- On a host without Doppelganger (Corvo), the partner's Deadly Shade appears in the host's world as the Doppelganger decoy.
