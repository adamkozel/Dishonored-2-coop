"""Reversible right-forearm/wrist correction for two low cast phases.
Preserves the moving wrist path, skeleton lengths, finger grip and socket.
The conservative blade-centreline screen must be followed by D2 visual review.
"""
import sys,struct,json,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['bendtime-balanced-source-manifest-v42.json'],'crouch-power-clearance-v49')
OUT=P/'crouch-power-clearance-banks-v49';OUT.mkdir(exist_ok=True);reports=[]
def distance(p,a,b):
 v=b-a;t=max(0,min(1,(p-a).dot(v)/max(v.length_squared,1e-8)));return(p-a-v*t).length
for role in ('corvo','emily'):
 data=(P/'outofmana-context-banks-v46'/f'{role}-outofmana-context-bank.bin').read_bytes();bank=bytearray(data)
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert version==41 and count==251
 size=24+121*n*28;rig=rig_for(role);axis=rig.bind[16].translation.normalized();clips=[]
 q=read(P.parent/('fullbody-proof/inputs/client-target-sword.json'if role=='corvo'else'crouch-grip-reference/host-target-sword.json'))['quaternion']
 binding=Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix().to_4x4()
 for cid,phase in ((239,'in'),(245,'out')):
  fs=[[qts(struct.unpack_from('<7f',data,32+cid*size+24+(f*n+j)*28))for j in range(n)]for f in range(120)]
  def offset(p,t,roll,bend):
   weight=ease(t/.10)*(1-ease((t-.95)/.05));out=copy_pose(p)
   for j,rotation in ((15,Quaternion(axis,math.radians(roll*weight)).to_matrix().to_4x4()),(16,Matrix.Rotation(math.radians(bend*weight),4,'Z'))):
    out[j]=p[j]@rotation;out[j].translation=p[j].translation
   return out
  def measure(roll,bend,step=6):
   low=10;head=10;torso=10
   for f in range(0,120,step):
    g=rig.fk(offset(fs[f],f/119,roll,bend));weapon=g[17]@binding
    for x in (-.1,0,.15,.3,.45,.6,.75):
     point=weapon@Vector((x,0,0));low=min(low,point.z);head=min(head,(point-g[12].translation).length-.16);torso=min(torso,distance(point,g[3].translation,g[5].translation)-.15)
   score=100*max(0,.08-low)**2+80*max(0,.05-head)**2+80*max(0,.03-torso)**2+.000001*(roll*roll+2*bend*bend)
   return dict(roll=roll,bend=bend,min_blade_z=low,head_clearance=head,torso_clearance=torso,score=score)
  candidates=[measure(r,b)for r in range(-60,61,15)for b in range(-45,46,15)];best=min(candidates,key=lambda x:x['score']);dense=measure(best['roll'],best['bend'],1)
  print('CLEARANCE_CANDIDATE',role,cid,'baseline',measure(0,0),'chosen',dense,flush=True)
  poses=[];wrist=0
  for f,p in enumerate(fs):
   out=offset(p,f/119,best['roll'],best['bend']);wrist=max(wrist,(rig.fk(p)[16].translation-rig.fk(out)[16].translation).length);preview_helpers(rig,out);poses.append(out)
  assert wrist<1e-5
  for f,pose in enumerate(poses+[poses[0]]):
   for j,matrix in enumerate(pose):struct.pack_into('<7f',bank,32+cid*size+24+(f*n+j)*28,*row(matrix))
  alias=f'native_bendtime_clearance_{phase}_crouched_armed';meta=next(m for m in L.rows if m['role']==role and m['selection']['alias']==f'native_bendtime_balanced_{phase}_crouched_armed')
  L.save(rig,role,alias,poses,[identity()for _ in poses],'powers','Bounded local forearm pronation and wrist tilt during native-timed crouched Bend Time. Moving wrist position, lengths, grip, socket and feet preserved. Conservative blade centreline screen only; requires actual-game comparison. Emily remains offline reference.',dict(previous_sha256=meta['sha256'],correction=dense),native_duration_ms=533 if phase=='in' else 433,wrist_path_error=wrist)
  clips.append(dict(clip=cid,alias=alias,baseline=measure(0,0,1),chosen=dense,candidates=candidates,wrist_path_error=wrist))
 untouched=all(bank[32+i*size:32+(i+1)*size]==data[32+i*size:32+(i+1)*size]for i in range(count)if i not in(239,245));assert untouched
 path=OUT/f'{role}-crouch-power-clearance-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE41',clips=251,changed_clips=[239,245],other_payloads_unchanged=untouched,screening=clips,visual_accepted=False,runtime_verified=False))
L.finish('crouch-power-clearance-source-manifest-v49.json');(P/'crouch-power-clearance-bank-manifest-v49.json').write_text(json.dumps(reports,indent=2))
