"""Isolated sliding takedown study using native target timing and body contacts.

Contact goals follow the target, not the local player's first-person hands.
No gameplay outcome or physics event is executed by these clips.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
from edge_data import poses as raw_poses
L=Library(['dynamics-retarget-manifest-v3.json','library-retarget-manifest.json','interaction-retarget-manifest.json','contextual-victim-retarget-manifest.json'],'slide-pair-v2');actors=[];victims=[];metrics=[]
events=read(P/'pair-research/native-context-event-times.json');scene=next(x for x in events if x['asset'].endswith('/choke_slide_back.midnightscene'))
for role in ['corvo','emily']:
 rig=rig_for(role);vrig=rig_for('corvo');em,entry=L.load(role,'slide_enter');km,kneel=L.load(role,'empty_distraction_loot_ground_in');om,out=L.load(role,'empty_distraction_loot_ground_out');gm,grasp=L.load(role,'sword_doppelganger_assassination_fastback');im,idle=L.load(role,'empty_idle')
 vm,vf=L.load('corvo','generic_'+role+'_takedown_slide_back');raw=raw_poses(P/vm['source']['sample_file']);place=qts(raw['frames'][0][1]).inverted();handoff=next(x['start_ms']/1000 for x in scene['rows']if x['class_name'].endswith('StartRagdoll')and '/'+role+'/'in x['player_body'])
 prefix=em['duration'];duration=prefix+vm['duration']+1.2;ps=[];roots=[];vs=[];vroots=[];errors=[];lastcontact=None
 for f in range(round(duration*60)+1):
  t=f/60;vt=max(0,min(vm['duration'],t-prefix));u=vt/vm['duration'];vu=min(vt,handoff)/vm['duration'];v=sample(vf,vu);vr=sample([[qts(r)]for r in vm['root_track']],vu)[0];vg=vrig.fk(v);world=place@vr;neck=(world@vg[11]).translation;chest=(world@vg[5]).translation
  if t<prefix:
   p=sample(entry,t/prefix);origin=Vector((-1.25+1.45*ease(t/prefix),0,0));contact=0
  elif t<prefix+vm['duration']:
   if role=='corvo':
    p=blend(entry[-1],sample(kneel,.7+.3*ease(vt/.75)),ease(vt/.55));origin=Vector((.20+.55*ease(vt/.45),-.36*ease(vt/.6),0))
   else:
    # Emily's target stays upright until the final shove. Use an upright
    # native grappling stance while Corvo's target is taken to the floor.
    p=blend(entry[-1],sample(grasp,.24),ease(vt/.65));origin=Vector((.20+.70*ease(vt/.5)+.45*ease((u-.55)/.45),0,0))
   contact=ease((vt-.16)/.25)*(1-ease((u-.70)/.24))
   if role=='corvo':
    yaw=-.5*ease(vt/.5)*(1-ease((u-.72)/.24));rotation=Matrix.Rotation(yaw,3,'Z');rig.orient(p,2,rotation@rig.fk(p)[2].to_3x3());p[2].translation=rotation@p[2].translation
   free=copy_pose(p);g=rig.fk(p)
   if vt<.30:
    w=math.sin(math.pi*vt/.30);ankle=(world@vg[25]).translation+Vector((-.08,0,.06))-origin;goal=g[25].translation.lerp(ankle,w);rig.solve(p,23,24,25,goal,g[24].translation)
   if contact>0:
    goals=[chest+Vector((-.12,.12,.06))-origin,neck+Vector((-.12,-.08,.04))-origin]
    for (a,b,j),goal,side in zip([(7,8,9),(14,15,16)],goals,[1,-1]):rig.solve(p,a,b,j,goal,g[a].translation+Vector((-.10,.38*side,-.06)))
    p=blend(free,p,contact)
    if contact>.999:
     g=rig.fk(p);errors.append(max((g[j].translation-goal).length for j,goal in zip([9,16],goals)))
   lastcontact=copy_pose(p)
  else:
   phase=(t-prefix-vm['duration'])/1.2;target=sample(out,phase)if role=='corvo'else sample(idle,phase,True);p=blend(lastcontact,target,ease(phase/.4));origin=Vector((.75-.5*ease(phase),-.36 if role=='corvo'else 0,0))if role=='corvo'else Vector((1.35-.5*ease(phase),0,0))
  preview_helpers(rig,p);ps.append(p);roots.append(identity(*origin));vs.append(v);vroots.append(row(world))
 first=qts(roots[0]);roots=[row(first.inverted()@qts(r))for r in roots];alias='sliding_nonlethal_back_study'
 L.save(vrig,'corvo','victim_'+role+'_'+alias,vs,vroots,'paired_victim_reference','Native slide victim motion held at its exact ragdoll event. Post-handoff physics is absent.',dict(victim_sha256=vm['sha256'],scene_sha256=scene['sha256'],handoff_seconds=handoff));target=L.reports[-1];victims.append(target)
 partner=dict(role='corvo',id=target['id'],binary=target['binary'],sha256=target['sha256'],frames=len(vs),fps=60,duration=(len(vs)-1)/60,root_track=vroots,placement=vroots[0],appearance='Neutral-grey native target',placement_basis='Native victim initial anchor; not a verified runtime paired frame')
 L.save(rig,role,alias,ps,roots,'paired_actions','Sliding rear takedown contact study: full-body compression, a low sweep, target-relative contact and recovery. Native victim clocks differ by protagonist. Initial sweep, reach, weight transfer, target clearance and pose-to-ragdoll transition require visual review. No damage, ragdoll or runtime pairing occurs.',dict(slide_sha256=em['sha256'],kneel_sha256=km['sha256'],grasp_sha256=gm['sha256'],victim_sha256=vm['sha256'],scene_sha256=scene['sha256']),partner=partner,preview_placement=row(first),phase_marks=[dict(phase='sliding_entry',at=0),dict(phase='target_action',at=prefix),dict(phase='native_ragdoll_reference_only',at=prefix+handoff)],max_full_contact_error_m=max(errors,default=0))
 L.reports[-1]['equipment_policy']='hands_free';(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2));actors.append(L.reports[-1]);metrics.append(dict(role=role,max_full_contact_error_m=max(errors,default=0)));print('SLIDE_CONTACT',role,max(errors,default=0),flush=True)
(P/'slide-pair-manifest-v2.json').write_text(json.dumps(actors,indent=2));(P/'slide-pair-victims-v2.json').write_text(json.dumps(victims,indent=2));(P/'pair-research/slide-pair-screening-v2.json').write_text(json.dumps(metrics,indent=2))
