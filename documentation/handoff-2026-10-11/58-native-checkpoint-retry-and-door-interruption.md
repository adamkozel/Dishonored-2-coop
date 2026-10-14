# Native checkpoint retries while doors are moving

This continues [ordinary-door catalogue and traversal work](57-native-door-catalog-and-traversal.md). The experiments use the isolated Dunwall fixed-SLOT82 checkpoint, exact executable SHA256 `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`, native Steam transport, the existing local first-person player, and native loading/menu presentation. This is not campaign-wide checkpoint acceptance.

## Established runtime evidence before the retry correction

Both native copies of `ld_door_350cm_int_rich_wood_01_3` must be in Moving when the source requests a shared checkpoint. The other player immediately sends a duplicate memory-checkpoint command. The test waits for both ready tokens, role restoration, a new world epoch, exactly one restored counterpart, and retirement of all six old-world door channels. It reacquires the new door by name and components instead of reading an old-world pointer. The checkpoint door must remain closed, with no replayed old-world native door request.

| Take | Candidate | Outcome |
| --- | --- | --- |
| [736](../../work/natural-moveset/native-door-reload-host-v736/result.json) | 728 | Retained harness failure: the old fixed 800 ms wait sampled before counterpart restoration finished. Both loads completed; full inventory/health restoration and 14 protected hashes passed. |
| [737](../../work/natural-moveset/native-door-reload-host-v737/result.json) | 728 | Host-requested interruption passed after replacing that sleep with actual counterpart readiness. Both held for roughly 1.5 seconds with zero measured displacement. |
| [738](../../work/natural-moveset/native-door-reload-client-v738/result.json) | 728 | Real failure: client loaded and waited; host never reached loading entry, then both failed into native pause. This is not a passing checkpoint result. |
| [741](../../work/natural-moveset/native-door-reload-client-v741/result.json) | 740 | Client-requested interruption passed. New assertions count one actual native loading entry and finish in each process, in addition to logical transaction counters. No busy-return branch occurred in this particular run. |
| [743](../../work/natural-moveset/native-door-retry-probe-client-v743/result.json) | 742 | Retained harness timing failure before reload: command consumption was incorrectly treated as immediate diagnostic readiness. Cleanup then released AI while capture finished; an ordinary guard killed the source. Both inventories/health and protected hashes were restored. This is not shared-death evidence. |
| [744](../../work/natural-moveset/native-door-retry-probe-client-v744/result.json) | 742 | Client-requested strict probe passed after polling actual probe readiness before opening the door. Four native deferrals followed by one acceptance per role, one native entry/finish each. |
| [745](../../work/natural-moveset/native-door-retry-probe-host-v745/result.json) | 742 | Host-requested strict probe passed with the same four deferrals then one acceptance per role. |

The failed 738 logs, status, and reset snapshots are in [door-reload-failure-v738](../../work/movement-integration/door-reload-failure-v738). Its native callback trace was not copied before restart, so the exact original cause cannot be claimed retrospectively. The recovery restarted only the identity-checked isolated executables and restored SLOT81 and the native sword fixture. A reporting-only PowerShell typo (`True` instead of `$true`) interrupted the first recovery report after restart; subsequent verification completed without another restart. [Recovery receipt](../../work/movement-integration/door-reload-failure-v738/recovery.json) confirms both inventories, health, fresh connectivity and 14 unchanged protected files.

Source and observer contact sheets for 736, 737, 738 and 741 were inspected. They show native loading/return for the successful cases; 738 shows the client waiting at loading while the host remains in the room, followed by native pause. Capture startup generally misses the pre-load Moving interval, which is proven separately by native component observations. Shared checkpoint spawn positions can put the counterpart too close to the first-person view. These recordings do not establish acceptable body animation or spawn separation.

## Exact-executable decompilation and assembly

[Full transcripts and independent disk disassembly](../../work/movement-integration/checkpoint-continuation-v740) preserve both useful results and tool limitations. Ghidra's function lookup for the shared implementation at `140659190` reports only a one-byte body; its xref query returned a data reference and its disassembly endpoint returned empty text. The decompiler follows the tail jump from `14065db00`; existing local Capstone 5.0.6 verifies the actual pinned executable bytes independently. Do not interpret the incomplete Ghidra function/xref index as absence of native callers.

