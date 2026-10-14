"""Append four observed native-clock Domino phases; retain all bank46 bytes.

Standing armed Emily candidate. Corvo is an offline skeleton comparison only.
The selection hand uses native FP finger data while the shoulder/torso remain
complete-body authored acting; there are no camera-space wrist constraints.
"""
import sys,struct,json,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
from finger_reference import HandReference
from edge_data import poses as read_edge
L=Library(['power-retarget-manifest-v1.json'],'domino-sources-v192')
OUT=P/'domino-banks-v192';OUT.mkdir(exist_ok=True);reports=[]
reference=next(r for r in read(P/'fp-power-reference-manifest.json')['rows'] if 'ply_leftarm_power_phantasmal_select'in r['aliases'])
fp=read_edge(P/reference['sample_file']);assert fp['sha256']==reference['sample_sha256']
fingerFrames=[[qts(v)for v in f]for f in fp['frames']]
for role in ('corvo','emily'):
 data=(P/'mesmerize-context-banks-v132'/f'{role}-mesmerize-context-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE46'and version==46 and count==285
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE47',47,n,289,fps,layout)+data[32:])
 rig=rig_for(role);hand=HandReference(rig);grip=[qts(struct.unpack_from('<7f',data,32+24+j*28))for j in range(n)];fingers=[];sources=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and j!=16 and 'helper'not in rig.joints[j]['nativeName']:fingers.append(j)
 for offset,(phase,alias,duration)in enumerate([('entry','select_in',266),('aim','select_loop',1333),('select','select_loop',400),('exit','select_out',266)]):
  m,fs=L.load(role,'power_domino_'+alias);poses=[]
  for f in range(120):
   u=f/119;pose=sample(fs,u)
   if phase=='select':
    # A brief native-body accent returns to the same prepared pose. The FP
    # selection is a finger reference only, never an arm-position anchor.
    pose=sample(fs,u*.16)
    pulse=math.sin(math.pi*u)**2
    for j in (3,4,5):rig.orient(pose,j,Matrix.Rotation(math.radians(-1.5*pulse),3,'Y')@rig.fk(pose)[j].to_3x3())
    rig.orient(pose,7,Matrix.Rotation(math.radians(4*pulse),3,'Y')@rig.fk(pose)[7].to_3x3())
    hand.apply(pose,sample(fingerFrames,u),ease(u*5)*(1-ease((u-.8)/.2)))
   for j in fingers:pose[j]=grip[j].copy()
   pose[17]=grip[17].copy();preview_helpers(rig,pose);poses.append(pose)
  bank.extend(struct.pack('<IIIIff',285+offset,121,0,0,0.,duration/1000))
  for pose in poses+[poses[0]]:
   assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
   for matrix in pose:bank.extend(struct.pack('<7f',*row(matrix)))
  provenance=dict(body=m['sha256'],native_selection_fingers=reference['sample_sha256']if phase=='select'else None,native_take='domino-native-source-v187',subscene=['Cast_In','Cast_Loop','Cast_Select','Cast_Select_out'][offset])
  L.save(rig,role,'native_domino_'+phase,poses,[identity()for _ in poses],'powers','Native support whole-body acting, observed Domino phase timing and FP finger inspiration only. Standing armed Emily runtime candidate. Current sword grip/attachment, native lengths and corrective helpers retained. No root, AI, damage, power or VFX events.',provenance,runtime_phase=19+offset,native_duration_ms=duration)
  sources.append(dict(variant=19+offset,clip=285+offset,phase=phase,duration_ms=duration,source=provenance))
 path=OUT/f'{role}-domino-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),format='D2MOVE47',clips=289,prior_payload_unchanged=bank[32:len(data)]==data[32:],sources=sources,native_character='emily',visual_accepted=False,runtime_verified=False))
L.finish('domino-bank-source-manifest-v192.json');(P/'domino-bank-manifest-v192.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
print('DOMINO_BANKS_GENERATED',flush=True)
