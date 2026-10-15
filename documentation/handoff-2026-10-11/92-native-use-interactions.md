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

**Real mechanisms for every variant (#4, confirmed static, [resource reader](../../tools/diag/d2_resource_index.py)):**
- Each usable's entityDef carries a native interaction name:

  | Native name | Examples | Variant |
  |---|---|---|
  | `Usable_lever` | `lever_01`, `lever_template` | 2 lever |
  | `Usable_vise`, `Usable_crank` | `vise_01`; `valve_set_01` (a turned valve wheel) | 3 crank; 4 by its `valve` name |
  | `Usable_tap` | `rich_sink_tap_01`, dissection-table `tap` | 4 wheel/valve |
  | `Usable_lock` | `ring_locker_01` | 5 key/lock |
  | `Usable_latch` | `door_latch_01` | 5 key/lock |
  | `Usable_redbutton` | `button_01` | 1 |

- **Dunwall fixture** (`dunwall_escape_tower_p`): ring lockers `ld_ring_locker_01_{1,2}_{lock,open}` and `_3`, about 22 m from the window; a sink tap `esc_env_rich_sink_tap_01_1`.
- **Edge of the World:** the turbine lever; sink taps and dissection-table taps; three door latches. No valve wheels.
- **Elsewhere:** a real `wheel_01` (`ld_wheel_01_1_1`) is in the Royal Conservatory (`curator`). Valve sets and levers are in A Crack in the Slab (`bunker`) and the Clockwork Mansion (`madinventor`).
- **Candidate 1213g** classifies whole-token `tap` and `faucet` as 4 and `latch` as 5 (unit-tested), and deploys the `--focus` probe.
- **Harness `Test-UseVariants-v1228`** (`-Mission dunwall|eotw`): the source aims its native eye at the target point (`Set-NativeInteractionAim-v691`) on four sides at 1.0 m and 0.75 m, then sweeps pitch. No aim or pitch failure can throw. Each case records pass or fail, and the required cases decide the take.

**Dunwall results (1213g, takes 1228 and 1228b, confirmed runtime):**
- **Lock (ring locker), both roles, both takes:** the native use registered and was classified 5. The focus named the co-located twin `ld_ring_locker_01_1_open`. The partner played the key/lock clip at weight 1.0 to 100%, ending by duration.
- **Tap:**
  - client: 1228 and 1228b, variant 4, 80 frames, to 100%;
  - host: failed in 1228 (focused, but no use registered), passed in 1228b (74 frames, to 100%).
- **Harness press bug (confirmed static):** `Send-D2Input` publishes an expiring input packet and returns at once. `X 600` followed directly by Neutral therefore held the press for only about 50–150 ms, so the first press often did not register (1227b, 1228). 1228b holds the press (X 400, wait, Neutral), and every use registered on the first press.
- **Window (#5), both roles in 1228:** the use ended by movement (`endReason` 1) after 0.5–0.8 s.
  - 1228b sampled the source's position. The source had been placed on something about 0.57 m above the floor in front of the window, and the opening sash dropped it 0.46 m with 0.36 m of horizontal travel. The 3D distance of 0.668 m crossed the 0.6 m "walked away" rule.
  - **Candidate 1213h:** a use ends only on more than 0.6 m of horizontal travel (`useMovedAway`, unit-tested with the measured numbers).
- **Visual:** not accepted. The observer was too close or occluded (candelabra), and in the 1228 tap case it stood behind a wall. v1228c films from the source's own open approach side, 3.4 m from the target.

**Edge of the World (1213h, takes 1228c–1228e):**
- **1228c, stopped by Claude:** teleporting to the wind-turbine lever raised a native tutorial modal (game logic 12) on both peers. Look input was ignored, so the seek could not succeed.
  - Recovery: the fixture was restored, but a stuck client fixture isolation refused the note restore. A `Deploy-SourceClock` relaunch (protected saves unchanged) and a note restore completed it.
  - From 1228d on, `ClearModals` dismisses logic 12 and pause menus before every seek pose and press.
- **1228d, failed:** the helper was first named `Clear`, which PowerShell resolves to the `Clear-Host` alias before any function. Every case threw "CursorPosition: The handle is invalid".
- **1228e, lever in both roles (confirmed runtime):** both sources focused `script_lever_windturbine_1_1_1`, registered the use on the first held press and classified it 2.
  - Client source: the partner played it at weight 1.0 to 100%.
  - Host source: the partner stopped at 27% because one observer frame was refused, which drops the whole presentation lease (#34).
- **Hazard:** city watch guards near the turbine then killed the host observer (logic 13), so the latch and tap cases could not run. The take was stopped and the lab relaunched and restored. No native `notarget` exists (confirmed static).
- **Visual:** neither lever video is reviewable (#33). One shows Corvo about 10 m away; in the other the observer is under attack.

**#4 status:** every variant has been exercised natively in both roles on a real mechanism, recorded and reviewed:

| Variant | Mechanism | Takes |
|---|---|---|
| Lever | wind-turbine lever | 1228e |
| Crank | bench vise | 1227b |
| Wheel/valve | sink tap | 1228b |
| Key/lock | ring locker | 1228, 1228b |

Accepted visuals so far: Corvo at the vise and Emily at the ring lock. Lever and valve visual acceptance continues in #33.

**#33 lever and valve visuals (1214ak, [audit 1282](../../work/movement-integration/lever-valve-audit-v1282.json)):** accepted (functional) in both roles.
- **Framing fix:** the observer takes its pitch, then teleports and holds the camera in the same step. The held view stays at the spot while the body falls. Every earlier lever spot fell from the turbine platform (z 20.5) to the street before the hold.

| Take | Mechanism | Source | Visual |
|---|---|---|---|
| [use-variants-v1228i](../../work/natural-moveset/use-variants-v1228i) | turbine lever | Emily | Accepted: full body on the platform, reaching the lever on the post |
| use-variants-v1228i | turbine lever | Corvo | Not accepted: head and shoulders above the frame (2.4 m, pitch 15) |
| [use-variants-v1228j](../../work/natural-moveset/use-variants-v1228j) | turbine lever | Corvo | Accepted: fully framed from 3.0 m at pitch 5; turbine blades sometimes pass in front |
| [use-variants-dunwall-v1228k](../../work/natural-moveset/use-variants-dunwall-v1228k) | sink tap | Emily | Accepted: reaches and turns the tap |
| use-variants-dunwall-v1228k | sink tap | Corvo | Not accepted: his body fills the frame and hides the tap (1.3 m behind, 0.5 m aside) |
| [use-variants-dunwall-v1228l](../../work/natural-moveset/use-variants-dunwall-v1228l) | sink tap | Corvo (Emily again) | Accepted: fully framed with his hand at the tap (1.4 m behind, 0.9 m aside, pitch 20) |

Harness options: `-LeversOnly` (1228i); `-Back`, `-ObserverPitch` and `-LeverSources` (1228j); `-Side` and `-TapsOnly` (1228k).

**#5 verified (1213h, take `use-variants-dunwall-v1228e`, passed, restoration passed):**
- The host's window use now plays to duration (100%, 31 frames) despite the 0.641 m drop off the ledge. Under the old rule it ended by movement after 12 frames (1228b).
- The client's window use also plays to duration.
- In the same take, the lock and tap passed again in both roles.
- Framing note: while the observer camera is held, `setviewpos` z is the observer's feet. The close focus-line camera therefore sat about 3.3 m up and looked down at the head (#33).

**Audit:** [use-variants-audit-v1229](../../work/movement-integration/use-variants-audit-v1229.json). Per-video reviews are in [use-variants-review-v1229](../../work/movement-integration/use-variants-review-v1229.json).

**Hand targeting (#6, candidate 1213k):**
- **Native IK exists (confirmed static):** `arkFabrIK`, `arkAnimIKRequest` (10-slot request manager) and IKinema task definitions. Each is referenced only from the large type-registration function, so driving them natively needs more RE. The mod corrects its own written pose instead, as the knee pole correction already does.
- **Remote rig (confirmed runtime, `--skeleton`):**
  - right arm: 13 `right_shoulder_jnt`, 14 `right_arm_jnt`, 15 `right_elbow_jnt`, 16 `right_wrist_jnt`, 17 `right_hand_attachment_jnt`;
  - left arm: 6–10.
- **Target:** the focused entity's origin (`actor+0x300`, as `readNativeUsable`) travels in the use packet (`useTarget`, protocol 75, 432 bytes). On the partner it maps into model space through the skeleton root (`actor+0x300`: position, then rotation columns). This is the exact inverse of the joint-to-world mapping in `grenade_fx` (unit-tested).
- **Solver (`movement_arm_reach.h`, unit-tested):** a two-bone wrist reach.
  - The arm joint stays fixed, segment lengths are kept exactly, and so is the authored elbow bend side.
  - The hand keeps its orientation relative to the forearm.
  - Targets beyond reach clamp to the reachable shell.
- **Application:** forward kinematics of this frame's authored pose gives the model-space chain. The reach weight is `useWeight` times `UsePresentation::contact(amount)`: none while reaching out (0–12%), full from 30% while operating, eased out over 72–92%. The authored local bone offsets stay exact.
- **Status:** `useTargetValid`, `useTarget`, `useReachApplied`, `useReachRefused`, `useReachError`, `useReachWeight`.
- **Verified (take [1234b](../../work/natural-moveset/use-reach-eotw-v1234b), passed; [audit 1235](../../work/movement-integration/use-reach-audit-v1235.json); confirmed runtime):** on the vise in both roles, the target arrived and the reach was applied on 88 and 131 frames, with no refusals, at full weight through the contact window.
  - The vise entity origin sits behind its screw handle, beyond a fully extended arm at a 1 m stand-off (closest approach 0.52 and 0.69 m). The arm extends toward it, and the hands meet the screw handle.
  - **Visual accepted:** Corvo (take 1234) and Emily (1234b, filmed from the mirrored side, away from the wandering worker NPC).
  - Take 1234 itself failed on a harness bug; its runtime data is the same.

**Open:**
- Lever and valve visual acceptance (#33); latch (optional).
- #6 follow-up: exact per-handle contact points; the target is the entity origin.
- Hand targeting to the real handle geometry (the clips are stand-ins).

## Window traversal (`window_enter_exit`, audit 1207)

- 1201 (Dunwall Tower window) and 1206 (Edge of the World docks apartment): both catalogued window usables opened natively, but neither is climbable.
  - The tower window is a chest-height opening over a sheer drop: jumping toward it only bumps the frame, and no mantle triggers.
  - The docks one is a venetian-blind toggle.
- A Ghidra string census found no window-specific player traversal state. Window entry and exit is the generic native mantle (state 8, confined/low variants), already partially bound.
- **A genuinely climbable window, both roles (#7, candidate 1213l, [audit 1238](../../work/movement-integration/window-traversal-audit-v1238.json); confirmed runtime):**
  - **Probe 1231b:** a fall-guarded probe of the 41 openable Edge of the World windows. `script_rich_small_01_sndctrl`, `_10` and `_18` are climbable with the native Mantle state: forward plus jump crosses about 2 m. `_16` crossed by slipping. The first probe (1231) lost the host to a fall and pushed backwards.
  - **1235c:** both roles mantled through `script_rich_small_01_sndctrl`, but the partner showed only a jump.
  - **Diagnosis (1237c, research probe):** the scene is the known confined low-crouch mantle (kind 1, next 10, `mantle_low_crouch.midnightscene`, 500 ms, Scene_2). It binds the player and the **left** arm, while the source reader accepted only a right arm for low climbs.
  - **1213l:** the confined low mantle may bind a single left arm. In take 1235d (passed) the partner presents the traversal (variant 2) at weight about 1.0 in both roles.
  - **Visual:** not accepted. The side-on observer spot had no line of sight to the window (#33).
  - Harness lessons:
    - `$probe` vs the `[string]$Probe` parameter (coercion, 1235);
    - teleport only after the partner is live (1235b).

## Counterattack and knockdown (audit 1211)

- **1208 / 1208b.** Host Corvo in the Dunwall arena recorded block and timed-parry scenes. One timed parry occurred (the attacker was left off-balance). The follow-up attack collided with the block driver's input (two input owners), so no follow-up scene was captured.
  - The player's parry-win chance is 0% in this save, so a parry never knocks the enemy down. The native finisher on a knocked-down enemy is `fatality_knockdown`.
- **1209.** An attack spree recorded ten native slashes and focus scenes, but no player knockdown before the health safety stop (three attackers, 7.3 s).
  - The native Corvo knockdown assets exist: `ply_body_generic_knockdown_fall`, `_front`, `_land` and `_recovery`. They come from the `PLAYER_HITREACT_KNOCKDOWN` blowback.
  - Provoking one needs a guard parry-win against the player.
