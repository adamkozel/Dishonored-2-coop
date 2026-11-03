# The native character choice in the first mission (stage 8 step 5, #243)

Read-only d2-ghidra research, 2026-11-02. Confirmed static unless marked.

## The Kiscule node `arkKisculeNodeActionPlayerCharacter`

- **Classes:** node vtable 0x141b7dae8, parameters vtable 0x141b7daa8.
- **Pins:** Corvo → `SwitchCharacter(game, 1)` (0x140533fd0); Emily → `SwitchCharacter(game, 2)` (0x140533ff0); Start → 0x140534010.
- **Start:**
  - It starts the choice scene and requests player state 37 (`FUN_140806020(sm, 6, 0x25, ...)`).
  - Nothing remembered or preset skips it: the only check is "state 37 already active".
- **Its event handler (0x140533c80)** maps the scene's events:
  - "Ev Emily Chosen" → `SwitchCharacter(game, 2)`, and "Ev Corvo Chosen" → `SwitchCharacter(game, 1)`;
  - "Ev Inputs Off" and "Ev Inputs On" write state 37's input byte +0x58.
- **`arkPlayerCharacters_t`:** 1 = Corvo, 2 = Emily.
- **`D2_Game_SwitchCharacter` (0x1408a7650)** does nothing when `settings(game+0x1e8948)+0x4c0` already holds the value.

## Logic state 14, `arkGameLogicStateCharacterChoice`

- **The instance:** vtable 0x141b698d8, live at `*(game+0x1f6f50)` and created per map.
- **Fields:**
  - +0x48: the selection (`arkCharacterChoice_t`: **0 = Emily, 1 = Corvo**);
  - +0x110: the open box handle (-1 when none);
  - +0x114: the confirmed flag.
- **Player state 37** (`arkPlayerStateCharacterChoice`, vtable 0x141c3aaa8):
  - On enter it pushes logic 14, sets the selection to 0 (Emily), and sets the input byte +0x58 to 1.
  - Each update while +0x58 == 1, the first case that applies:
    - +0x114 set: it plays the chosen ending through the scene variable (3 Emily, 2 Corvo) and hides the surfaces;
    - left or right input: it moves the selection;
    - the confirm input: it opens the "Play as …?" box (logic 3).
- **The box callback** (0x1404f0460) writes +0x110 = -1 and +0x114 = (result != 0).

## The fix

On the partner only, with logic 14 active, the instance's vtable matching, no box open (+0x110 == -1) and not yet confirmed, write the selection (+0x48) and then +0x114 = 1. The native update plays the chosen ending, and the node calls `SwitchCharacter` itself. Afterwards, check `settings+0x4c0` and fall back to `SwitchCharacter(game, pick)`; it is idempotent.

**Timing (hypothesis):** write only after the scene's "Inputs On", so after a short settle once logic 14 appears. The host keeps its own native screen; the node is allow-local.
