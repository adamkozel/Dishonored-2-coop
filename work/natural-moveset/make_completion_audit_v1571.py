"""Writes completion-audit-v1571-window.json (#11 wall_and_window_variants): the native window takedowns (choke_window_*,
assassination_window_*) in both roles with the victim streamed to the partner; the Shadow Walk wall chokes cannot occur
in retail play. Takes and verdicts are filled from the coverage chapter (99) rows 1545-1570."""
import json, sys
from pathlib import Path
N = Path(__file__).resolve().parent
CORVO = json.loads(sys.argv[1]) if len(sys.argv) > 1 else None  # filled once 1569/1570 are reviewed
M = ("Window takedowns are native: the player's window sensor (arkCpntPlayerSensorWindowAssassination, read every loop with "
     "probe --window-sensor) must hold the target, a window point, a sill and the target within the angle of the wall normal "
     "(chapter 100), then the choke rule handler (isBehindWindow plus the 4-way direction) or the window assassination rule "
     "handler (unaware, direction, height difference at most 0.45 m) picks choke_window_* or assassination_window_*. The "
     "fixture (harnesses 1545-1569) places the Edge of the World square overseer outside the sash window "
     "script_rich_small_01_sndctrl, stands the source inside, turns it through headings until the sensor is ready and "
     "presses at once, with the outside camera on the partner. The partner presents the source's native scene with the "
     "authored clip of its kind (choke entry, loop and win; lethal) on the native scene clock, as for every takedown (audits "
     "v1354, v1369), and the victim follows the source's paired scene by the NPC stream (the host's own victim at the "
     "priority rate; a client's victim by reverse poses). Outcomes are checked in both worlds. Direction follows the "
     "overseer's facing: back (both kinds, both roles) and left (choke) ran; with the overseer facing the window he saw "
     "the source at the sill and fought (1564a/b), so the front variants need an unaware NPC facing a window, which the "
     "lethal rule requires and the fixture cannot keep. Wall variants: choke_sw_wall_* needs Shadow Walk's hide-on-wall "
     "mode, gated by ark_shadow_walk_hiding_enabled, which is 0 in retail and set by no config (chapter 100, 1469a); they "
     "are not applicable.")
print('draft only')
