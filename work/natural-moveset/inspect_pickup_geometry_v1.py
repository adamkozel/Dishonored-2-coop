"""Read native victim pickup geometry before designing a carrier timeline."""
import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['carry-victim-retarget-manifest.json','carry-body-manifest-v3.json'],'pickup-geometry-v1')
rig=rig_for('corvo');m,frames=L.load('corvo','generic_carrycorpse_in')
rows=[]
for u in [0,.1,.2,.3,.4,.5,.6,.7,.8,.9,1]:
    g=rig.fk(sample(frames,u));root=sample([[qts(r)]for r in m['root_track']],u)[0]
    rows.append(dict(time=u*m['duration'],root=row(root),joints={rig.joints[j]['nativeName']:list(g[j].translation)for j in [2,4,5,7,9,14,16,19,20,21,23,24,25]}))
result=dict(source=m['source'],source_duration=m['duration'],frames=rows,complete=False,
    scope='Offline retargeted native victim pickup shape. Does not prove native ragdoll blending or current floor contact.')
(P/'pickup-geometry-v1/geometry.json').write_text(json.dumps(result,indent=2))
print(json.dumps(result),flush=True)
