"""Native full-body turn candidates with generated root/contact feedback.

Root transforms are authoring metadata only. No runtime installation here.
Use the accepted walking candidate's FP-inspired ready arm as the constraint.
"""
import json,math,bisect
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
P=Path(__file__).resolve().parent;OUT=P/'turns';OUT.mkdir(exist_ok=True)
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def qts(v):
 m=Quaternion((v[3],*v[:3])).normalized().to_matrix().to_4x4();m.translation=Vector(v[4:7]);return m
def ease(x):x=max(0,min(1,x));return x*x*(3-2*x)
C=Matrix(((0,1,0,0),(1,0,0,0),(0,0,1,0),(0,0,0,1)))
def ue(v):
 m=C@Matrix.LocRotScale(Vector(v[:3]),Quaternion((v[6],*v[3:6])).normalized(),Vector(v[7:10]))@C;m.translation*=.01;return m
mapping={2:'pelvis',3:'spine_01',4:'spine_03',5:'spine_05',6:'clavicle_l',7:'upperarm_l',8:'lowerarm_l',9:'hand_l',11:'neck_01',12:'head',13:'clavicle_r',14:'upperarm_r',15:'lowerarm_r',16:'hand_r',19:'thigh_l',20:'calf_l',21:'foot_l',22:'ball_l',23:'thigh_r',24:'calf_r',25:'foot_r',26:'ball_r'}
mesh=read(P.parent/'animgen-feasibility/exports/uefn-native-mesh.json')
class Rig:
 def __init__(self,role,donor):
  self.J=read(P.parent/f'mixamo-crouch/native-{role}.json')['joints'];self.B=[qts(j['bindLocalQTS']) for j in self.J];self.BG=self.fk(self.B);self.names={j['nativeName']:i for i,j in enumerate(self.J)}
  self.donor=donor;self.indices={i:donor['names'].index(n) for i,n in mapping.items()};sr=self.source_fk([b['local'] for b in mesh['bones']]);self.rest={i:sr[k] for i,k in self.indices.items()}
  self.corr={i:Matrix.Identity(3) for i in mapping}
  for i,ch in {7:8,8:9,14:15,15:16}.items():self.corr[i]=(self.BG[ch].translation-self.BG[i].translation).normalized().rotation_difference((self.rest[ch].translation-self.rest[i].translation).normalized()).to_matrix()
  self.scale=(self.BG[19].translation-self.BG[21].translation).length/(self.rest[19].translation-self.rest[21].translation).length
  self.floor=min(self.BG[22].translation.z,self.BG[26].translation.z);self.locks=[None,None]
  ready=read(P/'retargeted'/f'{role}-idle.json');self.ready=[qts(v) for v in ready['frames'][0]];rg=self.fk(self.ready);self.hand=rg[16];self.binding=ready['weapon_binding']
  self.fingers=[i for i,j in enumerate(self.J) if j['nativeName'].startswith('right') and any(s in j['nativeName'] for s in ('thumb','index','middle','ring','pinky'))]
 def fk(self,local):
  g=[]
  for i,j in enumerate(self.J):p=j['parent'];g.append(local[i] if p<0 else g[p]@local[i])
  return g
 def source_fk(self,values):
  g=[]
  for i,v in enumerate(values):p=self.donor['parents'][i];m=ue(v);g.append(m if p<0 else g[p]@m)
  return g
 def orient(self,local,i,rotation):
  g=self.fk(local);m=(g[self.J[i]['parent']].to_3x3().inverted()@rotation).to_4x4();m.translation=local[i].translation;local[i]=m
 def solve(self,local,a,b,c,goal,pole):
  g=self.fk(local);p=g[a].translation;L1=self.B[b].translation.length;L2=self.B[c].translation.length;v=goal-p;distance=max(.0001,min(v.length,L1+L2-.0005));axis=v.normalized();side=pole-p-axis*(pole-p).dot(axis);side.normalize();along=(distance*distance+L1*L1-L2*L2)/(2*distance);middle=p+axis*along+side*math.sqrt(max(0,L1*L1-along*along))
  for i,child,dst in ((a,b,middle),(b,c,goal)):
   g=self.fk(local);rot=(g[child].translation-g[i].translation).rotation_difference(dst-g[i].translation).to_matrix()@g[i].to_3x3();self.orient(local,i,rot)
 def helpers(self,local):
  g=[]
  for i,j in enumerate(self.J):
   p=j['parent'];n=j['nativeName'];driver=None;side='left' if n.startswith('left') else 'right' if n.startswith('right') else ''
   if side and 'helper' in n:
    if 'upper_leg' in n or 'thigh' in n:driver=side+'_femur_jnt'
    elif 'knee' in n:driver=side+'_tibia_jnt'
    elif 'arm01' in n and 'forearm' not in n:driver=side+'_arm_jnt'
    elif 'elbow' in n or 'forearm' in n:driver=side+'_elbow_jnt'
    elif 'wrist' in n:driver=side+'_wrist_jnt'
    elif 'ankle' in n:driver=side+'_ankle_jnt'
   if driver:
    k=self.names[driver];assert k<i;local[i]=g[p].inverted()@(g[k]@self.BG[k].inverted()@self.BG[i])
   g.append(local[i] if p<0 else g[p]@local[i])
 def pose(self,frame,root):
  sg=self.source_fk(frame['local']);local=[m.copy() for m in self.B];g=[]
  for i,j in enumerate(self.J):
   p=j['parent']
   if i in mapping:
    target=sg[self.indices[i]].to_quaternion().to_matrix()@self.rest[i].to_quaternion().to_matrix().inverted()@self.corr[i]@self.BG[i].to_3x3();rot=g[p].to_3x3().inverted()@target if p>=0 else target
    local[i]=rot.to_quaternion().normalized().to_matrix().to_4x4();local[i].translation=self.B[i].translation
    if i==2:local[i].translation+=(sg[self.indices[i]].translation-self.rest[i].translation)*self.scale
   g.append(local[i] if p<0 else g[p]@local[i])
  local[2].translation.z+=self.floor-min(g[22].translation.z,g[26].translation.z)
  for side,(a,b,c) in enumerate(((19,20,21),(23,24,25))):
   g=self.fk(local);world=(root@g[c]).translation;confidence=frame['contacts'][side]
   if confidence>=.85 and self.locks[side] is None:self.locks[side]=world.copy()
   if confidence<.15:self.locks[side]=None
   if self.locks[side] is not None:
    goal=root.inverted()@world.lerp(self.locks[side],ease((confidence-.15)/.70));rot=g[c].to_3x3();self.solve(local,a,b,c,goal,Vector((.8,goal.y,.5)));self.orient(local,c,rot)
  self.solve(local,14,15,16,self.hand.translation,Vector((.02,-.64,1.2)));self.orient(local,16,self.hand.to_3x3())
  for i in self.fingers:local[i]=self.ready[i].copy()
  local[17]=self.ready[17].copy();self.helpers(local)
  return local
