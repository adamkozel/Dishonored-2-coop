"""Bounded full-body floor-to-shoulder hoist study; no game mutations.

Native dead-body/loaded-body poses form a target, not camera-space constraints.
Hands seek the actual carried body while legs, hips and trunk lift together.
"""
import sys,math,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
from contact_hand import descendants
L=Library(['carry-body-manifest-v3.json','carry-victim-retarget-manifest.json','library-retarget-manifest.json','interaction-retarget-manifest.json'],'body-lift-v2');actors=[];victims=[];diagnostics=[]
vrig=rig_for('corvo');dm,death=L.load('corvo','empty_stinghit_death');vm,vin=L.load('corvo','generic_carrycorpse_in');bm,vbase=L.load('corvo','generic_carrycorpse_idle')
for role in ['corvo','emily']:
 rig=rig_for(role);im,idle=L.load(role,'empty_idle');cm,loaded=L.load(role,'carry_body_idle');gm,grasp=L.load(role,'sword_doppelganger_assassination_fastback');gr=sample(grasp,.24)
 attach=qts(cm['partner']['root_track'][0]);rotation=attach.to_3x3();floor=copy_pose(death[-1]);fg=vrig.fk(floor);axis=fg[12].translation-fg[2].translation
 yaw=-math.pi/2-math.atan2(axis.y,axis.x);fr=Matrix.Rotation(yaw,3,'Z');floor[2]=rotation.inverted().to_4x4()@fr.to_4x4()@floor[2];floor[2].translation=Vector()
 fg=vrig.fk(floor);low=min((rotation@fg[j].translation).z for j in [2,5,9,12,16,21,25]);floorcenter=Vector((.42,.03,.105-low));finalcenter=(attach@vrig.fk(vbase[0])[2]).translation
 g0=rig.fk(idle[0]);feet=[g0[j].translation.copy()for j in [21,25]];feet[0].y+=.04;feet[1].y-=.04
 ps=[];vs=[];roots=[];vroots=[];errors=[];duration=4.8
 for f in range(round(duration*60)+1):
  t=f/60;down=ease((t-.2)/1.15)*(1-ease((t-1.7)/1.65));lift=ease((t-1.55)/1.8)
  p=sample(idle,t/im['duration'],True);p[2].translation+=Vector((.035,0,-.64))*down
  for j,w in [(2,.55),(3,.20),(4,.15),(5,.10)]:rig.orient(p,j,Matrix.Rotation(math.radians(75*w*down),3,'Y')@rig.fk(p)[j].to_3x3())
  g=rig.fk(p)
  for k,(a,b,j)in enumerate([(19,20,21),(23,24,25)]):
   rot=g[j].to_3x3();rig.solve(p,a,b,j,feet[k],g[a].translation+Vector((.8,0,-.1)));rig.orient(p,j,rot)
  # A complete rigid-body position plus local articulated shape. No bone
  # length scaling is used to pull a target into reach.
  vp=sample(vin,max(0,(t-1.9)/1.4));vp[2].translation=Vector();v=blend(floor,vp,ease((t-1.55)/1.15));end=copy_pose(vbase[0]);end[2].translation=Vector();v=blend(v,end,ease((t-2.95)/.40))
  center=floorcenter.lerp(finalcenter,lift);world=rotation.to_4x4();world.translation=center;vg=vrig.fk(v);free=copy_pose(p);g=rig.fk(p)
  left=world@vg[19].translation.lerp(vg[20].translation,.72)+Vector((.015,-.065,.035));right=world@vg[5].translation+Vector((-.015,-.035,-.05))
  w=ease((t-.8)/.65);wr=w*(1-ease((t-2.55)/.60))
  # The crouched torso follows the weight during the two-handed phase.
  if wr>.05:
   mid=(left+right)/2;shoulder=(g[7].translation+g[14].translation)/2
   shift=(mid+Vector((-.22,0,.25))-shoulder)*wr*.65
   shift.z=max(shift.z,.28-p[2].translation.z); p[2].translation+=shift;g=rig.fk(p)
   for k,(a,b,j)in enumerate([(19,20,21),(23,24,25)]):
    rot=g[j].to_3x3();rig.solve(p,a,b,j,feet[k],g[a].translation+Vector((.8,0,-.1)));rig.orient(p,j,rot)
  for a,b,j,target,weight,sign in [(7,8,9,left,w,1),(14,15,16,right,wr,-1)]:
   prior=copy_pose(p);g=rig.fk(p);rot=rig.fk(gr)[j].to_3x3();rig.solve(p,a,b,j,target,g[a].translation+Vector((.1,.3*sign,-.3)));rig.orient(p,j,rot)
   for k in descendants(rig,j):p[k]=gr[k].copy()
   for k in [a,b,j,*descendants(rig,j)]:p[k]=blend([prior[k]],[p[k]],weight)[0]
   if weight>.999:errors.append(dict(t=t,joint=j,error=(rig.fk(p)[j].translation-target).length))
  if t>3.35:p=blend(p,loaded[0],ease((t-3.35)/.6))
  if t>3.2:
   lg=rig.fk(loaded[0]);g=rig.fk(p)
   for k,(a,b,j)in enumerate([(19,20,21),(23,24,25)]):
    step=ease((t-3.2-k*.45)/.55);target=feet[k].lerp(lg[j].translation,step);target.z+=.065*math.sin(math.pi*step)
    rot=g[j].to_3x3();rig.solve(p,a,b,j,target,g[a].translation+Vector((.8,0,-.1)));rig.orient(p,j,rot)
  preview_helpers(rig,p);preview_helpers(vrig,v);ps.append(p);vs.append(v);roots.append(identity());vroots.append(row(world))
 alias='carry_body_floor_lift_study';L.save(vrig,'corvo','victim_'+role+'_'+alias,vs,vroots,'carried_body_reference','Native death, body pickup and loaded shapes on an authored floor-to-shoulder path; no physics or live NPC.',dict(death_sha256=dm['sha256'],pickup_sha256=vm['sha256'],loaded_sha256=bm['sha256']));target=L.reports[-1];victims.append(target)
 partner=dict(role='corvo',id=target['id'],binary=target['binary'],sha256=target['sha256'],frames=len(vs),fps=60,duration=duration,root_track=vroots,placement=vroots[0],appearance='Neutral native carried-body reference',placement_basis='Authored floor-to-shoulder path; not runtime paired validation')
 L.save(rig,role,alias,ps,roots,'interactions','Experimental full-body floor-to-shoulder hoist. Knees and hips lower, both hands gather the body, legs drive the lift and the shoulder settles under the load. Native target shapes are blended along an authored path. This is a visual feasibility candidate: contacts, body collision, weight, equipment transitions and native NPC ownership remain unverified.',dict(idle_sha256=im['sha256'],loaded_sha256=cm['sha256'],grasp_sha256=gm['sha256']),partner=partner,phase_marks=[dict(phase='approach',at=0),dict(phase='two_hand_gather',at=1.45),dict(phase='hoist',at=1.9),dict(phase='shoulder_settle',at=3.35)])
 L.reports[-1]['equipment_policy']='hands_free';(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2));actors.append(L.reports[-1]);diagnostics.append(dict(role=role,max_error=max(e['error']for e in errors),worst=sorted(errors,key=lambda e:e['error'],reverse=True)[:8]));print('LIFT_CONTACT',diagnostics[-1],flush=True)
(P/'body-lift-manifest-v2.json').write_text(json.dumps(actors,indent=2));(P/'body-lift-victims-v2.json').write_text(json.dumps(victims,indent=2));(P/'carry-research/lift-screening-v2.json').write_text(json.dumps(diagnostics,indent=2))