| Address / field | Confirmed interpretation |
| --- | --- |
| `14065db00`, tail jump at `14065db63` | Native `RestartFromMemoryCheckpoint`; enters shared implementation `140659190` with mode 2. |
| `game + 2e75d8` | Native save manager used by the checkpoint callbacks. |
| `1406591e6`, `1406591f9`, `140659201` | Busy guards: operation flag, native I/O readiness helper, and lower 24-bit count at save manager +128. |
| `14064efb0` | I/O readiness helper checks operation state +108 and native storage conditions; the new diagnostic reports fields without calling or modifying the helper. |
| `14065993c`, string `141c34788` | Busy mode-2 path queues `RestartFromMemoryCheckpoint`. |
| `140659950`, string `141c35114` | Same path queues native `wait 1`. |
| `14065997c` | Deferred path clears AL before returning. |
| `1406598c7`, `14065990b` | Accepted path sets BL to one and returns it in EAX after queuing the native transition. |
| `142318198` | Cached native slot identifier, required to be 82 by the mod's continuation gate. |

The old gate consumed its one continuation permission when dispatching the callback. Dispatch can return zero and ask the native command buffer to retry; consuming permission then can swallow the retry needed to enter loading. This is a confirmed implementation mismatch with the native routine. It is a plausible explanation for 738, whose missing callback trace prevents a specific causal claim.

## Correction and bounded regression probe

[Candidate 740](../../work/movement-integration/candidate-checkpoint-retry-v740/identity.json) captures the native memory callback's low return byte. Zero preserves the existing continuation permission; one retires it. Actual native loading entry also retires permission. The cached slot must still be 82, the active game must match the pre-load game, and the shared transaction must still own the boundary. Unexpected return values fail the transaction. Opcode checks pin the return convention before arming. Packet formats, Steam transport, role restoration, native first person and movement banks remain unchanged.

The trace ring now records 128 callbacks, consumed/forwarded state, return byte, cached slot, and observed save-manager/I/O fields. New harnesses preserve the trace before cleanup, including on failure. These field names describe observations; an I/O field alone is not claimed to explain every native busy condition.

[Candidate 742](../../work/movement-integration/candidate-checkpoint-retry-v742/identity.json) adds an explicit, one-shot `coop_checkpoint_sync retry_probe`. It requires the armed matching checkpoint pair and an active isolated movement fixture. For the next load within ten seconds, with unchanged arm/world, it appends exactly two fixed native memory-restart commands behind `LoadGame 82`. It does not modify save-manager memory or inject a fabricated busy state. Normal runs do not arm it. The strict regression must observe a real zero return followed by one acceptance, exactly one native entry/finish, and the original restoration/protection checks.

The [746 audit](../../work/movement-integration/native-checkpoint-retry-audit-v746.json) verifies four passing receipts and preserves all three failures. Both 744 and 745 observe native I/O states 18, 20, 21 and 22 returning AL=0, followed by state 0 returning AL=1. Each process enters and finishes native loading exactly once, waits for both ready tokens, restores the intended role and one counterpart, retires all old-world door channels, and restores inventories/health with 14 protected save/settings hashes unchanged. Both source and observer contact sheets were inspected; the visible sequence is native loading and Dunwall return. No body-animation acceptance is claimed. Fourteen actual videos from these seven takes are indexed, including failures.

Verified live candidate 742 DLL SHA256 is `A59AF758A40FC67F2EB35595A2F352FFA371025F9B30EA3681E87C7C8CC2AD85`; diagnostic runtime probe remains `1ECB428D77B93915EA0417F3531E9103BB8E24DEC2336E874A809BA827810B22`. Candidate 740's ordinary client reload is retained separately with DLL `2E750FF68CCCD3A7010F9F9E85B68CD82D6B4C3D27B78408DC138829EC3921EA`. Build checks cover native identity/ownership, checkpoint and action protocols, presentation resume, proxy smoke, and both actual 357-clip banks. The saved Ghidra `NativeCheckpointRetry` bookmark includes the runtime findings.

The working retry harness now keeps the isolated AI hold until captures have finished before disabling the fixture. Historical run-script copies and failed videos remain unchanged. This closes the cleanup gap observed in 743; it does not count as another runtime acceptance case.

## Routine scene setup

`work/PrepareShadowSword.ps1` now requests the native skip when both games report the same skippable scene and segment. It verifies one host native confirmation, one Steam peer application, and no direct peer input change. The `-PlaySetupScene` switch retains full playback for dedicated cinematic tests. Recent ordinary setup receipts finish the scene step in about four seconds. This optimization does not accelerate the game clock.

## Remaining work

Resume overlapping door inputs and authored lock/key/group/script handling. Door channels currently retire on world change; automatic rebuilding after a shared load is not covered. General campaign saves, failure recovery without restarting, safe shared spawn separation, shared pickups/world authority, complete movement coverage, native session UI and real separate-account Steam sessions remain outstanding. Keep animation polish secondary to functional coverage.
