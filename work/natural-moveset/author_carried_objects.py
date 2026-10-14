"""Connected object handling with native full-body kneeling and locomotion.

The neutral box is a contact fixture, not a native object or gameplay event.
World movement and released-object physics remain network/gameplay owned.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
from contact_hand import descendants,grip_center,fit_contact
from finger_reference import HandReference,palm
from edge_data import poses as edge_poses
fpmeta=next(r for r in read(P/'fp-item-reference-manifest.json')['rows']if'ply_leftarm_crossbow_idle'in r['aliases']);fpraw=edge_poses(P/fpmeta['sample_file']);fp=[qts(v)for v in fpraw['frames'][0]]
L=Library(['library-retarget-manifest.json','interaction-retarget-manifest.json','gait-review-manifest.json'],'object-carry-v2')
for role in ['corvo','emily']:
 rig=rig_for(role);names={j['nativeName']:i for i,j in enumerate(rig.joints)};im,idle=L.load(role,'empty_idle');wm,walk=L.load(role,'walk_forward');lm,lower=L.load(role,'empty_distraction_loot_ground_in');um,upper=L.load(role,'empty_distraction_loot_ground_out');gm,grasp=L.load(role,'sword_idle');reference=sample(grasp,0);HandReference(rig).apply(reference,fp)
 grips={j:grip_center(rig,reference,j)for j in [9,16]};orientations={}
 for side,wrist,sign in [('left',9,-1),('right',16,1)]:
  local=palm(rig.global_bind,wrist,names[side+'_index01_jnt'],names[side+'_pinky02_jnt'],names[side+'_middle01_jnt']);forward=Vector((1,0,0));normal=Vector((0,sign,0));across=forward.cross(normal);desired=Matrix((across,forward,normal)).transposed();orientations[wrist]=desired@local.inverted()
 def center(p):
  g=rig.fk(p);return(g[7].translation+g[14].translation)*.5+Vector((.27,0,-.33))
 floor_center=center(lower[-1]);floor_center.z=max(.165,floor_center.z)
 saved={}
 for action in ['pickup','idle','walk','place','throw']:
  duration={'pickup':7.6,'idle':3.,'walk':3.5,'place':7.6,'throw':3.0}[action];count=round(duration*60)+1;ps=[];roots=[];tracks=[];errors=[];marks=[];release=None
  for f in range(count):
   t=f/60;u=t/duration;root=qts(identity());held=1.;contact=1.;free_after=0
   if action in ['pickup','place']:
    clock=t if action=='pickup'else duration-t
    if clock<4:
     p=sample(lower,clock/4);contact=ease((clock-3.05)/.85);c=floor_center.copy()
    elif clock<4.35:p=copy_pose(lower[-1]);c=floor_center.copy()
    else:
     phase=(clock-4.35)/(duration-4.35);p=sample(upper,phase);p=blend(lower[-1],p,ease(phase/.12));c=center(p);c=floor_center.lerp(c,ease(phase/.10))
    prop=qts(identity(*c));weight=contact
   elif action=='walk':
    speed=.65;source_speed=(qts(wm['root_track'][-1]).translation-qts(wm['root_track'][0]).translation).length/wm['duration'];p=sample(walk,t*speed/source_speed/wm['duration'],True);p[2].translation.x-=.015;p[2].translation.z-=.02;c=center(p);prop=qts(identity(*c));root=qts(identity(speed*t,0,0));weight=1
   elif action=='idle':
    p=sample(idle,t/im['duration'],True);p[2].translation.x-=.015;p[2].translation.z-=.02;c=center(p);prop=qts(identity(*c));weight=1
   else:
    p=sample(idle,t/im['duration'],True);base=rig.fk(p);feet=[base[j].translation.copy()for j in [21,25]];wind=ease(t/.6)*(1-ease((t-.65)/.45));drive=ease((t-.62)/.48)*(1-ease((t-1.35)/.8));p[2].translation+=Vector((.16*drive,0,-.12*wind));feet[0].x+=.22*drive;feet[0].z+=.04*math.sin(math.pi*drive)
    for j,w in [(2,.12),(3,.28),(4,.3),(5,.3)]:rig.orient(p,j,Matrix.Rotation(math.radians((18*drive-8*wind)*w),3,'Y')@Matrix.Rotation(math.radians(-12*w*wind),3,'Z')@rig.fk(p)[j].to_3x3())
    pin_feet(rig,p,feet);c=center(p);c.x+=-.14*wind+.17*drive;c.z+=.10*drive-.045*wind;prop=qts(identity(*c));weight=1-ease((t-1.35)/.45)
    if t>=1.15:
     if release is None:release=c.copy()
     flight=t-1.15;impact=(1.8+math.sqrt(1.8**2+19.62*(release.z-.165)))/9.81;airtime=min(flight,impact);settle=.12*(1-math.exp(-6*max(0,flight-impact)));prop.translation=release+Vector((2.5*airtime+settle,0,1.8*airtime-4.905*airtime*airtime));prop.translation.z=max(.165,prop.translation.z)
   free=copy_pose(p)
   for wrist,sign in [(9,1),(16,-1)]:
    for j in descendants(rig,wrist):p[j]=reference[j].copy()
    target=c+Vector((-.02,.205*sign,-.045));a=7 if wrist==9 else 14
    fit_contact(rig,p,wrist,target,orientations[wrist],rig.fk(p)[a].translation+Vector((.15,.30*sign,-.23)),grips[wrist])
   p=blend(free,p,weight);preview_helpers(rig,p)
   if weight>.999:
    errors.append(max((rig.fk(p)[j]@grips[j]-(c+Vector((-.02,.205*sign,-.045)))).length for j,sign in [(9,1),(16,-1)]))
   ps.append(p);roots.append(row(root));tracks.append(row(prop))
  assert max(errors,default=0)<.015,(role,action,max(errors))
  alias='carry_object_'+action;prop=dict(kind='fixture_box',model='contact-fixtures/box.json',skeleton='contact-fixtures/skeleton.json',parent_joint=0,attachment_track=tracks,visible=[True]*count,mechanism=None,fixture=True,events=False)
  L.save(rig,role,alias,ps,roots,'interactions','Full-body object '+action+' study using native ground kneeling/recovery and gait timing, two palm contacts and a neutral box fixture. Wrists are free before contact. '+('The release trajectory is an offline demonstration, not native physics.'if action=='throw'else'')+' Exact grip, object size/weight, floor contact and gameplay ownership remain to be validated.',dict(idle_sha256=im['sha256'],lower_sha256=lm['sha256'],upper_sha256=um['sha256'],walk_sha256=wm['sha256'],left_fingers_reference=fpmeta),props=[prop],max_hand_target_error_m=max(errors,default=0))
  L.reports[-1]['equipment_policy']='hands_free';(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2));print('OBJECT_CONTACT',role,action,max(errors,default=0),flush=True)
L.finish('object-carry-manifest-v2.json')
