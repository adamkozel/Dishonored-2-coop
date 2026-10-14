import sys,json,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['mixamo-retarget-manifest-v5.json','library-retarget-manifest.json'],'transition-research')
out=[]
for alias in ['mixamo_climbing_to_top','mixamo_swimming_to_edge','mixamo_freehang_climb','mixamo_climbing_ladder','sword_mantlemiddle1_walkleft_in','sword_mantlehigh_walkleft_in','sword_mantle_walk_out']:
 for role in ['corvo','emily']:
  m,fs=L.load(role,alias);rig=rig_for(role);rows=[]
  for t in [0,.5,1,1.5,2,2.5,3,3.5,4,4.5,5]:
   if t>m['duration']:continue
   i=min(len(fs)-1,round(t*m['fps']));g=rig.fk(fs[i]);rt=qts(m['root_track'][i]);rows.append(dict(t=t,root=[round(v,4)for v in rt.translation],joints={rig.joints[j]['nativeName']:[round(v,4)for v in (rt@g[j]).translation]for j in [2,9,16,21,25]}))
  out.append(dict(alias=alias,role=role,duration=m['duration'],samples=rows))
(P/'transition-research/donor-phases.json').write_text(json.dumps(out,indent=2))
print(json.dumps(out))
