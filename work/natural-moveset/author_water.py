"""Water movement studies with free full-body strokes and separated travel.

Sculling donors supply lateral/backward motion; travel values are review
parameters, not claims about native water physics or final propulsion fit.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['mixamo-retarget-manifest-v5.json'],'water-movement-v1')
for role in ['corvo','emily']:
 rig=rig_for(role);sm,swim=L.load(role,'mixamo_swimming');tm,tread=L.load(role,'mixamo_treading_water')
 for action in ['forward','back','left','right','up','down','dive','surface','start_forward','stop_forward']:
  duration=sm['duration']if action in ['forward','up','down']else 3.;count=round(duration*60)+1;ps=[];roots=[]
  for f in range(count):
   t=f/60;u=f/(count-1);a=sample(swim,t/sm['duration'],True);b=sample(tread,t/tm['duration'],True);origin=Vector()
   if action in ['forward','up','down']:
    p=a;pitch=math.radians(-30 if action=='up'else 30 if action=='down'else 0);rig.orient(p,2,Matrix.Rotation(pitch,3,'Y')@rig.fk(p)[2].to_3x3());origin=Vector((math.cos(pitch),0,-math.sin(pitch)))*(.60*t)
   elif action in ['back','left','right']:
    p=b;direction=Vector((-1,0,0))if action=='back'else Vector((0,1 if action=='left'else -1,0));lean=math.sin(math.pi*min(1,t/.45)) if t<.45 else 0
    # Upright sculling keeps the face forward, as expected when looking
    # forward while translating sideways or backward under water.
    if action=='back':rig.orient(p,2,Matrix.Rotation(math.radians(-10),3,'Y')@rig.fk(p)[2].to_3x3())
    else:rig.orient(p,2,Matrix.Rotation(math.radians(-9 if action=='left'else 9),3,'X')@rig.fk(p)[2].to_3x3())
    origin=direction*(.25*t)
   elif action in ['dive','start_forward']:
    weight=ease((t-.25)/1.1);p=blend(b,a,weight);origin=Vector((.6*max(0,t-.65),0,-.8*ease((t-.35)/1.5)if action=='dive'else 0))
   else:
    weight=ease((t-.25)/1.1);p=blend(a,b,weight);origin=Vector((.6*min(t,1.3),0,.8*ease((t-.35)/1.5)if action=='surface'else 0))
   preview_helpers(rig,p);ps.append(p);roots.append(identity(*origin))
  L.save(rig,role,'water_'+action+'_sequence',ps,roots,'traversal','Whole-body water '+action.replace('_',' ')+' study. '+('Upright sculling donor supplies lateral/backward body motion; propulsion and leg timing need further review. 'if action in ['back','left','right']else'')+'Arms, spine, pelvis and legs remain authored motion; no first-person wrist anchors. World path and preview elevation are offline metadata. Water surface, capsule, buoyancy, collision, water-exit contact and gameplay binding are not demonstrated.',dict(swim_sha256=sm['sha256'],scull_sha256=tm['sha256']),preview_placement=identity(0,0,1.2),demonstration_speed_mps=.25 if action in ['back','left','right']else .6)
  L.reports[-1]['equipment_policy']='hands_free';(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2))
L.finish('water-movement-manifest-v1.json')
