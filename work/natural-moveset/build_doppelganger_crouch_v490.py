"""Full-body crouched Doppelganger acting; retain all322 previous clips.

Native489 supplies crouch6/UsePower63 context and466/1200/666ms timing.
Whole-body407 donor supplies changing acting, not first-person wrist anchors.
"""
import sys,struct,json,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['doppelganger-bank-source-manifest-v407.json'],'doppelganger-crouch-sources-v490')
OUT=P/'doppelganger-crouch-banks-v490';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'doppelganger-banks-v407'/f'{role}-doppelganger-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data)
 assert magic==b'D2MOVE55' and version==55 and count==322
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE56',56,n,325,fps,layout)+data[32:]);entry=24+121*n*28
 rig=rig_for(role);_,initial=L.load(role,'native_doppelganger_in');reference=initial[0];refG=rig.fk(reference)
 rest=[qts(struct.unpack_from('<7f',data,32+17*entry+24+j*28)) for j in range(n)];restG=rig.fk(rest);right=[];clips=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and j!=16 and 'helper' not in rig.joints[j]['nativeName']:right.append(j)
 for offset,(phase,duration) in enumerate([('in',466),('loop',1200),('out',666)]):
  meta,frames=L.load(role,'native_doppelganger_'+phase);errors=[];heights=[];poses=[]
  for f in range(120):
   t=f/119;source=sample(frames,t);sourceG=rig.fk(source);pose=copy_pose(source)
   for j in range(2,27):
    if j in (10,17):continue
    gain=.55 if j in (2,18) else .8 if j in (3,4,5) else 1.
    delta=reference[j].to_quaternion().inverted()@source[j].to_quaternion()
    translation=pose[j].translation.copy();pose[j]=(rest[j].to_quaternion()@Quaternion().slerp(delta,gain)).normalized().to_matrix().to_4x4();pose[j].translation=translation
   pose[2].translation=rest[2].translation+(source[2].translation-reference[2].translation)*.4
   for j in right:pose[j]=rest[j].copy()
   pose[17]=rest[17].copy()
   weight=ease(t/.25) if phase=='in' else 1-ease(t) if phase=='out' else 1.
   for j in (7,8,9):
    g=rig.fk(pose);rig.orient(pose,j,g[j].to_quaternion().slerp(sourceG[j].to_quaternion(),weight).to_matrix())
   targets=[restG[j].translation+(sourceG[j].translation-refG[j].translation)*.4 for j in (21,25)]
   rotations=[restG[j].to_3x3()@refG[j].to_3x3().inverted()@sourceG[j].to_3x3() for j in (21,25)]
   pin_feet(rig,pose,targets,rotations);g=rig.fk(pose)
   errors.append(max((g[j].translation-targets[k]).length for k,j in enumerate((21,25))));heights.append(pose[2].translation.z)
   assert max((pose[j].translation-rig.bind[j].translation).length for j in range(3,27) if j!=17)<1e-5
   poses.append(pose)
  assert max(errors)<.002 and min(heights)>.5,(role,phase,max(errors),min(heights))
  bank.extend(struct.pack('<IIIIff',322+offset,121,0,0,0.,duration/1000))
  for pose in poses+[poses[0]]:
   for matrix in pose:bank.extend(struct.pack('<7f',*row(matrix)))
  L.save(rig,role,'native_doppelganger_'+phase+'_crouched',poses,[identity() for _ in poses],'powers','Full-body Posture-100 crouched Doppelganger acting with animated torso, supported feet, changing casting-arm directions and native sword grip/socket. Native489 supplies context/timing only. No fixed first-person wrist targets, root writes or gameplay/FX events. Candidate requires actual-game visual inspection; Corvo offline skeleton comparison only.',[meta['sha256']],native_duration_ms=duration,stance=1,armed=True,max_foot_target_error=max(errors),minimum_pelvis_height=min(heights))
  clips.append(dict(id=322+offset,variant=29+offset,phase=phase,max_foot_error=max(errors),minimum_pelvis_height=min(heights)))
 path=OUT/f'{role}-doppelganger-crouch-bank.bin';path.write_bytes(bank);assert bank[32:len(data)]==data[32:]
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest().upper(),format='D2MOVE56',clips=325,prior_payload_unchanged=True,sources=clips,visual_accepted=False,runtime_verified=False))
L.finish('doppelganger-crouch-source-manifest-v490.json');(P/'doppelganger-crouch-bank-manifest-v490.json').write_text(json.dumps(reports,indent=2))
print('DOPPELGANGER_CROUCH_BANKS_GENERATED',flush=True)
