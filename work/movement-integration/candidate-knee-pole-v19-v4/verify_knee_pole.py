"""Check actual D2 publication before/after the authored knee-plane correction.

Phase2 is the native terrain solution; phase3 is the final corrected skeleton.
This does not replace the existing flat-ground ankle or visual acceptance test.
"""
import argparse,json,math
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('folder',type=Path);p.add_argument('--min-poses',type=int,default=5);a=p.parse_args()
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
dot=lambda a,b:sum(x*y for x,y in zip(a,b))
sub=lambda a,b:[x-y for x,y in zip(a,b)]
norm=lambda a:math.sqrt(dot(a,a))
def pole_error(ref,actual,hip):
 axis=sub(actual[hip+2][12:15],actual[hip][12:15]);axis=[x/norm(axis)for x in axis]
 out=[]
 for poses in (ref,actual):
  knee=sub(poses[hip+1][12:15],poses[hip][12:15]);d=dot(knee,axis);v=[x-d*y for x,y in zip(knee,axis)];n=norm(v)
  if n<.005:return None
  out.append([x/n for x in v])
 return math.degrees(math.acos(max(-1,min(1,dot(*out)))))
result=read(a.folder/'result.json');assert result['failure'] is None
rows=[]
for f in a.folder.glob('pose-*.json'):
 s=read(f);assert s['stable']and s['finalPublicationComplete']
 ph={x['phase']:{m['joint']:m['matrix'] for m in x['matrices']}for x in s['phases']}
 before,after=ph[2],ph[3];leg=[]
 for hip in (19,23):
  endpoint=max(math.dist(before[j][12:15],after[j][12:15])for j in (hip,hip+2))
  angle=max(abs(before[hip+2][k]-after[hip+2][k])for k in (0,1,2,4,5,6,8,9,10))
  length=max(abs(math.dist(before[j][12:15],before[j-1][12:15])-math.dist(after[j][12:15],after[j-1][12:15]))for j in (hip+1,hip+2))
  old,new=pole_error(ph[0],before,hip),pole_error(ph[0],after,hip)
  assert endpoint<.00005,(f.name,'native terrain endpoint changed',endpoint)
  assert angle<.00005,(f.name,'native terrain foot orientation changed',angle)
  assert length<.00005,(f.name,'native leg length changed',length)
  assert new is None or new<.1,(f.name,'authored knee plane lost',new)
  leg.append(dict(hip=hip,endpoint_error_m=endpoint,ankle_rotation_element_error=angle,bone_length_error_m=length,pole_error_before_deg=old,pole_error_after_deg=new))
 rows.append(dict(pose=f.name,legs=leg))
assert a.min_poses>=1 and len(rows)>=a.min_poses,'Incomplete native phase evidence'
states=read(a.folder/'states.json');assert max(x['movement'].get('kneeCorrections',0)for x in states)>50,'No live knee correction evidence'
out=dict(passed=True,dll=result['dll'],visual_accepted=False,scope='Native phase2 to final phase3: authored knee planes with native endpoint, foot orientation and bone lengths preserved.',poses=rows)
(a.folder/'knee-pole-validation.json').write_text(json.dumps(out,indent=2),encoding='utf-8')
print('PASS_KNEE_POLE_NATIVE',a.folder.name,len(rows),'complete native poses')
