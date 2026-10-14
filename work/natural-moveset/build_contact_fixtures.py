"""Neutral authoring fixtures, explicitly not D2 assets or new game UI."""
import json,math
from pathlib import Path
P=Path(__file__).resolve().parent;OUT=P/'contact-fixtures';OUT.mkdir(exist_ok=True)
def model(kind):
 vertices=[];indices=[]
 def box(center,size):
  base=len(vertices);vertices.extend([[center[k]+size[k]*s[k]/2 for k in range(3)]for s in [(-1,-1,-1),(-1,-1,1),(-1,1,-1),(-1,1,1),(1,-1,-1),(1,-1,1),(1,1,-1),(1,1,1)]])
  for a,b,c,d in [(0,4,6,2),(1,3,7,5),(0,1,5,4),(2,6,7,3),(0,2,3,1),(4,5,7,6)]:indices.extend([base+a,base+b,base+c,base+a,base+c,base+d])
 if kind=='box':box((0,0,0),(.34,.38,.32))
 elif kind=='button':box((0,0,0),(.04,.10,.10))
 elif kind=='lever':box((0,0,.14),(.035,.035,.28));box((0,0,.28),(.07,.16,.05))
 elif kind=='key':box((.065,0,0),(.13,.014,.014));box((.015,0,0),(.025,.07,.045));box((.115,0,-.015),(.012,.012,.04))
 elif kind=='ladder':
  box((0,-.34,1.8),(.07,.07,3.6));box((0,.34,1.8),(.07,.07,3.6))
  for k in range(16):box((0,0,.11+k*.22),(.045,.70,.04))
 elif kind=='window':
  box((0,0,.325),(.14,2.0,.65));box((0,-.94,1.55),(.14,.12,1.8));box((0,.94,1.55),(.14,.12,1.8));box((0,0,2.45),(.14,2.0,.12))
 elif kind in ['wheel','crank']:
  major=.23 if kind=='wheel'else .13;minor=.018
  for i in range(48):
   a=i*math.tau/48
   for j in range(8):
    b=j*math.tau/8;vertices.append([minor*math.sin(b),(major+minor*math.cos(b))*math.cos(a),(major+minor*math.cos(b))*math.sin(a)])
  for i in range(48):
   for j in range(8):
    a=i*8+j;b=((i+1)%48)*8+j;c=((i+1)%48)*8+(j+1)%8;d=i*8+(j+1)%8;indices.extend([a,b,c,a,c,d])
  box((0,0,0),(.05,major*2,.028));box((0,0,0),(.05,.028,major*2))
  if kind=='crank':box((-.055,major,0),(.11,.035,.035))
 return dict(vertices=vertices,indices=indices,weights=[[['fixture_root',1.]]for _ in vertices],scope='Neutral geometric contact fixture; not an extracted native game asset')
sk=dict(joints=[dict(nativeName='fixture_root',parent=-1,bindLocalQTS=[0,0,0,1,0,0,0])])
(OUT/'skeleton.json').write_text(json.dumps(sk,indent=2))
for kind in ['box','button','lever','key','wheel','crank','ladder','window']:(OUT/(kind+'.json')).write_text(json.dumps(model(kind)))
print('CONTACT_FIXTURES',8)
