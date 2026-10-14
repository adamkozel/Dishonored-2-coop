"""Offline-only native equipment preview, including skeletal mechanisms."""
import bpy,hashlib
from clip_io import *
from edge_data import poses as edge_poses
class NativePropPreview:
 def __init__(self,name,definition):
  self.definition=definition;self.cache={};self.native=NativeRig(read(P/definition['skeleton'])['joints']);J=self.native.joints
  bpy.ops.object.select_all(action='DESELECT');self.rig=bpy.data.objects.new(name,bpy.data.armatures.new(name+'_skeleton'));bpy.context.scene.collection.objects.link(self.rig);bpy.context.view_layer.objects.active=self.rig;self.rig.select_set(True)
  bpy.ops.object.mode_set(mode='EDIT')
  for i,j in enumerate(J):
   bone=self.rig.data.edit_bones.new(j['nativeName']);bone.length=.015;bone.matrix=self.native.global_bind[i]
   if j['parent']>=0:bone.parent=self.rig.data.edit_bones[J[j['parent']]['nativeName']]
  bpy.ops.object.mode_set(mode='OBJECT');self.rig.select_set(False)
  for b in self.rig.pose.bones:b.rotation_mode='QUATERNION'
  d=read(P/definition['model']);mesh=bpy.data.meshes.new(name+'_mesh');ix=d['indices'];mesh.from_pydata(d['vertices'],[],[ix[i:i+3]for i in range(0,len(ix),3)]);mesh.update();self.mesh=bpy.data.objects.new(mesh.name,mesh);bpy.context.scene.collection.objects.link(self.mesh);self.mesh.parent=self.rig;self.mesh.modifiers.new('native_weights','ARMATURE').object=self.rig
  groups={j['nativeName']:self.mesh.vertex_groups.new(name=j['nativeName'])for j in J}
  for i,weights in enumerate(d['weights']):
   for bone,weight in weights:
    if weight>0:groups[bone].add([i],weight,'REPLACE')
  mat=bpy.data.materials.new(name+'_native_prop');mat.diffuse_color=(.32,.21,.16,1)if definition['kind']=='heart'else(.38,.35,.28,1);mesh.materials.append(mat)
  for poly in mesh.polygons:poly.use_smooth=True
 def hide(self):self.mesh.hide_render=True
 def apply(self,parent,definition,seconds,fps):
  frames=definition['attachment_track'];at=max(0,min(len(frames)-1,seconds*fps));a=int(at);b=min(a+1,len(frames)-1)
  self.rig.matrix_world=parent@blend([qts(frames[a])],[qts(frames[b])],at-a)[0]
  self.mesh.hide_render=not definition['visible'][a]
  mechanism=definition.get('mechanism');phase=seconds/max(.001,(len(frames)-1)/fps)
  if definition.get('mechanism_segments'):
   segments=definition['mechanism_segments'];segment=next((s for s in segments if s['start']<=seconds<s['end']),segments[-1]);mechanism=segment['mechanism'];phase=(seconds-segment['start'])/max(.001,segment['end']-segment['start'])
  if mechanism:
   key=mechanism['sample_sha256']
   if key not in self.cache:
    data=edge_poses(P/mechanism['sample_file']);assert data['sha256']==key and data['joints']==len(self.native.joints)
    self.cache[key]=[[qts(v)for v in f]for f in data['frames']]
   local=sample(self.cache[key],phase)
  else:local=self.native.bind
  for i,j in enumerate(self.native.joints):self.rig.pose.bones[j['nativeName']].matrix_basis=self.native.bind[i].inverted()@local[i]
