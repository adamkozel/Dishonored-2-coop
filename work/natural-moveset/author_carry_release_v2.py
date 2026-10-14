"""Whole-body release acting at native carried-victim phase boundaries.

Victim playback stops at the actual ragdoll event. A frozen handoff pose is
explicit evidence of the animation boundary, never a claim of native physics.
"""
import sys,math,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['carry-body-manifest-v3.json','carry-victim-retarget-manifest.json','library-retarget-manifest.json'],'carry-release-v2');actors=[];victims=[];metrics=[]
scenes=read(P/'carry-research/native-carry-scenes.json')
for role in ['corvo','emily']:
 rig=rig_for(role);vrig=rig_for('corvo');bm,loaded=L.load(role,'carry_body_idle');im,idle=L.load(role,'sword_idle');initial=qts(bm['partner']['root_track'][0]);vm,vbase=L.load('corvo','generic_carrycorpse_idle');support0=(initial@vrig.fk(vbase[0])[4]).translation
 for name,key in [('low','drop_low_stand'),('high','drop_high'),('soft','drop_soft')]:
  scene=next(s for s in scenes if s['asset'].endswith('/carry_corpse_'+key+'.midnightscene'));events=scene['rows'];handoff=next(e['start_ms']/1000 for e in events if e['class_name'].endswith('StartRagdoll'));early=next(e['start_ms']/1000 for e in events if e['properties'].get('m_event')=='ARK_MDNT_MSG_ENABLE_EARLY_RAGDOLL');actorend=max(e['duration_ms']/1000 for e in events if e['class_name']=='arkMidnightKeyPropAnim'and e['group'].startswith('Player'))
  dm,donor=L.load('corvo','generic_carrycorpse_drop'+name);rot=initial.to_3x3();attach=rot.to_4x4();attach.translation=support0-rot@vrig.fk(donor[0])[4].translation;duration=.4+actorend+.8;ps=[];vs=[];roots=[];vroots=[];errors=[];feet0=[rig.fk(loaded[0])[j].translation.copy()for j in [21,25]]
  for f in range(round(duration*60)+1):
   t=f/60;clock=max(0,min(handoff,t-.4));v=sample(donor,clock/dm['duration']);v=blend(vbase[0],v,ease(clock/.08));vr=sample([[qts(r)]for r in dm['root_track']],clock/dm['duration'])[0];world=attach@vr;vg=vrig.fk(v);p=sample(loaded,t/bm['duration'],True)
   act=ease((t-.35)/max(.18,handoff))*(1-ease((t-.4-actorend)/.65));p[2].translation+=Vector((.23 if name=='high'else .16,0,-.045 if name!='soft'else-.09))*act
   for j,w in [(3,.3),(4,.4),(5,.3)]:rig.orient(p,j,Matrix.Rotation(math.radians((15 if name=='high'else 11)*w*act),3,'Y')@rig.fk(p)[j].to_3x3())
   pin_feet(rig,p,feet0);g=rig.fk(p);goal=world@(vg[19].translation.lerp(vg[20].translation,.72))+Vector((.015,-.065,.035));free=copy_pose(p);rotation=g[9].to_3x3();rig.solve(p,7,8,9,goal,g[7].translation+Vector((.2,.30,-.35)));rig.orient(p,9,rotation)
   contact=1-ease((t-.4-early)/max(.08,handoff-early));p=blend(free,p,contact)
   if contact>.999:
    error=(rig.fk(p)[9].translation-goal).length;errors.append(error)
    if error>.08 and f%3==0:print('CARRY_REACH',role,name,round(t,3),'shoulder',list(g[7].translation),'goal',list(goal),'error',round(error,4),flush=True)
   if t>.4+early:
    recovery=ease((t-.4-early)/max(.4,actorend+.5-early));p=blend(p,sample(idle,t/im['duration'],True),recovery)
   preview_helpers(rig,p);ps.append(p);vs.append(v);roots.append(identity());vroots.append(row(world))
  alias='carry_body_drop_'+name+'_sequence';L.save(vrig,'corvo','victim_'+role+'_'+alias,vs,vroots,'carried_body_reference','Native victim response sampled only to the confirmed ragdoll marker. Final pose is held; native physical continuation is deliberately not claimed.',dict(native_sha256=dm['sha256'],scene_sha256=scene['sha256']),ragdoll_marker_seconds=handoff);victim=L.reports[-1];victims.append(victim)
  partner=dict(role='corvo',id=victim['id'],binary=victim['binary'],sha256=victim['sha256'],frames=len(vs),fps=60,duration=(len(vs)-1)/60,root_track=vroots,placement=vroots[0],appearance='Neutral native carried-body reference',placement_basis='Current shoulder attachment aligned to native initial torso; not runtime paired validation')
  L.save(rig,role,alias,ps,roots,'interactions','Full-body '+name+' carried-body release: loaded counterbalance, forward weight transfer, supporting-arm release and armed recovery. Native victim motion stops at its exact ragdoll marker and holds there in this review. The final floating target pose is not a completed fall. Actual NPC ownership, physical continuation, shoulder/hand fit and outcome binding remain pending.',dict(loaded_sha256=bm['sha256'],victim_sha256=dm['sha256'],recovery_sha256=im['sha256'],scene=scene['asset'],scene_sha256=scene['sha256']),partner=partner,phase_marks=[dict(phase='loaded',at=0),dict(phase='native_release',at=.4),dict(phase='native_early_ragdoll_boundary',at=.4+early),dict(phase='native_ragdoll_boundary_target_held',at=.4+handoff)],max_hand_target_error_m=max(errors,default=0),runtime_events=False)
  L.reports[-1]['preview_caption']='VICTIM FROZEN AT NATIVE RAGDOLL MARKER - PHYSICS NOT SHOWN'
  (P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2))
  actors.append(L.reports[-1]);metrics.append(dict(role=role,kind=name,max_hand_target_error_m=max(errors,default=0),native_early_ragdoll_seconds=early,native_ragdoll_seconds=handoff));print('CARRY_RELEASE',metrics[-1],flush=True)
(P/'carry-release-manifest-v2.json').write_text(json.dumps(actors,indent=2));(P/'carry-release-victims-v2.json').write_text(json.dumps(victims,indent=2));(P/'carry-research/release-screening-v2.json').write_text(json.dumps(metrics,indent=2))
