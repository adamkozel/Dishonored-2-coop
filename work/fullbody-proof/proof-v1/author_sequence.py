"""Corvo: one offline authored step / sword guard / recovery proof.
Native skeleton and weights; D2 first-person wrist/weapon reference; no Mixamo.
Root travel is metadata only. Exported local poses never animate origin/anchor.
Run in Blender's existing MCP session.
"""
import bpy,json,math,struct,hashlib,re,bisect
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
P=Path(r'C:/Users/adako/Documents/Codex/2026-10-08/i-x20/work/fullbody-proof')
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def mat(a):return Matrix([[a[c*4+r] if r<3 else float(c==3) for c in range(4)]for r in range(4)])
def qts(q):
 m=Quaternion((q[3],q[0],q[1],q[2])).normalized().to_matrix().to_4x4();m.translation=Vector(q[4:7]);return m
def root(a):
 m=Matrix.Identity(4);m.translation=Vector(a[:3])
 for c in range(3):
  for r in range(3):m[r][c]=a[3+c*3+r]
 return m
def fk(l):
 g=[]
 for i,j in enumerate(J):g.append(l[i] if j['parent']<0 else g[j['parent']]@l[i])
 return g
def ease(t):t=max(0,min(1,t));return t*t*(3-2*t)
def lerp(a,b,t):return a+(b-a)*t
def nearest(rows,tick):return min(rows,key=lambda a:abs(a['tick']-tick))
def palm(js,a,b):
 a=Vector(js[a]['bindLocalQTS'][4:7]);b=Vector(js[b]['bindLocalQTS'][4:7]);x=(b-a).normalized();y=(a+b)*.5;y=(y-x*y.dot(x)).normalized();return Matrix((x,y,x.cross(y))).transposed()
J=read(P.parent/'mixamo-crouch/native-corvo.json')['joints'];B=[qts(j['bindLocalQTS'])for j in J];BG=fk(B)
G=read(P.parent/'mixamo-crouch/native-corvo-sword-grip.json')['joints']
armJ=read(P.parent/'crouch-grip-reference/host-arm.json')['joints']
take=P/'clean-take-2';arms=read(take/'fp-arm-reference.json')['samples'];bodies=read(take/'fp-body-reference.json')['samples'];events=read(take/'reference-events.json');guardTick=next(x['tick']for x in events if x.get('button')=='RightShoulder')
bindingData=read(P.parent/'crouch-grip-reference/client-target-sword.json');q=bindingData['quaternion'];weaponBinding=Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix()
fpSword=read(P.parent/'crouch-grip-reference/host-sword.json');q=fpSword['attachment'][3:7];fpBinding=Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix()
A=palm(J,38,46)@palm(armJ,5,48).inverted()
def fp(tick):
 a=nearest(arms,tick);b=nearest(bodies,a['tick']);al=[mat(v)for v in a['local']];ag=[]
 for i,j in enumerate(armJ):ag.append(al[i] if j['parent']<0 else ag[j['parent']]@al[i])
 conv=root(b['root']).inverted()@root(a['root']);w=conv@ag[4];s=conv@ag[59]
 return w.translation,w.to_3x3()@A.inverted(),s.to_3x3()@fpBinding
refs=[fp(guardTick+x)for x in [-500,0,180,400,750,1050,1450,1900]]
fpTimes=[a['tick']for a in arms];fpValues=[fp(t)for t in fpTimes]
def fpSmooth(t):
 i=max(0,min(len(fpTimes)-2,bisect.bisect_right(fpTimes,t)-1));u=max(0,min(1,(t-fpTimes[i])/(fpTimes[i+1]-fpTimes[i])));a,b=fpValues[i:i+2]
 return a[0].lerp(b[0],u),a[1].to_quaternion().slerp(b[1].to_quaternion(),u).to_matrix(),a[2].to_quaternion().slerp(b[2].to_quaternion(),u).to_matrix()
