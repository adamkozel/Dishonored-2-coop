"""Standing/low, armed/empty failure contexts; no new root or event authority."""
import sys,struct,json,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['outofmana-source-manifest-v45.json'],'outofmana-contexts-v46')
OUT=P/'outofmana-context-banks-v46';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'outofmana-banks-v45'/f'{role}-outofmana-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE40' and count==248
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE41',41,n,251,fps,layout)+data[32:]);entry=24+121*n*28
 rig=rig_for(role);meta,fs=L.load(role,'native_out_of_mana_standing_armed');reference=fs[0];rg=rig.fk(reference);right=[];sources=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and j!=16 and 'helper'not in rig.joints[j]['nativeName']:right.append(j)
 for index,(restid,low,armed)in enumerate(((59,False,False),(17,True,True),(76,True,False))):
  rest=[qts(struct.unpack_from('<7f',data,32+restid*entry+24+j*28))for j in range(n)];cg=rig.fk(rest);deltas={j:(rest[j].to_quaternion()@reference[j].to_quaternion().inverted()).normalized()for j in range(2,27)if j not in(10,17)}
  poses=[];errors=[]
  for frame in range(120):
   t=frame/119;weight=math.sin(math.pi*t)**.8;source=sample(fs,t);g=rig.fk(source);pose=copy_pose(source)
   for j,d in deltas.items():
    translation=pose[j].translation.copy();pose[j]=(d@pose[j].to_quaternion()).normalized().to_matrix().to_4x4();pose[j].translation=translation
   pose[2].translation+=rest[2].translation-reference[2].translation
   # Whole stance follows its native rest. The moving arm directions gently
   # reuse the failure donor without doubling the crouched torso angle.
   for j in(7,8,9,14,15,16):
    current=rig.fk(pose);rig.orient(pose,j,current[j].to_quaternion().slerp(g[j].to_quaternion(),weight).to_matrix())
   for j in right:pose[j]=rest[j].copy()
   pose[17]=rest[17].copy()
   targets=[cg[j].translation+(g[j].translation-rg[j].translation)for j in(21,25)]
   pin_feet(rig,pose,targets,[cg[j].to_3x3()@rg[j].to_3x3().inverted()@g[j].to_3x3()for j in(21,25)])
   preview_helpers(rig,pose);actual=rig.fk(pose);errors.append(max((actual[j].translation-targets[k]).length for k,j in enumerate((21,25))));poses.append(pose)
  assert max(errors)<.002
  cid=248+index;bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,.333))
  for pose in poses+[poses[0]]:
   for matrix in pose:bank.extend(struct.pack('<7f',*row(matrix)))
  alias=f'native_out_of_mana_{"crouched"if low else"standing"}_{"armed"if armed else"empty"}'
  L.save(rig,role,alias,poses,[identity()for _ in poses],'powers','Native-length whole-body failed-cast stance adaptation. Moving actor-space arm rotations, native corrective bones, right grip/attachment and feet retained. No root, gameplay or damage. Corvo native evidence required per context; Emily offline rig candidate only.',[meta['sha256']],native_duration_ms=333,crouched=low,armed=armed,max_foot_target_error=max(errors))
  sources.append(dict(clip=cid,alias=alias,crouched=low,armed=armed))
 path=OUT/f'{role}-outofmana-context-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE41',clips=251,prior_payload_unchanged=bank[32:len(data)]==data[32:],sources=sources,visual_accepted=False,runtime_verified=False))
L.finish('outofmana-context-source-manifest-v46.json');(P/'outofmana-context-bank-manifest-v46.json').write_text(json.dumps(reports,indent=2))
print('FAILURE_CONTEXTS',len(reports))
