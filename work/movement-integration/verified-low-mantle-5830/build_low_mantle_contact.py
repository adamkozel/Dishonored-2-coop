"""Bounded Dunwall contact study. Roots/events stay outside the pose bank."""
import sys,struct,json,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['library-retarget-manifest.json'],'low-mantle-sources-v2')
OUT=P/'low-mantle-banks-v2';OUT.mkdir(exist_ok=True);reports=[]
observed=read(P/'low-mantle-v3-test/states.json')
samples=[];height=span=0
for r in observed:
 if r['source']!='host' or not r['armed'] or r['trial']!=1 or not r['native'].get('traversalSceneValid'):continue
 s=r['native'];a=s['active'][0]['mantleResearch'];delta=Vector(a['goalBc'])-Vector(a['startE8']);h=Vector((delta.x,delta.y,0));unit=h.normalized();pos=Vector(s['position'])-Vector(a['startE8']);height=delta.z;span=h.length
 samples.append((s['traversalScene']['timeMs']/500,pos.dot(unit),pos.z))
samples=[(0,0,0)]+samples+[(1,span,height)]
def trajectory(t):
 for a,b in zip(samples,samples[1:]):
  if a[0]<=t<=b[0]:
   w=(t-a[0])/(b[0]-a[0]);return Vector((a[1]+(b[1]-a[1])*w,0,a[2]+(b[2]-a[2])*w))
 raise ValueError(t)
for role in ('corvo','emily'):
 data=(P/'low-mantle-banks-v1'/f'{role}-low-mantle-bank.bin').read_bytes();magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE12' and count==113
 entry=24+121*n*28;bank=bytearray(data[:32+111*entry]);rig=rig_for(role);m,donor=L.load(role,'sword_mantlelow_walkleft_in');metrics=[]
 def previous(cid):return [qts(struct.unpack_from('<7f',data,32+cid*entry+24+j*28))for j in range(n)]
 hand=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and 'helper'not in rig.joints[j]['nativeName']:hand.append(j)
 for unarmed in (False,True):
  stand=previous(59 if unarmed else 0);sg=rig.fk(stand);poses=[];errors=[]
  for f in range(120):
   t=f/119;pos=trajectory(t);pose=sample(donor,(10+20*t)/30)
   if unarmed:
    for j in hand:pose[j]=stand[j].copy()
   # Recover over the final third of the native climb instead of abruptly
   # discarding the donor's deeply folded finishing pose after the scene.
   recovery=ease((t-.64)/.36);pose=blend(pose,stand,recovery)
   g=rig.fk(pose)
   # The native ledge is below the donor's 1.5 m surface. Retain the donor's
   # body acting but aim the supporting palm at the measured existing desk.
   contact=ease((t-.05)/.12)*(1-ease((t-.58)/.18))
   target=Vector((span-.22,.25 if role=='corvo'else .22,height+.055))-pos
   if contact:
    reach=rig.bind[8].translation.length+rig.bind[9].translation.length-.02
    shoulder=g[7].translation;excess=max(0,(target-shoulder).length-reach)
    # Small whole-body balance shift makes the support reachable; anatomical
    # bone translations and the network actor transform remain untouched.
    correction=(target-shoulder).normalized()*min(.20,excess)*contact
    pose[2].translation+=correction;g=rig.fk(pose)
    point=g[9].translation.lerp(target,contact);rotation=g[9].to_3x3().copy()
    rig.solve(pose,7,8,9,point,g[8].translation);rig.orient(pose,9,rotation)
    if contact>.99:errors.append((rig.fk(pose)[9].translation-target).length)
   # Clear the edge with the leading leg, then plant both feet at their
   # native standing spacing as weight transfers onto the surface.
   g=rig.fk(pose)
   for j,a,b,begin,end in ((21,19,20,.16,.55),(25,23,24,.44,.86)):
    weight=ease((t-begin)/(end-begin));goal=sg[j].translation+Vector((span,0,height))-pos
    goal.z+=.08*math.sin(math.pi*min(1,max(0,(t-begin)/(end-begin))))
    targetFoot=g[j].translation.lerp(goal,weight);rot=g[j].to_quaternion().slerp(sg[j].to_quaternion(),weight).to_matrix()
    rig.solve(pose,a,b,j,targetFoot,g[b].translation);rig.orient(pose,j,rot)
   preview_helpers(rig,pose);poses.append(pose)
  poses.append(poses[0]);bank.extend(struct.pack('<IIIIff',111+int(unarmed),121,0,0,0.,.5))
  for pose in poses:
   assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
   for mat in pose:bank.extend(struct.pack('<7f',*row(mat)))
  alias=('unarmed_'if unarmed else'armed_')+'native_low_mantle_contact'
  L.save(rig,role,alias,poses[:-1],[identity()for _ in poses[:-1]],'traversal','Low-mantle contact study on observed native ledge trajectory; finite palm support, leg clearance and standing recovery.',[m['sha256']],native_scene_duration_ms=500,contact_max_error=max(errors),canonical_height=height,canonical_span=span)
  metrics.append(dict(unarmed=unarmed,contact_max_error=max(errors)))
 path=OUT/f'{role}-low-mantle-bank.bin';path.write_bytes(bank);reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),prior_payload_unchanged=bank[:32+111*entry]==data[:32+111*entry],contact=metrics,canonical_height=height,canonical_span=span,trajectory_source='low-mantle-v3-test/states.json: host armed trial1',events=False,root_writes=False,visual_accepted=False,limitations=['Contact currently fitted to measured Dunwall desk trajectory; arbitrary ledge heights and runtime contact adaptation remain unverified.','No weapon VFX or gameplay events are emitted.']))
L.finish('low-mantle-source-manifest-v2.json');(P/'low-mantle-bank-manifest-v2.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
