# 116. Stage 8 step 1: measure before changing

Card [#234](https://github.com/adamkozel/Dishonored-2-coop/issues/234), from [the stage 8 brief](115-stage8-plan-brief.md). This step measures how the game decides what is awake, where it writes shared state, and where it assumes one player, before any authority moves to the host. Three read-only helper surveys (d2-ghidra, the resource index, the docs and source) produced the notes in [stage8-step1-notes](stage8-step1-notes/). The main session re-checked their key claims in Ghidra and measured the activation range in the lab. Labels: confirmed static, confirmed runtime, hypothesis.

## 1. The activation range (confirmed runtime)

**The engine pauses an NPC's AI when the entity is not rendered and more than 50 m from the camera.** Take `activation-range-v2001a`, on Edge of the World, is the evidence. The new read-only probe mode is `d2_runtime_probe --npc-activity`, candidate `work/stage8/candidate-probe-activation-v2001`. The harness is `work/stage8/Test-ActivationRange-v2001.ps1` and the analysis `work/stage8/analyze_activation_range.py`. The take set the stage up like this:
- the partner went about 205 m into the interior while the host stayed at the docks spawn;
- both worlds were sampled about once a second for 30 + 60 + 60 s;
- the lab was restored (`restoration.json`: passed).

The mechanism:
- **The LOD reference is one view, the camera** (confirmed static).
  - The LOD manager `FUN_14064cff0` measures every entity from the top of `game+0x1f52b8`, at view positions +0xa8 and +0xc0.
  - The level is written to entity +0x298. The effective level, at +0x294, is the lower of that and the clamp at +0x29c.
  - Level 7 is `LOD_LEVEL_HIDDEN_FAR`: not rendered and beyond `lod_dist_far`, 50 m.
- **Every NPC AI component auto-pauses at level 7** (confirmed runtime).
  - The probe read the decl's auto-pause level, decl +0x64, used by the base `OnLODChanged` handler `FUN_1407e7560`. It was 7 on all 3191 to 6494 NPC samples per phase, in both worlds.
  - The static survey had read the base constructor's default of 8, which means never. The runtime value is what counts.
- **The pause follows the level exactly** (confirmed runtime). With NPC authority off, the host world had 4899 of 4899 AI samples at level 7 paused and out of the scheduler, and 0 of 1595 samples at levels 0–6 paused.

What it means:

| Phase | NPCs 15–40 m from the far partner, in the host's world | The same NPCs in the partner's world |
|---|---|---|
| Partner far, NPC authority off | Level 7, paused 100 %, path 0 m in 60 s | Levels 4–6, running, up to 24 m walked; positions 0.4–4.2 m apart at the end |
| Partner far, NPC authority on (today's 0.9.6) | Level 7, paused 100 %, path 0 m | The bound guards are paused too, as host-driven replicas: path 0 m. An unbound shopkeeper walked 8.8 m |

- **This is a 0.9.6 defect** (confirmed runtime). NPC relevance binds NPCs within 55 m of *either* player (`npc_authority_stream.h`, `npcNearestPlayerSq`). So guards around a partner who is far from the host's camera are handed to a host world in which they are frozen. The partner sees them stand still.
- **Pure host authority over far guards does not work as is.** Gothic 3's lesson holds for Void Engine too, with a 50 m, camera-based range.
- **The native fix to try first:** the entity LOD request API, which keeps an entity at a chosen level or lower.
  - It is `FUN_140720780(requestManager, outHandle, ownerName, level)`, which writes the clamp through `FUN_140879ae0` (entity +0x29c) (confirmed static).
  - The game itself uses it: swarms (`arkCpntSwarmBase::LockLOD`), the evolved animation component (`arkCpntAnimEvolved`, manager from `FUN_1403da910(cpnt+0x448, 2)`) and others.
  - The host would hold a level-6 request on each NPC bound for the partner while that NPC is near the partner, and release it on unbind. Level 6 is below the auto-pause level 7, so the AI keeps running. This is a hypothesis to test in step 4.5. If it fails, the fallback is closest-player ownership with handoffs.

## 2. Where the game writes shared state (confirmed static)

The full tables are in [state-writers.md](stage8-step1-notes/state-writers.md). The findings that shape step 4:

- **One choke point for "the partner runs zero story scripts":** the Kiscule signal dispatch `0x1407c0e30`, which the mod already hooks, with an allow-list by node class.
  - Story-flag variables (`var+0x19`) are already written quietly.
  - Gameflow needs a raw `node+0xc` write plus the HUD notice `0x1404e9a30` alone, because today's set-state fires the change event on the partner.
- **Doors and usables have a native quiet apply:** usable apply `0x14077a880(c, state, transition, quiet)` sends no script events or messages when quiet = 1. The main session re-checked this. Doors have a snap restore, `0x14074adf0` (quiet: hypothesis).
- **Chaos:** the partner's own add `0x1404c14b0` must be suppressed, and partner kills must reach the host as outcomes.
- **RNG:**
  - One game RNG (`game+0x600f8`) serves AI, physics and loot, so the host must send results, not seeds.
  - Random bonecharms reseed from the system clock (`0x14060fad0`) and always differ between worlds.
  - Safe dials are scrambled by a Level Loaded script.
- **Items under the per-player rule:**
  - The pickup itself stays local: claim and finish run in the player's own loot state.
  - Only the host-decided existence of an item, and story items, cross over.

## 3. Where the game assumes one player (confirmed static)

The full list is in [one-player-assumptions.md](stage8-step1-notes/one-player-assumptions.md).
- "The player" is entity-table slot 0, handle 1 (`0x1408a8ba0`, re-checked). There is no player list, and the slot is hard-coded at about 165 sites.
- So "the host runs it as the partner" needs per-feature substitution, not one global swap.

The top risks by area:

| Area | Risk |
|---|---|
| 4.1 story | Player-scoped script actions (cinematic, teleport, grants, statistics, achievements, chaos deeds) reach the wrong player once the partner's scripts stop. Inventory gates (keys, the ring, the Heart) read only the host's inventory. 780 look-at nodes never see the partner. Chaos and kill stats count only the local player, so the shared chaos misses partner kills |
| 4.2 items | Random bonecharms and NPC loot differ per world |
| 4.3 doors and safes | Native door use with a non-player user takes the NPC branch (`0x14074dd50`, a possible null dereference). Safe codes and riddles roll at load |
| 4.5 far guards | AI escalation keyed to handle 1 (`0x1405cf2c0` and others), plus the activation range above |

## 4. Video capture (confirmed static)

Every take records a single game window: FFmpeg `gfxcapture` (Windows Graphics Capture) on the game's HWND, with the cursor off. That is 5798 window-handle captures across the harnesses and none of the desktop. The VM's frame-rate clips use the same method. Nothing needs changing.

## 5. What changes in step 4

- **Area 4.5 starts with the keep-awake fix,** because it is a live defect. Test the native LOD request (level 6) on host-bound NPCs near the partner, and measure it with the same probe. Only if that fails, design closest-player ownership with the vendored `ownership/` library.
- **Area 4.1 adds:** player-scoped actions dispatched to the partner's own player, chaos and kill credit for partner kills, and story items reaching both inventories so that script gates pass.
- **Area 4.3 adds:** doors run as the partner must not take the NPC branch. Either apply the result with the quiet paths, or substitute the player for the actor.
- **The order otherwise stands:** story → items → doors and safes → join and quickload → far guards → partner powers. The difference detector (step 2) comes first so that each area can be proven.

## Evidence

- Take `work/stage8/activation-range-v2001a`: the identity, three phases of samples in both worlds, `analysis.json`, the `far` videos from both windows (not visually reviewed; this is a numeric measurement), and the restoration.
- Probe candidate: `work/stage8/candidate-probe-activation-v2001` (the probe exe and source copy, with hashes; the DLL is unchanged, 1214kq).
- The guard baseline for `profile.bin` was re-recorded after the user's own play on 2026-10-28 (`work/movement-integration/protected-baseline-rerecord-2026-10-29.json`); the file itself was untouched.
