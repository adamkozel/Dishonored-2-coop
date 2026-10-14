"""Append a full-body second-impulse study, leaving prior gameplay clips intact."""
import struct,json,hashlib,sys
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['jumps-retarget-manifest-v3.json'],'double-jump-sources-v1')
OUT=P/'double-jump-banks-v1';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'high-mantle-banks-v1'/f'{role}-high-mantle-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE20'and count==177
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE21',21,n,179,fps,layout)+data[32:]);entry=24+121*n*28
 m,poses=L.load(role,'double_jump');rig=rig_for(role)
 start=m['source']['authored_source']['apex_frame'];end=start+48;assert start==56 and m['frames']==190
 empty=[qts(struct.unpack_from('<7f',data,32+100*entry+24+(119*n+j)*28))for j in range(n)]
 right=[]
 for j in range(n):
  k=j
  while k>=0 and k!=13:k=rig.joints[k]['parent']
  if k==13:right.append(j)
 clips=[]
 for unarmed in (False,True):
  cid=177+int(unarmed);frames=[]
  for f in range(120):
   pose=sample(poses,(start+(end-start)*f/119)/(len(poses)-1))
   if unarmed:
    for j in right:pose[j]=empty[j].copy()
   preview_helpers(rig,pose)
   assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
   frames.append(pose)
  frames.append(frames[0]);bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,.8))
  for pose in frames:
   for matrix in pose:bank.extend(struct.pack('<7f',*row(matrix)))
  clips.append(dict(id=cid,source=m['binary'],sha256=m['sha256'],source_frames=[start,end],equipment='holstered'if unarmed else'drawn',purpose='Midair tuck and extension with whole-body counterbalance; excludes grounded takeoff and original offline root trajectory. Native second-impulse timing and visual transition remain to be validated.'))
 path=OUT/f'{role}-double-jump-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE21',clips=179,previous_payload_unchanged=bank[32:len(data)]==data[32:],sources=clips,root_writes=False,events=False,runtime_verified=False,visual_accepted=False))
(P/'double-jump-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2))
print('Double-jump bank candidates prepared; gameplay and visual validation pending')
