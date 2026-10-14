"""Eight-direction full-body crouch study, using authored cardinal donors.

Diagonals interpolate whole bodies at a shared foot phase. No animation root
motion is sent to D2. Cadence comes from measured planted-foot travel.
"""
import sys,math,statistics
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['mixamo-retarget-manifest-v5.json','mixamo-dynamic-retarget-manifest.json','stance-retarget-manifest.json'],'crouch-directions-authored')
for role in ['corvo','emily']:
 rig=rig_for(role);J=rig.joints;fm,forward=L.load(role,'mixamo_crouched_walking_posture100');im,idle=L.load(role,'crouch_idle')
 def meanq(fs,i):
  q=fs[0][i].to_quaternion()
  for k in range(1,len(fs)):q=q.slerp(fs[k][i].to_quaternion(),1/(k+1))
  return q.normalized()
 upper={i:meanq(forward,i)for i in [3,4,5,6,7,8,9,11,12,13,14,15,16]}
 descendants=[]
 for i in range(len(J)):
  p=i
  while p>=0 and p!=16:p=J[p]['parent']
  if p==16 and i!=16 and 'helper'not in J[i]['nativeName']:descendants.append(i)
 sources={};card={};stride={};directions={'forward':Vector((1,0,0)),'left':Vector((0,1,0)),'back':Vector((-1,0,0)),'right':Vector((0,-1,0))}
 for name,alias in [('forward','mixamo_crouched_walking_posture100'),('left','mixamo_crouch_walk_left'),('back','mixamo_crouch_walk_back'),('right','mixamo_crouch_walk_right')]:
  meta,fs=L.load(role,alias);sources[name]=dict(alias=alias,sha256=meta['sha256']);direction=directions[name]
  # Match arm/spine style using a constant local orientation offset. The
  # donor's changing full-body motion survives; wrists remain free.
  if name!='forward':
   offsets={i:Quaternion().slerp(upper[i]@meanq(fs,i).inverted(),.70 if i in [6,7,8,9,13,14,15,16]else .35)for i in upper}
   for p in fs:
    for i,q in offsets.items():
     v=p[i].translation.copy();p[i]=(q@p[i].to_quaternion()).normalized().to_matrix().to_4x4();p[i].translation=v
    for i in descendants:p[i]=forward[0][i].copy()
  global_poses=[rig.fk(p)for p in fs]
  # Align left-foot swing maximum, retaining the authored step ordering.
  phase=max(range(len(fs)-1),key=lambda f:global_poses[f][21].translation.z)/(len(fs)-1)
  cycle=[sample(fs,k/120+phase,True)for k in range(121)]
  for k in range(115,121):cycle[k]=blend(cycle[k],cycle[0],ease((k-114)/6))
  cycle[-1]=copy_pose(cycle[0]);g=[rig.fk(p)for p in cycle];est=[]
  for foot in [21,25]:
   low=min(x[foot].translation.z for x in g)
   for f in range(2,115):
    if max(g[f-1][foot].translation.z,g[f+1][foot].translation.z)<low+.035:
     travel=-(g[f+1][foot].translation-g[f-1][foot].translation).dot(direction)*60
     if .4<travel<3:est.append(travel)
  assert len(est)>3,(role,name,'insufficient planted-foot evidence')
  stride[name]=statistics.median(est);assert .5<stride[name]<2.5,(name,stride[name])
  for p in cycle:preview_helpers(rig,p)
  card[name]=cycle
 for name,a,b in [('forward','forward',None),('forward_left','forward','left'),('left','left',None),('back_left','back','left'),('back','back',None),('back_right','back','right'),('right','right',None),('forward_right','forward','right')]:
  cycle=card[a]if b is None else[blend(x,y,.5)for x,y in zip(card[a],card[b])]
  displacement=directions[a]*stride[a]if b is None else(directions[a]*stride[a]+directions[b]*stride[b])*.5
  step=displacement.length;direction=displacement.normalized();speed=.9;duration=2*step/speed
  poses=[];roots=[]
  for f in range(round(duration*60)+1):
   t=f/60;p=sample(cycle,t*speed/step,True);preview_helpers(rig,p);poses.append(p);v=direction*(t*speed);roots.append(identity(*v))
  L.save(rig,role,'crouch_walk_'+name,poses,roots,'stance','Authored whole-body cardinal crouch, or whole-body phase-aligned diagonal blend. Cadence is fitted to planted-ankle travel. Diagonal contact and forward-arm style still require visual acceptance.',dict(cardinals=[sources[x]for x in [a,b]if x]),stride_m=step,review_speed_mps=speed,contact_fit='Median ankle travel near stance; diagonal stride is the length of blended cardinal travel',diagonal_blend=b is not None)
 print('CROUCH_STRIDES',role,stride,flush=True)
L.finish('crouch-directions-retarget-manifest.json')
