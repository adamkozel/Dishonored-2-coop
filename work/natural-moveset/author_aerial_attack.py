"""One-handed adaptation of a complete authored jump attack.

The unused shield/support arm becomes a body-relative balance gesture.
The right hand retains native closed fingers/socket. No FP spatial anchors.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from contact_hand import descendants
from rig_math import ease
L=Library(['mixamo-aerial-retarget-manifest.json','library-retarget-manifest.json'],'aerial-attack-v1');report=[]
for role in ['corvo','emily']:
 rig=rig_for(role);m,src=L.load(role,'mixamo_sword_and_shield_attack');im,idle=L.load(role,'sword_idle');hand=descendants(rig,16);base=[];roots=[]
 floor=min(rig.fk(src[0])[j].translation.z for j in [21,25]);axis=rig.bind[16].translation.normalized()
 for f,s in enumerate(src):
  u=f/(len(src)-1);p=copy_pose(s);g=rig.fk(p);height=max(0,min(g[j].translation.z for j in [21,25])-floor);p[2].translation.z-=height
  for j in hand:p[j]=idle[0][j].copy()
  p[17]=idle[0][17].copy();g=rig.fk(p);effort=math.sin(math.pi*min(1,u/.8))**2
  # Offset is expressed in the moving chest frame, with the elbow out and
  # behind during the downward strike. It never targets the camera.
  chest=g[5].to_3x3()@rig.global_bind[5].to_3x3().inverted();goal=g[7].translation+chest@Vector((-.10-.12*effort,.34+.06*effort,-.35+.18*effort));pole=g[7].translation+chest@Vector((-.16,.35,-.2))
  rig.solve(p,7,8,9,goal,pole);preview_helpers(rig,p);base.append(p);root=qts(m['root_track'][f]);root.translation.z+=height;roots.append(row(root))
 q=read(P.parent/('fullbody-proof/inputs/client-target-sword.json'if role=='corvo'else'crouch-grip-reference/host-target-sword.json'))['quaternion'];binding=Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix().to_4x4()
 def adjust(p,deg):
  out=copy_pose(p);out[15]=p[15]@Quaternion(axis,math.radians(deg)).to_matrix().to_4x4();out[15].translation=p[15].translation;return out
 def score(deg):
  penalty=0;clearance=10
  for p in base[::3]:
   g=rig.fk(adjust(p,deg));weapon=g[17]@binding;head=g[12].translation+Vector((0,0,.08))
   for x in [.12,.25,.4,.55,.7]:
    d=(weapon@Vector((x,0,0))-head).length-.15;clearance=min(clearance,d);penalty+=max(0,.13-d)**2*40
  return dict(degrees=deg,score=penalty+.000015*deg*deg,head_clearance_m=clearance)
 options=[score(x)for x in range(-75,76,15)];best=min(options,key=lambda x:x['score']);frames=[]
 for p in base:p=adjust(p,best['degrees']);preview_helpers(rig,p);frames.append(p)
 # The donor begins and ends in a combat crouch. Connect both ends to
 # native ready so the review shows preparation, strike and recovery.
 ps=[];rs=[]
 for f in range(31):
  p=blend(idle[0],frames[0],ease(f/30));preview_helpers(rig,p);ps.append(p);rs.append(roots[0])
 ps.extend(frames);rs.extend(roots)
 for f in range(37):
  p=blend(frames[-1],sample(idle,f/36),ease(f/36));preview_helpers(rig,p);ps.append(p);rs.append(roots[-1])
 L.save(rig,role,'aerial_sword_attack_sequence',ps,rs,'combat','Authored one-handed aerial sword attack with complete compression, flight, strike, landing and ready recovery. Shield/support arm is adapted to free body-relative balance; native closed grip/socket are retained. Vertical travel is metadata, never an extra gameplay root write. Target contact, sword clearance through the full sweep, native air-attack timing and gameplay binding require review. This is not yet a paired drop assassination.',dict(donor_sha256=m['sha256'],ready_sha256=im['sha256'],forearm_screening=best),phase_marks=[dict(phase='donor_action',at=31/60),dict(phase='recovery',at=(31+len(frames))/60)])
 report.append(dict(role=role,screening=options,chosen=best,max_preview_height=max(r[6]for r in roots),visual_accepted=False));print('AERIAL_WEAPON',role,best,flush=True)
L.finish('aerial-attack-manifest-v1.json');(P/'aerial-attack-screening-v1.json').write_text(json.dumps(report,indent=2))
