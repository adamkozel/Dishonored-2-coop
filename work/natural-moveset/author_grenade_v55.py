"""Grenade candidate: free full-body acting, native finger/prop references.

No first-person wrist/camera anchors. Native events are reference timings only;
these offline clips never spawn, consume, arm, release or damage anything.
"""
import sys,math,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
from finger_reference import HandReference,SR,SN
from edge_data import poses as edge_poses
L=Library(['items-retarget-manifest-v2.json','campaign-retarget-manifest.json','library-retarget-manifest.json'],'grenade-authored-v55')
refs=read(P/'fp-grenade-reference-manifest-v53.json')['rows'];mechanisms=read(P/'grenade-mechanism-manifest-v55.json');calibrations=[]
durations={'idle':1.3333333333,'equip':.5,'unequip':14/60,'cooking_in':.5,'cooking_idle':80/60,'throw_high':.4,'throw_high_ooa':34/60,'throw_low':.4,'throw_low_ooa':.4}
def warp(u,release,source_start,source_release,source_end):
 return source_start+(source_release-source_start)*(u/release) if u<release else source_release+(source_end-source_release)*((u-release)/(1-release))
for role in ('corvo','emily'):
 rig=rig_for(role);hand=HandReference(rig)
 donors={a:L.load(role,a) for a in ['stunmine_idle','stunmine_equip','stunmine_unequip','sword_idle','sword_grenade_launch','sword_grenade_launch_down']}
 held=donors['stunmine_idle'][1];neutral=donors['sword_idle'][1];high=donors['sword_grenade_launch'][1];low=donors['sword_grenade_launch_down'][1];saved={}
 for part,duration in durations.items():
  ref=next(r for r in refs if 'ply_leftarm_grenade_'+part in r['aliases']);data=edge_poses(P/ref['sample_file']);assert data['sha256']==ref['sample_sha256']
  fp=[[qts(v)for v in f]for f in data['frames']];count=round(duration*60)+1;poses=[];attachments=[];visible=[]
  for f in range(count):
   u=f/(count-1);h=sample(held,u,True)
   if part=='idle':pose=h
   elif part=='equip':pose=blend(sample(donors['stunmine_equip'][1],u),h,ease(u))
   elif part=='unequip':pose=blend(h,sample(donors['stunmine_unequip'][1],u),ease(u))
   elif part=='cooking_in':pose=blend(h,sample(high,.30),ease(u))
   elif part=='cooking_idle':pose=sample(high,.30+.009*math.sin(2*math.pi*u))
   elif part in ('throw_high','throw_low'):
    islow=part=='throw_low';source=low if islow else high
    # NPC high launch starts at 1,000 ms, native fire at 2,167 ms;
    # low starts at 1,000, fire at 2,350. Align those reference instants
    # with the actual player's 293/302 ms native release in a 400 ms scene.
    release=(302 if islow else 293)/400;source_duration=donors['sword_grenade_launch_down' if islow else 'sword_grenade_launch'][0]['duration']
    phase=warp(u,release,.30,(1.350 if islow else 1.167)/source_duration,.76 if islow else .70)
    pose=sample(source,phase)
    if islow:pose=blend(sample(high,.30),pose,ease(min(1,u/.22)))
   else:
    islow=part=='throw_low_ooa';source=low if islow else high;start=.76 if islow else .70
    pose=blend(sample(source,start+(1-start)*u),sample(neutral,u,True),ease(u)*.75)
   reference=sample(fp,u,part in ('idle','cooking_idle'));hand.apply(pose,reference);preview_helpers(rig,pose)
   fg=SR.fk(reference);attach=hand.invcal.to_4x4()@fg[4].inverted()@fg[SN['left_hand_attachment_jnt']]
   poses.append(pose);attachments.append(row(attach))
   show=False if part.endswith('_ooa') else (u<((302 if part=='throw_low' else 293)/400) if part.startswith('throw_') else u>=.2 if part=='equip' else u<=.75 if part=='unequip' else True)
   visible.append(show)
  mechanism=next(m for m in mechanisms if m['id']==('grenade_cooking_in' if part=='cooking_in' else 'grenade_cooking_idle' if part=='cooking_idle' or part.startswith('throw_') else 'grenade_idle'))
  prop=dict(kind='grenade',model='item-models/grenade.json',skeleton='item-mechanisms/grenade-skeleton.json',parent_joint=9,attachment_track=attachments,visible=visible,mechanism=mechanism,events=False)
  sources=dict(body=[dict(alias=a,sha256=m[0]['sha256']) for a,m in donors.items()],hand_asset=ref['asset'],hand_sha256=ref['sample_sha256'],native_release_ms=302 if part=='throw_low' else 293 if part=='throw_high' else None)
  alias='grenade_'+part
  L.save(rig,role,alias,poses,[identity()for _ in poses],'ranged_and_items','Grenade full-body candidate with native wrist-local fingers, attachment and six-joint prop. Source native scene durations/release guide timing. Free wrist and body; events disabled. Remote binding and visual acceptance pending.',sources,props=[prop])
  saved[part]=(poses,prop,L.reports[-1])
  if part=='idle':calibrations.append(dict(role=role,attachment=attachments[0],binary=L.reports[-1]['binary']))
 for branch in ('high','low'):
  sequence=['equip','idle','cooking_in','cooking_idle','throw_'+branch,'throw_'+branch+'_ooa'];poses=[];attachments=[];visible=[];segments=[];marks=[]
  for part in sequence:
   fs,prop,m=saved[part];start=len(poses)/60;marks.append(dict(phase=part,at=start));poses.extend(copy_pose(p) for p in fs);attachments.extend(prop['attachment_track']);visible.extend(prop['visible']);segments.append(dict(start=start,end=len(poses)/60,mechanism=prop['mechanism']))
  prop=dict(saved['idle'][1]);prop.update(attachment_track=attachments,visible=visible,mechanism_segments=segments)
  L.save(rig,role,'grenade_'+branch+'_sequence',poses,[identity()for _ in poses],'ranged_and_items','Connected grenade candidate, native source timings and free full-body motion. Held prop only; no projectile or effects in authoring preview. Transitions, grip and game binding unaccepted.',dict(parts=sequence),props=[prop],phase_marks=marks)
L.finish('grenade-authoring-manifest-v55.json')
lines=['#pragma once','// Generated by author_grenade_v55.py. Free-wrist local equipment transform.','inline bool grenadePropIdleCalibration(unsigned character,float* offset,float* axis){']
for c in calibrations:
 m=qts(c['attachment']);values=lambda xs:','.join(f'{float(v):.9f}f' for v in xs)
 lines.append(' if(character==%d){const float p[]={%s},r[]={%s};std::copy(p,p+3,offset);std::copy(r,r+9,axis);return true;}'%((1 if c['role']=='corvo' else 2),values(m.translation),values(m[r][col] for col in range(3) for r in range(3))))
lines+=[' return false;','}'];(P.parent.parent/'outputs/d2-coop-lab/src/grenade_prop_calibration.h').write_text('\n'.join(lines)+'\n')
(P/'grenade-calibration-v55.json').write_text(json.dumps(calibrations,indent=2));print('Grenade authoring candidates:',len(L.reports),flush=True)
