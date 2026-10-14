"""Full-body donor retarget. No fixed wrists, first-person pose or limb stretching."""
import bpy,sys,json,math,struct,hashlib,re,argparse
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from rig_math import NativeRig,read,qts,preview_helpers
parser=argparse.ArgumentParser();parser.add_argument('--extra-folder');parser.add_argument('--out',default='mixamo-retargeted-v5');parser.add_argument('--manifest',default='mixamo-retarget-manifest-v5.json')
args=parser.parse_args(sys.argv[sys.argv.index('--')+1:] if '--'in sys.argv else [])
SRC=P/'mixamo-adventure';OUT=P/args.out;OUT.mkdir(exist_ok=True)
mapping={2:'Hips',3:'Spine',4:'Spine1',5:'Spine2',6:'LeftShoulder',7:'LeftArm',8:'LeftForeArm',9:'LeftHand',11:'Neck',12:'Head',13:'RightShoulder',14:'RightArm',15:'RightForeArm',16:'RightHand',19:'LeftUpLeg',20:'LeftLeg',21:'LeftFoot',22:'LeftToeBase',23:'RightUpLeg',24:'RightLeg',25:'RightFoot',26:'RightToeBase'}
source_manifest=read(SRC/'source-manifest.json');library=read(P/'library-retarget-manifest.json')
sources=[dict(path=SRC/r['file'],label='mixamo_'+Path(r['file']).stem.replace(' ','_').replace('(','').replace(')',''),family='stance' if any(s in r['file'] for s in ['crouch','cover']) else 'traversal' if any(s in r['file'] for s in ['fall','jump','land']) else 'locomotion',roles=['corvo','emily']) for r in source_manifest['files'] if r['file']!='X Bot.fbx']
sources.extend(dict(path=P.parent/'crouch-grip-reference/source'/f'{role}-posture100.fbx',label='mixamo_crouched_walking_posture100',family='stance',roles=[role]) for role in ['corvo','emily'])
for folder in ['mixamo-traversal','mixamo-water','mixamo-stance']:
 manifest=P/folder/'source-manifest.json'
 if manifest.exists():
  sources.extend(dict(path=manifest.parent/r['file'],label='mixamo_'+Path(r['file']).stem.lower().replace(' ','_'),family='stance' if folder=='mixamo-stance' else 'traversal',roles=['corvo','emily'],download=r) for r in read(manifest)['files'])
if args.extra_folder:
 manifest=P/args.extra_folder/'source-manifest.json'
 sources=[dict(path=manifest.parent/r['file'],label='mixamo_'+Path(r['file']).stem.lower().replace(' ','_'),family=r['family'],roles=['corvo','emily'],download=r)for r in read(manifest)['files']]
def rigid(m):
 r=m.to_quaternion().normalized().to_matrix().to_4x4();r.translation=m.translation;return r
def row(m):
 q=m.to_quaternion().normalized();return [q.x,q.y,q.z,q.w,*m.translation]
def captured(v):
 m=Matrix.Identity(4)
 for c in range(4):
  for r in range(3):m[r][c]=v[c*4+r]
 return m
def palm_frame(wrist,index,pinky,middle):
 x=(index-pinky).normalized();y=middle-wrist;y=(y-x*y.dot(x)).normalized();return Matrix((x,y,x.cross(y))).transposed()
