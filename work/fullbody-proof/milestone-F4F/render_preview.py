"""Render the final proof with its comparison camera; execute in Blender MCP."""
import bpy
from pathlib import Path
from mathutils import Vector
p=Path(__file__).resolve().parent
s=bpy.data.scenes['D2_Corvo_Fullbody_Native'];bpy.context.window.scene=s
s.camera.location=(1.3,4.8,2.6)
s.camera.rotation_euler=(Vector((.2,0,.92))-s.camera.location).to_track_quat('-Z','Y').to_euler()
s.camera.data.ortho_scale=2.22
s.render.resolution_x=640;s.render.resolution_y=640;s.frame_step=4
s.render.filepath=str(p/'preview-frames/frame_');bpy.ops.render.render(animation=True)
s.render.resolution_x=1000;s.render.resolution_y=1000;s.frame_step=1;s.frame_set(185)
s.render.filepath=str(p/'final-authoring-guard.png');bpy.ops.render.render(write_still=True)
bpy.ops.wm.save_as_mainfile(filepath=str(p/'corvo-fullbody-authored.blend'))
result={'file':bpy.data.filepath,'frameStepForVideo':4,'fps':s.render.fps}
