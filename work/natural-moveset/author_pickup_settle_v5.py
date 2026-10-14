"""Preserve supported hoist contact at the end; v34 owns one loaded crossfade."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['body-lift-contact-manifest-v4.json'],'body-lift-contact-v5')
parents={r['role']:r for r in read(P/'carry-pickup-contact-bank-manifest-v4.json')}
reports=[];metrics=[];out=P/'carry-pickup-contact-banks-v5';out.mkdir(exist_ok=True)
for role in ['corvo','emily']:
 rig=rig_for(role);m,frames=L.load(role,'carry_pickup_contact_pilot')
 trace=read(P/f'native-paired-pickup-{role}-v24-fast/contact-analysis.json');samples=sorted(trace['samples'],key=lambda r:r['phase'])
 def target(u):
  if u<=samples[0]['phase']:return Vector(samples[0]['left_target'])
  if u>=samples[-1]['phase']:return Vector(samples[-1]['left_target'])
  k=next(k for k in range(len(samples)-1)if samples[k]['phase']<=u<=samples[k+1]['phase'])
  a,b=samples[k:k+2];return Vector(a['left_target']).lerp(Vector(b['left_target']),(u-a['phase'])/(b['phase']-a['phase']))
 poses=[];errs=[]
 for f,old in enumerate(frames):
  u=f/(len(frames)-1);t=u*1.4;p=copy_pose(old);g=rig.fk(p);rotation=g[9].to_3x3().copy();w=ease((t-1.02)/.08)
  if w:
   rig.solve(p,7,8,9,target(u),g[7].translation+Vector((.12,.35,-.15)));rig.orient(p,9,rotation)
   for j in [7,8,9]:p[j]=blend([old[j]],[p[j]],w)[0]
   preview_helpers(rig,p)
  if w>.999:errs.append((rig.fk(p)[9].translation-target(u)).length)
  poses.append(p)
 L.save(rig,role,'carry_pickup_supported_settle',poses,[identity()for _ in poses],'interactions',
  'Measured-Mayhew pickup ending with left-hand support retained; D2MOVE34 crossfades once to the loaded pose over 350ms. Not arbitrary-body contact or accepted complete pickup.',
  dict(parent_sha256=m['sha256'],trace_sha256=trace['source_sha256']),required_bank_version=34)
 clip=L.reports[-1];clip['equipment_policy']='hands_free';(P/clip['binary']).with_suffix('.json').write_text(json.dumps(clip,indent=2))
 parent=parents[role];data=(P/parent['bank']).read_bytes();assert hashlib.sha256(data).hexdigest().upper()==parent['sha256']
 n=len(rig.joints);entry=24+121*n*28;at=32+231*entry;bank=bytearray(data);bank[:8]=b'D2MOVE34';struct.pack_into('<I',bank,8,34)
 resampled=[sample(poses,f/119)for f in range(120)]
 bank[at+24:at+entry]=b''.join(struct.pack('<7f',*row(mat))for pose in resampled+[resampled[0]]for mat in pose)
 assert bank[12:at+24]==data[12:at+24]and len(bank)==len(data)
 path=out/f'{role}-carry-pickup-settle-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),parent_sha256=parent['sha256'],
  clip_sha256=clip['sha256'],format='D2MOVE34',clips=232,changed_clips=[231],other_clips_unchanged=True,visual_accepted=False,runtime_verified=False,
  settle_ms=350,limitations=['Pinned Mayhew fixture only.','Right-hand grasp, arbitrary bodies and crouched hoist remain incomplete.']))
 metrics.append(dict(role=role,max_support_error_m=max(errs),native_length_error=clip['native_length_error'],complete=False,visual_accepted=False))
L.finish('body-lift-contact-manifest-v5.json')
(P/'carry-pickup-contact-bank-manifest-v5.json').write_text(json.dumps(reports,indent=2))
(P/'carry-research/pickup-settle-v5.json').write_text(json.dumps(metrics,indent=2));print(json.dumps(metrics),flush=True)
