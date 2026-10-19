"""1400 knockdown composition check: renders the takedown knockdown falls and the ground recoveries (preview_native.py,
filtered); review only."""
import bpy,json,sys,struct,math
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
P=Path(__file__).resolve().parent
# Reuse only the self-contained scene construction, not earlier pose authoring.
src=(P/'preview_natural_walk.py').read_text()
exec(compile(src.split("for name in ('idle'")[0],str(P/'preview_natural_walk.py'),'exec'),globals())
OUT=P/'native-previews';OUT.mkdir(exist_ok=True)
WANT={'74db9323b65a2b4f','283f7b3a04d9828b','5872edfd4c3e0454','87033099b70826fe','ff89071dc7623afe','da84d9222118fcab'}
manifest=[m for name in ('native-retarget-manifest-knockdown-v1362.json','native-retarget-manifest-knockdown-v1374.json') for m in read(P/name) if m['source']['id'] in WANT]
for m in manifest:
    b=(P/m['binary']).read_bytes();n=m['joints'];m['_frames']=[[struct.unpack_from('<7f',b,28+(f*n+j)*28) for j in range(n)] for f in range(m['frames'])]
groups={}
for m in manifest:groups.setdefault(m['source']['id'],{})[m['role']]=m
for key,pair in groups.items():
    label=pair['corvo']['source']['aliases'][0]
    for u in (0,.5,1):
        for role,rig,J,B,sword in rigs:
            data=pair[role];frame=min(data['frames']-1,round(u*(data['frames']-1)));local=[matrix(v) for v in data['_frames'][frame]];g=[]
            for i,j in enumerate(J):
                rig.pose.bones[j['nativeName']].matrix_basis=B[i].inverted()@local[i];p=j['parent'];g.append(local[i] if p<0 else g[p]@local[i])
            q=read(P.parent/'fullbody-proof/inputs/client-target-sword.json' if role=='corvo' else P.parent/'crouch-grip-reference/host-target-sword.json')['quaternion']
            binding=Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix().to_4x4()
            sword.matrix_world=Matrix.Translation(rig.location)@g[17]@binding
        bpy.context.view_layer.update();scene.render.filepath=str(OUT/f'kd1400-{key[:6]}-{label}-{int(u*100):03}.png');bpy.ops.render.render(write_still=True)
bpy.ops.wm.save_as_mainfile(filepath=str(P/'native-source-preview.blend'))
