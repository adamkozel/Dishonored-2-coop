"""Match the evaluated component transform to ControlRoot during offline capture.

AnimNode_AnimGenController.cpp uses GetComponentTransform in FromPoseVectors.
Updating only ControlRoot fixes requested heading but leaves decoder feedback
in a different frame. Neither experiment changes D2 or installed UE assets.
"""
from pathlib import Path
P=Path(__file__).resolve().parent
source=(P/'probe_root_feedback.py').read_text()
source=source.replace("OUT=P/'donors-feedback'", "OUT=P/'donors-component-feedback'")
source=source.replace("'donor-manifest-feedback.json'", "'donor-manifest-component-feedback.json'")
needle="instance.set_editor_property('control_root',unreal.Transform(location=unreal.Vector(*root_pos),rotation=unreal.Rotator(pitch=0,yaw=root_yaw,roll=0)))"
replacement="root_transform=unreal.Transform(location=unreal.Vector(*root_pos),rotation=unreal.Rotator(pitch=0,yaw=root_yaw,roll=0))\\n            actor.set_actor_transform(root_transform,False,True)\\n            instance.set_editor_property('control_root',root_transform)"
# The parent script contains this code inside its multiline replacement string.
source=source.replace(needle,replacement)
assert needle not in source and 'set_actor_transform' in source
exec(compile(source,str(P/'probe_root_feedback.py'),'exec'),{'__file__':str(P/'probe_root_feedback.py'),'__name__':'__main__'})
