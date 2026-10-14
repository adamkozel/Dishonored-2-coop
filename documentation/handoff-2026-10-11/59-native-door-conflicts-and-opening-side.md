# Native door conflicts and actual opening direction

Scope: exact Steam executable SHA-256 `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`, isolated Dunwall checkpoint, existing Steam transport and native game use prompt. This extends [chapter 57](57-native-door-catalog-and-traversal.md) and [chapter 58](58-native-checkpoint-retry-and-door-interruption.md). Full shared-world authority and general door support remain incomplete.

## Findings before the correction

The logical target alone did not establish door geometry convergence. In take754 both players independently opened the same closet door from opposite sides. Native Moving parameters were `0x7000001` and `0x6010001`, representing opposite opening sides. Host ordering settled on revision3, Open2, side1. Both channels incorrectly acknowledged that state as applied, despite the actual host Open-state side being0 and client side being1. Neither channel had called the network-applied native transition. This is a measured defect in candidate742, not an inference from screenshots.

Evidence: [take754 result](../../work/natural-moveset/native-door-simultaneous-sides-v754/result.json), [independent openings](../../work/natural-moveset/native-door-simultaneous-sides-v754/competing-native-starts.json), [actual final sides](../../work/natural-moveset/native-door-simultaneous-sides-v754/native-final-sides.json), [incorrect logical convergence](../../work/natural-moveset/native-door-simultaneous-sides-v754/native-sync-final.json).

## Native use while a door is moving

Ghidra transcripts are retained in [door-conflict-v749](../../work/movement-integration/door-conflict-v749). These are targeted decompilations and cross-references, not a complete reconstruction of the interaction system.

| Address | Confirmed interpretation | Limitation |
| --- | --- | --- |
| `14074a400` | Native explicit queue rejects current Moving1, checks transition eligibility, writes the four Moving parameters and queues state1. | Call only on a stable eligible machine. |
| `14074b6e0` | Ordinary use stores the interactor at component+114. Open2 chooses closing from the active Open side. Other states choose opening side from player/door geometry and clips+1e0/+1e1. Its Moving branch completes/commits the existing native motion before queuing another. | This branch does not prove that the player UI delivers another use event during Moving. |
| `14074ba10` | Special use requires Closed0 and selected definition flags; chooses separate animation entries. | Not substituted for ordinary door use. |
| `14074d7a0` | Referenced by other interruption paths. Earlier710 demonstrated that applying it to inactive Moving state is unsafe. | Remains excluded from ordinary replicated transitions. |

Tests752 and753 establish a narrower runtime fact: pressing use while the received door is closing leaves the current ordered close unchanged. A fresh use after closing completes reopens the door for both players. Both source roles passed; this is not a claim that all native Moving states reject or buffer input.

## Retained failed experiments

| Take | Outcome and interpretation |
| --- | --- |
| 747 | Same-side fixture positions collided. Source moved away from the usable prompt; first use never opened the door. Placement failure. |
| 748 | Opposite-side opening succeeded; a second use during Moving did not reverse it. Harness assumed reversal and failed. |
| 750 | Bathroom reverse-side position resolved into a wall-facing view. Cannot establish valid second-player use eligibility. |
| 751 | The second player was moved to the known front use position during closing; use still left the close unchanged. The old reversal expectation failed. |
| 752 / 753 | Corrected expectation: busy use leaves closing alone; a fresh stable-state use reopens. Both pass. |
| 754 | Real simultaneous opposite-side geometry divergence described above. |

All retained takes restored complete checkpoint inventories, player health56, counterpart health40, and14 protected save/settings hashes. Failed cases remain indexed as failures. Setup cleanup now retains AI isolation until all video capture and monitoring have completed, preventing the earlier743 cleanup attack from recurring.

## Candidate755 implementation

Archived predecessor: [candidate742](../../work/movement-integration/candidate-checkpoint-retry-v742/identity.json). New immutable build: [candidate755](../../work/movement-integration/candidate-door-side-v755/identity.json), DLL `381063871DCA6B3A1AFDEE51E96BC22A955BAE0C455364D25D09AE13C21A78C0`; unchanged probe `1ECB428D77B93915EA0417F3531E9103BB8E24DEC2336E874A809BA827810B22`, protocol69, bank67 with357 clips per role.

Implementation files: [native_openable.h](../../outputs/d2-coop-lab/src/native_openable.h), [door_sync.h](../../outputs/d2-coop-lab/src/door_sync.h), [native_openable_tests.h](../../outputs/d2-coop-lab/src/native_openable_tests.h).

Stable Closed0 matches a closed target regardless of inactive side data. Stable Open2 matches an open target only when the native Open-state side equals the host order. Moving or pending transitions cannot be acknowledged as stable.

