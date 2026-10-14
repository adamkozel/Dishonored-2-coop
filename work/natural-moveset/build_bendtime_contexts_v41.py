"""Whole-body Bend Time context candidates with one common phase reference."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['bendtime-bank-source-manifest-v40.json'],'bendtime-contexts-v41')
OUT=P/'bendtime-context-banks-v41';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'bendtime-banks-v40'/f'{role}-bendtime-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE38'and count==238
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE39',39,n,247,fps,layout)+data[32:]);entry=24+121*n*28
 rig=rig_for(role);_,start=L.load(role,'native_phase_bend_time_in');reference=start[0];rg=rig.fk(reference);right=[];clips=[]
 for i in range(n):
  k=i
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and i!=16 and 'helper'not in rig.joints[i]['nativeName']:right.append(i)
 for phaseIndex,(phase,duration)in enumerate([('in',533),('loop',300),('out',433)]):
  m,fs=L.load(role,'native_phase_bend_time_'+phase)
  for context,(restid,crouched,armed)in enumerate(((59,False,False),(17,True,True),(76,True,False))):
   cid=238+phaseIndex*3+context;rest=[qts(struct.unpack_from('<7f',data,32+restid*entry+24+j*28))for j in range(n)];cg=rig.fk(rest)
   delta={i:(rest[i].to_quaternion()@reference[i].to_quaternion().inverted()).normalized()for i in range(2,27)if i not in(10,17)}if crouched else{}
   shifts=[cg[i].translation-rg[i].translation if crouched else Vector((0,0,0))for i in(21,25)];errors=[]
   def authored(t):
    source=sample(fs,t);g=rig.fk(source);p=copy_pose(source)
    for i,d in delta.items():
     translation=p[i].translation.copy();p[i]=(d@p[i].to_quaternion()).normalized().to_matrix().to_4x4();p[i].translation=translation
    if crouched:p[2].translation+=rest[2].translation-reference[2].translation
    for i in right:p[i]=rest[i].copy()
    p[17]=rest[17].copy()
    targets=[g[i].translation+shifts[k]for k,i in enumerate((21,25))]
    pin_feet(rig,p,targets,[cg[i].to_3x3()@rg[i].to_3x3().inverted()@g[i].to_3x3()if crouched else g[i].to_3x3()for i in(21,25)])
    actual=rig.fk(p);errors.append(max((actual[i].translation-targets[k]).length for k,i in enumerate((21,25))));return p
   poses=[authored(f/119)for f in range(120)];bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,duration/1000))
   for pose in poses+[poses[0]]:
    for matrix in pose:bank.extend(struct.pack('<7f',*row(matrix)))
   alias=f'native_bendtime_{phase}_{"crouched"if crouched else"standing"}_{"armed"if armed else"empty"}'
   L.save(rig,role,alias,poses,[identity()for _ in poses],'powers','Full-body Bend Time stance adaptation with common start reference across phases. Native lengths, corrective helpers, weapon attachments and feet preserved. No root motion or gameplay. Hold is offline only until observed; Emily offline rig comparison only.',[m['sha256']],native_duration_ms=duration,stance=int(crouched),armed=armed,max_foot_target_error=max(errors))
   if not armed:L.reports[-1]['equipment_policy']='hands_free';(L.out/(L.reports[-1]['id']+'.json')).write_text(json.dumps(L.reports[-1],indent=2),encoding='utf-8')
   assert max(errors)<.002,(role,cid,'Unreachable feet',max(errors))
   clips.append(dict(id=cid,variant=12+phaseIndex,alias=alias,crouched=crouched,armed=armed,max_foot_target_error=max(errors)))
 path=OUT/f'{role}-bendtime-context-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE39',clips=247,prior_payload_unchanged=bank[32:len(data)]==data[32:],sources=clips,visual_accepted=False,runtime_verified=False))
L.finish('bendtime-context-source-manifest-v41.json');(P/'bendtime-context-bank-manifest-v41.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('BENDTIME context banks generated')
