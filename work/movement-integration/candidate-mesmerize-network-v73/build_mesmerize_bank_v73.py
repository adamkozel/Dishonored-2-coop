"""Append native-clock Mesmerize body acting; no gameplay/FX or root tracks."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['power-retarget-manifest-v1.json'],'mesmerize-sources-v73')
OUT=P/'mesmerize-banks-v73';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'crouched-grenade-banks-v62'/f'{role}-crouched-grenade-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE44'and count==273
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE45',45,n,276,fps,layout)+data[32:])
 rig=rig_for(role);grip=[qts(struct.unpack_from('<7f',data,32+24+j*28))for j in range(n)];fingers=[];sources=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and j!=16 and 'helper'not in rig.joints[j]['nativeName']:fingers.append(j)
 for offset,(phase,duration)in enumerate([('in',466),('loop',1200),('out',666)]):
  m,fs=L.load(role,'power_mesmerize_fire_'+phase);poses=[]
  for f in range(120):
   pose=sample(fs,f/119)
   for j in fingers:pose[j]=grip[j].copy()
   pose[17]=grip[17].copy();preview_helpers(rig,pose);poses.append(pose)
  bank.extend(struct.pack('<IIIIff',273+offset,121,0,0,0.,duration/1000))
  for pose in poses+[poses[0]]:
   assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
   for matrix in pose:bank.extend(struct.pack('<7f',*row(matrix)))
  L.save(rig,role,'native_mesmerize_'+phase,poses,[identity()for _ in poses],'powers',
   'Native support full-body donor with first-person Mesmerize timing/finger inspiration. Standing armed Emily runtime scope; Corvo offline comparison only. Current sword grip, attachment, native lengths and helpers retained. No root motion, AI, resource writes or effects.',[m['sha256']],runtime_phase=16+offset,native_duration_ms=duration)
  sources.append(dict(variant=16+offset,clip=273+offset,source=m['sha256'],phase=phase,duration_ms=duration))
 path=OUT/f'{role}-mesmerize-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE45',clips=276,
  prior_payload_unchanged=bank[32:len(data)]==data[32:],sources=sources,native_character='emily',visual_accepted=False,runtime_verified=False,
  effects_policy='No remote native power invocation or NPC-affecting entity. World cosmetic VFX needs independent ownership/lifetime validation.'))
L.finish('mesmerize-bank-source-manifest-v73.json');(P/'mesmerize-bank-manifest-v73.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('MESMERIZE_BANKS generated',flush=True)
