from pathlib import Path
import json
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration';T=P/'possession-exit-continuity-v343';A=W/'candidate-possession-exit-v343'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
r=read(T/'result.json');c=read(T/'exit-continuity.json');assert r['passed'] and len(r['checks'])==15 and c['dualOwnedSamples']==3 and c['remoteDrawSamples']==6 and c['walkSamples']==5
write(A/'runtime-verification.json',dict(result=r,continuity=c,visualReview=read(T/'visual-review.json'),complete=False))
# Preserve archival identity files; explicitly correct inherited metadata.
old=W/'candidate-possession-release-body-v338'
write(old/'identity-errata-v343.json',dict(note='Original identity.json inherited stale bank paths/references from328 despite actual banks/ containing52/316. Do not use those inherited paths to reconstruct338. Actual archived files and authoritative source manifest are listed here; original archive retained.',actualBanks=read(P/'possession-bank-manifest-v334.json'),banksDirectory='banks',runtimeEvidence=['../../natural-moveset/possession-visibility-network-v339/result.json','../../natural-moveset/possession-visibility-return-v342/result.json'],complete=False))
f=R/'documentation/handoff-2026-10-11/32-native-possession-investigation.md';s=f.read_text();heading='## Native Possession exit stream continuity,343'
if heading not in s:s+='''

## Native Possession exit stream continuity,343

[342 passes14](../../work/natural-moveset/possession-visibility-return-v342/result.json), but [viewing its return](../../work/natural-moveset/possession-visibility-return-v342/visual-review.json) reveals an occluded body and the source stream retiring during the native exit. Static reader inspection plus raw342 scenes identify the cause: state64 owns `power_equip/Scene_1/333ms` concurrently with state53 owning Corvo `sword_equip/Scene_2/1066ms`. The old paired-equipment reader allowed only dual holster scenes.

[Candidate343](../../work/movement-integration/candidate-possession-exit-v343/identity.json) validates that exact Corvo standing draw combination, including independent state handles, reciprocal left/right arm ownership, native scene clocks, character-specific subscenes, unchanged lists and final identity revalidation. Emily/crouched dual draw remains refused. The source camera, arms and gameplay remain native. Native-memory tests cover foreign/aliased/expired scenes and concurrent state/arm replacement; action protocol and proxy suites also pass. An initial shell command could not find CMake and ran old tests; that attempt is not fresh-build validation. The archived explicit-path build and subsequent logs are the authoritative successful validation.

[343 passes15 runtime checks](../../work/natural-moveset/possession-exit-continuity-v343/result.json): three owned dual-equip observations, six remote draw samples,18 post-exit rows with both streams ready, five walking samples and0.4907341m native player displacement, without any stream rearm. Visibility still hides/shows exactly once. Full checkpoint inventory/health and protected14 restoration pass; final process exit0 consumed.

[The343 visual review](../../work/natural-moveset/possession-exit-continuity-v343/visual-review.json) confirms the native first-person exit, but the observer is too far from this guard endpoint to show the remote body. This is **runtime continuity proof, not visual acceptance**. A closer camera must use a floor independently measured in the same take. Remote NPC control/animation, VFX, shared outcomes and alternate hosts remain unfinished.

Archive audit:338's original identity document inherited older bank paths despite its actual `banks/` files containing52/316. [An errata record](../../work/movement-integration/candidate-possession-release-body-v338/identity-errata-v343.json) corrects that reconstruction ambiguity without overwriting the archive.343's identity lists hashes and paths of its actual archived bank files.
'''
f.write_text(s,encoding='utf-8');print('343 runtime evidence and archival metadata correction recorded')
