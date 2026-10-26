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
| A Long Day in Dunwall | campaign-dunwall-cc-v1750a: pass (with the route's deaths), both in the Void | campaign-dunwall-ee-v1751a: pass, both in the Void | campaign-dunwall-ec-v1751a: pass, both in the Void |
| Edge of the World | campaign-eotw-cc-v1750a: pass, both in The Good Doctor | not sampled | not sampled |
| The Good Doctor | campaign-institute-cc-v1750a: pass, both on the Clockwork Mansion boat | not sampled | not sampled |
| The Clockwork Mansion | campaign-madinventor-cc-v1750b: pass, both in the Void (`void_02`); v1750a: the opening use on the boat did not register | campaign-madinventor-ee-v1751b: pass; v1751a: the partner died at the heal step (both GameOver) | campaign-madinventor-ec-v1751b: pass; v1751a: the partner died at the guards step (both GameOver) |
| The Royal Conservatory | campaign-curator-cc-v1750b: pass, both on the Dust District boat; v1750a: the archived-journals note did not register (as in 1697a, 1713a) | not sampled | not sampled |
| The Dust District | campaign-dust-cc-v1750a: pass, both in A Crack in the Slab | not sampled | not sampled |
| A Crack in the Slab | campaign-bunker-cc-v1750a: pass, both on the Grand Palace boat | not sampled | not sampled |
| The Grand Palace | campaign-palace-cc-v1750a: pass, both on the Death to the Empress boat | not sampled | not sampled |
| Death to the Empress | campaign-empress-cc-v1751a: pass, through the credits (skipped) to the title. v1750a/b: the mission passed but the end check read the main menu's figure (fixed in 1750); v1750b's credits were skipped by hand with Esc | campaign-empress-ee-v1751a: pass, to the title | campaign-empress-ec-v1753b: pass, to the title. v1751a: the partner stayed on the next map's mission card (the expect step pressed no key; fixed in 1753); v1751b: the partner died in the interior (both GameOver); v1753a: the host's tower door use did not register |

**Result:** the Corvo-Corvo campaign passes all nine missions, and the Emily-Emily and Emily-Corvo samples pass the
first mission, The Clockwork Mansion and the last mission. Every failed attempt was a route step (a use that did not
register), a harness gap (the end check at the menu, the card key in the expect step) or a partner death in combat,
which both games share as designed; none was a divergence between the worlds: gameflow was equal in both at every
step, and both games played the requested pairing.

Harness versions: 1751 skips the end credits; 1752 uses the VM's pad (B) for them, as its keyboard Esc took dozens of
presses (campaign-empress-cc-v1751a: host 1 press, VM 45); 1753 presses A for a world on a mission card during an
expect step.

- **The end credits skip natively with Esc** (found by the user, 2026-10-22). Route-MissionVM-v1751 sends Esc to the VM
  and B to the host's pad every 9 s while a game is still on the credits map and the route waits for the menu.

## The campaign on co-op saves (#175)

Release 0.9.0 left deaths and checkpoints unshared in real co-op sessions (fixed in 0.9.1, [chapter 90](90-release-packaging-and-steam-sessions.md)). The lab's mission takes had both games on their normal saves, while installed players are on co-op saves (`savegame_coop` and its own profile, [chapter 107](107-coop-menu-and-saves.md)). This section replays the campaign the installed way.

**Tools:**
- 1214in adds the lab command `coop_saves on|off`. It is 1214im (the 0.9.1 binary) plus that command; product mode never reads lab commands.
- Route-MissionVM-v1762 adds `-CoopSaves`: both games switch to co-op saves before the mission loads, and the notes record both switch states.
- v1763 saves a frame of every message box before answering it.
- v1765 answers a pending choice (logic 1, pending 9) with A.

**Results** (1214in, release pairing: host Corvo, VM partner Emily):

| Mission | Passing take | Earlier attempts |
|---|---|---|
| A Long Day in Dunwall (deaths) | campaign-dunwall-coop-v1762a | |
| Edge of the World | campaign-eotw-coop-v1762a | |
| The Good Doctor | campaign-institute-coop-v1762a | |
| The Clockwork Mansion | campaign-madinventor-coop-v1762a | |
| The Royal Conservatory | campaign-curator-coop-v1763a | v1762a: the archived-journals use did not register; v1762b: the final use on Meagan did not end the mission |
| The Dust District | campaign-dust-coop-v1762a | |
| A Crack in the Slab | campaign-bunker-coop-v1762a | |
| The Grand Palace | campaign-palace-coop-v1763c | v1762a: the closing talk to Meagan did not end the mission; v1762b: cut off by a power failure (recovery log beside it); v1763a/b: the host died in the Duke fight |
| Death to the Empress | campaign-empress-coop-v1765e | v1763a/v1764c: sat on the tower door's Go / Stay choice; v1763b: manually assisted, not evidence; v1764d: stopped by hand at its start |

**Found (confirmed runtime):**
- **Difficulty:** on co-op saves the game runs `difficulty/preset/medium`, set by the co-op profile's campaign slot; on the lab's normal saves it runs `easy`.
  - Health hold refills health, so on Medium a big hit still killed the host in the Duke fight twice.
  - A joining partner's world takes its own profile's difficulty, not the host's (#177).
  - **Research (1214jt, [difficulty-override-v1881a](../../work/natural-moveset/difficulty-override-v1881a), confirmed runtime):**
    - The probe's `--difficulty` reads the difficulty manager embedded in the game at `+0x1f7b90`; its active
      `arkDeclDifficultyPreset` is at `+0x10` (`difficulty/preset/easy.difficultypreset` in the lab).
    - The probe's cvar reader also reports `ark_difficulty_current` (0 on Easy) and `ark_difficulty_level_override`
      (-1). Both are listed in the saved cvar references, and the game binary has the string "Difficulty decl not found".
    - The console command `ark_difficulty_level_override 2` was queued (engine log) but did not change the cvar
      (still -1). The preset stayed Easy, including after a mission load. The console's reply is not logged.
      Hypothesis: the cvar is protected (cheat or init-only).
    - Resolved through the d2-ghidra MCP and fixed in 1214kb: see [Difficulty sync](#difficulty-sync-177-1214kb) below.
- **Level-transition choice:** the Death to the Empress tower door offers Go / Stay on co-op saves. The choice is a pending switch to logic 9 while the logic stays 1; X closes it and A takes Go.
  - The user saw the harness sit on it ("leave or stay").
  - v1765 confirms it in the use step and in Settle.
- **Endings:** the stalled endings (Royal Conservatory v1762b, Grand Palace v1762a) were route-step misfires: the talk and use steps count any scene change as a hit. Both endings passed on the next attempts, as Meagan's endings did in four other missions.

## The campaign on 0.9.6 (1214kq)

The release binary of 0.9.6 (candidate 1214kq: power effects on any install,
[#230](https://github.com/adamkozel/Dishonored-2-coop/issues/230), [chapter 113](113-power-effects-on-every-install.md))
plays the nine missions on two Steam accounts. The host is Corvo on this PC, and the partner is Emily in the lab VM.
Both games run in product mode on co-op saves that already hold the autosaves of earlier takes, as a player's do.
- **Runner:** [Run-Campaign-v1929.sh](../../work/natural-moveset/Run-Campaign-v1929.sh) (plan campaign-v1926-plan.txt),
  with the retakes in [Run-Campaign-v1931.sh](../../work/natural-moveset/Run-Campaign-v1931.sh) (plan
  campaign-v1931-plan.txt). Both run the harness Route-MissionVM-v1918.
- **Added since 0.9.5:**
  - After Dunwall, each cast selects the power first. It must move the caster in its own world, move its
    avatar by the same distance in the other world, and show its native effect in the other world: the Blink burst,
    and the Far Reach beam.
  - Every mission after Dunwall has a cast pair where both players can move.
- **Every mission has one evidence take below.** Each one passed with the right characters and equal gameflow at every
  step. Both games ended on the next mission's map, or at the main menu after the credits, and restoration passed.

| Mission | Evidence take | Casts: host Blink / partner Far Reach (own world = other world; effect shown) | Story flags differing (start / end) |
|---|---|---|---|
| A Long Day in Dunwall (deaths, checkpoints) | [campaign-dunwall-vm-v1929a](../../work/natural-moveset/campaign-dunwall-vm-v1929a) | none: before the Outsider's Mark | 0 / 0 (31 and 26 flags) |
| Edge of the World (death) | [campaign-eotw-vm-v1931a](../../work/natural-moveset/campaign-eotw-vm-v1931a) | 4.78 m / 14.89 m, both shown | 0 / 0 |
| The Good Doctor (on Easy) | [campaign-institute-vm-v1929a](../../work/natural-moveset/campaign-institute-vm-v1929a) | 2.59 m / 2.25 m, both shown | 0 / 0 (25 and 104 flags) |
| The Clockwork Mansion | [campaign-madinventor-vm-v1929a](../../work/natural-moveset/campaign-madinventor-vm-v1929a) | 0.98 m / 0.90 m, both shown | 0 / 0 |
| The Royal Conservatory (on Easy) | [campaign-curator-vm-v1929a](../../work/natural-moveset/campaign-curator-vm-v1929a) | 4.70 m / 0.88 m, both shown | 0 / 0 |
| The Dust District | [campaign-dust-vm-v1931a](../../work/natural-moveset/campaign-dust-vm-v1931a) | 0.98 m / 0.36 m inside the Dreadful Wale, both shown | 0 / 0 |
| A Crack in the Slab | [campaign-bunker-vm-v1929a](../../work/natural-moveset/campaign-bunker-vm-v1929a) | 5.09 m / 14.68 m on the mansion grounds, both shown (the casts in the opening scene move no one) | 0 / 0 |
| The Grand Palace (on Easy) | [campaign-palace-vm-v1929a](../../work/natural-moveset/campaign-palace-vm-v1929a) | 3.04 m / 3.05 m, both shown | 0 / 0 |
| Death to the Empress, through the credits to the menu | [campaign-empress-vm-v1931a](../../work/natural-moveset/campaign-empress-vm-v1931a) | 0.93 m / 0.98 m inside the Dreadful Wale, both shown | 0 (140 flags) / none: the campaign ends at the main menu |

- **The Dreadful Wale:** the partner's Far Reach stops short inside the boat. Her casts there must move her 0.1 m
  (0.3 m elsewhere); the beam shows in the host's world either way.
- **Easy:** three routes play on Easy, set through the host's difficulty sync, because route combat there had killed
  the host through the health hold on normal (the watchtower, the journals, the palace).
- **Powers beyond the casts:** every power of both characters in both roles shows in the other world on two accounts
  ([chapter 113](113-power-effects-on-every-install.md)).
- **Failed takes on 1214kq, kept as evidence but not the mission's evidence take:**
  - [campaign-eotw-vm-v1929a](../../work/natural-moveset/campaign-eotw-vm-v1929a): the cable car did not reach the
    Institute (route variance). Its casts moved 4.71 m / 14.82 m with both effects shown.
  - [campaign-dust-vm-v1929a](../../work/natural-moveset/campaign-dust-vm-v1929a) and
    [campaign-empress-vm-v1929a](../../work/natural-moveset/campaign-empress-vm-v1929a): the partner's Far Reach moved
    0.21 m inside the Wale, below the 0.3 m then required. The beam was shown in both.
  - campaign-dunwall-vm-v1926a (1214kp): stopped in its first take to build 1214kq with the version 0.9.6; recovered.

## The campaign on 0.9.5 (1214km)

The release binary of 0.9.5 (candidate 1214km) plays the nine missions on two Steam accounts: the host is Corvo on this
PC, and the partner is Emily in the lab VM. Both games run in product mode on co-op saves, which already hold the
autosaves of earlier takes, as a player's do.
- **Runner:** [Run-Campaign-v1916.sh](../../work/natural-moveset/Run-Campaign-v1916.sh) on the harness
  Route-MissionVM-v1915.
- **Added since 0.9.4:**
  - Both players cast their power near the start of each mission from The Good Doctor on. The step records how far
    each caster moved in its own world and how far its avatar moved in the other world.
  - Both worlds' bool story flags are compared at every mission's start and end, for the story flag mirror of
    [#220](https://github.com/adamkozel/Dishonored-2-coop/issues/220).

| Mission | Passing take | Casts (host / partner, own world = other world) | Story flags differing (start / end) |
|---|---|---|---|
| A Long Day in Dunwall (deaths) | [campaign-dunwall-vm-v1916a](../../work/natural-moveset/campaign-dunwall-vm-v1916a) | before the powers | 0 / 0 |
| Edge of the World (death) | [campaign-eotw-vm-v1916a](../../work/natural-moveset/campaign-eotw-vm-v1916a) | before the powers | 0 / 0 |
| The Good Doctor (on Easy) | [campaign-institute-vm-v1916c](../../work/natural-moveset/campaign-institute-vm-v1916c) | Blink 3.57 m / Far Reach 14.35 m | 0 / 0 (25 and 104 flags) |
| The Clockwork Mansion | [campaign-madinventor-vm-v1916a](../../work/natural-moveset/campaign-madinventor-vm-v1916a) | Blink 2.26 m / Far Reach 2.27 m | 0 / 0 |
| The Royal Conservatory | [campaign-curator-vm-v1916b](../../work/natural-moveset/campaign-curator-vm-v1916b) | none: both stand still in the Dreadful Wale's arrival scene | 0 / 0 |
| The Dust District | [campaign-dust-vm-v1916a](../../work/natural-moveset/campaign-dust-vm-v1916a) | 2.26 m / 0.43 m | 0 / 0 |
| A Crack in the Slab | [campaign-bunker-vm-v1916a](../../work/natural-moveset/campaign-bunker-vm-v1916a) | none: both stand still in the opening scene | 0 / 0 |
| The Grand Palace (on Easy) | [campaign-palace-vm-v1916a](../../work/natural-moveset/campaign-palace-vm-v1916a) | none: both stand still in the Dreadful Wale's opening scene | 0 / 0 |
| Death to the Empress, through the credits to the menu | [campaign-empress-vm-v1916a](../../work/natural-moveset/campaign-empress-vm-v1916a) | 2.28 m / 0.39 m | 0 (140 flags) / none: the campaign ends at the main menu, where no world is left to read |

- **The Good Doctor:**
  - [campaign-institute-vm-v1915a](../../work/natural-moveset/campaign-institute-vm-v1915a) (1214kl) passed: Blink 3.55 m and Far Reach 14.59 m; 25 and 104 flags,
    none differing.
  - On 1214km, [campaign-institute-vm-v1916a](../../work/natural-moveset/campaign-institute-vm-v1916a) and [campaign-institute-vm-v1916b](../../work/natural-moveset/campaign-institute-vm-v1916b) failed in route
    combat. Their casts moved 3.58 m / 14.35 m and 3.63 m / 13.89 m.
    - In v1916a, the clear before the whale oil tank spent its presses on a blocking elite, and the guard it never
      reached killed the host.
    - In v1916b, three watchtower guards killed the host through the health hold, which is not god mode.
  - [campaign-institute-vm-v1916c](../../work/natural-moveset/campaign-institute-vm-v1916c) plays the mission on Easy through the host's difficulty sync, as the Grand Palace route does
    ([routes/campaign-institute-vm-v1916c.json](../../work/natural-moveset/routes/campaign-institute-vm-v1916c.json)).
- **Other failed takes, kept:**
  - [campaign-curator-vm-v1916a](../../work/natural-moveset/campaign-curator-vm-v1916a): a witch held the host in a sword clash at the archived journals after
    Breanna died. The retake route clears witches and gravehounds first.
  - On 1214kl: [campaign-dunwall-vm-v1915a](../../work/natural-moveset/campaign-dunwall-vm-v1915a) lost the host's autosave
    ([#225](https://github.com/adamkozel/Dishonored-2-coop/issues/225), [chapter 88](88-mission-shared-death-and-checkpoint.md)).
  - [campaign-eotw-vm-v1915a](../../work/natural-moveset/campaign-eotw-vm-v1915a) failed in the harness: its console closed when the runner's wrapper was stopped.
- **Powers:** a cast moves the caster in both worlds by the same distance in four missions (The Good Doctor, The
  Clockwork Mansion, The Dust District, Death to the Empress).
  - The partner's Far Reach moves her less where it stops short on the Dreadful Wale.
  - Three missions start in a scene where neither player can move, so their casts at the start move no one.
- **Story flags:** in every mission both worlds held the same decisions at the start and, until the credits, at the
  end ([#220](https://github.com/adamkozel/Dishonored-2-coop/issues/220)).
- **Every passing take:** the right characters, equal gameflow at every step, both games on the next mission's map (the
  main menu after the credits), and restoration passed.

## The campaign on 0.9.4 (1214kf)

The release binary of 0.9.4 (candidate 1214kf: the partner's combat choke of #209, story characters agree between
worlds) plays the nine missions on two Steam accounts: the host is Corvo on this PC, and the partner is Emily in the lab
VM. Both games run in product mode on co-op saves. The runner is
[Run-Campaign-v1906.sh](../../work/natural-moveset/Run-Campaign-v1906.sh) on the harness Route-MissionVM-v1891, with the
0.9.3 routes; the Grand Palace runs on Easy, as in campaign-palace-vm-v1890a.

| Mission | Passing take |
|---|---|
| A Long Day in Dunwall (deaths) | [campaign-dunwall-vm-v1906a](../../work/natural-moveset/campaign-dunwall-vm-v1906a) |
| Edge of the World (death) | [campaign-eotw-vm-v1906a](../../work/natural-moveset/campaign-eotw-vm-v1906a) |
| The Good Doctor | [campaign-institute-vm-v1906a](../../work/natural-moveset/campaign-institute-vm-v1906a) |
| The Clockwork Mansion | [campaign-madinventor-vm-v1906a](../../work/natural-moveset/campaign-madinventor-vm-v1906a) |
| The Royal Conservatory | [campaign-curator-vm-v1906a](../../work/natural-moveset/campaign-curator-vm-v1906a) |
| The Dust District | [campaign-dust-vm-v1906a](../../work/natural-moveset/campaign-dust-vm-v1906a) |
| A Crack in the Slab | [campaign-bunker-vm-v1906a](../../work/natural-moveset/campaign-bunker-vm-v1906a) |
| The Grand Palace (on Easy) | [campaign-palace-vm-v1906a](../../work/natural-moveset/campaign-palace-vm-v1906a) |
| Death to the Empress, through the credits to the menu | [campaign-empress-vm-v1906a](../../work/natural-moveset/campaign-empress-vm-v1906a) |

- **All nine passed on the first attempt**, from 20:28 to 00:05 (about 24 minutes per mission).
- **Every take:**
  - the right characters;
  - equal gameflow in both worlds at every step and at the end;
  - both games on the next mission's map (the main menu after the credits);
  - restoration passed.

## The campaign on 0.9.3 (#213, 1214kb)

The release binary of 0.9.3 (candidate 1214kb, with the difficulty sync of #177) plays the nine missions on two Steam
accounts: the host is Corvo on this PC, and the partner is Emily in the lab VM. Both games are in product mode
(`coop_product_auto`) on co-op saves. The runner is
[Run-Campaign-v1889.sh](../../work/natural-moveset/Run-Campaign-v1889.sh), with the harness Route-MissionVM-v1889 and
each mission's route from its last passing take; re-runs use Run-Campaign-v1889b.sh with a plan file.

| Mission | Passing take | Earlier attempts |
|---|---|---|
| A Long Day in Dunwall (deaths) | [campaign-dunwall-vm-v1889a](../../work/natural-moveset/campaign-dunwall-vm-v1889a) | |
| Edge of the World (death) | [campaign-eotw-vm-v1889a](../../work/natural-moveset/campaign-eotw-vm-v1889a) | |
| The Good Doctor | [campaign-institute-vm-v1889a](../../work/natural-moveset/campaign-institute-vm-v1889a) | |
| The Clockwork Mansion | [campaign-madinventor-vm-v1889a](../../work/natural-moveset/campaign-madinventor-vm-v1889a) | |
| The Royal Conservatory | [campaign-curator-vm-v1889b](../../work/natural-moveset/campaign-curator-vm-v1889b) | v1889a: the archived-journals note use did not register (the host's focus never reached the note), as in v1750a and v1762a |
| The Dust District | [campaign-dust-vm-v1889a](../../work/natural-moveset/campaign-dust-vm-v1889a) | |
| A Crack in the Slab | [campaign-bunker-vm-v1889a](../../work/natural-moveset/campaign-bunker-vm-v1889a) | |
| The Grand Palace | [campaign-palace-vm-v1890a](../../work/natural-moveset/campaign-palace-vm-v1890a), on Easy (see below) | v1889a: the closing talk with Meagan did not start the mission end, as in v1762a. v1889b and v1889c: the host died in the Duke fight; both worlds went to GameOver together. v1889d (Easy): the Duke died, then the attack step re-picked the nearest luca_abele by name and teleported the host into the guards (fixed in Route-MissionVM-v1890) |
| Death to the Empress | [campaign-empress-vm-v1889a](../../work/natural-moveset/campaign-empress-vm-v1889a), through the credits to the menu | |

- **Every passing take:**
  - the right characters (host Corvo, partner Emily);
  - equal gameflow in both worlds at every step;
  - both games end on the next mission's map;
  - restoration passed.
- **Every failure was the route or combat, not a divergence:**
  - two route-step misfires (a use and a talk that did not register);
  - one host death in the Duke fight, where both worlds went to GameOver together (the shared death works as designed).
- **The first run (campaign-*-vm-v1888a) failed before playing.** Its runner read the CRLF plan file's carriage return
  into every steps path; Run-Campaign-v1889.sh strips it.
- **The Grand Palace on Easy:** the route sets the host's difficulty to Easy with the native setter (`coop_difficulty_set 0`), and back to Medium at the end. The partner follows both changes through the difficulty sync (#177): both worlds ran the Easy preset through the mission and its level transitions, then Medium (`step03-difficulty`, `step26-difficulty`). On Medium a big hit in the Duke fight gets through the lab's health hold. Difficulty is a player's choice, and the other eight missions ran on Medium.
- **Harness:** [Route-MissionVM-v1890](../../work/natural-moveset/Route-MissionVM-v1890.ps1) keeps the attacked NPC by its handle. [v1891](../../work/natural-moveset/Route-MissionVM-v1891.ps1) also stops the lab's idle local partner instance during two-account takes, since it rendered beside the host with about as much CPU as the host game.

## Difficulty sync (#177, 1214kb)

**Native path (d2-ghidra, confirmed static):**
- The difficulty manager is embedded in the game object at `+0x1f7b90`:
  - dword `+0` is 1 once it is initialized;
  - `+4` is the preset index: 0 Easy, 1 Medium, 2 Hard, 3 Very Hard, 4 Custom;
  - `+8` is the `arkDeclDifficulty`, and `+0x10` the active preset.
- `FUN_140648f20(manager, index)` is the native setter. It:
  1. removes every registered entity's difficulty modifiers (`FUN_1406491f0`);
  2. takes preset `[index]` of the decl. An index past the decl's presets takes the static custom preset at `0x1439889f0`, whose fields `+0x68..+0xc4` are the 22 custom values;
  3. re-applies the modifiers (`FUN_14060b970`);
  4. sets `+4` and the cvar `ark_difficulty_current`;
  5. writes the profile keys of the current campaign slot (`FUN_1406492c0`, under the profile write lock): `difficultyPresetIndex_%02d`, and for Custom the custom values (`difficultyEnemyDamage`, `difficultyEnemyPerception` and so on).
- Its only caller is the profile load `FUN_140648590`. That reads the slot's keys, or takes the one-shot override `ark_difficulty_level_override` (it resets it to -1), then calls the setter.
  - This explains difficulty-override-v1881a: the override is consumed only by that load.
- `difficultySaveModel` (`0x143988aa4`) is written for every preset; the sync leaves it alone.
- The setter's profile write lands in the partner's co-op profile (`savegame_coop/profile.bin`, chapter 107). Her single-player campaigns keep their own difficulty.

**1214kb (`difficulty_sync.h`):**
- The host publishes its index, and for Custom the 22 values, in a new packet `D2DF` (120 bytes), every 2 s and on a change.
- The partner calls the setter on the engine thread in gameplay when her index or custom values differ, at most every 5 s. For Custom she first writes the host's values into the static preset.
- Status: `coop-status.json.difficulty.json`.
- Lab commands: `coop_difficulty_sync on|off`, and `coop_difficulty_set <0-4>`, which calls the same setter in the local game.
- The harness Route-MissionVM-v1889 adds a `difficulty` step. It keeps both games' status and the probe's `--difficulty` preset, and with `expect` it waits until both games run that index with the same preset.

**Confirmed runtime** ([difficulty-sync-vm-v1889a](../../work/natural-moveset/difficulty-sync-vm-v1889a), two accounts, both on co-op saves, Dunwall tower), pass:

| Step | Host | Partner |
|---|---|---|
| Start | Medium | Medium (matched, nothing applied) |
| Partner sync off, partner set to Easy | Medium | Easy |
| Partner sync on | Medium | Medium (applied) |
| Host set to Hard | Hard | Hard (applied) |
| Host set to Custom | Custom (the static preset `+0x39889f0`) | Custom, the same values (applied) |
| Host set back to Medium | Medium | Medium (applied) |

- The partner applied 4 times with no failures. Both games ended on Medium, as they started, and restoration passed.
- Hypothesis: the host's options menu changes the index through the same profile load, so the partner follows it too. That path was not driven in this take.
