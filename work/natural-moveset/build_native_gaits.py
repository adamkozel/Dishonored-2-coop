"""Two authored native gaits, distance-clocked and aligned by foot-contact phase."""
import sys,json,struct,math,hashlib
from pathlib import Path
from mathutils import Quaternion,Vector
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from rig_math import read,qts,NativeRig,ease
OUT=P/'native-gaits';OUT.mkdir(exist_ok=True)
library=read(P/'library-retarget-manifest.json')
def blend(a,b,t):
 out=[]
 for x,y in zip(a,b):
  q=Quaternion((x[3],*x[:3])).slerp(Quaternion((y[3],*y[:3])),t);q.normalize()
  out.append([q.x,q.y,q.z,q.w,*[x[k]+(y[k]-x[k])*t for k in range(4,7)]])
 return out
def sample(frames,u):
 f=(u%1)*(len(frames)-1);a=int(f);return blend(frames[a],frames[min(a+1,len(frames)-1)],f-a)
def load(alias,role):
 m=next(m for m in library if m['role']==role and m['selection']['alias']==alias);b=(P/m['binary']).read_bytes();n=m['joints']
 fs=[[list(struct.unpack_from('<7f',b,28+(f*n+j)*28)) for j in range(n)]for f in range(m['frames'])]
 delta=Vector(m['root_track'][-1][4:7])-Vector(m['root_track'][0][4:7]);return m,fs,delta
reports=[]
for role in ['corvo','emily']:
 rig=NativeRig(read(P.parent/f'mixamo-crouch/native-{role}.json')['joints']);n=len(rig.joints);clips=[];sources=[]
 m,fs,_=load('sword_idle',role);idle=[sample(fs,f/120)for f in range(121)];idle[-1]=idle[0]
 clips.append(dict(name='armed_idle',poses=idle,stride=0,angle=0));layout=m['layout_hash']
 reference=None
 for gait in ['walk','run']:
  cards=[];deltas=[]
  for direction in 'nwse':
   m,fs,delta=load(f'sword_{gait}_{direction}_add',role)
   assert m['root_track_semantics'].startswith('origin joint 0 only')
   poses=[sample(fs,f/120)for f in range(120)]
   heights=[]
   for pose in poses:
    g=rig.fk([qts(v) for v in pose]);heights.append((g[21].translation.z,g[25].translation.z))
   if reference is None:reference=heights
   offset=min(range(120),key=lambda k:sum(sum((heights[(f+k)%120][j]-reference[f][j])**2 for j in range(2)) for f in range(120)))
   poses=poses[offset:]+poses[:offset];poses.append(poses[0]);cards.append(poses);deltas.append(delta)
   sources.append(dict(alias=m['selection']['alias'],sha256=m['sha256'],phase_offset=offset/120,stride=delta.length,seconds=m['duration'],native_speed=delta.length/m['duration']))
  for d in range(8):
   a=d//2;b=(a+1)%4;t=.5 if d%2 else 0;poses=[blend(x,y,t)for x,y in zip(cards[a],cards[b])];poses[-1]=poses[0]
   delta=deltas[a].lerp(deltas[b],t);stride=delta.length
   assert .2<stride<(6 if gait=='run' else 3)
   clips.append(dict(name=gait+'_'+['forward','forward_left','left','back_left','back','back_right','right','forward_right'][d],poses=poses,stride=stride,angle=d*math.pi/4))
 payload=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE02',2,n,17,60,layout));max_seam=0
 for cid,c in enumerate(clips):
  payload.extend(struct.pack('<IIIIff',cid,121,0,0,c['stride'],c['angle']))
  for pose in c['poses']:
   for v in pose:payload.extend(struct.pack('<7f',*v))
  meta={k:v for k,v in c.items() if k!='poses'}
  # Also export review clips with one real-time source cycle for labelled review.
  review=bytearray(struct.pack('<8sIIfQ',b'D2NATV01',n,121,60,layout))
  for pose in c['poses']:
   for v in pose:review.extend(struct.pack('<7f',*v))
  (OUT/f'{role}-{c["name"]}.bin').write_bytes(review)
 bank=OUT/f'{role}-gait-bank.bin';bank.write_bytes(payload)
 report=dict(role=role,bank=str(bank.relative_to(P)),sha256=hashlib.sha256(payload).hexdigest(),sources=sources,clips=[{k:v for k,v in c.items() if k!='poses'}for c in clips],gaits=['walk','run'],phase='Travel distance divided by blended native stride; no elapsed-time gait advancement',diagonals='Whole-body pose and displacement blends of native cardinal motions',root_writes=False,events=False,first_person_constraints=False,visual_accepted=False)
 (OUT/f'{role}-manifest.json').write_text(json.dumps(report,indent=2));reports.append(report)
(P/'native-gait-manifest.json').write_text(json.dumps(reports,indent=2));print('NATIVE_GAITS_WRITTEN',len(reports),'roles,17 clips each')
