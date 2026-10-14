"""Native full-body NPC donor adapted to observed player keyhole phases."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['interaction-retarget-manifest.json','campaign-retarget-manifest.json'],'keyhole-sources-v1')
OUT=P/'keyhole-banks-v1';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'hard-landing-crouch-banks-v1'/f'{role}-hard-landing-crouch-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE23'and count==183
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE24',24,n,193,fps,layout)+data[32:]);entry=24+121*n*28;rig=rig_for(role)
 mi,enter=L.load(role,'empty_distraction_look_keyhole_in');ml,loop=L.load(role,'empty_distraction_look_keyhole_loop');mo,leave=L.load(role,'empty_distraction_look_keyhole_out')
 rests={i:[qts(struct.unpack_from('<7f',data,32+i*entry+24+j*28))for j in range(n)]for i in (59,76)}
 lookup={j['nativeName']:i for i,j in enumerate(rig.joints)};pairs=[]
 for j in rig.joints:
  name=j['nativeName'];other=('right'+name[4:])if name.startswith('left')else('left'+name[5:])if name.startswith('right')else name
  pairs.append(lookup.get(other,lookup[name]))
 reflection=Matrix.Diagonal((1,-1,1));clips=[]
 def mirrored(pose):
  old=rig.fk(pose);out=copy_pose(rig.bind);g=[]
  for i,j in enumerate(rig.joints):
   if i<2:out[i]=Matrix.Identity(4)
   else:
    k=pairs[i];target=reflection@old[k].to_3x3()@rig.global_bind[k].to_3x3().inverted()@reflection@rig.global_bind[i].to_3x3()
    parent=j['parent'];out[i]=(g[parent].to_3x3().inverted()@target).to_4x4();out[i].translation=rig.bind[i].translation
    if i==2:out[i].translation=reflection@pose[2].translation
   g.append(out[i]if j['parent']<0 else g[j['parent']]@out[i])
  preview_helpers(rig,out);return out
 for side in (0,1):
  for variant,kind,crouch,duration in ((7,'in',False,1.5),(8,'loop',False,ml['duration']),(9,'out',False,1.),(10,'in',True,1.5),(11,'out',True,1.)):
   cid=183+side*5+variant-7;rest=rests[76 if crouch else 59]
   def authored(t):
    if kind=='in':
     pose=sample(enter,t);pose=blend(rest,pose,ease(t/.16));pose=blend(pose,loop[0],ease((t-.86)/.14))
    elif kind=='out':
     pose=sample(leave,t);pose=blend(loop[0],pose,ease(t/.14));pose=blend(pose,rest,ease((t-.75)/.25))
    else:
     pose=sample(loop,t);pose=blend(pose,loop[0],ease((t-.9)/.1))
    if side:pose=mirrored(pose)
    preview_helpers(rig,pose);return pose
   poses=[authored(f/119)for f in range(120)];poses.append(poses[0]);bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,duration))
   for pose in poses:
    for matrix in pose:bank.extend(struct.pack('<7f',*row(matrix)))
   alias=f'empty_native_keyhole_{"right"if side else"left"}_{"crouch_"if crouch else""}{kind}'
   frames=round(duration*60)+1
   L.save(rig,role,alias,[authored(f/(frames-1))for f in range(frames)],[identity()for _ in range(frames)],'stance','Complete-body NPC keyhole motion; native player scene drives phase, network owns world root.',[mi['sha256'],ml['sha256'],mo['sha256']],native_variant=variant+side*5,native_duration_ms=500 if kind=='loop'else round(duration*1000),authored_loop_duration_ms=round(ml['duration']*1000),mirrored=bool(side))
   L.reports[-1]['equipment_policy']='hands_free';(L.out/(L.reports[-1]['id']+'.json')).write_text(json.dumps(L.reports[-1],indent=2),encoding='utf-8')
   clips.append(dict(id=cid,alias=alias,variant=variant+side*5,duration=duration))
 path=OUT/f'{role}-keyhole-bank.bin';path.write_bytes(bank);reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE24',clips=193,prior_payload_unchanged=bank[32:len(data)]==data[32:],sources=clips,events=False,root_writes=False,visual_accepted=False,runtime_verified=False))
L.finish('keyhole-source-manifest-v1.json');(P/'keyhole-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
