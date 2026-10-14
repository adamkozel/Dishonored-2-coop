# Work performed while creating this handoff

The runtime work continued alongside documentation rather than being suspended for a separate documentation milestone.

## v30 carry stance correction

Corvo crouched-to-standing and Emily standing-to-crouched tests now pass their scoped lifecycle, orientation, native low-drop and load-target checks. Actual D2 temporal sheets were inspected and both takes indexed into the review dashboard. The candidate archive is marked runtime-validated only for those named cases; visual acceptance and complete carry remain false.

- [Corvo sampled v30 take](../../work/natural-moveset/native-crouched-to-standing-carry-corvo-v30-sampled): 51 checks; final shoulder target residual 0.01708 m; supporting wrist residual 0.10524 m; relative body-yaw drift 0.269 degrees.
- [Emily v30 take](../../work/natural-moveset/native-standing-to-crouched-carry-emily-v30): 48 checks; shoulder residual 0.01685 m; wrist residual 0.03895 m; yaw drift -0.017 degrees.
- [First Corvo v30 take](../../work/natural-moveset/native-crouched-to-standing-carry-corvo-v30): retained failed carrier validation because preparation transmission was not sampled, despite passing later geometry/orientation checks. It was not reclassified as a pass.

The v30 source remains in [candidate-carry-stance-anchor-v30](../../work/movement-integration/candidate-carry-stance-anchor-v30). The latest continuation was corrected to its actual DLL hash, and the Ghidra bookmark `D2Coop/CarryStanceAnchor-v30` was saved at `140757830`.

## Initial native body throw investigation (superseded by the continuation below)

The bounded native first-person observation uses a normal X pickup, levels the camera after the body reaches the native loaded loop, then sends right trigger at approximately seven seconds. It observes the real native state machine and Midnight scenes; it does not inject a remote throw.

[Corvo observation](../../work/natural-moveset/native-body-throw-corvo-v1) and [Emily observation](../../work/natural-moveset/native-body-throw-emily-v1) completed and recorded `carry_corpse_in`, `carry_corpse_loop` and **`carry_corpse_drop_high.midnightscene`**, Scene_1, duration 666 ms, with carry/drop states absent afterward. Both actual video sheets were inspected; the native throw prompt, forward release and sword recovery are visible. The fixture faces a nearby wall, so it does not validate an unobstructed trajectory.

Both strict all-samples state/scene checks remain failed: the first sequential read crosses the Carry45-to-Drop46 transition, while the later samples show Drop46. The failed reports are retained. This is a read-timing limitation requiring a bracketed source reader before extending the whitelist; it is not silently counted as a passed complete throw test.

Fresh decompilation of `140832770` confirms selector-dependent scene lookup, storing the selector at Drop46+0x54, body handle at +0x38 and native body/physics flag updates. Mapping high-drop to selector 2 remains a hypothesis. The current whitelisted carry reader intentionally refuses the high-drop variant. No remote throw or impulse replication was added. Full findings are saved in [native-throw-findings-v1.json](../../work/natural-moveset/carry-research/native-throw-findings-v1.json) and the Ghidra project. Both source-reference recordings are labelled explicitly in the dashboard.

## Documentation artifacts

Subsequent high-throw work added a guarded source reader, source camera-derived velocity, protocol31, the234th bank clip, a victim-only native scene and isolated release experiments. v31/v32 failed to queue the impulse. v33 fixes ordering by requesting ragdoll after scene retirement and passes23 pickup+21 throw checks on each character; actual videos show forward flight, with hand/body quality still unaccepted. v34 investigates the newly traced native retained attachment. The [full continuation chapter](13-native-body-throw.md) records exact builds, failed comparisons, alternate native soft-drop selection, asset exports, decompilation, actual-video evidence and remaining work. Earlier claims that selector2 was only a hypothesis or that no remote throw existed refer to the initial observation milestone above.

The handoff includes authored chapters, an exact requirement-by-requirement roadmap, full saved finding transcripts, exported Ghidra bookmarks/function documentation, current source and native-address indexes, historical milestone/state records, test/capture script inventories, recording provenance and a SHA-256 source manifest. Generation and link validation are reported in the generated audit file. The original Steam installation, saves and recorded failed comparisons are preserved.
