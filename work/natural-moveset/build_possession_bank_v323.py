"""Observed Possession entry/aim from full-body studies; root/world effects stay external."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['power-retarget-manifest-v1.json'],'possession-sources-v323')
OUT=P/'possession-banks-v323';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'swarm-context-banks-v272'/f'{role}-swarm-context-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
 assert magic==b'D2MOVE50' and version==50 and count==313
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE51',51,n,315,fps,layout)+data[32:])
 rig=rig_for(role);grip=[qts(struct.unpack_from('<7f',data,32+24+j*28))for j in range(n)];fingers=[];sources=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and j!=16 and 'helper'not in rig.joints[j]['nativeName']:fingers.append(j)
 for offset,(phase,duration)in enumerate([('in',333),('loop',200)]):
  meta,frames=L.load(role,'power_possession_fire_'+phase);poses=[]
  for f in range(120):
   pose=sample(frames,f/119)
   for j in fingers:pose[j]=grip[j].copy()
   pose[17]=grip[17].copy();preview_helpers(rig,pose);poses.append(pose)
  bank.extend(struct.pack('<IIIIff',313+offset,121,0,0,0.,duration/1000))
  for pose in poses+[poses[0]]:
   assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
   for matrix in pose:bank.extend(struct.pack('<7f',*row(matrix)))
  provenance=dict(body=meta['sha256'],native_take='possession-empty-v322',asset=f'midnight/gameplay/player/base/powers/power_possession_cast_{phase}.midnightscene')
  L.save(rig,role,'native_possession_'+phase,poses,[identity()for _ in poses],'powers','Complete-body native caster acting with FP timing/finger inspiration only. Standing armed Corvo candidate. Current native lengths/correctives and sword grip/socket retained. No fixed FP wrist anchors, root, AI, damage or VFX events. Emily is offline skeleton comparison only.',provenance,runtime_phase=26+offset,native_duration_ms=duration)
  sources.append(dict(variant=26+offset,clip=313+offset,phase=phase,duration_ms=duration,source=provenance))
 path=OUT/f'{role}-possession-bank.bin';path.write_bytes(bank);assert bank[32:len(data)]==data[32:]
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),format='D2MOVE51',clips=315,prior_payload_unchanged=True,sources=sources,native_character='corvo',visual_accepted=False,runtime_verified=False))
L.finish('possession-bank-source-manifest-v323.json');(P/'possession-bank-manifest-v323.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('SWARM_BANKS_GENERATED',flush=True)
