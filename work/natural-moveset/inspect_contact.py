import sys,struct,json,math
from pathlib import Path
from mathutils import Vector
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from rig_math import NativeRig,read,qts
library=read(P/'library-retarget-manifest.json');rig=NativeRig(read(P.parent/'mixamo-crouch/native-corvo.json')['joints'])
report=[]
for alias in ['sword_walk_n','sword_run_n']:
 m=next(m for m in library if m['role']=='corvo' and m['selection']['alias']==alias);raw=(P/m['binary']).read_bytes();n=m['joints'];samples=[]
 for f in range(m['frames']):
  local=[qts(struct.unpack_from('<7f',raw,28+(f*n+j)*28)) for j in range(n)];g=rig.fk(local);root=qts(m['root_track'][f]);samples.append(dict(frame=f,root=list(root.translation),left=list(g[21].translation),right=list(g[25].translation),world_left=list((root@g[21]).translation),world_right=list((root@g[25]).translation)))
 print(alias,'duration',m['duration'],'root',samples[-1]['root']);print(json.dumps(samples[::max(1,len(samples)//10)],indent=1))
 report.append(dict(alias=alias,source=m['id'],samples=samples))
(P/'source-contact-inspection.json').write_text(json.dumps(report,indent=2))
