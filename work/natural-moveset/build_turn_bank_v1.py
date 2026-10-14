"""Native standing turns sampled by authored root yaw, with event-free poses.

World root stays identity. Root yaw supplies a monotone phase map, not another
actor transform, so the replicated facing and the footwork use one angle.
"""
import sys,struct,json,math,hashlib,bisect
from pathlib import Path
from mathutils import Quaternion
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from rig_math import read
OUT=P/'turn-banks-v1';OUT.mkdir(exist_ok=True)
rows=read(P/'library-retarget-manifest.json')+read(P/'turn-retarget-manifest-v1.json')
def blend(a,b,t):
 out=[]
 for x,y in zip(a,b):
  q=Quaternion((x[3],*x[:3])).slerp(Quaternion((y[3],*y[:3])),t).normalized()
  out.append([q.x,q.y,q.z,q.w,*[x[k]+(y[k]-x[k])*t for k in range(4,7)]])
 return out
reports=[]
for role in ('corvo','emily'):
 data=(P/'terrain-banks-v2'/f'{role}-terrain-bank.bin').read_bytes()
 magic,v,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
 assert magic==b'D2MOVE17' and count==149
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE18',18,n,161,fps,layout)+data[32:]);sources=[]
 for equipment in ('sword','empty'):
  for degrees in (45,90,180):
   for side in ('left','right'):
    alias=f'{equipment}_turn{side}{degrees}'
    row=next(r for r in rows if r['role']==role and alias in r['source']['aliases'])
    raw=(P/row['binary']).read_bytes();assert hashlib.sha256(raw).hexdigest()==row['sha256']
    tag,joints,frames,rate,source_layout=struct.unpack_from('<8sIIfQ',raw)
    assert tag==b'D2NATV01' and joints==n and source_layout==layout and row['native_length_error']<.0001
    fs=[[list(struct.unpack_from('<7f',raw,28+(f*n+j)*28))for j in range(n)]for f in range(frames)]
    yaw=[math.atan2(2*(q[3]*q[2]+q[0]*q[1]),1-2*(q[1]**2+q[2]**2))for q in row['root_track']]
    sign=1 if side=='left' else -1;arc=[0.]
    for a,b in zip(yaw,yaw[1:]):arc.append(arc[-1]+sign*math.remainder(b-a,2*math.pi))
    assert abs(arc[-1]-math.radians(degrees))<.003,(alias,arc[-1])
    reversal=max(a-b for a,b in zip(arc,arc[1:]))
    assert reversal<.01,(alias,reversal)
    for i in range(1,len(arc)):arc[i]=max(arc[i],arc[i-1])
    total=arc[-1];poses=[]
    for f in range(120):
     amount=total*f/119
     b=min(max(1,bisect.bisect_left(arc,amount)),frames-1);a=b-1
     t=(amount-arc[a])/max(1e-8,arc[b]-arc[a]);t=max(0,min(1,t))
     poses.append(blend(fs[a],fs[b],t))
    poses.append(poses[0]);cid=149+len(sources)
    bank.extend(struct.pack('<IIIIff',cid,121,0,0,math.radians(degrees),row['duration']))
    for pose in poses:
     for joint in pose:bank.extend(struct.pack('<7f',*joint))
    sources.append(dict(clip=cid,alias=alias,source=row['binary'],sha256=row['sha256'],
     arc_radians=total,yaw_map=arc,max_reverse_radians=reversal,source_duration=row['duration'],
     source_root_translation=row['root_track'][-1][4:7],root_translation_applied=False))
 out=OUT/f'{role}-turn-bank.bin';out.write_bytes(bank)
 reports.append(dict(role=role,bank=str(out.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),
  format='D2MOVE18',joints=n,clips=161,previous_payload_unchanged=bank[32:32+149*(24+121*n*28)]==data[32:],
  sources=sources,events=False,root_writes=False,gameplay_verified=False,visual_accepted=False,
  limitations=['Standing turns only; crouched turn binding remains separate.','Native turn source translation is omitted to preserve replicated position; in-place foot drift needs actual visual review.']))
(P/'turn-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('TURN_BANKS: 161 clips per role; source-root-yaw phase; no world root or events')
