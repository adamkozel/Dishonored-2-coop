"""Adapt native hands-free turn acting to the existing complete crouch stance.

Uses the same anatomical method as the archived armed crouch-turn authoring.
No camera-space arm target, world root motion, damage event or game write.
"""
import sys,struct,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['turn-retarget-manifest-v1.json'],'unarmed-crouch-turns-v1')
for role in ('corvo','emily'):
 rig=rig_for(role);n=len(rig.joints)
 bank=(P/'terrain-banks-v2'/f'{role}-terrain-bank.bin').read_bytes()
 entry=24+121*n*28
 crouch=[qts(struct.unpack_from('<7f',bank,32+76*entry+24+j*28))for j in range(n)]
 cg=rig.fk(crouch)
 for degrees in (45,90,180):
  for side in ('left','right'):
   alias=f'empty_turn{side}{degrees}';m,fs=L.load(role,alias)
   reference=fs[0];rg=rig.fk(reference)
   delta={i:(crouch[i].to_quaternion()@reference[i].to_quaternion().inverted()).normalized()for i in range(2,27)if i!=17}
   shifts=[cg[i].translation-rg[i].translation for i in (21,25)]
   frames=round(m['duration']*60)+1;poses=[];roots=[];errors=[]
   for f in range(frames):
    u=f/(frames-1);source=sample(fs,u);g=rig.fk(source);pose=copy_pose(source)
    for i,d in delta.items():
     q=(d@pose[i].to_quaternion()).normalized();t=pose[i].translation.copy();pose[i]=q.to_matrix().to_4x4();pose[i].translation=t
    pose[2].translation+=crouch[2].translation-reference[2].translation
    targets=[g[i].translation+shifts[k]for k,i in enumerate((21,25))]
    pin_feet(rig,pose,targets,[g[i].to_3x3()for i in (21,25)])
    actual=rig.fk(pose);errors.append(max((actual[i].translation-targets[k]).length for k,i in enumerate((21,25))))
    poses.append(pose)
    at=u*(len(m['root_track'])-1);a=int(at);b=min(a+1,len(m['root_track'])-1)
    roots.append(row(blend([qts(m['root_track'][a])],[qts(m['root_track'][b])],at-a)[0]))
   L.save(rig,role,f'unarmed_crouch_turn_{side}{degrees}',poses,roots,'stance',
    'Complete low stance with native empty-hand turning, distributed spine and free arms. Native segment lengths and source foot trajectory are retained. No first-person constraints. Runtime contact and natural movement require recorded review.',
    dict(native_alias=alias,native_sha256=m['sha256'],crouch_bank_sha256=hashlib.sha256(bank).hexdigest()),max_foot_target_error=max(errors))
   L.reports[-1]['equipment_policy']='hands_free'
   (P/L.reports[-1]['binary']).with_suffix('.json').write_text(json.dumps(L.reports[-1],indent=2),encoding='utf-8')
L.finish('unarmed-crouch-turns-manifest-v1.json')
