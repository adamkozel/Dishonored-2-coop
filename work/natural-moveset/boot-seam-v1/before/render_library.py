"""Recorded Blender review of authored native-rig assets, with source root motion.

These are authoring-tool recordings. Their root trajectories are never sent to
D2 and their native cloth simulation is not reproduced in this preview.
"""
import bpy,sys,json,struct,math,subprocess,time,argparse,hashlib
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
P=Path(__file__).resolve().parent
parser=argparse.ArgumentParser();parser.add_argument('--only',nargs='*');parser.add_argument('--limit',type=int,default=0);parser.add_argument('--manifest',default='library-retarget-manifest.json');parser.add_argument('--folder',default='library-videos-v3');parser.add_argument('--high-detail-emily',action='store_true')
args=parser.parse_args(sys.argv[sys.argv.index('--')+1:] if '--' in sys.argv else [])
if args.high_detail_emily:MODEL_OVERRIDES={'emily-body':P/'item-models/emily-body-high.json'}
sys.path.insert(0,str(P))
from native_prop_preview import NativePropPreview
from render_identity import fingerprint
src=(P/'preview_natural_walk.py').read_text()
exec(compile(src.split("for name in ('idle'")[0],str(P/'preview_natural_walk.py'),'exec'),globals())
OUT=P/args.folder;OUT.mkdir(exist_ok=True)
scene.render.resolution_x=1152;scene.render.resolution_y=720;scene.render.fps=30
scene.display.shading.color_type='MATERIAL';scene.display.shading.show_shadows=True
camera.data.ortho_scale=4.25
for role,rig,J,B,sword in rigs:rig.location=(0,-.95 if role=='corvo' else .95,0)
# A fixed floor grid makes trajectory and contact visible in a following view.
verts=[];faces=[]
for x in range(-12,13):
 for y in range(-12,13):
  at=len(verts);verts.extend([(x,y,-.02),(x+1,y,-.02),(x+1,y+1,-.02),(x,y+1,-.02)]);faces.append((at,at+1,at+2,at+3))
grid=bpy.data.meshes.new('contact_grid');grid.from_pydata(verts,[],faces);obj=bpy.data.objects.new('contact_grid',grid);scene.collection.objects.link(obj)
for i in range(2):
 mat=bpy.data.materials.new('floor'+str(i));v=.17 if i else .21;mat.diffuse_color=(v,v,v,1);grid.materials.append(mat)
