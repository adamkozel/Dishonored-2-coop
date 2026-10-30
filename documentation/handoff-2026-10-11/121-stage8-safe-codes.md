# 121. Stage 8 area 4.3: one safe code for both players (in progress)

Card #239. Before this, every safe in the two worlds had its own code, its own note text and its own dial start positions. That gave the safe-dial differences in every two-account world diff ([chapter 119](119-stage8-host-story.md), [chapter 120](120-stage8-items-per-player.md)). The research is in [the safe code map](stage8-step1-notes/safe-code-map.md).

## How it works (candidate safecodes-v2039, `safe_code_mirror.h`)

Every code is a Kiscule int, rolled by a script in each world. It is either the `Combination` of the `3_digits_lock.kiscule` decl nested in the safe's entity instance, or a level script that writes that `Combination`. No native safe code exists (confirmed static).

- **Host** (in plain gameplay, 3 s after a map load, read every 2 s): it sends a D2SC message when anything changed, and at least every 6 s. The message holds:
  - each lock's `Combination`, `Position` and `PositionDigit1-3`, read as ints at +0x1c of the int variable (vtable RVA 0x1c9f998);
  - each safe dial's state (a 10-state usable whose name holds `combinaison_numbers`);
  - the journal's three-digit formatted lists (the note and clue texts of safe codes).
- **Partner** (same map, plain gameplay):
  - it writes the host's `Combination` into its own lock and into the safe entity's `Combination` whenever they differ. This is a raw write, so no Updated event fires and nothing rerolls;
  - once per map load, it writes the host's `Position` and digits, and puts each dial on the host's state with the quiet state apply `0x14077a880(component, state, 0, 1)`, which fires no script event;
  - it overwrites in place each journal digit list whose text id and length match its own. It never calls the formatter with a foreign id, because the formatter stops the game on an unknown one.
- **After the start**, each player turns their own dials and opens their own safe. The contents stay pickups, per player ([chapter 120](120-stage8-items-per-player.md)).
- **Switches:**
  - Lab: `coop_safe_codes on|off`, on by default.
  - Product: `d2coop.cfg` `safe_codes=0` sets `D2COOP_SAFE_CODES=0`.
  - A host with it off sends nothing, so each world keeps its own codes, as in 0.9.6.
- **Status:** `<status>.safe-codes.json`. It has the counters plus `locksDetail` (entity hash, `Combination` and `Position` of this world's locks).

## Results

All on The Good Doctor (`campaign/institute/institute.mapinfo`), two lab games, `Test-SafeCodes-v2037.ps1` / `v2039.ps1`.

| Take | Candidate | Result |
|---|---|---|
| `safe-codes-v2037a` | safecodes-v2037 | Failed in the harness: no `.safe-codes.json`. The writer was only in the product status rotation, and the lab runs the diagnostics rotation. v2038 also writes it there |
| `safe-codes-v2038a` | safecodes-v2038 | **Pass.** The host found 2 locks, 6 dials and 4 texts. The partner holds the host's codes (183 and 15) and positions: 4 code writes, 8 position writes, 6 dial writes, 10 journal writes, nothing unknown. World diff equal (2158). Flaw: the host sent once, and the partner fell back to `waiting_host` after 15 s, because one clock served both the 2 s read and the 6 s resend |
| `safe-codes-v2038b` | safecodes-v2038 | **Mutation twin, red as required.** With `coop_safe_codes off` on the host, all 6 safe dials differ (`ld_combinaison_numbers`, `_15` to `_19`) |
| `safe-codes-v2039a` | safecodes-v2039 | **Pass** with the stricter test (the host keeps publishing, and the partner ends in `applied`). Host sent 8; partner applied, with nothing unknown; world diff equal (2103) |

The int value at +0x1c and the journal layout (`+0xd8` lists parallel to the decl's text ids) are now confirmed runtime by these takes: the partner's reads and writes agree with the host's values, and the world diff is equal.

## The campaign on two accounts (v2040a)

`Run-Campaign-v2040.sh a`: all nine missions, harness `Route-MissionVM-v2016`, candidate safecodes-v2039 (host story, items per player and safe codes on), host Corvo on this PC, VM partner Emily. Results in `work/natural-moveset/campaign-<mission>-vm-ipp-v2040a/`.

| Mission | Result |
|---|---|
| Dunwall | **Equal** (286). The take sat 1 h 45 min at one screenshot: the host game window was minimized, and gfxcapture gets no frame from a minimized window, so the single-frame ffmpeg grab never ended (confirmed runtime: grabs resumed once the window was restored). Harness v2017 kills a grab after 15 s and restores a minimized window first |
| Edge of the World | **Equal** (2103). In v2036a the safe dials differed; the safe codes fix it (confirmed runtime) |
| The Good Doctor | 2 values differ: `b_hyp_arrived_lab_graduated_cylinder_01` and `b_hyp_arrived_lab_retort_01` (host 0, partner absent), the same lab glass as v2036a |
| The Clockwork Mansion | Partner GameOver "You have lost a critical ally: Anton Sokolov" on the streets after the host carried him out; the host showed the copied plain GameOver. The streets map has its own Sokolov-loss script (`script_kiscule_gameover`, confirmed static), and the partner ran its `ActionGameOver` itself although it is a World class. The route fixes of v2038 and v2016 worked: the run passed Jindosh, the carry and the exterior door. Candidate gameover-v2041 blocks the partner's own `ActionGameOver` under host story (below) |
| The Royal Conservatory | The host died ("You Have Perished") at the last audiograph: the route switches the health hold off before the exit use while an enemy was still near. Route v2041 keeps the hold on until `boat_dust` has loaded. Not a sync fault |
| Dust District | **Equal** (2325). Differed in v2036a; the safe codes fix it (confirmed runtime) |
| A Crack in the Slab | The host's co-op saves switch stayed in `profile_saving` (phase 2, no switch), the second time after palace v2036a. Under study |
| Death to the Duke | **Equal** (608). Failed in v2036a and v2036b (skiff scene) |
| Death to the Empress | The host loaded `campaign/end/end_p` alone: its transition hold timed out after 90 s, and the partner stayed on `dunwall_return_tower_p`. Under study |

Four missions are now equal that the 0.9.6 rules also passed, plus Edge of the World and Dust District, which needed the safe codes.

**Candidate gameover-v2041** (`kiscule_action_mirror.h`): under host story the partner never runs its own `ActionGameOver` node (vtable RVA 0x1b79cc8). It reaches GameOver only from the host's `Death` message (`checkpoint_sync.h`), so the host decides. Counted as `hostStoryGameOverBlocked` in `<status>.kiscule.json`. What harmed the partner's Sokolov is still a hypothesis (his spawn is a replay of the host's run, out of the partner's own flow order). `Run-Campaign-v2041.sh a` reruns The Clockwork Mansion, Dunwall, The Royal Conservatory and A Crack in the Slab on it with harness v2017.

## Still to do in area 4.3

- **The campaign on two accounts:** the v2041 reruns, then The Good Doctor glass and the Death to the Empress ending.
- **Note text on screen:** check that the partner's note shows the host's digits. This is visual acceptance, recorded separately.
- **A partner opens a safe with the host's code** (a route step on Edge of the World).
- **Codes outside `3_digits_lock`:** the Jindosh lock (chapter 110) and other lock decls, if any.
- **Safe-room doors** (result-level door sync) and the door mutation twin.
- The 1.0 `d2coop.cfg` and README document `safe_codes=0`.
