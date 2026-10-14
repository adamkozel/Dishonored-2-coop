"""Full-body power studies: native body acting plus FP timing/finger articulation.

No FP wrist position OR orientation is imposed on the third-person body.
These are review candidates, without effects, movement, damage or power events.
"""
import sys, math, re
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
from edge_data import poses as read_edge

L=Library(['library-retarget-manifest.json'],'power-authored-v1')
references=read(P/'fp-power-reference-manifest.json')['rows']
SR=NativeRig(read(P/'fp-leftarm-skeleton.json')['joints'])
SN={j['nativeName']:i for i,j in enumerate(SR.joints)}
refs={}
for m in references:
 data=read_edge(P/m['sample_file']);assert data['sha256']==m['sample_sha256']
 frames=[[qts(v)for v in f]for f in data['frames']]
 for alias in m['aliases']:refs[alias.removeprefix('ply_leftarm_power_')]=(m,frames,(len(frames)-1)/data['fps'])

from finger_reference import HandReference

# Different authored body actions/poses are retained; first-person names
# identify the reference, not a claim that a gameplay power is implemented.
SPECS=[
 dict(name='windblast',body='projectile',prefix='windblast',parts=['fire'],strength=1.0,tilt=0,arm=0),
 dict(name='doppelganger',body='summon',prefix='decoy',parts=['fire_in','fire_loop','fire_out'],strength=1.0,tilt=0,arm=0),
 dict(name='mesmerize',body='support',prefix='mesmerize',parts=['fire_in','fire_loop','fire_out'],strength=1.0,tilt=-2,arm=0),
 dict(name='bend_time',body='summon',prefix='bendtime',parts=['fire_in','fire_loop','fire_out'],strength=.78,tilt=-4,arm=22),
 dict(name='devouring_swarm',body='projectile',prefix='devourswarm',parts=['fire_in','fire_loop','fire_out'],strength=.85,tilt=9,arm=30),
 dict(name='dark_vision',body='summon',prefix='darkvision',parts=['fire'],strength=.65,tilt=-3,arm=-12),
 dict(name='blink',body='projectile',prefix='blink',parts=['fire_in','fire_loop','fire_out'],strength=.62,tilt=2,arm=-8),
 dict(name='possession',body='summon',prefix='possession',parts=['fire_in','fire_loop','fire_out'],strength=.8,tilt=3,arm=10),
 dict(name='far_reach',body='projectile',prefix='shadowgrapple',parts=['fire_in','fire_in_loop','fire_out','fire_out_loop','pull_in','pull_loop','pull_out'],strength=.9,tilt=4,arm=-4),
 dict(name='domino',body='support',prefix='phantasmal',parts=['select_in','select_loop','fire','select_out'],strength=.65,tilt=1,arm=32),
]

