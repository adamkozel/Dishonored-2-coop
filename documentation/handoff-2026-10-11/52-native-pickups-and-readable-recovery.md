# Native pickups and readable-screen recovery

Continue functional coverage through the existing authoring/runtime pipeline. The user accepts deferring fine animation polish to an animator. Preserve editable sources; do not resume third-party co-op mod research or claim the movement phase complete.

## Verified standing coin pickup

Candidate666 fixes a concrete source-state gap. Corvo664 entered native Running0 + LootObject47 + Undraw81 during ordinary coin pickup, which the prior movement classifier rejected. The new classifier accepts only the exact standing/default-mode combination, with Running0 required and Undraw81 optional. It does not infer crouched, airborne, combat, corpse-loot or pickpocket behavior.

Both Corvo667 and Emily668 pass 20 total scoped checks in [the reproducible audit](../../work/movement-integration/audit_loot_motion_v668.py), with [results](../../work/movement-integration/loot-motion-audit-v668.json). Each has 111 slower network/body observations. Compact read-only monitoring recorded 639 and 637 native samples respectively. Both forms of LootObject47 were captured; all compact samples remained valid. Receiver body weight stayed above0.999, both native Steam presentation streams stayed ready, the exact source coin disappeared, and the source returned to Running0.

Both source and observer contact sheets were inspected. The source views show the native coin prompt, removal and currency notification. The observer views show continued standing presentation without a separately authored reach gesture. Corvo's view includes a stationary guard beside the actor. These views do not establish fine grip quality or shared inventory/world removal. No coin or inventory event was synthesized on the receiver.

Evidence directories:

- [Corvo667](../../work/natural-moveset/native-interactions-corvo-coin-v667/)
- [Emily668](../../work/natural-moveset/native-interactions-emily-coin-v668/)
- [Candidate666 immutable pins/source/binaries](../../work/movement-integration/candidate-loot-motion-v666/identity.json)

Protocol69 and the existing bank67/357 clips remain unchanged. The local first-person animation and interaction remain native.

## Readable screen discovery and candidate670

Corvo669 ordinary X opened the native **Ship Launch Protocol** note. Its read-only native logic stack was `[InGame1, ReadNote4]`, pending0, with `ReadNote` vtable RVA `1c61e68`. The prior fixture and movement-resume policies covered pause2 and state13 only. The label "wheel13" was later found incorrect:13 is native GameOver. Candidate779 removes it; see [the correction and checkpoint barrier](62-continuous-checkpoint-death-recovery.md). The NPC fixture retired with `scope_changed`, so the harness aborted before its planned B close. The original failure receipt also records checkpoint restoration failing while the note remained open. Preserve that failure.

Recovery used ordinary B, followed by the existing isolated checkpoint restoration. [The separate recovery receipt](../../work/natural-moveset/native-interactions-corvo-note-v669/recovery-after-close.json) verifies both inventories, health and14 protected files. The source video was inspected and confirms the native note UI. It is not a successful pause/resume proof.

Candidate670 adds exact ReadNote4 to the existing same-world **suspension** and finite NPC fixture retention policies. `sampleLocalPlayer` validates its exact native vtable. Reading does not become gameplay-ready. The existing pinned actor/world/session identity, fresh-packet requirement, new stream tokens,300ms readiness settling and bounded recovery remain intact. No reading pose or scene is fabricated. Build, resume-policy, native ownership, action, proxy and actual-bank tests pass; runtime results must be read from subsequent recordings and audits.

- [Candidate670](../../work/movement-integration/candidate-read-note-v670/identity.json)
- [Current note harness671](../../work/natural-moveset/Test-NativeInteractions-v671.ps1)
- [Failed669 record](../../work/natural-moveset/native-interactions-corvo-note-v669/result.json)
- [Native logic query669](../../work/natural-moveset/native-interactions-corvo-note-v669/logic-after-failure.json)

671/672 now verify automatic recovery on both roles: [20 scoped checks](../../work/movement-integration/readable-resume-audit-v672.json). Both source and observer sheets were inspected. The actor disappears while the note is open, then returns; full body weighting recovers3.65/3.74s after the first gameplay sample. This is a recovery result with a visible pause defect, not complete reading presentation.

