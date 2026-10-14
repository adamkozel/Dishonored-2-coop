from pathlib import Path
import json
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
T=P/'possession-scoped-fx-v350'
review=dict(inspected=True, method='Source and peer temporal contact sheets viewed at 1,2.8,4.4,6.4,7.9,9.8 seconds', evidence=['peer-sheet.jpg','source-sheet.jpg'],complete=False,visual_accepted=False,notes='Source retains native occupied vision and equipment recovery. Peer Corvo casts, disappears and returns, but the native guard occludes/overlaps the return. These recordings verify scoped ownership and body lifecycle, not acceptable complete NPC presentation. DLL343 contains no remote target particles. No audio captured.')
(T/'visual-review.json').write_text(json.dumps(review,indent=2))
r=json.loads((T/'result.json').read_text(encoding='utf-8-sig'));assert r['passed'] and len(r['checks'])==17
(W/'candidate-possession-scoped-fx-v350/runtime-verification-v350.json').write_text(json.dumps(dict(take=str(T),passed=True,checks=17,scoped=json.loads((T/'scoped-fx-summary.json').read_text(encoding='utf-8-sig')),visualAccepted=False,scope=review['notes']),indent=2))
d=R/'documentation/handoff-2026-10-11/32-native-possession-investigation.md'
s=d.read_text(encoding='utf-8-sig')
heading='## Selected human target particles,349–351'
assert heading not in s
s+='''

## Selected human target particles,349–351

[349 passes16 runtime checks](../../work/natural-moveset/possession-fx-observe-v349/result.json). Its external inventory identifies human target particles separately from the five player-controller camera/travel slots. The target effect belongs to a temporary `idEntityFx`, not the player actor. The first348 attempt could not cast because collision shifted the player horizontally despite valid floor height;349 requires both horizontal coordinates within0.08m of the requested point and never relocates the NPC.

Target selection in `140448200` updates the power's weak NPC reference and sends message0x135. `1404c4350` registers its NPC handler at `1404c53a0`. The target component's exact human declaration has vtableRVA`1b63d68`, tag at+0x80 and target FX declaration at+0xa8. `1404c4e10` creates a temporary FX actor at component+0x28, attaches the native possession_target tag and starts the declared particles through `14087cb90`, retaining the ID at+0x30. `1404c4f20` stops the effect and sets that ID toFFFFFFFF **without clearing the actor pointer**. An invalid ID must therefore be treated as absence before dereferencing that potentially stale pointer. Source declarations, component dumps and targeted decompilation transcripts are preserved under `work/movement-integration/*v350*`.

[350 passes17 runtime checks](../../work/natural-moveset/possession-scoped-fx-v350/result.json). Its [scoped reader](../../outputs/d2-coop-lab/src/native_possession_target_fx.h) validates selected-item ownership, weak generation, unique NPC component membership, exact human declaration, FX actor type, ID bucket identity, owner, distance and rigid transform, then repeats identity reads. All15 present samples match the independent full inventory;159 samples include38 valid results and121 stage1 unknowns after the item is unselected. Unknown is not absence. The external reader maximum9.1784ms includes cross-process reads and is not an in-game thread performance measurement. Synthetic tests cover stale/recycled pointers, wrong declarations, shared components, wrong FX owners, malformed matrices and races.

[350's visual review](../../work/natural-moveset/possession-scoped-fx-v350/visual-review.json) retains the known guard occlusion and overlap; complete visual acceptance remains false. Full checkpoint inventory/health and protected14 checks pass. The preserved350 runtime receipt supplements its original pre-test identity document.

[Candidate351](../../work/movement-integration/candidate-possession-target-fx-v351/identity.json) adds a separate172-byte Steam particle protocol with source/world/token/root freshness, monotonically ordered events and a300ms receiver lease. It performs scoped80ms sampling rather than a broad live-effect scan. The receiver starts only a cosmetic native-derived asset on its exact held Corvo clone and updates that owned effect's world transform. The asset retains both native particle definitions/timing and removes controller rumble. Original resources are not replaced. NPC AI, possession control, progression and damage remain source-local. Build, native-memory ownership, packet protocol and proxy tests pass;351 runtime and visual results must be read from their own completed receipts. Alignment with independently simulated NPCs is not yet guaranteed.
'''
d.write_text(s,encoding='utf-8')
f=W/'refresh_grenade_handoff_v58.py';s=f.read_text();s=s.replace('Validate and integrate successful Possession release, host visibility and target lifecycle; complete remaining powers/movement and continue toward playable co-op beyond movement.','Validate remote Possession target particles and cleanup, then close remaining movement/power gaps and implement shared NPC/world authority toward playable co-op.');f.write_text(s)
print('350 evidence and chapter32 updated;351 runtime remains pending')
