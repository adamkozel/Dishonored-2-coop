"""Read-only geometric screen; approximate blade centreline is not visual acceptance."""
import sys,struct,json,math
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
reports=[]
for role in ('corvo','emily'):
 data=(P/'outofmana-context-banks-v46'/f'{role}-outofmana-context-bank.bin').read_bytes()
 magic,version,n,count,fps,layout=struct.unpack_from('<8sIIIIQ',data);assert version==41 and count==251
 size=24+121*n*28;rig=rig_for(role)
 q=read(P.parent/('fullbody-proof/inputs/client-target-sword.json'if role=='corvo'else'crouch-grip-reference/host-target-sword.json'))['quaternion']
 binding=Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix().to_4x4()
 for cid in (17,239,242,245,249):
  samples=[]
  for f in range(0,120,6):
   pose=[qts(struct.unpack_from('<7f',data,32+cid*size+24+(f*n+j)*28))for j in range(n)]
   g=rig.fk(pose);weapon=g[17]@binding
   samples.append(dict(frame=f,bladeMinZ=min((weapon@Vector((x,0,0))).z for x in(-.1,0,.2,.4,.6,.75)),handZ=g[16].translation.z,ankleZ=[g[j].translation.z for j in(21,25)],toeZ=[g[j].translation.z for j in(22,26)]))
  reports.append(dict(role=role,clip=cid,samples=samples))
(P/'crouch-power-clearance-screen-v49.json').write_text(json.dumps(reports,indent=2))
for r in reports:print('BLADE_SCREEN',r['role'],r['clip'],min(s['bladeMinZ']for s in r['samples']),min(s['handZ']for s in r['samples']),r['samples'][0]['toeZ'])
