"""Append a separate standing carrier study. Existing v32 payload is unchanged."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['body-lift-short-manifest-v6.json'],'carry-pickup-bank-sources-v1')
O=P/'carry-pickup-banks-v1';O.mkdir(exist_ok=True);reports=[]
parents={m['role']:m for m in read(P/'carry-contact-bank-manifest-v2.json')}
for role in ['corvo','emily']:
    rig=rig_for(role);n=len(rig.joints);parent=parents[role];data=(P/parent['bank']).read_bytes()
    assert hashlib.sha256(data).hexdigest().upper()==parent['sha256']
    magic,version,jn,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
    assert (magic,version,jn,count,fps)==(b'D2MOVE32',32,n,231,60)
    m,frames=L.load(role,'carry_body_short_lift_study')
    poses=[sample(frames,f/119)for f in range(120)]
    bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE33',33,n,232,fps,layout)+data[32:])
    bank.extend(struct.pack('<IIIIff',231,121,0,0,0.,1.4))
    for pose in poses+[poses[0]]:
        for mat in pose:bank.extend(struct.pack('<7f',*row(mat)))
    file=O/f'{role}-carry-pickup-bank.bin';file.write_bytes(bank)
    assert bank[32:len(data)]==data[32:]
    L.save(rig,role,'carry_pickup_runtime_study',poses,[identity()for _ in poses],'interactions',
        'Standing carrier: authored gather 0-.30s follows observed native preparation; .30-1.40s follows native 1000ms Carry45 In. Victim choreography is separate and incomplete.',
        [m['sha256'],parent['sha256']],bank_clip=231)
    reports.append(dict(role=role,bank=str(file.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),parent_sha256=parent['sha256'],
        clip_sha256=m['sha256'],format='D2MOVE33',clips=232,prior_payload_unchanged=True,visual_accepted=False,runtime_verified=False,
        phase_mapping=dict(preparation='0-.30 authored seconds / first 300ms observed preparation, hold if longer',native_in='.30-1.40 authored seconds / native 1000ms scene'),
        limitations=['standing carrier only','no victim binding','no first-person or gameplay event changes']))
L.finish('carry-pickup-bank-source-manifest-v1.json')
(P/'carry-pickup-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2))
print('CARRY_PICKUP_BANK_V33 separate carrier experiment',flush=True)
