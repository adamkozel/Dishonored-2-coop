"""Pin UI-downloaded full-body sources. No game assets were uploaded."""
import json,hashlib,shutil
from pathlib import Path
P=Path(__file__).resolve().parent;D=Path.home()/'Downloads';O=P/'mixamo-dynamics';O.mkdir(exist_ok=True)
clips=[
 ('Running Slide','Running To Slide And Back To Running',47,None,'slide','traversal'),
 ('Two Cycle Sprint','Full Speed Sprint For Two Cycles',33,True,'sprint','locomotion'),
 ('Running Jump','Jumping From A Sprint',28,None,'sprint','traversal'),
 ('Crouch Walk Back','Walking Backwards While Crouched',38,True,'crouch','stance'),
 ('Crouch Walk Left','Walking Left While Crouched',36,True,'crouch','stance'),
 ('Crouch Walk Right','Walking Right While Crouched',39,True,'crouch','stance')]
rows=[]
for title,description,frames,in_place,query,family in clips:
 src=D/(title+'.fbx');dst=O/src.name;digest=hashlib.sha256(src.read_bytes()).hexdigest()
 if dst.exists():assert hashlib.sha256(dst.read_bytes()).hexdigest()==digest
 else:shutil.copy2(src,dst)
 rows.append(dict(title=title,description=description,file=dst.name,sha256=digest,size=dst.stat().st_size,source='https://www.mixamo.com/#/?page=1&query='+query+'&type=Motion%2CMotionPack',character='X Bot',overdrive=50,arm_space=50,mirror=False,in_place=in_place,trim=[0,100],ui_total_frames=frames,format='FBX Binary',skin='Without Skin',requested_fps=60,keyframe_reduction='none',family=family))
(O/'source-manifest.json').write_text(json.dumps(dict(scope='Authored dynamic full-body donors; no direct FBX game loading, root motion remains offline metadata',files=rows),indent=2));print('PINNED',len(rows))
