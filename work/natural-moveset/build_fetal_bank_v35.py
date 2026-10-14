"""Append a compact release/recovery to preserved v36 banks.

Uses the full-body high-release preparation as an acting baseline only,
then recovers after the166ms compact victim clip. No FP position constraints,
gameplay events, root writes or bone-length changes. Visual candidate only.
"""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['library-retarget-manifest.json'],'fetal-bank-sources-v35')
O=P/'fetal-banks-v35';O.mkdir(exist_ok=True);reports=[]
for parent in read(P/'high-throw-bank-manifest-v31.json'):
 role=parent['role'];rig=rig_for(role);n=len(rig.joints);data=(P/parent['bank']).read_bytes()
 assert hashlib.sha256(data).hexdigest().upper()==parent['sha256']
 magic,version,jn,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
 assert (magic,version,jn,count,fps)==(b'D2MOVE36',36,n,234,60)
 entry=24+121*n*28
 def frame(clip,f):return[qts(struct.unpack_from('<7f',data,32+clip*entry+24+(f*n+j)*28))for j in range(n)]
 donor=[frame(233,f)for f in range(120)]
 im,idle=L.load(role,'sword_idle');poses=[]
 for f in range(120):
  t=(f/119)*.666
  pose=sample(donor,min(t,.166)/.666)
  # The arms unwind and the load counterbalance resolves after release.
  pose=blend(pose,sample(idle,t/im['duration'],True),ease((t-.166)/.5))
  preview_helpers(rig,pose);poses.append(pose)
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE37',37,n,235,60,layout)+data[32:])
 bank.extend(struct.pack('<IIIIff',234,121,0,0,0.,.666))
 for pose in poses+[poses[0]]:
  for mat in pose:bank.extend(struct.pack('<7f',*row(mat)))
 assert bank[32:len(data)]==data[32:]
 L.save(rig,role,'carry_fetal_release_runtime',poses,[identity()for _ in poses],'interactions',
  'Compact fallback release with source-clock recovery. Native fetal variant5, not drop_soft; hand contact and handoff remain experimental.',
  dict(parent_sha256=parent['sha256'],recovery_sha256=im['sha256']),bank_clip=234,native_duration_ms=666,candidate_handoff_ms=166)
 path=O/f'{role}-fetal-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),
  parent_sha256=parent['sha256'],format='D2MOVE37',clips=235,prior_payload_unchanged=True,runtime_verified=False,visual_accepted=False))
L.finish('fetal-bank-source-manifest-v35.json')
(P/'fetal-bank-manifest-v35.json').write_text(json.dumps(reports,indent=2))
print(json.dumps(reports),flush=True)
