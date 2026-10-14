"""Version-5 candidate: native-amplitude lean paths appended to the gait bank.

Lean entries are parameter curves, not time loops. Frames 0..119 represent
normalized lean magnitude; frame 120 closes the storage record but is never
used by parameter sampling. The authored paths preserve both support feet.
"""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from rig_math import read
from mathutils import Quaternion
OUT=P/'lean-banks-v1';OUT.mkdir(exist_ok=True)
def blend(a,b,t):
    out=[]
    for x,y in zip(a,b):
        q=Quaternion((x[3],*x[:3])).slerp(Quaternion((y[3],*y[:3])),t).normalized()
        out.append([q.x,q.y,q.z,q.w,*[x[k]+(y[k]-x[k])*t for k in range(4,7)]])
    return out
def inverse_ease(w):
    lo,hi=0.,1.
    for _ in range(30):
        m=(lo+hi)/2
        if m*m*(3-2*m)<w:lo=m
        else:hi=m
    return (lo+hi)/2
rows=read(P/'lean-retarget-manifest.json');reports=[]
for role in ('corvo','emily'):
    data=(P/'sprint-banks-v1'/f'{role}-sprint-bank.bin').read_bytes()
    magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
    assert magic==b'D2MOVE04' and count==29
    bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE05',5,n,33,fps,layout)+data[32:]);sources=[]
    for cid,alias in enumerate(('stand_lean_left','stand_lean_right','crouch_lean_left','crouch_lean_right'),29):
        row=next(r for r in rows if r['role']==role and r['selection']['alias']==alias)
        raw=(P/row['binary']).read_bytes();assert hashlib.sha256(raw).hexdigest()==row['sha256']
        tag,joints,frames,source_fps,source_layout=struct.unpack_from('<8sIIfQ',raw)
        assert tag==b'D2NATV01' and joints==n and source_layout==layout and source_fps==60
        fs=[[list(struct.unpack_from('<7f',raw,28+(f*n+j)*28))for j in range(n)]for f in range(38)]
        poses=[]
        for f in range(120):
            t=inverse_ease(f/119)*36;a=int(t);poses.append(blend(fs[a],fs[a+1],t-a))
        poses.append(poses[0]);bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,0.))
        for pose in poses:
            for v in pose:bank.extend(struct.pack('<7f',*v))
        sources.append(dict(alias=alias,source_binary=row['binary'],sha256=row['sha256'],source_frames=[0,36],parameter='absolute native lateral lean',mapping='inverse smoothstep into the authored 0.6s entry; preserves its support-foot solve'))
    path=OUT/f'{role}-lean-bank.bin';path.write_bytes(bank)
    reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE05',joints=n,clips=33,previous_payload_unchanged=bank[32:len(data)]==data[32:],parameter_clips=[29,30,31,32],sources=sources,events=False,root_writes=False,visual_accepted=False,gameplay_verified=False,vertical_peek_bound=False))
(P/'lean-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2));print('LEAN_BANKS: parameter curves prepared; runtime binding pending')
