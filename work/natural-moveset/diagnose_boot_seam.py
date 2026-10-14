"""Bounded comparison of preview calf helpers with final D2 publication."""
import sys,json,struct,math
from pathlib import Path
from mathutils import Matrix,Quaternion
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import rig_for,read,qts
def mat(a):return Matrix([[a[col*4+row] if row<3 else float(col==3) for col in range(4)]for row in range(4)])
rows=[]
for folder in ['stance-pose-test-1','review-test-2','jumps-v3-pose-test-1','carry-release-v3-pose-test-1']:
 r=read(P/folder/'result.json');metas={m['id']:m for m in read(P/r.get('manifest','native-retarget-manifest.json'))}
 for c in r['cases']:
  if c['role']!='corvo':continue
  m=metas[c['id']];rig=rig_for('corvo');b=(P/m['binary']).read_bytes();local=[qts(struct.unpack_from('<7f',b,28+(c['frame']*m['joints']+i)*28))for i in range(m['joints'])];s=read(Path(c['poseFile']))['samples'][0];actual=[mat(a)for a in s['local']];G=rig.fk(local);A=rig.fk(actual)
  for i,k in [(196,21),(203,25)]:
   bind=rig.bind[i];q=(bind.inverted()@actual[i]).to_quaternion();aq=(rig.bind[k].inverted()@actual[k]).to_quaternion();delta=(A[i].translation-G[i].translation).length
   row=dict(clip=m.get('selection',{}).get('alias',m['id']),helper=i,preview_native_position_error=delta,native_translation_error=(actual[i].translation-bind.translation).length,helper_delta_q=list(q),ankle_delta_q=list(aq),native_helper_translation=list(actual[i].translation),old_preview_translation=list(local[i].translation));rows.append(row)
(P/'boot-seam-diagnosis.json').write_text(json.dumps(rows,indent=2))
for r in rows:print(r['clip'],r['helper'],'poserr',round(r['preview_native_position_error'],4),'helperQ',[round(v,4)for v in r['helper_delta_q']],'ankleQ',[round(v,4)for v in r['ankle_delta_q']])
