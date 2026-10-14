"""Record both protagonist roles against clearly marked grey victim stand-ins."""
import bpy,sys,json,struct,math,subprocess,hashlib,time,argparse
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
parser=argparse.ArgumentParser();parser.add_argument('--manifest',default='paired-native-manifest-v1.json');parser.add_argument('--folder',default='paired-native-videos-v1');parser.add_argument('--only',nargs='*');args=parser.parse_args(sys.argv[sys.argv.index('--')+1:]if'--'in sys.argv else[])
MODEL_OVERRIDES={'emily-body':P/'item-models/emily-body-high.json'}
exec(compile((P/'preview_natural_walk.py').read_text().split("for name in ('idle'")[0],str(P/'preview_natural_walk.py'),'exec'),globals())
from render_identity import fingerprint,digest
from preview_skin import correct_calf_helpers,VERSION as SKIN_VERSION
from native_prop_preview import NativePropPreview
OUT=P/args.folder;OUT.mkdir(exist_ok=True)
scene.render.resolution_x=1440;scene.render.resolution_y=810;scene.render.fps=30
camera.data.ortho_scale=6.3
groups={}
for m in read(P/args.manifest):groups.setdefault(m['selection']['alias'],{})[m['role']]=m
# Duplicate the existing native rigs without touching preserved Blender files.
victims={};grey=bpy.data.materials.new('neutral victim stand-in');grey.diffuse_color=(.36,.36,.36,1)
for role,rig,J,B,sword in rigs:
 clone=rig.copy();clone.data=rig.data.copy();clone.name='victim_'+role;scene.collection.objects.link(clone)
 for obj in list(rig.children):
  copy=obj.copy();copy.data=obj.data.copy();scene.collection.objects.link(copy);copy.parent=clone
  for mod in copy.modifiers:
   if mod.type=='ARMATURE':mod.object=clone
  copy.data.materials.clear();copy.data.materials.append(grey)
  for poly in copy.data.polygons:poly.material_index=0
 victims[role]=(clone,J,B)
def hide_rig(rig,hidden):
 rig.hide_render=hidden
 for child in rig.children:child.hide_render=hidden
def load(m):
 b=(P/m['binary']).read_bytes();assert hashlib.sha256(b).hexdigest()==m['sha256'];n=struct.unpack_from('<I',b,8)[0]
 return [[matrix(struct.unpack_from('<7f',b,28+(f*n+j)*28))for j in range(n)]for f in range(m['frames'])]
from clip_io import sample as interp
def sample(m,frames,t):
 u=min(1,t/max(m['duration'],1e-6));p=interp(frames,u);root=interp([[matrix(r)]for r in m['root_track']],u)[0]
 return p,matrix(m['root_track'][0]).inverted()@root
def pose(rig,J,B,local):
 correct_calf_helpers(J,B,local)
 g=[]
 for i,j in enumerate(J):
  rig.pose.bones[j['nativeName']].matrix_basis=B[i].inverted()@local[i];parent=j['parent'];g.append(local[i]if parent<0 else g[parent]@local[i])
 return g
