import sys,statistics
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['mixamo-dynamic-retarget-manifest.json'],'contact-inspection')
for alias in ['mixamo_crouch_walk_left','mixamo_crouch_walk_right','mixamo_crouch_walk_back','mixamo_two_cycle_sprint']:
 m,fs=L.load('corvo',alias);rig=rig_for('corvo');g=[rig.fk(p)for p in fs];v=[]
 for foot in [21,25]:
  low=min(x[foot].translation.z for x in g)
  for i in range(1,len(g)-1):
   if max(g[i-1][foot].translation.z,g[i+1][foot].translation.z)<low+.035:v.append((g[i+1][foot].translation-g[i-1][foot].translation)*(-.5*(len(g)-1)))
 print(alias,'samples',len(v),'plant inferred travel XYZ',[statistics.median(a[i]for a in v)for i in range(3)],'range Y',min(a.y for a in v),max(a.y for a in v),flush=True)
