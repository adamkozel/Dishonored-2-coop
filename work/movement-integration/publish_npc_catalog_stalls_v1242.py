"""Audit 1242 (#43): NPC stream catalog reads stall both engine threads on Edge of the World. Take 1235e (1213m)
found it - its counters were read from the live lab status right after the take and are not archived - and take
1235f (1213n, harness v1235f) archives both peers' native service timings and NPC stream status per case.
Writes the audit; candidate archives are never rewritten."""
from pathlib import Path
import json, datetime, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
sys.path.insert(0, str(W)); from presentation_gap_metrics import metrics
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
TAKE = 'window-traversal-v1235f'; CAND = 'candidate-npc-combat-v1213n'
def stall(take, case, role):
    t = rd(N / take / f'cross-{case}-{role}.native-service-timing.json'); n = rd(N / take / f'cross-{case}-{role}.npc-stream.json')
    st = {s['name']: s for s in t['stages']}
    return dict(relevance=n.get('relevance'), stages={k: {f: st[k][f] for f in ('count', 'slow', 'maximumMs', 'totalMs')} for k in ('exchangePoses', 'serviceNpcStream', 'transport_all', 'native_services_all') if k in st},
                slowEvents=[dict(name=e['name'], ms=e['ms']) for e in t['events'] if e['name'] in ('exchangePoses', 'serviceNpcStream')])
r = rd(N / TAKE / 'result.json')
at = datetime.datetime.now().astimezone().isoformat()
wr(W / 'npc-catalog-stalls-audit-v1242.json', dict(at=at, candidate=CAND,
    take=dict(take=f'../natural-moveset/{TAKE}', passed=r['passed'], restoration=rd(N / TAKE / 'restoration.json')['passed'],
              cases=[{k: c.get(k) for k in ('source', 'passed', 'crossed', 'mantle', 'maxTraversalWeight')} for c in r['cases']]),
    timings={case: {role: stall(TAKE, case, role) for role in ('host', 'client')} for case in ('host', 'client')},
    gaps={src: metrics(N / TAKE / f'cross-{src}-observer-movement.ndjson') for src in ('host', 'client')},
    baseline_1235e=dict(source='live lab status files read right after take 1235e (1213m); not archived',
        client=dict(clientBound=27, clientRefused=55, exchangePosesSlow=46, exchangePosesMaxMs=924.675, slowEventsMs='78.8-103.4 about every 0.5 s'),
        host=dict(relevanceScans=21, serviceNpcStreamSlow=22, serviceNpcStreamMaxMs=161.945, slowEventsMs='89.7-161.9 every 2 s')),
    findings=['Confirmed runtime (1235e, live status): on Edge of the World every host Hello for an NPC the client had not bound re-read the whole native NPC catalog on the client engine thread; refused binds repeated on the next Hello (27 bound, 55 refused), giving 46 exchangePoses stalls of 79-103 ms (max 925 ms). The host 2 s relevance scan cost 90-162 ms.',
              'Confirmed static: memoryRead is a ReadProcessMemory syscall per read; the world catalog walk read every live entity twice, and readNativeNpcAuthority reads each NPC frame twice.',
              'Confirmed runtime (1235f, 1213n, passed both roles, restoration passed): one catalog snapshot per second and a 3 s refusal backoff cut the client catalog reads to 10 over both cases (101 Hellos deferred) and its exchangePoses stalls to 10; each remaining read still costs 70-86 ms.',
              'The single-pass walk did not fix the host: serviceNpcStream still stalls 80-207 ms per relevance scan (17 slow over 15 scans). The catalog read must leave the engine thread (next candidate).'],
    complete=False, scope='#43 first step: client Hello reads bounded; host relevance scan still on the engine thread.'))
print('ok')
