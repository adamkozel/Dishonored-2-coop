"""Append an authored full-body Windblast study without changing existing clips."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['power-retarget-manifest-v1.json'],'windblast-sources-v1')
OUT=P/'windblast-banks-v1';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'darkvision-context-banks-v1'/f'{role}-darkvision-context-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE26'and count==197
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE27',27,n,198,fps,layout)+data[32:])
 rig=rig_for(role);grip=[qts(struct.unpack_from('<7f',data,32+24+j*28))for j in range(n)]
 fingers=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and j!=16 and 'helper'not in rig.joints[j]['nativeName']:fingers.append(j)
 m,fs=L.load(role,'power_windblast_fire')
 def authored(t):
  pose=sample(fs,t)
  for j in fingers:pose[j]=grip[j].copy()
  pose[17]=grip[17].copy();preview_helpers(rig,pose);return pose
 poses=[authored(f/119)for f in range(120)]
 bank.extend(struct.pack('<IIIIff',197,121,0,0,0.,1.0))
 for pose in poses+[poses[0]]:
  assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
  for mat in pose:bank.extend(struct.pack('<7f',*row(mat)))
 L.save(rig,role,'native_phase_windblast_fire',[authored(f/60)for f in range(61)],[identity()for _ in range(61)],'powers',
  'Complete-body Windblast cast study with native first-person phase timing/finger articulation as inspiration, no wrist anchors. Native right-hand grip and sword attachment retained. Corvo gameplay availability only; Emily is an offline rig comparison. Native source owns world movement and gameplay outcomes.',[m['sha256']],runtime_phase=11,native_duration_ms=1000)
 path=OUT/f'{role}-windblast-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE27',clips=198,
  prior_payload_unchanged=bank[32:len(data)]==data[32:],source=m['sha256'],clip=197,variant=11,native_character='corvo',visual_accepted=False,runtime_verified=False,
  effects_policy='Native blast has gameplay damage/physics. Remote representation must use cosmetic native FX only, never duplicate the native blast entity.'))
L.finish('windblast-source-manifest-v1.json');(P/'windblast-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('WINDBLAST_BANKS generated',flush=True)