673/674 then retained the already visible native clone during exact ReadNote and same-world recovery. The model remained visible in all79 telemetry observations and inspected observer frames. However, keeping `networkFrameOwned=false` through semantic-bank recovery prevented the semantic handshake itself, whose native pair check requires root ownership. Thus674 is a **failed movement-recovery comparison**, despite its execution/restoration receipt passing. Do not promote673.

676 corrects this dependency: keep visibility during ReadNote and while follow is invalidated, then end the frozen hold once `enableProbeFollow` has validated and restored fresh native root ownership. The ordinary Steam semantic handshake and pinned bank can then resume. No new pose, root or gameplay outcome is applied while the readable hold is active. The hold requires the explicit armed resume identity, current peer packets, exact same-world pair and an existing verified native AI pause reference. Load/death/cutscene/foreign logic, lost ownership and stale peers retain ordinary retirement. Tests cover these exclusions. Runtime verification is the next required evidence.

- [Failed visibility/recovery take674](../../work/natural-moveset/native-interactions-corvo-note-v674/result.json)
- [ReadNote visibility policy](../../outputs/d2-coop-lab/src/native_readable_pause.h)
- [Latest reading-to-walking harness677](../../work/natural-moveset/Test-NativeInteractions-v677.ps1)

## Failed and partial experiments retained

| Take | What actually happened | Limit |
|---|---|---|
|662|Food/coin camera assumed an eye height that collision invalidated. One stream retired; note aiming aborted.|No successful target activation accepted.|
|663|Feedback-controlled aiming reached fruit. Native focus selected neighboring grapes, which disappeared; the next prompt was Pear. Both food videos inspected.|Not the planned apple. Brief body blend dip to0.689115. Later coin startup failed; whole batch failed. Receiver fruit stayed present.|
|664|Corvo coin prompt/removal and native LootObject47 observed.|Old classifier rejected loot movement. Baseline only.|
|665|Emily was attacked by guards during recording.|Invalid coin comparison despite the old harness's execution/restoration PASS. Earlier Ghidra wording was corrected; no accepted Emily pickup here.|
|667/668|Guard isolation acquired and verified before approach; coin tests pass on666.|Stationary standing coin only; world outcomes and fine polish remain open.|
|669|Native ReadNote4 activated; unsupported pause state retired the fixture and aborted the test.|Separate recovery succeeded; no accepted automatic resume.|

## Reproducible testing and safety

[Aim helper663](../../work/natural-moveset/Set-NativeInteractionAim-v663.ps1) reads the actual camera and uses ordinary native controller look to settle the aim. Furniture can alter actual player height after `setviewpos`; do not restore the earlier fixed-eye assumption.

The five exact native NPCs must be held **before** approaching the balcony pickups. The fixture refuses already-attacking guards. A command-consumed receipt is not a successful hold: require `ready` and10 held decision/reflex references, enough remaining lease, and unchanged player health.669 also exposed the need to close a known native note screen during cleanup before checkpoint restoration;671 implements this.

`d2_runtime_probe PID --watch-player-states 10000` is a finite, read-only diagnostic added in666. It holds one process handle, polls at10ms requested cadence and emits only changed states, including invalid periods. Windows observed cadence was roughly64Hz, not an asserted100Hz. It does not force state transitions or invoke pickup callbacks. Gameplay's existing source sampling remains separate.

Serialize every build/deploy/runtime test with `Local\D2CoopLab_NativeOperation_01a11a65`. Consume the previous process and its cleanup before the next native operation. Use fresh launch PIDs, exact candidate hashes, isolated `work/asset-game`, and14 protected save/settings hashes. Original Steam executable and DLL are never modified.

## Exact-executable analysis

The Ghidra program is the verified Steam executable, SHA256 `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`.

