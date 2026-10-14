import re
from clip_io import *
SR=NativeRig(read(P/'fp-leftarm-skeleton.json')['joints'])
SN={j['nativeName']:i for i,j in enumerate(SR.joints)}

def palm(g,w,i,p,m):
 inv=g[w].inverted();a=inv@g[i].translation;b=inv@g[p].translation;c=inv@g[m].translation
 x=(a-b).normalized();y=(c-x*c.dot(x)).normalized();return Matrix((x,y,x.cross(y))).transposed()

class HandReference:
 def __init__(self,rig):
  self.rig=rig;self.names={j['nativeName']:i for i,j in enumerate(rig.joints)};self.mapping={}
  for i,j in enumerate(rig.joints):
   match=re.fullmatch(r'left_(thumb|index|middle|ring|pinky)(\d\d)_jnt',j['nativeName'])
   if not match:continue
   finger,part=match.groups();k=int(part)
   # FP index/middle/ring have a metacarpal absent from the target. The
   # target pinky retains that extra segment; thumb layouts agree.
   if finger in ('index','middle','ring'):k+=1
   src='left_'+('medium'if finger=='middle'else finger)+'_'+str(k).zfill(2)+'_jnt'
   if src in SN:self.mapping[i]=SN[src]
  assert len(self.mapping)==16
  sg=SR.global_bind;tg=rig.global_bind
  sp=palm(sg,4,SN['left_index_02_jnt'],SN['left_pinky_02_jnt'],SN['left_medium_02_jnt'])
  tp=palm(tg,9,self.names['left_index01_jnt'],self.names['left_pinky02_jnt'],self.names['left_middle01_jnt'])
  self.cal=sp@tp.inverted();self.invcal=self.cal.inverted()
  self.srest={i:sg[4].to_3x3().inverted()@sg[s].to_3x3()for i,s in self.mapping.items()}
  self.trest={i:tg[9].to_3x3().inverted()@tg[i].to_3x3()for i in self.mapping}
 def apply(self,pose,source,weight=1):
  sg=SR.fk(source);si=sg[4].to_3x3().inverted();w=self.rig.fk(pose)[9].to_3x3()
  for i,s in sorted(self.mapping.items()):
   relative=si@sg[s].to_3x3()
   desired=w@self.invcal@relative@self.srest[i].inverted()@self.cal@self.trest[i]
   current=self.rig.fk(pose)[i].to_quaternion()
   self.rig.orient(pose,i,current.slerp(desired.to_quaternion(),weight).to_matrix())

