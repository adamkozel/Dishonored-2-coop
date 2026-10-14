"""Compare a final native publication with its authored bank and attachment."""
import json,struct,sys
from pathlib import Path
import numpy as np
P=Path(__file__).resolve().parent
take=P/sys.argv[1];role=sys.argv[2];revision=int(sys.argv[3])
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def quat(q):
    x,y,z,w=np.array(q[:4])/np.linalg.norm(q[:4])
    return np.array([[1-2*(y*y+z*z),2*(x*y-z*w),2*(x*z+y*w)],
                     [2*(x*y+z*w),1-2*(x*x+z*z),2*(y*z-x*w)],
                     [2*(x*z-y*w),2*(y*z+x*w),1-2*(x*x+y*y)]])
def matrix(q):
    m=np.eye(4);m[:3,:3]=quat(q);m[:3,3]=q[4:7];return m
def native16(v):
    m=np.array(v).reshape(4,4).T.copy();m[3,3]=1;return m
def native12(v):
    m=np.eye(4);m[:3,3]=v[:3];m[:3,:3]=np.array(v[3:]).reshape(3,3).T;return m
def fk(fs,joints):
    out=[]
    for m,j in zip(fs,joints):out.append(m if j['parent']<0 else out[j['parent']]@m)
    return out
s=read(take/'idle-final-skeleton.json');a=read(take/'idle-sword-attachment.json');m=read(take/'idle-movement.json')
assert s['stable'] and s['finalPublicationComplete']
data=(P/f'crouched-ranged-banks-v{revision}'/f'{role}-crouched-ranged-bank.bin').read_bytes();n=len(s['joints']);entry=24+121*n*28
clip=213+(6 if m['rangedKind']==2 else 0);clip_axes=[]
phase=m['rangedAmount'];best=None
actual=[native16(j['localPose']) for j in s['joints']]
for frame in range(120):
    fs=[matrix(struct.unpack_from('<7f',data,32+clip*entry+24+(frame*n+j)*28)) for j in range(n)]
    error=max(np.abs(fs[j]-actual[j]).max() for j in range(2,27))
    if best is None or error<best[0]:best=(error,frame,fs)
g=fk(best[2],s['joints']);phase3={j['joint']:native16(j['matrix']) for j in s['phases'][3]['matrices']}
q=a['quaternion'];binding=np.eye(4);binding[:3,:3]=quat([-q[0],-q[1],-q[2],q[3]]);binding[:3,3]=a['offset']
root=native12(a['parentRenderRoot']);weapon=native12(a['weaponRenderRoot'])
reconstructed=root@native16(a['jointMatrix'])@binding
for frame in range(120):
    fs=[matrix(struct.unpack_from('<7f',data,32+clip*entry+24+(frame*n+j)*28)) for j in range(n)]
    clip_axes.append((fk(fs,s['joints'])[17]@binding)[:3,0])
report=dict(take=take.name,role=role,revision=revision,clip=clip,read_phase=phase,nearest_frame=best[1],
            local_pose_max_error=float(best[0]),attachment_stable=a['stable'],
            authored_blade_axis=(g[17]@binding)[:3,0].tolist(),
            final_blade_axis=(phase3[17]@binding)[:3,0].tolist(),
            observed_world_blade_axis=weapon[:3,0].tolist(),
            attachment_reconstruction_error=float(np.abs(weapon-reconstructed).max()),
            clip_blade_z_minmax=[float(min(x[2] for x in clip_axes)),float(max(x[2] for x in clip_axes))],
            notes='Separate native publication and attachment timestamps; max frame error includes independent idle clock sampling. Numerical basis is diagnostic, not visual acceptance.')
(take/'sword-pose-analysis.json').write_text(json.dumps(report,indent=2),encoding='utf-8');print(json.dumps(report,indent=2))
