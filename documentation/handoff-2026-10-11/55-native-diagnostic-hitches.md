# Native diagnostic hitches and presentation continuity

This continues the intermittent failures recorded in [chapter54](54-crouched-loot-and-native-food.md). The working693 candidate remains archived. Protocol69, bank67 and all357 clips are unchanged by700/702. This work does not finish the movement phase or provide automatic recovery after an expired stream.

## Confirmed measurement

Candidate700 adds bounded timing scopes on the native game thread, with128 stage counters and32 recent events of at least50ms. Scopes overlap; do not sum nested values. Timings include scheduling delays, and do not identify disk hardware or antivirus as the underlying cause.

The exact Corvo crouched coin test701 reproduces the existing native interaction and records both game views. During its roughly13-second measurement, `status_all` consumes3371ms on the source and3300ms on the observer. Recent periodic status calls take roughly55–65ms, including93ms source and108ms observer spikes. Source semantic sampling is separately costly, about2135ms total inside `sendShadowStream`; it is not optimized in702.

This confirms periodic diagnostic batches stall the native game thread. It does **not** establish that every previous250ms gap had this cause. Test701 itself retains Steam readiness and full body weight above0.9998.

## Implementation702

- Seventy-one diagnostic writers run in round-robin order on the native game thread.
- Each invocation performs at most four writers, stopping after a4ms soft budget. One already-started atomic file write may exceed that budget.
- Each writer retains a minimum250ms interval. Under load the cycle may take longer; do not assume all files represent one simultaneous snapshot.
- Main status, movement, semantic Steam status, movement resume and NPC isolation keep their earlier update cadences. Main status still includes current world/player/scene identities.
- No worker reads native game memory. No transport freshness limit, ownership policy, damage callback, animation or gameplay behavior is changed.
- Scheduler tests cover fair rotation, independent deadlines, clock rollback and no catch-up burst after a delayed frame. Existing native identity/action/resume, proxy smoke and both actual animation banks also pass.

The scheduler reduces burst size; it does not eliminate total diagnostic cost or guarantee a hard upper bound on filesystem calls. A future release should reduce inactive diagnostics or serialize snapshots asynchronously after copying safe data on the native thread.

## Files and reproducibility

- Source: `outputs/d2-coop-lab/src/native_service_timing.h`, `diagnostic_schedule.h`, `engine_bridge.cpp`, `movement_resume_tests.cpp`.
- Preparation scripts: `work/movement-integration/prepare_native_timing_v700.py`, `prepare_diagnostic_slices_v702.py`. These are one-shot transformations; do not rerun against already-transformed source.
- Build/deploy: `Build-NativeTiming-v700.ps1`, `Deploy-NativeTiming-v700.ps1`, `Build-DiagnosticSlices-v702.ps1`, `Deploy-DiagnosticSlices-v702.ps1` under the same directory. Deploy staging is one-shot and requires the exact prior DLL.
- Immutable archives: `candidate-crouch-loot-v693`, `candidate-native-timing-v700`, `candidate-diagnostic-slices-v702` under `work/movement-integration`; each includes source, DLL, probe, bank hashes and identity.
- Runtime harnesses: `work/natural-moveset/Test-NativeInteractions-v700.ps1` and `Test-NativeInteractions-v702.ps1`, using unique takes701/703/704 and the same existing crouched coin fixture.
- Audit: `work/movement-integration/audit_diagnostic_slices_v705.py`.
- Native timings: `work/lab-instances/{host,client}/coop-status.json.native-service-timing.json`; snapshots embedded in the recorded interaction observations remain persistent after subsequent tests.

Runtime comparison and six contact-sheet inspections are complete; see the bounded results below. Original Steam files and protected saves remain governed by the existing18-file integrity verification.

## Final runtime comparison705

Audit705 passes32 checks:8 baseline and12 per new character test. In baseline701, source/observer record52/51 status calls of at least50ms. Both703Corvo and704Emily record zero such status or slice calls during their approximately13-second measurement windows. The budget is soft: startup/history includes individual slow writes, so this is not a universal latency guarantee.

Both new takes activate exact native Sneaking6 + LootObject47, remove the source coin and retain crouch, connected Steam presentation and full-body weight. Minimum observed weight is0.999396 forCorvo and1.0 forEmily. Both full inventories, health and14 protected save/settings hashes are restored. The original18-file integrity refresh runs separately.

All six source/observer contact sheets were inspected. The source coin notification/removal is visible; the remote crouched body remains present on the lectern. Rear viewpoints limit grip and foot-contact evaluation. Corvo703 includes an NPC walking nearby, with no measured health change. Static sheets cannot prove smoother motion or frametime; numerical native timing counters establish the narrower hitch result. No new looting gesture is claimed and peer item removal remains unfinished.

Live702 DLL: `AC0ADACDF58FBEC682D28788B01EE224C4D377C1944F657FE4C4A18770D5DB49`. Probe remains `1ECB428D77B93915EA0417F3531E9103BB8E24DEC2336E874A809BA827810B22`.

Next: continue functional movement/shared-world coverage. Same-identity recovery after other data gaps is still outstanding; do not treat logging changes as a general disconnect recovery implementation. Preserve all earlier failures and do not relax native actor/world/session, load/death or explicit-stop checks. Native semantic sampling remains another measurable cost to investigate if useful.
