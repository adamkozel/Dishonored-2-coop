"""Append three native-clock Bend Time body phases; keep existing payload intact."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['power-retarget-manifest-v1.json'],'bendtime-sources-v40')
OUT=P/'bendtime-banks-v40';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'fetal-banks-v35'/f'{role}-fetal-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE37'and count==235
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE38',38,n,238,fps,layout)+data[32:])
 rig=rig_for(role);grip=[qts(struct.unpack_from('<7f',data,32+24+j*28))for j in range(n)];fingers=[];sources=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and j!=16 and 'helper'not in rig.joints[j]['nativeName']:fingers.append(j)
 for offset,(phase,duration)in enumerate([('in',533),('loop',300),('out',433)]):
  m,fs=L.load(role,'power_bend_time_fire_'+phase)
  def authored(t):
   pose=sample(fs,t)
   for j in fingers:pose[j]=grip[j].copy()
   pose[17]=grip[17].copy();preview_helpers(rig,pose);return pose
  poses=[authored(f/119)for f in range(120)]
  bank.extend(struct.pack('<IIIIff',235+offset,121,0,0,0.,duration/1000))
  for pose in poses+[poses[0]]:
   assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
   for mat in pose:bank.extend(struct.pack('<7f',*row(mat)))
  L.save(rig,role,'native_phase_bend_time_'+phase,poses,[identity()for _ in poses],'powers',
   'Authored full-body Bend Time phase; native standing Corvo source clock. Right grip and weapon attachment retained. No root motion, damage, time requests or screen effects. Emily offline rig comparison only.',[m['sha256']],runtime_phase=12+offset,native_duration_ms=duration)
  sources.append(dict(variant=12+offset,clip=235+offset,source=m['sha256'],phase=phase,duration_ms=duration))
 path=OUT/f'{role}-bendtime-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE38',clips=238,
  prior_payload_unchanged=bank[32:len(data)]==data[32:],sources=sources,native_character='corvo',visual_accepted=False,runtime_verified=False,
  effects_policy='Never invoke native Bend Time gameplay toggle on the remote avatar. Viewer effects require separate verified ownership.'))
L.finish('bendtime-bank-source-manifest-v40.json');(P/'bendtime-bank-manifest-v40.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('BENDTIME_BANKS generated',flush=True)