floor=bpy.data.materials.new('floor');floor.diffuse_color=(.18,.19,.20,1)
bpy.ops.mesh.primitive_plane_add(size=200,location=(0,0,-.025));bpy.context.object.data.materials.append(floor)
scene.display.render_aa='FXAA'
records=[];props={}
for alias,pair in groups.items():
 if args.only and alias not in args.only:continue
 out=OUT/(alias+'.mp4');meta=OUT/(alias+'.json');assert not out.exists(),'Use a new output folder for a changed pair'
 identity=hashlib.sha256(''.join(pair[r]['sha256']for r in ['corvo','emily']).encode()).hexdigest();provenance=fingerprint(pair,MODEL_OVERRIDES)
 for prop in props.values():prop.hide()
 for role,m in pair.items():
  for definition in m.get('props',[]):
   key=role+'-'+definition['kind']
   if key not in props:props[key]=NativePropPreview(key,definition)
 provenance['paired_inputs']={r:m['partner']for r,m in pair.items()};provenance['paired_renderer_sha256']=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
 af={r:load(m)for r,m in pair.items()};vf={r:load(m['partner'])for r,m in pair.items()}
 duration=max(m['duration']for m in pair.values());total=round(duration*30)+1
 view=Vector(pair['corvo'].get('preview_view',(5.5,-7,4.0)));forward=-view.normalized();right=forward.cross(Vector((0,0,1))).normalized();up=right.cross(forward).normalized();projected=[]
 # Fit both complete bodies, including elevated aerial entry. Keep the
 # camera between actor and target rather than following only actor root.
 for f in range(total):
  bodies=[]
  for role,rig,J,B,sword in rigs:
   m=pair[role];offset=Matrix.Translation((0,-1.2 if role=='corvo'else 1.2,0));local,root=sample(m,af[role],f/30);placement=matrix(m['preview_placement'])if 'preview_placement'in m else Matrix.Identity(4);bodies.append((J,local,offset@placement@root))
   _,VJ,_=victims[m['partner']['role']];local,vroot=sample(m['partner'],vf[role],f/30);bodies.append((VJ,local,offset@matrix(m['partner']['placement'])@vroot))
  center=sum((world.translation for _,_,world in bodies),Vector())/4
  for J,local,world in bodies:
   g=[]
   for i in range(27):
    parent=J[i]['parent'];g.append(local[i]if parent<0 else g[parent]@local[i])
    if i in [2,5,9,12,16,21,25]:
     point=world@g[i].translation-center;projected.append((point.dot(right),point.dot(up)))
 minx=min(x for x,y in projected);maxx=max(x for x,y in projected);miny=min(y for x,y in projected);maxy=max(y for x,y in projected)
 camera.data.ortho_scale=max(6.3,maxx-minx+.9,(maxy-miny+.9)*scene.render.resolution_x/scene.render.resolution_y);target=right*((minx+maxx)*.5)+up*((miny+maxy)*.5)
 # Both protagonists can use the same victim skeleton: create a second
 # independent instance for the right pair when that is the case.
 active=[]
 for r in ['corvo','emily']:
  source=victims[pair[r]['partner']['role']];rig,J,B=source
  instance=rig.copy();instance.data=rig.data.copy();scene.collection.objects.link(instance)
  for obj in list(rig.children):
   child=obj.copy();child.data=obj.data;child.parent=instance;scene.collection.objects.link(child)
   for mod in child.modifiers:
    if mod.type=='ARMATURE':mod.object=instance
  hide_rig(instance,False);active.append((instance,J,B))
 for v,_,_ in victims.values():hide_rig(v,True)
 caption=alias+'  |  '+('CORVO / EMILY'if view.x>=0 else'EMILY / CORVO');note=pair['corvo'].get('preview_caption','GREY TARGET = VICTIM STAND-IN - native timing - contact review pending');filter_="drawtext=fontfile='C\\:/Windows/Fonts/arial.ttf':text='"+caption+"':fontcolor=white:fontsize=22:x=20:y=20,drawtext=fontfile='C\\:/Windows/Fonts/arial.ttf':text='"+note+"':fontcolor=white:fontsize=17:x=20:y=h-28"
 temp=OUT/(alias+'.partial.mp4')
 log=(OUT/(alias+'.encode.log')).open('wb');process=subprocess.Popen(['ffmpeg','-hide_banner','-loglevel','error','-y','-f','image2pipe','-vcodec','png','-framerate','30','-i','pipe:0','-vf',filter_,'-c:v','libx264','-preset','fast','-crf','20','-pix_fmt','yuv420p','-movflags','+faststart',str(temp)],stdin=subprocess.PIPE,stderr=log)
 try:
  for f in range(total):
   center=Vector((0,0,0))
   for index,(role,rig,J,B,sword)in enumerate(rigs):
    m=pair[role];offset=Matrix.Translation((0,-1.2 if role=='corvo'else 1.2,0));local,root=sample(m,af[role],f/30);placement=matrix(m['preview_placement'])if 'preview_placement'in m else Matrix.Identity(4);rig.matrix_world=offset@placement@root;g=pose(rig,J,B,local)
    q=read(P.parent/'fullbody-proof/inputs/client-target-sword.json'if role=='corvo'else P.parent/'crouch-grip-reference/host-target-sword.json')['quaternion'];sword.matrix_world=rig.matrix_world@g[17]@Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix().to_4x4()
    sword.hide_render=m.get('equipment_policy')=='hands_free'
    for definition in m.get('props',[]):props[role+'-'+definition['kind']].apply(rig.matrix_world@g[definition['parent_joint']],definition,f/30,m['fps'])
    v,VJ,VB=active[index];local,vroot=sample(m['partner'],vf[role],f/30);v.matrix_world=offset@matrix(m['partner']['placement'])@vroot;pose(v,VJ,VB,local)
    center+=(rig.matrix_world.translation+v.matrix_world.translation)*.25
   focus=target+center;camera.location=focus+view;camera.rotation_euler=(focus-camera.location).to_track_quat('-Z','Y').to_euler()
   bpy.context.view_layer.update();bpy.ops.render.render();tmp=OUT/'frame.png';bpy.data.images['Render Result'].save_render(str(tmp),scene=scene);process.stdin.write(tmp.read_bytes())
  process.stdin.close();assert process.wait(timeout=60)==0
  temp.replace(out)
 finally:
  log.close()
  if process.poll()is None:process.kill();process.wait()
 for rig,J,B in active:
  for child in list(rig.children):bpy.data.objects.remove(child,do_unlink=True)
  bpy.data.objects.remove(rig,do_unlink=True)
 result=dict(alias=alias,identity=identity,video=str(out.relative_to(P)),frames=total,fps=30,duration=total/30,source='Blender native paired choreography preview',status='paired_candidate_recorded',visual_accepted=False,cloth='Emily high-detail body omits simulated coat panels',victim='Neutral-grey stand-in; this is not co-op friendly fire',**provenance)
 result['skin_preview_version']=SKIN_VERSION
 result['skin_preview_limit']='Corvo calf anchors corrected for protagonists and grey stand-ins; native helper twist remains approximate.'
 result['render_quality']='Workbench FXAA, full-resolution 30 fps';result['renderer_script']='render_paired_fast.py';result['renderer_script_sha256']=hashlib.sha256(Path(__file__).read_bytes()).hexdigest();result['render_identity']=hashlib.sha256((result['render_identity']+result['renderer_script_sha256']+'FXAA').encode()).hexdigest()
 meta.write_text(json.dumps(result,indent=2));records.append(result);print('PAIRED_RECORDED',alias,total,flush=True)
(OUT/'manifest.json').write_text(json.dumps(records,indent=2))
