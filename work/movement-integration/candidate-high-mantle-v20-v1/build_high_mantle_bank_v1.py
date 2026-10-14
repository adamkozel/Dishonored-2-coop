"""Native full-body high climb retimed to observed D2 scenes; no root/events."""
import sys,struct,json,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['library-retarget-manifest.json'],'high-mantle-sources-v1')
OUT=P/'high-mantle-banks-v1';OUT.mkdir(exist_ok=True)
reports=[]

def curve(role,crouched):
 path=P/f'native-high-mantle-{role}-v3/states.json';trial='high-crouched-a' if crouched else 'high-standing-a'
 points={};height=span=None
 for r in read(path):
  if r['trial']!=trial:continue
  active=next((a for a in r['state']['active']if a['id']==8),None)
  if not active:continue
  a=active['mantleResearch'];assert a['kindd8']==2 and a['nextb8']==(6 if crouched else 0)
  scene=next((v for v in r['midnight']['instances']if v['id']==a['scene24']and v['state']==3),None)
  if scene is None:continue # Native scene retired between the two read-only probes.
  assert scene['sceneDurationMs']==833 and scene['subscene']==('Scene_2'if role=='corvo'else'Scene_1')
  delta=Vector(a['goalBc'])-Vector(a['startE8']);horizontal=Vector((delta.x,delta.y,0));height=delta.z;span=horizontal.length
  pos=Vector(r['state']['position'])-Vector(a['startE8']);t=scene['clockWords'][0]/833
  if 0<t<1:points[t]=Vector((pos.dot(horizontal.normalized()),0,pos.z))
 assert len(points)>=6 and 1.7<height<2.2 and .8<span<1.4
 points={0:Vector((0,0,0)),**points,1:Vector((span,0,height))};pairs=sorted(points.items())
 def at(t):
  for (a,pa),(b,pb) in zip(pairs,pairs[1:]):
   if a<=t<=b:return pa.lerp(pb,(t-a)/(b-a))
  raise ValueError(t)
 return at,height,span,str(path.relative_to(P)),trial

for role in ('corvo','emily'):
 data=(P/'crouch-turn-banks-v1'/f'{role}-crouch-turn-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE19' and count==173
 entry=24+121*n*28;bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE20',20,n,177,fps,layout)+data[32:])
 rig=rig_for(role);m,donor=L.load(role,'sword_mantlemiddle1_walkleft_in');assert m['frames']==46 and m['fps']==30
 roots=[Vector(v[4:7])for v in m['root_track']]
 startRoot=roots[18];endRoot=roots[45]
 def previous(cid):return[qts(struct.unpack_from('<7f',data,32+cid*entry+24+j*28))for j in range(n)]
 hand=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and 'helper'not in rig.joints[j]['nativeName']:hand.append(j)
 clips=[]
 for crouched in (False,True):
  trajectory,height,span,source,trial=curve(role,crouched)
  for unarmed in (False,True):
   cid=173+2*int(crouched)+int(unarmed);rest=previous((76 if crouched else 59)if unarmed else(17 if crouched else 0));restG=rig.fk(rest)
   palmErrors=[];balanceMax=[0.];offsetMax=[0.]
   def authored(t,measure=False):
    pos=trajectory(t);frame=18+27*t;pose=sample(donor,frame/45)
    if unarmed:
     for j in hand:pose[j]=rest[j].copy()
    # Preserve the native donor's weight transfer despite the different native
    # player trajectory. Only the visual pelvis moves; joint lengths stay fixed.
    f=int(frame);r=roots[f].lerp(roots[min(45,f+1)],frame-f)
    desired=Vector(((r.x-startRoot.x)/(endRoot.x-startRoot.x)*span,0,(r.z-startRoot.z)/(endRoot.z-startRoot.z)*height))
    offset=desired-pos;pose[2].translation+=offset
    if measure:offsetMax[0]=max(offsetMax[0],offset.length)
    recovery=ease((t-.70)/.30);pose=blend(pose,rest,recovery)
    g=rig.fk(pose)
    # Finite left-hand support, independent of all first-person transforms.
    contact=ease(t/.10)*(1-ease((t-.58)/.15))
    target=Vector((span-.22,.31 if role=='corvo'else .28,height+.055))-pos
    if contact:
     shoulder=g[7].translation;reach=rig.bind[8].translation.length+rig.bind[9].translation.length-.02
     correction=(target-shoulder).normalized()*min(.23,max(0,(target-shoulder).length-reach))*contact
     pose[2].translation+=correction;g=rig.fk(pose)
     rotation=g[9].to_3x3().copy();rig.solve(pose,7,8,9,g[9].translation.lerp(target,contact),g[8].translation);rig.orient(pose,9,rotation)
     if measure:
      balanceMax[0]=max(balanceMax[0],correction.length)
      if contact>.99:palmErrors.append((rig.fk(pose)[9].translation-target).length)
    # Land on the ledge progressively, retaining the donor's swing and tuck.
    g=rig.fk(pose)
    for j,a,b,begin,end in ((25,23,24,.54,.88),(21,19,20,.68,1.0)):
     amount=max(0,min(1,(t-begin)/(end-begin)));weight=ease(amount)
     target=restG[j].translation+Vector((span,0,height))-pos
     target.z+=.09*math.sin(math.pi*amount)
     rotation=g[j].to_quaternion().slerp(restG[j].to_quaternion(),weight).to_matrix()
     rig.solve(pose,a,b,j,g[j].translation.lerp(target,weight),g[b].translation);rig.orient(pose,j,rotation)
    preview_helpers(rig,pose)
    assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
    return pose
   poses=[authored(f/119,True)for f in range(120)];poses.append(poses[0]);bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,.833))
   for pose in poses:
    for matrix in pose:bank.extend(struct.pack('<7f',*row(matrix)))
   alias=('empty_'if unarmed else'armed_')+'native_high_mantle_'+('crouched'if crouched else'standing')
   preview=[authored(f/50)for f in range(51)];previewRoots=[identity(*trajectory(f/50))for f in range(51)]
   L.save(rig,role,alias,preview,previewRoots,'traversal','Native complete-body high climb, finite ledge support and native stance recovery; observed 833 ms gameplay clock.',[m['sha256']],native_scene_duration_ms=833,donor_frames=[18,45],trajectory_source=source,trajectory_trial=trial,canonical_height=height,canonical_span=span,equipment_policy_override='hands_free'if unarmed else'sword_ready',contact_max_error=max(palmErrors,default=0))
   L.reports[-1]['equipment_policy']='hands_free'if unarmed else'sword_ready'
   clips.append(dict(id=cid,alias=alias,canonical_height=height,canonical_span=span,contact_max_error=max(palmErrors,default=0),balance_max=balanceMax[0],visual_pelvis_offset_max=offsetMax[0],trajectory_source=source,trial=trial))
 path=OUT/f'{role}-high-mantle-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE20',clips=177,prior_payload_unchanged=bank[32:len(data)]==data[32:],sources=clips,events=False,root_writes=False,visual_accepted=False,runtime_verified=False,limitations=['Initial contact calibration is the observed unmodified Dunwall canopy. Other ledge geometry and interrupted climbs need runtime evaluation.']))
L.finish('high-mantle-source-manifest-v1.json');(P/'high-mantle-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2))