for role in ['corvo','emily']:
 rig=rig_for(role);hand=HandReference(rig);idle_m,idle=L.load(role,'sword_idle');base=idle[0]
 pm,projectile=L.load(role,'sword_power_projectileattack');sm,summon=L.load(role,'sword_power_summontentacule')
 su,up=L.load(role,'sword_power_support_in');sl,loop=L.load(role,'sword_power_support_loop');sd,down=L.load(role,'sword_power_support_out')
 donor={'projectile':(pm,projectile),'summon':(sm,summon)}
 for spec in SPECS:
  phase_rows=[];sequence=[];sources=[];phase_marks=[]
  if spec['body']=='support':peak=0;body_meta=su
  else:
   body_meta,body=donor[spec['body']]
   axis=0 if spec['body']=='projectile'else 2
   peak=max(range(len(body)),key=lambda f:rig.fk(body[f])[9].translation[axis])/(len(body)-1)
   peak=max(.2,min(.8,peak))
  previous=None
  for part_index,part in enumerate(spec['parts']):
   key=spec['prefix']+'_'+part;rm,rf,duration=refs[key];frames=max(2,round(duration*60)+1);phase=[]
   is_loop='loop'in part
   is_end=part_index==len(spec['parts'])-1 and len(spec['parts'])>1
   for f in range(frames):
    u=f/(frames-1)
    if len(spec['parts'])==1:
     body_u=u;weight=math.sin(math.pi*u)**.8
     p=sample(body,body_u)
    elif spec['body']=='support':
     raw=down if is_end else loop if is_loop or part=='fire'else up
     p=sample(raw,u,is_loop);weight=1-ease(u)if is_end else 1 if is_loop or part=='fire'else ease(u)
    else:
     if is_end:body_u=peak+(1-peak)*ease(u);weight=1-ease(u)
     elif part.startswith('pull'):
      body_u=peak+.24*ease(u);weight=1
     elif is_loop:body_u=peak+.013*math.sin(math.tau*u);weight=1
     elif part=='fire_out':body_u=peak+.09*ease(u);weight=1
     else:body_u=peak*ease(u);weight=ease(u)
     p=sample(body,body_u)
    p=blend(base,p,spec['strength'])
    # Broad body acting stays in the native donor. Small style changes
    # rotate the shoulder chain; no target hand position is solved.
    if spec['tilt']:
     for j in (3,4,5):rig.orient(p,j,Matrix.Rotation(math.radians(spec['tilt']*weight/3),3,'Y')@rig.fk(p)[j].to_3x3())
    if spec['arm']:rig.orient(p,7,Matrix.Rotation(math.radians(spec['arm']*weight),3,'Y')@rig.fk(p)[7].to_3x3())
    hand.apply(p,sample(rf,u,is_loop),min(1,ease(u*5))if part_index==0 else 1)
    # Match the complete-body entry/recovery to the common armed stance.
    if part_index==0:p=blend(base,p,ease(u*5))
    if is_end or len(spec['parts'])==1:p=blend(p,base,ease((u-.78)/.22))
    if previous is not None and f<7:p=blend(previous,p,ease(f/6))
    preview_helpers(rig,p);phase.append(p)
   if is_loop:
    for f in range(max(0,len(phase)-7),len(phase)):
     phase[f]=blend(phase[f],phase[0],ease((f-(len(phase)-7))/6));preview_helpers(rig,phase[f])
   previous=copy_pose(phase[-1]);phase_marks.append(dict(phase=part,start=len(sequence)/60,duration=(len(phase)-1)/60))
   sequence.extend(phase if not sequence else phase[1:])
   sources.append(dict(reference=key,sha256=rm['sample_sha256'],duration=duration))
   phase_rows.append((part,phase))
  provenance=dict(body_alias=body_meta['selection']['alias'],body_sha256=body_meta['sha256'],first_person=sources,finger_mapping={rig.joints[i]['nativeName']:SR.joints[s]['nativeName']for i,s in hand.mapping.items()})
  note=('Full-body '+spec['name'].replace('_',' ')+' study with native caster body acting and exact first-person phase durations/finger articulation. Wrists follow the native body; no camera-space hand constraints. Effects, targets and gameplay outcomes are absent. ')
  if spec['name']=='domino':note+='Phantasmal-to-Domino identification confirmed by the exported player component: Emily usage, phantasmal cast scene and bsp_power_domino_cast_fail sound. '
  for part,phase in phase_rows:
   L.save(rig,role,'power_'+spec['name']+'_'+part,phase,[identity()for _ in phase],'powers',note+'Standalone phase; transition/contact review pending.',provenance,reference_phase=part)
  L.save(rig,role,'power_'+spec['name']+'_sequence',sequence,[identity()for _ in sequence],'powers',note+'Connected phase demonstration; pose-only in-game validation and visual acceptance pending.',provenance,phase_marks=phase_marks)
  # Cancel is a distinct whole-body release from the prepared stance,
  # not an interrupted playback that leaves the upper body frozen.
  if len(phase_rows)>1:
   held=phase_rows[0][1][-1];cancel=[]
   for f in range(25):
    p=blend(held,base,ease(f/24));preview_helpers(rig,p);cancel.append(p)
   L.save(rig,role,'power_'+spec['name']+'_cancel',cancel,[identity()for _ in cancel],'powers',note+'Authored 0.4s full-body cancellation to armed stance; no power event.',provenance)
 L.finish('power-retarget-manifest-v1.json')
