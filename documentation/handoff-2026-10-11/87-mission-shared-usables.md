# Shared usables on mission maps (candidates 1123–1123e and 1214jh–1214jn, stage 5)

Updated 2026-10-14. Follows [chapter 84](84-mission-shared-doors.md) (doors) and the Dunwall usable work in the chapters 56–61 era (window family, takes 828–835). Audit: [mission-usable-audit-v1145](../../work/movement-integration/mission-usable-audit-v1145.json). Final harness: [`Test-MissionUsable-v1144.ps1`](../../work/natural-moveset/Test-MissionUsable-v1144.ps1).

## From a Dunwall name rule to a structural family

Usable sync previously had two Dunwall-only restrictions:
- a map gate;
- a window family recognised by name (`ld_rich_small_01_sndctr_*`) and by declaration `map_cpntusablewithstatelist_decl_4`.

Declaration names are numbered per map, so neither rule carried over to missions.

**Census.** A new read-only probe mode, `--usable-catalog`, lists every state-list usable with its name, declaration, state count, current and initial state, flags and per-state entries.

**Dunwall (52 usables), confirmed runtime:**
- Windows, bathtubs, cupboards, cabinet and room doors, sinks and the food-shop shutter all share instant two-state entries: `{enter 255, exit i}`, no apply-on-finish and no transition time. The only difference is a uniform custom prompt such as Open/Close.
- One-state usables (taps, clocks, globes) have nothing to sync.
- Multi-state devices (a 5-state typewriter) and audiographs are different. Audiographs carry flags bit 0 and a non-toggle entry.

**Edge of the World, confirmed runtime:**
- There are 269 usables per role.
- 104 toggles are identical on both characters, and all of them pair.
- 44 more two-state usables are *reversed* (entry *i* exits to 1−*i*: sinks, taps and cabinets). Including them gives **145 shared toggles**, which pair on both peers.

**The family (`nativeUsableToggleFamily`).** A usable qualifies when all of the following hold:
- it is a state-list usable with 2 states, initial state 0, and is enabled;
- flags bit 0 is clear;
- entries are `{255, i}`, or uniformly reversed, with no apply-on-finish, no transition time and a uniform prompt;
- it is not a vault (name contains `safe`);
- it has a unique authored name, with `definition == name`.

The rules hash covers the declaration and the entries, so peers only pair identical usables. The catalog cap is 256.

The map gate became `coopWorldPairReady`. Pairing is by name, so the per-character worlds intersect naturally, as pickups and doors already do.

## Native use focus (1133, confirmed runtime)

Blind aim sweeps on the mission (1124–1132) failed for different reasons:
- prompts belonged to neighbouring usables (the sink cabinet vs the tap);
- one window was locked from the inside;
- a teleport stand lay outside streamed geometry, and the host fell to Game Over (lab restored);
- the first target was 2 m from hostile dock workers.

A read-only probe in Dunwall compared player components while the camera was focused on a window and when it was looking away:

| Field | Meaning |
|---|---|
| `arkCpntPlayerSensorInteraction+0x80`, high dword | Focused entity handle (`0x3fff....` means none) |
| `arkCpntPlayerInteraction+0x38` | Focused usable component |

**Finding:** windows are operated through a separate `*_window_handle_*` UsableStatic, not through the state-list itself. The mission harness now:
1. steers aim until native focus is a usable within 2.5 m;
2. presses X;
3. diffs the census to see which shared toggle the press drove.

## Runtime

