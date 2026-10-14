"""Pin explicit UI-downloaded traversal donors; no game assets are uploaded."""
import json,hashlib,shutil
from pathlib import Path
P=Path(__file__).resolve().parent;D=Path.home()/'Downloads';O=P/'mixamo-traversal';O.mkdir(exist_ok=True)
clips=[('Climbing Ladder','Climbing Up A Ladder',24,True),('Start Climbing Ladder','Standing To Climb Ladder Quickly',62,None),('Climbing To Top','Climbing Up A Ladder To Standing',121,None),('Climbing A Rope','Climbing A Rope With Hands And Feet',73,True),('Freehang Climb','Freehang Climb To Standing',117,None)]
records=[]
for title,description,frames,inplace in clips:
 src=D/(title+'.fbx');assert src.is_file();dst=O/src.name
 digest=hashlib.sha256(src.read_bytes()).hexdigest()
 if dst.exists():assert hashlib.sha256(dst.read_bytes()).hexdigest()==digest
 else:shutil.copy2(src,dst)
 records.append(dict(title=title,description=description,file=dst.name,sha256=digest,size=dst.stat().st_size,source='https://www.mixamo.com/#/?page=1&query=climbing&type=Motion%2CMotionPack',character='X Bot',overdrive=50,arm_space=50,mirror=False,in_place=inplace,in_place_note='Control unavailable' if inplace is None else 'Enabled',trim=[0,100],ui_total_frames=frames,format='FBX Binary',skin='Without Skin',requested_fps=60,keyframe_reduction='none'))
(O/'source-manifest.json').write_text(json.dumps(dict(scope='Authored traversal donors, not accepted game animations',files=records),indent=2));print('PINNED_TRAVERSAL',len(records))
