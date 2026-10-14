"""Keep a native low sword guard while the other arm and body cast freely.
Actor-space rotations act on the native arm chain; no wrist position target.
"""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['bendtime-balanced-source-manifest-v42.json'],'crouch-power-guard-v50')
OUT=P/'crouch-power-guard-banks-v50';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'outofmana-context-banks-v46'/f'{role}-outofmana-context-bank.bin').read_bytes();bank=bytearray(data)
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert version==41 and count==251
 size=24+121*n*28;rig=rig_for(role);clips=[]
 rest=[qts(struct.unpack_from('<7f',data,32+17*size+24+j*28))for j in range(n)];rg=rig.fk(rest)
 for cid,phase in ((239,'in'),(245,'out')):
  fs=[[qts(struct.unpack_from('<7f',data,32+cid*size+24+(f*n+j)*28))for j in range(n)]for f in range(120)];poses=[];max_move=0;grip_error=0
  for f,p in enumerate(fs):
   t=f/119;weight=.88*(ease(t/.25) if phase=='in' else 1-ease(t))
   out=copy_pose(p);g=rig.fk(p)
   for j in (14,15,16):rig.orient(out,j,g[j].to_quaternion().slerp(rg[j].to_quaternion(),weight).to_matrix())
   # The already closed hand and its socket remain the native crouch grip.
   preview_helpers(rig,out);actual=rig.fk(out);max_move=max(max_move,(actual[16].translation-g[16].translation).length)
   poses.append(out)
  for f,pose in enumerate(poses+[poses[0]]):
   for j,matrix in enumerate(pose):struct.pack_into('<7f',bank,32+cid*size+24+(f*n+j)*28,*row(matrix))
  alias=f'native_bendtime_guard_{phase}_crouched_armed';meta=next(m for m in L.rows if m['role']==role and m['selection']['alias']==f'native_bendtime_balanced_{phase}_crouched_armed')
  L.save(rig,role,alias,poses,[identity()for _ in poses],'powers','Native crouched sword guard supplies most right arm orientation during the left-hand cast. Remaining donor rotation and the moving shoulder preserve body response. Native lengths, closed finger grip and weapon socket preserved; wrist position follows the arm, not first-person anchors. Visual candidate only.',dict(previous_sha256=meta['sha256'],native_guard_clip=17,guard_weight=.88),native_duration_ms=533 if phase=='in' else 433,max_wrist_displacement=max_move)
  clips.append(dict(clip=cid,alias=alias,max_wrist_displacement=max_move))
 assert all(bank[32+i*size:32+(i+1)*size]==data[32+i*size:32+(i+1)*size]for i in range(count)if i not in(239,245))
 path=OUT/f'{role}-crouch-power-guard-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE41',clips=251,changed_clips=[239,245],other_payloads_unchanged=True,sources=clips,visual_accepted=False,runtime_verified=False))
L.finish('crouch-power-guard-source-manifest-v50.json');(P/'crouch-power-guard-bank-manifest-v50.json').write_text(json.dumps(reports,indent=2))
