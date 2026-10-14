"""Keep turn anticipation/foot steps at source timing; bounded preview-only pins.

This is an authoring candidate, not angle-indexed runtime coverage. The native
actor root remains a future network input, never animation-owned game motion.
"""
from pathlib import Path
P=Path(__file__).resolve().parent
source=(P/'retarget_turns.py').read_text().split('report=[]')[0]
source=source.replace("OUT=P/'turns'","OUT=P/'turns-timed'")
source=source.replace("self.hand=rg[16];self.binding","self.hand=rg[16];self.chest=rg[5];self.binding")
source=source.replace("self.solve(local,14,15,16,self.hand.translation,Vector((.02,-.64,1.2)));self.orient(local,16,self.hand.to_3x3())",
 "g=self.fk(local);delta=g[5]@self.chest.inverted();hand=delta@self.hand;pole=delta@Vector((.02,-.64,1.2))\n  self.solve(local,14,15,16,hand.translation,pole);self.orient(local,16,hand.to_3x3())")
source=source.replace("goal=root.inverted()@world.lerp", "offset=self.locks[side]-world\n    if offset.length>.10:self.locks[side]=world+offset.normalized()*.10\n    goal=root.inverted()@world.lerp")
exec(compile(source,str(P/'retarget_turns.py'),'exec'),globals())
manifest=[]
for role in ('corvo','emily'):
 for name in ('turn_left_90','turn_right_90','turn_left_180','turn_right_180'):
  donor=read(P/'donors-component-feedback'/f'{name}.json');rig=Rig(role,donor);frames=[];roots=[];largest=0.;length_error=0.;hand_error=0.;last_yaw=0.;yaw=0.
  for n,f in enumerate(donor['frames'][:301]):
   q=f['feedback_rotation'];raw=-math.atan2(2*q[3]*q[2],1-2*q[2]*q[2]);yaw+=(raw-last_yaw+math.pi)%(2*math.pi)-math.pi;last_yaw=raw
   local=rig.pose(f,Matrix.Rotation(yaw,4,'Z'));row=as_row(local)
   if n<90:continue
   g=rig.fk(local);hand=g[5]@rig.chest.inverted()@rig.hand;hand_error=max(hand_error,(hand.translation-g[16].translation).length)
   length_error=max(length_error,max((local[j].translation-rig.B[j].translation).length for j in range(3,27) if j!=17))
   if frames:
    for j in (2,3,5,7,8,14,15,19,20,21,23,24,25):
     a=Quaternion((frames[-1][j][3],*frames[-1][j][:3]));b=Quaternion((row[j][3],*row[j][:3]));r=a.rotation_difference(b).angle;largest=max(largest,min(r,math.tau-r))
   frames.append(row);roots.append(yaw)
  meta=dict(role=role,name=name,fps=60,source_frames=[90,300],source_seconds=3.5,frames=frames,root_angles=roots,joints=[j['nativeName'] for j in rig.J],parents=[j['parent'] for j in rig.J],weapon_binding=rig.binding,
   native_length_error_m=length_error,max_hand_error_m=hand_error,max_core_joint_step_degrees=math.degrees(largest),root_writes=False,loop_closed=False,d2_playback_verified=False,
   notes='Original 60 Hz time preserved; anticipation and recovery included. Chest-relative sword constraint. Preview foot pin radius bounded to 10 cm, modeled after sample rig setting; not a reproduction of its full Control Rig.')
  (OUT/f'{role}-{name}.json').write_text(json.dumps(meta,separators=(',',':')))
  manifest.append({k:v for k,v in meta.items() if k not in ('frames','root_angles','joints','parents','weapon_binding')})
(OUT/'manifest.json').write_text(json.dumps(manifest,indent=2));print(json.dumps(manifest,indent=2))
