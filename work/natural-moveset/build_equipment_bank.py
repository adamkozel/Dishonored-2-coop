"""Native unarmed directions and body-relative sword contact, event-free v9 bank."""
import sys,struct,json,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['library-retarget-manifest.json','unarmed-retarget-manifest.json','stance-retarget-manifest.json'],'equipment-bank-sources-v3')
OUT=P/'equipment-banks-v3';OUT.mkdir(exist_ok=True);reports=[]
attachment=read(P/'native-shadow-equipment-v6/samples.json')
contacts={}
for role in ('corvo','emily'):
 data=(P/'combat-banks-v1'/f'{role}-combat-bank.bin').read_bytes();magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE08' and count==59
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE09',9,n,89,fps,layout)+data[32:]);entry=24+121*n*28;rig=rig_for(role);sources=[]
 def previous(cid):return [[qts(struct.unpack_from('<7f',data,32+cid*entry+24+(f*n+j)*28))for j in range(n)]for f in range(121)]
 def write(cid,fs,stride=0,angle=0):
  assert len(fs)==121;bank.extend(struct.pack('<IIIIff',cid,121,0,0,stride,angle))
  for f in fs:
   assert max(abs(f[j][a][b]-float(a==b))for j in (0,1)for a in range(4)for b in range(4))<1e-6
   assert max((f[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
   for m in f:bank.extend(struct.pack('<7f',*row(m)))
  print('BANK',role,cid,flush=True)
 m,idle=L.load(role,'empty_idle');fs=[sample(idle,f/120,True)for f in range(121)];fs[-1]=fs[0];write(59,fs)
 reference=previous(1);heights=[]
 def waves(poses):
  raw=[rig.fk(p)for p in poses[:120]];channels=[[g[j].translation.z for g in raw]for j in (21,25)]
  return [[(v-min(a))/max(1e-5,max(a)-min(a))for v in a]for a in channels]
 rw=waves(reference)
 for gi,gait in enumerate(('walk','run')):
  cards=[];strides=[]
  for d in 'nwse':
   m,fs=L.load(role,f'empty_{gait}_{d}_add');poses=[sample(fs,f/120,True)for f in range(120)];hw=waves(poses)
   offset=min(range(120),key=lambda k:sum((hw[j][(f+k)%120]-rw[j][f])**2 for j in range(2)for f in range(120)))
   poses=poses[offset:]+poses[:offset];poses.append(poses[0]);cards.append(poses)
   delta=Vector(m['root_track'][-1][4:7])-Vector(m['root_track'][0][4:7]);strides.append(delta)
   sources.append(dict(alias=m['selection']['alias'],sha256=m['sha256'],phase_offset=offset/120,stride=delta.length))
  for d in range(8):
   a=d//2;b=(a+1)%4;t=.5 if d%2 else 0;poses=[blend(x,y,t)for x,y in zip(cards[a],cards[b])];poses[-1]=poses[0]
   stride=strides[a].lerp(strides[b],t).length;assert .2<stride<6
   write(60+gi*8+d,poses,stride,d*math.pi/4)
 # Preserve the established whole-body crouch; release the sword fingers.
 # These poses are unarmed candidates, not new donor-clip coverage.
 fingers=[i for i,j in enumerate(rig.joints)if any(k in j['nativeName']for k in ('thumb','index','middle','ring','pinky'))]
 for c in range(9):
  fs=previous(17+c)
  for f in fs:
   for j in fingers:f[j]=idle[0][j].copy()
  stride,angle=struct.unpack_from('<ff',data,32+(17+c)*entry+16);write(76+c,fs,stride,angle)
 sampleRole='client'if role=='corvo'else'host'
 drawn=next(x['attachment']for x in attachment if x['role']==sampleRole and x['label']=='expired-restored')
 holstered=next(x['attachment']for x in attachment if x['role']==sampleRole and x['label']=='holstered')
 # Native reference-frame quaternion is inverse to the rig's local rotation.
 # Corroborated by mixamo-crouch/verify_attachment.py against native render roots.
 def attachment_matrix(a):
  q=a['quaternion'];return qts([-q[0],-q[1],-q[2],q[3]]+a['offset'])
 D=attachment_matrix(drawn);H=attachment_matrix(holstered)
 stand=previous(0)[0];low=previous(17)[0];sg=rig.fk(stand);lg=rig.fk(low)
 def lower(p):
  g=rig.fk(p);p=copy_pose(p);p[2].translation+=low[2].translation-stand[2].translation
  for j in (2,3,4,5):
   # The donor already bends and rotates the chest toward its holster.
   # Adding the entire crouch rotation folded that bend twice. Blend absolute
   # body posture instead, retaining donor motion and the native leg lengths.
   q=low[j].to_quaternion().slerp(p[j].to_quaternion(),.35);t=p[j].translation.copy();p[j]=q.normalized().to_matrix().to_4x4();p[j].translation=t
  return pin_feet(rig,p,[g[j].translation+lg[j].translation-sg[j].translation for j in (21,25)],[g[j].to_3x3()for j in (21,25)])
 roleContacts=[]
 for k,(alias,lowPose) in enumerate((('sword_equipsword',False),('sword_unequipsword',False),('sword_equipsword',True),('sword_unequipsword',True))):
  m,fs=L.load(role,alias);poses=[sample(fs,f/119)for f in range(120)]
  if lowPose:poses=[lower(p)for p in poses]
  def target(p):
   g=rig.fk(p);return g[195]@H@D.inverted()@p[17].inverted()
  # Choose the source's nearest hand/holster approach, then refine a finite
  # contact interval. The target moves with the pelvis; never a FP hand anchor.
  nearest=min(range(20,100),key=lambda f:(rig.fk(poses[f])[16].translation-target(poses[f]).translation).length)
  # Reach with the chest as well as the arm. Never stretch anatomical bones
  # to make a wrist-only solution meet the opposite-side holster.
  peak=copy_pose(poses[nearest]);torso=Quaternion();axes=[Vector((1,0,0)),Vector((0,1,0)),Vector((0,0,1))]
  reach=rig.bind[15].translation.length+rig.bind[16].translation.length-.012
  def chest_pose(q,p):
   p=copy_pose(p);v=p[4].translation.copy();p[4]=(q@p[4].to_quaternion()).normalized().to_matrix().to_4x4();p[4].translation=v;return p
  def reach_score(q):
   p=chest_pose(q,peak);g=rig.fk(p);excess=max(0,(target(p).translation-g[14].translation).length-reach)
   return excess*excess+q.angle*q.angle*.00002
  for step in (6,3,1):
   for iteration in range(14):
    candidates=[torso]+[(Quaternion(a,math.radians(step*sign))@torso).normalized()for a in axes for sign in (-1,1)]
    candidates=[q for q in candidates if q.angle<math.radians(28)]
    best=min(candidates,key=reach_score)
    if best==torso:break
    torso=best
  errors=[];angles=[]
  for f,p in enumerate(poses):
   distance=abs(f-nearest);w=ease(1-distance/23)
   if w:
    p=chest_pose(Quaternion().slerp(torso,w),p);poses[f]=p
    desired=target(p);g=rig.fk(p);orientation=g[16].to_quaternion().slerp(desired.to_quaternion(),w).to_matrix()
    point=g[16].translation.lerp(desired.translation,w);pole=g[15].translation
    rig.solve(p,14,15,16,point,pole);rig.orient(p,16,orientation);preview_helpers(rig,p)
   if f==nearest:
    g=rig.fk(p);a=g[17]@D;b=g[195]@H;errors.append((a.translation-b.translation).length);angles.append(math.degrees(a.to_quaternion().rotation_difference(b.to_quaternion()).angle))
  assert max(errors)<.02,(role,alias,lowPose,'Unreachable hand contact',errors)
  poses.append(poses[0]);write(85+k,poses,0,m['duration'])
  aliasOut=('crouch_'if lowPose else'')+alias+'_contact'
  L.save(rig,role,aliasOut,poses[:-1],[identity()for _ in poses[:-1]],'equipment','Body-relative native sword grip / holster contact candidate',[m['sha256']],contact_frame=nearest,contact_amount=nearest/119,contact_position_error=max(errors),contact_rotation_error=max(angles))
  roleContacts.append(dict(clip=85+k,source_alias=alias,source_sha256=m['sha256'],crouched=lowPose,frame=nearest,amount=nearest/119,chest_reach_degrees=math.degrees(torso.angle),contact_position_error=max(errors),contact_rotation_error=max(angles)))
 contacts[role]=[r['amount']for r in roleContacts]
 path=OUT/f'{role}-equipment-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE09',prior_payload_unchanged=bank[32:len(data)]==data[32:],directions=sources,contacts=roleContacts,native_attachments=dict(drawn=drawn,holstered=holstered),events=False,root_writes=False,visual_accepted=False,gameplay_verified=False,limitations=['All-direction unarmed gaits use verified Corvo donor; Emily is retargeted.','Unarmed crouch preserves existing body posture and releases fingers.','Unarmed sprint, lean, air and slide need separate full-body weaponless adaptation.','Contact solves require actual-game visual review; numerical coincidence is not acceptance.']))
(P/'equipment-bank-manifest-v3.json').write_text(json.dumps(reports,indent=2),encoding='utf-8');L.finish('equipment-contact-manifest-v3.json')
header='#pragma once\n// Generated by build_equipment_bank.py; native body-relative handoff phases.\nconstexpr float movementEquipmentContact[2][4]={\n'+',\n'.join('    {'+','.join(f'{v:.9f}f'for v in contacts[r])+'}'for r in ('corvo','emily'))+'\n};\n'
(P.parent.parent/'outputs/d2-coop-lab/src/movement_equipment_contact.h').write_text(header,encoding='utf-8')
