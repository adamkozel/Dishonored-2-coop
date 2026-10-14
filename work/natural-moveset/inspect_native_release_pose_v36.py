import sys,json,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from rig_math import NativeRig,read,qts
from edge_data import poses
rows=read(P/'native-expanded-sample-manifest.json')['rows'];out=[]
for role,base in [('corvo','tall'),('emily','female')]:
 rig=NativeRig(read(P/f'base-{base}.json')['joints']);idx={j['nativeName']:i for i,j in enumerate(rig.joints)}
 for alias in ['generic_carrycorpse_idle','generic_carrycorpse_drophigh','generic_carrycorpse_dropfoetal']:
  m=next(r for r in rows if r['source_role']==role and alias in r['aliases']);p=poses(P/m['sample_file']);sampled=[]
  for t in [0,.033,.083,.133,.166,.25]:
   f=min(len(p['frames'])-1,round(t*p['fps']));g=rig.fk([qts(v)for v in p['frames'][f]])
   knees={}
   for side in ['left','right']:
    a,b,c=[g[idx[f'{side}_{name}_jnt']].translation for name in ['femur','tibia','ankle']]
    knees[side]=180-math.degrees((a-b).angle(c-b))
   sampled.append(dict(time=f/p['fps'],knees=knees,hips=list(g[idx['center_hips_jnt']].translation)if 'center_hips_jnt'in idx else None,head=list(g[idx['center_head_jnt']].translation)if 'center_head_jnt'in idx else None))
  out.append(dict(role=role,alias=alias,frames=len(p['frames']),fps=p['fps'],source=m['sample_file'],samples=sampled))
(P/'carry-research/native-release-pose-screening-v36.json').write_text(json.dumps(out,indent=2))
print(json.dumps(out))
