"""Reuse the original full-body combat crouch adaptation; append three clips."""
import sys,struct,json,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['library-retarget-manifest.json','stance-retarget-manifest.json'],'heavy-sources-v633')
O=P/'heavy-banks-v633';O.mkdir(exist_ok=False);reports=[]
for role in ('corvo','emily'):
 old=(P/'kick-banks-v625'/f'{role}-choke-bank.bin').read_bytes();magic,v,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',old);assert(magic,v,count)==(b'D2MOVE66',66,354)
 rig=rig_for(role);sm,sf=L.load(role,'sword_idle');cm,cf=L.load(role,'crouch_idle');stand=sf[0];crouch=cf[0];sg=rig.fk(stand);cg=rig.fk(crouch)
 shifts=[cg[i].translation-sg[i].translation for i in (21,25)]
 def lower_pose(source):
  p=copy_pose(source);g=rig.fk(source)
  for i in range(2,27):
   if i==17:continue
   weight=0 if i in (6,7,8,9,13,14,15,16) else .3 if i in (2,3,4,5,11,12) else 1
   delta=(crouch[i].to_quaternion()@stand[i].to_quaternion().inverted()).normalized()
   q=Quaternion().slerp(delta,weight)@p[i].to_quaternion();v=p[i].translation.copy();p[i]=q.normalized().to_matrix().to_4x4();p[i].translation=v
  p[2].translation+=crouch[2].translation-stand[2].translation
  return pin_feet(rig,p,[g[i].translation+shifts[k] for k,i in enumerate((21,25))],[g[i].to_3x3()for i in (21,25)])
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE67',67,n,357,fps,layout)+old[32:]);entry=24+121*n*28;length_error=0
 for old_id,new_id,duration in ((50,354,.266),(51,355,2.),(52,356,1. if role=='corvo' else .833)):
  poses=[];offset=32+old_id*entry+24
  for f in range(120):
   p=lower_pose([qts(struct.unpack_from('<7f',old,offset+(f*n+j)*28))for j in range(n)])
   length_error=max(length_error,max((p[i].translation-rig.bind[i].translation).length for i in range(3,27)if i!=17))
   pose=[row(m)for m in p];assert all(math.isfinite(x) for j in pose for x in j)
   assert max(abs(pose[i][k]-float(k==3))for i in (0,1)for k in range(7))<1e-6
   poses.append(pose)
  bank.extend(struct.pack('<IIIIff',new_id,121,0,0,0,duration))
  for pose in poses+[poses[0]]:
   for j in pose:bank.extend(struct.pack('<7f',*j))
 assert length_error<1e-5 and bank[32:len(old)]==old[32:]
 out=O/f'{role}-choke-bank.bin';out.write_bytes(bank)
 reports.append(dict(role=role,bank=str(out.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),version=67,clips=357,prior354PayloadsUnchanged=True,native_length_error=length_error,source_clips=[50,51,52],output_clips=[354,355,356],conversion='Existing full-body heavy clips with the established combat crouch stance delta and foot IK; native bone lengths and attachments retained. Native source scene clock controls phase. No first-person position constraints.',source_bank_sha256=hashlib.sha256(old).hexdigest(),root_writes=False,events=False,runtime_verified=False,visual_accepted=False,complete=False))
 print('Authored crouched heavy phases',role,flush=True)
(P/'heavy-bank-manifest-v633.json').write_text(json.dumps(reports,indent=2))
