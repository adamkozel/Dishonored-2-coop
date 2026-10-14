"""Reauthor crouched power weight transfer around the balanced full-body idle."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['windblast-source-manifest-v1.json','darkvision-source-manifest-v1.json'],'balanced-crouch-powers-v1')
O=P/'balanced-crouch-power-banks-v1';O.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 rig=rig_for(role);n=len(rig.joints);data=(P/'crouch-balance-banks-v1'/f'{role}-crouch-balance-bank.bin').read_bytes();bank=bytearray(data);entry=24+121*n*28;modified=[]
 def previous(cid):return [qts(struct.unpack_from('<7f',data,32+cid*entry+24+j*28))for j in range(n)]
 right=[]
 for i in range(n):
  k=i
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and i!=16 and 'helper'not in rig.joints[i]['nativeName']:right.append(i)
 for alias,start,duration in (('native_phase_dark_vision_fire',195,933),('native_phase_windblast_fire',199,1000)):
  m,fs=L.load(role,alias);ref=fs[0];refG=rig.fk(ref);wind=start==199
  for armed in (True,False):
   cid=start+int(not armed);rest=previous(17 if armed else 76);restG=rig.fk(rest);errors=[];heights=[];poses=[]
   def authored(t):
    source=sample(fs,t);sourceG=rig.fk(source);p=copy_pose(source)
    # Keep full-body acting, attenuating the standing lunge's compression
    # and leg spread to avoid kneeling during a crouched power release.
    for j in range(2,27):
     if j in (10,17):continue
     gain=.50 if wind and j in (2,18)else .82 if wind and j in (3,4,5)else 1.
     delta=ref[j].to_quaternion().inverted()@source[j].to_quaternion();trans=p[j].translation.copy()
     p[j]=(rest[j].to_quaternion()@Quaternion().slerp(delta,gain)).normalized().to_matrix().to_4x4();p[j].translation=trans
    offset=source[2].translation-ref[2].translation
    if wind:offset*=.30
    p[2].translation=rest[2].translation+offset
    for j in right:p[j]=rest[j].copy()
    p[17]=rest[17].copy();targets=[];rotations=[]
    for ankle in (21,25):
     d=sourceG[ankle].translation-refG[ankle].translation
     if wind:d*=.25
     targets.append(restG[ankle].translation+d)
     rotations.append(restG[ankle].to_3x3() if wind else restG[ankle].to_3x3()@refG[ankle].to_3x3().inverted()@sourceG[ankle].to_3x3())
    pin_feet(rig,p,targets,rotations);g=rig.fk(p);errors.append(max((g[i].translation-targets[k]).length for k,i in enumerate((21,25))));heights.append(p[2].translation.z);return p
   frames=[authored(f/119)for f in range(120)];assert max(errors)<.002,(role,cid,max(errors));assert min(heights)>.5
   for f,p in enumerate(frames+[frames[0]]):
    for j,v in enumerate(p):struct.pack_into('<7f',bank,32+cid*entry+24+(f*n+j)*28,*row(v))
   key='balanced_'+('windblast'if wind else'darkvision')+'_crouched_'+('armed'if armed else'empty');preview=[authored(f/60)for f in range(61)]
   L.save(rig,role,key,preview,[identity()for _ in preview],'powers','Whole-body crouched cast adapted around balanced Posture-100 idle; standing lunge compression and footprint reduced for crouched balance. Native source owns root, clock, damage and effects.',[m['sha256']],native_duration_ms=duration,max_foot_error=max(errors),minimum_pelvis_height=min(heights))
   if not armed:L.reports[-1]['equipment_policy']='hands_free'
   modified.append(cid)
 out=O/f'{role}-balanced-crouch-power-bank.bin';out.write_bytes(bank)
 assert all(bank[32+cid*entry:32+(cid+1)*entry]==data[32+cid*entry:32+(cid+1)*entry]for cid in range(201)if cid not in modified)
 reports.append(dict(role=role,bank=str(out.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),parent=hashlib.sha256(data).hexdigest(),modified_clips=modified,other_clips_unchanged=True,visual_accepted=False))
L.finish('balanced-crouch-power-source-manifest-v1.json');(P/'balanced-crouch-power-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2))
