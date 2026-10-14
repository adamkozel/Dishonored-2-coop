import json,math,sys
from pathlib import Path
p=Path(__file__).resolve().parent
name=sys.argv[1] if len(sys.argv)>1 else 'sword-before-fix'
data=json.loads((p/(name+'.json')).read_text(encoding='utf-8-sig'))
def mul(a,b):return [sum(a[k*3+i%3]*b[i//3*3+k] for k in range(3)) for i in range(9)]
rows=[]
for a in data:
 if not a['stable']:continue
 assert a['joint']==17 and a['tag']=='right_hand_wpn' and not any(a['offset'])
 x,y,z,w=a['quaternion']
 q=[1-2*(y*y+z*z),2*(x*y-z*w),2*(x*z+y*w),2*(x*y+z*w),1-2*(x*x+z*z),2*(y*z-x*w),2*(x*z-y*w),2*(y*z+x*w),1-2*(x*x+y*y)]
 j=a['jointMatrix'];r=a['parentRenderRoot']
 pos=[r[i]+sum(r[3+k*3+i]*j[12+k] for k in range(3)) for i in range(3)]
 basis=mul(r[3:],mul([j[i//3*4+i%3] for i in range(9)],q))
 rows.append({'tick':a['tick'],'position_error_m':math.dist(pos,a['weaponRenderRoot'][:3]),'basis_error':max(abs(v-b) for v,b in zip(basis,a['weaponRenderRoot'][3:]))})
assert len(rows)>=3,'Insufficient coherent attachment samples'
report={'source':name+'.json','scope':'Rendered sword vs native phase-3 attachment reconstruction; does not prove visual finger grip or absence of transient lag','stable_samples':len(rows),'max_position_error_m':max(r['position_error_m'] for r in rows),'max_basis_error':max(r['basis_error'] for r in rows),'rows':rows}
(p/(name+'-analysis.json')).write_text(json.dumps(report,indent=2))
print(json.dumps({k:v for k,v in report.items() if k!='rows'},indent=2))
