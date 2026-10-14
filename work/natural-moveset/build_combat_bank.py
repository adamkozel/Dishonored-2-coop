"""Scene-timed, pose-only combat candidates appended to the verified v7 bank."""
import sys,struct,json,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['library-retarget-manifest.json','stance-retarget-manifest.json','crouch-actions-retarget-manifest-v2.json','guard-sequences-manifest-v1.json'],'combat-bank-sources-v1')
OUT=P/'combat-banks-v1';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'air-banks-v1'/f'{role}-air-bank.bin').read_bytes();magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE07' and count==44
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE08',8,n,59,fps,layout)+data[32:]);sources=[];rig=rig_for(role)
 sm,sf=L.load(role,'sword_idle');cm,cf=L.load(role,'crouch_idle');stand=sf[0];crouch=cf[0];sg=rig.fk(stand);cg=rig.fk(crouch)
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
 requests=[(44,'sword_attack_fasta',0,1,False),(45,'sword_attack_fastb',0,1,False),(46,'sword_attack_shorta',0,1,False),(47,'sword_attack_shortb',0,1,False),
  (48,'crouch_attack_fast_a',0,1,False),(49,'crouch_attack_fast_b',0,1,False),
  (50,'sword_attack_longa',0,.28,False),(51,'sword_attack_longa',.28,.28,False),(52,'sword_attack_longa',.28,1,False)]
 gm,gf=L.load(role,'guard_ll_hold_sequence');marks={m['phase']:m['at']/gm['duration']for m in gm['phase_marks']}
 for offset,low in ((0,False),(3,True)):
  requests +=[(53+offset,'guard_ll_hold_sequence',0,marks['guard_hold'],low),(54+offset,'guard_ll_hold_sequence',marks['guard_hold'],marks['native_recovery']-1/60/gm['duration'],low),(55+offset,'guard_ll_hold_sequence',marks['native_recovery'],1,low)]
 for cid,alias,lo,hi,low in requests:
  m,fs=L.load(role,alias);poses=[]
  for f in range(120):
   u=lo+(hi-lo)*f/119;p=sample(fs,u)
   if low:p=lower_pose(p)
   # Authored helper corrections run in D2 after this local pose is applied.
   poses.append([row(v)for v in p])
  poses.append(poses[0]);duration=max(.05,(hi-lo)*m['duration'])
  bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,duration))
  for pose in poses:
   for v in pose:bank.extend(struct.pack('<7f',*v))
  sources.append(dict(id=cid,alias=alias,source=m['binary'],sha256=m['sha256'],range=[lo,hi],crouch_adaptation=low))
 path=OUT/f'{role}-combat-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE08',previous_payload_unchanged=bank[32:len(data)]==data[32:],sources=sources,root_writes=False,events=False,gameplay_verified=False,visual_accepted=False,limitations=['Attack-direction mapping is a candidate pending actual-game comparison.','Heavy hold uses a static authored windup pose; breathing pass pending.','Crouched guard uses whole-body stance adaptation and foot IK; visual review required.','Airborne combat, hit reactions, directional parry outcomes and victim contact remain separate work.']))
(P/'combat-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2));print('COMBAT_BANKS prepared; gameplay and visual validation pending',flush=True)
