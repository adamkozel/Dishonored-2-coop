"""Append source-clock low-drop carrier poses to the preserved v31 bank.

The authoring sequence has a 0.4-second lead-in. Trim that lead-in and map the
next native second to Drop46; recovery blends back into the live locomotion.
The crouched candidate retains the complete crouch base and local upper-body
release changes, with native bone lengths. It still needs visual acceptance.
"""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['carry-release-manifest-v3.json'],'carry-release-bank-sources-v1')
O=P/'carry-release-banks-v1';O.mkdir(exist_ok=True);reports=[]
for role in ['corvo','emily']:
    rig=rig_for(role);n=len(rig.joints)
    data=(P/'carry-banks-v1'/f'{role}-carry-bank.bin').read_bytes()
    magic,version,jn,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
    assert magic==b'D2MOVE31' and version==31 and count==229 and jn==n
    entry=24+121*n*28
    def old(clip,u):
        x=(u%1)*120;a=int(x);b=a+1
        return [blend([qts(struct.unpack_from('<7f',data,32+clip*entry+24+(a*n+j)*28))],
                      [qts(struct.unpack_from('<7f',data,32+clip*entry+24+(b*n+j)*28))],x-a)[0] for j in range(n)]
    m,frames=L.load(role,'carry_body_drop_low_sequence')
    stand0=old(225,0)
    bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE32',32,n,231,fps,layout)+data[32:])
    for crouch in [False,True]:
        poses=[];cid=229+int(crouch)
        for f in range(120):
            u=f/119
            pose=sample(frames,(.4+u)/m['duration'])
            if crouch:
                low=old(227,0);out=copy_pose(low)
                for j in range(3,18):out[j]=pose[j].copy()
                for j in (3,4,5):
                    delta=stand0[j].to_quaternion().inverted()@pose[j].to_quaternion()
                    out[j]=(low[j].to_quaternion()@delta).normalized().to_matrix().to_4x4()
                    out[j].translation=rig.bind[j].translation
                pose=out
            preview_helpers(rig,pose);poses.append(pose)
        # Fixed bank storage closes its sentinel frame; parameter sampling
        # uses frames 0..119 and never loops this one-shot action.
        bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,1.))
        for pose in poses+[poses[0]]:
            for mat in pose:bank.extend(struct.pack('<7f',*row(mat)))
        L.save(rig,role,('crouched_' if crouch else '')+'carry_drop_low_runtime',poses,
            [identity()for _ in poses],'interactions','Native-second low-drop carrier candidate; victim animation and outcome remain separately owned.',
            [m['sha256'],hashlib.sha256(data).hexdigest()],bank_clip=cid,source_time_range_seconds=[.4,1.4])
    path=O/f'{role}-carry-release-bank.bin';path.write_bytes(bank)
    assert bank[32:len(data)]==data[32:]
    reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),
        parent_sha256=hashlib.sha256(data).hexdigest().upper(),clips=231,format='D2MOVE32',prior_payload_unchanged=True,visual_accepted=False))
L.finish('carry-release-bank-source-manifest-v1.json')
(P/'carry-release-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2))
print('CARRY_RELEASE_BANK_V32 authored carrier candidates appended',flush=True)
