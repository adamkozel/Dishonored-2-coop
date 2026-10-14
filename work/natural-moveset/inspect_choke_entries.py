import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['choke-victim-retarget-manifest.json'],'pair-research');rig=rig_for('corvo');report=[]
for role in ['corvo','emily']:
 for direction in ['front','back','left','right']:
  m,p=L.load('corvo',f'generic_{role}_choke{direction}_in');points=[]
  for u in [0,.25,.5,.75,1]:
   g=rig.fk(sample(p,u));root=sample([[qts(r)]for r in m['root_track']],u)[0];neck=Vector((1,0,0))+root@g[11].translation;forward=(root@g[5]).to_3x3()@rig.global_bind[5].to_3x3().inverted()@Vector((1,0,0));points.append(dict(u=u,neck=list(neck),forward=list(forward)))
  report.append(dict(role=role,direction=direction,duration=m['duration'],points=points))
(P/'pair-research/choke-entry-frames.json').write_text(json.dumps(report,indent=2));print(json.dumps(report),flush=True)
