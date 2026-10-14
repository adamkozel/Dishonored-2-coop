"""Append authored airborne combat balance to the protected v15 body bank.

The native sword scene remains the attack clock. The jump donor supplies free
legs and pelvis balance, never a world trajectory, floor constraint or FP hand
anchor. Landing blends back to the original grounded action at the same phase.
"""
import sys, struct, json, hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['library-retarget-manifest.json'],'air-combat-bank-sources-v1')
OUT=P/'air-combat-banks-v1';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'power-banks-v1'/f'{role}-power-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
 assert magic==b'D2MOVE15' and count==126
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE16',16,n,141,fps,layout)+data[32:])
 rig=rig_for(role);donor,frames=L.load(role,'sword_attack_jumpa');entry=24+121*n*28;sources=[]
 for grounded in range(44,59):
  poses=[];cid=126+grounded-44
  for f in range(120):
   attack=[qts(struct.unpack_from('<7f',data,32+grounded*entry+24+(f*n+j)*28))for j in range(n)]
   # The inspected 0.34..0.84s donor segment is genuinely airborne. Its
   # takeoff and landing are excluded: native physics owns those transitions.
   flight=sample(frames,(.34+.50*f/119)/donor['duration'])
   pose=copy_pose(attack)
   pose[2]=blend([flight[2]],[attack[2]],.25)[0]
   pose[2].translation=flight[2].translation.copy()
   for j in range(19,27):pose[j]=flight[j].copy()
   preview_helpers(rig,pose);poses.append(pose)
  duration=struct.unpack_from('<f',data,32+grounded*entry+20)[0]
  bank.extend(struct.pack('<IIIIff',cid,121,0,0,0.,duration))
  for pose in poses+[poses[0]]:
   assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
   for mat in pose:bank.extend(struct.pack('<7f',*row(mat)))
  sources.append(dict(clip=cid,grounded_clip=grounded,donor=donor['sha256'],donor_seconds=[.34,.84]))
  L.save(rig,role,f'airborne_sword_phase_{grounded}',poses,[identity()for _ in poses],'combat',
   'Native scene-timed sword acting with free airborne legs and pelvis counterbalance. No root writes, floor pinning, FP anchors or native damage events.',
   [hashlib.sha256(data).hexdigest(),donor['sha256']],runtime_ground_clip=grounded)
 path=OUT/f'{role}-air-combat-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE16',
  prior_payload_unchanged=bank[32:len(data)]==data[32:],sources=sources,root_writes=False,events=False,visual_accepted=False))
L.finish('air-combat-bank-source-manifest-v1.json')
(P/'air-combat-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('AIR_COMBAT_BANKS generated; runtime and visual checks pending',flush=True)
