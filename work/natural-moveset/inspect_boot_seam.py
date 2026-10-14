"""Compare one identical pose with old helpers, shin-bound helpers and D2 helpers."""
import bpy,sys,json,struct
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from rig_math import NativeRig
exec(compile((P/'preview_natural_walk.py').read_text().split("for name in ('idle'")[0],str(P/'preview_natural_walk.py'),'exec'),globals())
OUT=P/'boot-seam-v1';OUT.mkdir(exist_ok=True)
for role,rig,J,B,sword in rigs:
 sword.hide_render=True
 if role=='emily':
  for obj in rig.children:obj.hide_render=True
role,rig,J,B,sword=rigs[0];rig.location=(0,0,0);nr=NativeRig(J)
result=read(P/'boot-seam-native-test-1/result.json');case=next(c for c in result['cases']if c['role']=='corvo')
m=next(m for m in read(P/'library-retarget-manifest.json')if m['id']==case['id']);binary=(P/m['binary']).read_bytes()
local=[matrix(struct.unpack_from('<7f',binary,28+(case['frame']*m['joints']+j)*28))for j in range(m['joints'])]
s=read(Path(case['poseFile']))['samples'][0]
actual=[Matrix([[a[col*4+row]if row<3 else float(col==3)for col in range(4)]for row in range(4)])for a in s['local']]
scene.render.resolution_x=1000;scene.render.resolution_y=1000
g=nr.fk(local);center=(g[20].translation+g[24].translation+g[21].translation+g[25].translation)*.25
camera.data.ortho_scale=1.25;camera.location=center+Vector((3.7,-4,1.8));camera.rotation_euler=(center-camera.location).to_track_quat('-Z','Y').to_euler()
reports=[]
for label in ['old','shin','native']:
 pose=[x.copy()for x in local]
 if label=='shin':
  for i in [196,203]:pose[i]=B[i].copy()
 if label=='native':
  for i,j in enumerate(J):
   if 'helper'in j['nativeName']:pose[i]=actual[i].copy()
 for i,j in enumerate(J):rig.pose.bones[j['nativeName']].matrix_basis=B[i].inverted()@pose[i]
 bpy.context.view_layer.update();scene.render.filepath=str(OUT/(label+'.png'));bpy.ops.render.render(write_still=True)
 meshobj=next(o for o in rig.children if o.type=='MESH'and o.name=='corvo_body');evaluated=meshobj.evaluated_get(bpy.context.evaluated_depsgraph_get());mesh=evaluated.to_mesh();positions=[list(v.co)for v in mesh.vertices];evaluated.to_mesh_clear()
 reports.append(dict(label=label,positions=positions,helpers={str(i):list(nr.fk(pose)[i].translation)for i in [196,203]}))
(OUT/'skin-comparison.json').write_text(json.dumps(reports))
print('BOOT_SEAM_STUDY_COMPLETE')
