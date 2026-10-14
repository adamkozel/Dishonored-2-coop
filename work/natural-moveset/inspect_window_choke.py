import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from edge_data import poses
L=Library(['window-choke-victim-manifest.json','choke-victim-retarget-manifest.json'],'window-choke-research');rig=rig_for('corvo');out=[]
for role in ['corvo','emily']:
 m,fs=L.load('corvo','generic_'+role+'_chokeback_window_in');raw=poses(P/m['source']['sample_file']);place=qts(raw['frames'][0][1]).inverted();rows=[]
 for u in [0,.25,.5,.75,1]:
  p=sample(fs,u);root=sample([[qts(r)]for r in m['root_track']],u)[0];g=rig.fk(p);rows.append(dict(u=u,neck=list((place@root@g[11]).translation),feet=[list((place@root@g[j]).translation)for j in [21,25]]))
 out.append(dict(role=role,placement=row(place),phases=rows));print(role,json.dumps(rows))
(P/'pair-research/window-choke-phases.json').write_text(json.dumps(out,indent=2))
