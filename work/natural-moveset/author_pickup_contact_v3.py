"""Corvo-only bounded contact pilot, using measured isolated Mayhew geometry.

Keeps the full-body base and native proportions. This is not an arbitrary-body
solution. Source first-person joints are never used as constraints.
"""
import sys,math,json,hashlib,struct
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['body-lift-short-manifest-v6.json'],'body-lift-contact-v3')
trace=read(P/'native-paired-pickup-corvo-v24-fast/contact-analysis.json')
samples=sorted(trace['samples'],key=lambda r:r['phase'])
assert len(samples)>=8 and all(0<=r['phase_span']<.18 for r in samples)
def target(u,key):
    if u<=samples[0]['phase']:return Vector(samples[0][key])
    if u>=samples[-1]['phase']:return Vector(samples[-1][key])
    k=next(k for k in range(len(samples)-1)if samples[k]['phase']<=u<=samples[k+1]['phase'])
    a=samples[k];b=samples[k+1];span=b['phase']-a['phase'];t=(u-a['phase'])/span
    # Time-aware cubic Hermite interpolation, bounded by measured neighbours.
    pa=Vector(a[key]);pb=Vector(b[key]);lo=samples[max(0,k-1)];hi=samples[min(len(samples)-1,k+2)]
    ma=(pb-Vector(lo[key]))/(b['phase']-lo['phase']);mb=(Vector(hi[key])-pa)/(hi['phase']-a['phase'])
    v=pa*(2*t**3-3*t*t+1)+ma*span*(t**3-2*t*t+t)+pb*(-2*t**3+3*t*t)+mb*span*(t**3-t*t)
    return Vector([max(min(pa[i],pb[i])-.03,min(max(pa[i],pb[i])+.03,v[i]))for i in range(3)])
role='corvo';rig=rig_for(role);m,frames=L.load(role,'carry_body_short_lift_study');poses=[];metrics=[]
for f in range(85):
    u=f/84;t=u*1.4;p=copy_pose(frames[f]);g=rig.fk(p)
    feet=[g[j].translation.copy()for j in [21,25]];rotations=[g[j].to_3x3().copy()for j in [21,25]]
    gather=ease(t/.32);drive=ease((t-.30)/.68);down=gather*(1-drive)
    # Shift the supported trunk toward this actual floor body; feet keep the
    # base's authored contacts and recovery steps, rather than sliding.
    p[2].translation.x+=.45*down
    w=ease((t-.12)/.18)*(1-ease((t-1.06)/.26))
    g=rig.fk(p);reach=target(u,'left_target')-g[7].translation
    arm_length=rig.bind[8].translation.length+rig.bind[9].translation.length
    correction=reach.normalized()*min(.30,max(0,reach.length-arm_length+.025))*w
    p[2].translation+=correction
    for k,(a,b,j)in enumerate([(19,20,21),(23,24,25)]):
        g=rig.fk(p);rig.solve(p,a,b,j,feet[k],g[a].translation+Vector((.8,0,-.1)));rig.orient(p,j,rotations[k])
    contacts=[]
    for a,b,j,key,weight,sign in [(7,8,9,'left_target',w,1),(14,15,16,'right_target',w*ease((t-.45)/.22)*(1-ease((t-.83)/.22)),-1)]:
        prior=copy_pose(p);g=rig.fk(p);rotation=g[j].to_3x3().copy();goal=target(u,key)
        rig.solve(p,a,b,j,goal,g[a].translation+Vector((.12,.35*sign,-.15)));rig.orient(p,j,rotation)
        for k in [a,b,j]:p[k]=blend([prior[k]],[p[k]],weight)[0]
        contacts.append(dict(joint=j,weight=weight,error=(rig.fk(p)[j].translation-goal).length))
    preview_helpers(rig,p);poses.append(p)
    metrics.append(dict(t=t,contacts=contacts,root_correction=list(correction),foot_error=max((rig.fk(p)[j].translation-feet[k]).length for k,j in enumerate([21,25]))))
L.save(rig,role,'carry_pickup_contact_pilot',poses,[identity()for _ in poses],'interactions',
       'Corvo-only measured-Mayhew contact pilot: preserve authored whole-body lift, shift trunk toward floor target and retarget wrist contact. Generic corpses, visual quality and runtime evidence pending.',
       dict(base_sha256=m['sha256'],trace_sha256=trace['source_sha256']),equipment_policy_override='hands_free')
L.reports[-1]['equipment_policy']='hands_free'
(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2))
L.finish('body-lift-contact-manifest-v3.json')
parent=next(r for r in read(P/'carry-pickup-bank-manifest-v1.json')if r['role']==role);data=(P/parent['bank']).read_bytes()
assert hashlib.sha256(data).hexdigest().upper()==parent['sha256']
n=len(rig.joints);entry=24+121*n*28;at=32+231*entry;bank=bytearray(data);resampled=[sample(poses,f/119)for f in range(120)]
blob=b''.join(struct.pack('<7f',*row(mat))for pose in resampled+[resampled[0]]for mat in pose)
bank[at+24:at+entry]=blob;assert bank[:at+24]==data[:at+24] and len(bank)==len(data)
out=P/'carry-pickup-contact-banks-v3';out.mkdir(exist_ok=True);path=out/'corvo-carry-pickup-contact-bank.bin';path.write_bytes(bank)
reports=read(P/'carry-pickup-bank-manifest-v1.json')
for r in reports:
    if r['role']=='corvo':r.update(bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),parent_sha256=parent['sha256'],clip_sha256=L.reports[-1]['sha256'],contact_pilot=True)
(P/'carry-pickup-contact-bank-manifest-v3.json').write_text(json.dumps(reports,indent=2))
analysis=dict(role=role,complete=False,visual_accepted=False,metrics=metrics,max_foot_error_m=max(r['foot_error']for r in metrics),
    max_left_error_when_gripping=max(c['error']for r in metrics for c in r['contacts']if c['joint']==9 and c['weight']>.98))
(P/'carry-research/pickup-contact-pilot-v3.json').write_text(json.dumps(analysis,indent=2))
print(json.dumps({k:v for k,v in analysis.items()if k!='metrics'}),flush=True)