def as_row(local):
 row=[]
 for m in local:
  q=m.to_quaternion().normalized();row.append([q.x,q.y,q.z,q.w,*m.translation])
 return row
def sample(a,b,t):
 row=[]
 for x,y in zip(a,b):
  q=Quaternion((x[3],*x[:3])).slerp(Quaternion((y[3],*y[:3])),t);row.append([q.x,q.y,q.z,q.w,*[x[k]+(y[k]-x[k])*t for k in range(4,7)]])
 return row
report=[]
for role in ('corvo','emily'):
 for name in ('turn_left_90','turn_right_90','turn_left_180','turn_right_180'):
  donor=read(P/'donors-feedback'/f'{name}.json');rig=Rig(role,donor);signed=math.radians(donor['request']['turn']);sign=1 if signed>0 else -1
  angles=[];last=0;unwrapped=0
  for f in donor['frames']:
   q=f['feedback_rotation'];a=-math.atan2(2*q[3]*q[2],1-2*q[2]*q[2]);unwrapped+=(a-last+math.pi)%(2*math.pi)-math.pi;last=a;angles.append(unwrapped*sign)
  # Full observed angular range; remove tiny initial/final inference noise.
  start=next(i for i,a in enumerate(angles) if a>=math.radians(.5));end=next(i for i,a in enumerate(angles) if i>start and a>=abs(signed)-math.radians(.5))
  low,high=angles[start],angles[end];curve=[max(0,min(1,(a-low)/(high-low))) for a in angles]
  for i in range(1,len(curve)):curve[i]=max(curve[i],curve[i-1])
  frames=[];length_error=0
  for i,f in enumerate(donor['frames'][:end+1]):
   # Pure rotation target: generated position drift is deliberately removed.
   root=Matrix.Rotation(angles[i]*sign,4,'Z');local=rig.pose(f,root);frames.append(as_row(local))
   length_error=max(length_error,max((local[j].translation-rig.B[j].translation).length for j in range(3,27) if j!=17))
  normalized=[]
  for i in range(121):
   u=i/120;k=max(start,min(end-1,bisect.bisect_right(curve,u)-1));t=(u-curve[k])/max(1e-9,curve[k+1]-curve[k]);normalized.append(sample(frames[k],frames[k+1],max(0,min(1,t))))
  meta={'role':role,'name':name,'signed_angle_radians':signed,'source_frames':[start,end],'source_seconds':(end-start)/60,'frames':normalized,'joints':[j['nativeName'] for j in rig.J],'parents':[j['parent'] for j in rig.J],
   'weapon_binding':rig.binding,'root_angles':[signed*i/120 for i in range(121)],'native_length_error_m':length_error,'root_writes':False,'native_first_person_changed':False,'loop_closed':False,'d2_playback_verified':False}
  (OUT/f'{role}-{name}.json').write_text(json.dumps(meta,separators=(',',':')))
  report.append({k:v for k,v in meta.items() if k not in ('frames','joints','parents','root_angles','weapon_binding')});print(role,name,start,end,round((end-start)/60,3),length_error,flush=True)
(OUT/'manifest.json').write_text(json.dumps(report,indent=2));print('NATIVE_TURN_CANDIDATES_WRITTEN')
