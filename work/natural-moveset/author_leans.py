"""Whole-body lean candidates with support-foot contact and free hands.

These are authored balance shifts, inspired by D2's peek intent. They do not
copy camera-relative first-person wrist positions or modify the local player.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['library-retarget-manifest.json','stance-retarget-manifest.json'],'lean-authored')
for role in ['corvo','emily']:
 rig=rig_for(role)
 for stance,alias in [('stand','sword_idle'),('crouch','crouch_idle')]:
  meta,fs=L.load(role,alias);base=fs[0];g=rig.fk(base);feet=[g[i].translation.copy()for i in [21,25]];rots=[g[i].to_3x3().copy()for i in [21,25]]
  for side,sign in [('left',-1),('right',1)]:
   poses=[];targets=[]
   for f in range(181):
    t=f/60;w=ease(t/.6)if t<.6 else 1 if t<2.1 else 1-ease((t-2.1)/.9)
    p=sample(fs,t/max(meta['duration'],.1),True)
    # Shift support through pelvis and distribute the tilt up the spine.
    # Native segment lengths and support feet are retained; arms follow
    # shoulder/chest motion rather than being held in world/camera space.
    p[2].translation.y-=sign*w*(.065 if stance=='stand' else .085)
    p[2].translation.z-=.02*w
    for i,degrees in [(2,5),(3,4),(4,4),(5,4),(11,-3),(12,-3)]:
     before=rig.fk(p)[i].to_3x3();rig.orient(p,i,Matrix.Rotation(math.radians(sign*degrees*w),3,'X')@before)
    pin_feet(rig,p,feet,rots);poses.append(p);targets.append(identity())
   L.save(rig,role,stance+'_lean_'+side,poses,targets,'stance','Whole-body enter/hold/recover lean. Pelvis shifts over planted feet; spine distributes balance and shoulders carry both free wrists. Camera collision limits and network lean state still need binding.',dict(manifest='stance-retarget-manifest.json'if stance=='crouch'else'library-retarget-manifest.json',alias=alias,sha256=meta['sha256']),support_feet='Both planted; native segment lengths',lean_side=side)
L.finish('lean-retarget-manifest.json')
