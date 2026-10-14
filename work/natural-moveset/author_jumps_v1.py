"""Complete airborne studies. Source motion stays natural; root is metadata only."""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['mixamo-jump-retarget-manifest.json','library-retarget-manifest.json','stance-retarget-manifest.json'],'jumps-authored-v1')
for role in ['corvo','emily']:
 rig=rig_for(role);J=rig.joints;m,donor=L.load(role,'mixamo_standing_jump');sm,stand=L.load(role,'sword_idle');cm,crouch=L.load(role,'crouch_idle');hand=[]
 for i in range(len(J)):
  parent=i
  while parent>=0 and parent!=16:parent=J[parent]['parent']
  if parent==16 and i!=16 and 'helper'not in J[i]['nativeName']:hand.append(i)
 def grip(p):
  for i in hand:p[i]=stand[0][i].copy()
  preview_helpers(rig,p);return p
 floor=min(rig.fk(donor[0])[i].translation.z for i in [21,25]);heights=[max(0,min(rig.fk(p)[i].translation.z for i in [21,25])-floor)for p in donor]
 airborne=[i for i,h in enumerate(heights)if h>.06];assert airborne
 takeoff=airborne[0];land=airborne[-1]+1;apex=max(range(len(heights)),key=lambda i:heights[i]);compression=min(range(takeoff),key=lambda i:donor[i][2].translation.z)
 frames=[];roots=[]
 for i,original in enumerate(donor):
  p=grip(copy_pose(original));p[2].translation.z-=heights[i];preview_helpers(rig,p);frames.append(p)
  roots.append(identity(m['root_track'][i][4],m['root_track'][i][5],heights[i]))
 source=dict(donor_sha256=m['sha256'],native_grip_sha256=sm['sha256'],source_fps=m['fps'],takeoff_frame=takeoff,apex_frame=apex,landing_frame=land)
 L.save(rig,role,'standing_jump_sequence',frames,roots,'traversal','Complete authored standing takeoff, airborne body motion, landing and recovery. Native hand-local closed sword grip; world height is extracted from ankle clearance into offline trajectory metadata. Capsule, collision and state timing still need game binding.',source,phase_marks=[dict(phase=n,at=i/60)for n,i in [('compression',compression),('takeoff',takeoff),('apex',apex),('landing',land)]])
 # Start from the actual common crouch, transfer the entire body into the
 # donor compression, and retain its authored flight/landing mechanics.
 for finish in ['stand','crouch']:
  ps=[];rs=[];feet=[rig.fk(crouch[0])[i].translation.copy()for i in [21,25]]
  for f in range(19):
   u=f/18;p=blend(crouch[0],frames[compression],ease(u));pin_feet(rig,p,feet);ps.append(p);rs.append(identity())
  last=len(frames)-1 if finish=='stand'else min(range(land,len(frames)),key=lambda i:frames[i][2].translation.z)
  for i in range(compression+1,last+1):
   p=copy_pose(frames[i]);w=1-ease((i-compression)/max(1,takeoff+6-compression))
   if w>0:
    fk=rig.fk(p);pin_feet(rig,p,[fk[j].translation.lerp(feet[k],w)for k,j in enumerate([21,25])])
   ps.append(p);rs.append(roots[i])
  if finish=='crouch':
   beginning=copy_pose(ps[-1]);startfeet=[rig.fk(beginning)[i].translation.copy()for i in [21,25]]
   for f in range(1,31):
    u=f/30;p=blend(beginning,crouch[0],ease(u));pin_feet(rig,p,[a.lerp(b,ease(u))for a,b in zip(startfeet,feet)]);ps.append(p);rs.append(identity())
  L.save(rig,role,'crouch_jump_to_'+finish,ps,rs,'traversal','Whole-body common-crouch entry into authored jump and '+finish+' recovery. No stationary first-person wrist targets. Foot transfer, sword grip reversal during stance changes, collision and actual capsule height require review.',dict(**source,crouch_sha256=cm['sha256']),phase_marks=[dict(phase='takeoff',at=(18+takeoff-compression)/60),dict(phase='landing',at=(18+land-compression)/60)])
 # A deliberately authored supernatural second impulse: tuck/extend the
 # legs with counterbalancing torso motion. No ground push is replayed.
 ps=[copy_pose(p)for p in frames[:apex+1]];rs=roots[:apex+1].copy();base=copy_pose(frames[apex]);duration=.8;extra_height=.78
 for f in range(1,49):
  u=f/48;p=copy_pose(base);tuck=math.sin(math.pi*u)**2
  for j,angle in [(19,-23),(23,-23),(20,40),(24,40),(3,5),(4,4),(5,3)]:
   rig.orient(p,j,Matrix.Rotation(math.radians(angle*tuck),3,'Y')@rig.fk(p)[j].to_3x3())
  for j,angle in [(7,-8),(14,8)]:rig.orient(p,j,Matrix.Rotation(math.radians(angle*tuck),3,'Z')@rig.fk(p)[j].to_3x3())
  preview_helpers(rig,p);ps.append(p);r=roots[apex].copy();r[6]+=4*extra_height*u*(1-u);rs.append(r)
 ps.extend(copy_pose(p)for p in frames[apex+1:]);rs.extend(roots[apex+1:])
 L.save(rig,role,'double_jump',ps,rs,'traversal','Complete standing jump with a newly authored midair second-impulse tuck and extension, then the donor landing. The second impulse is a style candidate, not native D2 physics calibration. Offline ballistic example only; actual ability, height, collision and sword clearance remain unverified.',dict(**source,second_impulse='48 authored frames with local thigh/knee/spine/arm counterbalance'),second_impulse=dict(at=apex/60,duration=duration,example_extra_height_m=extra_height))
 print('JUMP_PHASES',role,'compression',compression,'takeoff',takeoff,'apex',apex,'landing',land,'height',max(heights),flush=True)
L.finish('jumps-retarget-manifest-v1.json')
