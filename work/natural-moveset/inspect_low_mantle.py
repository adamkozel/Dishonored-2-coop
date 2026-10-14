import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['library-retarget-manifest.json'],'low-mantle-sources-v1')
for role in ('corvo','emily'):
 m,frames=L.load(role,'sword_mantlelow_walkleft_in');rig=rig_for(role)
 for f in (0,5,10,15,20,25,30):
  g=rig.fk(frames[f]);root=Vector(m['root_track'][f][4:7]);print(role,f,'root',list(root),'world',[(j,[round(v,3)for v in g[j].translation+root])for j in (2,9,16,21,25)],flush=True)
