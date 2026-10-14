# Ordinary doors: source behavior and native state ownership

This bounded investigation uses live candidate635, protocol69 and bank67/357. It does not add a new production animation or claim shared doors. See the [644 audit receipt](../../work/movement-integration/native-door-research-v644.json), [retained research files](../../work/movement-integration/door-research-v639) and [641 harness](../../work/natural-moveset/Observe-NativeDoor-v641.ps1).

## Recorded behavior

- [Corvo638](../../work/natural-moveset/native-door-corvo-v638/result.json): ordinary short-X at the office door produces the native “Royal Quarters Key needed” message. It is a failed locked interaction, not an open action.
- [Corvo641](../../work/natural-moveset/native-unlocked-door-corvo-v641/result.json) and [Emily642](../../work/natural-moveset/native-unlocked-door-emily-v642/result.json): two ordinary short-X inputs open and close the adjacent unlocked door. Independently sampled native source Openable states traverse **0 → 1 → 2 → 1 → 0**. The other process's door remains closed throughout.
- The player is sampled in native Running0, briefly accompanied by Interact75 and, around the second tap, Undraw81 (VT RVA1c7df88). No separate first-person reach or player Midnight scene was observed. Existing body locomotion and equipment overlays are appropriate for these ordinary taps; do not force the authored door gesture candidates merely because the row is named “door.” This finding does not cover all doors or interactions.
- Both successful tests restore complete checkpoint inventories, player/partner health and all14 protected save/settings hashes. Original installation and current DLL/bank are unchanged.

The source recordings visibly show the native door leaf opening and closing. Observer recordings show a separate closed door; the avatar is visible initially and later obscured/absent. Its root and body telemetry alone do not establish continuous visual success. The cause of the disappearance has not been established. Native visibility/portal behavior is a hypothesis only. No visual acceptance or shared-world completion is claimed.

## Reproducible fixtures

The ordinary locked office door is `ld_door_350cm_int_rich_wood_01` at approximately `[-36.9,-153.55,66.4]`. Source camera `setviewpos -35.9 -153.55 68.1 180 0` faces it.

The adjacent unlocked door is `ld_door_350cm_int_rich_wood_01_3` at approximately `[-47.05,-164.5,66.4]`. Source camera `setviewpos -47.05 -163.5 68.1 270 0` faces the +Y side. Observer camera is `[-44,-161,68.1]`, yaw220/pitch10. Both use the existing isolated Dunwall checkpoint. Reacquire entities by exact native name and component ownership after every reload; archived addresses and handles are not stable across incarnations.

[640](../../work/natural-moveset/native-unlocked-door-corvo-v640/result.json) failed its harness lifetime guard after opening the door. PowerShell literal `0xffffffff` was signed -1 and did not strip changing flags above the component generation.641 explicitly masks with unsigned decimal4294967295. The failure is retained; its restoration checks passed. Its observer capture was reported incomplete and is not upgraded to accepted evidence.

## Exact-executable findings

Preferred image base is `0x140000000`. All findings target the pinned installation EXE SHA256 `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`. Ghidra bookmarks and program were saved. `decompile-*.txt` in the research directory retain pseudocode; dynamic snapshots retain their own process pointers.

| Object/offset | Evidence and meaning |
|---|---|
| ArkDoor VT RVA1cb7c68, +370 | Own Openable component pointer, reciprocal owner+10 validated in runtime snapshots. |
| Openable VT RVA1c569c8, +18/+1c | Generation DWORD then changing flags DWORD. Do not compare the combined64-bit word as generation. |
| Openable +40/+48/+4c | Embedded state machine, current and pending states. Ordinary runtime correlation:0 closed,1 transitioning,2 open. |
| Openable +dc/+f8 | Generation-bearing handle of spawned visual child, cached native AnimBase component. Door rendering is not simply its parent transform. |
| Openable +1e0..1e7 | Native animation entry IDs resolved from declaration animation names during initialization. |
| Openable +1e8 | Lock byte. Correlated with locked/unlocked UI and static lock routine. Other bytes in this region are distinct flags. |
| Player state machine +318 | PlayerInteraction component used by Interact75. |
| UseInteraction state +30/+38 | Generation-checked target component reference, copied from PlayerInteraction+30/+38. |

| Function | Confirmed static trace; runtime scope remains as stated above |
|---|---|
| 140870e10 / 140871250 / 140870fe0 | Interact75 entry/update/exit; hold timer at state+44 and binding+40. |
| 140871020 | Selects short descriptor at interaction+44 or hold descriptor+70, queues player mode/state via a type5c request. Use75 alone does not prove success. |
| 1403da910 / 1403dab80 | Resolve/set native component references using generation and active checks. |
| 14074b1c0 | Openable interaction descriptors: closed/locked decl+f8, closed/unlocked decl+a8, open decl+d0, optional hold decl+148. |
| 14074cd90 | Resolves visual child/AnimBase, native animation entries, linked components and events. |
| 14074c5c0 / 14074e7a0 / 14074cd10 | Moving-state entry/update/exit; native animation, contact/event, sound and navigation side effects accompany transitions. |
| 14074ca10 / 14074cba0 | Closed/open state entry notifications and native sound/navigation updates. |
| 14074e4c0 | Changes lock byte, emits native events, dirties owner/child, optionally recurses into linked Openable with a guard. No direct runtime invocation tested. |
| 14074a960 | Door contact damage processing; **not** a generic open/close function. |
| 14074e3c0 | Movement/contact flag propagation and contact-list cleanup; **not** the unlock routine. |
| 14074e080 | Native Openable serialization. Initial declaration enum at+d8 is not automatically the same numbering as current machine state. |

Generic128-word `--object` snapshots can extend beyond the allocation; do not infer more fields from adjacent objects. There is no justification for writing arbitrary copied state or calling an unidentified function.

## Remaining work and priority

Shared native door outcomes, lock/key changes, linked doors, breakage, script effects and continuity across area visibility/save-load are unfinished. Future world integration must preserve native event/physics semantics and establish authority/idempotence. Current work returns to batched functional movement coverage in the established pipeline. Fine pose/contact polishing stays deferred for later animator review.
