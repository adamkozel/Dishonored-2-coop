"""Compare cumulative native service counters in quiet active/hidden windows.

These intervals are transport-pump callbacks, not display FPS. Transform and
identity timings overlap; do not add stage percentages as exclusive CPU costs.
"""
from pathlib import Path
import json,sys
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';take=sys.argv[1];T=P/take
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
out={'take':take,'scope':'Quiet windows after video/watcher exit. Timing stages overlap. Pump capped by10ms; callback rate is not display FPS. No render/animation CPU or GPU attribution.','windows':{}}
for name,phase in [('active','source_present'),('hidden','source_absent')]:
    rows=read(T/f'quiet-{name}.json');assert len(rows)>=8
    assert all(bool(r['activeModels'])==(name=='active') for r in rows)
    first,last=[next(p for p in r['timings']['phases'] if p['phase']==phase) for r in (rows[0],rows[-1])]
    n=last['intervalCount']-first['intervalCount'];assert n>0
    elapsed=last['intervalTotalMs']-first['intervalTotalMs'];assert elapsed>1000
    stages={}
    for a,b in zip(first['stages'],last['stages']):
        assert a['stage']==b['stage'];count=b['count']-a['count'];ms=b['totalMs']-a['totalMs'];assert count>=0 and ms>=0
        stages[a['stage']]={'calls':count,'totalMs':ms,'meanCallMs':ms/count if count else None}
    calls=stages['total']['calls'];assert calls>0
    for stage in stages.values():stage['msPerPoolService']=stage['totalMs']/calls
    out['windows'][name]={'observations':len(rows),'modelCounts':sorted(set(r['activeModels'] for r in rows)),'serviceIntervalSamples':n,'serviceIntervalMeanMs':elapsed/n,'serviceObservedHz':n*1000/elapsed,'stages':stages,'measuredPoolServiceFraction':stages['total']['totalMs']/elapsed}
(T/'performance-summary.json').write_text(json.dumps(out,indent=2))
print(json.dumps(out,indent=2))
