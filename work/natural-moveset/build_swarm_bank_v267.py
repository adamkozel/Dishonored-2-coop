"""Native Swarm phases from full-body studies; root/world effects stay external."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['power-retarget-manifest-v1.json'],'swarm-sources-v267')
OUT=P/'swarm-banks-v267';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'domino-context-banks-v243'/f'{role}-domino-context-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
 assert magic==b'D2MOVE48' and version==48 and count==301
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE49',49,n,304,fps,layout)+data[32:])
 rig=rig_for(role);grip=[qts(struct.unpack_from('<7f',data,32+24+j*28))for j in range(n)];fingers=[];sources=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and j!=16 and 'helper'not in rig.joints[j]['nativeName']:fingers.append(j)
 for offset,(phase,duration)in enumerate([('in',400),('loop',333),('out',1600)]):
  meta,frames=L.load(role,'power_devouring_swarm_fire_'+phase);poses=[]
  for f in range(120):
   pose=sample(frames,f/119)
   for j in fingers:pose[j]=grip[j].copy()
   pose[17]=grip[17].copy();preview_helpers(rig,pose);poses.append(pose)
  bank.extend(struct.pack('<IIIIff',301+offset,121,0,0,0.,duration/1000))
  for pose in poses+[poses[0]]:
   assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
   for matrix in pose:bank.extend(struct.pack('<7f',*row(matrix)))
  provenance=dict(body=meta['sha256'],native_take='swarm-native-standing-observe-v266',asset=f'midnight/gameplay/player/base/powers/power_swarm_cast_{phase}.midnightscene')
  L.save(rig,role,'native_swarm_'+phase,poses,[identity()for _ in poses],'powers','Complete-body native caster acting with FP timing/finger inspiration only. Standing armed Corvo candidate. Current native lengths/correctives and sword grip/socket retained. No fixed FP wrist anchors, root, AI, damage or VFX events. Emily is offline skeleton comparison only.',provenance,runtime_phase=23+offset,native_duration_ms=duration)
  sources.append(dict(variant=23+offset,clip=301+offset,phase=phase,duration_ms=duration,source=provenance))
 path=OUT/f'{role}-swarm-bank.bin';path.write_bytes(bank);assert bank[32:len(data)]==data[32:]
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),format='D2MOVE49',clips=304,prior_payload_unchanged=True,sources=sources,native_character='corvo',visual_accepted=False,runtime_verified=False))
L.finish('swarm-bank-source-manifest-v267.json');(P/'swarm-bank-manifest-v267.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('SWARM_BANKS_GENERATED',flush=True)
