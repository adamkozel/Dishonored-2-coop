"""Carrier-only proof: loaded idle/forward walking, standing and crouched.

No partner mutation or pickup/release coverage is implied by this bank.
"""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['carry-body-manifest-v3.json'],'carry-bank-sources-v1')
O=P/'carry-banks-v1';O.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
    rig=rig_for(role);n=len(rig.joints)
    data=(P/'crouched-ranged-banks-v4'/f'{role}-crouched-ranged-bank.bin').read_bytes()
    magic,version,jn,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
    assert magic==b'D2MOVE30' and version==30 and count==225 and jn==n
    entry=24+121*n*28
    def old(clip,u):
        x=(u%1)*120;a=int(x);b=a+1
        return [blend([qts(struct.unpack_from('<7f',data,32+clip*entry+24+(a*n+j)*28))],
                      [qts(struct.unpack_from('<7f',data,32+clip*entry+24+(b*n+j)*28))],x-a)[0] for j in range(n)]
    ref=old(0,0);refG=rig.fk(ref)
    bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE31',31,n,229,fps,layout)+data[32:]);sources=[]
    for crouch in (False,True):
        for walking in (False,True):
            alias='carry_body_'+('walk' if walking else 'idle');m,fs=L.load(role,'carry_body_idle')
            duration=m['duration'];cid=225+int(crouch)*2+int(walking)
            baseclip=(18 if crouch else 1) if walking else (17 if crouch else 0)
            stride=struct.unpack_from('<f',data,32+baseclip*entry+16)[0] if walking else 0.
            poses=[]
            for f in range(120):
                u=f/120;loaded=sample(fs,u,True);pose=copy_pose(loaded)
                if crouch or walking:
                    # Whole crouched base and its native-length legs are
                    # combined with the loaded upper body's changing posture.
                    low=old(baseclip,u);pose=copy_pose(low)
                    for j in range(3,18):pose[j]=loaded[j].copy()
                    # Retain the crouch spine and layer the animated load
                    # counterbalance locally, rather than anchoring FP wrists.
                    for j in (3,4,5):
                        delta=ref[j].to_quaternion().inverted()@loaded[j].to_quaternion()
                        pose[j]=(low[j].to_quaternion()@delta).normalized().to_matrix().to_4x4()
                        pose[j].translation=rig.bind[j].translation
                    preview_helpers(rig,pose)
                poses.append(pose)
            bank.extend(struct.pack('<IIIIff',cid,121,0,0,stride,duration))
            for pose in poses+[poses[0]]:
                for mat in pose:bank.extend(struct.pack('<7f',*row(mat)))
            L.save(rig,role,('crouched_' if crouch else '')+alias+'_runtime',poses,[identity() for _ in poses],'interactions',
                'Carrier-only loaded pose candidate. Distance drives gait. Corpse pairing, pickup, release and non-forward travel are pending.',
                [m['sha256'],hashlib.sha256(data).hexdigest()],bank_clip=cid,cycle_distance_m=stride)
            sources.append(dict(clip=cid,walking=walking,crouched=crouch,cycle_distance_m=stride,source=m['sha256']))
    path=O/f'{role}-carry-bank.bin';path.write_bytes(bank)
    assert bank[32:len(data)]==data[32:]
    reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),parent=hashlib.sha256(data).hexdigest(),
                        clips=229,format='D2MOVE31',prior_payload_unchanged=True,source=sources,corpse_playback=False,runtime_verified=False,visual_accepted=False))
L.finish('carry-bank-source-manifest-v1.json')
(P/'carry-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('CARRY_BANK_V31 carrier-only candidate; matched NPC playback still pending',flush=True)
