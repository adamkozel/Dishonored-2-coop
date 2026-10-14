"""Native weighted turn preview; root rotation is authoring-only."""
from pathlib import Path
P=Path(__file__).resolve().parent
source=(P/'preview_walk_bank.py').read_text()
exec(source.split("for name in ('idle',")[0])
OUT=P/'turn-previews';OUT.mkdir(exist_ok=True)
camera.data.ortho_scale=4.4
for name in ('turn_left_90','turn_right_90','turn_left_180','turn_right_180'):
 for frame in (0,30,60,90,120):
  for role,rig,J,B,sword in rigs:
   data=read(P/'turns'/f'{role}-{name}.json');local=[matrix(v) for v in data['frames'][frame]];g=[]
   rig.rotation_euler.z=data['root_angles'][frame]
   for i,j in enumerate(J):
    rig.pose.bones[j['nativeName']].matrix_basis=B[i].inverted()@local[i];p=j['parent'];g.append(local[i] if p<0 else g[p]@local[i])
   q=data['weapon_binding'];binding=Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix().to_4x4()
   sword.matrix_world=Matrix.Translation(rig.location)@Matrix.Rotation(rig.rotation_euler.z,4,'Z')@g[17]@binding
  bpy.context.view_layer.update();scene.render.filepath=str(OUT/f'{name}-{frame}.png');bpy.ops.render.render(write_still=True)
bpy.ops.wm.save_as_mainfile(filepath=str(P/'native-turn-preview.blend'))
print('TURN_PREVIEW_COMPLETE')
