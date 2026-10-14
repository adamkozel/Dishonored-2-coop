"""Swim-to-edge and ledge exit studies, with event-free obstacle contacts."""
import sys,math,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
from contact_hand import descendants,grip_center
# Reuse definitions only; importing the authoring script would regenerate it.
source=(P/'author_top_transitions.py').read_text();definitions=source[source.index('def fixture('):source.index("for role in ['corvo','emily']:")];exec(definitions)
L=Library(['mixamo-retarget-manifest-v5.json','library-retarget-manifest.json'],'water-exit-v2')
for role in ['corvo','emily']:
 rig=rig_for(role);sm,swim=L.load(role,'mixamo_swimming_to_edge');cm,climb=L.load(role,'mixamo_freehang_climb');im,idle=L.load(role,'empty_idle');gm,gr=L.load(role,'sword_doppelganger_assassination_fastback');grasp=sample(gr,.24)
 edge=1.5;height=1.75;sole=sum(rig.fk(idle[0])[j].translation.z for j in [21,25])/2;begin=1.15
 cf=rig.fk(climb[-1]);cr=root_at(cm,cm['duration']);offz=height+sole-sum((cr@cf[j]).translation.z for j in [21,25])/2
 cg=rig.fk(sample(climb,begin/cm['duration']));rr=root_at(cm,begin);offx=edge+.025-sum((rr@cg[j]).translation.x for j in [9,16])/2;offset=Vector((offx,0,offz))
 sg=rig.fk(sample(swim,3/sm['duration']));sr=root_at(sm,3);soff=Vector((edge+.025-sum((sr@sg[j]).translation.x for j in [9,16])/2,0,0))
 model=fixture('water-ledge-'+role+'.json',edge,height)
 for ending in ['hold','exit']:
  duration=4.5 if ending=='hold' else 3+cm['duration']-begin+.8;ps=[];roots=[];tracks=[];errors=[];footerrors=[];last_swim=None
  for f in range(round(duration*60)+1):
   t=f/60
   if t<=3 or ending=='hold':
    at=min(t,3);p=sample(swim,at/sm['duration']);root=root_at(sm,at);root.translation+=soff
    if t>3:p=sample(swim,(3+(t-3)%1.0)/sm['duration']);p[2].translation+=root_at(sm,3+(t-3)%1.).translation-root_at(sm,3).translation
    for hand,start in [('left',1.7),('right',2.3)]:errors.append(hand_fit(rig,p,root,hand,Vector((edge+.025,.20 if hand=='left' else -.20,height+.025)),ease((t-start)/.3),grasp))
    last_swim=(copy_pose(p),root.copy())
   else:
    at=min(cm['duration'],begin+t-3);p=sample(climb,at/cm['duration']);root=root_at(cm,at);root.translation+=offset
    w=ease((t-3)/.4)
    # Blend in a common world frame, so joining two clips cannot teleport
    # the pelvis. The whole body changes pose, not just camera-facing arms.
    if w<1:
     prior,priorroot=last_swim;converted=copy_pose(prior);converted[2]=root.inverted()@priorroot@prior[2];p=blend(converted,p,w)
    for hand,end in [('left',1.88),('right',2.28)]:
     err=hand_fit(rig,p,root,hand,Vector((edge+.025,.20 if hand=='left' else -.20,height+.025)),1-ease((at-end)/.28),grasp);errors.append(err)
     if f%30==0 and err>.02:print('REACH',role,t,at,hand,round(err,4),flush=True)
    # Below the top surface, a boot cannot pass through the review wall.
    for a,b,j in [(19,20,21),(23,24,25)]:
     g=rig.fk(p);world=root@g[j].translation
     if world.z<height+sole*.75 and world.x>edge-.10:
      world.x=edge-.10;target=root.inverted()@world;rot=g[j].to_3x3();rig.solve(p,a,b,j,target,g[a].translation+Vector((.6,0,-.15)));rig.orient(p,j,rot);footerrors.append((rig.fk(p)[j].translation-target).length)
    if t>3+cm['duration']-begin:
     u=ease((t-(3+cm['duration']-begin))/.8);p=blend(p,sample(idle,t/im['duration'],True),u)
     targets=[]
     for j in [21,25]:
      end=cr@cf[j].translation+offset;end.z=height+sole;targets.append(root.inverted()@end)
     pin_feet(rig,p,targets)
   preview_helpers(rig,p);ps.append(p);roots.append(row(root));tracks.append(row(root.inverted()))
  prop=dict(kind='fixture_water_ledge',model=model,skeleton='contact-fixtures/skeleton.json',parent_joint=0,attachment_track=tracks,visible=[True]*len(ps),mechanism=None,fixture=True,events=False)
  alias='water_ledge_'+ending+'_sequence';L.save(rig,role,alias,ps,roots,'traversal','Swim approach and '+('held edge contact'if ending=='hold'else'full-body climb onto a ledge with standing recovery')+'. Native-length rigs use donor body motion with obstacle-relative palms and boot clearance. Freehang donor begins at 1.15s for a low water edge; joining, contact and weight transfer require visual review. Neutral fixture only; water, capsule, native ledge geometry and gameplay transition binding are absent.',dict(swim_sha256=sm['sha256'],climb_sha256=cm['sha256'],idle_sha256=im['sha256']),props=[prop],preview_placement=roots[0],max_full_contact_error_m=max(errors),max_boot_clearance_solve_error_m=max(footerrors,default=0),ledge_height_m=height,phase_marks=[dict(phase='swim_to_edge',at=0),dict(phase='climb'if ending=='exit' else 'hold',at=3)])
  L.reports[-1]['equipment_policy']='hands_free';(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2));print('WATER_EXIT_CONTACT',role,ending,max(errors),max(footerrors,default=0),flush=True)
L.finish('water-exit-manifest-v2.json')
