"""Whole-body contact studies for native-style environmental interactions.

Neutral fixtures make physical contact reviewable. Contact constraints are
world/object-space only during touch; no first-person pose or wrist is fixed.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['library-retarget-manifest.json'],'mechanisms-v2')
for role in ['corvo','emily']:
 rig=rig_for(role);im,idle=L.load(role,'empty_idle');gm,grasp=L.load(role,'sword_doppelganger_assassination_fastback');reference=sample(grasp,.24);rg=rig.fk(reference);ig=rig.fk(idle[0]);feet=[ig[j].translation.copy()for j in [21,25]]
 for kind in ['button','lever','crank','wheel','key']:
  duration={'button':2.3,'lever':3.2,'crank':4.8,'wheel':5.2,'key':3.2}[kind];count=round(duration*60)+1;ps=[];tracks=[];visible=[];errors=[]
  for f in range(count):
   t=f/60;u=t/duration;engage=ease(t/.65)*(1-ease((t-(duration-.65))/.65));active=max(0,min(1,(t-.75)/(duration-1.5)));effort=math.sin(math.pi*active);p=sample(idle,t/im['duration'],True)
   p[2].translation+=Vector((.13*engage,-.025*engage,-.04*engage))
   twist=4*math.sin(active*math.tau*2)*effort if kind in ['crank','wheel']else 3*effort
   for j,w in [(2,.12),(3,.28),(4,.3),(5,.3)]:rig.orient(p,j,Matrix.Rotation(math.radians((7+3*effort)*w*engage),3,'Y')@Matrix.Rotation(math.radians(twist*w*engage),3,'Z')@rig.fk(p)[j].to_3x3())
   stepfeet=[v.copy()for v in feet];stepfeet[0].x+=.14*engage;stepfeet[0].z+=.045*math.sin(math.pi*engage)
   pin_feet(rig,p,stepfeet);free=copy_pose(p);targets={};prop=Matrix.Identity(4)
   if kind=='button':
    press=math.sin(math.pi*active)**4;prop.translation=Vector((.58+.018*press,-.17,1.24));targets[16]=prop.translation+Vector((-.055,0,0))
   elif kind=='lever':
    angle=math.radians(-35-55*ease(active));prop=Matrix.Rotation(angle,4,'Y');prop.translation=Vector((.64,.19,1.0));targets[9]=prop@Vector((-.02,0,.28))
   elif kind=='crank':
    angle=math.tau*2*active;prop=Matrix.Rotation(angle,4,'X');prop.translation=Vector((.56,.12,1.16));targets[9]=prop@Vector((-.075,.13,0))
   elif kind=='wheel':
    # Turn through a bounded arc, then let go. Repeated continuous wheel
    # operation needs a separate alternating regrip sequence.
    angle=math.radians(80)*ease(active);prop=Matrix.Rotation(angle,4,'X');prop.translation=Vector((.58,0,1.15));targets[9]=prop@Vector((-.025,.20,.11));targets[16]=prop@Vector((-.025,-.20,-.11))
   else:
    insertion=ease(active/.35);turn=ease((active-.4)/.35);prop=Matrix.Rotation(math.radians(85*turn),4,'X');prop.translation=Vector((.35+.18*insertion,-.16,1.18));targets[16]=prop.translation.copy()
   prop.translation.x-=.14
   for goal in targets.values():goal.x-=.14
   for wrist,goal in targets.items():
    a,b=(7,8)if wrist==9 else(14,15);sign=1 if wrist==9 else -1;pole=rig.fk(p)[a].translation+Vector((.12,.33*sign,-.30));rig.solve(p,a,b,wrist,goal,pole)
    orientation=rg[wrist].to_3x3()
    if kind in ['wheel','crank','key']:orientation=prop.to_3x3()@orientation
    rig.orient(p,wrist,orientation)
    for j in range(len(rig.joints)):
     ancestor=j
     while ancestor>=0 and ancestor!=wrist:ancestor=rig.joints[ancestor]['parent']
     if ancestor==wrist and j!=wrist and 'helper'not in rig.joints[j]['nativeName']:p[j]=reference[j].copy()
    if kind=='button':
     # Open index finger toward the button; final fingertip fit remains a
     # visual contact review item instead of a fabricated precision claim.
     for j,item in enumerate(rig.joints):
      if item['nativeName'].startswith('right_index'):p[j]=rig.bind[j].copy()
   p=blend(free,p,engage);preview_helpers(rig,p)
   if engage>.999:errors.append(max((rig.fk(p)[j].translation-goal).length for j,goal in targets.items()))
   ps.append(p);tracks.append(row(prop));visible.append(kind!='key'or .15<t<duration-.2)
  prop=dict(kind='fixture_'+kind,model='contact-fixtures/'+kind+'.json',skeleton='contact-fixtures/skeleton.json',parent_joint=0,attachment_track=tracks,visible=visible,mechanism=None,fixture=True,events=False)
  L.save(rig,role,'interact_'+kind+'_sequence',ps,[identity()for _ in ps],'interactions','Complete-body '+kind+' reach, operation and recovery study with planted feet, pelvis weight transfer and distributed spine effort. The neutral fixture demonstrates contact; it is not a game asset or UI. Hand shaping, exact target geometry and native interaction timing/binding remain to be validated.'+(' This is one bounded turn; continuous operation still needs alternating regrips.'if kind=='wheel'else''),dict(idle_sha256=im['sha256'],grasp_sha256=gm['sha256']),props=[prop],max_hand_target_error_m=max(errors,default=0))
  L.reports[-1]['equipment_policy']='hands_free';(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2));print('MECHANISM_CONTACT',role,kind,max(errors,default=0),flush=True)
L.finish('mechanisms-manifest-v2.json')
