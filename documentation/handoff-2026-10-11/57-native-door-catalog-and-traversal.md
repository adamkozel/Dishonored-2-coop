# Native door catalogue and collision traversal

Continuation of [native door API and Steam replication](56-native-door-transition-api.md). This chapter separates verified native movement through a doorway from catalogue discovery and from still-unverified campaign semantics. The ordinary-door feature remains opt-in and requires the isolated fixture.

## Collision and traversal on candidate716

The [Emily723 receipt](../../work/natural-moveset/native-door-traversal-emily-v723/result.json) and [Corvo724 receipt](../../work/natural-moveset/native-door-traversal-corvo-v724/result.json) each pass five checks. In each run the other character operates the ordinary Dunwall closet door through native X input. Native Steam replication queues exactly two receiver transitions, with zero API calls on the interacting source. Both engines complete Closed→Moving→Open→Moving→Closed and host revision3. No rejected-packet counter increases during measured input.

The receiving player starts at `[-46.5,-163,66.403648]`, facing south toward the door plane at y=-164.5. Native forward input stops at y≈-164.0594 while closed, crosses to y=-166.894928 for Emily / -166.797623 for Corvo while remotely opened, and is blocked again at y≈-164.0594 after closure. No teleport occurs during any measured walk. The source is moved away during those intervals to avoid the permitted partner movement collision. Vertical change stays below0.05 and lateral change below0.001. Both complete checkpoint inventories, player56/partner40 health and all14 protected files are restored.

Both source and observer contact sheets in723/724 were visually inspected. They show the closed door, opened closet, receiver inside the closet, and closed door afterward. The nearby wall/doorframe limits third-person visibility. This is traversal evidence, not a complete-body animation review. Audio was not captured.

[722](../../work/natural-moveset/native-door-traversal-emily-v722/result.json) is retained as a failed geometry test. Starting at x=-47.05 crossed the opened door but drifted laterally≈0.52, exceeding the declared0.35 threshold. The centered path in723 corrects the fixture. Doorjamb collision is a plausible explanation for the lateral motion, not an independently instrumented collision-contact diagnosis.722 imagery has not yet been visually reviewed in this continuation.

## Authored ordinary-door catalogue

[Read-only721](../../work/movement-integration/door-catalog-v721/summary.json) found ten native ArkDoor entities in both isolated games: six apparently ordinary candidates, one blocked door, two locked linked doors, and one additional locked door. The two ordinary wooden doors `_1` and `_2` start Open; `_3` and three eligible large window doors start Closed.

Candidate725 replaces the literal `_3` and Dunwall-map selection in [door_sync.h](../../outputs/d2-coop-lab/src/door_sync.h) with a bounded snapshot of native authored identities. Every entry gets its own session/world/arm validation, host revision, client request acknowledgment, native transition completion and replay guard. Duplicate authored names exclude every matching entry, including cases where definitions differ or only one duplicate is locked. Native handles remain process-local. Animation-component generations are now pinned as part of lifetime identity.

The catalogue requires stable state, all four ordinary directional open/close clips, no lock/block/breakage/linked door and at most128 eligible entries. Service visits entries round-robin with a soft3ms budget. This is not a hard real-time guarantee; the timer and one whole native entry may exceed that budget. The full scan happens only while arming. Newly spawned/unlocked doors are not dynamically incorporated. Other map compatibility has not been exercised.

## Failed726 and inactive Open-state data

[726](../../work/natural-moveset/native-door-catalog-corvo-v726/result.json) failed before gameplay input: host registered five eligible doors and client six. Five paired channels converged; the unmatched client channel stayed awaiting a peer. Both inventories/health and protected hashes were restored. No video was started, so this run must not be indexed as a video proof.

[Read-only727](../../work/movement-integration/door-catalog-v727/summary.json) isolated the difference in `ld_door_big_rich_int_window_350_0_4`: the host's inactive Open-state side byte at Openable+0xc8 was0xf0 while the same door was Closed; the client's was0. The reader had incorrectly required that byte≤1 regardless of the active state.

