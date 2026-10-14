"""Nonlethal paired hold pilot: contact belongs to the victim, never the camera."""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['library-retarget-manifest.json','choke-victim-retarget-manifest.json'],'choke-contact-v1');actors=[];victim_reports=[]
for role in ['corvo','emily']:
 rig=rig_for(role);vrig=rig_for('corvo');am,idle=L.load(role,'sword_idle');gm,grab=L.load(role,'sword_doppelganger_assassination_fastback');vm,victim=L.load('corvo',f'generic_{role}_choke_loop')
 reference=sample(grab,.24);referenceg=rig.fk(reference);leftrot=referenceg[9].to_3x3();rightrot=referenceg[16].to_3x3();finger_ids=[]
 for i,j in enumerate(rig.joints):
  parent=i
  while parent>=0 and parent not in [9,16]:parent=rig.joints[parent]['parent']
  if parent in [9,16]and i not in [9,16]and'helper'not in j['nativeName']:finger_ids.append(i)
 duration=2*vm['duration'];count=round(duration*60)+1;ps=[];vs=[];errors=[];placement=Matrix.Translation((1,0,0));initial_feet=[rig.fk(idle[0])[j].translation.copy()for j in [21,25]]
 vg0=vrig.fk(victim[0]);neck0=(placement@vg0[11]).translation
 for f in range(count):
  phase=f/60/vm['duration'];v=sample(victim,phase,True);vg=vrig.fk(v);neck=(placement@vg[11]).translation;delta=neck-neck0
  p=sample(idle,f/60/max(am['duration'],.1),True);p[2].translation+=Vector((.14,0,-.065))+delta*.3
  for j,fraction in [(3,.35),(4,.35),(5,.3)]:rig.orient(p,j,Matrix.Rotation(math.radians(8*fraction),3,'Y')@rig.fk(p)[j].to_3x3())
  pin_feet(rig,p,initial_feet)
  # Choke contact curve: one forearm crosses below the chin; the other
  # hand braces that wrist. Body/neck movement carries both contact goals.
  left=neck+Vector((.08,-.15,-.005));right=left+Vector((-.025,-.045,-.025))
  rig.solve(p,7,8,9,left,neck+Vector((.20,.35,-.05)))
  rig.solve(p,14,15,16,right,neck+Vector((-.03,-.50,-.28)))
  for wrist,rotation in [(9,leftrot),(16,rightrot)]:rig.orient(p,wrist,rotation)
  for j in finger_ids:p[j]=reference[j].copy()
  preview_helpers(rig,p);g=rig.fk(p);errors.append(max((g[9].translation-left).length,(g[16].translation-right).length));ps.append(p);vs.append(v)
 L.save(vrig,'corvo','victim_'+role+'_choke_hold',vs,[identity()for _ in vs],'paired_victim_reference','Native victim loop repeated at its original cadence; not a player action.',dict(native_victim_sha256=vm['sha256']))
 target=L.reports[-1];victim_reports.append(target)
 partner=dict(role='corvo',id=target['id'],binary=target['binary'],sha256=target['sha256'],frames=target['frames'],fps=60,duration=duration,root_track=target['root_track'],placement=row(placement),source=vm['source']['asset'],appearance='Neutral-grey native-rig stand-in',placement_basis='Native loop anchor offset plus authored victim-relative contact targets')
 L.save(rig,role,'paired_nonlethal_hold',ps,[identity()for _ in ps],'paired_actions','Nonlethal full-body hold pilot against a native moving victim. Planted feet, weight shift and spine balance support two victim-relative arm contacts. No camera-space targets or gameplay events. Entry, release, knockout, grip shaping and visual acceptance remain pending.',dict(idle_sha256=am['sha256'],grab_hand_sha256=gm['sha256'],victim_sha256=vm['sha256']),partner=partner,contact_error_max_m=max(errors))
 L.reports[-1]['equipment_policy']='hands_free';actors.append(L.reports[-1]);(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2));print('CONTACT_ERROR',role,max(errors),flush=True)
(P/'choke-contact-manifest-v1.json').write_text(json.dumps(actors,indent=2));(P/'choke-contact-victims-v1.json').write_text(json.dumps(victim_reports,indent=2))
