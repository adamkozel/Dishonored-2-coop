from pathlib import Path
root=Path(r'C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/mixamo-crouch')
code=(root/'import_donor.py').read_text()
code=code.replace('source/crouched-walking-inplace-60fps.fbx','source/crouched-walking-posture35-arm70-inplace-60fps.fbx')
code=code.replace('crouched-walking-source.blend','crouched-walking-corvo-upright.blend')
code=code.replace('D2_Mixamo_Source','D2_Mixamo_Corvo_Upright')
code=code.replace('rig = rigs[0]','rig = rigs[0]\nrig.name="D2_Mixamo_Corvo_Upright_Rig"')
code=code.replace("root/'source-import.json'","root/'corvo-upright-import.json'")
exec(compile(code,str(root/'import_donor.py'),'exec'))
