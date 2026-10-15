# Stage 3: native use interactions, and the window, counter and knockdown investigations (candidates 1213–1213b)

Updated 2026-10-14.
- Audits: [use-interaction-audit-v1215](../../work/movement-integration/use-interaction-audit-v1215.json), [window-traversal-audit-v1207](../../work/movement-integration/window-traversal-audit-v1207.json), [native-combat-use-observations-v1211](../../work/movement-integration/native-combat-use-observations-v1211.json).

## Native use → authored interaction (`lever_button_crank_wheel`, `key_use`)

**Native source (confirmed runtime, 1210c).**
- A successful native use enters player state 75 (vtable `0x1c86ab8`) for about one engine frame. This was observed on both the open and the close of the Dunwall window handle.
- The native player animation library has no first-person lever, button, crank, wheel or key animation. So the use event itself is the semantic source, and the partner body needs an authored reach-and-operate clip.

**Candidate 1213, refined in 1213b.**
- **Source.** The rising edge of state 75 marks a use. The focused entity comes from the interaction sensor (`arkCpntPlayerSensorInteraction`, vtable `0x1b403b8`, `+0x80` high dword = focused handle).
  - The entity name picks the variant: 1 button or generic reach, 2 lever, 3 crank, 4 wheel or valve, 5 key or lock.
  - The use ends on any other action, or when the player moves more than 0.6 m.
  - 1213b samples the edge every engine frame. 1213 sampled only at the 33 ms packet cadence and missed uses (1214b).
- **Wire protocol 74** (416 bytes) adds `useVariant`, `useTimeMs`, `useDurationMs` and `useEvent`. Its own use event counter keeps the timeline valid inside the unchanged ground action. A use is accepted only on the ground action, never alongside another scene. Unit tests cover durations, clocks, incarnations, overlap refusal and name classification.
- **Bank 71/374** ([manifest](../../work/natural-moveset/use-bank-manifest-v1212.json), [builder](../../work/natural-moveset/build_use_bank_v1212.py)) appends the authored `mechanisms-v4` sequences as clips 369–373 for both rigs. The prior 369 payloads are byte-identical.
- **Playback.** The partner body blends the clip on its authored duration. It fades out as soon as the partner moves; the root stays network-owned.

**Runtime.**
- **1214c (pass, both roles).** In the Dunwall fixture with the product controller armed, each player's native use of the window handle was detected at its source. The partner's remote body played the authored generic interaction.
  - The client's use: remote Emily on the host played the full clip at weight 1.0. **Visual accepted**: she steps to the window and extends her arm as it opens.
  - The host's use: remote Corvo on the client ended at 24% of the clip. The source ended the use early, for a reason not yet isolated.
- **1214 / 1214b (1213, failed).** The host's use passed in 1214 (Corvo leans into the window; backlit). The client's uses were missed by the 33 ms sampling.

**Vise, both roles (1213f, takes 1226–1227b, #3):**
- **Cause of the 1218d failure:** the host's vise use never registered because the native interaction sensor wasn't focused when Use was pressed. The video shows Corvo beside the bench with no interaction.
- **New probe mode `d2_runtime_probe --focus` (1226):** reads the sensor exactly as the mod does, `arkCpntPlayerSensorInteraction` +0x80. On the Dunwall window it named the window's handle sub-entity `ld_rich_small_gp_window_handle_37`, not the window root: focus targets handle entities.
- **v1227b:** before each Use press, the harness searches stand-off side, yaw and pitch until `--focus` names a `vise` handle.
  - Host: focused on the first pose.
  - Client: focused on try 11 at yaw +12°, because the partner's body stood on the shared spot.
  - Both roles registered the use with variant 3 (crank), as expected. The partner presented it at weight 1.0 for 60 and 65 frames, playing to 100% and ending by duration.
  - In both roles the first focused press did not register and the second did; the harness retries.
- The take is recorded as failed only because the optional turbine-lever case afterwards threw "Observer pitch did not settle" (#4).

**Open:**
- Lever, wheel and key variants are untested on real mechanisms. Crank/vise passes (1227b).
- The early end of the host-source use (#5): not reproduced on the vise, which plays to 100%. Re-check on the window in the #4 run.
- Hand targeting to the real handle geometry (the clips are stand-ins).

## Window traversal (`window_enter_exit`, audit 1207)

- 1201 (Dunwall Tower window) and 1206 (Edge of the World docks apartment): both catalogued window usables opened natively, but neither is climbable.
  - The tower window is a chest-height opening over a sheer drop: jumping toward it only bumps the frame, and no mantle triggers.
  - The docks one is a venetian-blind toggle.
- A Ghidra string census found no window-specific player traversal state. Window entry and exit is the generic native mantle (state 8, confined/low variants), already partially bound.
- Remaining: record that mantle binding through a genuinely climbable window.

## Counterattack and knockdown (audit 1211)

- **1208 / 1208b.** Host Corvo in the Dunwall arena recorded block and timed-parry scenes. One timed parry occurred (the attacker was left off-balance). The follow-up attack collided with the block driver's input (two input owners), so no follow-up scene was captured.
  - The player's parry-win chance is 0% in this save, so a parry never knocks the enemy down. The native finisher on a knocked-down enemy is `fatality_knockdown`.
- **1209.** An attack spree recorded ten native slashes and focus scenes, but no player knockdown before the health safety stop (three attackers, 7.3 s).
  - The native Corvo knockdown assets exist: `ply_body_generic_knockdown_fall`, `_front`, `_land` and `_recovery`. They come from the `PLAYER_HITREACT_KNOCKDOWN` blowback.
  - Provoking one needs a guard parry-win against the player.
