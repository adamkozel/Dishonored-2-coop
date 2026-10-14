"""Compare independently inventoried native source/peer particle transforms.

Requires completed watcher files. Same-machine nearest samples are useful for
stationary attachment correctness, never a measurement of packet latency.
"""
from pathlib import Path
import argparse, json, math
R=Path(__file__).resolve().parents[2]
arg=argparse.ArgumentParser();arg.add_argument('take');a=arg.parse_args()
p=R/'work/natural-moveset'/a.take;assert p.parent==R/'work/natural-moveset'
read=lambda n:[json.loads(l) for l in (p/n).read_text(encoding='utf-8-sig').splitlines() if l.strip()]
source=[(r['tick'],e) for r in read('possession-effects.ndjson') if r['valid'] for e in r['effects'] if e['name'].endswith('/possession_target_human_01.fx')]
peer=[(r['tick'],e) for r in read('peer-effects.ndjson') if r['valid'] for e in r['effects'] if e['name']=='models/effects/d2coop/possession_target_remote.fx']
assert source and peer, 'No independent paired particle samples'
rows=[]
for tick,e in peer:
    st,se=min(source,key=lambda x:abs(x[0]-tick))
    rows.append(dict(tick=tick,sourceTick=st,ageMs=tick-st,id=e['id'],owner=e['owner'],sourceId=se['id'],distance=math.dist(e['origin'],se['origin']),axisMax=max(abs(x-y) for x,y in zip(e['axis'],se['axis']))))
x=dict(samples=len(rows),maxDistance=max(x['distance'] for x in rows),maxAxisDelta=max(x['axisMax'] for x in rows),maxNearestSampleGap=max(abs(x['ageMs']) for x in rows),rows=rows,scope='Nearest same-machine native inventory sample; stationary target. This is not a transport-latency measurement or proof of NPC world authority.')
(p/'independent-transform-comparison.json').write_text(json.dumps(x,indent=2))
print(json.dumps({k:v for k,v in x.items() if k!='rows'}))
