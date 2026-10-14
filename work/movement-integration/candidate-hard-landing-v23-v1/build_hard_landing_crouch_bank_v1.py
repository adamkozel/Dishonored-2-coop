"""Append direct crouched CatFall recovery; preserve all standing landing clips."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library([],'hard-landing-crouch-sources-v1');OUT=P/'hard-landing-crouch-banks-v1';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'hard-landing-banks-v1'/f'{role}-hard-landing-bank.bin').read_bytes();magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE22'and count==181
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE23',23,n,183,fps,layout)+data[32:]);entry=24+121*n*28;rig=rig_for(role);clips=[]
 for unarmed in (False,True):
  cid=181+int(unarmed);source=179+int(unarmed);restId=76 if unarmed else 17
  rest=[qts(struct.unpack_from('<7f',data,32+restId*entry+24+j*28))for j in range(n)]
  donor=[[qts(struct.unpack_from('<7f',data,32+source*entry+24+(f*n+j)*28))for j in range(n)]for f in range(120)]
  def authored(t):
   # Native crouch requests can end CatFall near .6 of its nominal clock.
   # Finish low before that boundary, while preserving initial compression.
   pose=blend(sample(donor,t),rest,ease((t-.24)/.32));preview_helpers(rig,pose)
   return pose
  poses=[authored(f/119)for f in range(120)];poses.append(poses[0]);bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,1.))
  for pose in poses:
   for matrix in pose:bank.extend(struct.pack('<7f',*row(matrix)))
  alias=('empty_'if unarmed else'armed_')+'native_hard_landing_crouched'
  L.save(rig,role,alias,[authored(f/60)for f in range(61)],[identity()for _ in range(61)],'jump','Complete-body impact compression and direct crouched recovery on native CatFall clock.',[hashlib.sha256(data).hexdigest()],native_scene_duration_ms=1000,source_clip=source,crouch_reference=restId,recovery_window=[.24,.56])
  L.reports[-1]['equipment_policy']='hands_free'if unarmed else'sword_ready'
  (L.out/(L.reports[-1]['id']+'.json')).write_text(json.dumps(L.reports[-1],indent=2),encoding='utf-8')
  clips.append(dict(id=cid,alias=alias,source_clip=source,crouch_reference=restId))
 path=OUT/f'{role}-hard-landing-crouch-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE23',clips=183,prior_payload_unchanged=bank[32:len(data)]==data[32:],sources=clips,events=False,root_writes=False,visual_accepted=False,runtime_verified=False))
L.finish('hard-landing-crouch-source-manifest-v1.json');(P/'hard-landing-crouch-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
