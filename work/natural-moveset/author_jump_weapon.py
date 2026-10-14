"""Screen a local forearm pronation correction, then require rendered review.
The twist axis is the actual elbow-to-wrist bone axis, preserving wrist travel.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['jumps-retarget-manifest-v2.json'],'jumps-authored-v3');reports=[]
def segment_distance(p,a,b):
 v=b-a;u=max(0,min(1,(p-a).dot(v)/max(v.length_squared,1e-8)));return(p-a-v*u).length
def envelope(m,t):
 marks={p['phase']:p['at']for p in m.get('phase_marks',[])};takeoff=marks.get('takeoff',m['source']['takeoff_frame']/60);land=marks.get('landing',m['source']['landing_frame']/60+(.8 if m['selection']['alias']=='double_jump'else 0))
 return ease((t-takeoff+.28)/.28)*(1-ease((t-land-.04)/.25))
for role in ['corvo','emily']:
 rig=rig_for(role);axis=rig.bind[16].translation.normalized();q=read(P.parent/('fullbody-proof/inputs/client-target-sword.json'if role=='corvo'else'crouch-grip-reference/host-target-sword.json'))['quaternion'];binding=Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix().to_4x4()
 m,baseline=L.load(role,'standing_jump_sequence')
 def offset(p,roll,bend,weight):
  out=copy_pose(p)
  for j,rotation in [(15,Quaternion(axis,math.radians(roll*weight)).to_matrix().to_4x4()),(16,Matrix.Rotation(math.radians(bend*weight),4,'Z'))]:
   out[j]=p[j]@rotation;out[j].translation=p[j].translation
  return out
 def measure(roll,bend):
  penalty=0;head_min=10;body_min=10;face_alignment=-1
  for f in range(0,len(baseline),3):
   weight=envelope(m,f/60);g=rig.fk(offset(baseline[f],roll,bend,weight));s=g[17]@binding;head=g[12].translation+Vector((0,0,.07));handle=s.translation;direction=s.to_3x3()@Vector((1,0,0))
   inward=direction.dot((head-handle).normalized());face_alignment=max(face_alignment,inward)
   penalty+=max(0,inward-.15)**2*2*weight+max(0,direction.z+.1)**2*.5*weight
   for x in [.10,.22,.34,.46,.58,.68]:
    point=s@Vector((x,0,0));dh=(point-head).length-.15;db=segment_distance(point,g[3].translation,g[5].translation)-.19;head_min=min(head_min,dh);body_min=min(body_min,db)
    penalty+=max(0,.13-dh)**2*30+max(0,.05-db)**2*10
  return dict(roll=roll,bend=bend,score=penalty+.00003*(roll*roll+2*bend*bend),head_clearance=head_min,torso_clearance=body_min,inward_alignment=face_alignment)
 candidates=[measure(roll,bend)for roll in range(-90,91,15)for bend in [-25,0,25]];best=min(candidates,key=lambda r:r['score']);reports.append(dict(role=role,baseline=measure(0,0),chosen=best,candidates=candidates,visual_accepted=False))
 for source in [r for r in L.rows if r['role']==role]:
  name=source['selection']['alias'];meta,fs=L.load(role,name);ps=[];wrist_error=0
  for f,p in enumerate(fs):
   result=offset(p,best['roll'],best['bend'],envelope(meta,f/60));wrist_error=max(wrist_error,(rig.fk(p)[16].translation-rig.fk(result)[16].translation).length);preview_helpers(rig,result);ps.append(result)
  assert wrist_error<1e-5
  L.save(rig,role,name,ps,meta['root_track'],'traversal',meta['selection']['purpose']+' A bounded local forearm pronation/wrist correction is blended during flight; the wrist position is unchanged. Clearance remains a visual review item.',dict(previous_sha256=meta['sha256'],authored_source=meta['source'],weapon_correction=best),phase_marks=meta.get('phase_marks',[]),wrist_trajectory_error_m=wrist_error)
 print('JUMP_WEAPON',role,best,flush=True)
L.finish('jumps-retarget-manifest-v3.json');(P/'jump-weapon-screening-v3.json').write_text(json.dumps(reports,indent=2))
