import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['library-retarget-manifest.json','interaction-retarget-manifest.json'],'pair-research');out=[]
for role in ['corvo','emily']:
 for alias in ['empty_distraction_loot_ground_in','empty_distraction_loot_ground_out']:
  m,ps=L.load(role,alias);rig=rig_for(role)
  out.append(dict(role=role,alias=alias,duration=m['duration'],samples=[dict(phase=u,positions={j:list(rig.fk(sample(ps,u))[j].translation)for j in [2,5,9,12,16,21,25]})for u in [0,.25,.5,.75,1]]))
(P/'pair-research/ground-actor-reference.json').write_text(json.dumps(out,indent=2));print(json.dumps(out))
