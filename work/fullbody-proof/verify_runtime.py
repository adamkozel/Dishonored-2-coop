"""Read-only final-publication analysis. Run in Blender for mathutils."""
import json,math,statistics
from pathlib import Path
from mathutils import Matrix,Vector
P=Path(r'C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/fullbody-proof')
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def mat(a):return Matrix([[a[c*4+r]if r<3 else float(c==3)for c in range(4)]for r in range(4)])
take='controller-take-4'
J=read(P.parent/'mixamo-crouch/native-corvo.json')['joints'];d=read(P/take/'poses.json');manifest=read(P/take/'manifest.json');start=manifest['Start'];samples=d['samples'];base=Vector(samples[0]['root'][:3]);forward=Vector(samples[0]['root'][3:6]);finish=None
contacts={'left_early':[],'right_late':[],'left_late':[]};rows=[];lengthError=0
for s in samples:
 time=(s['tick']-start)/1000;travel=(Vector(s['root'][:3])-base).dot(forward)
 if finish is None and travel>=.395:finish=s['tick']
 if finish is None:
  a,b=0,1
  for _ in range(22):u=(a+b)/2;a,b=(u,b)if u*u*(3-2*u)<max(0,min(1,travel/.4))else(a,u)
  phase=min(.8,time)if time<.8 else .8+1.5*(a+b)/2
 else:phase=2.3+(s['tick']-finish)/1000
 if time<.4 or phase>5.9:continue
 local=[mat(v)for v in s['local']];g=[]
 for i,j in enumerate(J):g.append(local[i]if j['parent']<0 else g[j['parent']]@local[i])
 root=Matrix.Identity(4);root.translation=Vector(s['root'][:3])
 for c in range(3):
  for r in range(3):root[r][c]=s['root'][3+c*3+r]
 lf=root@g[22].translation;rf=root@g[26].translation
 if phase<1.57:contacts['left_early'].append(lf)
 if phase>1.58:contacts['right_late'].append(rf)
 if phase>2.34:contacts['left_late'].append(lf)
 for i in [7,8,9,14,15,16,19,20,21,23,24,25]:lengthError=max(lengthError,(local[i].translation-Vector(J[i]['bindLocalQTS'][4:7])).length)
 rows.append({'time':time,'phase':phase,'travel':travel,'left':list(lf),'right':list(rf)})
metrics={}
for key,points in contacts.items():
 mean=[math.fsum(p[k] for p in points)/len(points) for k in range(3)];metrics[key]={'samples':len(points),'maxDeviationFromMeanM':max(math.dist(x,mean) for x in points),'axisRangeM':[max(x[k]for x in points)-min(x[k]for x in points)for k in range(3)]}
report={'take':manifest,'finalPublicationSamples':len(samples),'sourceVerticalRootRangeM':max(s['root'][2]for s in samples)-min(s['root'][2]for s in samples),'travelM':max(x['travel']for x in rows),'anatomicalLocalTranslationMaxErrorM':lengthError,'footContact':metrics,'scope':'Flat Dunwall test. Final-published pose; original source movement over native Steam. Stance contacts exclude entry/exit blend. Numerical agreement does not alone establish animation quality.','visualAccepted':False}
(P/'runtime-verification.json').write_text(json.dumps(report,indent=2));(P/'foot-trace.json').write_text(json.dumps(rows));result=report