| Take | Candidate | Result |
|---|---|---|
| 1128 | 1123b | One transient context blip retired all 145 client channels. **1123c**: inside an armed session, pose stalls (under 1 s), the native mutation gate and pair-readiness blips pause channels; identity changes still retire them. |
| 1135–1138 | 1123c | The host opened `sndctrl_14` through `window_handle_149`, and the client converged natively. Once open, the handle could not be refocused by either role, because the sash moves the handle. |
| 1139 | 1123c | The client's arm was refused right after the intro cinematic. **1123d**: a session arm checks identity scope only. |
| **1140 (pass)** | 1123c | Host opens `sndctrl_14`; the client converges in 370 ms. The client closes `sndctrl_11` through `window_handle_146`; the host commits it and converges in 57 ms. The client use fell after the recording ended. |
| 1141 | 1123d | The session stopped with `unsupported_native_phase`. **1123e**: the item wheel (8) and tutorial modals (12) suspend door, usable and coin sessions instead of ending them, including the opening sample (logic 1 with the overlay pending and no cinematic metadata). Door-scope tests were updated. |
| 1142–1143 | 1123e | Sessions stayed active with no suspensions. A focused single press sometimes doesn't register natively, so the harness retries while focus holds. Both directions converged in 1143; the take only failed on the harness's recording wait. |
| **1144 (pass, recorded)** | 1123e | Host opens `sndctrl_14` (client converges in 221 ms); client closes `sndctrl_11` (host converges in 58 ms). 145 channels paired on both peers, sessions active, lab restored. |

**Visual acceptance (1144):** accepted on the client video only. It shows window 11 open, with the outside visible and the native "Window" prompt, then closed after Emily's own use. On the host video, the blinds hide the sash of window 14; its open state is confirmed only by the native census of both peers.

## Scripted levers, buttons and lifts (candidates 1214jh-1214jn, #187)

Updated 2026-10-24. Harnesses: [`Survey-MissionUsables-v1856.ps1`](../../work/natural-moveset/Survey-MissionUsables-v1856.ps1) and [`Test-MissionLever-v1867.ps1`](../../work/natural-moveset/Test-MissionLever-v1867.ps1), with earlier versions from v1857. The take watches the lever, the scripted objects within a radius of it, both worlds' story flags, both worlds' Kiscule signals in the 2 s after the press (`*_ai` scripts left out) and both worlds' posted entity events. After the press, the other player is placed by the lever, because animations far from a player are not updated. Audit: [scripted-levers-audit-v1868](../../work/movement-integration/scripted-levers-audit-v1868.json).

**Survey ([mission-usables-v1856a](../../work/natural-moveset/mission-usables-v1856a)):** the nine mission maps have identical usables in both worlds. Lever-like usables fall into three groups:
- **Two-state toggle levers, shared by state:** the Clockwork Mansion's 40 `tr_*_lever` levers and lift hatches, and the wind turbine levers.
- **One-state buttons, not synced at all:** lift and elevator calls, the Clockwork Mansion's escape buttons, and valves.
- **Two-state non-toggles, not synced:** lever templates whose entries name animation indices, folding lift doors driven by the lift's script, and safe doors.

**The gap (confirmed runtime, [mission-lever-v1858a](../../work/natural-moveset/mission-lever-v1858a)):** her pull of the Edge of the World wind turbine lever set the host's lever too, but only her turbine started.

**Finding the cause:**
- 1214jh (the instigator) and 1214ji (the native use method, vtable slot 30 `0x14077ae00`, on the replica) did not start the host's turbine (v1858b, v1858c).
- The lab trace 1214jj/jk (`coop_kiscule_trace`, every dispatched Kiscule signal) showed that the press runs a chain in her world only (v1860a). The chain starts at an `ActionBoolControl` in `script_kiscule_eotw` and runs through gates to the turbine script's `Fx`, keyframe and sound nodes.
- **Confirmed runtime by name:** the usable's own events are `stageChangeStart` and `stageChangeFinish` (`_user`, `_stageIndex`), and the replica posted them.
- The lab event trace 1214jl (the native post variants `0x140883f80`, `0x140884920`, `0x1408848a0`) showed what else her real use posts (v1861b): the entity event **`use`** (`_user`, descriptor `0x1439aa130`) to the lever, and `interactWith` to her player. The replica posted neither. The lever's script listens to `use`.

