import sys, json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['mixamo-retarget-manifest-v5.json'],'ladder-research')
out=[]
for role in ['corvo','emily']:
    rig=rig_for(role);m,frames=L.load(role,'mixamo_climbing_ladder')
    out.append(dict(role=role,duration=m['duration'],samples=[dict(frame=i,joints={rig.joints[j]['nativeName']:list(rig.fk(frames[i])[j].translation)for j in [2,9,16,21,25]})for i in [0,len(frames)//4,len(frames)//2,3*len(frames)//4,len(frames)-1]]))
(P/'ladder-research/reference.json').write_text(json.dumps(out,indent=2));print(json.dumps(out))
