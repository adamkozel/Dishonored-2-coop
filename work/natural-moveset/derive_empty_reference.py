"""Validate a constant local additive reference against two complete native clips."""
import sys,json,math,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from mathutils import Quaternion
from edge_data import poses
from rig_math import read
rows=read(P/'native-allcatalog-sample-manifest.json')['rows']
def get(alias):
 r=next(r for r in rows if alias in r['aliases'] and r['source_role']=='corvo' and r['status']=='native_pose_sampled_offline')
 return r,poses(P/r['sample_file'])['frames']
def q(v):return Quaternion((v[3],*v[:3])).normalized()
om,of=get('empty_walk_n');dm,df=get('empty_walk_n_add');ref=[]
for a,b in zip(of[0],df[0]):
 rotation=(q(a)@q(b).inverted()).normalized();ref.append([rotation.x,rotation.y,rotation.z,rotation.w,*[a[k]-b[k]for k in range(4,7)]])
results=[]
for gait in ('walk','run'):
 om,of=get('empty_'+gait+'_n');dm,df=get('empty_'+gait+'_n_add');assert len(of)==len(df)
 err=[];translations=[]
 for f,(a,b) in enumerate(zip(of,df)):
  for j,(x,y) in enumerate(zip(a,b)):
   rotation=(q(ref[j])@q(y)).normalized();degrees=math.degrees(rotation.rotation_difference(q(x)).angle)
   degrees=min(degrees,360-degrees);distance=max(abs(x[k]-ref[j][k]-y[k])for k in range(4,7))
   err.append(degrees);translations.append(distance)
 results.append(dict(gait=gait,original=om['sample_sha256'],additive=dm['sample_sha256'],frames=len(of),joints=len(ref),maximum_degrees=max(err),mean_degrees=sum(err)/len(err),maximum_translation=max(translations)))
out=dict(scope='Analytically reconstructed constant local reference; validated against all frames/joints of independently sampled native forward walk and run. Sideways playback still needs visual validation.',source_character='corvo',reference=ref,validation=results)
(P/'empty-additive-reference.json').write_text(json.dumps(out,indent=2),encoding='utf-8');print(json.dumps(results,indent=2))
