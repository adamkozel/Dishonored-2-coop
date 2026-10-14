from pathlib import Path
import json
R=Path(__file__).resolve().parents[2]
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
p=R/'work/handoff-project-bookmarks-current.json'
merged={(b['address'],b['category'],b.get('type','Note')):b for b in read(p)}
for b in read(R/'work/ghidra-bookmarks-export-v227.json'):merged[(b['address'],b['category'],b.get('type','Note'))]=b
write(p,sorted(merged.values(),key=lambda b:(b['address'],b['category'])))
p=R/'documentation/handoff-2026-10-11/30-native-domino-investigation.md'
s=p.read_text(encoding='utf-8-sig');heading='## Native cancellation investigation, v226 onward'
if heading in s:s=s[:s.index(heading)]
s+='''\n\n## Native cancellation investigation, v226 onward

All tests preserve candidate222 and the original installation. No new production binary was deployed for these harness changes. Tests distinguish pre-existing native links from cosmetic beam retirement; the latter already passedv223/v225. Native X while holding LT is a hypothesis about cancelling established links until measured in both instances.

| Take | Outcome and reason |
|---|---|
| [v226](../../work/natural-moveset/domino-native-cancel-v226/result.json) | First guard was only0.4m from source; first cast aimed past him. Second guard marked, no beam, cancellation not reached. Early capture wait also incorrectly flagged the still-running40s receiver recording. |
| [v227](../../work/natural-moveset/domino-native-cancel-v227/result.json) | Guard moved from4.1m to0.6m after camera selection; new range guard correctly stopped before any cast. |
| [v228](../../work/natural-moveset/domino-native-cancel-v228/result.json) | Floor/yaw and stable native position verified, but eastern initial guard position lay behind a closed door from balcony source. First cast selected nobody. No beam/cancellation reached. |
| [v229](../../work/natural-moveset/domino-native-cancel-v229/result.json) | PowerShell array arithmetic error before casting. Reproduced locally and corrected by parenthesizing array elements. No recording started. |
| [v230](../../work/natural-moveset/domino-native-cancel-v230/result.json) | Same-corridor source point established two native targets and replicated beam. Harness failed before Cancel on malformed `Where-Object id -eq72`. Corrected to `-eq 72` and verified against saved native state data. |
| [v231](../../work/natural-moveset/domino-native-cancel-v231/result.json) | X was sent after exact state72 and two links were verified. Receiver ownership had not retired at700ms. Native source looked at floor, so video alone does not distinguish retained native links from a replication bug. |

All six final receipts include restoration of both isolated checkpoints and all14 protected save/settings hashes. Source/receiver temporal sheets for all recorded takes were inspected and indexed as failed tests; no acceptance was inferred from setup successes. The native AI reference hold does not prove a stopped scripted movement, visibility or attack immunity. Earlier cleanup released those references before the40s recordings ended, allowing native NPC reactions/damage in later frames; the checkpoint recovery succeeded. [v232 harness](../../work/natural-moveset/Test-DominoNativeCancel-v232.ps1) keeps AI references until recording completion and captures paired native links, native state, source/receiver FX, stream counters and body status after X before evaluating retirement.

### Further exact-executable static findings

[Saved decompilation v229](../../work/ghidra-domino-cancel-state-v229.json) and [v232](../../work/ghidra-domino-cancel-base-v232.json) preserve the new static evidence. `14068ec30` changes four native Midnight boolean inputs and triggers the phase update; this is not itself proof of network cancellation. `14068eee0` was previously uncreated in Ghidra: five bytes `c6 41 72 01 c3` set state+72 to1. A function was created as `D2_DominoState_SetSelectionReady`; that descriptive name is inferred from its caller, while the write itself is confirmed. `14068eb20` registers it for event0x1a9, and `14068ed50` uses the byte to consult `140447520` target eligibility and advance selection.

The component vtable at`141b52ec8` has+110 pointing to`140447020`, which requests native stop of network component+ b8 through`1407e7c00`, then calls base`14061bcc0`. The generic stop function queues component retirement according to native lifecycle bits and invokes vtable+40. `140447160` is a five-byte jump to`14061be50`, which clears base equipment flags+a4..a6 and updates equipment timing. None of these functions were invoked or hooked by the cancellation test. Their association with a particular player button still requires evidence; do not equate generic stop with the on-screen Cancel action.
'''
p.write_text(s,encoding='utf-8')
print('Recorded cancellation setup failures, scoped native evidence, and static findings')
