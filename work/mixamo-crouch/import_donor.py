"""Run through the connected Blender MCP; creates a separate scene, no deletions."""
import bpy
import json
from pathlib import Path

root = Path(r"C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/mixamo-crouch")
clip = root / "source/crouched-walking-inplace-60fps.fbx"
destination = root / "crouched-walking-source.blend"
if destination.exists() or bpy.data.scenes.get("D2_Mixamo_Source"):
    raise RuntimeError("Existing experiment; inspect before rerunning import")
if bpy.context.object and bpy.context.object.mode != 'OBJECT':
    raise RuntimeError("Unexpected active edit mode; no scene modification")

bpy.ops.scene.new(type='NEW')
scene = bpy.context.scene
scene.name = "D2_Mixamo_Source"
scene.unit_settings.system = 'METRIC'
scene.unit_settings.scale_length = 1.0
scene.render.fps = 60
scene.render.fps_base = 1.0
before = set(bpy.data.objects)
options = dict(filepath=str(clip), use_manual_orientation=False, global_scale=1.0,
               bake_space_transform=False, use_anim=True, anim_offset=1.0,
               ignore_leaf_bones=False, force_connect_children=False,
               automatic_bone_orientation=False, use_prepost_rot=True,
               use_image_search=False)
bpy.ops.import_scene.fbx(**options)
created = [o for o in bpy.data.objects if o not in before]
rigs = [o for o in created if o.type == 'ARMATURE']
if len(rigs) != 1:
    raise RuntimeError(f"Expected one source rig, got {len(rigs)}")
rig = rigs[0]
action = rig.animation_data.action if rig.animation_data else None
if not action:
    raise RuntimeError("Imported donor has no action")
scene.frame_start = int(action.frame_range[0])
scene.frame_end = int(action.frame_range[1])
scene.frame_set(scene.frame_start)
bpy.context.view_layer.update()
samples = []
for frame in (scene.frame_start, (scene.frame_start+scene.frame_end)//2, scene.frame_end):
    scene.frame_set(frame)
    bpy.context.view_layer.update()
    samples.append({'frame': frame, 'world': [list(row) for row in rig.matrix_world],
                    'hips': list((rig.matrix_world @ rig.pose.bones['mixamorig:Hips'].matrix).translation),
                    'left_foot': list((rig.matrix_world @ rig.pose.bones['mixamorig:LeftFoot'].matrix).translation),
                    'right_foot': list((rig.matrix_world @ rig.pose.bones['mixamorig:RightFoot'].matrix).translation)})
scene.frame_set(scene.frame_start)
bpy.ops.wm.save_as_mainfile(filepath=str(destination), check_existing=True)
result = {'blend': str(destination), 'clip': str(clip), 'import_options': options,
          'scene': scene.name, 'fps': scene.render.fps, 'fps_base': scene.render.fps_base,
          'action': action.name, 'range': list(action.frame_range), 'bones': len(rig.data.bones),
          'bone_names': [b.name for b in rig.data.bones],
          'objects': [{'name': o.name, 'type': o.type, 'dimensions': list(o.dimensions)} for o in created],
          'samples': samples, 'first_person_changed': False, 'game_playback_proven': False}
(root/'source-import.json').write_text(json.dumps(result, indent=2), encoding='utf-8')
