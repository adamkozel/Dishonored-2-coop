"""Full-body held-item studies with native prop geometry and hand references.

Body motion is authored from native full-body acting. FP data contributes
timing, finger articulation and the attachment LOCAL TO THE FREE WRIST only.
No player camera, wrist position, gameplay event or game file is modified.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
from finger_reference import HandReference,SR,SN
from edge_data import poses as edge_poses,skeleton
L=Library(['library-retarget-manifest.json','campaign-retarget-manifest.json'],'items-authored-v2')
refs=read(P/'fp-item-reference-manifest.json')['rows'];mechanisms=read(P/'item-mechanism-manifest.json')
models=read(P.parent.parent/'outputs/d2-coop-lab/research/asset-exports/natural-item-models/manifest.json')['Resources']
mine_skeleton=next(r for r in models if r['Name']=='models/weapons/grenade/stun_mine.md6skl')
(P/'item-mechanisms/stun_mine-skeleton.json').write_text(json.dumps(skeleton(mine_skeleton['ExportFile']),indent=2))
def reference(role,alias):
 matches=[r for r in refs if alias in r['aliases']]
 m=min(matches,key=lambda r:('/'+role+'/')not in r['asset']);data=edge_poses(P/m['sample_file']);assert data['sha256']==m['sample_sha256']
 return m,[[qts(v)for v in f]for f in data['frames']],(len(data['frames'])-1)/data['fps']

for role in ['corvo','emily']:
 rig=rig_for(role);hand=HandReference(rig)
 donors={a:L.load(role,a)for a in ['sword_idle','sword_gun_idle','sword_gun_fire','sword_gun_reload','sword_gun_equip_fast','sword_gun_unequip','sword_power_summontentacule','empty_distraction_clipboard_loop_talk']}
 gun=donors['sword_gun_idle'][1];neutral=donors['sword_idle'][1];summon=donors['sword_power_summontentacule'][1]
 peak=max(range(len(summon)),key=lambda f:rig.fk(summon[f])[9].translation.z)
 carrying=donors['empty_distraction_clipboard_loop_talk'][1]
 sword_arm=[13,14,15,16]
 for i in range(len(rig.joints)):
  parent=i
  while parent>=0 and parent!=16:parent=rig.joints[parent]['parent']
  if parent==16 and i!=16 and 'helper'not in rig.joints[i]['nativeName']:sword_arm.append(i)
 for kind,parts in [('crossbow',['equip','idle','fire','reload_in','reload_loop','reload_out','ooa','unequip']),('heart',['equip','idle','use','unequip']),('stunmine',['equip','idle','ready_in','ready_loop','place','reload','ready_out','ooa','unequip'])]:
  props_kind='crossbow_'+role if kind=='crossbow'else 'stun_mine'if kind=='stunmine'else'heart';saved={}
  for part in parts:
   rm,fp,duration=reference(role,'ply_leftarm_'+kind+'_'+part);count=max(2,round(duration*60)+1);poses=[];attachments=[];visibility=[]
   if part=='equip':donor='sword_gun_equip_fast'
   elif part=='unequip':donor='sword_gun_unequip'
   elif part in ['fire','ooa','place','use']:donor='sword_gun_fire'
   elif part.startswith('reload'):donor='sword_gun_reload'
   else:donor='sword_gun_idle'
   dm,body=donors[donor]
   for f in range(count):
    u=f/(count-1);phase=u
    if kind=='crossbow'and part.startswith('reload_'):
     a,b={'reload_in':(0,.3),'reload_loop':(.3,.7),'reload_out':(.7,1)}[part];phase=a+(b-a)*u
    p=sample(body,phase,part in ['idle','ready_loop','reload_loop'])
    if kind=='crossbow'and part=='fire':p=blend(sample(gun,u,True),p,.55)
    if part=='ooa':p=blend(sample(gun,u,True),p,.18)
    if kind in ['heart','stunmine']:
     # A native carrying posture keeps the elbow bent near the body, unlike
     # the initial gun-like extended arm. Preserve its full-body balance
     # motion while the right arm carries the native sword independently.
     weight=ease(u)**2 if part=='equip'else 1-ease(u)**2 if part=='unequip'else 1
     moving=sample(carrying,u,True)
     if part in ['use','place']:
      motion=sample(summon,u*.42)if part=='use'else p
      moving=blend(moving,motion,.5*math.sin(math.pi*u)**2)
     if part.startswith('ready'):moving=blend(moving,sample(gun,u,True),.45*(ease(u)if part=='ready_in'else 1-ease(u)if part=='ready_out'else 1))
     p=blend(p,moving,weight)
     weapon_arm=sample(neutral,u,True)
     for i in sword_arm:p[i]=weapon_arm[i].copy()
    fp_pose=sample(fp,u,part in ['idle','ready_loop','reload_loop']);hand.apply(p,fp_pose);preview_helpers(rig,p)
    fg=SR.fk(fp_pose);attachment=hand.invcal.to_4x4()@fg[4].inverted()@fg[SN['left_hand_attachment_jnt']]
    poses.append(p);attachments.append(row(attachment));visibility.append(u>=.2 if part=='equip'else u<=.78 if part=='unequip'else True)
   mechanism_name=('crossbow_fire'if part=='fire'else'crossbow_outofammo'if part=='ooa'else'crossbow_idle')if kind=='crossbow'else'heart_use'if kind=='heart'and part=='use'else'heart_idle'if kind=='heart'else None
   mechanism=next((r for r in mechanisms if r['id']==props_kind+'-'+str(mechanism_name)),None)
   prop=dict(kind=props_kind,model='item-models/'+props_kind+'.json',skeleton='item-mechanisms/'+props_kind+'-skeleton.json',parent_joint=9,attachment_track=attachments,visible=visibility,mechanism=mechanism,mechanism_policy='Native idle/fire/empty or Heart-use pose. Ordinary crossbow reload mechanism timing is not verified.',events=False)
   alias=kind+'_'+part
   notes='Full-body '+alias.replace('_',' ')+' study with the native item model, freely moving native body/arm acting, and first-person finger/timing references. Hand-local equipment attachment follows the body wrist. '
   if kind=='crossbow'and part.startswith('reload'):notes+='Body and hand reload phases are authored; the prop stays in its idle mechanism pose until the normal reload mechanism is verified. '
   if kind=='stunmine'and part=='place':notes+='Held-item placement gesture only: no surface contact, detachment or live trap. '
   notes+='Grip, contact, transitions, inventory and gameplay binding are pending.'
   provenance=dict(body_alias=donor,body_sha256=dm['sha256'],first_person=rm['asset'],first_person_sha256=rm['sample_sha256'])
   if kind in ['heart','stunmine']:provenance['carrying_body_sha256']=donors['empty_distraction_clipboard_loop_talk'][0]['sha256'];provenance['right_sword_arm_sha256']=donors['sword_idle'][0]['sha256']
   L.save(rig,role,alias,poses,[identity()for _ in poses],'ranged_and_items',notes,provenance,props=[prop])
   L.reports[-1]['equipment_policy']='sword_plus_native_'+kind
   (P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2))
   saved[part]=(poses,prop,L.reports[-1])
  order=['equip','idle','fire','reload_in','reload_loop','reload_out','idle','unequip']if kind=='crossbow'else['equip','idle','use','idle','unequip']if kind=='heart'else['equip','idle','ready_in','ready_loop','place','ready_out','unequip']
  poses=[];attach=[];visible=[];marks=[];source=[];segments=[]
  for part in order:
   fs,prop,m=saved[part];frames=fs[:61]if part=='idle'else fs;marks.append(dict(phase=part,at=len(poses)/60));start=len(poses);source.append(dict(alias=m['selection']['alias'],sha256=m['sha256']))
   for k,pose in enumerate(frames):
    p=blend(poses[-1],pose,.35)if poses and k<4 else copy_pose(pose);preview_helpers(rig,p);poses.append(p);attach.append(prop['attachment_track'][k]);visible.append(prop['visible'][k])
   segments.append(dict(start=start/60,end=len(poses)/60,mechanism=prop['mechanism']))
  prop=dict(saved['idle'][1]);prop.update(attachment_track=attach,visible=visible,mechanism_segments=segments)
  L.save(rig,role,kind+'_sequence',poses,[identity()for _ in poses],'ranged_and_items','Connected '+kind+' review study. Actual native prop and hand articulation, independent complete-body acting; equipment/contact and game-state binding remain unverified. Crossbow normal reload mechanism and trap release/surface contact are not demonstrated.',dict(clips=source),props=[prop],phase_marks=marks)
  L.reports[-1]['equipment_policy']='sword_plus_native_'+kind
  (P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2))
L.finish('items-retarget-manifest-v2.json')
