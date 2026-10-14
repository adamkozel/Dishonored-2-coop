import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
lib=Library(['library-retarget-manifest.json'],'high-mantle-sources-v1')
rig=rig_for('corvo');m,frames=lib.load('corvo','sword_mantlemiddle1_walkleft_in')
out=[]
for i in range(0,len(frames),3):
 g=rig.fk(frames[i]);root=Vector(m['root_track'][i][4:7])
 out.append(dict(frame=i,root=list(root),joints={str(j):list(g[j].translation+root)for j in [2,7,9,12,16,19,20,21,23,24,25]}))
(P/'high-mantle-donor-inspection.json').write_text(json.dumps(out,indent=2))
print(json.dumps(out))