Ghidra [native interaction14074b6e0](../../work/movement-integration/door-native-side-v728/14074b6e0.json) and [autoclose14074ac50](../../work/movement-integration/door-native-side-v728/14074ac50.json) establish that ordinary closing obtains this byte from the active Open state. Closed-door interaction calculates its side from geometry. Moving stores direction separately at Openable+0x92. The inactive value appears uninitialized, but construction/serialization was not traced; that is an interpretation, not a proven constructor defect.

Candidate728 canonicalizes only the reader snapshot to side0 outside Open. It never overwrites the inactive native byte. Active Open still rejects invalid sides. Unit coverage verifies inactive0xf0 in Closed/Moving, active invalid-side rejection, and active side1 preservation, alongside existing reciprocal ownership/recycled-handle/ambiguous-name tests. The complete native build, protocol, actual357-clip banks, resume and proxy smoke checks pass. Ghidra bookmark `NativeDoorOpenSide` at14074ac50 was saved in the persistent exact-executable project.

## Runtime continuation

The later [audit735](../../work/movement-integration/native-door-catalog-audit-v735.json) contains eight passing receipts: two716 collision/traversal tests above and six728 catalogue tests.729 operates the original closed closet door;730Emily/731Corvo operate the initially open bathroom door `_2`;732/733 change operator within one arm in both orders;734 repairs the bathroom observer view. All restore both inventories, health and14 protected files. The original game and isolated executable/Steam DLL pins are checked by the separate18-file integrity refresh.

Every728 run discovers the same six eligible doors and excludes four. The selected native cycles reach host revision3; the other five entries stay at revision1 with their initial targets and zero native calls. Unmatched/global rejection counts stay zero. Per-entry startup rejection can occur before the measured action; measured rejection deltas are zero. This corrects the catalogue discovery defect in726 but does not prove actual operation of all six doors. Only the closet and bathroom were operated.

For one-operator tests, the receiver makes exactly two native queue calls and the source zero. For [Corvo-first732](../../work/natural-moveset/native-door-mixed-host-first-v732/result.json) and [Emily-first733](../../work/natural-moveset/native-door-mixed-client-first-v733/result.json), one player opens and the other closes after completion; each process makes one received native call, with one client request ordered by the host. Explicit duplicate requests/states never add a call. These are sequential actions, not simultaneous conflicts or active-motion reversal.

All729–734 source/observer sheets were inspected.730/731 source views show the bathroom door closing and reopening, but their observer cameras show a bright blank view and are not usable visual proof. [734 observer sheet](../../work/natural-moveset/native-door-catalog-second-view-v734/observer-sheet.jpg) fixes the camera to `[-49.2,-161.4,68.1]`, yaw163: it visibly shows the remote character and the bathroom opening/closing. The source is `[-50.3,-160.75,68.1]`, yaw180. Some old case-summary `yaw` fields retain the closet default270; the saved run scripts contain the actual180 command and are authoritative for these bathroom runs. Native recordings show door operation; they do not establish complete-body animation acceptance or sound behavior.

Catalogue timing fields use `GetTickCount64`, which reports coarse0/15/16ms samples here. They are not precise subframe profiling or proof of a strict3ms cap. Do not present the configured soft budget as measured performance.

Candidate728 DLL SHA256: `A03BD0627788F8DA6778CB14C491E8782F2591BC61A1CBDA55885486C2CFB389`; probe: `1ECB428D77B93915EA0417F3531E9103BB8E24DEC2336E874A809BA827810B22`. Both357-clip version67 banks and protocol69 are unchanged. [Archived728](../../work/movement-integration/candidate-door-catalog-v728/identity.json) preserves exact source and binary; rejected725 remains archived with its failure. This is experimental functionality under the explicit isolated fixture, disabled after every test.

Next cases are rapid/conflicting input, checkpoint interruption and stale-arm/epoch refusal, followed by locks/keys/groups/scripts and shared pickups. Locked/keyed/linked/broken doors, NPC operations, story-event actor payloads, navigation/audio and campaign-wide acceptance remain separate work. Native UI, native Steam, original assets and the local first-person path are preserved.
