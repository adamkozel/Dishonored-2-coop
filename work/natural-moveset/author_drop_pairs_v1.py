"""Bounded aerial paired-contact study. Native victim timing is retained.

Complete-body descent/compression uses authored aerial and native kneeling
acting. Contacts target the other body, never first-person camera space.
No damage, inventory or ragdoll events are evaluated.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
from contact_hand import descendants
from edge_data import poses as raw_poses
L=Library(['contextual-victim-retarget-manifest.json','mixamo-aerial-retarget-manifest.json','interaction-retarget-manifest.json','library-retarget-manifest.json'],'drop-pairs-v1');actors=[];victims=[];metrics=[]
for role in ['corvo','emily']:
 rig=rig_for(role);vrig=rig_for('corvo');am,air=L.load(role,'mixamo_sword_and_shield_attack');km,kneel=L.load(role,'empty_distraction_loot_ground_in');om,recover=L.load(role,'empty_distraction_loot_ground_out');im,idle=L.load(role,'sword_idle')
 airborne=sample(air,.36);g=rig.fk(airborne);airborne[2].translation.z-=min(g[j].translation.z for j in [21,25])-.14
 q=read(P.parent/('fullbody-proof/inputs/client-target-sword.json'if role=='corvo'else'crouch-grip-reference/host-target-sword.json'))['quaternion'];binding=Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix().to_4x4()
 for lethal in [False,True]:
  alias='aerial_'+('lethal'if lethal else'nonlethal')+'_back_study';vm,vf=L.load('corvo','generic_'+role+('_assassination_dropback'if lethal else'_takedown_dropback'));raw=raw_poses(P/vm['source']['sample_file']);place=qts(raw['frames'][0][1]).inverted();duration=.4+vm['duration']+1.2;count=round(duration*60)+1;ps=[];vs=[];roots=[];vroots=[];errors=[]
  for f in range(count):
   t=f/60;vt=max(0,min(vm['duration'],t-.4));u=vt/vm['duration'];v=sample(vf,u);vr=sample([[qts(r)]for r in vm['root_track']],u)[0];vg=vrig.fk(v);neck=(place@vr@vg[11]).translation;shoulder=(place@vr@vg[5]).translation
   if t<.4:
    p=copy_pose(airborne);origin=Vector((.52*ease(t/.4),0,1.8-ease(t/.4)))
    contact=0
   elif t<.4+vm['duration']:
    land=ease(vt/.48);p=blend(airborne,sample(kneel,.80+.20*ease(vt/.55)),land);origin=Vector((.52,0,.8*(1-land)));contact=ease((vt-.18)/.32)*(1-ease((u-.78)/.22));g=rig.fk(p)
    # Straddle the target with a planted lower body after impact.
    feet=[g[j].translation.copy()for j in [21,25]]
    for i,foot in enumerate(feet):foot.y+=.10*land*(1 if i==0 else-1)
    pin_feet(rig,p,feet)
   else:
    phase=(t-.4-vm['duration'])/1.2;p=sample(recover,phase);origin=Vector((.52,0,0));contact=0
   if lethal:
    for j in descendants(rig,16):p[j]=idle[0][j].copy()
    p[17]=idle[0][17].copy()
   free=copy_pose(p)
   if contact>0:
    left=shoulder+Vector((0,.12,.075))-origin;right=neck+Vector((-.10,-.08,.73 if lethal else .08))-origin;g=rig.fk(p)
    for a,b,j,goal,side in [(7,8,9,left,1),(14,15,16,right,-1)]:
     rig.solve(p,a,b,j,goal,g[a].translation+Vector((-.12,.42*side,.05)))
    if lethal:
     g=rig.fk(p);weapon=g[17]@binding;current=weapon.to_3x3()@Vector((1,0,0));direction=Vector((.10,.08,-.73)).normalized();rig.orient(p,16,current.rotation_difference(direction).to_matrix()@g[16].to_3x3())
    p=blend(free,p,contact)
    if contact>.999:errors.append(max((rig.fk(p)[9].translation-left).length,(rig.fk(p)[16].translation-right).length))
   preview_helpers(rig,p);ps.append(p);roots.append(identity(*origin));vs.append(v);vroots.append(row(place@vr))
  # Keep the presentation origin above the ground without altering the
  # body pose or adding motion to the D2 gameplay root.
  first=qts(roots[0]);roots=[row(first.inverted()@qts(r))for r in roots]
  L.save(vrig,'corvo','victim_'+role+'_'+alias,vs,vroots,'paired_victim_reference','Native contextual victim timing with an initial pre-contact hold.',dict(victim_sha256=vm['sha256']));target=L.reports[-1];victims.append(target)
  partner=dict(role='corvo',id=target['id'],binary=target['binary'],sha256=target['sha256'],frames=len(vs),fps=60,duration=(len(vs)-1)/60,root_track=vroots,placement=vroots[0],appearance='Neutral-grey native-rig target',source=vm['source']['asset'],placement_basis='Inverse native victim initial anchor, offline placement hypothesis')
  L.save(rig,role,alias,ps,roots,'paired_actions','Aerial '+('lethal'if lethal else'nonlethal')+' paired study: authored full-body descent, native kneeling compression and recovery, with exact-game victim timing. Contact is target-relative. Initial impact, weight transfer, full-range reach, sword/hand geometry, outcome, ragdoll and runtime pairing are unverified. Do not promote based on format or pose checks alone.',dict(aerial_sha256=am['sha256'],kneel_sha256=km['sha256'],recover_sha256=om['sha256'],victim_sha256=vm['sha256']),partner=partner,preview_placement=row(first),max_full_contact_error_m=max(errors,default=0),phase_marks=[dict(phase='descent',at=0),dict(phase='native_target_action',at=.4),dict(phase='recover',at=.4+vm['duration'])])
  L.reports[-1]['equipment_policy']='sword_ready'if lethal else'hands_free';(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2));actors.append(L.reports[-1]);metrics.append(dict(role=role,alias=alias,max_full_contact_error_m=max(errors,default=0)));print('DROP_CONTACT',role,alias,max(errors,default=0),flush=True)
(P/'drop-pairs-manifest-v1.json').write_text(json.dumps(actors,indent=2));(P/'drop-pairs-victims-v1.json').write_text(json.dumps(victims,indent=2));(P/'pair-research/drop-pair-screening-v1.json').write_text(json.dumps(metrics,indent=2))
