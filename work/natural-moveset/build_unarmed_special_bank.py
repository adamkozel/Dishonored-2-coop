"""Restore the hands-free donor motion for sprint, lean, slide and airborne.

Keeps the verified v9 payload unchanged. All roots remain identity, animation
events are absent, and anatomical translations stay native. No game writes.
"""
import sys,struct,json,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
from rig_math import ease
L=Library(['dynamics-retarget-manifest.json','mixamo-dynamic-retarget-manifest.json','mixamo-jump-retarget-manifest.json','library-retarget-manifest.json','jumps-retarget-manifest-v2.json'],'unarmed-special-sources-v1')
OUT=P/'unarmed-special-banks-v1';OUT.mkdir(exist_ok=True);reports=[]
for role in ('corvo','emily'):
 data=(P/'equipment-banks-v3'/f'{role}-equipment-bank.bin').read_bytes();magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert magic==b'D2MOVE09' and count==89
 bank=bytearray(struct.pack('<8sIIIIQ',b'D2MOVE10',10,n,107,fps,layout)+data[32:]);entry=24+121*n*28;rig=rig_for(role);sources=[]
 def previous(cid):return [[qts(struct.unpack_from('<7f',data,32+cid*entry+24+(f*n+j)*28))for j in range(n)]for f in range(121)]
 def metadata(cid):return struct.unpack_from('<ff',data,32+cid*entry+16)
 def emit(cid,poses,alias,source):
  assert len(poses)==121
  poses[-1]=copy_pose(poses[0]);stride,angle=metadata(cid-63);bank.extend(struct.pack('<IIIIff',cid,121,0,0,stride,angle))
  foot_error=0;original=previous(cid-63)
  for f,p in enumerate(poses):
   assert max(abs(p[j][a][b]-float(a==b))for j in (0,1)for a in range(4)for b in range(4))<1e-6
   assert max((p[j].translation-rig.bind[j].translation).length for j in range(3,27)if j!=17)<1e-5
   g=rig.fk(p);ref=rig.fk(original[f]);foot_error=max(foot_error,max((g[j].translation-ref[j].translation).length for j in (21,25)))
   for mat in p:bank.extend(struct.pack('<7f',*row(mat)))
  # These changes must not silently change gait cadence, root extraction or
  # ground contacts that the existing semantic clock was calibrated against.
  assert foot_error<.0001,(role,alias,foot_error)
  sources.append(dict(id=cid,alias=alias,armed_clip=cid-63,source=source,foot_trajectory_error=foot_error))
  L.save(rig,role,alias,poses[:-1],[identity()for _ in poses[:-1]],'unarmed',
   'Hands-free full-body variant. Donor shoulders/arms preserved; phase, anatomy and foot trajectory match the existing runtime family.',source,armed_clip=cid-63,foot_trajectory_error=foot_error)
 right_hand=[]
 for j in range(n):
  k=j
  while k>=0 and k!=16:k=rig.joints[k]['parent']
  if k==16 and 'helper'not in rig.joints[j]['nativeName']:right_hand.append(j)
 arm=[13,14,15]+right_hand
 em,empty=L.load(role,'empty_idle');low=previous(76)[0]
 def release(p,donor):
  p=copy_pose(p)
  for j in right_hand:p[j]=donor[j].copy()
  preview_helpers(rig,p);return p
 sm,sprint=L.load(role,'mixamo_two_cycle_sprint')
 sprint_manifest=next(r for r in read(P/'sprint-bank-manifest-v1.json')if r['role']==role)
 for k,alias in enumerate(('sprint_forward','sprint_forward_left','sprint_forward_right')):
  m,fs=L.load(role,alias);spec=sprint_manifest['sources'][k];period=spec['source_period_frames'];offset=round(spec['phase_offset']*120)
  poses=[];armed=previous(26+k)
  for f in range(120):
   u=((f+offset)%120)/120;donor=sample(sprint,(period*u/m['fps'])*m['review_speed_mps']/m['stride_m'],True)
   # Original dynamics authoring already includes the full natural arm swing;
   # v3 replaced it with a sword-carrying arm. Restore the original local arm.
   natural=sample(fs,period*u/(len(fs)-1));p=armed[f]
   for j in arm:p[j]=natural[j].copy()
   for j in right_hand:p[j]=donor[j].copy()
   preview_helpers(rig,p);poses.append(p)
  # Match the existing gait's five-percent cycle closure.
  for f in range(115,120):
   w=ease((f-114)/6)
   for j in arm:poses[f][j]=blend([poses[f][j]],[poses[0][j]],w)[0]
   preview_helpers(rig,poses[f])
  emit(89+k,poses+[poses[0]],'unarmed_'+alias,[m['sha256'],sm['sha256']])
 # Lean retains its established complete-body balance and planted feet. The
 # right arm follows its moving shoulder with the ordinary native empty hold.
 for k in range(4):
  poses=previous(29+k)
  for p in poses:
   for j in arm:p[j]=empty[0][j].copy() if k<2 else low[j].copy()
   preview_helpers(rig,p)
  emit(92+k,poses,'unarmed_'+('stand_'if k<2 else'crouch_')+'lean_'+('left'if k%2==0 else'right'),[em['sha256']])
 dm,slide=L.load(role,'mixamo_running_slide');lowest=min(range(len(slide)),key=lambda i:slide[i][2].translation.z)
 for k in range(4):
  poses=previous(33+k)
  for f,p in enumerate(poses[:-1]):
   u=f/119;index=u*lowest if k==0 else lowest if k==1 else lowest+u*(len(slide)-1-lowest)
   donor=sample(slide,index/(len(slide)-1))
   if k==3:donor=blend(donor,low,ease(u))
   poses[f]=release(p,donor)
  emit(96+k,poses,'unarmed_'+('slide_enter','slide_loop','slide_exit_to_run','slide_to_crouch')[k],[dm['sha256']])
 jm,jump=L.load(role,'mixamo_standing_jump');standing=next(r for r in L.rows if r['role']==role and r['selection']['alias']=='standing_jump_sequence')
 marks={m['phase']:round(m['at']*standing['fps'])for m in standing['phase_marks']}
 air_manifest=next(r for r in read(P/'air-bank-manifest-v1.json')if r['role']==role)
 rm,running=L.load(role,'mixamo_running_jump')
 for k,spec in enumerate(air_manifest['sources']):
  poses=previous(37+k);lo,hi=spec['source_frames'];alias=spec['alias']
  for f,p in enumerate(poses[:-1]):
   u=f/119;at=lo+u*(hi-lo)
   if alias=='standing_jump_sequence':donor=sample(jump,at/(len(jump)-1))
   elif alias=='crouch_jump_to_crouch':
    at=at-18+marks['compression'];donor=sample(jump,at/(len(jump)-1));donor=blend(donor,low,ease((at-marks['landing'])/30))
   elif alias=='running_jump_sequence':
    # Its only weapon change was the right-hand grip; shoulder/arm is already
    # the original running-jump donor, and root extraction remains unchanged.
    donor=sample(running,at/max(1,round(rm['duration']*60)))
   else:
    # Native falling supplies whole-body balance; open hands without inventing
    # a stationary wrist target or copying any first-person transform.
    donor=sample(empty,u,True)
   if alias in ('standing_jump_sequence','crouch_jump_to_crouch'):
    for j in arm:p[j]=donor[j].copy()
   else:
    for j in right_hand:p[j]=donor[j].copy()
   preview_helpers(rig,p)
  emit(100+k,poses,'unarmed_'+spec['part']+'_'+alias,[jm['sha256'],rm['sha256']])
 path=OUT/f'{role}-unarmed-special-bank.bin';path.write_bytes(bank)
 reports.append(dict(role=role,bank=str(path.relative_to(P)),sha256=hashlib.sha256(bank).hexdigest(),format='D2MOVE10',clips=107,prior_payload_unchanged=bank[32:len(data)]==data[32:],sources=sources,root_writes=False,events=False,gameplay_verified=False,visual_accepted=False,limitations=['Unarmed crouch remains the existing full-body posture with open fingers.','Unarmed fall retains native full-body fall with open hands.','Directional, transition and terrain visual acceptance still require runtime review.']))
L.finish('unarmed-special-source-manifest-v1.json');(P/'unarmed-special-bank-manifest-v1.json').write_text(json.dumps(reports,indent=2),encoding='utf-8')
