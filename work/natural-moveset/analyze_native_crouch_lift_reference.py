"""Read existing exact-game victim geometry; no first-person wrist constraints."""
import json,hashlib,statistics
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def world(s,name):
 p=next(j['globalPose'][12:15]for j in s['joints']if j['nativeName']==name);r=s['root']
 return [r[k]+sum(r[3+c*3+k]*p[c]for c in range(3))for k in range(3)]
out=[]
for role in ['corvo','emily']:
 for crouched in [False,True]:
  take=(f'native-pickup-crouched-{role}-'+('v21b'if role=='corvo'else'v21'))if crouched else f'native-pickup-geometry-{role}-v1'
  path=P/take/'observations.json';samples=[]
  for r in read(path):
   n=r.get('native');s=r.get('skeleton')
   if not n or not s or not s.get('stable')or not n.get('carrySceneValid'):continue
   scene=n['carryScene'];v=scene['variant'];p=n['position']
   if v not in [1,2]:continue
   samples.append(dict(variant=v,phase=scene['timeMs']/scene['durationMs'],stance=n['stance'],span_ms=r['endMs']-r['beginMs'],
    root_height=s['root'][2]-p[2],abdomen_height=world(s,'center_spine02_jnt')[2]-p[2],anchor_height=world(s,'anchor_jnt')[2]-p[2]))
  groups=[]
  for variant in [1,2]:
   for lo,hi in ([(k*.25,(k+1)*.25)for k in range(4)]if variant==1 else [(0,10)]):
    group=[x for x in samples if x['variant']==variant and lo<=x['phase']<hi]
    if group:groups.append(dict(variant=variant,phase=[lo,hi],samples=len(group),
     **{key:statistics.median(x[key]for x in group)for key in ['root_height','abdomen_height','anchor_height']}))
  out.append(dict(role=role,crouched=crouched,take=take,source_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),groups=groups,samples=samples))
result=dict(cases=out,scope='Sequential native victim skeleton references from prior exact-game recordings. Heights relative to source player root; first-person hands never used as body anchors. No runtime crouched-pickup implementation or visual acceptance.',complete=False)
(P/'carry-research/native-crouch-lift-heights.json').write_text(json.dumps(result,indent=2))
print(json.dumps([{k:v for k,v in r.items()if k!='samples'}for r in out],indent=2))
