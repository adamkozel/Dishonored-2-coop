import sys,struct,statistics,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from rig_math import read,NativeRig,qts
for r in read(P/'mixamo-retarget-manifest-v5.json'):
 if r['selection']['alias']!='mixamo_crouched_walking_posture100':continue
 rig=NativeRig(read(P.parent/f'mixamo-crouch/native-{r["role"]}.json')['joints']);raw=(P/r['binary']).read_bytes();n=r['joints'];feet=[]
 for f in range(r['frames']):
  g=rig.fk([qts(struct.unpack_from('<7f',raw,28+(f*n+j)*28))for j in range(n)]);feet.append([g[i].translation for i in [21,25]])
 result=[]
 for foot in range(2):
  low=min(v[foot].z for v in feet);values=[]
  for f in range(1,len(feet)-1):
   if max(feet[f-1][foot].z,feet[f+1][foot].z)<low+.035:
    value=-(feet[f+1][foot].x-feet[f-1][foot].x)/2*(len(feet)-1)
    if value>.1:values.append(value)
  result.append(dict(foot=foot,height=low,samples=len(values),stride=statistics.median(values)if values else None,range=[min(values),max(values)]if values else []))
 print(r['role'],json.dumps(result))
