"""Low-body Shadow Walk movement study, without transformation or effects.

This evaluates a complete authored donor on the native human rig. It is not
a claim that D2's native supernatural body presentation has been reproduced.
"""
import sys,math,statistics
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['mixamo-low-crawl-retarget-manifest.json','library-retarget-manifest.json','stance-retarget-manifest.json'],'shadow-body-v1')
for role in ['corvo','emily']:
 rig=rig_for(role);cm,crawl=L.load(role,'mixamo_low_crawl');im,idle=L.load(role,'empty_idle');sm,crouch=L.load(role,'crouch_idle');gs=[rig.fk(p)for p in crawl];vel=[]
 # Estimate the original contact speed from low, rearward-moving palms.
 for joint in [9,16]:
  floor=min(g[joint].translation.z for g in gs)
  for a,b in zip(gs,gs[1:]):
   v=(b[joint].translation.x-a[joint].translation.x)*cm['fps']
   if max(a[joint].translation.z,b[joint].translation.z)<floor+.045 and v<-.05:vel.append(-v)
 speed=statistics.median(vel)if vel else .35;speed=max(.1,min(1.2,speed));transition=[]
 for f in range(91):
  t=f/60
  if t<.6:p=blend(idle[0],crouch[0],ease(t/.6))
  else:
   u=ease((t-.6)/.9);p=blend(crouch[0],crawl[0],u);cg=rig.fk(crouch[0]);low=gs[0];targets=[cg[j].translation.lerp(low[j].translation,u)for j in [21,25]]
   for v in targets:v.z+=.045*math.sin(math.pi*u)
   pin_feet(rig,p,targets)
   # Keep hands above the floor during the descent, allowing authored
   # contact to take over at the end rather than locking wrists to a camera.
   g=rig.fk(p)
   for a,b,c in [(7,8,9),(14,15,16)]:
    if g[c].translation.z<.025:
     target=g[c].translation.copy();target.z=.025;rotation=g[c].to_3x3();rig.solve(p,a,b,c,target,g[b].translation);rig.orient(p,c,rotation)
  preview_helpers(rig,p);transition.append(p)
 for action in ['enter','idle','move_forward','attack_study','exit']:
  duration=1.5 if action in ['enter','exit']else cm['duration']*2 if action=='move_forward'else 2.8;count=round(duration*60)+1;ps=[];roots=[]
  for f in range(count):
   t=f/60;u=f/(count-1);root=qts(identity())
   if action=='enter':p=sample(transition,u)
   elif action=='exit':p=sample(transition,1-u)
   elif action=='move_forward':p=sample(crawl,t/cm['duration'],True);root.translation.x=speed*t
   elif action=='idle':
    p=copy_pose(crawl[0]);p[2].translation.z+=.006*math.sin(t*2.4)
    for j,w in [(3,.35),(4,.35),(5,.3)]:rig.orient(p,j,Matrix.Rotation(math.radians(1.2*w*math.sin(t*1.6)),3,'Z')@rig.fk(p)[j].to_3x3())
   else:
    p=copy_pose(crawl[0]);effort=math.sin(math.pi*ease((u-.1)/.8))**2;p[2].translation.z+=.14*effort;p[2].translation.x+=.08*effort
    for j,w in [(3,.35),(4,.35),(5,.3)]:rig.orient(p,j,Matrix.Rotation(math.radians(-18*w*effort),3,'Y')@rig.fk(p)[j].to_3x3())
    g=rig.fk(p);target=g[9].translation+Vector((.32*effort,-.14*effort,.23*effort));rig.solve(p,7,8,9,target,g[8].translation+Vector((0,.1,.05)));root.translation.x=.48*ease((u-.2)/.4)
   preview_helpers(rig,p);ps.append(p);roots.append(row(root))
  L.save(rig,role,'shadow_walk_'+action,ps,roots,'powers','Shadow Walk low-body '+action.replace('_',' ')+' motion study using an authored in-place Low Crawl donor and native-rig transitions. '+('Exit currently reverses the entry study; dedicated push-up recovery needs refinement. 'if action=='exit'else'')+'This is a human-rig movement candidate only: native shadow morphology, effects, wall/rathole travel, attack target contact and gameplay binding are not reproduced. Emily is the base-campaign target; Corvo is a comparison adaptation.',dict(donor_sha256=cm['sha256'],donor_download=cm['source'].get('download'),stand_sha256=im['sha256'],crouch_sha256=sm['sha256']),contact_speed_estimate_mps=speed)
  L.reports[-1]['equipment_policy']='hands_free';(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2))
 print('SHADOW_CONTACT_SPEED',role,speed,flush=True)
L.finish('shadow-body-manifest-v1.json')
