import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from edge_data import poses as raw_poses
L=Library(['carry-victim-retarget-manifest.json'],'carry-research');rig=rig_for('corvo');report=[]
for m in L.rows:
 _,ps=L.load('corvo',m['selection']['alias']);raw=raw_poses(P/m['source']['sample_file']);samples=[]
 for f in [0,len(ps)//2,len(ps)-1]:
  g=rig.fk(ps[f]);anchor=qts(raw['frames'][f][1]);origin=qts(raw['frames'][f][0]);placement=anchor.inverted()
  samples.append(dict(frame=f,anchor=row(anchor),origin=row(origin),points={rig.joints[i]['nativeName']:[round(x,4)for x in placement@origin@g[i].translation]for i in [2,5,11,9,16,19,20,21,23,24,25]}))
 report.append(dict(alias=m['selection']['alias'],samples=samples))
(P/'carry-research/reference-frames.json').write_text(json.dumps(report,indent=2))
for r in report:
 print(r['alias'],json.dumps(r['samples'][len(r['samples'])//2]),flush=True)
