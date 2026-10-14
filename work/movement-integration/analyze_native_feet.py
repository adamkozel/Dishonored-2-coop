"""Compare complete native pose publication phases; no game writes."""
import json, math, sys
from pathlib import Path
folder=Path(sys.argv[1]);rows=[]
for f in folder.glob('pose-*.json'):
 s=json.loads(f.read_text(encoding='utf-8-sig'))
 assert s['stable'] and s['finalPublicationComplete']
 phases={p['phase']:{v['joint']:v['matrix'] for v in p['matrices']} for p in s['phases']}
 changes=[]
 for j in s['joints']:
  i=j['index']
  if i not in phases.get(0,{}) or i not in phases.get(3,{}):continue
  a,b=phases[0][i],phases[3][i]
  pos=math.dist(a[12:15],b[12:15]);mat=max(abs(a[k]-b[k]) for k in [0,1,2,4,5,6,8,9,10,12,13,14])
  if mat>1e-5:changes.append(dict(joint=i,name=j['nativeName'],position_delta=pos,matrix_delta=mat))
 rows.append(dict(file=f.name,changed=len(changes),changes=changes))
out=folder/'phase-analysis.json';out.write_text(json.dumps(rows,indent=2))
for r in rows:
 print(r['file'],r['changed'],'changed')
 for v in r['changes']:
  if any(x in v['name'] for x in ['ankle','hip','knee','pelvis','wrist']):print(v)
