"""Sustained guard and counter sequences built around native parry acting."""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['library-retarget-manifest.json'],'guard-sequences-v1')
for role in ['corvo','emily']:
 rig=rig_for(role);im,idle=L.load(role,'sword_idle')
 for quadrant in ['ll','lr','rl','rr']:
  em,entry=L.load(role,'sword_parry_'+quadrant+'_in');om,out=L.load(role,'sword_parry_'+quadrant+'_out');am,attack=L.load(role,'sword_parry_'+quadrant+'_attack')
  for counter in [False,True]:
   ps=[];roots=[];marks=[];phase=0.;root=Matrix.Identity(4)
   def append(name,duration,make):
    marks.append(dict(phase=name,at=len(ps)/60));count=round(duration*60)+1
    for i in range(count):
     pose,origin=make(i/(count-1),i/60);preview_helpers(rig,pose);ps.append(pose);roots.append(row(origin))
   append('ready',.35,lambda u,t:(blend(sample(idle,t/im['duration'],True),entry[0],ease(u)),root))
   append('native_parry',em['duration'],lambda u,t:(sample(entry,u),root))
   def hold(u,t):
    p=copy_pose(entry[-1]);g=rig.fk(p);feet=[g[j].translation.copy()for j in [21,25]];w=math.sin(math.pi*u)**2
    p[2].translation.z+=.006*math.sin(t*math.tau/.95)*w
    for j,deg in [(3,.6),(4,.7),(5,.7),(11,1.5),(12,1.)]:rig.orient(p,j,Matrix.Rotation(math.radians(deg*math.sin(t*2.1)*w),3,'Z')@rig.fk(p)[j].to_3x3())
    pin_feet(rig,p,feet);return p,root
   append('guard_hold',2.1,hold)
   if counter:
    prior=ps[-1]
    def counter_pose(u,t):
     p=sample(attack,u);p=blend(prior,p,ease(t/.12));origin=sample([[qts(r)]for r in am['root_track']],u)[0];origin=qts(am['root_track'][0]).inverted()@origin;return p,origin
    append('counterattack',am['duration'],counter_pose);root=qts(roots[-1]);prior=ps[-1]
    append('recover',.55,lambda u,t:(blend(prior,sample(idle,u,True),ease(u)),root))
   else:
    append('native_recovery',om['duration'],lambda u,t:(sample(out,u),root));prior=ps[-1]
    append('recover',.4,lambda u,t:(blend(prior,sample(idle,u,True),ease(u)),root))
   alias='guard_'+quadrant+('_counter_sequence'if counter else'_hold_sequence')
   L.save(rig,role,alias,ps,roots,'combat','Connected native parry, sustained full-body guard and '+('counterattack'if counter else'recovery')+' study. Native quadrant code '+quadrant+' is retained without inventing a directional meaning. Subtle pelvis/spine/head motion supports the hold; wrists and sword follow the body. Impact timing, interruption and game combat-state binding are pending.',dict(idle_sha256=im['sha256'],entry_sha256=em['sha256'],recovery_sha256=om['sha256'],counter_sha256=am['sha256']),phase_marks=marks)
L.finish('guard-sequences-manifest-v1.json')
