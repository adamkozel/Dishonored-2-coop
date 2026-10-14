"""Offline carried-body contact study using actual native victim motion.

The carrier is complete-body acting; first-person wrists are never anchors.
This does not attach a live NPC, change its state, or execute damage events.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['library-retarget-manifest.json','gait-review-manifest.json','carry-victim-retarget-manifest.json'],'carry-body-v1');actors=[];victims=[]
for role in ['corvo','emily']:
 rig=rig_for(role);vrig=rig_for('corvo');im,idle=L.load(role,'sword_idle');ig=rig.fk(idle[0])
 for action in ['idle','walk']:
  bm,body=L.load(role,'sword_idle'if action=='idle'else'walk_forward');vm,vbody=L.load('corvo','generic_carrycorpse_'+action)
  duration=3.2 if action=='walk' else 3.;count=round(duration*60)+1;ps=[];vs=[];roots=[];vroots=[];errors=[]
  for f in range(count):
   t=f/60;speed=.65;source_speed=(qts(bm['root_track'][-1]).translation-qts(bm['root_track'][0]).translation).length/max(bm['duration'],.001)
   clock=t*speed/max(source_speed,.001) if action=='walk' else t
   u=clock/max(bm['duration'],.001);p=sample(body,u,True);g0=rig.fk(p);feet=[g0[j].translation.copy()for j in [21,25]]
   p[2].translation+=Vector((-.015,-.035,-.04))
   for j,w in [(2,.12),(3,.28),(4,.3),(5,.3)]:
    rotation=Matrix.Rotation(math.radians(9*w),3,'Y')@Matrix.Rotation(math.radians(6*w),3,'X')@rig.fk(p)[j].to_3x3();rig.orient(p,j,rotation)
   pin_feet(rig,p,feet);g=rig.fk(p);v=sample(vbody,clock/vm['duration'],True);vg=vrig.fk(v)
   # Place the carried body's abdomen over the loaded shoulder. Native
   # victim poses supply its sag and leg motion; the shoulder supplies sway.
   rotation=g[5].to_3x3()@ig[5].to_3x3().inverted();attach=rotation.to_4x4();attach.translation=g[7].translation+Vector((.025,.045,.115))-rotation@vg[4].translation
   knee=attach@vg[20].translation;goal=knee+Vector((.015,-.055,.055))
   rig.solve(p,7,8,9,goal,g[7].translation+Vector((.15,.25,-.35)))
   # The forearm follows the supported leg, with hand orientation inherited
   # from native grasp acting, not copied from first-person camera space.
   _,grasp=L.load(role,'sword_doppelganger_assassination_fastback') if f==0 else (None,grasp)
   grasp_pose=sample(grasp,.24);gg=rig.fk(grasp_pose);rig.orient(p,9,rotation@gg[9].to_3x3())
   for j in range(len(rig.joints)):
    a=j
    while a>=0 and a!=9:a=rig.joints[a]['parent']
    if a==9 and j!=9 and 'helper'not in rig.joints[j]['nativeName']:p[j]=grasp_pose[j].copy()
   preview_helpers(rig,p);errors.append((rig.fk(p)[9].translation-goal).length)
   root=qts(identity(t*.65 if action=='walk' else 0,0,0));ps.append(p);vs.append(v);roots.append(row(root));vroots.append(row(root@attach))
  alias='carry_body_'+action;L.save(vrig,'corvo','victim_'+role+'_'+alias,vs,vroots,'carried_body_reference','Native carried victim pose; offline shoulder placement.',dict(victim_sha256=vm['sha256']));target=L.reports[-1];victims.append(target)
  partner=dict(role='corvo',id=target['id'],binary=target['binary'],sha256=target['sha256'],frames=count,fps=60,duration=duration,root_track=vroots,placement=vroots[0],appearance='Neutral-grey carried-body stand-in',source=vm['source']['asset'],placement_basis='Authored abdomen-to-left-shoulder contact, not a verified engine attachment')
  L.save(rig,role,alias,ps,roots,'interactions','Carried-body '+action+' study: loaded shoulder, whole-body counterbalance, supported leg and original native victim sag. Position remains network-owned. Grip, weight, lift/drop transitions, native body attachment and gameplay ownership require review.',dict(body_sha256=bm['sha256'],victim_sha256=vm['sha256']),partner=partner,max_hand_target_error_m=max(errors))
  actors.append(L.reports[-1]);print('CARRY_CONTACT',role,action,max(errors),flush=True)
(P/'carry-body-manifest-v1.json').write_text(json.dumps(actors,indent=2));(P/'carry-body-victims-v1.json').write_text(json.dumps(victims,indent=2))
