"""Append the authored whole-body Dark Vision cast; native source owns time/VFX."""
import sys, struct, json, hashlib
from pathlib import Path
P=Path(__file__).resolve().parent
sys.path.insert(0,str(P))
from clip_io import *
L=Library(['power-retarget-manifest-v1.json'],'darkvision-sources-v1')
OUT=P/'darkvision-banks-v1';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'keyhole-banks-v2'/f'{role}-keyhole-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
 assert magic==b'D2MOVE24' and count==193
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE25',25,n,194,fps,layout)+data[32:])
 rig=rig_for(role);grip=[qts(struct.unpack_from('<7f',data,32+24+j*28))for j in range(n)]
 fingers=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and j!=16 and 'helper'not in rig.joints[j]['nativeName']:fingers.append(j)
 m,fs=L.load(role,'power_dark_vision_fire')
 def authored(t):
  pose=sample(fs,t)
  for j in fingers:pose[j]=grip[j].copy()
  pose[17]=grip[17].copy();preview_helpers(rig,pose)
  return pose
 poses=[authored(f/119)for f in range(120)]
 bank.extend(struct.pack('<IIIIff',193,121,0,0,0.,.933))
 for pose in poses+[poses[0]]:
  assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
  for mat in pose:bank.extend(struct.pack('<7f',*row(mat)))
 preview=[authored(f/56)for f in range(57)]
 L.save(rig,role,'native_phase_dark_vision_fire',preview,[identity()for _ in preview],'powers',
  'Authored full-body Dark Vision cast. Native first-person timing and finger articulation inform the performance, without first-person wrist anchoring. Sword fingers and attachment use current native bank grip. Source native scene owns phase; no gameplay events, effects, camera or root writes.',
  [m['sha256']],runtime_phase=10,native_duration_ms=933)
 path=OUT/f'{role}-darkvision-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE25',clips=194,
  prior_payload_unchanged=bank[32:len(data)]==data[32:],source=m['sha256'],clip=193,variant=10,visual_accepted=False,runtime_verified=False,
  effects_policy='Native casting player owns Dark Vision postprocess and reveal; no observer overlay or remote damage/events.'))
L.finish('darkvision-source-manifest-v1.json')
(P/'darkvision-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('DARKVISION_BANKS generated',flush=True)
