from pathlib import Path
import json,subprocess,sys,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
notes={
 'possession-target-fx-view-v354':'Both source/peer sheets viewed. Source target glow and native cast/occupation/exit work; peer target region is obscured by nearby Corvo and an independently simulated guard.18 runtime checks pass but remote glow is not clearly demonstrated by these angles. No audio.',
 'possession-target-fx-side-v355':'Both temporal sheets viewed. Peer side camera is blocked by a bookshelf pillar until the late reframe; source native target halo/streaks, entry and exit are visible.19 runtime checks include19 independent peer-native FX records with the exact streamed effect ID and Corvo clone owner. Particle appearance remains unproven in this take. No audio.',
 'possession-target-fx-cancel-v358':'Both temporal sheets viewed at0.7,1,1.3,1.7,2.1,3 seconds. Peer native target halo and streaks appear at the guard head, then clear after native source cancellation. Source first-person aim/cancel remains native. This accepts the scoped stationary target-particle appearance/retirement only; independently simulated NPC alignment across movement and full Possession gameplay remain unfinished. No audio.'
}
latest='Native source aim cancellation358 passes7 checks: one remote target halo/streak effect starts and stops, normal movement stays ready, mana is unchanged, both checkpoints and protected14 restore. Both game videos visibly show the target particles and cancellation;16 independent peer transform samples are within7.43mm of the nearest source sample. This is scoped stationary cosmetic presentation only. Shared NPC control/outcomes, other host kinds, complete movement and playable co-op remain unfinished.'
for take,note in notes.items():
 t=P/take;r=json.loads((t/'result.json').read_text(encoding='utf-8-sig'));assert r['passed']
 review=dict(inspected=True,method='Viewed source and peer temporal contact sheets',evidence=['peer-sheet.jpg','source-sheet.jpg'],complete=False,visual_accepted=False,target_particles_accepted=take.endswith('cancel-v358'),notes=note)
 (t/'visual-review.json').write_text(json.dumps(review,indent=2))
 subprocess.run([sys.executable,str(W/'record_reviewed_possession_take.py'),take,'--label',take.rsplit('-v',1)[-1]+' target particles','--notes',latest],check=True)
archive=W/'candidate-possession-fx-probe-v355'
receipt={take:json.loads((P/take/'result.json').read_text(encoding='utf-8-sig')) for take in notes}
receipt['targetParticlesVisualEvidence']='possession-target-fx-cancel-v358/visual-review.json';receipt['transformComparison']=json.loads((P/'possession-target-fx-cancel-v358/independent-transform-comparison.json').read_text())
(archive/'runtime-verification-v358.json').write_text(json.dumps(receipt,indent=2))
for p in [P/'Test-PossessionFxLifecycle-v358.ps1',W/'analyze_possession_target_replication.py',Path(__file__)]:shutil.copy2(p,archive/p.name)
d=R/'documentation/handoff-2026-10-11/32-native-possession-investigation.md';s=d.read_text();assert '## Visible target particles and cancellation,354–358' not in s
s+='''

## Visible target particles and cancellation,354–358

[354 passes18](../../work/natural-moveset/possession-target-fx-view-v354/result.json), but its viewed peer camera is obstructed by Corvo and a nearby guard. [355 passes19](../../work/natural-moveset/possession-target-fx-side-v355/result.json); the [independent receiver inventory](../../work/natural-moveset/possession-target-fx-side-v355/independent-peer-fx.json) observes19 records for effect398 on the exact held Corvo clone.355's side camera is blocked by a bookshelf pillar. Native record ownership is verified, not promoted to appearance acceptance.

[358 cancellation passes7 checks](../../work/natural-moveset/possession-target-fx-cancel-v358/result.json). [Viewed paired recordings](../../work/natural-moveset/possession-target-fx-cancel-v358/visual-review.json) now clearly show the remote native halo/streaks at the target head and their retirement after the source's native X cancellation. Normal movement remains armed/ready; source mana is unchanged. Both complete checkpoint inventories/health and protected14 restore; final native exit0 consumed. The [independent transform comparison](../../work/natural-moveset/possession-target-fx-cancel-v358/independent-transform-comparison.json) covers16 peer records, maximum7.43mm position and0.00989 matrix-element differences from the nearest source sample, with sample gaps at most31ms. This is stationary-transform evidence, not transport latency. Native NPC simulation remains independent; the aligned guard in this fixture does not establish shared NPC authority.

The exact native target particle declarations were exported through pinned read-only Void tooling to `outputs/d2-coop-lab/research/asset-exports/possession-target-particles-v355`; original indices were unchanged. No particle retargeting was necessary. The previously created separate FX asset removes rumble and otherwise retains native particle definitions/timing. Runtime ownership/lease code remains DLL351, while probe355 adds read-only receiver inventory. [The preserved355 receipt](../../work/movement-integration/candidate-possession-fx-probe-v355/runtime-verification-v358.json) binds the new evidence to these binaries.

Retained harness failures:353 incorrectly used foot height for native setviewpos, which expects eye height; stream preflight refused and cleanup passed.354 restores eye height and validates the resulting floor.356 generated three previews but the local agent's choice arrived36seconds after publication, exceeding its35second bound; it never cast and checkpoint cleanup passed.357 cancelled and visibly retired the native effect, but a PowerShell `-ge500` token prevented final assertions;358 corrects it to `-ge 500` and avoids issuing a redundant second cancellation input. These failures are not gameplay passes.358 uses the inspected corridor-interior camera coordinates and checks the floor anew.

Scoped target-particle appearance is accepted for358; the whole-power visual acceptance and complete movement flags remain false. Source/receiver menu and stream retirement tests continue, followed by remaining movement and shared-world systems.
''';d.write_text(s,encoding='utf-8')
print('354/355/358 indexed; scoped particle visual acceptance only')
