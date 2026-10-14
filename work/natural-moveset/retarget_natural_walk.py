"""Natural two-rig full-body walking revision. Run in Blender.

Generated donor movement is metadata only. The bank contains local poses,
native layout identity and cycle distance; never root placement or events.
"""
import bpy,json,math,re,struct,hashlib,bisect
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
P=Path(__file__).resolve().parent
OUT=P/'retargeted';OUT.mkdir(exist_ok=True)
PROOF=P.parent/'fullbody-proof'
import sys
sys.path.insert(0,str(P))
from rig_math import read,qts,ease,NativeRig,header_floats
mapping={2:'pelvis',3:'spine_01',4:'spine_03',5:'spine_05',6:'clavicle_l',7:'upperarm_l',8:'lowerarm_l',9:'hand_l',11:'neck_01',12:'head',13:'clavicle_r',14:'upperarm_r',15:'lowerarm_r',16:'hand_r',19:'thigh_l',20:'calf_l',21:'foot_l',22:'ball_l',23:'thigh_r',24:'calf_r',25:'foot_r',26:'ball_r'}
C=Matrix(((0,1,0,0),(1,0,0,0),(0,0,1,0),(0,0,0,1)))
def ue(v):
 m=C@Matrix.LocRotScale(Vector(v[:3]),Quaternion((v[6],*v[3:6])).normalized(),Vector(v[7:10]))@C;m.translation*=.01;return m
mesh=read(P.parent/'animgen-feasibility/exports/uefn-native-mesh.json')
analysis={v['name']:v for v in read(P.parent/'moveset-growth/donor-analysis.json')}
names=['idle','walk_forward','walk_forward_left','walk_left','walk_back_left','walk_back','walk_back_right','walk_right','walk_forward_right']
def sfk(d,values):
 g=[]
 for i,v in enumerate(values):
  m=ue(v);p=d['parents'][i];g.append(m if p<0 else g[p]@m)
 return g
def row(local):
 out=[]
 for m in local:
  q=m.to_quaternion().normalized();out.append([q.x,q.y,q.z,q.w,*m.translation])
 return out
def blend(a,b,t):
 out=[]
 for x,y in zip(a,b):
  q=Quaternion((x[3],*x[:3])).slerp(Quaternion((y[3],*y[:3])),t)
  out.append([q.x,q.y,q.z,q.w,*[x[k]+(y[k]-x[k])*t for k in range(4,7)]])
 return out
def fast_helpers(local):
 # Same preview approximation as preserved proof; one ordered hierarchy pass.
 g=[]
 for i,j in enumerate(J):
  p=j['parent'];n=j['nativeName'];driver=None
  side='left' if n.startswith('left') else 'right' if n.startswith('right') else ''
  if side and 'helper' in n:
   if 'upper_leg' in n or 'thigh' in n:driver=side+'_femur_jnt'
   elif 'knee' in n:driver=side+'_tibia_jnt'
   elif 'arm01' in n and 'forearm' not in n:driver=side+'_arm_jnt'
   elif 'elbow' in n or 'forearm' in n:driver=side+'_elbow_jnt'
   elif 'wrist' in n:driver=side+'_wrist_jnt'
   elif 'ankle' in n:driver=side+'_ankle_jnt'
  if driver:
   k=joint_index[driver]
   assert k<i
   local[i]=g[p].inverted()@(g[k]@BG[k].inverted()@BG[i])
  g.append(local[i] if p<0 else g[p]@local[i])
 return g
