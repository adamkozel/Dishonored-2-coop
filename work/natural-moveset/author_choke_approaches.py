"""Front/side nonlethal studies in the native victim's directional frame.

Actor placement is offline metadata, never a displacement in its body pose.
Native end-of-entry alignment is inferred and remains a runtime hypothesis.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['library-retarget-manifest.json','choke-victim-retarget-manifest.json'],'choke-approaches-v1');actors=[];victims=[];alignment=[]
for role in ['corvo','emily']:
 rig=rig_for(role);vrig=rig_for('corvo');im,idle=L.load(role,'empty_idle');gm,grab=L.load(role,'sword_doppelganger_assassination_fastback');reference=sample(grab,.24);rg=rig.fk(reference);hands=[]
 for j in range(len(rig.joints)):
  a=j
  while a>=0 and a not in [9,16]:a=rig.joints[a]['parent']
  if a in [9,16]and j!=a and'helper'not in rig.joints[j]['nativeName']:hands.append(j)
 native={part:L.load('corvo','generic_'+role+'_'+part)for part in ['chokefront_in','chokeleft_in','chokeright_in','choke_loop','choke_win','choke_cancel']};base=Matrix.Translation((1,0,0));lm,loop=native['choke_loop'];lg=vrig.fk(loop[0]);looproot=qts(lm['root_track'][0]);neck0=(base@looproot@lg[11]).translation;feet0=[rig.fk(idle[0])[j].translation.copy()for j in [21,25]];feet1=[Vector((.36,.20,feet0[0].z)),Vector((-.10,-.18,feet0[1].z))]
 def yaw(m):
  forward=m.to_3x3()@vrig.global_bind[5].to_3x3().inverted()@Vector((1,0,0));return math.atan2(forward.y,forward.x)
 for direction in ['front','left','right']:
  entryname='choke'+direction+'_in';em,entry=native[entryname];eg=vrig.fk(entry[-1]);endroot=qts(em['root_track'][-1]);angle=yaw(endroot@eg[5])-yaw(looproot@lg[5]);placement=Matrix.Rotation(angle,4,'Z');placement.translation=(base@endroot@eg[11]).translation-placement.to_3x3()@neck0;placement.translation.z=0;inv=placement.inverted();alignment.append(dict(role=role,direction=direction,actor_placement=row(placement),basis='Match entry-end neck position and horizontal spine facing to native loop start; offline hypothesis'))
  for ending in ['hold','finish','release']:
   order=[entryname,'choke_loop','choke_loop']+(['choke_win']if ending=='finish'else['choke_cancel']if ending=='release'else[]);ps=[];vs=[];vroots=[];marks=[];errors=[];source=[]
   for phase in order:
    vm,frames=native[phase];source.append(dict(alias=phase,sha256=vm['sha256']));count=round(vm['duration']*60)+1;marks.append(dict(phase=phase,at=len(ps)/60))
    for f in range(count):
     u=f/(count-1);v=sample(frames,u);vr=sample([[qts(r)]for r in vm['root_track']],u)[0];world=base@vr if phase==entryname else placement@base@vr;vg=vrig.fk(v);neck=(inv@world@vg[11]).translation
     if phase==entryname:body=ease(u);contact=ease((u-.18)/.68)
     elif phase=='choke_loop':body=contact=1
     elif phase=='choke_win':body=1-ease((u-.35)/.60);contact=1-ease((u-.12)/.30)
     else:body=1-ease((u-.15)/.8);contact=1-ease((u-.03)/.45)
     p=sample(idle,len(ps)/60/im['duration'],True);p[2].translation+=Vector((.33,0,-.065))*body+(neck-neck0)*(.3*contact)
     for j,w in [(3,.35),(4,.35),(5,.3)]:rig.orient(p,j,Matrix.Rotation(math.radians(8*w*body),3,'Y')@rig.fk(p)[j].to_3x3())
     feet=[a.lerp(b,body)for a,b in zip(feet0,feet1)]
     if phase!='choke_loop':feet[0].z+=.10*math.sin(math.pi*body);feet[1].z+=.03*math.sin(math.pi*body)
     pin_feet(rig,p,feet);free=copy_pose(p);rotation=(inv@world@vg[11]).to_3x3()@(base@looproot@lg[11]).to_3x3().inverted();left=neck+rotation@Vector((.08,.02,-.005));right=left+rotation@Vector((-.025,-.045,-.025))
     rig.solve(p,7,8,9,left,neck+Vector((.20,.35,-.05)));rig.solve(p,14,15,16,right,neck+Vector((-.03,-.50,-.28)))
     for wrist in [9,16]:rig.orient(p,wrist,rotation@rg[wrist].to_3x3())
     for j in hands:p[j]=reference[j].copy()
     p=blend(free,p,contact);preview_helpers(rig,p)
     if contact>.999:errors.append(max((rig.fk(p)[9].translation-left).length,(rig.fk(p)[16].translation-right).length))
     ps.append(p);vs.append(v);vroots.append(row(world))
   alias='nonlethal_'+direction+'_'+ending+'_sequence';L.save(vrig,'corvo','victim_'+role+'_'+alias,vs,vroots,'paired_victim_reference','Native directional entry and rear hold/finish phases; no gameplay events.',dict(clips=source));vm=L.reports[-1];victims.append(vm)
   partner=dict(role='corvo',id=vm['id'],binary=vm['binary'],sha256=vm['sha256'],frames=len(vs),fps=60,duration=(len(vs)-1)/60,root_track=vroots,placement=vroots[0],appearance='Neutral-grey native-rig stand-in',source='Native '+role+' '+direction+' nonlethal phases',placement_basis='Entry is retained in its original victim frame; following phases use inferred entry-to-loop alignment')
   L.save(rig,role,alias,ps,[identity()for _ in ps],'paired_actions','Full-body '+direction+' nonlethal '+ending+' study with native victim timing and an authored stepping/holding stance. Contact is victim-relative, not camera-relative. Initial reach/grab choreography, precise hand contact, ragdoll and live paired ownership remain under review.',dict(body_sha256=im['sha256'],hands_sha256=gm['sha256'],victim_clips=source),partner=partner,preview_placement=row(placement),phase_marks=marks,full_contact_error_m=max(errors,default=0))
   L.reports[-1]['equipment_policy']='hands_free';(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2));actors.append(L.reports[-1]);print('APPROACH_CONTACT',role,direction,ending,max(errors,default=0),flush=True)
(P/'choke-approaches-manifest-v1.json').write_text(json.dumps(actors,indent=2));(P/'choke-approaches-victims-v1.json').write_text(json.dumps(victims,indent=2));(P/'pair-research/choke-approach-alignment-v1.json').write_text(json.dumps(alignment,indent=2))
