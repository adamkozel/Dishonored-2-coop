"""Window rear attack: authored braced step/pull/cut/recovery, native target clock.

FP body contributes rhythm and weight-shift inspiration only. No FP wrist
coordinates or anatomical translation changes enter these full-body poses.
"""
import sys,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
from contact_hand import descendants
from edge_data import poses as raw_poses
L=Library(['window-victim-retarget-manifest.json','library-retarget-manifest.json'],'window-pair-v3');actors=[];victims=[];metrics=[]
scene=next(r for r in read(P/'pair-research/native-context-event-times.json')if r['asset'].endswith('/assassination_window_back.midnightscene'))
for role in ['corvo','emily']:
 rig=rig_for(role);vrig=rig_for('corvo');im,idle=L.load(role,'sword_idle');gm,grab=L.load(role,'sword_doppelganger_assassination_fastback');reference=sample(grab,.24);rg=rig.fk(reference)
 vm,vf=L.load('corvo','generic_'+role+'_assassination_window_back');raw=raw_poses(P/vm['source']['sample_file']);place=qts(raw['frames'][0][1]).inverted();handoff=next(x['start_ms']/1000 for x in scene['rows']if x['class_name'].endswith('StartRagdoll')and '/'+role+'/'in x['player_body'])
 q=read(P.parent/('fullbody-proof/inputs/client-target-sword.json'if role=='corvo'else'crouch-grip-reference/host-target-sword.json'))['quaternion'];binding=Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix().to_4x4()
 duration=vm['duration']+.9;ps=[];roots=[];vs=[];vroots=[];tracks=[];errors=[];worst={};origin=Vector((.28,-.26,0));base=rig.fk(idle[0]);feet0=[base[j].translation.copy()for j in [21,25]];feet1=[feet0[0]+Vector((.23,.045,0)),feet0[1]+Vector((-.12,-.065,0))]
 for f in range(round(duration*60)+1):
  t=f/60;u=min(1,t/vm['duration']);vu=min(t,handoff)/vm['duration'];v=sample(vf,vu);vr=sample([[qts(r)]for r in vm['root_track']],vu)[0];world=place@vr;vg=vrig.fk(v);neck=(world@vg[11]).translation
  reach=ease(t/.30);pull=ease((t-.35)/.6);recover=ease((t-vm['duration']+.10)/.95);brace=reach*(1-recover);contact=ease((t-.08)/.25)*(1-ease((u-.42)/.20));cut=ease((u-.25)/.16)*(1-ease((u-.48)/.16));p=sample(idle,t/im['duration'],True)
  # A forward brace becomes a rear weight shift before release. Both feet
  # take visible steps and remain planted during the pull/cut interval.
  p[2].translation+=Vector((.24*reach-.26*pull,-.025*pull,-.09*brace))*(1-recover)
  for j,w in [(3,.34),(4,.33),(5,.33)]:
   rot=Matrix.Rotation(math.radians((22*reach-36*pull)*w*(1-recover)),3,'Y')@Matrix.Rotation(math.radians(22*pull*w*(1-recover)),3,'Z');rig.orient(p,j,rot@rig.fk(p)[j].to_3x3())
  feet=[a.lerp(b,brace)for a,b in zip(feet0,feet1)]
  if t<.30:feet[0].z+=.07*math.sin(math.pi*reach)
  if recover>0:feet[0].z+=.07*math.sin(math.pi*recover)
  pin_feet(rig,p,feet);free=copy_pose(p)
  if contact>0:
   left=neck+Vector((-.07,.09,-.015))-origin;right=neck+Vector((.05,-.48+.12*cut,.05-.10*cut))-origin;g=rig.fk(p)
   for a,b,j,goal,side in [(7,8,9,left,1),(14,15,16,right,-1)]:rig.solve(p,a,b,j,goal,g[a].translation+Vector((-.15,.42*side,-.04)))
   rig.orient(p,9,rg[9].to_3x3());g=rig.fk(p);weapon=g[17]@binding;direction=neck-origin-weapon.translation
   if direction.length>.05:rig.orient(p,16,(weapon.to_3x3()@Vector((1,0,0))).rotation_difference(direction).to_matrix()@g[16].to_3x3())
   for j in descendants(rig,9):p[j]=reference[j].copy()
   p=blend(free,p,contact)
   if contact>.999:
    g=rig.fk(p);error=max((g[9].translation-left).length,(g[16].translation-right).length);errors.append(error)
    if error>=max(errors):worst=dict(t=t,left_error=(g[9].translation-left).length,right_error=(g[16].translation-right).length,left_goal=list(left),right_goal=list(right),right_shoulder=list(g[14].translation),right_wrist=list(g[16].translation))
  preview_helpers(rig,p);ps.append(p);roots.append(identity());vs.append(v);vroots.append(row(world));tracks.append(row(Matrix.Translation(-origin)@Matrix.Translation((.83,0,0))))
 alias='window_rear_lethal_study';L.save(vrig,'corvo','victim_'+role+'_'+alias,vs,vroots,'paired_victim_reference','Native window target timing, held at exact ragdoll event without physics.',dict(victim_sha256=vm['sha256'],scene_sha256=scene['sha256']));target=L.reports[-1];victims.append(target)
 partner=dict(role='corvo',id=target['id'],binary=target['binary'],sha256=target['sha256'],frames=len(vs),fps=60,duration=(len(vs)-1)/60,root_track=vroots,placement=vroots[0],appearance='Neutral-grey native victim',placement_basis='Inverse initial partner anchor, unverified runtime frame')
 prop=dict(kind='fixture_window',model='contact-fixtures/window.json',skeleton='contact-fixtures/skeleton.json',parent_joint=0,attachment_track=tracks,visible=[True for _ in ps],mechanism=None,fixture=True,events=False)
 L.save(rig,role,alias,ps,roots,'paired_actions','Rear window lethal study: a braced step, whole-body weight shift, target-relative grab/cut and stepping recovery accompany exact native victim timing. No fixed first-person wrist targets. Window clearance, finger/blade geometry, ragdoll continuation and native paired gameplay remain unverified.',dict(idle_sha256=im['sha256'],grasp_sha256=gm['sha256'],victim_sha256=vm['sha256'],scene_sha256=scene['sha256']),partner=partner,props=[prop],preview_placement=row(Matrix.Translation(origin)),max_full_contact_error_m=max(errors,default=0),phase_marks=[dict(phase='native_window_action',at=0),dict(phase='ragdoll_reference_only',at=handoff)])
 actors.append(L.reports[-1]);metrics.append(dict(role=role,max_full_contact_error_m=max(errors,default=0),worst=worst));print('WINDOW_CONTACT',metrics[-1],flush=True)
(P/'window-pair-manifest-v3.json').write_text(json.dumps(actors,indent=2));(P/'window-pair-victims-v3.json').write_text(json.dumps(victims,indent=2));(P/'pair-research/window-pair-screening-v3.json').write_text(json.dumps(metrics,indent=2))
