# Product partner-presentation recovery (candidates 1203–1203f)

Updated 2026-10-14. Audit: [presentation-recovery-audit-v1205](../../work/movement-integration/presentation-recovery-audit-v1205.json). Stage 3, gate 4 (pause, load and session boundaries), under the product session controller.

## The gap (found by code reading, confirmed at runtime)

The product controller (1181) only re-armed the partner presentation per world:
- avatar follow;
- the movement bank (animation);
- the shadow stream (semantic actions).

The lab's movement-resume policy was never armed in product mode.

Two consequences:
- In product mode, the partner presentation stopped after any native pause menu, readable, tutorial modal or cinematic, and nothing restored it until the next map load.
- The earlier product takes (1183, 1186) only checked that the avatar existed, so it went unnoticed.

## What each take found

| Take | Candidate | Result |
|---|---|---|
| 1204 | 1203 (resume step + whole-presentation watchdog) | Fail. Every movement-bank (re)load stalled the committing peer's engine thread for about 1–2 s, because the bind pose was checked against the whole ~270 MB bank on the engine thread. The other peer went stale, and both watchdogs ping-ponged. |
| 1204b | 1203b | The bank commit dropped from about 1–2 s to 0.5 ms (bind check moved to the load worker), and a dormant bank is reactivated in place instead of re-decoded. Still failed: the stream's token handshake rejected a peer that had restarted on its own. |
| 1204c | 1203c | A peer hello now restarts the local stream, and a newer peer token re-pins the handshake. The stream kept retiring within about 1 s: pose gaps of 250 ms or more every few seconds on the busy mission map. |
| 1204d/e | 1203d | Under the product controller only, presentation freshness is 1 s, and 1.5 s for hellos. Arming was stable. The host pause still failed: the client saw stale gaps of 1 s or more while the host's send queue backed up. |
| **1204f** | **1203e** | **Pass.** |

1203e has two changes:
- **Congestion gate (product only).** NPC body poses and world observations skip a cycle while Steam's queue estimate exceeds 40 ms.
- **Repair timing.** The watchdog repairs only the missing pieces, waiting 3 s and doubling up to 15 s. Losses of only the bank or stream are repaired after 1 s.

In 1204f the partner was restored by itself and then followed a 3 m move, each time:
- after the client's pause, in 19.0 s;
- after the host's pause, in 17.4 s;
- after the tutorial modal, in 14.5 s;
- after a 60 m harness warp, in 15.5 s.

The lab's strict 250/350 ms guards are unchanged outside the product controller.

## Reliable-traffic attribution and lifecycle (candidate 1203f, audit 1202)

1203f adds a per-message-type **send tally**: all 48 bridge send sites go through `coopSendMessage`, which writes `.send-tally.json`.

On Edge of the World it named the reliable flooder: **usable sync** (`D2US`), which reached 360 KB/s reliable while binding. Every one of the 145 usable channels sent a 504 B reliable hello every 250 ms, with a 1 s readiness window. The side analysis estimated about 292 KB/s per direction in steady state.

Three product-only changes:
- Usable and door channels send their hello every 2 s once the peer is bound, with a 6 s readiness window. Usables now run at 13–27 KB/s.
- The source pose timeline rebases once after more than 500 ms of samples arriving over 250 ms late. Before, it froze the partner on its last pose under congestion (the 1204f warp hold). It now degrades to extra latency instead.
- The lab keeps 250 ms hellos and the strict timeline.

**Lifecycle 1199d (pass)** — [audit 1202](../../work/movement-integration/lifecycle-audit-v1202.json):
- Only the host loaded Edge of the World; the client followed natively, and the product controller armed.
- The full partner presentation (avatar at the remote player, follow, bank, stream) was live on both peers 1.7 s after the map load. After a shared death and native checkpoint restart it was live again in 2.9 s (14–24 s before 1203f).
- No full repairs. The host queue peaked at 138 ms (about 1 s before).
- Host-side visuals are accepted: Emily stands on the boat in front of Corvo after the load and after the restart.
- The client's spot was displaced to 0.9 m from Corvo, too close for the camera to show him, so there is no client-side visual yet.

Earlier attempts:
- 1199 failed on a harness bug: it commanded a loading peer.
- 1199c's restart facing hit a 1.44 m offset at re-derived spots.

## Still open

- **Restore latency (14–19 s)** is too slow for final quality.
  - The resume policy suspends on pause, but in these takes it never completed a restore: it faults on the first transient loss, and the watchdog performs every restore.
- **Restore latency after pause/modal** (14–19 s in 1204f on 1203e) must be re-measured on 1203f; the load/restart cases already restore in 2–3 s. The reliable backlog itself is solved in 1203f: the culprit was usable-channel hellos.
- **Warp hold.** After a 60 m harness warp, the host-side partner stayed at the old spot for 7–15 s.
  - Root cause: the strict source pose timeline rejected samples during a latency spike. 1203f rebases it (product only); re-test pending.
- **Client-side visual** of the Corvo partner after load/restart: the spots must keep at least 2 m of reachable separation.
- **Cinematic (logic 5)** with a real mid-mission scene is untested.

## Related: stability soak attribution (audit 1200)

A 20-minute control soak with every co-op system off still grew about 5 MB/min at steady state. The product controller run grew 6–7 MB/min. The co-op systems are not the main source of the growth. See [chapter 90](90-release-packaging-and-steam-sessions.md).
