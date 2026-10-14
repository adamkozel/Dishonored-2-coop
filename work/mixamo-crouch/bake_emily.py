import bpy
from pathlib import Path
root=Path(r'C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/mixamo-crouch')
bpy.context.window.scene=bpy.data.scenes['D2_Mixamo_Source']
code=(root/'bake_corvo.py').read_text()
code=code.replace('native-corvo.json','native-emily.json').replace('len(joints)==215','len(joints)==219')
code=code.replace("'target_count':215","'target_count':219").replace('Corvo custom Doppelganger','Emily custom Doppelganger')
code=code.replace('corvo-clip.json','emily-clip.json').replace('mixamo_corvo_clip.h','mixamo_emily_clip.h')
code=code.replace('kMixamoCorvo','kMixamoEmily').replace('crouched-walking-source.blend','crouched-walking-emily.blend')
exec(compile(code,str(root/'bake_corvo.py'),'exec'))
code=(root/'calibrate_clips.py').read_text()
code=code.replace('native-corvo','native-emily').replace("('corvo-clip','corvo-upright-clip')","('emily-clip',)")
code=code.replace('corvo-upright-clip','emily-clip').replace('stride-calibration.json','emily-stride-calibration.json')
code=code.replace("len(grip['joints'])==215","len(grip['joints'])==219")
code=code.replace('indices=[16,17,*range(38,49),*range(63,73)]','indices=[16,17,*range(38,49),*range(60,70)]')
code=code.replace('kMixamoGrip','kMixamoEmilyGrip').replace('kMixamoCorvoStride','kMixamoEmilyStride')
code=code.replace('mixamo_corvo_grip.h','mixamo_emily_grip.h').replace('Native Corvo','Native Emily')
exec(compile(code,str(root/'calibrate_clips.py'),'exec'))
