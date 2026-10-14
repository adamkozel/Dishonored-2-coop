"""Build a native-space skinned authoring rig; no edits to the game or existing scenes."""
import bpy,json,math
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
root=Path(r'C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/fullbody-proof')
native=json.loads((root/'inputs/native-corvo.json').read_text(encoding='utf-8-sig'))
joints=native['joints']
assert 'D2_Proof_Corvo_Rig' not in bpy.data.objects, 'Open the existing authored blend and run author_sequence.py; do not import duplicate rigs.'
s=bpy.data.scenes.new('D2_Corvo_Fullbody_Native');bpy.context.window.scene=s
s.unit_settings.system='METRIC';s.render.fps=60
rig=bpy.data.objects.new('D2_Proof_Corvo_Rig',bpy.data.armatures.new('D2_Proof_Corvo_Skeleton'))
s.collection.objects.link(rig);bpy.context.view_layer.objects.active=rig;rig.select_set(True)
bpy.ops.object.mode_set(mode='EDIT')
bind=[]
for j in joints:
    q=j['bindLocalQTS'];m=Quaternion((q[3],q[0],q[1],q[2])).normalized().to_matrix().to_4x4();m.translation=Vector(q[4:7]);p=j['parent']
    g=m if p<0 else bind[p]@m;bind.append(g)
    b=rig.data.edit_bones.new(j['nativeName']);b.length=.045;b.matrix=g
    if p>=0:b.parent=rig.data.edit_bones[joints[p]['nativeName']]
bpy.ops.object.mode_set(mode='OBJECT')
assert max(abs(rig.data.bones[j['nativeName']].matrix_local[x][y]-g[x][y]) for j,g in zip(joints,bind) for x in range(4) for y in range(4))<1e-5, 'Native rest axes did not survive import'
counts=[]
for part in ('body','head'):
    data=json.loads((root/f'corvo-{part}.json').read_text(encoding='utf-8-sig'))
    mesh=bpy.data.meshes.new(f'D2_Proof_Corvo_{part}');ix=data['indices']
    mesh.from_pydata(data['vertices'],[],[ix[k:k+3] for k in range(0,len(ix),3)]);mesh.update()
    obj=bpy.data.objects.new(mesh.name,mesh);s.collection.objects.link(obj)
    mod=obj.modifiers.new('Native skin weights','ARMATURE');mod.object=rig
    obj.parent=rig
    groups={j['nativeName']:obj.vertex_groups.new(name=j['nativeName']) for j in joints}
    for v,weights in enumerate(data['weights']):
        for name,w in weights:
            assert name in groups,(part,name)
            if w>0:groups[name].add([v],w,'REPLACE')
    for matname in data['materials']:
        mat=bpy.data.materials.new('Proof_'+matname.split('/')[-1]);mat.diffuse_color=(.065,.085,.11,1) if part=='body' else (.48,.32,.23,1)
        if 'hand' in matname:mat.diffuse_color=(.5,.34,.24,1)
        mesh.materials.append(mat)
    for n,section in enumerate(data['sections']):
        a=section['FirstTriangleIndex']//3
        for poly in mesh.polygons[a:a+section['TriangleCount']]:poly.material_index=n;poly.use_smooth=True
    counts.append({'part':part,'vertices':len(mesh.vertices),'triangles':len(mesh.polygons)})
cam=bpy.data.objects.new('D2_Proof_Camera',bpy.data.cameras.new('D2_Proof_Camera'));s.collection.objects.link(cam)
cam.location=(3.7,-4.5,2.2);cam.rotation_euler=(Vector((0,0,1))-cam.location).to_track_quat('-Z','Y').to_euler();cam.data.type='ORTHO';cam.data.ortho_scale=2.5;s.camera=cam
s.render.engine='BLENDER_WORKBENCH';s.display.shading.light='STUDIO';s.display.shading.color_type='MATERIAL';s.display.shading.show_shadows=True;s.display.shading.show_cavity=True;s.display.shading.background_type='WORLD';s.world=bpy.data.worlds.new('D2_Proof_World');s.world.color=(.035,.035,.035)
s.render.resolution_x=1000;s.render.resolution_y=1000;s.render.resolution_percentage=100
s.render.image_settings.file_format='PNG';s.render.filepath=str(root/'native-bind.png')
bpy.ops.wm.save_as_mainfile(filepath=str(root/'corvo-fullbody-proof.blend'))
bpy.ops.render.render(write_still=True)
result={'scene':s.name,'rig':rig.name,'bones':len(rig.data.bones),'meshes':counts,'bind_preview':str(root/'native-bind.png')}
