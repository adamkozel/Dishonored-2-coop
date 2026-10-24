# 111. Frame pacing (#195)

Players saw frequent stutters in co-op sessions on earlier releases, while the average frame rate looked fine. This
chapter measures the frame time, attributes the spikes to the mod's own work, and records the fixes. Labels follow the
handoff rules: confirmed static, confirmed runtime or hypothesis.

## The recorder (1214jx, [frame_timing.h](../../outputs/d2-coop-lab/src/frame_timing.h))

- **What counts as a frame:** one engine frame runs one engine-thread command-buffer call (`executeBuffer`), so the
  interval between those calls is the frame time.
- **What it keeps, for the last 3600 frames:**
  - the interval;
  - the mod's own time in that frame (the bridge's work before native `ExecuteBuffer`);
  - the frame's most expensive named mod scope (`NativeServiceTimings`, any hook; aggregate scopes left out).
- **Spikes:** a frame of 25 ms or more keeps that attribution.
- **Output:** `coop-status.json.frame-timing.json`, once a second:
  - frame-time p50/p90/p99/max and a histogram;
  - spikes over 25/50/100 ms per minute;
  - the mod time's p50/p99/max;
  - the last 32 spikes;
  - whether the diagnostics are on.
- **Harness:** [Measure-FrameTiming-v1886.ps1](../../work/natural-moveset/Measure-FrameTiming-v1886.ps1).
  - Both lab games load Edge of the World with the health hold, and the product controller arms.
  - Phases of 120 s, each after a 15 s settle:
    - `lab`: product armed, with the lab's diagnostic status files;
    - `product`: the same with `coop_diagnostics off`, as an installed release runs;
    - `off`: the co-op services off too (baseline).
- **Caution:** two lab games share this PC's CPU and GPU, so compare phases, not absolute values.

## Findings and fixes

| Take | Candidate | Phase | Host fps | p50 / p99 / max (ms) | Spikes >25 / >50 ms per minute | Mod time p50 / p99 (ms) |
|---|---|---|---|---|---|---|
| [frame-timing-v1885a](../../work/natural-moveset/frame-timing-v1885a) | 1214jx | services on | 20.5 | 42.2 / 85.4 / 133 | 1206 / 544 | 35.3 / 76.8 |
| v1885a | 1214jx | services off | 56.6 | 15.4 / 44.0 / 59 | 419 / 4 | 8.6 / 36.4 |
| [frame-timing-v1885b](../../work/natural-moveset/frame-timing-v1885b) | 1214jy | services on | 76.3 | 12.2 / 25.0 / 143 | 55 / 3 | 5.9 / 16.8 |
| [frame-timing-v1886a](../../work/natural-moveset/frame-timing-v1886a) | 1214jz | lab diagnostics | 78.1 | 11.9 / 26.2 / 62 | 49 / 1 | 5.8 / 19.1 |
| v1886a | 1214jz | **product (release)** | **122.8** | **7.7 / 15.4 / 20.5** | **0 / 0** | **1.3 / 7.5** |
| v1886a | 1214jz | services off | 162.6 | 6.1 / 8.6 / 18.7 | 3 / 0.5 | 0.02 / 1.2 |

The partner's game shows the same pattern. In v1886a's product phase it ran at 129.8 fps (p99 17.8 ms) with 0.5 spikes
per minute.

- **1. Every native read entered the kernel (confirmed runtime, v1885a → v1885b):**
  - `memoryRead` called `ReadProcessMemory` on our own process. That is one system call per read, thousands per frame.
  - The mod's own work took 85% of every frame in product mode; the pose stream alone took 3.6 ms per frame.
  - **Fix (1214jy, [engine_bridge.cpp](../../outputs/d2-coop-lab/src/engine_bridge.cpp)):** `memoryRead` copies
    in-process under structured exception handling. An unmapped, guarded or failed page still fails the read.
    `coopGuardedRead` keeps the diagnostic exception logger ([lab_proxy.cpp](../../outputs/d2-coop-lab/src/lab_proxy.cpp))
    quiet for those faults.
  - **Effect:** the mod's time per frame fell from 35 ms to 6 ms, and spikes over 50 ms from 544 to 3 per minute.
    The full pickup census cycle (chapter 106) fell from 2.1-2.5 s to 0.7 s.
- **2. The diagnostic status files (confirmed runtime, v1885b; confirmed static for releases):**
  - With the reads fixed, `status_all` was the top scope of almost every remaining spike (29 of 32 on the host, all 32
    on the partner).
  - The 72-writer diagnostic slice exceeds its 4 ms budget whenever one writer is slow (`writeDoorSync` 10 ms,
    `writeCheckpointTrace` 9 ms, `writeGuardStatus` 6 ms).
  - An installed release sets `D2COOP_STATUS_FILE` (`lab_proxy.cpp`, product activation), so players paid for all of it.
  - **Fix (1214jz):** `coopDiagnostics` is on in the lab and off in an installed release, unless `d2coop.cfg` sets
    `diagnostics=1` (`D2COOP_DIAGNOSTICS=1`). With diagnostics off, only `coop-status.json` is written, once a second,
    plus the 1 Hz frame and service timings. The lab command `coop_diagnostics on|off` switches it at run time.
  - **Effect:** in the release setting the spike rate is at or below the mod-off baseline (0 and 0.5 spikes over
    25 ms per minute, against 3 and 0.5).
- **Regression check (confirmed runtime, [route-resumed-join-v1880b](../../work/natural-moveset/route-resumed-join-v1880b), 1214jz):**
  - The resumed-host join route passed with every session armed: the pickup catch-up both ways, 0 door and flag
    differences, and the shared pickups bound (287).

## Both PCs (two Steam accounts, the lab VM as the partner)

**Harness:** [Route-MissionVM-v1887.ps1](../../work/natural-moveset/Route-MissionVM-v1887.ps1), with the `command` and
`frametiming` steps.
- The host runs on this PC and the partner in the VM (40% GPU partition), in product mode on Edge of the World.
- Both games run with `coop_diagnostics off`, as installed.
- Phases of 120 s: the services on, then the product controller and every sync service off (baseline).

[frame-timing-vm-v1887b](../../work/natural-moveset/frame-timing-vm-v1887b), candidate 1214jz, confirmed runtime:

| PC | Phase | fps | p50 / p99 / max (ms) | Spikes >50 ms per minute | Mod time p50 / p99 (ms) |
|---|---|---|---|---|---|
| Host | services on | 155.2 | 7.4 / 15.1 / 22.7 | 0 (0.5 over 25 ms) | 0.1 / 7.8 |
| Host | baseline | 192.7 | 6.5 / 11.1 / 23.4 | 0 | 0.1 / 3.8 |
| VM partner | services on | 47.5 | 24.2 / 46.6 / 49.8 | 2 | 3.6 / 13.5 |
| VM partner | baseline | 49.6 | 21.8 / 46.2 / 49.3 | 0 | 1.8 / 10.4 |

- On both PCs the frame-time distribution with the mod is close to the mod-off baseline: no regular hitches
  attributable to the mod.
- The VM's frame time sits around 25 ms (GPU bound), so its count over 25 ms is not meaningful.
- [frame-timing-vm-v1887a](../../work/natural-moveset/frame-timing-vm-v1887a) measured the same "on" phase with the
  same result.

## Reporting stutters

The release notes' troubleshooting table tells players to send `d2coop\coop-status.json.frame-timing.json` (its `window`
and `spikes`), and, for a deeper report, to enable `diagnostics=1` for one session.