gripText=(P.parent.parent/'outputs/d2-coop-lab/src/mixamo_corvo_grip.h').read_text()
def floats(name):return [float(x)for x in re.search(name+r'[^=]*=\{(.*?)\};',gripText,re.S).group(1).replace('{','').replace('}','').replace('f','').split(',')if x.strip()]
fingerIds=[int(x)for x in floats('kMixamoGripJoints')];fingerQ=floats('kMixamoGripRotation');socketQ=floats('kMixamoCorvoStandingSocket');socketPos=floats('kMixamoCorvoStandingSocketPosition');socketRot=Quaternion((socketQ[3],*socketQ[:3])).to_matrix()
rig=bpy.data.objects['D2_Proof_Corvo_Rig'];scene=bpy.data.scenes['D2_Corvo_Fullbody_Native'];bpy.context.window.scene=scene
assert max(abs(rig.data.bones[j['nativeName']].matrix_local[x][y]-g[x][y])for j,g in zip(J,BG)for x in range(4)for y in range(4))<1e-5,'Importer rest axes mismatch'
rig.animation_data_clear()
rig.location=(0,0,0)
for b in rig.pose.bones:b.rotation_mode='QUATERNION';b.matrix_basis=Matrix.Identity(4)
# The low LOD native body includes the coat panels absent from the high-detail
# nocloth stream. Both remain in the file; use the complete body for silhouette.
if 'D2_Proof_Corvo_complete' not in bpy.data.objects:
 d=read(P/'corvo-body-complete.json');me=bpy.data.meshes.new('D2_Proof_Complete');ix=d['indices'];me.from_pydata(d['vertices'],[],[ix[i:i+3]for i in range(0,len(ix),3)]);me.update();o=bpy.data.objects.new('D2_Proof_Corvo_complete',me);scene.collection.objects.link(o);o.parent=rig;mod=o.modifiers.new('Native skin','ARMATURE');mod.object=rig
 groups={j['nativeName']:o.vertex_groups.new(name=j['nativeName'])for j in J}
 for i,ws in enumerate(d['weights']):
  for name,w in ws:
   if w:groups[name].add([i],w,'REPLACE')
 for name in d['materials']:
  m=bpy.data.materials.new('ProofComplete_'+name.split('/')[-1]);m.diffuse_color=(.055,.075,.10,1) if 'hand' not in name else (.48,.32,.23,1);me.materials.append(m)
 for k,sec in enumerate(d['sections']):
  for poly in me.polygons[sec['FirstTriangleIndex']//3:sec['FirstTriangleIndex']//3+sec['TriangleCount']]:poly.material_index=k;poly.use_smooth=True
bpy.data.objects['D2_Proof_Corvo_body'].hide_render=False
bpy.data.objects['D2_Proof_Corvo_complete'].hide_render=True
if 'D2_Proof_Sword' not in bpy.data.objects:
 d=read(P/'sword.json');me=bpy.data.meshes.new('D2_Proof_SwordMesh');ix=d['indices'];me.from_pydata(d['vertices'],[],[ix[i:i+3]for i in range(0,len(ix),3)]);me.update();sword=bpy.data.objects.new('D2_Proof_Sword',me);scene.collection.objects.link(sword);m=bpy.data.materials.new('Proof_Steel');m.diffuse_color=(.35,.37,.4,1);me.materials.append(m)
sword=bpy.data.objects['D2_Proof_Sword'];sword.animation_data_clear();sword.rotation_mode='QUATERNION'
if 'D2_Proof_Floor' not in bpy.data.objects:
 me=bpy.data.meshes.new('D2_Proof_Floor');me.from_pydata([(-3,-3,-.004),(3,-3,-.004),(3,3,-.004),(-3,3,-.004)],[],[(0,1,2,3)]);me.update();o=bpy.data.objects.new('D2_Proof_Floor',me);scene.collection.objects.link(o);m=bpy.data.materials.new('Proof_Floor');m.diffuse_color=(.17,.18,.19,1);me.materials.append(m)
# Analytic two-bone IK preserves native lengths, with an explicit knee/elbow pole.
def solve(l,a,b,c,goal,pole):
 g=fk(l);p=g[a].translation;L1=B[b].translation.length;L2=B[c].translation.length;v=goal-p;dist=max(.0001,min(v.length,L1+L2-.0005));axis=v.normalized();side=(pole-p)-axis*(pole-p).dot(axis);side.normalize();along=(dist*dist+L1*L1-L2*L2)/(2*dist);h=math.sqrt(max(0,L1*L1-along*along));middle=p+axis*along+side*h
 for i,child,dst in [(a,b,middle),(b,c,goal)]:
  g=fk(l);rot=(g[child].translation-g[i].translation).rotation_difference(dst-g[i].translation).to_matrix()@g[i].to_3x3();parent=J[i]['parent'];m=(g[parent].to_3x3().inverted()@rot).to_4x4();m.translation=l[i].translation;l[i]=m
def orient(l,i,rot):
 g=fk(l);m=(g[J[i]['parent']].to_3x3().inverted()@rot).to_4x4();m.translation=l[i].translation;l[i]=m
def helpers(l):
 # Authoring preview approximation for known deformation helpers. The game
 # recomputes its exact native correctives after playback; compare round trip.
 g=fk(l)
 for i,j in enumerate(J):
  n=j['nativeName'];driver=None
  if 'helper' not in n:continue
  side='left' if n.startswith('left')else 'right' if n.startswith('right')else ''
  if not side:continue
  if 'upper_leg' in n or 'thigh' in n:driver=side+'_femur_jnt'
  elif 'knee' in n:driver=side+'_tibia_jnt'
  elif 'arm01' in n and 'forearm' not in n:driver=side+'_arm_jnt'
  elif 'elbow' in n:driver=side+'_elbow_jnt'
  elif 'forearm' in n:driver=side+'_elbow_jnt'
  elif 'wrist' in n:driver=side+'_wrist_jnt'
  elif 'ankle' in n:driver=side+'_ankle_jnt'
  if driver:
   k=next((x for x,z in enumerate(J)if z['nativeName']==driver),None)
   if k is not None:
    wanted=g[k]@BG[k].inverted()@BG[i];l[i]=g[j['parent']].inverted()@wanted;g=fk(l)
frames=[];errors=[];fps=60;duration=6.;rootTrack=[]
for f in range(int(duration*fps)+1):
 t=f/fps;l=[m.copy()for m in B];advance=.40*ease((t-.8)/1.5);guard=ease((t-2.4)/.35)*(1-ease((t-4.1)/.65));settle=math.sin(math.pi*ease((t-.7)/1.8))
 l[2].translation+=Vector((0,-.008*math.sin(t*2),-.055-.035*guard-.018*settle))
 for i,deg in [(2,-3),(3,3),(4,4),(5,5)]:
  old=l[i].translation.copy();l[i]=l[i]@Matrix.Rotation(math.radians(deg)*guard,4,'Y');l[i].translation=old
 # A complete advancing step: right foot, then left; each contact is fixed
 # in world space during stance and root motion is removed from local poses.
 footGoals=[]
 for a,b,c,start,end in [(23,24,25,.8,1.55),(19,20,21,1.6,2.3)]:
  u=max(0,min(1,(t-start)/(end-start)));goal=BG[c].translation.copy();goal.x+=.40*ease(u)-advance;goal.z+=.065*math.sin(math.pi*u);footGoals.append(goal.copy());solve(l,a,b,c,goal,Vector((.8,goal.y,.5)));orient(l,c,BG[c].to_3x3())
 # Counter-rotation and weight shift involve the torso, shoulders, neck,
 # pelvis and both legs, not a detached arm layer.
 # The captured FP guard supplies actual wrist / blade orientation and timing.
 sampleTick=guardTick-500 if t<2.4 else guardTick+min(1900,(t-2.4)*1000)
 pos,wr,blade=fpSmooth(sampleTick)
 wr=blade@(socketRot@weaponBinding).inverted()
 pos=Vector((max(.20,min(.44,pos.x)),max(-.36,min(.12,pos.y*.8)),max(1.13,min(1.5,pos.z-.05))))
 solve(l,14,15,16,pos,Vector((.02,-.64,1.20)));orient(l,16,wr)
 # Native closed finger pose and one authored rigid socket relationship.
 for k,i in enumerate(fingerIds):
  if i==17:continue
  q=fingerQ[k*4:k*4+4];l[i]=Quaternion((q[3],*q[:3])).to_matrix().to_4x4();l[i].translation=B[i].translation
 l[17]=socketRot.to_4x4();l[17].translation=Vector(socketPos)
 left=Vector((.15+.12*guard,.30,1.08+.23*guard));solve(l,7,8,9,left,Vector((-.1,.62,1.2)))
 helpers(l);g=fk(l);errors.append(max((g[c].translation-goal).length for c,goal in zip([25,21],footGoals)))
 scene.frame_set(f+1)
 # Object motion visualizes the separate network root in Blender only.
 rig.location=(advance,0,0);rig.keyframe_insert('location')
 for i,j in enumerate(J):
  b=rig.data.bones[j['nativeName']];rest=b.matrix_local if b.parent is None else b.parent.matrix_local.inverted()@b.matrix_local;pb=rig.pose.bones[b.name];pb.matrix_basis=rest.inverted()@l[i];pb.keyframe_insert('location');pb.keyframe_insert('rotation_quaternion')
 sw=g[17]@weaponBinding.to_4x4();sw.translation.x+=advance;sword.matrix_world=sw;sword.keyframe_insert('location');sword.keyframe_insert('rotation_quaternion')
 row=[]
 for m in l:
  q=m.to_quaternion().normalized();row.append([q.x,q.y,q.z,q.w,*m.translation])
 frames.append(row);rootTrack.append(advance)
scene.frame_start=1;scene.frame_end=len(frames);scene.render.fps=fps
scene.camera.location=(3.6,-4.8,2.2);scene.camera.rotation_euler=(Vector((0,0,.95))-scene.camera.location).to_track_quat('-Z','Y').to_euler();scene.camera.data.ortho_scale=2.2
scene.render.resolution_x=1000;scene.render.resolution_y=1000
for f,label in [(1,'ready'),(76,'step'),(140,'settled'),(185,'guard'),(330,'recovery')]:
 scene.frame_set(f);bpy.context.view_layer.update();scene.render.filepath=str(P/f'authored-{label}.png');bpy.ops.render.render(write_still=True)
# Bone layout hash also checked by the runtime loader. Exact little-endian format.
layout='\n'.join(f"{j['nativeName']}:{j['parent']}" for j in J);layoutHash=int.from_bytes(hashlib.sha256(layout.encode()).digest()[:8],'little')
data=struct.pack('<8sIIIIQ',b'D2PROOF1',1,215,len(frames),fps,layoutHash)
for row in frames:
 for v in row:data+=struct.pack('<7f',*v)
(P/'corvo-guard-proof.bin').write_bytes(data)
meta={'version':1,'source':'Blender authored full-body step/guard; D2 FP wrist and sword orientation reference','seconds':duration,'fps':fps,'frames':len(frames),'joints':215,'layoutHash':layoutHash,'sha256':hashlib.sha256(data).hexdigest(),'rootTrack':rootTrack,'maxFootTargetErrorM':max(errors),'helperPreview':'approximate; native game correctives required for final validation','accepted':False,'firstPersonChanged':False}
(P/'authored-sequence.json').write_text(json.dumps(meta,indent=2));scene.frame_set(185);bpy.ops.wm.save_as_mainfile(filepath=str(P/'corvo-fullbody-authored.blend'));result=meta.copy();result.pop('rootTrack')
