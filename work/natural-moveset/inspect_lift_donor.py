"""Locate reach, lift and release phases in the authored heavy pickup donor."""
import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['heavy-pickup-retarget-manifest-v1.json'],'carry-research');out=[]
for role in ['corvo','emily']:
 m,ps=L.load(role,'mixamo_picking_up');rig=rig_for(role);rows=[]
 for f,p in enumerate(ps):
  g=rig.fk(p);world=qts(m['root_track'][f]);rows.append(dict(t=f/m['fps'],pelvis=list(world@g[2].translation),left=list(world@g[9].translation),right=list(world@g[16].translation),left_foot=list(world@g[21].translation),right_foot=list(world@g[25].translation)))
 low=min(range(len(rows)),key=lambda i:(rows[i]['left'][2]+rows[i]['right'][2])/2);report=dict(role=role,duration=m['duration'],lowest_hands=rows[low],samples=[rows[min(len(rows)-1,round(t*m['fps']))]for t in range(12)]);out.append(report);print(role,'LOW',rows[low]);print('HEIGHTS',[(r['t'],round(r['pelvis'][2],2),round((r['left'][2]+r['right'][2])/2,2))for r in report['samples']])
(P/'carry-research/heavy-pickup-phases.json').write_text(json.dumps(out,indent=2))
