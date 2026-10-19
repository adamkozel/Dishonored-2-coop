"""1409 shop gesture check: renders the retargeted NPC conversation gestures (think, arms-crossed think, open-hand think,
hands front, calm) on both partners at four phases; review only. Generated from preview_knockdown_v1400.py."""
import bpy,json,sys,struct,math
from pathlib import Path
from mathutils import Matrix,Quaternion,Vector
P=Path(__file__).resolve().parent
# Reuse only the self-contained scene construction, not earlier pose authoring.
src=(P/'preview_natural_walk.py').read_text()
exec(compile(src.split("for name in ('idle'")[0],str(P/'preview_natural_walk.py'),'exec'),globals())
OUT=P/'native-previews';OUT.mkdir(exist_ok=True)
WANT={'12463753d468616f','5053a8e9d7825de3','50c924a9aaf4a108','95da6eb1bbb39a5b','9ca9b6ae428c12fa','b32c72dc44654225','bc2d61264df580fa','c5d01bde2bca00c8','f235eb477f8698db','f4380ca901053f24'}
manifest=[m for name in ('native-retarget-manifest-gestures-v1409.json',) for m in read(P/name) if m['source']['id'] in WANT]
for m in manifest:
    b=(P/m['binary']).read_bytes();n=m['joints'];m['_frames']=[[struct.unpack_from('<7f',b,28+(f*n+j)*28) for j in range(n)] for f in range(m['frames'])]
groups={}
for m in manifest:groups.setdefault(m['source']['id'],{})[m['role']]=m
for key,pair in groups.items():
    label=pair['corvo']['source']['aliases'][0]
    for u in (0,.33,.66,1):
        for role,rig,J,B,sword in rigs:
            data=pair[role];frame=min(data['frames']-1,round(u*(data['frames']-1)));local=[matrix(v) for v in data['_frames'][frame]];g=[]
            for i,j in enumerate(J):
                rig.pose.bones[j['nativeName']].matrix_basis=B[i].inverted()@local[i];p=j['parent'];g.append(local[i] if p<0 else g[p]@local[i])
            q=read(P.parent/'fullbody-proof/inputs/client-target-sword.json' if role=='corvo' else P.parent/'crouch-grip-reference/host-target-sword.json')['quaternion']
            binding=Quaternion((q[3],-q[0],-q[1],-q[2])).normalized().to_matrix().to_4x4()
            sword.matrix_world=Matrix.Translation(rig.location)@g[17]@binding
        bpy.context.view_layer.update();scene.render.filepath=str(OUT/f'gs1409-{key[:6]}-{label}-{int(u*100):03}.png');bpy.ops.render.render(write_still=True)

