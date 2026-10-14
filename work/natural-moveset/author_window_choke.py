"""Window pull into rear nonlethal hold, finish and release studies.

Only native target pose timing is authoritative here. Actor stepping,
contact and phase alignment are offline authoring choices, not outcomes.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
from edge_data import poses as raw_poses
from contact_hand import descendants
L=Library(['window-choke-victim-manifest.json','choke-victim-retarget-manifest.json','library-retarget-manifest.json','gait-review-manifest.json'],'window-choke-v2');actors=[];victims=[];metrics=[]
for role in ['corvo','emily']:
 rig=rig_for(role);vrig=rig_for('corvo');im,idle=L.load(role,'empty_idle');wm,walk=L.load(role,'walk_back');gm,grab=L.load(role,'sword_doppelganger_assassination_fastback');grasp=sample(grab,.24);gg=rig.fk(grasp)
 em,entry=L.load('corvo','generic_'+role+'_chokeback_window_in');raw=raw_poses(P/em['source']['sample_file']);place=qts(raw['frames'][0][1]).inverted();endworld=place@qts(em['root_track'][-1]);endg=vrig.fk(entry[-1]);endneck=(endworld@endg[11]).translation
 native={x:L.load('corvo','generic_'+role+'_'+x)for x in ['choke_loop','choke_win','choke_cancel']};lm,loop=native['choke_loop'];lg=vrig.fk(loop[0]);lr=qts(lm['root_track'][0])
 def facing(m):
  forward=m.to_3x3()@vrig.global_bind[5].to_3x3().inverted()@Vector((1,0,0));return math.atan2(forward.y,forward.x)
 align=Matrix.Rotation(facing(endworld@endg[5])-facing(lr@lg[5]),4,'Z');align.translation=endneck-align.to_3x3()@(lr@lg[11]).translation
 rootend=Vector((endneck.x-.44,endneck.y,0));stride=max(.01,(qts(wm['root_track'][-1]).translation-qts(wm['root_track'][0]).translation).length)
 for ending in ['hold','finish','release']:
  ps=[];vs=[];roots=[];vroots=[];tracks=[];marks=[];sources=[];errors=[];previous_v=None;previous_p=None;distance=0.;prev_origin=Vector((.60,0,0))
  order=['entry','choke_loop','choke_loop']+(['choke_win']if ending=='finish'else['choke_cancel']if ending=='release'else[])
  for phase in order:
   vm,vf=(em,entry)if phase=='entry'else native[phase];count=round(vm['duration']*60)+1;marks.append(dict(phase=phase,at=len(ps)/60));sources.append(dict(alias=vm['selection']['alias'],sha256=vm['sha256']));prior_v=copy_pose(previous_v)if previous_v else None;prior_p=copy_pose(previous_p)if previous_p else None
   for f in range(count):
    t=f/60;u=f/(count-1);v=sample(vf,u);vr=sample([[qts(r)]for r in vm['root_track']],u)[0];world=(place if phase=='entry'else align)@vr
    if prior_v is not None:v=blend(prior_v,v,ease(t/.10))
    vg=vrig.fk(v);neck=(world@vg[11]).translation
    if phase=='entry':
     origin=Vector((min(.60,neck.x-.44),neck.y*ease(u/.3),0));distance+=(origin-prev_origin).length;walking=ease((u-.12)/.20)*(1-ease((u-.82)/.18));p=blend(sample(idle,u,True),sample(walk,distance/stride,True),walking);body=ease(t/.25);contact=ease((t-.08)/.35)
    else:
     origin=rootend.copy();p=sample(idle,len(ps)/60/im['duration'],True);body=contact=1
     if phase=='choke_win':body=1-ease((u-.35)/.60);contact=1-ease((u-.12)/.30)
     elif phase=='choke_cancel':body=1-ease((u-.15)/.8);contact=1-ease((u-.03)/.45)
    p[2].translation+=Vector((.20,0,-.03))*body
    for j,w in [(3,.35),(4,.35),(5,.3)]:rig.orient(p,j,Matrix.Rotation(math.radians(8*w*body),3,'Y')@rig.fk(p)[j].to_3x3())
    if prior_p is not None:p=blend(prior_p,p,ease(t/.12))
    # The actor remains inside the window; the forward boot must not pass
    # through its lower wall while the backward gait prepares its next step.
    for a,b,c in [(19,20,21),(23,24,25)]:
     g=rig.fk(p);foot=g[c].translation.copy();foot.x=min(foot.x,.64-origin.x);orientation=g[c].to_3x3().copy();rig.solve(p,a,b,c,foot,g[a].translation+Vector((.65,0,-.12)));rig.orient(p,c,orientation)
    free=copy_pose(p);left=neck-origin+Vector((.055,.035,-.02));right=left+Vector((-.025,-.05,-.02));g=rig.fk(p)
    for a,b,j,goal,side in [(7,8,9,left,1),(14,15,16,right,-1)]:
     rig.solve(p,a,b,j,goal,g[a].translation+Vector((.20,.36*side,-.16)));rig.orient(p,j,gg[j].to_3x3())
     for k in descendants(rig,j):p[k]=grasp[k].copy()
    p=blend(free,p,contact);preview_helpers(rig,p)
    if contact>.999:errors.append(max((rig.fk(p)[9].translation-left).length,(rig.fk(p)[16].translation-right).length))
    ps.append(p);vs.append(v);roots.append(row(Matrix.Translation(origin)));vroots.append(row(world));tracks.append(row(Matrix.Translation(-origin)@Matrix.Translation((.83,0,0))));previous_v=v;previous_p=p;prev_origin=origin
  first=qts(roots[0]);roots=[row(first.inverted()@qts(r))for r in roots];alias='window_nonlethal_'+ending+'_sequence'
  L.save(vrig,'corvo','victim_'+role+'_'+alias,vs,vroots,'paired_victim_reference','Native window entry joined to native hold/finish/release at matched neck and horizontal facing; alignment is an offline hypothesis. Ragdoll continuation is absent.',dict(clips=sources));target=L.reports[-1];victims.append(target)
  partner=dict(role='corvo',id=target['id'],binary=target['binary'],sha256=target['sha256'],frames=len(vs),fps=60,duration=(len(vs)-1)/60,root_track=vroots,placement=vroots[0],appearance='Neutral-grey native victim',placement_basis='Inverse initial anchor then neck/yaw alignment to native hold, not runtime verified')
  prop=dict(kind='fixture_window',model='contact-fixtures/window.json',skeleton='contact-fixtures/skeleton.json',parent_joint=0,attachment_track=tracks,visible=[True]*len(ps),mechanism=None,fixture=True,events=False)
  L.save(rig,role,alias,ps,roots,'paired_actions','Window nonlethal '+ending+' study with native victim entry and rear phases. Complete-body backward stepping follows the pull; wrists target the victim, never FP camera space. Entry contact, weight transfer, foot clearance, final physical continuation and live paired authority remain unverified.',dict(idle_sha256=im['sha256'],walk_sha256=wm['sha256'],grasp_sha256=gm['sha256'],victim_clips=sources),partner=partner,props=[prop],preview_placement=row(first),preview_view=[-5.5,-7,4.0],phase_marks=marks,max_full_contact_error_m=max(errors,default=0))
  L.reports[-1]['equipment_policy']='hands_free';(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2));actors.append(L.reports[-1]);metrics.append(dict(role=role,alias=alias,max_full_contact_error_m=max(errors,default=0)));print('WINDOW_CHOKE',metrics[-1],flush=True)
(P/'window-choke-manifest-v2.json').write_text(json.dumps(actors,indent=2));(P/'window-choke-victims-v2.json').write_text(json.dumps(victims,indent=2));(P/'pair-research/window-choke-screening-v2.json').write_text(json.dumps(metrics,indent=2))