for i,face in enumerate(grid.polygons):face.material_index=(i//25+i%25)%2
groups={}
for m in read(P/args.manifest):
 groups.setdefault(m['selection']['alias'],{})[m['role']]=m
def sample(m,t):
 f=min(m['frames']-1,max(0,t*m['fps']));a=int(f);b=min(a+1,m['frames']-1);u=f-a
 out=[]
 for va,vb in zip(m['_frames'][a],m['_frames'][b]):
  qa=Quaternion((va[3],*va[:3]));qb=Quaternion((vb[3],*vb[:3]));q=qa.slerp(qb,u)
  v=[q.x,q.y,q.z,q.w,*[va[k]+(vb[k]-va[k])*u for k in range(4,7)]];out.append(matrix(v))
 ra,rb=m['root_track'][a],m['root_track'][b];rq=Quaternion((ra[3],*ra[:3])).slerp(Quaternion((rb[3],*rb[:3])),u)
 root=rq.to_matrix().to_4x4();root.translation=Vector(ra[4:7]).lerp(Vector(rb[4:7]),u)
 return out,matrix(m.get('preview_placement',[0,0,0,1,0,0,0]))@m['_root0'].inverted()@root
records=[];started=time.time();props={}
for index,(label,pair) in enumerate(groups.items()):
 if args.only and label not in args.only:continue
 if args.limit and len(records)>=args.limit:break
 out=OUT/(label+'.mp4');meta=OUT/(label+'.json')
 identity=hashlib.sha256(''.join(pair[r]['sha256'] for r in ('corvo','emily')).encode()).hexdigest()
 provenance=fingerprint(pair,globals().get('MODEL_OVERRIDES',{}))
 if out.exists() and meta.exists():
  prior=read(meta)
  if prior['identity']!=identity:raise RuntimeError('Changed source needs new output folder')
  if prior.get('render_identity')!=provenance['render_identity']:raise RuntimeError('Changed or unpinned render inputs need a new output folder')
  records.append(prior);continue
 for m in pair.values():
  b=(P/m['binary']).read_bytes();n=m['joints']
  m['_frames']=[[struct.unpack_from('<7f',b,28+(f*n+j)*28) for j in range(n)] for f in range(m['frames'])]
  m['_root0']=matrix(m['root_track'][0])
 for prop in props.values():prop.hide()
 for role,m in pair.items():
  for definition in m.get('props',[]):
   key=role+'-'+definition['kind']
   if key not in props:props[key]=NativePropPreview(key,definition)
 duration=max(m['duration'] for m in pair.values());total=round(duration*30)+1
 # Frame the whole authored body, including elevated hands and feet. A
 # standing-height camera clipped heads in the initial ladder pilot.
 view=Vector(pair['corvo'].get('preview_view',(5.5,-7,3.7)));forward=-view.normalized();right=forward.cross(Vector((0,0,1))).normalized();up=right.cross(forward).normalized()
 projected=[]
 for f in range(total):
  at=[]
  for role,rig,J,B,sword in rigs:
   local,root=sample(pair[role],f/30);at.append((role,J,local,root))
  meanroot=(at[0][3].translation+at[1][3].translation)*.5
  for role,J,local,root in at:
   g=[];offset=Vector((0,-.95 if role=='corvo' else .95,0))
   for i in range(27):
    p=J[i]['parent'];g.append(local[i] if p<0 else g[p]@local[i])
    if i in (2,7,9,12,14,16,19,21,22,23,25,26):
     point=root@g[i].translation+offset-meanroot;projected.append((point.dot(right),point.dot(up)))
   for prop in pair[role].get('props',[]):
    at=min(len(prop['attachment_track'])-1,round(f/30*pair[role]['fps']))
    if not prop['visible'][at]:continue
    transform=root@g[prop['parent_joint']]@matrix(prop['attachment_track'][at]);point=transform.translation+offset-meanroot
    # Include released objects and interaction targets in the recording,
    # with a conservative margin for the small contact-review fixtures.
    for dx,dy in [(-.3,-.3),(-.3,.3),(.3,-.3),(.3,.3)]:projected.append((point.dot(right)+dx,point.dot(up)+dy))
 minx=min(v[0]for v in projected);maxx=max(v[0]for v in projected);miny=min(v[1]for v in projected);maxy=max(v[1]for v in projected)
 camera.data.ortho_scale=max(4.25,(maxx-minx+.8),(maxy-miny+.8)*scene.render.resolution_x/scene.render.resolution_y)
 # Blender orthographic scale spans the image WIDTH for a landscape frame.
 framing=right*((minx+maxx)*.5)+up*((miny+maxy)*.5)
 caption=label+'  |  '+('CORVO / EMILY'if view.x>=0 else'EMILY / CORVO')
 vf="drawtext=fontfile='C\\:/Windows/Fonts/arial.ttf':text='"+caption+"':fontcolor=white:fontsize=22:x=20:y=20,drawtext=fontfile='C\\:/Windows/Fonts/arial.ttf':text='AUTHORING PREVIEW - gameplay binding pending':fontcolor=white:fontsize=16:x=20:y=h-28"
 temp=OUT/(label+'.partial.mp4');log=(OUT/(label+'.encode.log')).open('wb')
 process=subprocess.Popen(['ffmpeg','-hide_banner','-loglevel','error','-y','-f','image2pipe','-vcodec','png','-framerate','30','-i','pipe:0','-vf',vf,'-c:v','libx264','-preset','fast','-crf','21','-pix_fmt','yuv420p','-movflags','+faststart',str(temp)],stdin=subprocess.PIPE,stderr=log)
 clipbegan=time.time()
 try:
  for f in range(total):
   center=Vector((0,0,0))
   for role,rig,J,B,sword in rigs:
    data=pair[role];local,root=sample(data,f/30);offset=Matrix.Translation((0,-.95 if role=='corvo' else .95,0));rig.matrix_world=offset@root;g=[]
    for i,j in enumerate(J):
     rig.pose.bones[j['nativeName']].matrix_basis=B[i].inverted()@local[i];p=j['parent'];g.append(local[i] if p<0 else g[p]@local[i])
    q=read(P.parent/'fullbody-proof/inputs/client-target-sword.json' if role=='corvo' else P.parent/'crouch-grip-reference/host-target-sword.json')['quaternion']
    sword.matrix_world=rig.matrix_world@g[17]@Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix().to_4x4()
    sword.hide_render=label.startswith('empty_') or data.get('equipment_policy')=='hands_free'
    for definition in data.get('props',[]):props[role+'-'+definition['kind']].apply(rig.matrix_world@g[definition['parent_joint']],definition,f/30,data['fps'])
    center+=rig.matrix_world.translation*.5
   target=center+framing;camera.location=target+view;camera.rotation_euler=(target-camera.location).to_track_quat('-Z','Y').to_euler()
   bpy.context.view_layer.update();bpy.ops.render.render()
   # Read a saved render because Blender's Render Result does not expose a
   # stable headless pixel buffer across render engines.
   tmp=OUT/'frame.png';bpy.data.images['Render Result'].save_render(str(tmp),scene=scene)
   process.stdin.write(tmp.read_bytes())
  process.stdin.close();code=process.wait(timeout=60)
  if code:raise RuntimeError('ffmpeg failed: '+str(code))
  temp.replace(out)
 finally:
  log.close()
  if process.poll() is None:process.kill();process.wait()
 result=dict(alias=label,family=pair['corvo']['selection']['family'],identity=identity,video=str(out.relative_to(P)),frames=total,fps=30,duration=total/30,render_seconds=time.time()-clipbegan,roles={r:pair[r]['id'] for r in pair},source='Blender weighted native model preview',root='Source trajectory; authoring preview only; see per-clip root_track_semantics',cloth='Emily LOD3 preview; runtime cloth/correctives checked separately',equipment=pair['corvo'].get('equipment_policy','native_action'),status='authored_candidate_recorded',visual_accepted=False)
 result['model_variant']='High-detail Emily body without simulated coat panels'if args.high_detail_emily else'Original review bodies; Emily LOD3'
 if args.high_detail_emily:result['cloth']='High-detail native Emily body without simulated coat panels. Exact clothing and native correctives require D2 validation.'
 result['model_sha256']={k:hashlib.sha256(Path(v).read_bytes()).hexdigest()for k,v in globals().get('MODEL_OVERRIDES',{}).items()}
 result['native_props']=bool(pair['corvo'].get('props'))
 result.update(provenance)
 meta.write_text(json.dumps(result,indent=2));records.append(result)
 for m in pair.values():del m['_frames'];del m['_root0']
 print('RECORDED',label,total,'frames',round(result['render_seconds'],2),'seconds',flush=True)
(OUT/'manifest.json').write_text(json.dumps(records,indent=2));print('RECORDINGS_COMPLETE',len(records),round(time.time()-started,1),flush=True)