When a stable door is open on the wrong side,755 queues native closing using the actual side. After native completion it queues opening using the latest host order. Both operations are owned by the channel, so the intermediate close is not reflected as another player's request. An updated closed order can finish at the intermediate closed state. No door transforms or Open-state side bytes are copied into the game. Fresh ownership, generation, eligibility and opening side are rechecked before every native apply. `sideCorrections` counts successful corrective closes; `nativeCalls` counts the close and reopen separately.

Build and preservation: [Build-DoorSide-v755.ps1](../../work/movement-integration/Build-DoorSide-v755.ps1), [Deploy-DoorSide-v755.ps1](../../work/movement-integration/Deploy-DoorSide-v755.ps1), [stage_door_side_v755.py](../../work/movement-integration/stage_door_side_v755.py). The staging script is one-shot and must not be rerun against the already-created archive. Native ownership, action/resume/checkpoint protocol, proxy smoke and actual357-clip bank tests pass. Pure checks cover both opening directions, closed-side irrelevance and pending/Moving refusal.

Runtime verification passed on755:

| Take | Source placement / order | Measured outcome |
| --- | --- | --- |
| [756](../../work/natural-moveset/native-door-simultaneous-sides-host-v756/result.json) | Corvo outside, Emily inside; simultaneous use | Corvo performs one native close/reopen correction; both actual sides finish1 at revision3. |
| [757](../../work/natural-moveset/native-door-simultaneous-sides-client-v757/result.json) | Emily outside, Corvo inside; simultaneous use | Emily performs one native close/reopen correction; both actual sides finish1 at revision3. |
| [758](../../work/natural-moveset/native-door-busy-use-host-v758/result.json) | Corvo closes, Emily tries use while busy, then uses again afterward | Both close then reopen; one received native call each, zero side corrections. |
| [759](../../work/natural-moveset/native-door-busy-use-client-v759/result.json) | Emily closes, Corvo tries use while busy, then uses again afterward | Same result with roles swapped; zero side corrections. |
| [760](../../work/natural-moveset/native-door-simultaneous-sides-reverse-v760/result.json) | Corvo inside, Emily outside; simultaneous use | Corvo corrects in the reverse direction; both actual sides finish0 at revision3. |

Each passes five harness checks, including five unrelated channels remaining unchanged, complete inventory/health restoration and14 protected hashes. The three simultaneous tests require evidence of both independent opposite-side Moving parameter sets before accepting convergence. Each measures exactly two corrective native calls and one correction across the pair. Audit: [native-door-side-audit-v762.json](../../work/movement-integration/native-door-side-audit-v762.json). The archive's `runtimeVerified` refers only to this bounded scope.

## Review and limitations

Source and observer contact sheets for747,748,750,751,752,753,754 were inspected.751 shows the door remaining closed after the busy press.752/753 show subsequent reopening.754 shows both local openings; native state reads distinguish the opposite geometry more reliably than the two opposite camera views. The relocation used in busy-use testing is a fixture action, not ordinary gameplay or a movement-quality demonstration. These reviews do not accept hand grips or whole-body animation quality.

Both sheets for756–760 were also inspected.756/757 show the corrective door motion pushing the nearby source toward a surface, limiting the final source view;756's measured displacement is approximately0.52 game units.760 provides clearer opposite room/closet views and visible close/reopen motion.758/759 preserve normal bathroom door use. Native collision is retained, and the correction is not accepted as seamless presentation. No new visual animation acceptance was assigned.

The close/reopen correction is deliberately visible and may later benefit from coordinated interaction admission. It must not be described as seamless simultaneous use. Existing4-second native and5-second desired-state deadlines remain bounded. Linked, locked, blocked, broken, newly spawned and dynamically eligible doors are outside this catalog's accepted scope. Catalog arming still requires the explicit isolated fixture; automatic post-load rearming, general save/load and campaign scripting remain outstanding.

Routine sword-fixture setup uses the native shared cutscene skip added in [chapter53](53-native-setup-scene-skip.md). `work/PrepareShadowSword.ps1 -PlaySetupScene` retains full playback for dedicated cinematic verification. Original Steam binaries,14 protected files and isolated checkpoint restoration remain preservation requirements.

## Next integration boundary: post-load catalog renewal

Read-only source inspection shows a concrete dependency: `checkpointEngineService` explicitly stops `observerIsolation` before dispatching fixed-SLOT82 load. The current door context requires a ready movement-isolation fixture with at least one second remaining. `prepareCheckpointPresentation` and `restoreCheckpointPresentation` restore the requested counterpart and follow state only; neither restores AI isolation. Therefore simply calling `enableDoorSync(true)` after avatar restoration cannot rearm the current catalog successfully.

The continuation must separate an explicitly armed session's intent from its ephemeral world bindings, retire all old pointers and packet arms, then rebuild only after a verified new-world pair boundary and the required test context are ready. Do not silently carry old AI references across loading or interpret counterpart restoration as shared AI authority. If the door subsystem is promoted beyond the isolated fixture, define and test its production scope explicitly instead of bypassing a failed eligibility check. No such promotion or renewal is implemented in755.
