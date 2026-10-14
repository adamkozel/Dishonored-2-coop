# Generic NPC Steam root and body stream, 839–846

Candidate847 adds the verified menu/checkpoint continuation described in [chapter72](72-npc-menu-and-checkpoint-lifecycle.md). This chapter preserves the842 transport result.

## Current result and limits

Candidate842 extends the generic native NPC binding/AI lease from837 with a host-authored presentation stream. Five Dunwall NPCs negotiate matching rigs over the existing native Steam connection. The client holds ten Decision/Reflex pause references, receives root and skeletal poses, and returns every reference when the experiment stops. Host AI remains native and active.

[Audit846](../../work/movement-integration/npc-stream-audit-v846.json) and [raw take846](../../work/natural-moveset/npc-stream-v846/result.json) establish the bounded result. The paired [host recording](../../work/natural-moveset/npc-stream-v846/host.mp4), [client recording](../../work/natural-moveset/npc-stream-v846/client.mp4), and [contact sheet](../../work/natural-moveset/npc-stream-v846/contact.png) show a guard progressing from idle into patrol on both views. Sampled frames look coherent; this is not a full motion-quality or combat review.

| Coverage | Evidence |
|---|---|
| Five matching223/224-joint rig streams | Each host stream had180 emitted poses at its snapshot; each client had186 received poses at its later snapshot. These reads are sequential, not one simultaneous loss measurement. No per-channel, decode or pose-scope rejection in the accepted interval. |
| All five client roots | More than1800 root applications per actor in client snapshot. |
| Visible bodies | Current-arm counts1423 for tutorial guard2 and1343 for tutorial guard1. |
| Other rigs | Ramsey224 joints and both escape guards negotiated and updated roots; native body callbacks were culled/not observed. Do not count them as visually verified. |
| Cleanup | Client released10 references, lost0. Both roles remained in gameplay at the same epochs; player health56, complete inventory and eligible catalog retained. |
| Presentation timing | Root and body use the same80ms delayed interpolation endpoints. One guard moved over3m during the captured interval. |

**Not implemented by this stream:** client hit forwarding, shared health/damage/death/corpses, enemy equipment state, AI perception of both players, automatic spawn/despawn, checkpoint/menu rearm, interest management or complete campaign behavior. Health fields are observations, never native health writes. It is explicitly armed by the development harness and ends disabled. Existing native Steam loopback evidence does not establish separate-account online play.

## Exact files and reproducibility

- [Candidate842 identity](../../work/movement-integration/candidate-npc-stream-v842/identity.json): DLL `F2331469DC16DBB1B226EECAD68988129B03CE30F3C38EDA0EC6D2CB1E8F6747`; probe `EA5444F1ECEDED2937AEE80E21AA7F5609AA03536B797D82B92FF27C28944489`.
- [Protocol](../../outputs/d2-coop-lab/src/npc_authority_protocol.h), [stream](../../outputs/d2-coop-lab/src/npc_authority_stream.h), [protocol tests](../../outputs/d2-coop-lab/src/npc_authority_protocol_tests.h), and [native lease](../../outputs/d2-coop-lab/src/npc_authority_hold.h).
- [Build842](../../work/movement-integration/Build-NpcStream-v842.ps1), [stage842](../../work/movement-integration/stage_npc_stream_v842.py), [deploy842](../../work/movement-integration/Deploy-NpcStream-v842.ps1). Immutable840 and verified837/831 milestones remain available.
- [Current harness](../../work/natural-moveset/Test-NpcStream-v843.ps1); actual accepted invocation: `-Take npc-stream-v846 -ViewTutorial`. Each take saves the exact script used. Always use a new take name.
- [Camera recovery844](../../work/natural-moveset/Recover-NpcCamera-v844.ps1), [recovery receipt](../../work/natural-moveset/npc-camera-recovery-v844/result.json), [publisher/auditor](../../work/movement-integration/publish_npc_stream_v846.py).

The exact original executable remains `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`. No Steam DLL, game asset, first-person animation or saved baseline is replaced. Movement/action protocol69, bank67,357 clips and the633 Corvo/Emily bank hashes remain unchanged. Tests validate isolated process path, creation time and DLL identity while holding `Local\D2CoopLab_NativeOperation_01a11a65`.

## Wire format and native execution

