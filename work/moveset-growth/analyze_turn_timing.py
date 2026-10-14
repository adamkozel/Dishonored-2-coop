import json,math
from pathlib import Path
from mathutils import Quaternion
P=Path(__file__).resolve().parent
reports=[]
for f in (P/'turns-chest').glob('corvo-turn*.json'):
 d=json.loads(f.read_text());steps=[]
 for n in range(1,len(d['frames'])):
  for j in (2,3,5,7,8,14,15,19,20,21,23,24,25):
   a,b=d['frames'][n-1][j],d['frames'][n][j];q=Quaternion((a[3],*a[:3]));angle=q.rotation_difference(Quaternion((b[3],*b[:3]))).angle;angle=min(angle,math.tau-angle)
   steps.append(dict(frame=n,joint=d['joints'][j],degrees=math.degrees(angle)))
 reports.append(dict(name=d['name'],largest_steps=sorted(steps,key=lambda s:s['degrees'],reverse=True)[:10]))
(P/'turns-chest/angular-timing-quality.json').write_text(json.dumps(reports,indent=2));print(json.dumps(reports,indent=2))
