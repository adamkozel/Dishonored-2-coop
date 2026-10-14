"""Shared offline clip authoring helpers; never writes to the game or actor root."""
import json,math,struct,hashlib
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
from rig_math import read,qts,NativeRig,preview_helpers
P=Path(__file__).resolve().parent
def copy_pose(p):return [m.copy()for m in p]
def blend(a,b,t):
 out=[]
 for x,y in zip(a,b):
  m=x.to_quaternion().slerp(y.to_quaternion(),t).normalized().to_matrix().to_4x4();m.translation=x.translation.lerp(y.translation,t);out.append(m)
 return out
def sample(frames,u,loop=False):
 u=u%1 if loop else max(0,min(1,u));f=u*(len(frames)-1);a=int(f);return blend(frames[a],frames[min(a+1,len(frames)-1)],f-a)
def row(m):
 q=m.to_quaternion().normalized();return [q.x,q.y,q.z,q.w,*m.translation]
def identity(x=0,y=0,z=0):return [0,0,0,1,x,y,z]
class Library:
 def __init__(self,manifests,out):
  self.rows=[r for name in manifests for r in read(P/name)];self.out=P/out;self.out.mkdir(exist_ok=True);self.reports=[]
 def load(self,role,alias):
  m=next(m for m in self.rows if m['role']==role and m['selection']['alias']==alias);b=(P/m['binary']).read_bytes();n=m['joints']
  assert hashlib.sha256(b).hexdigest()==m['sha256']
  return m,[[qts(struct.unpack_from('<7f',b,28+(f*n+j)*28))for j in range(n)]for f in range(m['frames'])]
 def save(self,rig,role,alias,poses,roots,family,purpose,sources,**extras):
  n=len(rig.joints);layout=int.from_bytes(hashlib.sha256('\n'.join(f"{j['nativeName']}:{j['parent']}"for j in rig.joints).encode()).digest()[:8],'little');key=alias+'-'+role
  assert len(poses)==len(roots) and 2<=len(poses)<=1801
  payload=bytearray(struct.pack('<8sIIfQ',b'D2NATV01',n,len(poses),60,layout));length_error=0
  for f in poses:
   assert max(abs(f[i][a][b]-float(a==b))for i in [0,1]for a in range(4)for b in range(4))<1e-6
   length_error=max(length_error,max((f[i].translation-rig.bind[i].translation).length for i in range(3,27)if i!=17))
   for m in f:payload.extend(struct.pack('<7f',*row(m)))
  assert length_error<1e-5, ('Anatomical translation changed',length_error)
  path=self.out/(key+'.bin');path.write_bytes(payload)
  result=dict(id=key,role=role,binary=str(path.relative_to(P)),sha256=hashlib.sha256(payload).hexdigest(),frames=len(poses),fps=60,duration=(len(poses)-1)/60,joints=n,layout_hash=layout,root_track=roots,root_track_semantics='Offline demonstration only; replicated gameplay owns world position',selection=dict(alias=alias,family=family,purpose=purpose),source=sources,equipment_policy='sword_ready',first_person_constraints=False,events=False,root_writes=False,native_length_error=length_error,visual_accepted=False,runtime_verified=False,**extras)
  path.with_suffix('.json').write_text(json.dumps(result,indent=2));self.reports.append(result);print('AUTHORED',alias,role,len(poses),flush=True)
 def finish(self,name):(P/name).write_text(json.dumps(self.reports,indent=2))
def rig_for(role):return NativeRig(read(P.parent/f'mixamo-crouch/native-{role}.json')['joints'])
def pin_feet(rig,pose,targets,rotations=None):
 g=rig.fk(pose)
 for k,(a,b,c)in enumerate([(19,20,21),(23,24,25)]):
  rotation=rotations[k]if rotations else g[c].to_3x3().copy();rig.solve(pose,a,b,c,targets[k],g[b].translation);rig.orient(pose,c,rotation)
 preview_helpers(rig,pose);return pose
