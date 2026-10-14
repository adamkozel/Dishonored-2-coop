"""Native crossbow geometry and hand reference contact feasibility scene."""
import bpy,sys,json,struct,math
from pathlib import Path
from mathutils import Matrix,Vector,Quaternion
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import Library,rig_for
from rig_math import qts,NativeRig,preview_helpers
from finger_reference import HandReference,SR,SN
from edge_data import poses as edge_poses
MODEL_OVERRIDES={'emily-body':P/'item-models/emily-body-high.json'}
src=(P/'preview_natural_walk.py').read_text();exec(compile(src.split("for name in ('idle'")[0],str(P/'preview_natural_walk.py'),'exec'),globals())
OUT=P/'item-contact-v2';OUT.mkdir(exist_ok=True)
bpy.context.view_layer.update()
L=Library(['library-retarget-manifest.json'],'item-contact-v2')
refs=read(P/'fp-item-reference-manifest.json')['rows'];mechanisms=read(P/'item-mechanism-manifest.json');evidence=[];wrist_targets={}
for role,blender_rig,J,B,sword in rigs:
 rig=rig_for(role);hand=HandReference(rig);bodymeta,body=L.load(role,'sword_gun_idle');pose=body[0]
 source=next(r for r in refs if 'ply_leftarm_crossbow_idle'in r['aliases']and('/'+role+'/')in r['asset'])
 sampled=edge_poses(P/source['sample_file']);fp=[qts(v)for v in sampled['frames'][0]]
 hand.apply(pose,fp);preview_helpers(rig,pose);g=rig.fk(pose)
 for i,j in enumerate(J):blender_rig.pose.bones[j['nativeName']].matrix_basis=B[i].inverted()@pose[i]
 q=read(P.parent/('fullbody-proof/inputs/client-target-sword.json'if role=='corvo'else'crouch-grip-reference/host-target-sword.json'))['quaternion']
 sword.matrix_world=blender_rig.matrix_world@g[17]@Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix().to_4x4()
 kind='crossbow_'+role;d=read(P/'item-models'/ (kind+'.json'));mr=NativeRig(read(P/'item-mechanisms'/(kind+'-skeleton.json'))['joints'])
 mechanism=next(r for r in mechanisms if r['id']==kind+'-crossbow_idle');weapon=edge_poses(P/mechanism['sample_file']);wp=[qts(v)for v in weapon['frames'][0]];wg=mr.fk(wp);bone_lookup={j['nativeName']:i for i,j in enumerate(mr.joints)}
 deform={name:wg[i]@mr.global_bind[i].inverted()for name,i in bone_lookup.items()};vertices=[]
 for v,weights in zip(d['vertices'],d['weights']):
  p=Vector(v);out=Vector((0,0,0));total=0
  for name,w in weights:
   if w>0:out+=(deform[name]@p)*w;total+=w
  vertices.append(out if total else p)
 mesh=bpy.data.meshes.new(kind);ix=d['indices'];mesh.from_pydata(vertices,[],[ix[i:i+3]for i in range(0,len(ix),3)]);mesh.update();obj=bpy.data.objects.new(kind,mesh);scene.collection.objects.link(obj)
 material=bpy.data.materials.new(kind+'_preview_metal');material.diffuse_color=(.37,.34,.27,1);mesh.materials.append(material)
 # Only equipment placement uses the FP hand-local attachment reference.
 # It follows the target body's freely moving wrist; it cannot move it.
 fg=SR.fk(fp);relative=fg[4].inverted()@fg[SN['left_hand_attachment_jnt']]
 attach=hand.invcal.to_4x4()@relative;obj.matrix_world=blender_rig.matrix_world@g[9]@attach
 wrist_targets[role]=blender_rig.matrix_world@g[9].translation
 evidence.append(dict(role=role,body_sha256=bodymeta['sha256'],fp_sha256=source['sample_sha256'],weapon_sha256=mechanism['sample_sha256'],attachment_local=[list(r)for r in attach],source='Native FP left_hand_attachment_jnt in wrist coordinates, palm-basis calibration only',visual_accepted=False))
bpy.context.view_layer.update();scene.render.resolution_x=1400;scene.render.resolution_y=900;camera.data.ortho_scale=3.7
camera.location=(4.7,-5.2,2.8);camera.rotation_euler=(Vector((.1,0,1))-camera.location).to_track_quat('-Z','Y').to_euler();scene.render.filepath=str(OUT/'both-crossbow-idle.png');bpy.ops.render.render(write_still=True)
for role in ['corvo','emily']:
 for suffix,offset in [('front',Vector((1.2,-1.8,.7))),('side',Vector((1.2,1.4,.5)))]:
  target=wrist_targets[role];camera.data.ortho_scale=.85;camera.location=target+offset;camera.rotation_euler=(target-camera.location).to_track_quat('-Z','Y').to_euler();scene.render.filepath=str(OUT/(role+'-'+suffix+'.png'));bpy.ops.render.render(write_still=True)
(OUT/'contact-evidence.json').write_text(json.dumps(evidence,indent=2));bpy.ops.wm.save_as_mainfile(filepath=str(OUT/'native-crossbow-contact.blend'))
