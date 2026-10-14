"""Connected rear nonlethal studies with native victim timing and physical contact."""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['library-retarget-manifest.json','choke-victim-retarget-manifest.json'],'choke-sequences-v1');actors=[];targets=[]
for role in ['corvo','emily']:
 rig=rig_for(role);vrig=rig_for('corvo');am,idle=L.load(role,'empty_idle');gm,grab=L.load(role,'sword_doppelganger_assassination_fastback');native={phase:L.load('corvo',f'generic_{role}_{phase}')for phase in ['chokeback_in','choke_loop','choke_win','choke_cancel']}
 reference=sample(grab,.24);rg=rig.fk(reference);loop0=vrig.fk(native['choke_loop'][1][0]);neck0=Vector((1,0,0))+loop0[11].translation;feet0=[rig.fk(idle[0])[j].translation.copy()for j in [21,25]];feet1=[Vector((.36,.20,feet0[0].z)),Vector((-.10,-.18,feet0[1].z))]
 hand=[]
 for i,j in enumerate(rig.joints):
  parent=i
  while parent>=0 and parent not in [9,16]:parent=rig.joints[parent]['parent']
  if parent in [9,16]and i not in [9,16]and'helper'not in j['nativeName']:hand.append(i)
 for ending in ['hold','finish','release']:
  order=['chokeback_in','choke_loop','choke_loop']+(['choke_win']if ending=='finish'else['choke_cancel']if ending=='release'else[]);ps=[];vs=[];vroots=[];marks=[];errors=[];source=[]
  for phase in order:
   vm,frames=native[phase];source.append(dict(alias=phase,sha256=vm['sha256']));duration=vm['duration'];count=round(duration*60)+1;marks.append(dict(phase=phase,at=len(ps)/60))
   for f in range(count):
    u=f/(count-1);v=sample(frames,u);vr=sample([[qts(r)]for r in vm['root_track']],u)[0];vg=vrig.fk(v);neck=(Matrix.Translation((1,0,0))@vr@vg[11]).translation
    if phase=='chokeback_in':body=ease(u);contact=ease((u-.12)/.70)
    elif phase=='choke_loop':body=contact=1
    elif phase=='choke_win':body=1-ease((u-.35)/.60);contact=1-ease((u-.12)/.30)
    else:body=1-ease((u-.15)/.8);contact=1-ease((u-.03)/.45)
    p=sample(idle,len(ps)/60/max(am['duration'],.1),True);p[2].translation+=Vector((.33,0,-.065))*body+(neck-neck0)*(.3*contact)
    for j,fraction in [(3,.35),(4,.35),(5,.3)]:rig.orient(p,j,Matrix.Rotation(math.radians(8*fraction*body),3,'Y')@rig.fk(p)[j].to_3x3())
    feet=[a.lerp(b,body)for a,b in zip(feet0,feet1)]
    if phase!='choke_loop':
     feet[0].z+=.10*math.sin(math.pi*body);feet[1].z+=.03*math.sin(math.pi*body)
    pin_feet(rig,p,feet);free=copy_pose(p);rotation=(vr@vg[11]).to_3x3()@loop0[11].to_3x3().inverted();left=neck+rotation@Vector((.08,.02,-.005));right=left+rotation@Vector((-.025,-.045,-.025))
    rig.solve(p,7,8,9,left,neck+Vector((.20,.35,-.05)));rig.solve(p,14,15,16,right,neck+Vector((-.03,-.50,-.28)))
    for wrist in [9,16]:rig.orient(p,wrist,rotation@rg[wrist].to_3x3())
    for j in hand:p[j]=reference[j].copy()
    p=blend(free,p,contact);preview_helpers(rig,p)
    if contact>.999:errors.append(max((rig.fk(p)[9].translation-left).length,(rig.fk(p)[16].translation-right).length))
    ps.append(p);vs.append(v);vroots.append(row(vr))
  alias='nonlethal_back_'+ending+'_sequence';L.save(vrig,'corvo','victim_'+role+'_'+alias,vs,vroots,'paired_victim_reference','Native rear nonlethal victim phases; no game events.',dict(clips=source));target=L.reports[-1];targets.append(target)
  partner=dict(role='corvo',id=target['id'],binary=target['binary'],sha256=target['sha256'],frames=len(vs),fps=60,duration=(len(vs)-1)/60,root_track=vroots,placement=identity(1,0,0),source='Matching native '+role+' rear-choke victim phases',appearance='Neutral-grey native-rig stand-in',placement_basis='One-metre native loop anchor; rear-entry and phase-boundary contact under review')
  L.save(rig,role,alias,ps,[identity()for _ in ps],'paired_actions','Connected rear nonlethal '+ending+' study: an authored planted stance and stepping approach support moving-victim arm contacts, followed by native victim struggle and '+ending+'. Whole body is independent of first-person camera space. Entry timing, hand shaping, foot transfer, release/knockout contact and gameplay authority require review.',dict(body_sha256=am['sha256'],hand_sha256=gm['sha256'],victim_clips=source),partner=partner,phase_marks=marks,full_contact_error_m=max(errors,default=0))
  L.reports[-1]['equipment_policy']='hands_free';actors.append(L.reports[-1]);(P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2));print('NONLETHAL_CONTACT',role,ending,max(errors,default=0),flush=True)
(P/'choke-sequences-manifest-v1.json').write_text(json.dumps(actors,indent=2));(P/'choke-sequences-victims-v1.json').write_text(json.dumps(targets,indent=2))
