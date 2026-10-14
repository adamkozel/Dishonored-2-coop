"""Offline skin correction, separate from animation and native runtime helpers.

Corvo's helper_ankle bones are calf deformation joints parented to the tibia,
not foot sockets. Native final-publication captures retain their bind-local
translation and apply a small axial twist. The old preview moved these joints
around the ankle pivot, pulling the trouser cuffs away from the boot shafts.

Use the shin-relative bind transform here. This is a conservative preview:
the small native axial twist is not reconstructed. D2 continues to evaluate
its exact model correctives. Mesh weights and anatomical joints are untouched.
"""
VERSION='corvo-shin-anchors-v1'
def correct_calf_helpers(joints,bind,local):
 if len(joints)!=215:return
 for i,j in enumerate(joints):
  if j['nativeName'] in ('left_helper_ankle_jnt','right_helper_ankle_jnt'):
   expected=j['nativeName'].split('_')[0]+'_tibia_jnt'
   assert joints[j['parent']]['nativeName']==expected
   local[i]=bind[i].copy()
