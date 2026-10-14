from pathlib import Path
import json,statistics
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
t=P/'domino-link-clear-view-v221';result=read(t/'result.json');assert result['passed']
rows=[json.loads(line)for line in (t/'domino-links.ndjson').read_text().splitlines()]
cached=sorted(x['readMs']for x in rows if x['cached'])
v={'take':t.name,'passed':True,'checks':result['checks'],'reader':read(t/'link-reader-summary.json'),'cachedReadMs':{'median':statistics.median(cached),'p95':cached[int(len(cached)*.95)],'maximum':max(cached)},'scope':'Read-only source native beam identity/endpoints; no receiver beam or gameplay claim.'}
(R/'work/movement-integration/probe-domino-links-v217/runtime-verification.json').write_text(json.dumps(v,indent=2),encoding='utf-8')
f=R/'documentation/handoff-2026-10-11/30-native-domino-investigation.md'
s=f.read_text(encoding='utf-8-sig')+f'''

## Native beam reader confirmed, v221; network candidate222

The [v221 final receipt](../../work/natural-moveset/domino-link-clear-view-v221/result.json) passes ten checks with all477 connection snapshots valid and227 containing two directed edges plus exactly one beam. The independently observed beam start/end transforms match the selected native nodes and opening interpolation. Cached external reader timing: median{v['cachedReadMs']['median']:.4f}ms, p95{v['cachedReadMs']['p95']:.4f}ms, max{v['cachedReadMs']['maximum']:.4f}ms; the initial uncached inventory discovery is separately included in the25.5208ms overall maximum. [Pinned probe verification](../../work/movement-integration/probe-domino-links-v217/runtime-verification.json) records the actual results.

The inspected video shows the first marked guard upstairs, the native first-person cast beside the lower guard, and the persistent beam running up to the first target. The second source placement was grounded at61.0036469 before casting. Both inventories/checkpoints and fourteen protected hashes pass. v219 had instead looked into the balcony floor and correctly failed its second selection; its video remains indexed.

**Fixture qualification:** `observer_isolation.h` already holds decision/reflex references for five exact native NPCs during movement tests, including both tutorial guards. They are held at their positions when the fixture is enabled, which vary with setup timing. Earlier wording about patrol variation should be read as this run-start variation, not a claim that the guards remained autonomously patrolling throughout these captures. This does not prove active AI synchronization. No new NPC hold, relocation or spawn was added forv221; only the source player moved, with its stream disabled.

The [exported native link FX declaration](../../outputs/d2-coop-lab/research/asset-exports/domino-beam-fx-v220/manifest.json), SHA256`13493613BF4D8B216365DB4E23F10C8B0FC763E04499AA328D8BB2E5C4BC5195`, contains a looping native particle link and a disabled validation layer. Its authored fade is not treated as proof of runtime record retirement. The original Steam indices/executable were hash checked around read-only Void export.

[Candidate222](../../work/movement-integration/candidate-domino-links-v222/identity.json) preserves body44/bank47 and adds Domino world2 (1000bytes). It serializes up to4 marks and6 observed beam effects, checking that each endpoint belongs to a selected mark, preventing duplicate/reversed visible edges, and bounding both transforms. Foreign handles/FX IDs remain opaque incarnation keys. The receiver [beam presentation module](../../outputs/d2-coop-lab/src/domino_link_presentation.h) owns each native FX on the current remote Emily presentation, updates its two endpoint matrices, and stops only its exact owned record when the scoped stream expires. It does not call native connection/NPC/damage methods or reproduce reverse edges without observed FX. Synthetic identity/race tests, protocol tests and proxy load checks pass. Paired native verification is ongoing inv223; source code/build success alone does not establish runtime or visual acceptance.
'''
f.write_text(s,encoding='utf-8')
print(json.dumps(v['cachedReadMs']))
