"""Bounded crouched Mayhew hoist; no first-person anchors or root motion.

Uses actual v28 remote crouched victim contact paths and a small authored
approach step; adjusts loaded wrist contact in the isolated v2 bank.
This is an initial contact hypothesis, to be checked against real D2 frames.
"""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from contact_hand import descendants
from rig_math import ease
L=Library([],'crouched-pickup-sources-v2');out=P/'crouched-pickup-banks-v2';out.mkdir(exist_ok=True)
reports=[];measurements=[]
for parent in read(P/'crouched-pickup-bank-manifest-v1.json'):
 role=parent['role'];rig=rig_for(role);n=len(rig.joints);data=(P/parent['bank']).read_bytes()
 assert hashlib.sha256(data).hexdigest().upper()==parent['sha256']
 assert struct.unpack_from('<8sIIII',data)==(b'D2MOVE35',35,n,233,60)
 entry=24+121*n*28
 def get(cid,u):
  f=max(0,min(1,u))*119;a=int(f);b=min(a+1,119)
  return [blend([qts(struct.unpack_from('<7f',data,32+cid*entry+24+(a*n+j)*28))],
                [qts(struct.unpack_from('<7f',data,32+cid*entry+24+(b*n+j)*28))],f-a)[0]for j in range(n)]
 trace=read(P/f'native-crouched-pickup-drop-{role}-v28/contact-analysis.json');points=sorted(trace['samples'],key=lambda x:x['phase'])
 def target(u,key):
  if u<=points[0]['phase']:v=Vector(points[0][key])
  elif u>=points[-1]['phase']:v=Vector(points[-1][key])
  else:
   k=next(k for k in range(len(points)-1)if points[k]['phase']<=u<=points[k+1]['phase']);a,b=points[k:k+2]
   v=Vector(a[key]).lerp(Vector(b[key]),ease((u-a['phase'])/(b['phase']-a['phase'])))
  return v
 held=read(P/f'native-crouched-pickup-drop-{role}-v28/held-contact-analysis.json')
 carrier=read(P/f'native-crouched-pickup-drop-{role}-v28/carrier-held.json')
 shoulder=Vector(next(j['globalPose'][12:15]for j in carrier['joints']if j['nativeName']=='left_arm_jnt'))
 support_delta=Vector(held['left_target'])-shoulder
 def support(p):
  p=copy_pose(p);g=rig.fk(p);rotation=g[9].to_3x3().copy();goal=g[7].translation+support_delta
  rig.solve(p,7,8,9,goal,g[7].translation+Vector((.1,.35,-.18)));rig.orient(p,9,rotation);preview_helpers(rig,p)
  return p
 low=get(76,0);loaded=support(get(227,0));loadedG=rig.fk(loaded);fingers=descendants(rig,9)
 startG=rig.fk(low);endG=rig.fk(loaded);poses=[];metrics=[]
 for frame in range(85):
  u=frame/84;t=u*1.4;load=ease((t-.45)/.85);p=blend(low,loaded,load)
  reach=ease(t/.30)*(1-ease((t-.52)/.52));p[2].translation+=Vector((.50*reach,0,-.20*reach))
  g=rig.fk(p);rig.orient(p,3,Matrix.Rotation(.18*reach,3,'Y')@g[3].to_3x3())
  goal=target(u,'left_target');g=rig.fk(p);v=goal-g[7].translation
  weight=ease(t/.22);length=rig.bind[8].translation.length+rig.bind[9].translation.length
  p[2].translation+=v.normalized()*min(.30,max(0,v.length-length+.02))*weight
  feet=[startG[j].translation.lerp(endG[j].translation,load)for j in [21,25]]
  feet[0].x+=.20*reach;feet[0].y+=.04*reach
  step_phase=t/.30 if t<.30 else (t-.52)/.52 if t>.52 else -1
  if 0<step_phase<1:feet[0].z+=.035*math.sin(math.pi*step_phase)
  for k,(a,b,c) in enumerate([(19,20,21),(23,24,25)]):
   g=rig.fk(p);rig.solve(p,a,b,c,feet[k],g[a].translation+Vector((.8,0,-.1)))
   rig.orient(p,c,startG[c].to_quaternion().slerp(endG[c].to_quaternion(),load).to_matrix())
  for a,b,c,key,w,side in [(7,8,9,'left_target',weight,1),(14,15,16,'right_target',weight*(1-ease((t-.70)/.38)),-1)]:
   old=copy_pose(p);g=rig.fk(p);rotation=loadedG[c].to_3x3().copy() if c==9 else g[c].to_3x3().copy()
   contact=target(u,key)
   if c==9:contact=contact.lerp(g[7].translation+support_delta,ease((t-1.05)/.35))
   rig.solve(p,a,b,c,contact,g[a].translation+Vector((.1,.35*side,-.18)));rig.orient(p,c,rotation)
   for j in [a,b,c]:p[j]=blend([old[j]],[p[j]],w)[0]
  for j in fingers:p[j]=blend([low[j]],[loaded[j]],weight)[0]
  preview_helpers(rig,p);g=rig.fk(p);poses.append(p)
  metrics.append(dict(phase=u,left_error=(g[9].translation-target(u,'left_target')).length,
   foot_error=max((g[j].translation-feet[k]).length for k,j in enumerate([21,25]))))
 L.save(rig,role,'crouched_carry_pickup',poses,[identity()for _ in poses],'interactions',
  'Whole-body crouched gather and hoist, timed to native Carry45. Preserves native skeleton lengths and locked actor root. Contact path is a bounded Mayhew hypothesis; requires real-game visual review.',
  dict(parent_sha256=parent['sha256'],crouched_trace_sha256=trace['source_sha256'],held_contact=held['source'],approach_step_m=.20),required_bank_version=35)
 clip=L.reports[-1];clip['equipment_policy']='hands_free';(P/clip['binary']).with_suffix('.json').write_text(json.dumps(clip,indent=2))
 bank=bytearray(data)
 for cid in [227,228,230,232]:
  if cid==232:frames=[sample(poses,f/119)for f in range(120)]
  else:
   frames=[]
   for f in range(120):
    old=get(cid,f/119);fixed=support(old)
    p=blend(old,fixed,1-ease((f/119)/.35)) if cid==230 else fixed
    frames.append(p)
  at=32+cid*entry+24
  bank[at:at+121*n*28]=b''.join(struct.pack('<7f',*row(m))for pose in frames+[frames[0]]for m in pose)
 assert len(bank)==len(data)
 for cid in range(233):
  if cid not in [227,228,230,232]:assert bank[32+cid*entry:32+(cid+1)*entry]==data[32+cid*entry:32+(cid+1)*entry]
 path=out/f'{role}-crouched-pickup-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),parent_sha256=parent['sha256'],
  clip_sha256=clip['sha256'],format='D2MOVE35',clips=233,changed_clips=[227,228,230,232],other_clips_unchanged=True,runtime_verified=False,visual_accepted=False))
 measurements.append(dict(role=role,metrics=metrics,max_foot_error=max(x['foot_error']for x in metrics),native_length_error=clip['native_length_error'],complete=False))
L.finish('crouched-pickup-source-manifest-v2.json')
(P/'crouched-pickup-bank-manifest-v2.json').write_text(json.dumps(reports,indent=2))
(P/'carry-research/crouched-pickup-v2-authoring.json').write_text(json.dumps(measurements,indent=2))
print(json.dumps([{k:v for k,v in r.items()if k!='metrics'}for r in measurements]),flush=True)
