import json,math
from pathlib import Path
p=Path(__file__).resolve().parent
reference=json.loads((p/'reference-grip-conversion.json').read_text())
def mul(a,b):return [[sum(a[i][k]*b[k][j] for k in range(3)) for j in range(3)] for i in range(3)]
def tr(a):return list(map(list,zip(*a)))
def rot(v):return [[v[3+c*3+r] for c in range(3)] for r in range(3)]
report={}
for char,role in [('corvo','client'),('emily','host')]:
 rows=json.loads((p/f'{role}-runtime-attachments.json').read_text(encoding='utf-8-sig'))
 expected=reference[char]['weapon_model_rotation'];errors=[]
 for row in rows:
  if not row['stable']:continue
  actual=mul(tr(rot(row['parentRenderRoot'])),rot(row['weaponRenderRoot']))
  delta=mul(actual,tr(expected));angle=math.degrees(math.acos(max(-1,min(1,(sum(delta[i][i] for i in range(3))-1)/2))))
  errors.append(angle)
 assert len(errors)>=4,'Need four coherent final snapshots'
 report[char]={'stable_samples':len(errors),'maximum_angle_error_degrees':max(errors),'mean_angle_error_degrees':sum(errors)/len(errors),'reference':'native first-person crouched weapon, transformed into player model coordinates','passed':max(errors)<1.0}
(p/'runtime-reference-comparison.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
assert all(v['passed'] for v in report.values()),'Remote blade orientation differs from native crouch reference'
