"""Separate Posture-100 idle comparison; do not promote until visually tested."""
import sys,struct,json,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['mixamo-retarget-manifest-v5.json'],'crouch-balance-sources-v1')
O=P/'crouch-balance-banks-v1';O.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 rig=rig_for(role);J=rig.joints;n=len(J);path=P/'windblast-context-banks-v1'/f'{role}-windblast-context-bank.bin';data=path.read_bytes();bank=bytearray(data);entry=24+121*n*28
 assert struct.unpack_from('<8sIIIIQ',data)[:4]==(b'D2MOVE28',28,n,201)
 m,walk=L.load(role,'mixamo_crouched_walking_posture100');walk=walk[:-1];globals_=[rig.fk(f)for f in walk]
 def previous(cid,f=0):return [qts(struct.unpack_from('<7f',data,32+cid*entry+24+(f*n+j)*28))for j in range(n)]
 mean=copy_pose(walk[0])
 for k in range(1,len(walk)):mean=blend(mean,walk[k],1/(k+1))
 # Average the whole locomotion stance, not a kneeling donor. Feet retain
 # their own mean lateral/forward spacing and contact the native floor.
 targets=[];orient=[];standG=rig.fk(previous(0))
 for ankle in (21,25):
  t=sum((g[ankle].translation for g in globals_),Vector())/len(globals_);t.z=standG[ankle].translation.z;targets.append(t)
  orient.append(rig.fk(mean)[ankle].to_3x3())
 poses=[];errors=[]
 for f in range(120):
  t=f/120;p=copy_pose(mean);wave=math.sin(t*2*math.pi);p[2].translation.z+=.003*wave
  for j,angle in ((3,.004),(4,.003),(5,-.003)):
   trans=p[j].translation.copy();p[j]=(p[j].to_quaternion()@Quaternion(Vector((0,0,1)),angle*wave)).to_matrix().to_4x4();p[j].translation=trans
  pin_feet(rig,p,targets,orient);g=rig.fk(p);errors.append(max((g[i].translation-targets[k]).length for k,i in enumerate((21,25))));poses.append(p)
 assert max(errors)<.002,(role,max(errors))
 modified=[]
 for cid,armed in ((17,True),(76,False)):
  frames=[copy_pose(p)for p in poses];empty=previous(76)
  if not armed:
   hand=[]
   for j in range(n):
    k=j
    while k>=0 and k!=16:k=J[k]['parent']
    if k==16 and j!=16 and 'helper'not in J[j]['nativeName']:hand.append(j)
   for p in frames:
    for j in hand:p[j]=empty[j].copy()
    preview_helpers(rig,p)
  for f,p in enumerate(frames+[frames[0]]):
   for j,v in enumerate(p):struct.pack_into('<7f',bank,32+cid*entry+24+(f*n+j)*28,*row(v))
  alias='balanced_posture100_'+('armed'if armed else'empty')+'_idle'
  # Source duration follows the existing runtime idle phase, 2.5 seconds.
  preview=[sample(frames+[frames[0]],f/150)for f in range(151)]
  L.save(rig,role,alias,preview,[identity()for _ in preview],'stance','Whole-body Posture-100 mean stance with small breathing and independently planted feet. Removes low kneeling-idle donor; no root, first-person or gameplay changes.',[m['sha256']],max_foot_error=max(errors),modified_bank_clip=cid)
  if not armed:L.reports[-1]['equipment_policy']='hands_free'
  modified.append(cid)
 out=O/f'{role}-crouch-balance-bank.bin';out.write_bytes(bank)
 unchanged=all(bank[32+cid*entry:32+(cid+1)*entry]==data[32+cid*entry:32+(cid+1)*entry]for cid in range(201)if cid not in modified)
 assert unchanged
 reports.append(dict(role=role,bank=str(out.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),parent_bank_sha256=hashlib.sha256(data).hexdigest(),modified_clips=modified,other_clips_unchanged=unchanged,maximum_foot_error=max(errors),pelvis_height=mean[2].translation.z,visual_accepted=False,context_rebake_pending=True))
L.finish('crouch-balance-source-manifest-v1.json');(P/'crouch-balance-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2))
