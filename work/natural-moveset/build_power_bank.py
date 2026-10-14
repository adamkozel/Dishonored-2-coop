"""Append phase-driven full-body Blink/Far Reach candidates; no native events."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['power-retarget-manifest-v1.json'],'power-bank-sources-v1')
OUT=P/'power-banks-v1';OUT.mkdir(exist_ok=True);reports=[]
aliases=['blink_fire_in','blink_fire_loop','blink_fire_out','far_reach_fire_in','far_reach_fire_in_loop','far_reach_fire_out','far_reach_pull_in','far_reach_pull_loop','far_reach_pull_out']
for role in ('corvo','emily'):
 data=(P/'vault-banks-v1'/f'{role}-vault-bank.bin').read_bytes();magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE14' and count==117
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE15',15,n,126,fps,layout)+data[32:]);rig=rig_for(role);sources=[]
 entry=24+121*n*28;grip=[qts(struct.unpack_from('<7f',data,32+24+j*28))for j in range(n)]
 fingers=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and j!=16 and 'helper'not in rig.joints[j]['nativeName']:fingers.append(j)
 for offset,alias in enumerate(aliases):
  m,fs=L.load(role,'power_'+alias);poses=[]
  for f in range(120):
   pose=sample(fs,f/119)
   for j in fingers:pose[j]=grip[j].copy()
   pose[17]=grip[17].copy();preview_helpers(rig,pose);poses.append(pose)
  # Loop closure is already authored; retain genuine phase endpoints.
  duration=m['duration'];bank.extend(struct.pack('<IIIIff',117+offset,121,0,0,0.,duration))
  for pose in poses+[poses[0]]:
   assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
   for mat in pose:bank.extend(struct.pack('<7f',*row(mat)))
  sources.append(dict(variant=offset+1,clip=117+offset,alias=m['selection']['alias'],sha256=m['sha256']))
  L.save(rig,role,'native_phase_'+alias,poses,[identity()for _ in poses],'powers','Native body-acting power candidate, right sword fingers/attachment from the current bank. Native confirmed source scene supplies phase/clock. No gameplay events, root writes, mana or effects.',[m['sha256']],runtime_phase=offset+1)
 path=OUT/f'{role}-power-bank.bin';path.write_bytes(bank);reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE15',prior_payload_unchanged=bank[32:len(data)]==data[32:],sources=sources,visual_accepted=False,native_vfx_integrated=False))
L.finish('power-bank-source-manifest-v1.json');(P/'power-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('POWER_BANKS generated',flush=True)
