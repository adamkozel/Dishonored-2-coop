"""Whole-body stance/equipment adaptations, preserving native lengths and feet."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['windblast-source-manifest-v1.json'],'windblast-contexts-v1')
OUT=P/'windblast-context-banks-v1';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'windblast-banks-v1'/f'{role}-windblast-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE27'and count==198
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE28',28,n,201,fps,layout)+data[32:]);entry=24+121*n*28
 rig=rig_for(role);m,fs=L.load(role,'native_phase_windblast_fire');reference=fs[0];rg=rig.fk(reference)
 right=[]
 for i in range(n):
  k=i
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and i!=16 and 'helper'not in rig.joints[i]['nativeName']:right.append(i)
 clips=[]
 for cid,restid,crouched,armed in ((198,59,False,False),(199,17,True,True),(200,76,True,False)):
  rest=[qts(struct.unpack_from('<7f',data,32+restid*entry+24+j*28))for j in range(n)];cg=rig.fk(rest)
  delta={i:(rest[i].to_quaternion()@reference[i].to_quaternion().inverted()).normalized()for i in range(2,27)if i not in (10,17)}
  shifts=[cg[i].translation-rg[i].translation for i in (21,25)];errors=[]
  if not crouched:
   delta={};shifts=[Vector((0,0,0)),Vector((0,0,0))]
  def authored(t):
   source=sample(fs,t);g=rig.fk(source);p=copy_pose(source)
   # Offset the complete primary body, including the spine, shoulders and
   # arms. Do not paste standing upper-body motion onto unrelated crouched legs.
   for i,d in delta.items():
    translation=p[i].translation.copy();p[i]=(d@p[i].to_quaternion()).normalized().to_matrix().to_4x4();p[i].translation=translation
   if crouched:p[2].translation+=rest[2].translation-reference[2].translation
   for i in right:p[i]=rest[i].copy()
   p[17]=rest[17].copy()
   targets=[g[i].translation+shifts[k]for k,i in enumerate((21,25))]
   pin_feet(rig,p,targets,[cg[i].to_3x3()@rg[i].to_3x3().inverted()@g[i].to_3x3() if crouched else g[i].to_3x3() for i in (21,25)])
   actual=rig.fk(p);errors.append(max((actual[i].translation-targets[k]).length for k,i in enumerate((21,25))))
   return p
  poses=[authored(f/119)for f in range(120)];bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,1.0))
  for pose in poses+[poses[0]]:
   for matrix in pose:bank.extend(struct.pack('<7f',*row(matrix)))
  alias=f'native_windblast_{"crouched"if crouched else"standing"}_{"armed"if armed else"empty"}'
  preview=[authored(f/60)for f in range(61)]
  L.save(rig,role,alias,preview,[identity()for _ in preview],'powers','Complete-body stance/equipment offset of the authored native-clock Windblast cast. Preserves native segment lengths, source foot trajectories and native right-hand grip; native source owns world movement and gameplay outcomes. Corvo gameplay availability only; Emily is an offline comparison. Visual/context validation pending.',[m['sha256']],native_duration_ms=1000,stance=int(crouched),armed=armed,max_foot_target_error=max(errors))
  if not armed:L.reports[-1]['equipment_policy']='hands_free';(L.out/(L.reports[-1]['id']+'.json')).write_text(json.dumps(L.reports[-1],indent=2),encoding='utf-8')
  assert max(errors)<.002, (role,cid,'Unreachable foot trajectory',max(errors))
  clips.append(dict(id=cid,alias=alias,crouched=crouched,armed=armed,max_foot_target_error=max(errors)))
 path=OUT/f'{role}-windblast-context-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE28',clips=201,prior_payload_unchanged=bank[32:len(data)]==data[32:],sources=clips,visual_accepted=False,runtime_verified=False))
L.finish('windblast-context-source-manifest-v1.json');(P/'windblast-context-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
