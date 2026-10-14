"""Bounded airborne curves: no root animation, physics, events or FP writes."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent
from mathutils import Quaternion
def blend(a,b,t):
    out=[]
    for x,y in zip(a,b):
        q=Quaternion((x[3],*x[:3])).slerp(Quaternion((y[3],*y[:3])),t).normalized()
        out.append([q.x,q.y,q.z,q.w,*[x[k]+(y[k]-x[k])*t for k in range(4,7)]])
    return out
OUT=P/'air-banks-v1';OUT.mkdir(exist_ok=True)
rows=[]
for manifest in ('jumps-retarget-manifest-v3.json','dynamics-retarget-manifest-v3.json','library-retarget-manifest.json'):
    rows+=json.loads((P/manifest).read_text())
reports=[]
for role in ('corvo','emily'):
    data=(P/'slide-banks-v1'/f'{role}-slide-bank.bin').read_bytes()
    magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
    assert magic==b'D2MOVE06' and count==37
    bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE07',7,n,44,fps,layout)+data[32:]);sources=[]
    standing=next(r for r in rows if r['role']==role and r['selection']['alias']=='standing_jump_sequence')
    marks={m['phase']:round(m['at']*standing['fps']) for m in standing['phase_marks']}
    for cid,alias,part in [(37,'standing_jump_sequence','rise'),(38,'sword_fall_loop','full'),(39,'standing_jump_sequence','land'),
        (40,'running_jump_sequence','running-rise'),(41,'running_jump_sequence','running-land'),(42,'crouch_jump_to_crouch','crouch-land'),(43,'crouch_jump_to_crouch','crouch-rise')]:
        row=next(r for r in rows if r['role']==role and r['selection']['alias']==alias)
        raw=(P/row['binary']).read_bytes();assert hashlib.sha256(raw).hexdigest()==row['sha256']
        tag,joints,frames,source_fps,source_layout=struct.unpack_from('<8sIIfQ',raw)
        assert tag==b'D2NATV01' and joints==n and layout==source_layout
        lo,hi=0,frames-1
        if part=='rise':lo,hi=marks['takeoff'],marks['apex']
        if part=='land':lo=marks['landing']
        if part.startswith('running'):
            z=[p[6] for p in row['root_track']];apex=max(range(len(z)),key=lambda i:z[i])
            if part=='running-rise':lo,hi=next(i for i,h in enumerate(z) if h>.06),apex
            else:lo=next(i for i in range(apex,len(z)) if z[i]<.025)
        if part.startswith('crouch'):
            cm={m['phase']:round(m['at']*source_fps) for m in row['phase_marks']}
            if part=='crouch-land':lo=cm['landing']
            else:lo,hi=cm['takeoff'],cm['takeoff']+marks['apex']-marks['takeoff']
        assert 0<=lo<hi<frames
        fs=[[list(struct.unpack_from('<7f',raw,28+(f*n+j)*28))for j in range(n)]for f in range(lo,hi+1)]
        poses=[]
        for f in range(120):
            t=(len(fs)-1)*f/119;a=int(t);poses.append(blend(fs[a],fs[min(a+1,len(fs)-1)],t-a))
        poses.append(poses[0]);duration=(hi-lo)/source_fps
        bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,duration))
        for pose in poses:
            for v in pose:bank.extend(struct.pack('<7f',*v))
        sources.append(dict(id=cid,alias=alias,part=part,source=row['binary'],sha256=row['sha256'],source_frames=[lo,hi],duration=duration))
    path=OUT/f'{role}-air-bank.bin';path.write_bytes(bank)
    reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE07',previous_payload_unchanged=bank[32:len(data)]==data[32:],sources=sources,root_writes=False,events=False,gameplay_verified=False,visual_accepted=False,limitations=['No double jump, hard-landing or airborne combat acceptance from this bank.','Running and crouched variants require native-state timing and actual-game visual review.']))
(P/'air-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2));print('AIR_BANKS prepared; native gameplay tests pending')
