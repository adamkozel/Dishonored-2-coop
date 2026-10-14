"""Full-body authored crouch transitions with foot contact and a free weapon wrist.

The new clips are review candidates. No first-person wrist positions, actor-root
writes, animation events or autonomous actions are included.
"""
import json,math,struct,hashlib,sys,statistics
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from rig_math import read,qts,NativeRig,ease,preview_helpers
O=P/'stance-authored';O.mkdir(exist_ok=True)
library=read(P/'library-retarget-manifest.json')+read(P/'mixamo-retarget-manifest-v5.json');reports=[]
def rows(ms):
 return [[q.x,q.y,q.z,q.w,*m.translation]for m in ms for q in [m.to_quaternion().normalized()]]
def blend(a,b,t):
 out=[]
 for x,y in zip(a,b):
  q=x.to_quaternion().slerp(y.to_quaternion(),t).normalized();m=q.to_matrix().to_4x4();m.translation=x.translation.lerp(y.translation,t);out.append(m)
 return out
def load(role,alias):
 m=next(m for m in library if m['role']==role and m['selection']['alias']==alias);b=(P/m['binary']).read_bytes();n=m['joints'];return m,[[qts(struct.unpack_from('<7f',b,28+(f*n+j)*28))for j in range(n)]for f in range(m['frames'])]
def sample(fs,u,loop=False):
 u=u%1 if loop else max(0,min(1,u));f=u*(len(fs)-1);i=int(f);return blend(fs[i],fs[min(i+1,len(fs)-1)],f-i)
def save(role,label,poses,roots,notes,extra=None):
 n=len(poses[0]);layout='\n'.join(f"{j['nativeName']}:{j['parent']}"for j in rig.joints);layout=int.from_bytes(hashlib.sha256(layout.encode()).digest()[:8],'little');key=label+'-'+role
 payload=bytearray(struct.pack('<8sIIfQ',b'D2NATV01',n,len(poses),60,layout))
 for f in poses:
  for v in rows(f):payload.extend(struct.pack('<7f',*v))
 path=O/(key+'.bin');path.write_bytes(payload)
 m=dict(id=key,role=role,binary=str(path.relative_to(P)),sha256=hashlib.sha256(payload).hexdigest(),frames=len(poses),fps=60,duration=(len(poses)-1)/60,joints=n,layout_hash=layout,root_track=roots,root_track_semantics='Demonstration trajectory only; production position must remain replicated player state',selection=dict(alias=label,family='stance',purpose=notes),source=dict(manifest='mixamo-retarget-manifest-v5.json',posture=100,corvo_arm_space=70,emily_arm_space=50),equipment_policy='sword_ready',first_person_constraints=False,events=False,root_writes=False,visual_accepted=False,runtime_verified=False)
 if extra:m.update(extra)
 (O/(key+'.json')).write_text(json.dumps(m,indent=2));reports.append(m)
