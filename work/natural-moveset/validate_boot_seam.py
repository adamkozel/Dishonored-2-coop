"""Skin-level regression against saved D2 poses, with comparison renders."""
import bpy,sys,json,struct,statistics,math
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from rig_math import NativeRig
from preview_skin import correct_calf_helpers,VERSION
exec(compile((P/'preview_natural_walk.py').read_text().split("for name in ('idle'")[0],str(P/'preview_natural_walk.py'),'exec'),globals())
OUT=P/'boot-seam-v1';OUT.mkdir(exist_ok=True)
for role,rig,J,B,sword in rigs:
 sword.hide_render=True
 if role=='emily':
  for obj in rig.children:obj.hide_render=True
role,rig,J,B,sword=rigs[0];rig.location=(0,0,0);nr=NativeRig(J)
body=read(P.parent/'fullbody-proof/corvo-body.json');lookup={j['nativeName']:i for i,j in enumerate(J)}
calf=[i for i,ws in enumerate(body['weights'])if sum(w for n,w in ws if n in ('left_helper_ankle_jnt','right_helper_ankle_jnt'))>.5]
obj=next(o for o in rig.children if o.name=='corvo_body')
scene.render.resolution_x=750;scene.render.resolution_y=850
reports=[]
studies=[('boot-seam-native-test-1','sword_walk_e_add'),('stance-pose-test-1','crouch_idle'),('jumps-v3-pose-test-1','standing_jump_sequence'),('carry-release-v3-pose-test-1','carry_body_drop_low_sequence')]
for folder,alias in studies:
 result=read(P/folder/'result.json');case=next(c for c in result['cases']if c['role']=='corvo'and c['alias']==alias)
 m=next(m for m in read(P/result['manifest'])if m['id']==case['id']);binary=(P/m['binary']).read_bytes();local=[matrix(struct.unpack_from('<7f',binary,28+(case['frame']*m['joints']+j)*28))for j in range(m['joints'])]
 s=read(Path(case['poseFile']))['samples'][0];actual=[Matrix([[a[col*4+row]if row<3 else float(col==3)for col in range(4)]for row in range(4)])for a in s['local']]
 G=nr.fk(local);center=sum((G[i].translation for i in [20,21,24,25]),Vector())*.25
 camera.data.ortho_scale=1.4;camera.location=center+Vector((3.7,-4,1.8));camera.rotation_euler=(center-camera.location).to_track_quat('-Z','Y').to_euler();skins={};states={}
 for label in ['before','corrected','native_helpers']:
  pose=[x.copy()for x in local]
  if label=='corrected':correct_calf_helpers(J,B,pose)
  if label=='native_helpers':
   for i,j in enumerate(J):
    if 'helper'in j['nativeName']:pose[i]=actual[i].copy()
  states[label]=nr.fk(pose)
  if label=='corrected':assert all(pose[i]==local[i]for i in range(len(J))if i not in [196,203])
  for i,j in enumerate(J):rig.pose.bones[j['nativeName']].matrix_basis=B[i].inverted()@pose[i]
  bpy.context.view_layer.update();scene.render.filepath=str(OUT/f'{alias}-{label}.png');bpy.ops.render.render(write_still=True)
  ev=obj.evaluated_get(bpy.context.evaluated_depsgraph_get());mesh=ev.to_mesh();skins[label]=[mesh.vertices[i].co.copy()for i in calf];ev.to_mesh_clear()
 errors={label:dict(mean=statistics.mean((p-q).length for p,q in zip(skins[label],skins['native_helpers'])),max=max((p-q).length for p,q in zip(skins[label],skins['native_helpers'])))for label in ['before','corrected']}
 anchor_error=max((states['corrected'][i].translation-states['native_helpers'][i].translation).length for i in [196,203])
 reports.append(dict(alias=alias,vertices=len(calf),skin_error_m=errors,corrected_anchor_error_m=anchor_error,frame=case['frame'],capture=case['poseFile'],passed=errors['corrected']['mean']<errors['before']['mean'] and anchor_error<1e-5))
report=dict(version=VERSION,passed=all(r['passed']for r in reports),scope='Same anatomy, skin weights and camera; old preview vs corrected anchors vs captured native helper transforms. Axial twist remains approximate.',cases=reports)
(OUT/'validation.json').write_text(json.dumps(report,indent=2));print(json.dumps(report));assert report['passed']
