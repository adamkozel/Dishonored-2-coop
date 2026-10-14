"""Isolated first native-timed mantle; roots/events remain absent."""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['library-retarget-manifest.json'],'low-mantle-sources-v1')
OUT=P/'low-mantle-banks-v1';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'vertical-peek-banks-v1'/f'{role}-vertical-peek-bank.bin').read_bytes();magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE11' and count==111
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE12',12,n,113,fps,layout)+data[32:]);entry=24+121*n*28;rig=rig_for(role)
 # The player has already jumped when Mantle takes over. Omit the donor's
 # preliminary approach steps, preserving its complete climb and tuck.
 m,donor=L.load(role,'sword_mantlelow_walkleft_in');start=10;assert m['fps']==30 and len(donor)==31
 empty=[qts(struct.unpack_from('<7f',data,32+59*entry+24+j*28))for j in range(n)]
 hand=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and 'helper'not in rig.joints[j]['nativeName']:hand.append(j)
 sources=[]
 for unarmed in (False,True):
  cid=111+int(unarmed);poses=[]
  for f in range(120):
   pose=sample(donor,(start+(len(donor)-1-start)*f/119)/(len(donor)-1))
   if unarmed:
    for j in hand:pose[j]=empty[j].copy()
   preview_helpers(rig,pose);poses.append(pose)
  alias=('unarmed_'if unarmed else'armed_')+'native_low_mantle';poses.append(poses[0]);bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,.5))
  for pose in poses:
   assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
   for mat in pose:bank.extend(struct.pack('<7f',*row(mat)))
  L.save(rig,role,alias,poses[:-1],[identity()for _ in poses[:-1]],'traversal','Native complete-body low mantle, cropped to the climbing phase; actual player scene owns timing and network root owns trajectory. Contact and visual acceptance pending.',[m['sha256']],donor_frames=[start,len(donor)-1],native_scene_duration_ms=500)
  sources.append(dict(id=cid,alias=alias,source=m['binary'],source_sha256=m['sha256'],donor_frames=[start,len(donor)-1],native_duration_ms=500))
 path=OUT/f'{role}-low-mantle-bank.bin';path.write_bytes(bank);reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE12',clips=113,prior_payload_unchanged=bank[32:len(data)]==data[32:],sources=sources,events=False,root_writes=False,gameplay_verified=False,visual_accepted=False,limitations=['Only the observed mantle_low_idle Scene_1 variant.','No ledge contact constraints yet.','Cropped native donor has a different original root trajectory; live visual comparison is required before acceptance.']))
L.finish('low-mantle-source-manifest-v1.json');(P/'low-mantle-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
