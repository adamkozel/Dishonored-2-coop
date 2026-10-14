"""Complete-body non-damaging CatFall recovery; native scene owns its timing."""
import sys,struct,json,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['mixamo-retarget-manifest-v5.json'],'hard-landing-sources-v1')
OUT=P/'hard-landing-banks-v1';OUT.mkdir(exist_ok=True);reports=[]
def donor_time(t):
 if t<.18:return .17+(.45-.17)*t/.18
 if t<.26:return .45+(1.-.45)*(t-.18)/.08
 return 1.+(.8)*(t-.26)/.74
for role in ('corvo','emily'):
 data=(P/'double-jump-banks-v1'/f'{role}-double-jump-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE21'and count==179
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE22',22,n,181,fps,layout)+data[32:]);entry=24+121*n*28
 m,donor=L.load(role,'mixamo_hard_landing');assert m['frames']==61 and m['fps']==30
 rig=rig_for(role);hand=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and 'helper'not in rig.joints[j]['nativeName']:hand.append(j)
 clips=[]
 for unarmed in (False,True):
  cid=179+int(unarmed);restId=59 if unarmed else 0
  rest=[qts(struct.unpack_from('<7f',data,32+restId*entry+24+j*28))for j in range(n)]
  def authored(t):
   pose=sample(donor,donor_time(t)/2.)
   if not unarmed:
    # Keep the sword clear of the ground with a dynamic right-arm gesture.
    # Left support, trunk, pelvis and legs remain the authored full-body donor.
    # The target follows this body's donor wrist, never first-person transforms.
    g=rig.fk(pose);contact=ease(t/.10)*(1-ease((t-.40)/.30))
    goal=g[16].translation+Vector((-.04,-.15,.32))*contact
    rig.solve(pose,14,15,16,goal,g[15].translation+Vector((0,-.12,0))*contact)
    for j in hand:pose[j]=rest[j].copy()
   pose=blend(pose,rest,ease((t-.78)/.22))
   preview_helpers(rig,pose)
   assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
   return pose
  poses=[authored(f/119)for f in range(120)];poses.append(poses[0]);bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,1.))
  for pose in poses:
   for matrix in pose:bank.extend(struct.pack('<7f',*row(matrix)))
  alias=('empty_'if unarmed else'armed_')+'native_hard_landing'
  L.save(rig,role,alias,[authored(f/60)for f in range(61)],[identity()for _ in range(61)],'jump','One-second native CatFall light recovery, complete body with sword-clearance adaptation.',[m['sha256']],native_scene_duration_ms=1000,donor_binary=m['binary'],donor_time_keys=[[0,.17],[.18,.45],[.26,1],[1,1.8]])
  L.reports[-1]['equipment_policy']='hands_free'if unarmed else'sword_ready'
  (L.out/(L.reports[-1]['id']+'.json')).write_text(json.dumps(L.reports[-1],indent=2),encoding='utf-8')
  clips.append(dict(id=cid,alias=alias,equipment='holstered'if unarmed else'drawn',source=m['binary'],sha256=m['sha256']))
 path=OUT/f'{role}-hard-landing-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE22',clips=181,prior_payload_unchanged=bank[32:len(data)]==data[32:],sources=clips,events=False,root_writes=False,visual_accepted=False,runtime_verified=False,limitations=['Only native non-damaging CatFall light, standing recovery. Strong damage and crouched exits remain unverified.']))
L.finish('hard-landing-source-manifest-v1.json');(P/'hard-landing-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
