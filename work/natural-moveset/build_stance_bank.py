"""Version-3 bank: native walk/run plus authored, distance-clocked crouch.

Preserves version-2 bank entries byte-for-byte. No game assets or animation
events are changed. Use Blender's Python for the existing rig math library.
"""
import sys, struct, json, hashlib, math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from rig_math import NativeRig, qts, read
from mathutils import Quaternion
OUT=P/'stance-banks-v1';OUT.mkdir(exist_ok=True)
directions=['forward','forward_left','left','back_left','back','back_right','right','forward_right']
metadata=read(P/'crouch-directions-retarget-manifest.json')+read(P/'stance-retarget-manifest.json')

def load(row):
    raw=(P/row['binary']).read_bytes();assert hashlib.sha256(raw).hexdigest()==row['sha256']
    magic,n,frames,fps,layout=struct.unpack_from('<8sIIfQ',raw)
    assert magic==b'D2NATV01' and n==row['joints'] and frames==row['frames']
    return [[list(struct.unpack_from('<7f',raw,28+(f*n+j)*28))for j in range(n)]for f in range(frames)]

def blend(a,b,t):
    out=[]
    for x,y in zip(a,b):
        q=Quaternion((x[3],*x[:3])).slerp(Quaternion((y[3],*y[:3])),t).normalized()
        out.append([q.x,q.y,q.z,q.w,*[x[k]+(y[k]-x[k])*t for k in range(4,7)]])
    return out

def sample(frames,frame):
    a=min(int(frame),len(frames)-1);b=min(a+1,len(frames)-1)
    return blend(frames[a],frames[b],frame-a)

reports=[]
for role in ['corvo','emily']:
    original=P/'native-gaits'/f'{role}-gait-bank.bin';data=original.read_bytes()
    magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
    assert magic==b'D2MOVE02' and count==17
    bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE03',3,n,26,fps,layout)+data[32:])
    rig=NativeRig(read(P.parent/f'mixamo-crouch/native-{role}.json')['joints'])
    entry=24+121*n*28
    native=[[list(struct.unpack_from('<7f',data,32+entry+24+(f*n+j)*28))for j in range(n)]for f in range(120)]
    # Normalize each foot's height envelope before matching phase; compare
    # timing rather than the very different standing/crouched pelvis height.
    def waves(fs):
        g=[rig.fk([qts(v)for v in pose])for pose in fs]
        channels=[[p[j].translation.z for p in g]for j in [21,25]]
        return [[(x-min(c))/max(.001,max(c)-min(c))for x in c]for c in channels]
    reference=waves(native);sources=[]
    for cid,alias in enumerate(['crouch_idle']+['crouch_walk_'+d for d in directions],17):
        row=next(r for r in metadata if r['role']==role and r['selection']['alias']==alias)
        frames=load(row);stride=0 if cid==17 else row['stride_m']
        # Direction studies contain two repetitions, with round-to-frame end
        # times. Sample one measured cycle, not half of an arbitrary duration.
        period=(len(frames)-1) if cid==17 else stride/row['review_speed_mps']*row['fps']
        poses=[sample(frames,period*f/120)for f in range(120)]
        offset=0
        if cid!=17:
            w=waves(poses)
            offset=min(range(120),key=lambda k:sum((w[j][(f+k)%120]-reference[j][f])**2 for j in range(2)for f in range(120)))
            poses=poses[offset:]+poses[:offset]
        # Close the cycle over the final five percent, as in the source bank.
        for f in range(115,120):
            t=(f-114)/6;t=t*t*(3-2*t);poses[f]=blend(poses[f],poses[0],t)
        poses.append(poses[0])
        bank.extend(struct.pack('<IIIIff',cid,121,0,0,stride,0 if cid==17 else (cid-18)*math.pi/4))
        for pose in poses:
            for v in pose:bank.extend(struct.pack('<7f',*v))
        sources.append(dict(alias=alias,source_binary=row['binary'],sha256=row['sha256'],stride_m=stride,phase_offset=offset/120,source_period_frames=period))
    path=OUT/f'{role}-stance-bank.bin';path.write_bytes(bank)
    reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE03',joints=n,clips=26,walk_run_source_sha256=hashlib.sha256(data).hexdigest(),standing_payload_unchanged=bank[32:32+17*entry]==data[32:],crouch_sources=sources,events=False,root_writes=False,visual_accepted=False,gameplay_verified=False))
(P/'stance-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2))
print('STANCE_BANKS',len(reports),'roles; 26 full-body clips each')
