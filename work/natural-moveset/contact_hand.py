"""Fit contact at the palm/fingers instead of confusing wrist with grip."""
from clip_io import *
def descendants(rig,wrist):
 out=[]
 for i,j in enumerate(rig.joints):
  parent=i
  while parent>=0 and parent!=wrist:parent=rig.joints[parent]['parent']
  if parent==wrist and i!=wrist and 'helper'not in j['nativeName']:out.append(i)
 return out
def grip_center(rig,pose,wrist):
 side='left'if wrist==9 else'right';names={j['nativeName']:i for i,j in enumerate(rig.joints)};g=rig.fk(pose);inv=g[wrist].inverted()
 points=[inv@g[names[side+'_'+name+'_jnt']].translation for name in ['thumb02','index02','middle02','ring02','pinky03'] if side+'_'+name+'_jnt'in names]
 assert len(points)>=4
 return sum(points,Vector())/len(points)
def fit_contact(rig,pose,wrist,point,orientation,pole,local_grip):
 a,b=(7,8)if wrist==9 else(14,15)
 rig.solve(pose,a,b,wrist,point-orientation@local_grip,pole);rig.orient(pose,wrist,orientation)
 return (rig.fk(pose)[wrist]@local_grip-point).length
