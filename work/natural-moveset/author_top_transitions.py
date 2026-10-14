"""Authored traversal transitions with obstacle-relative contacts.

World travel is metadata only. Native lengths and local first-person rig stay
unchanged. Contacts here are against neutral fixtures, not measured D2 geometry.
"""
import sys,math,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
from contact_hand import descendants,grip_center
L=Library(['mixamo-retarget-manifest-v5.json','library-retarget-manifest.json','ladder-contact-manifest-v2.json'],'top-transitions-v1')

def fixture(name,edge,height,ladder=False):
 vertices=[];ix=[]
 def box(c,s):
  a=len(vertices);vertices.extend([[c[k]+s[k]*v[k]/2 for k in range(3)]for v in [(-1,-1,-1),(-1,-1,1),(-1,1,-1),(-1,1,1),(1,-1,-1),(1,-1,1),(1,1,-1),(1,1,1)]])
  for q in [(0,4,6,2),(1,3,7,5),(0,1,5,4),(2,6,7,3),(0,2,3,1),(4,5,7,6)]:ix.extend(a+q[k]for k in [0,1,2,0,2,3])
 box((edge+1.0,0,height-.08),(2.,1.35,.16))
 if ladder:
  for y in [-.36,.36]:box((.35,y,(height+.72)/2),(.055,.055,height+.72))
  for k in range(round(height/.22)):box((.35,0,.11+k*.22),(.045,.77,.045))
 else:box((edge+.07,0,height/2),(.14,1.35,height))
 path=P/'contact-fixtures'/name;path.write_text(json.dumps(dict(vertices=vertices,indices=ix,weights=[[['fixture_root',1.]]for _ in vertices],scope='Neutral traversal review fixture, not native geometry')));return str(path.relative_to(P))

def root_at(m,t):return sample([[qts(v)]for v in m['root_track']],t/m['duration'])[0]
def hand_fit(rig,p,root,hand,goal,weight,reference):
 if weight<=0:return 0.
 g=rig.fk(p);before=copy_pose(p);j=9 if hand=='left'else 16;a,b=(7,8)if j==9 else(14,15)
 # Target a palm surface rather than declaring the wrist itself the grip.
 rot=g[j].to_3x3();center=grip_center(rig,reference,j);local=root.inverted()@goal-rot@center
 rig.solve(p,a,b,j,local,g[b].translation);rig.orient(p,j,rot)
 for k in descendants(rig,j):p[k]=reference[k].copy()
 for k in [a,b,j,*descendants(rig,j)]:p[k]=blend([before[k]],[p[k]],weight)[0]
 return ((root@rig.fk(p)[j])@center-goal).length if weight>.999 else 0.

for role in ['corvo','emily']:
 rig=rig_for(role);im,idle=L.load(role,'empty_idle');gm,gr=L.load(role,'sword_doppelganger_assassination_fastback');grasp=sample(gr,.24)
 tm,top=L.load(role,'mixamo_climbing_to_top');lm,ladder=L.load(role,'ladder_ascend_contact');height=2.97;offset=Vector((0,0,1.32));ps=[];roots=[];errors=[]
 finalg=rig.fk(top[-1]);finalroot=root_at(tm,tm['duration']);sole=sum(rig.fk(idle[0])[j].translation.z for j in [21,25])/2
 dz=height+sole-sum((finalroot@finalg[j]).translation.z for j in [21,25])/2-offset.z
 for f in range(289):
  t=f/60;u=min(t,tm['duration']);p=sample(top,u/tm['duration']);root=root_at(tm,u);root.translation+=offset+Vector((0,0,dz*ease(u/2.8)))
  if t<.3:p=blend(ladder[-1],p,ease(t/.3))
  for hand,start,end,anchor_t in [('left',0,.64,.10),('right',.75,1.92,1.40)]:
   a=sample(top,anchor_t/tm['duration']);g=rig.fk(a);j=9 if hand=='left' else 16;rt=root_at(tm,anchor_t);rt.translation+=offset+Vector((0,0,dz*ease(anchor_t/2.8)));goal=rt@(g[j]@grip_center(rig,grasp,j));goal.x=.35;goal.y=.36 if hand=='left' else -.36
   weight=ease((t-start)/.12)*(1-ease((t-end)/.18));errors.append(hand_fit(rig,p,root,hand,goal,weight,grasp))
  # Transfer onto the top surface and recover native standing, with feet
  # held at the donor's final support locations through the recovery.
  if t>2.8:
   targets=[];g=rig.fk(p)
   for j in [21,25]:
    end=finalroot@finalg[j].translation+offset+Vector((0,0,dz));end.z=height+sole
    targets.append(root.inverted()@end)
   w=ease((t-2.8)/.5)
   if t>4:p=blend(p,sample(idle,(t-4)/im['duration'],True),ease((t-4)/.8))
   g=rig.fk(p);pin_feet(rig,p,[g[j].translation.lerp(targets[k],w)for k,j in enumerate([21,25])])
  preview_helpers(rig,p);ps.append(p);roots.append(row(root))
 model=fixture('ladder-top-'+role+'.json',.50,height,True)
 tracks=[row(qts(r).inverted())for r in roots]
 prop=dict(kind='fixture_ladder_top',model=model,skeleton='contact-fixtures/skeleton.json',parent_joint=0,attachment_track=tracks,visible=[True]*len(ps),mechanism=None,fixture=True,events=False)
 alias='ladder_top_exit_sequence';L.save(rig,role,alias,ps,roots,'traversal','Full-body authored top climb followed by native standing recovery. The initial pose joins the prepared ascent loop. Hands contact a neutral ladder and feet settle on its top platform. Source travel is offline metadata; rung spacing, palm contact, capsule motion and live transition binding need review.',dict(top_sha256=tm['sha256'],ladder_sha256=lm['sha256'],idle_sha256=im['sha256']),props=[prop],preview_placement=roots[0],max_full_contact_error_m=max(errors),platform_height_m=height)
 L.reports[-1]['equipment_policy']='hands_free';(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2))
 print('TOP_CONTACT',role,max(errors),flush=True)
L.finish('top-transitions-manifest-v1.json')