def identity(x=0):return [0,0,0,1,x,0,0]
for role in ['corvo','emily']:
 rig=NativeRig(read(P.parent/f'mixamo-crouch/native-{role}.json')['joints']);J=rig.joints
 standmeta,stand=load(role,'sword_idle');wm,walk=load(role,'mixamo_crouched_walking_posture100');im,idle=load(role,'mixamo_crouching_idle');dm,down=load(role,'mixamo_standing_to_crouched');um,up=load(role,'mixamo_crouched_to_standing')
 # The grip is entirely hand-local. It follows the source wrist, unlike the
 # old camera-relative wrist lock. Finger/socket transforms come from v5.
 grip_indices=[]
 for i in range(len(J)):
  parent=i
  while parent>=0 and parent!=16:parent=J[parent]['parent']
  if parent==16 and i!=16 and 'helper'not in J[i]['nativeName']:grip_indices.append(i)
 def grip(pose):
  for i in grip_indices:pose[i]=walk[0][i].copy()
  return pose
 anchor=rig.fk(stand[0]);feet=[anchor[i].translation.copy()for i in [21,25]]
 def pin(pose,targets,weight=1):
  g=rig.fk(pose)
  for side,(a,b,c)in enumerate([(19,20,21),(23,24,25)]):
   rotation=g[c].to_3x3().copy();goal=g[c].translation.lerp(targets[side],weight);rig.solve(pose,a,b,c,goal,g[b].translation);rig.orient(pose,c,rotation)
  preview_helpers(rig,pose);return pose
 # Keep the requested walking posture present through the idle, while
 # retaining the authored low-idle breathing and balance movement.
 wg=[rig.fk(p)for p in walk]
 phase=min(range(len(walk)-1),key=lambda f:sum(wg[f][i].translation.z for i in [21,25]))
 idleposes=[]
 for f in range(151):
  p=blend(sample(idle,f/150,True),walk[phase],.55);grip(p);pin(p,feet);idleposes.append(p)
 idleposes[-1]=[m.copy()for m in idleposes[0]]
 def transition(donor,start,end):
  poses=[];frames=49
  for f in range(frames):
   u=f/(frames-1);p=sample(donor,u)
   # Endpoint inertial-style correction across the whole body. The source
   # supplies the motion between matched standing and crouching stances.
   for i in range(2,len(J)):
    q=p[i].to_quaternion();qs=(start[i].to_quaternion()@donor[0][i].to_quaternion().inverted()).normalized();qe=(end[i].to_quaternion()@donor[-1][i].to_quaternion().inverted()).normalized()
    correction=qs.slerp(qe,ease(u));m=(correction@q).normalized().to_matrix().to_4x4();m.translation=p[i].translation+(start[i].translation-donor[0][i].translation)*(1-ease(u))+(end[i].translation-donor[-1][i].translation)*ease(u);p[i]=m
   pin(p,feet);poses.append(p)
  poses[0]=[m.copy()for m in start];poses[-1]=[m.copy()for m in end];return poses
 crouch=idleposes[0];downposes=transition(down,stand[0],crouch);upposes=transition(up,crouch,stand[0])
 # Fit stride to the actual retargeted planted-ankle travel. This replaces
 # the old hand-tuned 2.04m number, which belongs to the previous retarget.
 samples=[]
 for foot in [21,25]:
  low=min(g[foot].translation.z for g in wg)
  for f in range(1,len(walk)-1):
   if max(wg[f-1][foot].translation.z,wg[f+1][foot].translation.z)<low+.035:
    s=-(wg[f+1][foot].translation.x-wg[f-1][foot].translation.x)*.5*(len(walk)-1)
    if .6<s<2.5:samples.append(s)
 stride=statistics.median(samples);assert 1<stride<1.8
 speed=.9;duration=stride/speed*2;walking=[];walkroots=[]
 for f in range(round(duration*60)+1):
  t=f/60;p=sample(walk,t*speed/stride,True);preview_helpers(rig,p);walking.append(p);walkroots.append(identity(t*speed))
 save(role,'crouch_idle',idleposes,[identity()for _ in idleposes],'Whole-body authored idle blended toward Posture-100 stance, with native closed grip and foot contact; visual review pending.')
 save(role,'stand_to_crouch',downposes,[identity()for _ in downposes],'Authored whole-body crouch entry, matched stance endpoints and planted feet. Wrist remains free; grip changes with the pose.')
 save(role,'crouch_to_stand',upposes,[identity()for _ in upposes],'Authored whole-body standing recovery matched to native armed idle; foot contact maintained.')
 save(role,'crouch_forward',walking,walkroots,'Posture-100 full-body crouch walking at 0.9m/s, cadence fitted from planted-ankle travel. Native closed reverse grip follows the authored wrist.',dict(stride_m=stride,stride_contact_samples=len(samples),review_speed_mps=speed))
 sequence=[];root=[];x=0
 def append(fs,rs=None,bridge=0):
  global x
  if bridge and sequence:
   previous=sequence[-1]
   for k in range(1,bridge+1):sequence.append(blend(previous,fs[0],ease(k/bridge)));root.append(identity(x))
  for i,p in enumerate(fs):sequence.append(p);root.append(identity(x+(rs[i][4]if rs else 0)))
  if rs:x+=rs[-1][4]
 append([sample(stand,f/120,True)for f in range(61)])
 append(downposes);append(idleposes[:61]);append(walking,walkroots,18);append(idleposes[:61],bridge=18);append(upposes);append([sample(stand,f/120,True)for f in range(61)])
 save(role,'crouch_sequence',sequence,root,'Connected review sequence: armed idle, authored crouch entry, crouch idle, forward travel, stop and authored standing recovery. Start/stop foot transfer and body clearance remain visual review items.',dict(stride_m=stride))
 print('STANCE_AUTHORED',role,'stride',round(stride,4),'sequence frames',len(sequence))
(P/'stance-retarget-manifest.json').write_text(json.dumps(reports,indent=2))
