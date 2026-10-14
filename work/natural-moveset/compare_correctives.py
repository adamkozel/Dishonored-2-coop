import sys,json,math,struct
from pathlib import Path
from mathutils import Quaternion,Matrix,Vector
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
result=[]
for c in read(P/'review-test-2/result.json')['cases']:
 meta=read(P/'native-retargeted'/(c['id']+'.json'));b=(P/meta['binary']).read_bytes();n=meta['joints'];frame=c['frame'];sample=read(Path(c['poseFile']))['samples'][-1]
 joints=read(P.parent/f"mixamo-crouch/native-{meta['role']}.json")['joints'];diffs=[]
 for j in range(n):
  v=struct.unpack_from('<7f',b,28+(frame*n+j)*28);a=sample['local'][j];m=Matrix([[a[col*4+row] if row<3 else float(col==3) for col in range(4)] for row in range(4)])
  ang=math.degrees(2*math.acos(min(1,abs(Quaternion((v[3],*v[:3])).normalized().dot(m.to_quaternion().normalized())))))
  dist=(Vector(v[4:7])-m.translation).length
  if ang>.15 or dist>.0001:diffs.append(dict(joint=j,name=joints[j]['nativeName'],angle=ang,distance=dist))
 result.append(dict(id=c['id'],changed=diffs))
(P/'corrective-comparison.json').write_text(json.dumps(result,indent=2));print(json.dumps([{**r,'changed':r['changed'][:10],'count':len(r['changed'])} for r in result],indent=2))
