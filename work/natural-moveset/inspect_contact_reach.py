import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['mechanisms-manifest-v2.json'],'contact-research')
for role in ['corvo','emily']:
 rig=rig_for(role);m,frames=L.load(role,'interact_wheel_sequence');f=int(len(frames)*.5);g=rig.fk(frames[f]);prop=qts(m['props'][0]['attachment_track'][f]);goals=[prop@Vector((-.025,.20,.11)),prop@Vector((-.025,-.20,-.11))]
 print(role,json.dumps(dict(points={rig.joints[i]['nativeName']:list(g[i].translation)for i in [2,7,8,9,14,15,16]},goals=[list(v)for v in goals],lengths=[rig.bind[i].translation.length for i in [8,9,15,16]])))