reports=[]
for source in sources:
 bpy.ops.wm.read_factory_settings(use_empty=True);scene=bpy.context.scene;scene.render.fps=60
 bpy.ops.import_scene.fbx(filepath=str(source['path']),use_anim=True,ignore_leaf_bones=True,anim_offset=0)
 rigs=[o for o in bpy.data.objects if o.type=='ARMATURE'];assert len(rigs)==1;donor=rigs[0]
 names={b.name.split(':')[-1]:b.name for b in donor.data.bones};assert all(n in names for n in mapping.values())
 donor_names=set(mapping.values())|{side+'Hand'+finger+str(k) for side in ['Left','Right'] for finger in ['Thumb','Index','Middle','Ring','Pinky'] for k in [1,2,3] if side+'Hand'+finger+str(k) in names}
 # Blender imports Mixamo facing -Y. Native D2's forward axis is +X.
 C=Matrix.Rotation(math.pi/2,4,'Z');donor_rest={n:rigid(C@donor.matrix_world@donor.data.bones[names[n]].matrix_local)for n in donor_names}
 action=donor.animation_data.action;first,last=action.frame_range;first=int(round(first));last=int(round(last));assert 1<last-first<1800
 donor_frames=[]
 for f in range(first,last+1):
  scene.frame_set(f);bpy.context.view_layer.update();donor_frames.append({n:rigid(C@donor.matrix_world@donor.pose.bones[names[n]].matrix)for n in donor_names})
 for role in source['roles']:
  fps=scene.render.fps/scene.render.fps_base;assert fps in (30,60), ('Unsupported embedded FBX timebase',fps)
  target=NativeRig(read(P.parent/f'mixamo-crouch/native-{role}.json')['joints']);J=target.joints;B=target.bind;BG=target.global_bind;n=len(J)
  # Resolve by NAMES independently for each native rig: Emily's distal
  # fingers have different indices after the shared anatomical prefix.
  current_mapping=dict(mapping)
  for i,j in enumerate(J):
   match=re.fullmatch(r'(left|right)_(thumb|index|middle|ring|pinky)(\d\d)_jnt',j['nativeName'])
   if not match:continue
   side,finger,segment=match.groups();segment=int(segment)-(1 if finger=='pinky' else 0)
   source_name=side.title()+'Hand'+finger.title()+str(segment)
   if 1<=segment<=3 and source_name in donor_names:current_mapping[i]=source_name
  rest={j:donor_rest[name] for j,name in current_mapping.items()}
  sampled=[{j:pose[name] for j,name in current_mapping.items()} for pose in donor_frames]
  scale=(BG[19].translation-BG[21].translation).length/(rest[19].translation-rest[21].translation).length
  corrections={j:Matrix.Identity(3)for j in current_mapping}
  for j,k in {7:8,8:9,14:15,15:16}.items():corrections[j]=(BG[k].translation-BG[j].translation).normalized().rotation_difference((rest[k].translation-rest[j].translation).normalized()).to_matrix()
  lookup={j['nativeName']:i for i,j in enumerate(J)}
  for side,wrist in [('left',9),('right',16)]:
   d=side.title()+'Hand'
   source_palm=palm_frame(donor_rest[d].translation,donor_rest[d+'Index1'].translation,donor_rest[d+'Pinky1'].translation,donor_rest[d+'Middle1'].translation)
   target_palm=palm_frame(BG[wrist].translation,BG[lookup[side+'_index01_jnt']].translation,BG[lookup[side+'_pinky02_jnt']].translation,BG[lookup[side+'_middle01_jnt']].translation)
   calibration=source_palm@target_palm.inverted();corrections[wrist]=calibration
   for i in current_mapping:
    if i>=27 and J[i]['nativeName'].startswith(side+'_'):corrections[i]=calibration
  native_grip=next(m for m in library if m['role']==role and m['selection']['alias']=='sword_idle');raw=(P/native_grip['binary']).read_bytes();grip=[qts(struct.unpack_from('<7f',raw,28+j*28))for j in range(n)]
  names_to_index={j['nativeName']:i for i,j in enumerate(J)};hand=[]
  for i,j in enumerate(J):
   parent=i
   while parent>=0 and parent!=16:parent=J[parent]['parent']
   if parent==16 and i!=16 and 'helper' not in j['nativeName']:hand.append(i)
  armed=source['label']=='mixamo_crouched_walking_posture100'
  if armed:
   # Reuse the coherent native closed grip and its measured handle-centred
   # reversal. Do NOT reuse the old first-person-fixed wrist orientation.
   cap=read(P.parent/f'mixamo-crouch/native-{role}-sword-grip.json')['joints']
   for i in hand:grip[i]=captured(cap[i]['localPose'])
   socket=captured(cap[17]['localPose']);global_socket=captured(cap[17]['globalPose'])
   centre=Vector((sum((global_socket.inverted()@captured(cap[i]['globalPose']).translation).x for i in [40,42,44,47])/4,0,0));flip=Matrix.Rotation(math.pi,3,'Y')
   reverse=(socket.to_3x3()@flip).to_4x4();reverse.translation=socket.translation+socket.to_3x3()@(centre-flip@centre);grip[17]=reverse
  climbing=any(x in source['label'] for x in ('climb','ladder'))
  initial=sampled[0][2].translation;end=sampled[-1][2].translation;rows=[];roots=[]
  # Translation is separated into a linear travelling root plus local hip
  # sway. Jump/landing vertical travel is retained in these review candidates;
  # gameplay airborne root extraction remains an explicit integration task.
  for fi,pose in enumerate(sampled):
   u=fi/(len(sampled)-1);travel=initial.lerp(end,u)
   # Vertical ascent is actor travel, not a 3m elongation of center_root.
   # Preserve authored compression/bob around a linear climb trajectory.
   travel.z=(initial.z+(end.z-initial.z)*u) if climbing else rest[2].translation.z
   local=[m.copy()for m in B];g=[]
   for i,j in enumerate(J):
    parent=j['parent']
    if i in current_mapping:
     desired=pose[i].to_3x3()@rest[i].to_3x3().inverted()@corrections[i]@BG[i].to_3x3();rot=g[parent].to_3x3().inverted()@desired
     local[i]=rot.to_quaternion().normalized().to_matrix().to_4x4();local[i].translation=B[i].translation
     if i==2:
      local[i].translation+=(pose[i].translation-travel)*scale
      if climbing:local[i].translation.z+=(initial.z-rest[2].translation.z)*scale
    if armed and i in hand:local[i]=grip[i].copy()
    g.append(local[i] if parent<0 else g[parent]@local[i])
   # All anatomical translations remain native. No repeated floor clamping:
   # doing that would erase authored jump/landing compression.
   preview_helpers(target,local)
   rows.append([row(m)for m in local]);rt=Matrix.Translation((travel-initial)*scale)
   if not climbing:rt.translation.z=0
   roots.append(row(rt))
  layout='\n'.join(f"{j['nativeName']}:{j['parent']}"for j in J);layout_hash=int.from_bytes(hashlib.sha256(layout.encode()).digest()[:8],'little')
  key=source['label']+'-'+role;payload=bytearray(struct.pack('<8sIIfQ',b'D2NATV01',n,len(rows),fps,layout_hash))
  for f in rows:
   for v in f:payload.extend(struct.pack('<7f',*v))
  path=OUT/(key+'.bin');path.write_bytes(payload)
  meta=dict(id=key,role=role,binary=str(path.relative_to(P)),sha256=hashlib.sha256(payload).hexdigest(),frames=len(rows),fps=fps,duration=(len(rows)-1)/fps,joints=n,layout_hash=layout_hash,root_track=roots,root_track_semantics='Linear donor hip travel separated from body sway; vertical movement retained in review pose pending gameplay-specific extraction',source=dict(asset=str(source['path']),sha256=hashlib.sha256(source['path'].read_bytes()).hexdigest(),aliases=[source['label']],pack='Action Adventure Pack' if source['path'].parent==SRC else 'Crouched Walking / Crouched Walk / Posture 100',actual_fbx_fps=fps,download_ui_fps=60),selection=dict(alias=source['label'],family=source['family'],purpose='Authored full-body donor candidate; contact, equipment and game state binding require visual validation'),first_person_constraints=False,events=False,root_writes=False,visual_status='unreviewed_candidate',runtime_verified=False,source_scale=scale)
  (OUT/(key+'.json')).write_text(json.dumps(meta,indent=2));reports.append(meta)
  meta['equipment_policy']='sword_ready' if armed else 'hands_free'
  meta['equipment_note']='Native closed fingers and sword reversed around the measured handle centre, following the freely moving authored wrist. No fixed first-person wrist orientation.' if armed else 'Authored donor finger motion with palm-frame calibration; weapons must be holstered for this candidate'
  meta['helper_preview']='Approximation; exact native skin correctives require in-game validation'
  meta['finger_mapping']={J[i]['nativeName']:name for i,name in current_mapping.items() if i>=27}
  if climbing:meta['root_track_semantics']='Linear donor hip XYZ ascent separated from body compression; preview only. D2 climbing root must come from replicated player/obstacle state.'
  if 'download' in source:meta['source']['download']=source['download'];meta['source']['pack']='Individual Mixamo motion'
  (OUT/(key+'.json')).write_text(json.dumps(meta,indent=2))
  print('MIXAMO_RETARGET',key,len(rows),'frames',flush=True)
(P/args.manifest).write_text(json.dumps(reports,indent=2));print('MIXAMO_BODY_LIBRARY',len(reports))
