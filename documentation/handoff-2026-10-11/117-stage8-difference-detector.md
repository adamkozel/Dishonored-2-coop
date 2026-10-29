# 117. Stage 8 step 2: the difference detector and the flight recorder

Card [#235](https://github.com/adamkozel/Dishonored-2-coop/issues/235), from [the stage 8 brief](115-stage8-plan-brief.md). This step builds the tools that prove each later area. The difference detector says *what* differs between the host's and the partner's worlds; the flight recorder says *when* and *after which message*. Both follow coop-anything's designs ([chapter 114](114-coop-anything-playbook.md)), with the vendored tools in `outputs/d2-coop-lab/third_party/coop-anything/` (MIT, generalised from fnv-coop, acknowledged at the user's request).

## The difference detector

It runs from outside the game, reads memory only, and adds no load to the game.

**`d2_runtime_probe PID --world-digest`** (candidate `work/stage8/candidate-probe-digest-v2002`) prints one snapshot of the world's shared state. Every key is a name that is the same in both games:

| Category | Key | Value |
|---|---|---|
| doors | door name | current, pending, locked, broken |
| usables | usable name, safe dials included | current state, enabled |
| storyflags | `decl\|variable` | the bool (+0x19) or int (+0x1c) value, chosen by the variable's vtable |
| gameflow | node id (FNV-1a of the name) | state |
| chaos | `points` | the local player's chaos points |
| npcs | unique spawner name; packs counted | alive, unconscious or dead |
| breakables | name | break step |
| pickups | name | taken, hidden. Per player by design (the user's decision) |

- A name seen twice in one category is dropped, because it is not a stable key.
- The mod's own entities (`d2coop_*`) are left out.

**`tools/diag/d2_world_diff.py`** compares snapshots from the two games:
- **The stable-world gate:** a key counts only when it has the same value in all three snapshots of its game, taken 3 s apart. Keys that are still changing are reported, never compared.
- **Digests:** each category gets a 256-bucket digest in the vendored flight format. Every difference is named, with both values.
- **Expected differences, listed separately, never failures:**
  - per player: pickups;
  - per character: entities named for Corvo or Emily, such as `ld_dun_audiograph_01_corvo`;
  - names generated at runtime, such as `outsider_rune_12363` versus `_12364` in the other game. These are grouped by their base name.
- **Statuses:** EQUAL, DIFFERENT, or SKIP when nothing was compared or the maps differ. A SKIP is never a pass.
- **Tests:** `tools/diag/test_d2_world_diff.py` runs 7 tests and catches 4 mutation twins (no stability gate, presence only, zero compared passes, no grouping of generated names).

**`tools/diag/Invoke-D2WorldDiff.ps1 -A host -B client|vm -OutDir <take> -Tag <name>`** runs the whole detector in one call. It works for any lab take, and for two-account sessions through the VM's probe. Test `world-diff-helper-v2005a` on the Dunwall fixture compared 554 values: equal.

## What it found (confirmed runtime)

| Take | Maps | Values compared after load | Real differences |
|---|---|---|---|
| `world-digest-v2002a` | Edge of the World | 2279 | the 6 safe dials |
| `drift-census-v2003a` | the other 10 mission maps (Dunwall streets, Institute, Clockwork streets and mansion, Conservatory, Dust District, Crack in the Slab, Grand Palace streets and palace, Dunwall return) | 1082–2350 per map | the safe dials on every map (1–9 per map, rolled per world at load); Crack in the Slab: one NPC spawned from a different spawner in each world (`pr_a_ld_spawner_73/74`); Clockwork Mansion: one breakable missing in the host's world (`env_mulberry_mead_01_9`) |

Doors, story flags, gameflow and NPC states were equal on every map after load, in a Corvo host / Emily client session with product sync on. This is the baseline. Area 4.3 (safes) and area 4.4 (load-time spawns) get the named items above, and the drift after play is measured area by area in step 4.

## The flight recorder

- **In the DLL**, candidate `work/stage8/candidate-flight-v2004`: 1214kq plus `src/flight_recorder.h` and the vendored `cofr.cpp`.
  - Every datagram sent (`coopSendMessage`) and received (the receive drain) goes into an 8 MB ring, keeping the first 256 bytes. The first 4 bytes are the packet magic.
  - `coop_flight dump` writes `<status folder>/d2coop-host|partner-<tick>.cofr`; `coop_flight off|on` stops or resumes recording.
  - Peers are recorded as slots (host 0, partner 1), never SteamIDs.
- **Take `flight-recorder-v2004b`** on the Dunwall fixture:
  - Both games dumped about 4 MB, roughly 93 s of traffic, about 13 200 records each.
  - coop-anything's analyzer (`flight.py align`/`diverge`) matched 13 209 messages between the machines, with a clock offset of +14.7 ms. 2 messages were lost and 2 orphaned, both at the edges of the ring window. No divergence was found.
  - The first take, `flight-recorder-v2004a`, failed because the lab's `Send-D2Command.ps1` allow-list lacked `coop_flight`. That was fixed, and the failed take is kept.
- **Build:** all unit tests pass. `d2_proxy_smoke` exits 86, but it does so for the 0.9.6 DLL as well. The failing check is compiled into the smoke executable, so it is not this change ([#247](https://github.com/adamkozel/Dishonored-2-coop/issues/247)).

## Honest takes

Every stage 8 harness now follows coop-anything's QA rules:
- it asserts its mode first: two peers, one map, both in gameplay, the transport fresh ("connected is not live");
- it records SKIP rather than a pass when nothing was compared;
- it keeps failed takes;
- it restores the lab and checks the protected files.

## Left for later steps

- **The game-level mutation twin for doors** (open a door with `coop_door_sync` off, and the detector must name it) is built with area 4.3's door harness. Step 2's live red proof is the real safe-dial drift, caught on all 11 maps.
- **Digests inside the DLL**, sent with the epoch and sequence number so the flight analyzer can compare them live, come with the first host-authoritative area (4.1). Until then the external detector gives the same answers between takes.
- **Message names for the analyzer:** a schema that decodes our packet magics.
