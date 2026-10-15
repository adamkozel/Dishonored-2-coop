"""Audit 1243 (#43): native NPC catalog reads on a worker thread (candidate 1213o), take 1235g (harness v1235f),
against take 1235f (1213n, audit 1242). Writes the audit; candidate archives are never rewritten."""
from pathlib import Path
import json, datetime, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
sys.path.insert(0, str(W)); from presentation_gap_metrics import metrics
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
TAKE = 'window-traversal-v1235g'; CAND = 'candidate-npc-combat-v1213o'
def stall(take, case, role):
    t = rd(N / take / f'cross-{case}-{role}.native-service-timing.json'); n = rd(N / take / f'cross-{case}-{role}.npc-stream.json')
    st = {s['name']: s for s in t['stages']}
    return dict(relevance=n.get('relevance'), stages={k: {f: st[k][f] for f in ('count', 'slow', 'maximumMs', 'totalMs')} for k in ('exchangePoses', 'serviceNpcStream', 'transport_all', 'native_services_all') if k in st},
                slowEvents=[dict(name=e['name'], ms=e['ms']) for e in t['events'] if e['name'] in ('exchangePoses', 'serviceNpcStream')])
r = rd(N / TAKE / 'result.json')
at = datetime.datetime.now().astimezone().isoformat()
wr(W / 'npc-catalog-worker-audit-v1243.json', dict(at=at, candidate=CAND,
    take=dict(take=f'../natural-moveset/{TAKE}', passed=r['passed'], restoration=rd(N / TAKE / 'restoration.json')['passed'],
              cases=[{k: c.get(k) for k in ('source', 'passed', 'crossed', 'mantle', 'maxTraversalWeight')} for c in r['cases']]),
    timings={case: {role: stall(TAKE, case, role) for role in ('host', 'client')} for case in ('host', 'client')},
    gaps={src: metrics(N / TAKE / f'cross-{src}-observer-movement.ndjson') for src in ('host', 'client')},
    baseline_1235f=dict(audit='npc-catalog-stalls-audit-v1242.json', host=dict(serviceNpcStreamSlow=17, serviceNpcStreamMaxMs=207.35), client=dict(helloCatalogReads=10, exchangePosesSlow=10, exchangePosesMaxMs=85.806)),
    findings=['Confirmed runtime (1235g, 1213o, passed both roles, restoration passed): with the catalog read on a worker thread the host relevance scan no longer stalls the engine thread (serviceNpcStream: 1 slow call of 73 ms, the one-off stream start, against 17 slow up to 207 ms in 1235f); the worker read the catalog 15 times in 87-95 ms off-thread, one read failed its list-consistency check and was retried on the next scan.',
              'Client: no slow exchangePoses at all (max 15.4 ms; 10 slow up to 86 ms in 1235f, 46 up to 925 ms in 1235e); 27 binds from 9 worker snapshots, no stale snapshot entry refused at bind.',
              'The stalls were the source of the partner-presentation gaps: stale-state eligibility records fell to 0 and eligibility failures to 1-2 per case (hundreds in 1235e/1235f); native-pose time 1.9% (host source) and 0.9% (client source), the remaining drops being the one-frame protocol-73 hold at the mantle end and root-step rejections.',
              'Every NPC bound from a worker snapshot is re-read and compared on the engine thread before bindNpcChannel (freshNpcCatalogEntry); the stream start still reads the catalog once on the engine thread.'],
    complete=True, scope='#43: NPC catalog reads off the engine thread on Edge of the World, both roles; the one-off stream-start read remains on the engine thread.'))
print('ok')
