"""Native weighted preview of the new bank. No changes to preserved scenes."""
import bpy,json,math
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
P=Path(__file__).resolve().parent;OUT=P/'previews';OUT.mkdir(exist_ok=True)
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def matrix(v):
 m=Quaternion((v[3],*v[:3])).normalized().to_matrix().to_4x4();m.translation=Vector(v[4:7]);return m
bpy.ops.wm.read_factory_settings(use_empty=True);scene=bpy.context.scene
scene.render.engine='BLENDER_WORKBENCH';scene.render.resolution_x=1300;scene.render.resolution_y=900;scene.render.resolution_percentage=100
scene.display.shading.light='STUDIO';scene.display.shading.color_type='MATERIAL';scene.display.shading.show_cavity=True;scene.display.shading.show_shadows=True
scene.display.shading.background_type='WORLD';scene.world=bpy.data.worlds.new('PreviewWorld');scene.world.color=(.055,.055,.06)
rigs=[]
for n,role in enumerate(('corvo','emily')):
 native=read(P.parent/f'mixamo-crouch/native-{role}.json');J=native['joints'];B=[matrix(j['bindLocalQTS']) for j in J];BG=[]
 rig=bpy.data.objects.new(role,bpy.data.armatures.new(role+'_skeleton'));scene.collection.objects.link(rig);bpy.context.view_layer.objects.active=rig;rig.select_set(True)
 bpy.ops.object.mode_set(mode='EDIT')
 for i,j in enumerate(J):
  parent=j['parent'];g=B[i] if parent<0 else BG[parent]@B[i];BG.append(g);b=rig.data.edit_bones.new(j['nativeName']);b.length=.045;b.matrix=g
  if parent>=0:b.parent=rig.data.edit_bones[J[parent]['nativeName']]
 bpy.ops.object.mode_set(mode='OBJECT');rig.select_set(False);rig.location=(0,(n-.5)*1.5,0)
 for pb in rig.pose.bones:pb.rotation_mode='QUATERNION'
 for part in ('body','head'):
  default_model=(P.parent/'fullbody-proof' if role=='corvo' else P.parent/'moveset-growth')/f'{role}-{part}.json'
  d=read(globals().get('MODEL_OVERRIDES',{}).get(role+'-'+part,default_model));mesh=bpy.data.meshes.new(role+'_'+part);ix=d['indices'];mesh.from_pydata(d['vertices'],[],[ix[i:i+3]for i in range(0,len(ix),3)]);mesh.update()
  obj=bpy.data.objects.new(mesh.name,mesh);scene.collection.objects.link(obj);obj.parent=rig;obj.modifiers.new('native_weights','ARMATURE').object=rig;groups={j['nativeName']:obj.vertex_groups.new(name=j['nativeName'])for j in J}
  for v,weights in enumerate(d['weights']):
   for name,w in weights:
    if w>0:groups[name].add([v],w,'REPLACE')
  for name in d['materials']:
   mat=bpy.data.materials.new(name);mat.diffuse_color=(.10,.14,.20,1) if part=='body' and 'hand' not in name else (.50,.34,.25,1);mesh.materials.append(mat)
  for k,section in enumerate(d['sections']):
   for poly in mesh.polygons[section['FirstTriangleIndex']//3:section['FirstTriangleIndex']//3+section['TriangleCount']]:poly.material_index=k;poly.use_smooth=True
 d=read(P.parent/'fullbody-proof/sword.json');mesh=bpy.data.meshes.new(role+'_sword');ix=d['indices'];mesh.from_pydata(d['vertices'],[],[ix[i:i+3]for i in range(0,len(ix),3)]);mesh.update();sword=bpy.data.objects.new(mesh.name,mesh);scene.collection.objects.link(sword)
 mat=bpy.data.materials.new(role+'_steel');mat.diffuse_color=(.5,.5,.52,1);mesh.materials.append(mat)
 rigs.append((role,rig,J,B,sword))
camera=bpy.data.objects.new('camera',bpy.data.cameras.new('camera'));scene.collection.objects.link(camera);scene.camera=camera;camera.data.type='ORTHO';camera.data.ortho_scale=3.7
camera.location=(4.7,-5.2,2.8);camera.rotation_euler=(Vector((.1,0,.92))-camera.location).to_track_quat('-Z','Y').to_euler()
for name in ('idle','walk_forward','walk_left','walk_right','walk_back','walk_back_left','walk_back_right'):
 for frame in (30,75):
  for role,rig,J,B,sword in rigs:
   data=read(P/'retargeted'/f'{role}-{name}.json');local=[matrix(v) for v in data['frames'][frame]];g=[]
   for i,j in enumerate(J):
    rig.pose.bones[j['nativeName']].matrix_basis=B[i].inverted()@local[i];p=j['parent'];g.append(local[i] if p<0 else g[p]@local[i])
   q=data['weapon_binding'];binding=Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix().to_4x4()
   sword.matrix_world=Matrix.Translation(rig.location)@g[17]@binding
  bpy.context.view_layer.update();scene.render.filepath=str(OUT/f'{name}-{frame}.png');bpy.ops.render.render(write_still=True)
bpy.ops.wm.save_as_mainfile(filepath=str(P/'native-walk-bank-preview.blend'))
print('PREVIEW_COMPLETE')
