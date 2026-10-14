"""Compare final-published fixed D2 poses with the exact authored clip frame."""
import json,math,struct,statistics,sys,argparse,re
from pathlib import Path
from mathutils import Quaternion,Vector,Matrix
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
parser=argparse.ArgumentParser();parser.add_argument('--take',default='review-test-2');parser.add_argument('--manifest');parser.add_argument('--out',default='review-roundtrip.json')
args=parser.parse_args(sys.argv[sys.argv.index('--')+1:] if '--' in sys.argv else [])
take=P/args.take;result=read(take/'result.json');assert result['passed']
manifest=args.manifest or result.get('manifest','native-retarget-manifest.json')
metadata={r['id']:r for r in read(P/manifest)}
rows=[]
for c in result['cases']:
    meta=metadata[c['id']];b=(P/meta['binary']).read_bytes();n=meta['joints'];frame=c['frame']
    assert __import__('hashlib').sha256(b).hexdigest().lower()==c['bankSha'].lower(), 'Captured source hash changed'
    poses=read(Path(c['poseFile']));angles=[];positions=[];root_positions=[];helpers=0
    joints=read(P.parent/f"mixamo-crouch/native-{meta['role']}.json")['joints']
    checked=list(range(2,27))+[i for i,j in enumerate(joints)if i>=27 and re.fullmatch(r'(left|right)_(thumb|index|middle|ring|pinky)\d\d_jnt',j['nativeName'])]
    for s in poses['samples']:
        for j in checked:
            v=struct.unpack_from('<7f',b,28+(frame*n+j)*28);a=s['local'][j]
            m=Matrix([[a[col*4+row] if row<3 else float(col==3) for col in range(4)] for row in range(4)])
            expected=Quaternion((v[3],*v[:3])).normalized();actual=m.to_quaternion().normalized()
            angles.append(math.degrees(2*math.acos(min(1,abs(expected.dot(actual))))));positions.append((Vector(v[4:7])-m.translation).length)
    row=dict(id=c['id'],samples=poses['accepted'],rejected=poses['rejected'],checked_joints=len(checked),fingers_and_weapon_socket_included=True,max_anatomical_angle_degrees=max(angles),max_anatomical_translation_m=max(positions),root_travel=c['rootTravel'],native_helpers_excluded=True,visual_accepted=False)
    row['passed']=row['samples']>=10 and row['max_anatomical_angle_degrees']<.12 and row['max_anatomical_translation_m']<.00002 and row['root_travel']<.02
    rows.append(row)
report=dict(passed=all(r['passed'] for r in rows),scope='Fixed-frame conversion and final native publication only; visual quality, native weapon attachment and action-state binding remain separate.',cases=rows)
(P/args.out).write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2));assert report['passed']
