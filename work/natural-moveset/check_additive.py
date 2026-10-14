import sys,json,math
from pathlib import Path
from mathutils import Quaternion
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from edge_data import poses
from rig_math import read
rows=read(P/'native-expanded-sample-manifest.json')['rows']
def get(alias):
 r=next(r for r in rows if alias in r['aliases'] and r['source_role']=='corvo' and r['status']=='native_pose_sampled_offline')
 return poses(P/r['sample_file'])['frames']
def q(v):return Quaternion((v[3],*v[:3]))
def angle(a,b):return math.degrees(2*math.acos(min(1,abs(a.normalized().dot(b.normalized())))))
results=[]
bind=[j['bindLocalQTS'] for j in read(P/'base-tall.json')['joints']]
for gait in ['run','walk']:
 original=get('sword_'+gait+'_n');delta=get('sword_'+gait+'_n_add')
 for refname,refs in [('bind',[bind]),('sword_neutral',get('sword_neutral')),('sword_idle',get('sword_idle'))]:
  for ri,ref in enumerate(refs[:3]):
   for order in ['ref_delta','delta_ref']:
    errors=[];translations=[]
    for a,b in zip(original,delta):
     for j in range(2,27):
      predicted=q(ref[j])@q(b[j]) if order=='ref_delta' else q(b[j])@q(ref[j])
      errors.append(angle(predicted,q(a[j])))
      translations.append(max(abs(a[j][k]-ref[j][k]-b[j][k]) for k in range(4,7)))
    results.append(dict(gait=gait,reference=refname,frame=ri,order=order,mean_degrees=sum(errors)/len(errors),max_degrees=max(errors),max_translation=max(translations)))
results.sort(key=lambda r:r['mean_degrees'])
(P/'additive-analysis.json').write_text(json.dumps(results,indent=2));print(json.dumps(results[:8],indent=2))
