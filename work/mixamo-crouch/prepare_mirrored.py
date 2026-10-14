import bpy, json, math
from pathlib import Path
from mathutils import Matrix, Quaternion, Vector
root=Path(r'C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/mixamo-crouch')
report={}
def rotation(v): return Matrix([[v[c*4+r] for c in range(3)] for r in range(3)])
for character,role,count,stride in [('corvo','host',215,2.04),('emily','client',219,2.05)]:
    scene='D2_Mixamo_'+character.title()+'_Mirrored'
    code=(root/'import_donor.py').read_text()
    code=code.replace('source/crouched-walking-inplace-60fps.fbx',f'source/crouched-walking-{character}-mirrored-inplace-60fps.fbx')
    code=code.replace('crouched-walking-source.blend',f'crouched-walking-{character}-mirrored.blend').replace('D2_Mixamo_Source',scene)
    code=code.replace('rig = rigs[0]',f'rig = rigs[0]\nrig.name="{scene}_Rig"')
    code=code.replace("root/'source-import.json'",f"root/'{character}-mirrored-import.json'")
    if not bpy.data.scenes.get(scene):exec(compile(code,'import_mirrored','exec'),{})
    bpy.context.window.scene=bpy.data.scenes[scene]
    code=(root/'bake_corvo.py').read_text()
    code=code.replace("scenes['D2_Mixamo_Source']",f"scenes['{scene}']").replace("objects['Armature']",f"objects['{scene}_Rig']")
    code=code.replace('native-corvo.json',f'native-{character}.json').replace('len(joints)==215',f'len(joints)=={count}').replace("'target_count':215",f"'target_count':{count}")
    code=code.replace('corvo-clip.json',f'{character}-mirrored-clip.json').replace('mixamo_corvo_clip.h',f'mixamo_{character}_mirrored_clip.h')
    code=code.replace('kMixamoCorvo','kMixamo'+character.title()).replace('crouched-walking-source.blend',f'crouched-walking-{character}-mirrored.blend')
    code=code.replace('D2_Mixamo_ProofCamera',scene+'_Camera')
    exec(compile(code,'bake_mirrored','exec'),{})
    grip=json.loads((root/f'native-{character}-sword-grip.json').read_text(encoding='utf-8-sig'))
    stand=json.loads((root/f'{role}-fp-standing.json').read_text(encoding='utf-8-sig'))
    crouch=json.loads((root/f'{role}-fp-crouch.json').read_text(encoding='utf-8-sig'))
    assert grip['finalPublicationComplete'] and len(grip['joints'])==count and stand['stable'] and crouch['stable']
    # Preserve the NPC hand's authored grip. Transfer only the native sword
    # flip in wrist space; no first-person shoulder/elbow or camera pose.
    # Hand space must not include the unrelated FP/NPC shoulder rest pose.
    # Derive anatomical palm axes from direct wrist children: thumb and pinky.
    def bindrot(b):
        q=b['bindLocalQTS'];return Quaternion((q[3],q[0],q[1],q[2])).to_matrix()
    def globalbind(joints,index):
        b=joints[index];return bindrot(b) if b['parent']<0 else globalbind(joints,b['parent'])@bindrot(b)
    def palm(joints,thumb,pinky,wrist):
        assert joints[thumb]['parent']==wrist and joints[pinky]['parent']==wrist
        t=Vector(joints[thumb]['bindLocalQTS'][4:7]);k=Vector(joints[pinky]['bindLocalQTS'][4:7])
        x=(k-t).normalized();y=(t+k)*.5;y=(y-x*y.dot(x)).normalized();z=x.cross(y)
        return Matrix((x,y,z)).transposed()
    basis=palm(grip['joints'],38,46,16)@palm(stand['joints'],5,48,4).inverted()
    delta=rotation(crouch['joints'][59]['localPose'])@rotation(stand['joints'][59]['localPose']).inverted()
    flip=basis@delta@basis.inverted()
    shift=basis@(Vector(crouch['joints'][59]['localPose'][12:15])-Vector(stand['joints'][59]['localPose'][12:15]))
    attachment_position=Vector(grip['joints'][17]['localPose'][12:15])+shift
    indices=[16,17,*range(38,49),*range(63 if count==215 else 60,73 if count==215 else 70)]
    prefix='kMixamoGrip' if character=='corvo' else 'kMixamoEmilyGrip'
    lines=['#pragma once','// Native NPC sword_idle grip plus observed first-person crouch attachment flip.',f'constexpr unsigned {prefix}Joints[]={{'+','.join(map(str,indices))+'};',f'constexpr float {prefix}Rotation[][4]={{']
    for index in indices:
        r=rotation(grip['joints'][index]['localPose'])
        if index==17:r=flip@r
        q=r.to_quaternion().normalized();lines.append('{'+','.join(f'{v:.9f}f' for v in [q.x,q.y,q.z,q.w])+'},')
    lines+=['};',f'constexpr float kMixamo{character.title()}AttachmentPosition[3]={{'+','.join(f'{v:.9f}f' for v in attachment_position)+'};',f'constexpr float kMixamo{character.title()}Stride={stride:.9f}f;']
    (root/f'mixamo_{character}_mirrored_grip.h').write_text('\n'.join(lines)+'\n')
    report[character]={'cycle_distance_m':stride,'native_attachment_flip_deg':math.degrees(delta.to_quaternion().angle),'native_attachment_shift_m':list(shift),'attachment_target_m':list(attachment_position),'wrist_bind_conversion':list(map(list,basis)),'source_standing':f'{role}-fp-standing.json','source_crouch':f'{role}-fp-crouch.json','first_person_modified':False,'style_rate_multiplier_approx':2/3}
(root/'mirrored-conversion.json').write_text(json.dumps(report,indent=2))
result=report
