"""Bounded crouched Mayhew hoist; no first-person anchors or root motion.

The prior remote victim contact path is lowered during the same snapshot
blend by the 0.674 m offset measured in native crouched Carry45 references.
This is an initial contact hypothesis, to be checked against real D2 frames.
"""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from contact_hand import descendants
from rig_math import ease
L=Library([],'crouched-pickup-sources-v1');out=P/'crouched-pickup-banks-v1';out.mkdir(exist_ok=True)
reports=[];measurements=[]
for parent in read(P/'carry-pickup-contact-bank-manifest-v5.json'):
 role=parent['role'];rig=rig_for(role);n=len(rig.joints);data=(P/parent['bank']).read_bytes()
 assert hashlib.sha256(data).hexdigest().upper()==parent['sha256']
 assert struct.unpack_from('<8sIIII',data)==(b'D2MOVE34',34,n,232,60)
 entry=24+121*n*28
 def get(cid,u):
  f=max(0,min(1,u))*119;a=int(f);b=min(a+1,119)
  return [blend([qts(struct.unpack_from('<7f',data,32+cid*entry+24+(a*n+j)*28))],
                [qts(struct.unpack_from('<7f',data,32+cid*entry+24+(b*n+j)*28))],f-a)[0]for j in range(n)]
 trace=read(P/f'native-paired-pickup-{role}-v24-fast/contact-analysis.json');points=sorted(trace['samples'],key=lambda x:x['phase'])
 def target(u,key):
  if u<=points[0]['phase']:v=Vector(points[0][key])
  elif u>=points[-1]['phase']:v=Vector(points[-1][key])
  else:
   k=next(k for k in range(len(points)-1)if points[k]['phase']<=u<=points[k+1]['phase']);a,b=points[k:k+2]
   v=Vector(a[key]).lerp(Vector(b[key]),ease((u-a['phase'])/(b['phase']-a['phase'])))
  native_phase=max(0,(u*1.4-.30)/1.1);v.z-=.674*ease((native_phase-.08)/.72)
  return v
 low=get(76,0);loaded=get(227,0);loadedG=rig.fk(loaded);fingers=descendants(rig,9)
 startG=rig.fk(low);endG=rig.fk(loaded);poses=[];metrics=[]
 for frame in range(85):
  u=frame/84;t=u*1.4;load=ease((t-.45)/.85);p=blend(low,loaded,load)
  reach=ease(t/.30)*(1-ease((t-.52)/.52));p[2].translation+=Vector((.26*reach,0,-.06*reach))
  for j in [3,4,5]:p[j]=p[j]@Matrix.Rotation(.055*reach,4,'Y')
  goal=target(u,'left_target');g=rig.fk(p);v=goal-g[7].translation
  weight=ease(t/.22);length=rig.bind[8].translation.length+rig.bind[9].translation.length
  p[2].translation+=v.normalized()*min(.16,max(0,v.length-length+.02))*weight
  feet=[startG[j].translation.lerp(endG[j].translation,load)for j in [21,25]]
  for k,(a,b,c) in enumerate([(19,20,21),(23,24,25)]):
   g=rig.fk(p);rig.solve(p,a,b,c,feet[k],g[a].translation+Vector((.8,0,-.1)))
   rig.orient(p,c,startG[c].to_quaternion().slerp(endG[c].to_quaternion(),load).to_matrix())
  for a,b,c,key,w,side in [(7,8,9,'left_target',weight,1),(14,15,16,'right_target',weight*(1-ease((t-.70)/.38)),-1)]:
   old=copy_pose(p);g=rig.fk(p);rotation=loadedG[c].to_3x3().copy() if c==9 else g[c].to_3x3().copy()
   rig.solve(p,a,b,c,target(u,key),g[a].translation+Vector((.1,.35*side,-.18)));rig.orient(p,c,rotation)
   for j in [a,b,c]:p[j]=blend([old[j]],[p[j]],w)[0]
  for j in fingers:p[j]=blend([low[j]],[loaded[j]],weight)[0]
  preview_helpers(rig,p);g=rig.fk(p);poses.append(p)
  metrics.append(dict(phase=u,left_error=(g[9].translation-target(u,'left_target')).length,
   foot_error=max((g[j].translation-feet[k]).length for k,j in enumerate([21,25]))))
 L.save(rig,role,'crouched_carry_pickup',poses,[identity()for _ in poses],'interactions',
  'Whole-body crouched gather and hoist, timed to native Carry45. Preserves native skeleton lengths and locked actor root. Contact path is a bounded Mayhew hypothesis; requires real-game visual review.',
  dict(parent_sha256=parent['sha256'],standing_trace_sha256=trace['source_sha256'],native_crouch_offset_m=.674),required_bank_version=35)
 clip=L.reports[-1];clip['equipment_policy']='hands_free';(P/clip['binary']).with_suffix('.json').write_text(json.dumps(clip,indent=2))
 bank=bytearray(data);bank[:8]=b'D2MOVE35';struct.pack_into('<I',bank,8,35);struct.pack_into('<I',bank,16,233)
 bank.extend(struct.pack('<IIIIff',232,121,0,0,0,1.4));frames=[sample(poses,f/119)for f in range(120)]
 bank.extend(b''.join(struct.pack('<7f',*row(m))for pose in frames+[frames[0]]for m in pose))
 assert bank[32:len(data)]==data[32:] and len(bank)==32+233*entry
 path=out/f'{role}-crouched-pickup-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),parent_sha256=parent['sha256'],
  clip_sha256=clip['sha256'],format='D2MOVE35',clips=233,added_clips=[232],other_clips_unchanged=True,runtime_verified=False,visual_accepted=False))
 measurements.append(dict(role=role,metrics=metrics,max_foot_error=max(x['foot_error']for x in metrics),native_length_error=clip['native_length_error'],complete=False))
L.finish('crouched-pickup-source-manifest-v1.json')
(P/'crouched-pickup-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2))
(P/'carry-research/crouched-pickup-v1-authoring.json').write_text(json.dumps(measurements,indent=2))
print(json.dumps([{k:v for k,v in r.items()if k!='metrics'}for r in measurements]),flush=True)
