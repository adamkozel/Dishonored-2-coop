"""Ladder reference adaptation: common forward axis, bounded contact fitting.

Descending reverses the authored climb cycle. This is an explicit adaptation,
not a claim that a dedicated descent source was downloaded.
"""
import sys,math,statistics
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['mixamo-retarget-manifest-v5.json'],'ladder-contact-v1')
for role in ['corvo','emily']:
 rig=rig_for(role);m,src=L.load(role,'mixamo_climbing_ladder');turn=Matrix.Rotation(math.pi,3,'Z');base=[]
 floor=min(rig.fk(src[0])[j].translation.z for j in [21,25]);lift=floor-.12
 for s in src:
  p=copy_pose(s);rig.orient(p,2,turn@rig.fk(p)[2].to_3x3());p[2].translation=turn@p[2].translation;p[2].translation.z-=lift;preview_helpers(rig,p);base.append(p)
 cycle=1.1;height=.44;speed=height/cycle;ladder_x=.40 if role=='emily'else .35
 for action,sign in [('ascend',1),('descend',-1)]:
  duration=cycle*3;ps=[];roots=[];track=[];locked={};errors=[];moving=[];offset=height*3 if sign<0 else 0
  for f in range(round(duration*60)+1):
   t=f/60;u=(t/cycle*sign)%1;p=sample(base,u,True);g=rig.fk(p);root=Matrix.Translation((0,0,sign*speed*t));world_offset=Vector((0,0,offset));framecontacts=[]
   before=rig.fk(sample(base,(u-.005*sign)%1,True));after=rig.fk(sample(base,(u+.005*sign)%1,True))
   for a,b,j in [(7,8,9),(14,15,16),(19,20,21),(23,24,25)]:
    # Contact is identified from motion toward the body along the ladder,
    # then held in a world rung frame. Swinging limbs retain their donor.
    vz=(after[j].translation.z-before[j].translation.z)/(.01*cycle)
    weight=ease(((-sign*vz)-.13)/.20)
    world=root@g[j].translation+world_offset
    if weight>.02 and j not in locked:
     z=.11+round((world.z-.11)/.22)*.22;locked[j]=Vector((ladder_x,world.y,z))
    if weight<=.02:locked.pop(j,None)
    if j in locked:
     target=locked[j]-root.translation-world_offset
     # Ankle is behind and above toe/rung contact; palm contact needs
     # further mesh fitting. Native limb lengths are never changed.
     if j in [21,25]:target+=Vector((-.09,0,.05))
     else:target+=Vector((-.015,0,0))
     target=g[j].translation.lerp(target,weight);orientation=g[j].to_3x3();rig.solve(p,a,b,j,target,g[b].translation);rig.orient(p,j,orientation)
     if weight>.999:errors.append((rig.fk(p)[j].translation-target).length);framecontacts.append(j)
   preview_helpers(rig,p);ps.append(p);roots.append(row(root));track.append(identity(ladder_x,0,-root.translation.z-offset));moving.append(framecontacts)
  prop=dict(kind='fixture_ladder',model='contact-fixtures/ladder.json',skeleton='contact-fixtures/skeleton.json',parent_joint=0,attachment_track=track,visible=[True for _ in ps],mechanism=None,fixture=True,events=False)
  L.save(rig,role,'ladder_'+action+'_contact',ps,roots,'traversal','Whole-body '+action+' ladder study, rotated into native forward space with the source height removed. Three cycles use a 22cm neutral rung fixture and bounded limb contact fitting. '+('Descending explicitly reverses the climb donor; a separately authored descent would improve weight transfer. 'if sign<0 else'')+'Palm/toe surface fit, reach limits, entry/top/bottom transitions and native ladder binding remain pending.',dict(donor_sha256=m['sha256']),props=[prop],preview_placement=identity(0,0,offset),rung_spacing_m=.22,cycle_seconds=cycle,review_speed_mps=speed,max_full_contact_error_m=max(errors,default=0),contact_joints=moving)
  L.reports[-1]['equipment_policy']='hands_free';(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2));print('LADDER_CONTACT',role,action,max(errors,default=0),flush=True)
L.finish('ladder-contact-manifest-v1.json')
