"""Whole-body crouched turns and attacks from native authored actions.

The low stance is a full-body pose offset, followed by anatomical foot IK.
Native turn/attack timing and shoulder motion survive. No FP hand anchors.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['library-retarget-manifest.json','stance-retarget-manifest.json'],'crouch-actions-v2')
for role in ['corvo','emily']:
 rig=rig_for(role);sm,sf=L.load(role,'sword_idle');cm,cf=L.load(role,'crouch_idle');stand=sf[0];crouch=cf[0]
 sg=rig.fk(stand);cg=rig.fk(crouch);delta={i:(crouch[i].to_quaternion()@stand[i].to_quaternion().inverted()).normalized()for i in range(2,27)if i!=17}
 shifts=[cg[i].translation-sg[i].translation for i in [21,25]]
 hand=[]
 for i in range(len(rig.joints)):
  a=i
  while a>=0 and a!=16:a=rig.joints[a]['parent']
  if a==16 and i!=16 and 'helper'not in rig.joints[i]['nativeName']:hand.append(i)
 requests=[('crouch_turn_'+side+str(angle),'sword_turn'+side+str(angle),'stance')for side in ['left','right']for angle in [45,90,180]]
 requests += [('crouch_attack_fast_a','sword_attack_fasta','combat'),('crouch_attack_fast_b','sword_attack_fastb','combat'),('crouch_attack_short_a','sword_attack_shorta','combat')]
 for label,alias,family in requests:
  m,fs=L.load(role,alias);frames=round(m['duration']*60)+1;poses=[];roots=[];errors=[]
  attack=family=='combat'
  reference=stand if attack else fs[0]
  delta={i:(crouch[i].to_quaternion()@reference[i].to_quaternion().inverted()).normalized()for i in range(2,27)if i!=17}
  reference_g=rig.fk(reference);shifts=[cg[i].translation-reference_g[i].translation for i in [21,25]]
  for f in range(frames):
   u=f/(frames-1);source=sample(fs,u);g=rig.fk(source);p=copy_pose(source)
   for i,d in delta.items():
    # Native attack arm articulation stays complete. The crouch offset
    # affects legs, pelvis and spine, while shoulders inherit body acting.
    weight=(0 if i in [6,7,8,9,13,14,15,16]else .30 if i in [2,3,4,5,11,12]else 1)if attack else 1
    q=Quaternion().slerp(d,weight)@p[i].to_quaternion();v=p[i].translation.copy();p[i]=q.normalized().to_matrix().to_4x4();p[i].translation=v
   p[2].translation+=crouch[2].translation-reference[2].translation
   if attack:p[2].translation.z+=.14*math.sin(math.pi*u)**2
   if not attack:
    for i in hand:p[i]=crouch[i].copy()
   targets=[g[i].translation+shifts[k]for k,i in enumerate([21,25])]
   pin_feet(rig,p,targets,[g[i].to_3x3()for i in [21,25]])
   actual=rig.fk(p);errors.append(max((actual[i].translation-targets[k]).length for k,i in enumerate([21,25])))
   poses.append(p)
   at=u*(len(m['root_track'])-1);a=int(at);b=min(a+1,len(m['root_track'])-1);ra=qts(m['root_track'][a]);rb=qts(m['root_track'][b]);roots.append(row(blend([ra],[rb],at-a)[0]))
  L.save(rig,role,label,poses,roots,family,'Native authored '+alias+' adapted to a complete crouched stance, retaining native segment lengths and source foot trajectories. '+('Native attacking arm and ordinary combat grip remain free. Crouch spine offset is reduced to avoid stacking two forward leans; pelvis rises 14cm through the effort. Reverse-grip transitions are not authored yet.'if attack else 'Turn offsets are calibrated against the source entry pose so the turn begins at the actual Posture-100 stance; both free arms and reverse grip follow the full-body turn.')+' Contact, low obstacle clearance and transitions require visual review; game action binding is pending.',dict(native_alias=alias,native_sha256=m['sha256'],crouch_sha256=cm['sha256']),max_foot_target_error=max(errors),equipment_transition_pending=attack)
 # Forward/downward ledge inspection with counterbalancing hips and a
 # moving head. No eye/hand camera-space locks or automatic root movement.
 ps=[];feet=[cg[i].translation.copy()for i in [21,25]];rots=[cg[i].to_3x3()for i in [21,25]]
 for f in range(181):
  t=f/60;w=ease(t/.6)if t<.6 else 1 if t<2 else 1-ease((t-2)/1)
  p=sample(cf,t/cm['duration'],True);p[2].translation.x-=.065*w;p[2].translation.z-=.03*w
  for j,degrees in [(2,4),(3,5),(4,5),(5,4),(11,7),(12,5)]:rig.orient(p,j,Matrix.Rotation(math.radians(degrees*w),3,'Y')@rig.fk(p)[j].to_3x3())
  pin_feet(rig,p,feet,rots);ps.append(p)
 L.save(rig,role,'ledge_peek',ps,[identity()for _ in ps],'stance','Crouched forward/downward ledge peek with pelvis counterbalance, distributed spine and neck motion, planted feet and free arms. Ledge geometry and collision-aware limits are not bound.',dict(crouch_sha256=cm['sha256']))
L.finish('crouch-actions-retarget-manifest-v2.json')
