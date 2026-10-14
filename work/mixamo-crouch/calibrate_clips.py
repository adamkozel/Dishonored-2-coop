import json,statistics
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
p=Path(r'C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/mixamo-crouch')
j=json.loads((p/'native-corvo.json').read_text(encoding='utf-8-sig'))['joints']
def mat(q):
 m=Quaternion((q[3],q[0],q[1],q[2])).normalized().to_matrix().to_4x4();m.translation=Vector(q[4:7]);return m
bind=[mat(b['bindLocalQTS']) for b in j]
reports={}
for name in ('corvo-clip','corvo-upright-clip'):
 d=json.loads((p/(name+'.json')).read_text());feet=[]
 for row in d['frames']:
  local=[m.copy() for m in bind]
  for idx,q in zip(d['indices'],row):local[idx]=mat(q)
  glob=[]
  for idx,b in enumerate(j):glob.append(local[idx] if b['parent']<0 else glob[b['parent']]@local[idx])
  feet.append({k:list(glob[k].translation) for k in (21,22,25,26)})
 report={}
 for k in (21,22,25,26):
  floor=min(f[k][2] for f in feet);speeds=[]
  for i in range(1,62):
   v=(feet[i+1][k][0]-feet[i-1][k][0])*30
   if feet[i][k][2]<floor+.035 and v<-.1:speeds.append(-v)
  report[str(k)]={'contact_samples':len(speeds),'stride_m':statistics.median(speeds)*62/60}
 stride=statistics.median(v['stride_m'] for v in report.values())
 reports[name]={'feet':report,'median_stride_m':stride,'samples':feet}
(p/'stride-calibration.json').write_text(json.dumps({'method':'Median backward foot speed during contacts within 3.5 cm of minimum height, central derivative, full cycle 62/60 s. Approximate flat-floor calibration, not an IK foot lock.','clips':reports},indent=2))
grip=json.loads((p/'native-corvo-sword-grip.json').read_text(encoding='utf-8-sig'))
assert grip['finalPublicationComplete'] and len(grip['joints'])==215
indices=[16,17,*range(38,49),*range(63,73)]
rows=[]
for idx in indices:
 v=grip['joints'][idx]['localPose'];m=Matrix([[v[c*4+r] for c in range(3)] for r in range(3)])
 q=m.to_quaternion().normalized();rows.append([q.x,q.y,q.z,q.w])
lines=['#pragma once','// Native Corvo sword_idle at 600 ms; coherent phase-3 capture. No first-person pose.','constexpr unsigned kMixamoGripJoints[]={'+','.join(map(str,indices))+'};','constexpr float kMixamoGripRotation[][4]={']
for row in rows:lines.append('{'+','.join(f'{v:.9f}f' for v in row)+'},')
lines+=['};',f'constexpr float kMixamoCorvoStride={reports["corvo-upright-clip"]["median_stride_m"]:.9f}f;']
(p/'mixamo_corvo_grip.h').write_text('\n'.join(lines)+'\n')
result={'stride_m':{k:v['median_stride_m'] for k,v in reports.items()},'native_grip_joints':len(indices)}
