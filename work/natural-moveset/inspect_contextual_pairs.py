import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from edge_data import poses as raw_poses
L=Library(['contextual-victim-retarget-manifest.json','window-victim-retarget-manifest.json'],'pair-research');out=[]
for m in L.rows:
    rig=rig_for('corvo');_,ps=L.load('corvo',m['selection']['alias']);raw=raw_poses(P/m['source']['sample_file']);anchor=qts(raw['frames'][0][1]);place=anchor.inverted()
    out.append(dict(alias=m['selection']['alias'],duration=m['duration'],placement=row(place),source=m['source']['asset'],samples=[dict(phase=u,anchor=row(qts(raw['frames'][round(u*(len(raw['frames'])-1))][1])),neck=list((place@qts(m['root_track'][round(u*(len(ps)-1))])@rig.fk(ps[round(u*(len(ps)-1))])[11]).translation),pelvis=list((place@qts(m['root_track'][round(u*(len(ps)-1))])@rig.fk(ps[round(u*(len(ps)-1))])[2]).translation))for u in [0,.25,.5,.75,1]]))
(P/'pair-research/contextual-pair-origins.json').write_text(json.dumps(out,indent=2));print('Contextual references:',len(out))
