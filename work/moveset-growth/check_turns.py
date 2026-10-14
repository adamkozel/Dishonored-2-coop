import json,math
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
P=Path(__file__).resolve().parent
exec((P/'retarget_turns.py').read_text().split('report=[]')[0])
result=[]
for role in ('corvo','emily'):
 for name in ('turn_left_90','turn_right_90','turn_left_180','turn_right_180'):
  d=read(P/'donors-feedback'/f'{name}.json');r=Rig(role,d);b=read(P/'turns'/f'{role}-{name}.json');hand_error=0;max_joint_step=0
  for f,frame in enumerate(b['frames']):
   g=r.fk([qts(v) for v in frame]);hand_error=max(hand_error,(g[16].translation-r.hand.translation).length)
   if f:
    for i in (2,3,5,7,8,14,15,19,20,23,24):
     q=Quaternion((frame[i][3],*frame[i][:3]));last=b['frames'][f-1][i];a=q.rotation_difference(Quaternion((last[3],*last[:3]))).angle;max_joint_step=max(max_joint_step,min(a,2*math.pi-a))
  result.append(dict(role=role,name=name,max_hand_error_m=hand_error,max_joint_degrees_per_angular_sample=math.degrees(max_joint_step),fingers=r.fingers))
(P/'turns/quality.json').write_text(json.dumps(result,indent=2));print(json.dumps(result,indent=2))
