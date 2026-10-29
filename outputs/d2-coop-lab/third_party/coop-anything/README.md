# coop-anything sync test tools (vendored)

These are five game-agnostic sync test tools from [cranebeaker/coop-anything](https://github.com/cranebeaker/coop-anything), copied unchanged from commit `3dbf6dda2d6bf8183fc21e9764e309d7fe2ef22b` (2026-10-28) under the MIT license in [LICENSE](LICENSE).

coop-anything says these tools come from the [fnv-coop](https://github.com/cranebeaker/fnv-coop) project, generalised so that each game's details live in config files and adapters. The `examples/fnv/` folders are their test fixtures. They contain no Dishonored 2 code, and we take no co-op code for this game from any other mod.

The user approved vendoring them with this acknowledgement on 2026-10-29 (stage 8, decision 1 in [the brief](../../../../documentation/handoff-2026-10-11/115-stage8-plan-brief.md)).

| Folder | What we use it for |
|---|---|
| `chaos/` | The network-trouble relay on the lab's UDP leg, and `cpp/impair.h` for in-process tests (step 3) |
| `flight/` | The flight recorder format (`.cofr`), its C++ writer and the analyzer that finds the first divergence (step 2) |
| `mutants/` | `coop_mut.h` and the twin runner: every fix ships with a switch that restores the bug (step 2 onward) |
| `animcheck/` | Measuring the partner's movement: sliding, jitter, snapping, pose error (step 5) |
| `ownership/` | Epochs, grant, transfer and release for NPC ownership, if step 4.5 needs it |

Each tool runs its own tests and mutation twins with `python -I <tool>/run_tests.py`. The C++ parts need the Visual Studio CMake on `PATH`. All five printed `ALL OK` when they were vendored.

Changes for Dishonored 2 go in our own source (`src/`), not in these copies, so the copies can be compared against upstream.
