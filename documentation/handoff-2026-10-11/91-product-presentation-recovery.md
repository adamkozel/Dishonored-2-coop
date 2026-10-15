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
- **Restore latency after pause, re-measured on 1213h (#10, take [1230](../../work/natural-moveset/mission-presentation-resume-v1230), [audit 1232](../../work/movement-integration/restore-latency-audit-v1232.json); confirmed runtime):**

  | Case | 1204f (1203e) | 1230 (1213h) |
  |---|---|---|
  | Client pause | 19.0 s | 1.56 s |
  | Host pause | 17.4 s | 1.57 s |
  | After the 60 m warp | 15.5 s | 0.33 s |
  | Armed, facing | 6.9 s | 0.34 s |

  - The reliable backlog is solved in 1203f: the culprit was usable-channel hellos.
  - **New regression (#35):** after the shared tutorial modal, every `coop_movement_bank on` repair is refused on both peers, and the presentation did not return within 30 s (14.5 s in 1204f). 1213i adds refusal reason codes to find the failing condition.

**Refusal reasons and the holstered-sword re-arm (candidates 1213i and 1213j, #34 and #35):**
- **1213i, `PresentationRefusalGate`:** a refused pose frame now skips only that frame, and the presentation stops only after 20 consecutive refusals (about 0.5 s).
  - Before, one refusal set `end=lease=0`. In 1228e that froze a lever use at 27%, and the partner stayed unpresented.
  - Lifecycle loss (actor, world, session, freshness) still stops at once in `serviceMovementBank`.
  - The movement status reports `refuseStreak`, `refuseLongest`, `refuseLast` and `refuseReasons` (not owned, skeleton read, unstable, not gameplay, identity, helpers, pose build, knee chain).
- **Bank-request refusals are named:** `beginRefusal` and `beginRefusals` in the status (17 codes), and the console line "continuous movement bank refused (reason N)".
- **#35 cause (take 1230b, confirmed runtime):** after the modal, the client's bank request was refused 7 times with reason 15, "avatar sword not equipped/started".
  - Confirmed static: the partner's holster state (`requestMovementEquipment(false)`) holsters the avatar sword natively (`equipped=started=false`, `holster_wpn`).
  - Both `beginMovementBank` and the bank commit demanded a drawn sword, so a bank that dropped while the partner was holstered could never re-arm.
- **1213j:** a settled native sword, drawn **or** holstered (`equipped==started`, attached to the matching tag), is valid, and the new presentation starts in that state (`presentedDrawn`, `armedMix`).
- **Verified (take [1230c](../../work/natural-moveset/mission-presentation-resume-v1230c), passed; [audit 1233](../../work/movement-integration/presentation-refusal-audit-v1233.json)):** the presentation is live again 5.1 s after the tutorial modal (never on 1213h/1213i; 14.5 s in 1204f). Pauses take 1.3–1.6 s and the warp 0.33 s. Both partners restart holstered, matching their source.
- **#34 runtime note:** no refused frame occurred in 1230b or 1230c, so the 1228e trigger was not reproduced. The gate itself is unit-tested.
- **Warp hold.** After a 60 m harness warp, the host-side partner stayed at the old spot for 7–15 s.
  - Root cause: the strict source pose timeline rejected samples during a latency spike. 1203f rebases it (product only); re-test pending.
- **Client-side visual** of the Corvo partner after load/restart: the spots must keep at least 2 m of reachable separation.
- **Cinematic (logic 5), real native scene (#8, 1213l, take [1236b](../../work/natural-moveset/cutscene-transition-v1236b), [audit 1239](../../work/movement-integration/cutscene-transition-audit-v1239.json); confirmed runtime):**
  - **Scene:** both players use Mayhew in the Dunwall fixture, and the native skippable scene plays in full.
  - **Entry:** both peers enter logic 5 together, 0.52 s after the use.
  - **During the 30 s scene:** the partner presentation is retired on both peers (follow and bank off), so no partner stands in the scene.
  - **Exit:** 1.57 s after the scene ends, the full partner presentation is live again.
  - **Visual accepted on both windows.** The native scene plays without the partner intruding, and the partner is presented in front right after it.
  - From the pre-Mayhew save the partner stream can't arm, because the scene itself hands over the sword (take 1236). Entry is therefore gated on the avatar being present (follow + bank).

## Holding the partner through source gaps (candidate 1213m, audit 1241)

On mission maps the source's state stream gaps by 250–375 ms several times per 10 s. Under the product controller each gap snapped the partner to its native pose until the stream resumed ([audit 1241](../../work/movement-integration/presentation-hold-audit-v1241.json)).

- **Measured on 1213l (take [1235d](../../work/natural-moveset/window-traversal-v1235d), confirmed runtime):** with the host as source, the client's partner was in its native pose for 29% of the samples, with 11 drops in 8 s. One drop hit the end of the window mantle.
- **Cause (confirmed static):** 1203d relaxed only the stream lease to 1 s. Every presentation timeline (movement semantics and the 12 action timelines) still:
  - invalidated its last sample after 200 ms;
  - cleared itself on a 250 ms gap;
  - ran an action on by at most 100 ms.

  Eligibility mask 16 (stale source state) also used a fixed 250 ms. The root snap after a gap was then rejected as a 12–18 m/s step.
- **1213m:** one shared `presentation_hold.h`. Under the product controller all of these follow the 1 s lease, so a held action keeps running on real time. The lab's strict values are unchanged.
- **Verified (take [1235e](../../work/natural-moveset/window-traversal-v1235e), same harness, passed in both roles, restoration passed):**
  - native-pose time is 1% in each direction (29% before);
  - one drop per case;
  - no host-source root-step rejections (3 before).
- **Scene-to-locomotion handoff:** the mantle pose is presented at full weight. The partner then blends from that actual pose to locomotion over about 250 ms: traversal weight 0.34, 0.07, 0.02 while locomotion returns 0.49, 0.89, 0.97. `scripted_scene_pose_handoff` is now partly bound.
- **Remaining:** one frame of the protocol-73 hold (state 24, an unclassified native state) at the mantle's end still drops to the native pose. The observer spot faces the stair alley, so the video is not reviewable (#33).

## Related: stability soak attribution (audit 1200)

A 20-minute control soak with every co-op system off still grew about 5 MB/min at steady state. The product controller run grew 6–7 MB/min. The co-op systems are not the main source of the growth. See [chapter 90](90-release-packaging-and-steam-sessions.md).