manifests=[]
for role in ('corvo','emily'):
 J=read(P.parent/f'mixamo-crouch/native-{role}.json')['joints'];B=[qts(j['bindLocalQTS']) for j in J];rig=NativeRig(J);fk=rig.fk;solve=rig.solve;orient=rig.orient;BG=fk(B)
 joint_index={j['nativeName']:i for i,j in enumerate(J)}
 gripText=(P.parent.parent/f'outputs/d2-coop-lab/src/mixamo_{role}_grip.h').read_text()
 floats=lambda name:header_floats(gripText,name)
 prefix='kMixamo' if role=='corvo' else 'kMixamoEmily'
 fingerIds=[int(v) for v in floats(prefix+'GripJoints')];fingerQ=floats(prefix+'GripRotation')
 socketQ=floats('kMixamo'+role.title()+'StandingSocket');socketPos=floats('kMixamo'+role.title()+'StandingSocketPosition');socketRot=Quaternion((socketQ[3],*socketQ[:3])).to_matrix()
 bind=read(PROOF/'inputs/client-target-sword.json' if role=='corvo' else P.parent/'crouch-grip-reference/host-target-sword.json')
 q=bind['quaternion'];weaponBinding=Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix()
 layout='\n'.join(f"{j['nativeName']}:{j['parent']}" for j in J);layoutHash=int.from_bytes(hashlib.sha256(layout.encode()).digest()[:8],'little')
 payload=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE01',1,len(J),len(names),60,layoutHash))
 manifest={'role':role,'layout_hash':layoutHash,'joints':len(J),'clips':[],'claims':'Walking bank candidate; native D2 roundtrip and visual acceptance required','first_person_changed':False,'events':False,'root_writes':False,'weapon_reference':'Native local grip/socket only; full-body source drives both arms', 'wrist_constraint':'none', 'source_first_person_constraints':False}
 for cid,name in enumerate(names):
  d=read(P.parent/'moveset-growth/donors'/f'{name}.json');sr=sfk(d,[b['local'] for b in mesh['bones']]);indices={i:d['names'].index(n) for i,n in mapping.items()};rest={i:sr[k] for i,k in indices.items()}
  corr={i:Matrix.Identity(3) for i in mapping}
  for i,ch in {7:8,8:9,14:15,15:16}.items():corr[i]=(BG[ch].translation-BG[i].translation).normalized().rotation_difference((rest[ch].translation-rest[i].translation).normalized()).to_matrix()
  scale=(BG[19].translation-BG[21].translation).length/(rest[19].translation-rest[21].translation).length
  cut=analysis[name]['loop_candidate'] if cid else {'start':180,'end':300}
  start,end=cut['start'],cut['end'];angle=math.radians(d['request']['angle']);direction=Vector((math.cos(angle),math.sin(angle),0))
  distances=[0.]
  for f in d['frames']:
   delta=ue(f['root_delta']).translation*scale
   distances.append(distances[-1]+max(0,delta.dot(direction)) if cid else 0.)
  stride=distances[end]-distances[start] if cid else 0.
  assert not cid or .2<stride<3
  locks=[None,None];frames=[];floor=min(BG[22].translation.z,BG[26].translation.z);length_error=0
  for index,f in enumerate(d['frames'][:end+1]):
   sg=sfk(d,f['local']);local=[m.copy() for m in B];g=[]
   for i,j in enumerate(J):
    p=j['parent']
    if i in mapping:
     target=sg[indices[i]].to_quaternion().to_matrix()@rest[i].to_quaternion().to_matrix().inverted()@corr[i]@BG[i].to_3x3()
     rot=g[p].to_3x3().inverted()@target if p>=0 else target
     local[i]=rot.to_quaternion().normalized().to_matrix().to_4x4();local[i].translation=B[i].translation
     if i==2:local[i].translation+=(sg[indices[i]].translation-rest[i].translation)*scale
    g.append(local[i] if p<0 else g[p]@local[i])
   local[2].translation.z+=floor-min(g[22].translation.z,g[26].translation.z)
   position=direction*distances[index]
   for side,(a,b,c) in enumerate(((19,20,21),(23,24,25))):
    g=fk(local);world=position+g[c].translation;confidence=f['contacts'][side]
    if confidence>=.85 and locks[side] is None:locks[side]=world.copy()
    if confidence<.15:locks[side]=None
    if locks[side] is not None:
     goal=world.lerp(locks[side],ease((confidence-.15)/.70))-position;rot=g[c].to_3x3()
     solve(local,a,b,c,goal,Vector((.8,goal.y,.5)));orient(local,c,rot)
   # The entire source shoulder/arm/wrist motion survives retargeting.
   # Native fingers and attachment follow the moving wrist without arm IK.
   for k,i in enumerate(fingerIds):
    if i==17:continue
    q=fingerQ[k*4:k*4+4];local[i]=Quaternion((q[3],*q[:3])).to_matrix().to_4x4();local[i].translation=B[i].translation
   local[17]=socketRot.to_4x4();local[17].translation=Vector(socketPos)
   fast_helpers(local)
   length_error=max(length_error,max((local[i].translation-B[i].translation).length for i in range(3,27) if i!=17))
   frames.append(row(local))
  # Uniform travel phase for walking, time for idle. Close the small cycle
  # residual gradually; no held endpoint and no time-based root movement.
  resampled=[]
  for f in range(121):
   u=f/120
   if cid:
    dist=distances[start]+stride*u;k=max(start,min(end-1,bisect.bisect_right(distances,dist)-1));t=(dist-distances[k])/max(1e-9,distances[k+1]-distances[k])
   else:k=min(end-1,start+int((end-start)*u));t=(end-start)*u-(k-start)
   resampled.append(blend(frames[k],frames[k+1],max(0,min(1,t))))
  mismatch=0
  for j in range(len(J)):
   a,b=resampled[0][j],resampled[-1][j];qa=Quaternion((a[3],*a[:3]));qb=Quaternion((b[3],*b[:3]));delta=qb.inverted()@qa
   residual=math.degrees(qa.rotation_difference(qb).angle)
   mismatch=max(mismatch,min(residual,360-residual))
   for f,pose in enumerate(resampled):
    u=ease(f/120);v=pose[j];q=Quaternion((v[3],*v[:3]))@Quaternion().slerp(delta,u);q.normalize();v[:4]=[q.x,q.y,q.z,q.w]
    for k in range(4,7):v[k]+=(a[k]-b[k])*u
  resampled[-1]=[v[:] for v in resampled[0]]
  payload.extend(struct.pack('<IIIIff',cid,121,0,0,stride,angle))
  for pose in resampled:
   for v in pose:payload.extend(struct.pack('<7f',*v))
  data={'role':role,'name':name,'frames':resampled,'stride':stride,'angle':angle,'joints':[j['nativeName'] for j in J],'parents':[j['parent'] for j in J],'weapon_binding':list(bind['quaternion']),'cut':cut,'native_length_error':length_error,'source_scale':scale,'max_loop_correction_degrees':mismatch}
  (OUT/f'{role}-{name}.json').write_text(json.dumps(data,separators=(',',':')))
  manifest['clips'].append({k:v for k,v in data.items() if k not in ('frames','joints','parents','weapon_binding')})
  print('RETARGETED',role,name,round(stride,3),round(mismatch,2),flush=True)
 bank=OUT/f'{role}-walk-bank.bin';bank.write_bytes(payload);manifest['sha256']=hashlib.sha256(payload).hexdigest();manifest['bytes']=len(payload)
 (OUT/f'{role}-manifest.json').write_text(json.dumps(manifest,indent=2));manifests.append(manifest)
(P/'retarget-manifest.json').write_text(json.dumps(manifests,indent=2))
print('TWO_NATIVE_WALK_BANKS_WRITTEN',flush=True)
