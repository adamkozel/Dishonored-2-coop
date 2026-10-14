"""Append phase-aligned authored armed sprint to the unchanged v3 bank."""
import sys,struct,json,math,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from rig_math import NativeRig,qts,read
from mathutils import Quaternion
OUT=P/'sprint-banks-v1';OUT.mkdir(exist_ok=True)
def blend(a,b,t):
    out=[]
    for x,y in zip(a,b):
        q=Quaternion((x[3],*x[:3])).slerp(Quaternion((y[3],*y[:3])),t).normalized()
        out.append([q.x,q.y,q.z,q.w,*[x[k]+(y[k]-x[k])*t for k in range(4,7)]])
    return out
def sample(fs,t):
    a=min(int(t),len(fs)-1);return blend(fs[a],fs[min(a+1,len(fs)-1)],t-a)
rows=read(P/'dynamics-retarget-manifest-v3.json');reports=[]
for role in ('corvo','emily'):
    data=(P/'stance-banks-v1'/f'{role}-stance-bank.bin').read_bytes()
    magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
    assert magic==b'D2MOVE03' and count==26
    bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE04',4,n,29,fps,layout)+data[32:])
    rig=NativeRig(read(P.parent/f'mixamo-crouch/native-{role}.json')['joints'])
    entry=24+121*n*28
    native=[[list(struct.unpack_from('<7f',data,32+entry+24+(f*n+j)*28))for j in range(n)]for f in range(120)]
    def waves(fs):
        g=[rig.fk([qts(v)for v in pose])for pose in fs]
        channels=[[p[j].translation.z for p in g]for j in (21,25)]
        return [[(x-min(c))/max(.001,max(c)-min(c))for x in c]for c in channels]
    reference=waves(native);sources=[]
    for cid,alias,angle in [(26,'sprint_forward',0),(27,'sprint_forward_left',math.pi/4),(28,'sprint_forward_right',-math.pi/4)]:
        row=next(r for r in rows if r['role']==role and r['selection']['alias']==alias)
        raw=(P/row['binary']).read_bytes();assert hashlib.sha256(raw).hexdigest()==row['sha256']
        tag,joints,frames,source_fps,source_layout=struct.unpack_from('<8sIIfQ',raw)
        assert tag==b'D2NATV01' and joints==n and layout==source_layout
        fs=[[list(struct.unpack_from('<7f',raw,28+(f*n+j)*28))for j in range(n)]for f in range(frames)]
        # The authoring metadata describes a two-cycle donor distance, not
        # one stride. Dividing by source_cycles is essential to correct speed.
        stride=row['stride_m']/row['source_cycles'];period=stride/row['review_speed_mps']*source_fps
        assert .2<stride<6 and 10<period<frames-1
        poses=[sample(fs,period*f/120)for f in range(120)];w=waves(poses)
        offset=min(range(120),key=lambda k:sum((w[j][(f+k)%120]-reference[j][f])**2 for j in range(2)for f in range(120)))
        poses=poses[offset:]+poses[:offset]
        for f in range(115,120):
            t=(f-114)/6;t=t*t*(3-2*t);poses[f]=blend(poses[f],poses[0],t)
        poses.append(poses[0]);bank.extend(struct.pack('<IIIIff',cid,121,0,0,stride,angle))
        for pose in poses:
            for v in pose:bank.extend(struct.pack('<7f',*v))
        sources.append(dict(alias=alias,source_binary=row['binary'],sha256=row['sha256'],stride_m=stride,phase_offset=offset/120,source_cycles=row['source_cycles'],source_period_frames=period))
    path=OUT/f'{role}-sprint-bank.bin';path.write_bytes(bank)
    reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE04',joints=n,clips=29,previous_payload_unchanged=bank[32:32+26*entry]==data[32:],sources=sources,events=False,root_writes=False,visual_accepted=False,gameplay_verified=False))
(P/'sprint-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2));print('SPRINT_BANKS: 29 clips each; unchanged v3 payload')
