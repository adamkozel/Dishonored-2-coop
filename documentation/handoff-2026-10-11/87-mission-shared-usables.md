# Shared usables on mission maps (candidates 1123–1123e, stage 5)

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

## Status

**Stage 5 on missions:** two-state world toggles are shared by ordinary native use from both roles. Each change commits once on the host and applies natively on the peer, as doors (chapter 84) and pickups (chapter 83) already do.

**Open:**
- **Scripted levers.** Levers such as `script_lever_windturbine_*` are structurally eligible. Whether the replica's native index request also replays their scripted consequences is a hypothesis.
- **Multi-state devices and vaults.** These (typewriters, safes, 5- and 10-state devices) need their own semantics.
- **Checkpoints.** Usable, door and coin sessions need renewal across mission checkpoints and reloads (stage 6).
- **Press reliability.** The native-press reliability gap is handled by harness retry; real players simply press again.
