# 109. Validation across the four pairings (#161)

The co-op menu, isolated saves, Leave co-op, the character choice and the four pairings (#156-#160, chapters 107 and
108) are validated on two Steam accounts: the host on this PC and the partner in the lab VM (second account), in product
mode, on candidate 1214ij.

## Plan

- **The full campaign in the Corvo-Corvo pairing** (host Corvo, VM partner Corvo): the nine missions with the steps of
  each mission's last passing take (chapter 97), one retry each.
- **Samples with an Emily host:** A Long Day in Dunwall, The Clockwork Mansion and Death to the Empress, in Emily-Emily
  and Emily-Corvo. The routes use no powers (only `do` steps), so an Emily host runs them unchanged.
- **Corvo-Emily** (the earlier pairing) is the release's campaign evidence, re-run on the release binaries.
- The powers by character are covered by the lab takes of #159, #160 and #168 (chapter 108).

## Harness: Route-MissionVM-v1749

`work/natural-moveset/Route-MissionVM-v1749.ps1` adds `-PartnerCharacter corvo|emily` to the two-account route harness
(1745-1749):

- The partner's mission starts as that character (`campaign <mapinfo> <chaos> <character>`).
- **In the first mission** the game's own character screen decides:
  - The partner's choice screen gets the host's handling: 1.5 s, then two presses with the stick (Left is Corvo).
  - Its confirmation ("Play as Corvo Attano?") is a message box with No on the left and Yes on the right; it is
    answered Yes (Right, then A).
  - On the slower VM the choice can still keep her remembered character (campaign-dunwall-cc-v1746a, v1747a, v1749a
    played Emily). After the start, a wrong partner character reloads her mission, up to 3 tries.
- The partner's own co-op *Character* box on a join (logic 3 while her character status shows it open) takes her side.
- The result checks both worlds' characters at the end: the take fails when either plays the wrong one.

## Results

| Mission | Corvo-Corvo | Emily-Emily | Emily-Corvo |
|---|---|---|---|
| A Long Day in Dunwall | campaign-dunwall-cc-v1750a: pass (with the route's deaths), both in the Void | | |
| Edge of the World | campaign-eotw-cc-v1750a: pass, both in The Good Doctor | | |
| The Good Doctor | campaign-institute-cc-v1750a: pass, both on the Clockwork Mansion boat | | |
| The Clockwork Mansion | campaign-madinventor-cc-v1750b: pass, both in the Void (`void_02`); v1750a: the opening use on the boat did not register | | |
| The Royal Conservatory | campaign-curator-cc-v1750b: pass, both on the Dust District boat; v1750a: the archived-journals note did not register (as in 1697a, 1713a) | | |
| The Dust District | campaign-dust-cc-v1750a: pass, both in A Crack in the Slab | | |
| A Crack in the Slab | campaign-bunker-cc-v1750a: pass, both on the Grand Palace boat | | |
| The Grand Palace | campaign-palace-cc-v1750a: pass, both on the Death to the Empress boat | | |
| Death to the Empress | campaign-empress-cc-v1750a/b: the mission passed (both games Corvo, through the credits to the title, gameflow equal), but the takes failed their end check, which read the main menu's figure (Emily) as both players (fixed in 1750). v1750b's credits were skipped by hand with Esc | | |

Every Corvo-Corvo failure was a route step (a use that did not register) or the harness's end check; none was a
divergence between the worlds: gameflow was equal in both at every step.

- **The end credits skip natively with Esc** (found by the user, 2026-10-22). Route-MissionVM-v1751 sends Esc to the VM
  and B to the host's pad every 9 s while a game is still on the credits map and the route waits for the menu.
