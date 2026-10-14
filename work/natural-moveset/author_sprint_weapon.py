"""Constant anatomical forearm/wrist offsets for the authored armed sprint.

Find a modest local rotation over the entire cycle, never a world-space wrist
lock. Primitive clearance is only a screening test; require rendered review.
"""
import sys, math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['dynamics-retarget-manifest.json'],'dynamics-armed-v2')
reports=[]
def segment_distance(p,a,b):
 v=b-a;t=max(0,min(1,(p-a).dot(v)/max(1e-12,v.length_squared)));return(p-a-v*t).length
def offset(pose,roll,bend):
 p=copy_pose(pose)
 for i,axis,angle in [(15,'Y',roll),(16,'Z',bend)]:
  old=p[i];p[i]=old@Matrix.Rotation(math.radians(angle),4,axis);p[i].translation=old.translation
 return p
for role in ['corvo','emily']:
 rig=rig_for(role);m,frames=L.load(role,'sprint_forward')
 q=read(P.parent/('fullbody-proof/inputs/client-target-sword.json'if role=='corvo'else'crouch-grip-reference/host-target-sword.json'))['quaternion']
 binding=Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix().to_4x4()
 def measure(roll,bend):
  hmin=tmin=ground=10;penalty=0
  for pose in frames[::3]:
   g=rig.fk(offset(pose,roll,bend));s=g[17]@binding;head=g[12].translation+Vector((0,0,.08))
   for x in [.10,.24,.38,.52,.68]:
    point=s@Vector((x,0,0));dh=(point-head).length-.145;dt=segment_distance(point,g[3].translation,g[5].translation)-.19
    hmin=min(hmin,dh);tmin=min(tmin,dt);ground=min(ground,point.z)
    penalty+=max(0,.09-dh)**2*15+max(0,.035-dt)**2*5+max(0,.035-point.z)**2*10
  return dict(roll=roll,bend=bend,head_clearance=hmin,torso_clearance=tmin,ground_clearance=ground,penalty=penalty,score=penalty+.000003*(roll*roll+2*bend*bend))
 candidates=[measure(r,b)for r in [-60,-40,-20,0,20,40,60]for b in [-35,-20,0,20,35]]
 best=min(candidates,key=lambda x:x['score']);baseline=measure(0,0)
 reports.append(dict(role=role,baseline=baseline,selected=best,candidates=candidates,visual_accepted=False,method='Constant local forearm roll and wrist bend, screened against head/torso primitives and floor; native hand/socket preserved.'))
 for alias in ['sprint_forward','sprint_forward_left','sprint_forward_right']:
  m,frames=L.load(role,alias);ps=[];maxfoot=0
  for pose in frames:
   p=offset(pose,best['roll'],best['bend']);before=rig.fk(pose);after=rig.fk(p)
   maxfoot=max(maxfoot,max((before[i].translation-after[i].translation).length for i in [21,25]));preview_helpers(rig,p);ps.append(p)
  assert maxfoot<1e-8
  L.save(rig,role,alias,ps,m['root_track'],'locomotion',m['selection']['purpose']+' A constant anatomical forearm roll/wrist bend keeps the blade farther from the face while preserving free arm swing; numerical clearance is not visual acceptance.',dict(previous_sha256=m['sha256'],weapon_adjustment=best),stride_m=m['stride_m'],source_cycles=2,review_speed_mps=m['review_speed_mps'],foot_trajectory_error=maxfoot)
 print('WEAPON_CLEARANCE',role,'BASELINE',baseline,'SELECTED',best,flush=True)
L.reports.extend(m for m in L.rows if not m['selection']['alias'].startswith('sprint_'))
L.finish('dynamics-retarget-manifest-v2.json')
(P/'sprint-weapon-clearance-v2.json').write_text(json.dumps(reports,indent=2))
