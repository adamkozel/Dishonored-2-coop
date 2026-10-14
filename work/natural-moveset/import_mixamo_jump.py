import json,hashlib,shutil
from pathlib import Path
P=Path(__file__).resolve().parent;out=P/'mixamo-jump';out.mkdir(exist_ok=True)
src=Path.home()/'Downloads/Standing Jump.fbx';dst=out/src.name;sha=hashlib.sha256(src.read_bytes()).hexdigest()
if dst.exists():assert hashlib.sha256(dst.read_bytes()).hexdigest()==sha
else:shutil.copy2(src,dst)
record=dict(title='Standing Jump',description='Jumping And Landing In Place',file=dst.name,sha256=sha,size=dst.stat().st_size,source='https://www.mixamo.com/#/?page=1&query=jump&type=Motion%2CMotionPack',character='X Bot',overdrive=50,arm_space=50,mirror=False,in_place=None,in_place_note='No In Place control for this clip; world trajectory is extracted offline and never drives the network actor.',trim=[0,100],ui_total_frames=72,format='FBX Binary',skin='Without Skin',requested_fps=60,keyframe_reduction='none',family='traversal')
(out/'source-manifest.json').write_text(json.dumps(dict(files=[record]),indent=2));print('PINNED',sha,dst.stat().st_size)
