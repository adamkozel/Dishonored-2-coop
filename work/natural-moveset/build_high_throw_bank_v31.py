"""Append a native-666ms throw to v35; retain all earlier clips byte-for-byte.

Uses the existing native-victim-inspired complete-body high release, trims
its 400ms review lead-in, and blends from the current supported loaded pose.
No scale changes, first-person constraints, root motion or animation events.
"""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['carry-release-manifest-v3.json'],'high-throw-bank-sources-v31')
O=P/'high-throw-banks-v31';O.mkdir(exist_ok=True);reports=[]
for parent in read(P/'crouched-pickup-bank-manifest-v3.json'):
 role=parent['role'];rig=rig_for(role);n=len(rig.joints);data=(P/parent['bank']).read_bytes()
 assert hashlib.sha256(data).hexdigest().upper()==parent['sha256']
 magic,version,jn,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
 assert (magic,version,jn,count,fps)==(b'D2MOVE35',35,n,233,60)
 entry=24+121*n*28
 loaded=[qts(struct.unpack_from('<7f',data,32+225*entry+24+j*28))for j in range(n)]
 m,donor=L.load(role,'carry_body_drop_high_sequence');poses=[]
 for f in range(120):
  t=(f/119)*.666
  pose=sample(donor,(.4+t)/m['duration'])
  pose=blend(loaded,pose,ease(t/.08))
  preview_helpers(rig,pose);poses.append(pose)
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE36',36,n,234,60,layout)+data[32:])
 bank.extend(struct.pack('<IIIIff',233,121,0,0,0.,.666))
 for pose in poses+[poses[0]]:
  for mat in pose:bank.extend(struct.pack('<7f',*row(mat)))
 assert bank[32:len(data)]==data[32:]
 L.save(rig,role,'carry_throw_high_runtime',poses,[identity()for _ in poses],'interactions',
  'Source-clock high-throw carrier candidate, native release at280ms; runtime comparison pending.',
  dict(parent_sha256=parent['sha256'],donor_sha256=m['sha256']),bank_clip=233,native_duration_ms=666,
  native_early_ragdoll_ms=200,native_ragdoll_ms=280,source_time_range_seconds=[.4,1.066])
 path=O/f'{role}-high-throw-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),
  parent_sha256=parent['sha256'],format='D2MOVE36',clips=234,prior_payload_unchanged=True,runtime_verified=False,visual_accepted=False))
L.finish('high-throw-bank-source-manifest-v31.json')
(P/'high-throw-bank-manifest-v31.json').write_text(json.dumps(reports,indent=2))
print(json.dumps(reports),flush=True)