- LootObject47 vtable RVA`1c7bcc8`, update slot+68 -> VA`140841500`. [Saved decompilation](../../work/movement-integration/decompile-loot-update-140841500-v666.txt) iterates the state+f0 object-handle array with+f8 low24 count and performs native pickup callbacks. It can branch into other states for other object types. This is evidence against treating every loot case as the short coin case.
- Undraw81 vtable RVA`1c7df88`; it is not Fidget.
- ReadNote4 vtable RVA`1c61e68`. Native RTTI and runtime stack confirm the class. VT+8 -> VA`1407907d0`; [saved decompilation](../../work/movement-integration/decompile-readnote-1407907d0-v670.txt) caches native component references at+60/+70/+80. Their semantics were not guessed or used for mutation.
- Persistent bookmarks use category `D2Coop Native Interaction`; save the program and merge new bookmarks with the existing audit export, never replace it with a category subset.

Remaining work includes food continuity, target-specific interactions, crouched/other loot variants, shared inventory/item changes and other movement contexts. Fine acting is deferred, not counted as completed functional coverage.

## Readable root retention681 and runtime audit684

676/677 fixed recovery and disappearance but did not freeze the native frame. The clone drifted roughly0.49m horizontally and0.46m vertically while the source remained stationary. Its visibility and execution PASS were insufficient evidence of correct positioning. The original677 videos and observations are retained.

[Candidate681](../../work/movement-integration/candidate-readable-body-v681/identity.json) keeps existing reference-frame ownership during the exact readable hold. It additionally verifies the frame pointer, owner, native vtable RVA1c573e8 and already active network ownership. Both transport invalidation and the per-frame follow path preserve that existing ownership during the same guarded hold. No fresh root, action, damage or body pose is applied from ReadNote. All existing identity, freshness, load/death/cinematic exclusions remain. The resume path revalidates normal follow, creates fresh Steam tokens and reloads the pinned bank. Native first-person behavior remains unchanged.

[Corvo685](../../work/natural-moveset/native-interactions-corvo-note-v685/) and [Emily683](../../work/natural-moveset/native-interactions-emily-note-v683/) pass [28 functional checks](../../work/movement-integration/readable-resume-audit-v684.json), reproduced by [audit684](../../work/movement-integration/audit_readable_resume_v684.py). They contain108/109 observations and35/32 exact native ReadNote samples. Reading root error and drift are both0 at recorded precision. Both bodies remain visible, recover with new stream tokens and the unchanged bank, and ordinary backward input produces matching final source/remote roots. Full body weight returns3.705/3.619s after the first gameplay sample. Both complete inventories, health and14 protected files are restored.

All four source/observer contact sheets were inspected at0.5/2.5/4.5/5.5/9.5/12.5 seconds. Source videos show the native note UI, normal close and backward movement. Observer videos show retained bodies and later movement. There is no authored reading gesture: the retained native standing pose can change while the root stays fixed. Furniture height/contact remains coarse because the fixture puts the source beside/on the tabletop edge. These results do not accept final acting, campaign-wide reading behavior or shared document collection.

**Preserved failed run682:** Corvo first recovered successfully around8.155s, then the strict250ms root-age check retired the stream around10.65s. The harness failed before its planned backward input. Both source/observer sheets were inspected; all cleanup checks passed. Subsequent Emily683 and Corvo685 pass without changing the stale-data threshold. This is a remaining transport/stability issue, not a failure to retain the root while reading, and must remain visible in audit reports. Do not describe all runs as passing.

The audit's NPC isolation/follow assertion uses fields actually present in the recorded status. `avatarAiHeld` exists in separate reset telemetry, not the primary bridge JSON; do not infer it from a missing primary field. The implementation validates the native AI reference independently.

Current immutable DLL pin: `10487E1F5F5DBC07F787996144D7A75CD290E8C5A61F05EBD0D02693075BC6D1`. Probe: `FE149B345CCA91B0285D0CC6AE7BBF115F3877A5DC56FE189F560B4FD98AFB09`. Protocol69 and bank67/357 unchanged. Build, native ownership, action, resume-policy, smoke and actual bank tests passed. Ghidra's persistent ReadNote bookmark is saved with the confirmed observations and failed history.

Routine cleanup now uses the [verified native scene skip](53-native-setup-scene-skip.md). Dedicated playback tests can use `PrepareShadowSword.ps1 -PlaySetupScene`.
