import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['body-lift-manifest-v1.json'],'carry-research')
for role in ['corvo','emily']:
 rig=rig_for(role);m,frames=L.load(role,'carry_body_floor_lift_study');first=rig.fk(frames[0]);errors=[]
 for t in [0,.8,1.5,2,2.3,2.6,3.1,3.7]:
  g=rig.fk(sample(frames,t/m['duration']));print(role,t,{rig.joints[j]['nativeName']:[round(x,3)for x in g[j].translation]for j in [2,7,9,14,16,20,21,24,25]},flush=True)
 for f,p in enumerate(frames):
  g=rig.fk(p)
  for j in [21,25]:errors.append(dict(t=f/60,joint=j,error=(g[j].translation-first[j].translation).length))
 print('FOOT_MAX',role,sorted(errors,key=lambda x:x['error'],reverse=True)[:3],flush=True)
