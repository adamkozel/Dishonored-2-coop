"""Native-window target response with complete-body reach/pull/recovery acting.

The neutral window is an offline contact fixture. Scene event times are read
from exact-game assets; no outcomes, ragdolls or collision are executed.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
from edge_data import poses as raw_poses
L=Library(['window-victim-retarget-manifest.json','library-retarget-manifest.json','interaction-retarget-manifest.json'],'window-pair-v1');actors=[];victims=[];metrics=[]
scene=next(r for r in read(P/'pair-research/native-context-event-times.json')if r['asset'].endswith('/assassination_window_back.midnightscene'))
for role in ['corvo','emily']:
 rig=rig_for(role);vrig=rig_for('corvo');am,action=L.load(role,'sword_doppelganger_assassination_fastback');im,idle=L.load(role,'sword_idle');km,kneel=L.load(role,'empty_distraction_loot_ground_in')
 vm,vf=L.load('corvo','generic_'+role+'_assassination_window_back');raw=raw_poses(P/vm['source']['sample_file']);place=qts(raw['frames'][0][1]).inverted();handoff=next(x['start_ms']/1000 for x in scene['rows']if x['class_name'].endswith('StartRagdoll')and '/'+role+'/'in x['player_body'])
 duration=vm['duration']+.8;ps=[];roots=[];vs=[];vroots=[];tracks=[];errors=[];prior=None
 for f in range(round(duration*60)+1):
  t=f/60;u=min(1,t/vm['duration']);vu=min(t,handoff)/vm['duration'];v=sample(vf,vu);vr=sample([[qts(r)]for r in vm['root_track']],vu)[0];world=place@vr;vg=vrig.fk(v);neck=(world@vg[11]).translation
  if t<=vm['duration']:
   p=sample(action,u);origin=Vector((.45-.40*ease((u-.20)/.6),-.18*ease((u-.45)/.5),0));weight=ease((t-.12)/.22)*(1-ease((u-.60)/.22));g=rig.fk(p);feet=[g[j].translation.copy()for j in [21,25]]
   lean=20*ease(u/.2)*(1-ease((u-.4)/.4))
   for j,w in [(3,.35),(4,.35),(5,.3)]:rig.orient(p,j,Matrix.Rotation(math.radians(lean*w),3,'Y')@rig.fk(p)[j].to_3x3())
   p[2].translation.x+=.10*ease(u/.2)*(1-ease((u-.4)/.3));pin_feet(rig,p,feet)
   free=copy_pose(p);g=rig.fk(p);goals=[neck+Vector((-.10,.06,-.02))-origin,neck+Vector((-.38,-.08,.25))-origin]
   for (a,b,j),goal,side in zip([(7,8,9),(14,15,16)],goals,[1,-1]):rig.solve(p,a,b,j,goal,g[a].translation+Vector((-.1,.4*side,.05)))
   p=blend(free,p,weight)
   if weight>.999:errors.append(max((rig.fk(p)[j].translation-goal).length for j,goal in zip([9,16],goals)))
   prior=copy_pose(p)
  else:
   vphase=(t-vm['duration'])/.8;p=blend(prior,sample(idle,vphase,True),ease(vphase));origin=Vector((.05-.15*ease(vphase),-.18,0))
  preview_helpers(rig,p);ps.append(p);roots.append(identity(*origin));vs.append(v);vroots.append(row(world));tracks.append(row(qts(roots[-1]).inverted()@Matrix.Translation((.83,0,0))))
 first=qts(roots[0]);roots=[row(first.inverted()@qts(r))for r in roots];alias='window_rear_lethal_study'
 L.save(vrig,'corvo','victim_'+role+'_'+alias,vs,vroots,'paired_victim_reference','Native window victim clock, stopped at native ragdoll event; physical continuation absent.',dict(victim_sha256=vm['sha256'],scene_sha256=scene['sha256']));target=L.reports[-1];victims.append(target)
 partner=dict(role='corvo',id=target['id'],binary=target['binary'],sha256=target['sha256'],frames=len(vs),fps=60,duration=(len(vs)-1)/60,root_track=vroots,placement=vroots[0],appearance='Neutral-grey native victim',placement_basis='Inverse initial native partner anchor, unverified runtime frame')
 prop=dict(kind='fixture_window',model='contact-fixtures/window.json',skeleton='contact-fixtures/skeleton.json',parent_joint=0,attachment_track=tracks,visible=[True for _ in ps],mechanism=None,fixture=True,events=False)
 L.save(rig,role,alias,ps,roots,'paired_actions','Rear window attack study using native full-body attacker acting, a planted lean and target-relative arms. Original native victim timing and a neutral window fixture expose contact/collision gaps. Reach, blade direction, body clearance, ragdoll handoff and actual scene pairing require review.',dict(actor_sha256=am['sha256'],victim_sha256=vm['sha256'],scene_sha256=scene['sha256']),partner=partner,props=[prop],preview_placement=row(first),max_full_contact_error_m=max(errors,default=0),phase_marks=[dict(phase='native_window_action',at=0),dict(phase='ragdoll_reference_only',at=handoff)])
 actors.append(L.reports[-1]);metrics.append(dict(role=role,max_full_contact_error_m=max(errors,default=0)));print('WINDOW_CONTACT',role,max(errors,default=0),flush=True)
(P/'window-pair-manifest-v1.json').write_text(json.dumps(actors,indent=2));(P/'window-pair-victims-v1.json').write_text(json.dumps(victims,indent=2));(P/'pair-research/window-pair-screening-v1.json').write_text(json.dumps(metrics,indent=2))
