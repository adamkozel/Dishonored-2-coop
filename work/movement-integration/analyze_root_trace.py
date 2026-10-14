import json,math,sys
from pathlib import Path
p=Path(sys.argv[1]);r=json.loads(p.read_text(encoding='utf-8-sig'));rows=r['samples'];spikes=[]
rates=[];dts=[]
for a,b in zip(rows,rows[1:]):
    dt=(b['t']-a['t'])/1000
    if dt<=0:continue
    d=math.dist(a['target'][:2],b['target'][:2]);rate=d/dt;rates.append(rate);dts.append(dt)
    if rate>10 or abs(b['target'][2]-a['target'][2])>.15:
        spikes.append(dict(t=b['t'],dt=dt,rate=rate,distance=d,vertical=b['target'][2]-a['target'][2],seq=b['sequence'],prev=a['sequence']))
summary=dict(samples=len(rows),maximum_rate=max(rates),minimum_dt=min(dts),mean_dt=sum(dts)/len(dts),spikes=spikes)
p.with_suffix('.analysis.json').write_text(json.dumps(summary,indent=2));print(json.dumps(summary,indent=2))
