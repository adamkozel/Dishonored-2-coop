"""Author sword-carrying sprint using the native armed shoulder/arm cycle.

An unarmed sprint's fist-to-face pump is inappropriate with a sword. Borrow
the complete native weapon arm, synchronized to the sprint's foot phase,
while retaining sprint pelvis, chest, legs and left-arm movement.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['dynamics-retarget-manifest.json'],'dynamics-armed-v3')
for role in ['corvo','emily']:
 rig=rig_for(role)
 b=(P/f'native-gaits/{role}-run_forward.bin').read_bytes();_,n,nf,fps,layout=struct.unpack_from('<8sIIfQ',b)
 native=[[qts(struct.unpack_from('<7f',b,28+(f*n+j)*28))for j in range(n)]for f in range(nf)]
 native_phase=max(range(nf-1),key=lambda f:rig.fk(native[f])[21].translation.x)/(nf-1)
 m,sprint=L.load(role,'sprint_forward')
 # The rendered sprint repeats a two-cycle donor twice = four strides.
 stride_time=m['stride_m']/m['source_cycles']/m['review_speed_mps']
 first_cycle=sprint[:round(stride_time*60)+1]
 sprint_phase=max(range(len(first_cycle)-1),key=lambda f:rig.fk(first_cycle[f])[21].translation.x)/(stride_time*60)
 offset=native_phase-sprint_phase
 for alias in ['sprint_forward','sprint_forward_left','sprint_forward_right']:
  m,frames=L.load(role,alias);result=[];maxfoot=0
  for f,pose in enumerate(frames):
   p=copy_pose(pose);arm=sample(native,f/60/stride_time+offset,True)
   # Local rotations keep the shoulder attached to the sprint torso. No
   # fixed hand target, IK anchor, translation change or root write exists.
   for i in [13,14,15,16,17]:p[i]=arm[i].copy()
   before=rig.fk(pose);after=rig.fk(p);maxfoot=max(maxfoot,max((before[i].translation-after[i].translation).length for i in [21,25]));preview_helpers(rig,p);result.append(p)
  assert maxfoot<1e-8
  L.save(rig,role,alias,result,m['root_track'],'locomotion','Armed sprint: authored sprint legs, pelvis, torso and free left-arm swing; native sword-running shoulder, elbow and wrist synchronized by left-foot phase. Native hand/socket and bone lengths retained. No fixed wrist target. Visual/contact review pending.',dict(sprint_sha256=m['sha256'],native_run_sha256=hashlib.sha256(b).hexdigest(),phase_offset=offset,method='whole weapon-arm local cycle aligned by left-ankle forward maximum'),stride_m=m['stride_m'],source_cycles=2,review_speed_mps=m['review_speed_mps'],foot_trajectory_error=maxfoot)
L.reports.extend(m for m in L.rows if not m['selection']['alias'].startswith('sprint_'))
L.finish('dynamics-retarget-manifest-v3.json')
