import sys,json,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['carry-victim-retarget-manifest.json','carry-body-manifest-v3.json'],'carry-research');rig=rig_for('corvo');_,idle=L.load('corvo','generic_carrycorpse_idle');bg=rig.fk(idle[0]);carrier,_=L.load('corvo','carry_body_idle');attach=qts(carrier['partner']['root_track'][0])
for alias in ['generic_carrycorpse_in','generic_carrycorpse_drophigh','generic_carrycorpse_droplow','generic_carrycorpse_dropsoft']:
 m,fs=L.load('corvo',alias);g=rig.fk(fs[0]);align=bg[4]@g[4].inverted();q=align.to_quaternion();print(alias,'torso_delta_deg',math.degrees(q.angle),'axis',list(q.axis),'offset',list(align.translation),'root0',m['root_track'][0],flush=True)
 if alias.endswith('_in'):
  for u in [0,.25,.5,.75,1]:
   g=rig.fk(sample(fs,u));print('PICKUP_NATIVE',u,{rig.joints[j]['nativeName']:[round(v,3)for v in(attach@g[j]).translation]for j in [2,5,12,9,16,21,25]},flush=True)
