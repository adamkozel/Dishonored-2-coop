"""Bounded full-body failure gesture, appended without changing prior clips."""
import sys,struct,json,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from edge_data import poses as read_edge
from finger_reference import HandReference,SR
L=Library(['library-retarget-manifest.json'],'outofmana-authored-v45')
refs=read(P/'outofmana-reference-manifest-v45.json')['rows'];reports=[]
OUT=P/'outofmana-banks-v45';OUT.mkdir(exist_ok=True)
for role in ('corvo','emily'):
 data=(P/'bendtime-balanced-banks-v42'/f'{role}-bendtime-balanced-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE39' and count==247
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE40',40,n,248,fps,layout)+data[32:])
 rig=rig_for(role);hand=HandReference(rig);rest=[qts(struct.unpack_from('<7f',data,32+24+j*28))for j in range(n)]
 meta,donor=L.load(role,'sword_power_projectileattack');reference=next(r for r in refs if r['role']==role)
 edge=read_edge(P/reference['sample']);assert edge['sha256']==reference['sample_sha256']
 fp=[[qts(v)for v in frame]for frame in edge['frames']];assert len(fp[0])==len(SR.joints)
 right=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16:right.append(j)
 g=rig.fk(rest);poses=[];errors=[]
 for frame in range(120):
  t=frame/119;weight=math.sin(math.pi*t)**.8
  # A brief unsuccessful press follows the authored caster's whole body.
  # Full native lengths and foot contacts remain; FP contributes fingers only.
  p=blend(rest,sample(donor,t),.32*weight)
  for j in right:p[j]=rest[j].copy()
  p[17]=rest[17].copy();hand.apply(p,sample(fp,t),weight)
  pin_feet(rig,p,[g[j].translation for j in(21,25)],[g[j].to_3x3()for j in(21,25)])
  preview_helpers(rig,p);actual=rig.fk(p);errors.append(max((actual[j].translation-g[j].translation).length for j in(21,25)))
  poses.append(p)
 assert max(errors)<.002
 bank.extend(struct.pack('<IIIIff',247,121,0,0,0.,.333))
 for pose in poses+[poses[0]]:
  for mat in pose:bank.extend(struct.pack('<7f',*row(mat)))
 L.save(rig,role,'native_out_of_mana_standing_armed',poses,[identity()for _ in poses],'powers','333 ms unsuccessful power gesture. Whole-body native caster donor, source finger reference only, fixed native lengths, corrective bones and right sword grip. No FP wrist anchors, root motion, damage, power activation or effects. Corvo Bend Time standing-armed binding only; Emily is an offline rig candidate.',dict(body=meta['sha256'],first_person=reference),native_duration_ms=333,runtime_phase=15,max_foot_target_error=max(errors))
 path=OUT/f'{role}-outofmana-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE40',clips=248,prior_payload_unchanged=bank[32:len(data)]==data[32:],native_character='corvo',visual_accepted=False,runtime_verified=False))
L.finish('outofmana-source-manifest-v45.json');(P/'outofmana-bank-manifest-v45.json').write_text(json.dumps(reports,indent=2))
print('OUTOFMANA_BANKS',len(reports))
