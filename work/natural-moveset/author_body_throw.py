"""Authored carry throw acting with an explicitly illustrative target flight.

This creates no native damage, NPC ownership changes or physics events.
"""
import sys,math,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['carry-body-manifest-v3.json','carry-victim-retarget-manifest.json','library-retarget-manifest.json'],'body-throw-v1');actors=[];victims=[];metrics=[]
vrig=rig_for('corvo');vm,vbase=L.load('corvo','generic_carrycorpse_idle');hm,high=L.load('corvo','generic_carrycorpse_drophigh');airpose=sample(high,.28/hm['duration'])
for role in ['corvo','emily']:
 rig=rig_for(role);bm,loaded=L.load(role,'carry_body_idle');im,idle=L.load(role,'sword_idle');initial=qts(bm['partner']['root_track'][0]);ig=rig.fk(loaded[0]);release=None;impact=None;ps=[];vs=[];roots=[];vroots=[];errors=[];duration=3.1
 for f in range(round(duration*60)+1):
  t=f/60;wind=ease(t/.50)*(1-ease((t-.50)/.42));drive=ease((t-.50)/.48);recover=ease((t-1.15)/1.1);p=copy_pose(loaded[0]);feet=[ig[j].translation.copy()for j in [21,25]]
  p[2].translation+=Vector((-.045*wind+.14*drive,0,-.10*wind));feet[0].x+=.22*drive;feet[0].z+=.07*math.sin(math.pi*drive)
  for j,w in [(2,.2),(3,.3),(4,.3),(5,.2)]:rig.orient(p,j,Matrix.Rotation(math.radians((16*drive-7*wind)*w),3,'Y')@Matrix.Rotation(math.radians((23*drive-12*wind)*w),3,'Z')@rig.fk(p)[j].to_3x3())
  pin_feet(rig,p,feet);g=rig.fk(p)
  if t<=.98:
   v=sample(vbase,t/vm['duration'],True);vg=vrig.fk(v);rot=Matrix.Rotation(math.radians(30*drive-10*wind),3,'Z')@initial.to_3x3();world=rot.to_4x4()
   abdomen=g[7].translation+Vector((.025+.07*drive,.19+.03*drive,.115+.06*drive));world.translation=abdomen-rot@vg[4].translation
   goal=world@vg[19].translation.lerp(vg[20].translation,.72)+Vector((.015,-.065,.035));orient=g[9].to_3x3();rig.solve(p,7,8,9,goal,g[7].translation+Vector((.18,.3,-.1)));rig.orient(p,9,orient);errors.append((rig.fk(p)[9].translation-goal).length)
   release=dict(t=t,world=world.copy(),pose=copy_pose(v),center=(world@vg[2]).translation.copy(),rotation=rot.copy(),actor=copy_pose(p))
  else:
   elapsed=t-release['t'];v=blend(release['pose'],airpose,ease(elapsed/.32));vg=vrig.fk(v);rot=Matrix.Rotation(math.radians(-22*min(elapsed,.6)),3,'Y')@release['rotation'];world=rot.to_4x4()
   center=release['center']+Vector((2.3*elapsed,.32*elapsed,1.25*elapsed-4.905*elapsed*elapsed));world.translation=center-rot@vg[2].translation
   low=min((world@vg[j]).translation.z for j in [2,5,9,12,16,21,25])
   if low<.10 and impact is None:world.translation.z+=.10-low;impact=dict(t=t,world=world.copy(),pose=copy_pose(v))
   if impact is not None:world=impact['world'].copy();v=copy_pose(impact['pose'])
   # Full-body follow-through, then a planted native armed recovery.
   p=blend(release['actor'],p,ease(elapsed/.25));p=blend(p,idle[0],recover)
   targets=[];endg=rig.fk(idle[0]);g=rig.fk(p)
   for k,j in enumerate([21,25]):
    u=ease((t-1.30-k*.40)/.55);target=feet[k].lerp(endg[j].translation,u);target.z+=.06*math.sin(math.pi*u);targets.append(target)
   pin_feet(rig,p,targets)
  preview_helpers(rig,p);preview_helpers(vrig,v);ps.append(p);vs.append(v);roots.append(identity());vroots.append(row(world))
 alias='carry_body_throw_study';L.save(vrig,'corvo','victim_'+role+'_'+alias,vs,vroots,'carried_body_reference','Native carried/early-release shapes on an illustrative ballistic arc, frozen at floor contact. Not native physics or a completed ragdoll response.',dict(loaded_sha256=vm['sha256'],release_sha256=hm['sha256']));target=L.reports[-1];victims.append(target)
 partner=dict(role='corvo',id=target['id'],binary=target['binary'],sha256=target['sha256'],frames=len(vs),fps=60,duration=duration,root_track=vroots,placement=vroots[0],appearance='Neutral carried body reference',placement_basis='Authored demonstration flight, not engine physics')
 L.save(rig,role,alias,ps,roots,'interactions','Full-body shoulder throw study: loaded wind-up, stepping drive, support-hand release and armed recovery. Grey target flight is an illustrative trajectory and stops at first floor contact. Actual throw speed, weight, hand contact, body collisions, native ragdoll and live NPC ownership remain unverified.',dict(loaded_sha256=bm['sha256'],recovery_sha256=im['sha256']),partner=partner,phase_marks=[dict(phase='wind_up',at=0),dict(phase='drive',at=.5),dict(phase='illustrative_release',at=.98),dict(phase='recovery',at=1.15)],max_hand_target_error_m=max(errors))
 L.reports[-1]['preview_caption']='ILLUSTRATIVE TARGET FLIGHT - FROZEN AT FLOOR CONTACT - NO NATIVE PHYSICS';(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2));actors.append(L.reports[-1]);metrics.append(dict(role=role,max_hand_target_error_m=max(errors),impact=impact['t']if impact else None));print('THROW_CONTACT',metrics[-1],flush=True)
(P/'body-throw-manifest-v1.json').write_text(json.dumps(actors,indent=2));(P/'body-throw-victims-v1.json').write_text(json.dumps(victims,indent=2));(P/'carry-research/throw-screening-v1.json').write_text(json.dumps(metrics,indent=2))
