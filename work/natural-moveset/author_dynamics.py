"""Adapt authored sprint/slide/jump sources for review on both native rigs."""
import sys,math,statistics
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['mixamo-dynamic-retarget-manifest.json','library-retarget-manifest.json','stance-retarget-manifest.json'],'dynamics-authored')
for role in ['corvo','emily']:
 rig=rig_for(role);J=rig.joints;sm,stand=L.load(role,'sword_idle');cm,crouch=L.load(role,'crouch_idle');hand=[]
 for i in range(len(J)):
  p=i
  while p>=0 and p!=16:p=J[p]['parent']
  if p==16 and i!=16 and 'helper'not in J[i]['nativeName']:hand.append(i)
 def grip(p):
  for i in hand:p[i]=stand[0][i].copy()
  preview_helpers(rig,p);return p
 m,fs=L.load(role,'mixamo_two_cycle_sprint');g=[rig.fk(p)for p in fs];est=[]
 for foot in [21,25]:
  low=min(p[foot].translation.z for p in g)
  for i in range(1,len(g)-1):
   if max(g[i-1][foot].translation.z,g[i+1][foot].translation.z)<low+.035:
    v=-(g[i+1][foot].translation.x-g[i-1][foot].translation.x)*.5*(len(g)-1)
    if 3<v<10:est.append(v)
 distance=statistics.median(est);speed=5.5;duration=distance/speed*2
 for name,angle in [('forward',0),('forward_left',math.pi/4),('forward_right',-math.pi/4)]:
  ps=[];roots=[];direction=Vector((math.cos(angle),math.sin(angle),0))
  for f in range(round(duration*60)+1):
   t=f/60;p=grip(sample(fs,t*speed/distance,True));rot=Matrix.Rotation(angle,3,'Z');rig.orient(p,2,rot@rig.fk(p)[2].to_3x3());p[2].translation=rot@p[2].translation
   # Legs run along travel; chest/head counter-yaw toward look direction.
   for j,fraction in [(3,.16),(4,.16),(5,.16),(11,.12),(12,.12)]:rig.orient(p,j,Matrix.Rotation(-angle*fraction,3,'Z')@rig.fk(p)[j].to_3x3())
   preview_helpers(rig,p);ps.append(p);roots.append(identity(*(direction*(t*speed))))
  L.save(rig,role,'sprint_'+name,ps,roots,'locomotion','Authored two-cycle sprint with native hand-local sword grip. Diagonals turn the lower body into travel and distribute counter-yaw through torso/head; distance determines cadence.',dict(alias=m['selection']['alias'],sha256=m['sha256']),stride_m=distance,source_cycles=2,review_speed_mps=speed)
 m,fs=L.load(role,'mixamo_running_slide');duration=m['duration'];floor=min(rig.fk(fs[0])[i].translation.z for i in [21,25]);low=min(range(len(fs)),key=lambda i:fs[i][2].translation.z)
 # Keep donor compression and contact. Root trajectory is metadata only.
 ps=[grip(sample(fs,f/(round(duration*60))))for f in range(round(duration*60)+1)];roots=[]
 for f in range(len(ps)):
  u=f/(len(ps)-1);at=u*(len(m['root_track'])-1);a=int(at);b=min(a+1,len(m['root_track'])-1);v=Vector(m['root_track'][a][4:7]).lerp(Vector(m['root_track'][b][4:7]),at-a);roots.append(identity(*v))
 L.save(rig,role,'slide_running_sequence',ps,roots,'traversal','Complete authored running-slide-running donor with native closed sword grip. Collision clearance and slide timing require actual player-state binding.',dict(alias=m['selection']['alias'],sha256=m['sha256']))
 lowtime=low/m['fps'];low60=round(lowtime*60);slidepose=ps[low60];loop=[]
 for f in range(61):
  p=copy_pose(slidepose);rig.orient(p,5,Matrix.Rotation(math.radians(.8*math.sin(f/60*math.tau)),3,'Y')@rig.fk(p)[5].to_3x3());preview_helpers(rig,p);loop.append(p)
 L.save(rig,role,'slide_loop',loop,[identity(f/60*4)for f in range(61)],'traversal','Sustained slide balance pose sampled from the authored lowest stance, with small chest motion. World travel is replicated; duration is a gameplay parameter.',dict(alias=m['selection']['alias'],sha256=m['sha256'],source_frame=low))
 L.save(rig,role,'slide_enter',ps[:low60+1],roots[:low60+1],'traversal','Authored compression into the slide, ending at the shared sustained-slide pose.',dict(alias=m['selection']['alias'],sha256=m['sha256'],source_frames=[0,low]))
 outroot=[identity(* (Vector(r[4:7])-Vector(roots[low60][4:7])))for r in roots[low60:]]
 L.save(rig,role,'slide_exit_to_run',ps[low60:],outroot,'traversal','Authored return from slide to running. Start pose matches slide_enter and slide_loop.',dict(alias=m['selection']['alias'],sha256=m['sha256'],source_frames=[low,len(fs)-1]))
 # Recovery to crouch keeps the authored initial recovery impulse, then
 # converges across the whole body into the common crouch stance.
 recover=[];feet0=rig.fk(slidepose);feet1=rig.fk(crouch[0]);targets0=[feet0[i].translation.copy()for i in [21,25]];targets1=[feet1[i].translation.copy()for i in [21,25]]
 for f in range(55):
  u=f/54;p=blend(sample(ps[low60:],u*.7),crouch[0],ease(u));pin_feet(rig,p,[a.lerp(b,ease(u))for a,b in zip(targets0,targets1)]);recover.append(p)
 recover[0]=copy_pose(slidepose);recover[-1]=copy_pose(crouch[0])
 L.save(rig,role,'slide_to_crouch',recover,[identity()for _ in recover],'traversal','Whole-body recovery from the shared slide pose into the common crouch stance. Foot transfer and collision contact remain visual review items.',dict(slide_sha256=m['sha256'],crouch_sha256=cm['sha256']))
 m,fs=L.load(role,'mixamo_running_jump');g=[rig.fk(p)for p in fs];floor=min(g[0][i].translation.z for i in [21,25]);ps=[];roots=[]
 for f in range(round(m['duration']*60)+1):
  u=f/(round(m['duration']*60));p=grip(sample(fs,u));global_=rig.fk(p);height=max(0,min(global_[i].translation.z for i in [21,25])-floor)
  # Algebraic root extraction preserves the complete offline pose while
  # making it possible to use replicated airborne position in the game.
  p[2].translation.z-=height;preview_helpers(rig,p);at=u*(len(m['root_track'])-1);a=int(at);b=min(a+1,len(m['root_track'])-1);v=Vector(m['root_track'][a][4:7]).lerp(Vector(m['root_track'][b][4:7]),at-a);v.z=height;ps.append(p);roots.append(identity(*v))
 L.save(rig,role,'running_jump_sequence',ps,roots,'traversal','Complete authored running jump. Foot-clearance height is separated as preview trajectory so it will not be added twice to a replicated airborne root. Capsule mapping and landing collision require runtime binding.',dict(alias=m['selection']['alias'],sha256=m['sha256']),airborne_root_extraction='Minimum authored ankle clearance over initial floor; exact visual recomposition, gameplay capsule calibration pending')
 print('SPRINT_DISTANCE',role,distance,'SLIDE_LOW_FRAME',low,'JUMP_HEIGHT',max(r[6]for r in roots),flush=True)
L.finish('dynamics-retarget-manifest.json')
