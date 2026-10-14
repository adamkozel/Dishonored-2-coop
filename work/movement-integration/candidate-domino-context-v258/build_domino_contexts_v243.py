"""Whole-body Domino stance candidates, preserving the v192 standing payload.

The first-person source supplies phase timing, not fixed body-space wrists.
New contexts remain offline candidates until their native runtime cases pass.
"""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['domino-bank-source-manifest-v192.json'],'domino-contexts-v243')
OUT=P/'domino-context-banks-v243';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'domino-banks-v192'/f'{role}-domino-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
 assert magic==b'D2MOVE47'and version==47 and count==289
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE48',48,n,301,fps,layout)+data[32:]);entry=24+121*n*28
 rig=rig_for(role);_,initial=L.load(role,'native_domino_entry');reference=initial[0];refG=rig.fk(reference);right=[];clips=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and j!=16 and 'helper'not in rig.joints[j]['nativeName']:right.append(j)
 for phaseIndex,(phase,duration)in enumerate([('entry',266),('aim',1333),('select',400),('exit',266)]):
  m,fs=L.load(role,'native_domino_'+phase)
  for context,(restid,crouched,armed)in enumerate(((59,False,False),(17,True,True),(76,True,False))):
   cid=289+phaseIndex*3+context
   rest=[qts(struct.unpack_from('<7f',data,32+restid*entry+24+j*28))for j in range(n)];restG=rig.fk(rest);errors=[];heights=[]
   def authored(t):
    source=sample(fs,t);sourceG=rig.fk(source);p=copy_pose(source)
    for j in range(2,27):
     if j in(10,17):continue
     if crouched or j in(14,15,16):
      gain=.55 if crouched and j in(2,18)else .8 if crouched and j in(3,4,5)else 1.
      delta=reference[j].to_quaternion().inverted()@source[j].to_quaternion()
      translation=p[j].translation.copy();p[j]=(rest[j].to_quaternion()@Quaternion().slerp(delta,gain)).normalized().to_matrix().to_4x4();p[j].translation=translation
    if crouched:p[2].translation=rest[2].translation+(source[2].translation-reference[2].translation)*.4
    for j in right:p[j]=rest[j].copy()
    p[17]=rest[17].copy()
    if crouched:
     weight=ease(t/.25)if phase=='entry'else 1-ease(t)if phase=='exit'else 1.
     # Only the casting arm follows the changing donor directions. The sword
     # arm keeps the authored crouch guard, fingers and socket relationship.
     for j in(7,8,9):
      g=rig.fk(p);rig.orient(p,j,g[j].to_quaternion().slerp(sourceG[j].to_quaternion(),weight).to_matrix())
    targets=[restG[j].translation+(sourceG[j].translation-refG[j].translation)*.4 if crouched else sourceG[j].translation for j in(21,25)]
    rotations=[restG[j].to_3x3()@refG[j].to_3x3().inverted()@sourceG[j].to_3x3()if crouched else sourceG[j].to_3x3()for j in(21,25)]
    pin_feet(rig,p,targets,rotations);g=rig.fk(p)
    errors.append(max((g[j].translation-targets[k]).length for k,j in enumerate((21,25))));heights.append(p[2].translation.z)
    return p
   poses=[authored(f/119)for f in range(120)]
   assert max(errors)<.002,(role,cid,'Foot support unreachable',max(errors))
   assert not crouched or min(heights)>.5,(role,cid,'Pelvis compression',min(heights))
   bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,duration/1000))
   for pose in poses+[poses[0]]:
    for matrix in pose:bank.extend(struct.pack('<7f',*row(matrix)))
   alias=f'native_domino_{phase}_{"crouched"if crouched else"standing"}_{"armed"if armed else"empty"}'
   L.save(rig,role,alias,poses,[identity()for _ in poses],'powers','Full-body stance acting from the v192 whole-body donor with a common phase reference, animated casting-arm directions, balanced Posture-100 crouch support and native sword grip/socket. No first-person wrist anchors, root tracks, gameplay or effect events. New contexts are unverified candidates; Corvo is offline comparison only.',[m['sha256']],native_duration_ms=duration,stance=int(crouched),armed=armed,max_foot_target_error=max(errors),minimum_pelvis_height=min(heights))
   if not armed:L.reports[-1]['equipment_policy']='hands_free';(L.out/(L.reports[-1]['id']+'.json')).write_text(json.dumps(L.reports[-1],indent=2),encoding='utf-8')
   clips.append(dict(id=cid,variant=19+phaseIndex,alias=alias,crouched=crouched,armed=armed,max_foot_target_error=max(errors)))
 path=OUT/f'{role}-domino-context-bank.bin';path.write_bytes(bank)
 assert bank[32:len(data)]==data[32:]
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE48',clips=301,prior_payload_unchanged=True,sources=clips,visual_accepted=False,runtime_verified=False))
L.finish('domino-context-source-manifest-v243.json');(P/'domino-context-bank-manifest-v243.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('DOMINO_CONTEXTS_GENERATED',flush=True)