**Fixes:**
- **1214jm:** the replica of a shared two-state usable posts `use` with its own world's player through `0x140884920`. The argument is built by `0x140882fa0` and the result released by `0x140882fc0`, as the lock routine does. It then runs the use method, unless the event already started the change.
- **1214jn, the use mirror (`D2UM`):** the post hook passes every `use` event whose `_user` is this world's player to the mirror. If the target is a one-state usable or a two-state usable outside the toggle family, its name is sent. The other world posts `use` with its own player and runs the native use method. Safes and combination dials stay per world ([chapter 106](106-partner-led-story-and-mid-mission-join.md), decided). A mirrored use is never sent back. Product step 23, maintained.
- **Unit tests:** use mirror packets, eligible usables and per-world safes.

| Take | Result |
|---|---|
| 1858a | The gap (1214jg). |
| 1858b, 1858c | 1214jh, 1214ji: lever shared, script not. |
| 1859a, 1859b, 1860a | The lab trace: the chain runs in her world only. |
| 1861a (fixture), 1861b | The event trace: `use` is missing on the replica. |
| 1861c | 1214jm: the host's world runs the same chain, every node, 360 ms later. The host's blades stood still 70 m away, because distant animations are not updated. |
| **1862a (pass, partner)** | Wind turbine lever: lever, chain and blades in both worlds. |
| 1862b | Host source: chain and turbine in both worlds. The comparison counted host AI scripts that ran 4.3 s later, when a guard noticed the host. |
| **1863a (pass, host)** | Wind turbine lever from the host, with the 2 s comparison. |
| 1864a, 1865b (fixture) | The lift button never had the use focus: aim heights, and the host landing 0.9 m off the stand. |
| **1865a (pass, partner)** | Royal Conservatory lift: her top-floor call was mirrored and the host applied it in 141 ms. The cabin rose from z −12.5 to 8.6 in both worlds, and the bottom and top folding doors swapped in both. |
| **1866a (pass, host)** | The same lift call from the host: the cabin and doors match in both worlds. |
| **1866b (pass, partner)** | Clockwork Mansion entrance lever `tr_entry_lever_2`, a spring lever: the transformation moved the same 7 objects in both worlds, including desk doors and the bridge model. |
| 1866c (host) | Clockwork Mansion escape button `tr_priv_escape_button_06`: mirrored and applied. The transformation moved the same 34 objects in both worlds. One Jindosh voice gate ran in the host's world only: it follows the host's touch of the `mark_trigger_lab` position trigger, not the button (audit). |
| 1867a (fixture) | A Crack in the Slab's past-layer lever never had the use focus. That layer needs the Timepiece. |
| 1867b (fixture) | Dust District's two-state non-toggle lever sits 4 m above the floor below: every stand fell to the lower floor, so the aim never reached it. **Open:** two-state non-toggle levers go through the use mirror by design and are unit-tested, but no take has yet passed for one. |

- **Vaults:** safes, safe doors and combination dials stay per world by decision ([chapter 106](106-partner-led-story-and-mid-mission-join.md)). The usable toggle family and the use mirror both leave out every name containing `safe`, `digit` or `combinaison`. What one player takes from an open safe is a shared pickup ([chapter 83](83-mission-shared-pickups.md)).
- **The post hooks:** the trace hooks on the three post variants are pass-through. The `use` observer on `0x140884920` is part of the product, and the traces record only while `coop_kiscule_trace` is on.

## Status

**Stage 5 on missions:** two-state world toggles are shared by ordinary native use from both roles. Each change commits once on the host and applies natively on the peer, as doors (chapter 84) and pickups (chapter 83) already do.

**Open:**
- **Scripted levers:** resolved above (#187). The index request did not replay a lever's script; the `use` event does.
- **Multi-state devices and vaults:** safes and combination dials stay per world (decided). Other multi-state devices, such as typewriters and audiographs, stay per world.
- **Checkpoints.** Usable, door and coin sessions need renewal across mission checkpoints and reloads (stage 6).
- **Press reliability.** The native-press reliability gap is handled by harness retry; real players simply press again.
