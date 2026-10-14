import bpy
from pathlib import Path
p=Path(r'C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/crouch-grip-reference');old=p.parent/'mixamo-crouch'
results={}
for char,count in [('emily',219),('corvo',215)]:
 if not (p/f'source/{char}-posture100.fbx').exists():continue
 scene='D2_'+char.title()+'_Posture100'
 code=(old/'import_donor.py').read_text().replace(str(old).replace('\\','/'),str(p).replace('\\','/'))
 code=code.replace('source/crouched-walking-inplace-60fps.fbx',f'source/{char}-posture100.fbx').replace('crouched-walking-source.blend',f'{char}-posture100.blend').replace('D2_Mixamo_Source',scene)
 code=code.replace('rig = rigs[0]',f'rig = rigs[0]\nrig.name="{scene}_Rig"').replace('source-import.json',f'{char}-posture100-import.json')
 if not bpy.data.scenes.get(scene):exec(compile(code,'import_posture100','exec'),{})
 bpy.context.window.scene=bpy.data.scenes[scene]
 code=(old/'bake_corvo.py').read_text().replace(str(old).replace('\\','/'),str(p).replace('\\','/'))
 code=code.replace("scenes['D2_Mixamo_Source']",f"scenes['{scene}']").replace("objects['Armature']",f"objects['{scene}_Rig']")
 code=code.replace('native-corvo.json',f'native-{char}.json').replace('len(joints)==215',f'len(joints)=={count}').replace("'target_count':215",f"'target_count':{count}")
 code=code.replace('corvo-clip.json',f'{char}-posture100-clip.json').replace('mixamo_corvo_clip.h',f'{char}-posture100-clip.h').replace('kMixamoCorvo','kMixamo'+char.title())
 code=code.replace('crouched-walking-source.blend',f'{char}-posture100.blend').replace('D2_Mixamo_ProofCamera',scene+'_Camera')
 code=code.replace('scene.frame_set(f);bpy.context.view_layer.update()','source_frame=scene.frame_start+(scene.frame_end-scene.frame_start)*(f-1)/62;scene.frame_set(math.floor(source_frame),subframe=source_frame%1);bpy.context.view_layer.update()')
 code=code.replace("'duration_seconds':62/60","'duration_seconds':(scene.frame_end-scene.frame_start)/60,'sampling':'entire donor cycle resampled to 63 normalized phase keys; runtime cycle distance unchanged'")
 namespace={};exec(compile(code,'bake_posture100','exec'),namespace);results[char]=namespace['result']
 results[char]['duration']=(bpy.data.scenes[scene].frame_end-bpy.data.scenes[scene].frame_start)/60
 results[char]['resampled_normalized_keys']=63
result=results