NPC protocol2 uses separate magic `0x414e3244`,112-byte header,496-byte control packets and176-byte pose prefix. Each compressed joint is14 bytes: four signed16 quaternion components, normalized when decoded, and three signed16 translations at10000 units/m. Translation is bounded to3m. A223-joint packet is3298 bytes;224 joints use3312 bytes. The internal pose remains full float. Exact rig identity hashes native model/skeleton names, parent hierarchy and bind transforms, excluding process-local intern IDs and scratch lanes.

[Census839](../../work/natural-moveset/npc-rig-census-v839/summary.json) captures both roles' five rigs. [Offline compact audit842](../../work/natural-moveset/npc-rig-census-v839/compact-wire-audit-v842.json) measures all2232 sampled joints: maximum translation error about0.05mm and angular error about0.00349 degrees. Those numerical bounds are format checks, not visual acceptance.

Hello/Ready/Stop carry spawner, definition, rig, session, map epoch, per-actor arm, handle and sequence. Client acquisition happens only after compatible host identity. All native operations validate process-local actor/component owners and generations, living state, active gameplay and owned AI references. The frame hook suppresses independent client parent-root integration while updating native child frames. Source final skeletons are read at20Hz. Client root placement uses `0x14087d200`; body application on the owning animation callback uses `0x14009cfa0`, followed by `0x14009dc40`. Eight-pose buffers use80ms delayed interpolation; stale data, teleport and lifecycle changes reject or retire ownership.

The original five full-float streams exceeded the transport budget. Protocol2 reduced those NPC payloads to about330KB/s at20Hz; the existing native Steam connection now explicitly requests512KiB/s minimum/maximum send rate. Diagnostics include pending reliable/unreliable bytes and estimated queue time. This is not a solution for arbitrarily many active NPCs; interest/rate selection remains necessary.

New tests cover malformed control fields, role/authority and life/menu rejection, compact round trips and bounds, truncated packets, invalid quaternions, rigid transforms, source-time interpolation, stale samples, teleports and rig fingerprints. Existing world/action/movement/checkpoint tests, both real banks and proxy smoke passed the842 build. Native callback visibility and manual frame inspection are required in addition to numerical tests.

## Failed and insufficient takes retained

| Take | Finding and correction |
|---|---|
| 841 / candidate840 | Five actors negotiated but full-float bandwidth queued excessively; pose timeouts. This is not a passed stream. Compact842 replaced this format. |
| 843 / candidate842 | All five compact streams sustained, but cameras faced the player room; NPC bodies were culled. Root/transport evidence only. |
| 843b | Harness used actor-root XYZ for native `setviewpos` eye coordinates and reversed yaw/pitch. Host fell below floor, reached native game-over, and the stream retired with `world_or_pair_changed`. Raw restoration failed. This is a test-harness error; separate844 recovery restored both isolated SLOT81 fixtures, sword setup and note inventory. |
| 845 | Camera coordinates fixed, but patrol had moved away from a hardcoded target. Raw PASS incorrectly used lifetime body totals although all current-arm body counts were zero. Preserve raw output and reject it as body proof. |
| 846 | Harness now observes the patrol's current root, places cameras behind it after ownership negotiation, checks floor height/health and requires fresh per-arm body callbacks. Accepted bounded presentation result. |

Cleanup now sends `off` independently to both roles before waiting on captures; capture or first-role failure cannot skip the other release. View restoration is refused after a life/epoch change. Per-role verification errors are collected without skipping the other game. Native camera command order is eye X,Y,Z,yaw,pitch, as used by the established sword fixture. Do not reuse candidate-root height as camera height.

## Persistent analysis and continuation

Ghidra bookmark `D2Coop_NpcStream_846` at `0x14009cfa0` records the runtime evidence and limitations; [saved response](../../work/handoff-npc-stream-bookmark-v846.json) confirms program save. It extends the existing decompilation/native setter findings; it is not a new claim to have fully decompiled NPC behavior. AI pause/resume analysis and exact APIs remain in [chapter70](70-generic-npc-binding-and-ai-ownership.md).

Next work is continuous lifecycle: release on menus/checkpoints, negotiate fresh actor incarnations after return, include new/despawned actors and validate the remaining visible rigs. Then extend general combat and world outcomes rather than adding separate consumable handlers. Keep host gameplay authority explicit and prevent duplicated damage events. Functional movement/VFX gaps, shared progression/campaign, native session UI and separate-account Steam validation remain open.
