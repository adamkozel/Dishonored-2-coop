# 110. Mission decisions in both worlds (#184)

Decisions carry across missions through two things: the gameflow tasks (each target has a Kill task and a NonLethal
task) and the **story flags**. Labels follow the handoff rules: confirmed static, confirmed runtime or hypothesis.

## Story flags (confirmed runtime)

- **What they are:** bool variables of the `storyflags/*.kiscule` script instances on the worldspawn
  (`tools/diag/d2_kiscule_vars.py <pid> storyflags`).
- **Where they come from:** each map loads `global.kiscule`, its own declaration and the earlier missions' declarations
  (Death to the Empress's tower: 12 declarations, 110 flags). They travel with the save, and the next mission reads
  them, for example to choose `dust_return_p` when Stilton was saved.
- **Kinds** (124 flags across the campaign, `story-flags-v1836a`):
  - **Alive flags** for every target and side character (`ramsey_is_alive`, `hypatia_is_alive`, `jindosh_is_alive`,
    `breanna_is_alive`, `stilton_is_alive`, `duke_is_alive`, `body_double_is_alive`, `delilah_is_alive`, Meagan,
    Sokolov, Mindy, Paolo, Byrne, Lucia, the printer, the shopkeepers).
  - **Non-lethal outcomes:** `ramsey_in_vault`, `hypatia_is_cured` and `hypatia_is_transfomed`, `jindosh_neutralized`,
    `breanna_is_powerless`, `stilton_is_crazy` and `stilton_is_incapacited`, `delilah_is_nl`.
  - **Robberies:** the black markets (`blackmarket_0N_robbed`, `bm_shops_robbed`) and `galvany_is_robbed`.
  - **Other:** `player_got_dukeplans`, `sokolov_carried`, `sokolov_with_meagan`, Meagan's lines.
- **Tool:** `tools/diag/d2_storyflags_diff.py` compares the two lab worlds' flags and writes only the differences.
  Bytes 8 and 9 of a bool are its default and value; the rest of the variable is padding.

## Who decides

| Decision | Who decides | Carried by |
|---|---|---|
| A target's or a character's death (alive flags) | whoever kills | NPC outcome sync (chapter 95): the host's own scripts then set the flag |
| A non-lethal outcome | whoever completes it | the body (carried-body sync), the place (trigger mirror), the device (shared usables, #187) and the story grants (chapter 97, 1214jc) |
| Chaos | the host | 1214ba |
| The Outsider's offer | each player | 1214bk (chapter 97) |
| Map variants (`dust_return_p` and others) | the host's world | map changes follow the host (chapter 97); they read the host's flags |
| Combination dials and the Jindosh lock riddle | each world | per world, decided (chapter 106); `EnigmaExpectedStage*` differ by design |

## Runtime

- **Every mission map loads with equal decision flags in both worlds** (story-flags-v1836a). Both lab games loaded
  the nine missions' maps with the native `campaign` command. The only differences are the Jindosh lock riddle's two
  `EnigmaExpectedStage*` values from Dust District on, which the game scrambles per world (non-bool, pointer-like bytes).
- **Her kill of Ramsey sets the host's flag** (route-dunwall-lead-v1835a). In a partner-led Dunwall (steps 1-15 of the
  v1674 route, held at the end), she kills and loots Ramsey, opens the vault and her locker. Both worlds then hold
  `ramsey_is_alive` 0, and the 22 flags show no difference (`storyflags-diff.json`, `storyflags-values.json`).

**Open (#197):** the non-lethal outcomes end to end, with the partner completing them. Each needs its mission's
non-lethal sequence (the Ramsey vault, the Hypatia cure, the Jindosh shock, the Breanna shrine, the Duke's double,
Stilton, Delilah). The campaign routes take the lethal branches. The final two-account campaign validation will take
non-lethal branches with the partner acting, and diff the story flags at each mission's end.
