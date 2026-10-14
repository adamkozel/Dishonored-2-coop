import sys,struct,json,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['mixamo-retarget-manifest-v5.json','stance-retarget-manifest.json'],'posture-grip-diagnostics-v1')
def captured(v):
 m=Matrix.Identity(4)
 for c in range(4):
  for r in range(3):m[r][c]=v[c*4+r]
 return m
out={}
for role in ('corvo','emily'):
 rig=rig_for(role);n=len(rig.joints);b=(P/'windblast-context-banks-v1'/f'{role}-windblast-context-bank.bin').read_bytes();entry=24+121*n*28
 cap=read(P.parent/f'mixamo-crouch/native-{role}-sword-grip.json')['joints'];lookup={j['nativeName']:i for i,j in enumerate(rig.joints)}
 fingers=[i for i,j in enumerate(rig.joints)if j['nativeName'].startswith('right_')and any(x in j['nativeName']for x in ('thumb','index','middle','ring','pinky'))]
 poses={f'bank{cid}':[qts(struct.unpack_from('<7f',b,32+cid*entry+24+j*28))for j in range(n)]for cid in (0,17,18,59,76,197,199)}
 for alias in ('mixamo_crouching_idle','mixamo_crouched_walking_posture100'):
  m,fs=L.load(role,alias)
  for at in (0,len(fs)//2):poses[f'{alias}-{at}']=fs[at]
 out[role]={}
 for name,pose in poses.items():
  g=rig.fk(pose)
  out[role][name]=dict(joints={rig.joints[i]['nativeName']:[round(v,5)for v in g[i].translation]for i in (2,12,16,19,20,21,23,24,25)},fingers_native_degrees={rig.joints[i]['nativeName']:round(math.degrees(pose[i].to_quaternion().rotation_difference(captured(cap[i]['localPose']).to_quaternion()).angle),3)for i in fingers},socket_translation=list(pose[17].translation))
(P/'posture-grip-diagnostics-v1/result.json').write_text(json.dumps(out,indent=2))
print(json.dumps({r:{k:dict(heights={n:v[2]for n,v in x['joints'].items()},finger_difference=max(x['fingers_native_degrees'].values()))for k,x in rows.items()}for r,rows in out.items()},indent=2))
