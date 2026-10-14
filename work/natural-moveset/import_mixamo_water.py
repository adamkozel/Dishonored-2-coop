import json,hashlib,shutil
from pathlib import Path
P=Path(__file__).resolve().parent;D=Path.home()/'Downloads';O=P/'mixamo-water';O.mkdir(exist_ok=True)
clips=[('Swimming','Swimming Underwater',137,True,dict(intensity=50,reach=50)),('Treading Water','Floating',91,None,dict(style=100,range=100)),('Swimming To Edge','Swimming Breastroke To Edge Of Pool',151,None,dict(intensity=50))]
rows=[]
for title,description,frames,inplace,controls in clips:
 src=D/(title+'.fbx');dst=O/src.name;digest=hashlib.sha256(src.read_bytes()).hexdigest()
 if dst.exists():assert hashlib.sha256(dst.read_bytes()).hexdigest()==digest
 else:shutil.copy2(src,dst)
 rows.append(dict(title=title,description=description,file=dst.name,sha256=digest,size=dst.stat().st_size,source='https://www.mixamo.com/#/?page=1&query=swim&type=Motion%2CMotionPack',character='X Bot',controls=controls,overdrive=50,arm_space=50,mirror=False,in_place=inplace,trim=[0,100],ui_total_frames=frames,format='FBX Binary',skin='Without Skin',requested_fps=60,keyframe_reduction='none'))
(O/'source-manifest.json').write_text(json.dumps(dict(scope='Water movement donors; not accepted game animations',files=rows),indent=2));print('PINNED_WATER',len(rows))
