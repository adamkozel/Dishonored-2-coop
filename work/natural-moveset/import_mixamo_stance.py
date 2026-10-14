import json,hashlib,shutil
from pathlib import Path
P=Path(__file__).resolve().parent;D=Path.home()/'Downloads';O=P/'mixamo-stance';O.mkdir(exist_ok=True)
clips=[('Standing To Crouched','Standing Adventure Idle To Crouched',20),('Crouched To Standing','Crouch To Stand Transition',20),('Crouching Idle','Low Crouching Idle',76)]
rows=[]
for title,description,frames in clips:
 src=D/(title+'.fbx');dst=O/src.name;digest=hashlib.sha256(src.read_bytes()).hexdigest()
 if dst.exists():assert hashlib.sha256(dst.read_bytes()).hexdigest()==digest
 else:shutil.copy2(src,dst)
 rows.append(dict(title=title,description=description,file=dst.name,sha256=digest,size=dst.stat().st_size,source='https://www.mixamo.com/#/?page=1&query=crouch&type=Motion%2CMotionPack',character='X Bot',overdrive=50,arm_space=50,mirror=False,in_place=None,trim=[0,100],ui_total_frames=frames,format='FBX Binary',skin='Without Skin',requested_fps=60,keyframe_reduction='none'))
(O/'source-manifest.json').write_text(json.dumps(dict(scope='Full-body stance donors, before contact/weapon adaptation',files=rows),indent=2));print('PINNED_STANCE',len(rows))
